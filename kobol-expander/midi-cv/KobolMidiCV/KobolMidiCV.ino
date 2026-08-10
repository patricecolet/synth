// KobolMidiCV — interface MIDI vers CV pour RSF Kobol Expander
//
// Cible  : ESP32-S3 VROOM N16R8
// Carte  : ../MIDI_MAP.md
// Réglage: ../CALIBRATION.md  <- à faire avant toute utilisation musicale
//
// Reprend v1-first-release/kobolDAC (Teensy 2.0 + MCP4822), en changeant
// de MCU et en passant de 4 à 12 paramètres. La v1 reste la référence
// fonctionnelle et n'est pas modifiée.
//
// Dépendances (gestionnaire de bibliothèques Arduino) :
//   - Adafruit TinyUSB Library      -> USB MIDI natif sur ESP32-S3
//   - MIDI Library (FortySevenEffects)
// Dans l'IDE : Outils > USB Mode > "USB-OTG (TinyUSB)"

#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

#include "config.h"
#include "output.h"

Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

// ─────────────────────────────────────────────────────────────────────
// État
// ─────────────────────────────────────────────────────────────────────

static uint8_t cc_value[PARAM_COUNT];   // dernier CC reçu par paramètre

// Pile de notes : priorité à la dernière jouée, retour à la précédente au
// relâchement. La v1 ne gardait qu'une note et perdait le legato.
static const uint8_t NOTE_STACK_MAX = 8;
static uint8_t note_stack[NOTE_STACK_MAX];
static uint8_t note_count = 0;

static bool    gate_from_note  = false;
static bool    gate_forced     = false;

static float   pitch_current_mv = CAL_NEUTRAL_MV;  // suit le portamento
static float   pitch_target_mv  = CAL_NEUTRAL_MV;
static uint16_t portamento_ms   = 0;
static bool    portamento_on    = true;
static uint32_t last_update_us  = 0;

static float   bend_semitones   = 0.0f;
static uint8_t last_velocity    = 100;

// Profondeurs de modulation, 0-127
static uint8_t vel_to_cut_on  = 0;
static uint8_t vel_to_cut_off = 0;
static uint8_t vel_to_vca     = 0;

// ─────────────────────────────────────────────────────────────────────
// Utilitaires
// ─────────────────────────────────────────────────────────────────────

// Déclaré ici : onNoteOn renvoie vers onNoteOff pour le cas vélocité 0.
// L'IDE Arduino génère les prototypes automatiquement, mais pas pour les
// fonctions `static` — sans cette ligne, la compilation échoue.
static void onNoteOff(byte channel, byte note, byte velocity);

static int8_t paramIndexForCC(uint8_t cc) {
  for (uint8_t i = 0; i < PARAM_COUNT; i++) {
    if (PARAMS[i].cc == cc) return (int8_t)i;
  }
  return -1;
}

// CC 0-127 -> tension au connecteur, interpolée sur la plage mesurée.
// v_min_mv peut être supérieur à v_max_mv : plusieurs pins du Kobol sont
// inversées (VCF Attack va de -660 à -1150 mV).
static int32_t ccToMv(const KobolParam& p, int32_t cc) {
  if (cc < 0)   cc = 0;
  if (cc > 127) cc = 127;
  return (int32_t)p.v_min_mv +
         (((int32_t)p.v_max_mv - (int32_t)p.v_min_mv) * cc) / 127;
}

// Applique un paramètre en tenant compte des modulations qui le visent.
static void applyParam(uint8_t i) {
  if (!outputIsLive(i)) return;
  const KobolParam& p = PARAMS[i];

  int32_t cc = cc_value[i];

  // Vélocité -> cutoff : profondeur différente à l'attaque et au relâché,
  // comme en v1 (CC 14/15, renumérotés 114/115).
  if (p.cc == 74) {
    const uint8_t depth = gate_from_note ? vel_to_cut_on : vel_to_cut_off;
    cc += ((int32_t)last_velocity * depth) / 127;
  }
  // Vélocité -> VCA sustain
  if (p.cc == 107 && vel_to_vca) {
    cc = (cc * (127 - vel_to_vca)) / 127 +
         ((int32_t)last_velocity * vel_to_vca) / 127;
  }

  outputWriteMv(i, ccToMv(p, cc));
}

static void applyAllParams() {
  for (uint8_t i = 0; i < PARAM_COUNT; i++) applyParam(i);
}

// Note MIDI -> tension pin 11. Réponse exponentielle du VCO : une tension
// proportionnelle au numéro de note donne des demi-tons réguliers.
static float noteToMv(float note) {
  return (float)CAL_NEUTRAL_MV +
         (note - (float)CAL_NEUTRAL_NOTE) * (cal_mv_per_octave / 12.0f);
}

static void updatePitchTarget() {
  if (note_count == 0) return;
  const uint8_t note = note_stack[note_count - 1];
  pitch_target_mv = noteToMv((float)note + bend_semitones);
}

static void setGate(bool on) {
  digitalWrite(GPIO_GATE, (on || gate_forced) ? HIGH : LOW);
}

// ─────────────────────────────────────────────────────────────────────
// Pile de notes
// ─────────────────────────────────────────────────────────────────────

static void pushNote(uint8_t note) {
  for (uint8_t i = 0; i < note_count; i++) {
    if (note_stack[i] == note) return;         // déjà présente
  }
  if (note_count < NOTE_STACK_MAX) {
    note_stack[note_count++] = note;
  } else {
    // Pile pleine : on écarte la plus ancienne.
    for (uint8_t i = 1; i < NOTE_STACK_MAX; i++) note_stack[i - 1] = note_stack[i];
    note_stack[NOTE_STACK_MAX - 1] = note;
  }
}

static void removeNote(uint8_t note) {
  for (uint8_t i = 0; i < note_count; i++) {
    if (note_stack[i] == note) {
      for (uint8_t j = i + 1; j < note_count; j++) note_stack[j - 1] = note_stack[j];
      note_count--;
      return;
    }
  }
}

// ─────────────────────────────────────────────────────────────────────
// Réception MIDI
// ─────────────────────────────────────────────────────────────────────

static void onNoteOn(byte channel, byte note, byte velocity) {
  (void)channel;
  if (velocity == 0) { onNoteOff(channel, note, 0); return; }

  last_velocity = velocity;
  const bool was_silent = (note_count == 0);

  pushNote(note);
  updatePitchTarget();

  // Première note d'une phrase : pas de glide depuis le néant.
  if (was_silent && !portamento_on) pitch_current_mv = pitch_target_mv;
  if (was_silent && portamento_ms == 0) pitch_current_mv = pitch_target_mv;

  gate_from_note = true;
  setGate(true);
  applyAllParams();
}

static void onNoteOff(byte channel, byte note, byte velocity) {
  (void)channel;
  if (velocity) last_velocity = velocity;

  removeNote(note);

  if (note_count == 0) {
    gate_from_note = false;
    setGate(false);
  } else {
    updatePitchTarget();   // legato : retour à la note encore tenue
  }
  applyAllParams();
}

static void onControlChange(byte channel, byte control, byte value) {
  (void)channel;

  const int8_t idx = paramIndexForCC(control);
  if (idx >= 0) {
    cc_value[idx] = value;
    applyParam((uint8_t)idx);
    return;
  }

  switch (control) {
    case CC_PORTAMENTO_MS:
      portamento_ms = (uint16_t)value * 10;      // 0 -> 1270 ms
      break;
    case CC_PORTAMENTO_SW:
      portamento_on = (value >= 64);
      break;
    case CC_GATE_FORCE:
      gate_forced = (value >= 64);
      setGate(gate_from_note);
      break;
    case CC_VEL_TO_CUT_ON:
      vel_to_cut_on = value;
      break;
    case CC_VEL_TO_CUT_OFF:
      vel_to_cut_off = value;
      break;
    case CC_VEL_TO_VCA:
      vel_to_vca = value;
      applyAllParams();
      break;
    case CC_MODWHEEL:
      // Réservé : profondeur du LFO logiciel, pas encore implémenté.
      break;
    default:
      break;
  }
}

static void onPitchBend(byte channel, int bend) {
  (void)channel;
  bend_semitones = ((float)bend / 8192.0f) * PITCH_BEND_SEMITONES;
  updatePitchTarget();
}

// ─────────────────────────────────────────────────────────────────────
// Portamento
// ─────────────────────────────────────────────────────────────────────

static void updatePortamento() {
  const uint32_t now = micros();
  const uint32_t dt  = now - last_update_us;
  if (dt < 500) return;                 // pas plus de 2 kHz
  last_update_us = now;

  if (!portamento_on || portamento_ms == 0) {
    pitch_current_mv = pitch_target_mv;
  } else {
    // Progression linéaire : la plage complète serait franchie en
    // portamento_ms. Une note voisine glisse donc plus vite qu'un grand
    // intervalle, ce qui est le comportement attendu d'un glide analogique.
    const float span     = (float)(PITCH_MAX_MV - PITCH_MIN_MV);
    const float step     = span * ((float)dt / 1000.0f) / (float)portamento_ms;
    const float distance = pitch_target_mv - pitch_current_mv;

    if (fabsf(distance) <= step) pitch_current_mv = pitch_target_mv;
    else                         pitch_current_mv += (distance > 0 ? step : -step);
  }

  outputWritePitchMv((int32_t)lroundf(pitch_current_mv));
}

// ─────────────────────────────────────────────────────────────────────

void setup() {
  Serial.begin(115200);

  MIDI.setHandleNoteOn(onNoteOn);
  MIDI.setHandleNoteOff(onNoteOff);
  MIDI.setHandleControlChange(onControlChange);
  MIDI.setHandlePitchBend(onPitchBend);

  usb_midi.setStringDescriptor("Kobol MIDI/CV");
  MIDI.begin(MIDI_CHANNEL_OMNI);

  // TinyUSB doit être démarré avant tout envoi ; sur ESP32-S3 le port
  // série USB met un instant à s'ouvrir, d'où l'attente bornée.
  const uint32_t t0 = millis();
  while (!TinyUSBDevice.mounted() && (millis() - t0) < 2000) delay(10);

  Serial.println(F("\n=== Kobol MIDI/CV ==="));
  const uint8_t live = outputBegin();
  Serial.printf("Sorties actives : %u\n", live);
  Serial.printf("Calibration pitch : %.1f mV/octave (A VERIFIER)\n", cal_mv_per_octave);

  for (uint8_t i = 0; i < PARAM_COUNT; i++) cc_value[i] = PARAMS[i].cc_default;
  applyAllParams();

  last_update_us = micros();
}

void loop() {
  MIDI.read();
  updatePortamento();
}

// KobolMidiCV — interface MIDI vers CV pour RSF Kobol Expander
//
// Cible  : Teensy 2.0 (ATmega32U4) + MCP4822
// Carte  : ../MIDI_MAP.md
// Réglage: ../CALIBRATION.md  <- à faire avant toute utilisation musicale
//
// Reprend v1-first-release/kobolDAC, même câblage, en corrigeant l'échelle
// du pitch et en ajoutant pile de notes, pitch bend et portamento
// commutable. La v1 reste la référence et n'est pas modifiée.
//
// Dans l'IDE : Outils > USB Type > "MIDI". Aucune bibliothèque externe :
// usbMIDI et SPI viennent de Teensyduino.
//
// Tout est en entier. L'ATmega32U4 n'a pas de FPU : chaque opération
// flottante coûte une dizaine de microsecondes, et le portamento tourne
// à 1 kHz.

#include <SPI.h>
#include "config.h"
#include "output.h"

// ─────────────────────────────────────────────────────────────────────
// État
// ─────────────────────────────────────────────────────────────────────

static uint8_t cc_value[PARAM_COUNT];

// Pile de notes : priorité à la dernière jouée, retour à la précédente au
// relâchement. La v1 ne gardait qu'une note et perdait le legato.
static const uint8_t NOTE_STACK_MAX = 6;
static uint8_t note_stack[NOTE_STACK_MAX];
static uint8_t note_count = 0;

static bool gate_from_note = false;
static bool gate_forced    = false;

// Pitch en millivolts × 16 : la fraction sert au portamento, qui doit
// avancer par pas plus fins qu'un millivolt sans recourir au flottant.
static const uint8_t PITCH_FRAC = 4;              // décalage, 2^4 = 16
static int32_t pitch_current_q = 0;
static int32_t pitch_target_q  = 0;

// Portamento à zéro au démarrage : les notes sautent. Il ne s'active que
// si un CC 5 non nul arrive.
static uint16_t portamento_ms = 0;
static bool     portamento_on = true;
static uint32_t last_update_ms = 0;

static int16_t bend_cents    = 0;
static uint8_t last_velocity = 100;

static uint8_t vel_to_cut_on  = 0;
static uint8_t vel_to_cut_off = 0;
static uint8_t vel_to_vca     = 0;

// Déclaré ici : onNoteOn renvoie vers onNoteOff pour le cas vélocité 0.
static void onNoteOff(byte channel, byte note, byte velocity);

// ─────────────────────────────────────────────────────────────────────
// Conversions
// ─────────────────────────────────────────────────────────────────────

static int8_t paramIndexForCC(uint8_t cc) {
  for (uint8_t i = 0; i < PARAM_COUNT; i++) {
    if (PARAMS[i].cc == cc) return (int8_t)i;
  }
  return -1;
}

// CC 0-127 -> tension. v_min_mv peut dépasser v_max_mv : plusieurs pins du
// Kobol sont inversées (VCF Attack va de -660 à -1150 mV).
static int32_t ccToMv(const KobolParam& p, int32_t cc) {
  if (cc < 0)   cc = 0;
  if (cc > 127) cc = 127;

#if KOBOL_OUTPUT_JACK
  // En sortie jack, pas de plage mesurée : on prend la pleine échelle du DAC.
  (void)p;
  return (cc * DAC_FULL_MV) / 127;
#else
  return (int32_t)p.v_min_mv +
         (((int32_t)p.v_max_mv - (int32_t)p.v_min_mv) * cc) / 127;
#endif
}

// Note MIDI -> millivolts. Réponse exponentielle du VCO : une tension
// proportionnelle au numéro de note donne des demi-tons réguliers.
// Le pitch bend est appliqué en centièmes de demi-ton.
static int32_t noteToMv(uint8_t note, int16_t cents) {
  const int32_t semitones_c = ((int32_t)note - (int32_t)CAL_BASE_NOTE) * 100
                            + (int32_t)cents;
  return (semitones_c * (int32_t)cal_mv_per_octave) / 1200;
}

// ─────────────────────────────────────────────────────────────────────
// Application des paramètres
// ─────────────────────────────────────────────────────────────────────

static void applyParam(uint8_t i) {
  if (!outputIsLive(i)) return;
  const KobolParam& p = PARAMS[i];

  int32_t cc = cc_value[i];

  // Vélocité -> cutoff, profondeur différente à l'attaque et au relâché
  // (CC 14/15 en v1, renumérotés 114/115).
  if (p.cc == 74) {
    const uint8_t depth = gate_from_note ? vel_to_cut_on : vel_to_cut_off;
    if (depth) cc += ((int32_t)last_velocity * depth) / 127;
  }
  if (p.cc == 107 && vel_to_vca) {
    cc = (cc * (127 - vel_to_vca)) / 127
       + ((int32_t)last_velocity * vel_to_vca) / 127;
  }

  outputWriteMv(i, ccToMv(p, cc));
}

static void applyAllParams() {
  for (uint8_t i = 0; i < PARAM_COUNT; i++) applyParam(i);
}

static void updatePitchTarget() {
  if (note_count == 0) return;
  const uint8_t note = note_stack[note_count - 1];
  pitch_target_q = (int32_t)noteToMv(note, bend_cents) << PITCH_FRAC;
}

static void setGate(bool on) {
  outputWriteGate(on || gate_forced);
}

// ─────────────────────────────────────────────────────────────────────
// Pile de notes
// ─────────────────────────────────────────────────────────────────────

static void pushNote(uint8_t note) {
  for (uint8_t i = 0; i < note_count; i++) {
    if (note_stack[i] == note) return;
  }
  if (note_count < NOTE_STACK_MAX) {
    note_stack[note_count++] = note;
  } else {
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
  if (was_silent || !portamento_on || portamento_ms == 0) {
    pitch_current_q = pitch_target_q;
    outputWritePitchMv(pitch_current_q >> PITCH_FRAC);
  }

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
    updatePitchTarget();       // legato : retour à la note encore tenue
  }
  applyAllParams();
}

static void onControlChange(byte channel, byte control, byte value) {
  (void)channel;

  const int8_t idx = paramIndexForCC(control);
  if (idx >= 0) {
    cc_value[idx] = value;
    outputMarkSeen((uint8_t)idx);   // à partir d'ici la carte prend la main
    applyParam((uint8_t)idx);
    return;
  }

  switch (control) {
    case CC_PORTAMENTO_MS:  portamento_ms = (uint16_t)value * 10; break;
    case CC_PORTAMENTO_SW:  portamento_on = (value >= 64);        break;
    case CC_LFO_RATE:       outputWriteLfoRate(value);            break;
    case CC_VEL_TO_CUT_ON:  vel_to_cut_on  = value;               break;
    case CC_VEL_TO_CUT_OFF: vel_to_cut_off = value;               break;
    case CC_VEL_TO_VCA:     vel_to_vca = value; applyAllParams(); break;
    case CC_GATE_FORCE:
      gate_forced = (value >= 64);
      setGate(gate_from_note);
      break;
    case CC_MODWHEEL:
      // Réservé : profondeur du LFO logiciel, pas encore implémenté.
      break;
    default: break;
  }
}

static void onPitchBend(byte channel, int bend) {
  (void)channel;
  // bend va de -8192 à +8191
  bend_cents = (int16_t)(((int32_t)bend * PITCH_BEND_CENTS_MAX) / 8192);
  updatePitchTarget();
}

// ─────────────────────────────────────────────────────────────────────
// Portamento
// ─────────────────────────────────────────────────────────────────────

static void updatePortamento() {
  const uint32_t now = millis();
  const uint32_t dt  = now - last_update_ms;
  if (dt == 0) return;
  last_update_ms = now;

  if (!portamento_on || portamento_ms == 0) {
    if (pitch_current_q != pitch_target_q) {
      pitch_current_q = pitch_target_q;
      outputWritePitchMv(pitch_current_q >> PITCH_FRAC);
    }
    return;
  }
  if (pitch_current_q == pitch_target_q) return;

  // Progression linéaire : la pleine échelle serait franchie en
  // portamento_ms. Un petit intervalle glisse donc plus vite qu'un grand,
  // ce qui est le comportement d'un glide analogique.
  const int32_t span = (int32_t)PITCH_MAX_MV << PITCH_FRAC;
  int32_t step = (int32_t)((span * (int32_t)dt) / (int32_t)portamento_ms);
  if (step < 1) step = 1;

  const int32_t distance = pitch_target_q - pitch_current_q;
  if (distance > -step && distance < step) pitch_current_q = pitch_target_q;
  else pitch_current_q += (distance > 0) ? step : -step;

  outputWritePitchMv(pitch_current_q >> PITCH_FRAC);
}

// ─────────────────────────────────────────────────────────────────────

void setup() {
  Serial.begin(115200);

  usbMIDI.setHandleNoteOn(onNoteOn);
  usbMIDI.setHandleNoteOff(onNoteOff);
  usbMIDI.setHandleControlChange(onControlChange);
  usbMIDI.setHandlePitchChange(onPitchBend);

  const uint8_t live = outputBegin();

  Serial.println(F("\n=== Kobol MIDI/CV (Teensy 2.0) ==="));
#if KOBOL_OUTPUT_JACK
  Serial.println(F("Sortie : jacks de facade, pleine echelle DAC 0-4096 mV"));
#else
  Serial.println(F("Sortie : connecteur P1 — ETAGE D'ADAPTATION REQUIS"));
#endif
  Serial.print(F("Parametres avec sortie : ")); Serial.println(live);
  Serial.print(F("Calibration pitch : ")); Serial.print(cal_mv_per_octave);
  Serial.println(F(" mV/octave (A VERIFIER, cf. CALIBRATION.md)"));
  outputReport();
  Serial.println(F("Sorties au repos jusqu'au premier CC — potards de facade actifs"));

  for (uint8_t i = 0; i < PARAM_COUNT; i++) cc_value[i] = 0;
  last_update_ms = millis();
}

void loop() {
  usbMIDI.read();
  updatePortamento();
}

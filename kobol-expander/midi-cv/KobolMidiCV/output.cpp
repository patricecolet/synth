#include "output.h"
#include <SPI.h>

// ─────────────────────────────────────────────────────────────────────
// Definitions uniques (declarees extern dans config.h)
// ─────────────────────────────────────────────────────────────────────

int16_t cal_mv_per_octave = 400;

const KobolParam PARAMS[] = {
  // CC   pin  dac            vmin   vmax   repos  def  état          nom
  {  74,  15,  DAC_CH_CUTOFF,    0,   900,      0,  90, PARAM_CHECK,  "VCF Cutoff"    },

  // Décrits, mais sans sortie tant qu'il n'y a qu'un MCP4822.
  {  71,  12,  DAC_CH_NONE,   -670,   610,    600,   0, PARAM_OK,     "VCF Resonance" },
  {  73,   8,  DAC_CH_NONE,   -660, -1150,   -520,   0, PARAM_CHECK,  "VCF Attack"    },
  {  75,   7,  DAC_CH_NONE,  -1350,  -360,  -1270,  40, PARAM_OK,     "VCF Decay"     },
  { 102,   4,  DAC_CH_NONE,      0,   920,    440,  90, PARAM_OK,     "VCF Sustain"   },
  { 103,  10,  DAC_CH_NONE,     50,   600,    590,  64, PARAM_OK,     "VCF ADS Ctrl"  },
  { 105,   5,  DAC_CH_NONE,   -660,  -280,   -500,   0, PARAM_OK,     "VCA Attack"    },
  { 106,   1,  DAC_CH_NONE,  -1410, -1170,    -90,  40, PARAM_OK,     "VCA Decay"     },
  { 107,  16,  DAC_CH_NONE,    -30,   940,    460, 100, PARAM_OK,     "VCA Sustain"   },
  { 109,   9,  DAC_CH_NONE,      0,   610,    600, 100, PARAM_OK,     "VCO2 Volume"   },
  { 108,  14,  DAC_CH_NONE,    130,   600,      0, 100, PARAM_CHECK,  "VCO1 Volume"   },

  // Bloqués — voir MIDI_MAP.md §6. Jamais émis.
  { 112,   3,  DAC_CH_NONE, 0, 0, 0, 0, PARAM_BLOCKED, "VCO1 Waveform" },
  { 113,   6,  DAC_CH_NONE, 0, 0, 0, 0, PARAM_BLOCKED, "VCO2 Waveform" },
};


static_assert(sizeof(PARAMS) / sizeof(PARAMS[0]) == PARAM_COUNT,
              "PARAM_COUNT (config.h) ne correspond plus au nombre de lignes de PARAMS[]");


// Écriture MCP4822.
//
// Trame 16 bits : bit 15 = canal (0 = A, 1 = B)
//                 bit 14 = indifférent
//                 bit 13 = GA  (0 = gain ×2, 1 = gain ×1)
//                 bit 12 = SHDN (1 = sortie active)
//                 bits 11..0 = valeur
//
// La v1 posait le gain sur le bit 14, qui est justement le bit
// indifférent : elle obtenait le gain ×2 par défaut, pas par la commande.
// Ici GA est écrit explicitement.
static void dacWrite(uint8_t channel, uint16_t code) {
  if (code > DAC_MAX_CODE) code = DAC_MAX_CODE;

  const uint16_t word = ((uint16_t)(channel & 1) << 15)
                      | (0u << 13)          // GA = 0 -> gain ×2, pleine échelle
                      | (1u << 12)          // sortie active
                      | (code & 0x0FFF);

  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  digitalWrite(PIN_DAC_CS, LOW);
  SPI.transfer((uint8_t)(word >> 8));
  SPI.transfer((uint8_t)(word & 0xFF));
  digitalWrite(PIN_DAC_CS, HIGH);
  SPI.endTransaction();
}

uint16_t mvToCode(int32_t mv) {
  if (mv <= 0) return 0;
  if (mv >= DAC_FULL_MV) return DAC_MAX_CODE;
  return (uint16_t)((mv * (int32_t)DAC_MAX_CODE) / DAC_FULL_MV);
}

uint8_t outputBegin() {
  pinMode(PIN_DAC_CS, OUTPUT);
  digitalWrite(PIN_DAC_CS, HIGH);
  SPI.begin();

  pinMode(PIN_GATE, OUTPUT);
  digitalWrite(PIN_GATE, LOW);

  pinMode(PIN_LFO_PWM, OUTPUT);
  analogWrite(PIN_LFO_PWM, 0);

  uint8_t live = 0;
  for (uint8_t i = 0; i < PARAM_COUNT; i++)
    if (PARAMS[i].dac_ch != DAC_CH_NONE && PARAMS[i].state != PARAM_BLOCKED) live++;

  // Pitch au point de départ : note CAL_BASE_NOTE, soit 0 mV.
  dacWrite(DAC_CH_PITCH, 0);

  // Le LFO n'est pas dans PARAMS[] (sortie PWM, pas DAC) : son défaut est posé ici.
  outputWriteLfoRate(CC_LFO_RATE_DEFAULT);

  return live;
}

void outputReport() {
  Serial.println(F("--- sorties ---"));
  for (uint8_t i = 0; i < PARAM_COUNT; i++) {
    const KobolParam& p = PARAMS[i];
    Serial.print(F("CC "));   Serial.print(p.cc);
    Serial.print(F("  "));    Serial.print(p.name);
    if (p.state == PARAM_BLOCKED) {
      Serial.println(F("  : bloque (mesure manquante)"));
    } else if (p.dac_ch == DAC_CH_NONE) {
      Serial.println(F("  : pas de canal DAC libre"));
    } else {
      Serial.print(F("  : DAC canal "));
      Serial.println(p.dac_ch == 0 ? 'A' : 'B');
    }
  }
}

bool outputIsLive(uint8_t i) {
  if (i >= PARAM_COUNT) return false;
  return PARAMS[i].dac_ch != DAC_CH_NONE && PARAMS[i].state != PARAM_BLOCKED;
}

void outputWriteMv(uint8_t i, int32_t mv) {
  if (!outputIsLive(i)) return;
  dacWrite(PARAMS[i].dac_ch, mvToCode(mv));
}

void outputWritePitchMv(int32_t mv) {
  if (mv < PITCH_MIN_MV) mv = PITCH_MIN_MV;
  if (mv > PITCH_MAX_MV) mv = PITCH_MAX_MV;
  dacWrite(DAC_CH_PITCH, mvToCode(mv));
}

void outputWriteGate(bool on) {
  digitalWrite(PIN_GATE, on ? HIGH : LOW);
}

void outputWriteLfoRate(uint8_t value) {
  analogWrite(PIN_LFO_PWM, value << 1);   // CC 0-127 -> PWM 0-254
}

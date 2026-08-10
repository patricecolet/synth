#include "output.h"
#include <SPI.h>

static bool s_seen[PARAM_COUNT];   // ce CC est-il déjà arrivé ?

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
  for (uint8_t i = 0; i < PARAM_COUNT; i++) {
    s_seen[i] = false;
    if (PARAMS[i].dac_ch != DAC_CH_NONE && PARAMS[i].state != PARAM_BLOCKED) live++;
  }

  // Le pitch est posé à 0 mV : en sortie jack c'est l'absence de CV, donc
  // le Kobol reste sur son accord de façade.
  dacWrite(DAC_CH_PITCH, 0);

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

void outputMarkSeen(uint8_t i) {
  if (i < PARAM_COUNT) s_seen[i] = true;
}

void outputWriteMv(uint8_t i, int32_t mv) {
  if (!outputIsLive(i)) return;
  // Tant que le CC n'est pas venu, on ne touche pas à la sortie : le potard
  // de façade garde la main et le Kobol sonne comme sans la carte.
  if (!s_seen[i]) return;
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

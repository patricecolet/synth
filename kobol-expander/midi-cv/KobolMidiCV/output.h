// output.h — couche de sortie CV
//
// Isole le reste du firmware du matériel qui génère les tensions.
// Aujourd'hui : MCP4822 sur SPI matériel, 2 canaux. Demain, un second
// MCP4822 sur un autre CS, ou le PWM d'un ESP32 : seul output.cpp change.

#pragma once
#include <Arduino.h>
#include "config.h"

// Initialise SPI, le DAC, le gate et le PWM du LFO.
// Renvoie le nombre de paramètres disposant réellement d'une sortie.
uint8_t outputBegin();

// Détaille sur le port série ce qui est câblé et ce qui ne l'est pas.
void outputReport();

// Ce paramètre a-t-il une sortie ?
bool outputIsLive(uint8_t param_index);

void outputWriteMv(uint8_t param_index, int32_t mv);
void outputWritePitchMv(int32_t mv);
void outputWriteGate(bool on);
void outputWriteLfoRate(uint8_t value);   // CC 0-127

// Conversion tension -> code DAC, exposée pour les tests.
uint16_t mvToCode(int32_t mv);

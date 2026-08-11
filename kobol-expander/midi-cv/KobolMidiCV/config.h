// config.h — Kobol Expander MIDI/CV
//
// Cible : Teensy 2.0 (ATmega32U4) + MCP4822, câblage repris de
//         v1-first-release/kobolDAC. L'ESP32-S3 de v2-simple reste la
//         cible d'évolution, mais le Teensy est monté et fonctionne.
//
// Le MCP4822 a DEUX canaux, donc deux sorties CV :
//   canal A -> pitch
//   canal B -> cutoff du filtre  (CC 74)
//
// Les autres paramètres de la table restent décrits mais sans sortie :
// il faudra un second MCP4822 sur un autre CS, ou passer à l'ESP32.
//
// Toutes les tensions sont en millivolts entiers. Pas de flottant :
// l'AVR à 16 MHz émule le float en logiciel, c'est ~10 µs par opération.

#pragma once
#include <Arduino.h>

// ─────────────────────────────────────────────────────────────────────
// Point d'injection : jacks de façade ou connecteur P1 ?
// ─────────────────────────────────────────────────────────────────────
//
// Les deux existent sur l'Expander et n'ont PAS la même échelle :
//
//   OUT_JACK  jacks CV de façade. Entrées prévues pour du CV externe,
//             derrière une résistance série de 100 kΩ. C'est ce que fait
//             la v1, qui sort 0-4 V du DAC sans conditionnement.
//
//   OUT_P1    connecteur séquenceur, en aval des résistances de
//             protection. Plage utile ±1,5 V seulement (mesures de
//             docs/measurements/). EXIGE un étage d'adaptation entre le
//             DAC et le connecteur — sans lui, on injecte 4 V sur une
//             entrée qui en attend 1,5.
//
// Le câblage actuel vient de la v1, donc OUT_JACK. À changer seulement
// après avoir monté l'étage d'adaptation.
#define KOBOL_OUTPUT_JACK 1

// ─────────────────────────────────────────────────────────────────────
// MCP4822
// ─────────────────────────────────────────────────────────────────────

// Broches Teensy 2.0, telles que câblées (cf. en-tête de kobolDAC.ino)
static const uint8_t PIN_DAC_CS  = 0;      // B0
// SCK = pin 1 (B1), MOSI = pin 2 (B2) : SPI matériel, gérés par SPI.h
static const uint8_t PIN_GATE    = 6;      // D1
static const uint8_t PIN_LFO_PWM = 10;     // C7, sortie PWM (OC4A)

static const uint8_t DAC_CH_PITCH  = 0;    // canal A
static const uint8_t DAC_CH_CUTOFF = 1;    // canal B

// 12 bits, gain ×2 sur référence interne 2,048 V -> 0 à 4,096 V.
static const uint16_t DAC_MAX_CODE = 4095;
static const int32_t  DAC_FULL_MV  = 4096;

// ─────────────────────────────────────────────────────────────────────
// Calibration du pitch
// ─────────────────────────────────────────────────────────────────────
//
// Valeur reprise de v1-first-release/kobolDAC.ino, qui tourne dans le
// Teensy et sonne juste :
//
//     noteVoltage = 4000 * note / 120.0;
//
// soit 4000 unités DAC pour 120 demi-tons = 33,3 mV par demi-ton, donc
// 400 mV par octave (le MCP4822 en gain ×2 sort 1 mV par unité).
//
// Ce n'est PAS du 1 V/octave, malgré ce qu'annonce le README de la v1.
// L'entrée visée est plus sensible d'un facteur 2,5. La valeur ci-dessous
// reproduit donc exactement le comportement validé à l'oreille — ne pas
// la « corriger » vers 1000 sans avoir mesuré à l'accordeur.
// Modifiable a l'execution (commande de calibration a venir), donc UNE SEULE
// definition, dans output.cpp. Un `static` ici donnerait une copie par unite de
// compilation : le .ino en ajusterait une et output.cpp lirait l'autre.
extern int16_t cal_mv_per_octave;

// Note produisant 0 mV. La v1 part de la note 0, ce qui place la note 120
// à 4000 mV, juste sous la pleine échelle du DAC.
static const uint8_t CAL_BASE_NOTE = 0;

// Butées dures en sortie DAC
static const int16_t PITCH_MIN_MV = 0;
static const int16_t PITCH_MAX_MV = 4096;

// Pitch bend, en centièmes de demi-ton pour rester en entier
static const int16_t PITCH_BEND_CENTS_MAX = 200;   // ±2 demi-tons

// ─────────────────────────────────────────────────────────────────────
// Table des paramètres
// ─────────────────────────────────────────────────────────────────────

static const uint8_t DAC_CH_NONE = 255;    // décrit mais sans sortie

enum ParamState : uint8_t {
  PARAM_OK,        // plage mesurée, pin confirmée
  PARAM_CHECK,     // pin fonctionnelle mais plage à confirmer
  PARAM_BLOCKED,   // en attente de mesure, jamais émis
};

struct KobolParam {
  uint8_t     cc;           // numéro de Control Change
  uint8_t     p1_pin;       // pin du connecteur P1
  uint8_t     dac_ch;       // canal DAC, ou DAC_CH_NONE
  int16_t     v_min_mv;     // tension pour CC = 0
  int16_t     v_max_mv;     // tension pour CC = 127
  int16_t     rest_mv;      // tension au repos, potard au centre
  ParamState  state;
  const char* name;
};

// AUCUNE valeur par défaut : le firmware n'écrit sur une sortie qu'après
// avoir reçu son CC. Le CV s'ADDITIONNE au potard de façade
// (connector_analysis.md : « CV externe + potentiomètre = contrôle
// hybride »), donc écrire au démarrage décalerait le réglage du Kobol
// avant qu'on ait touché à quoi que ce soit.
//
// v_min_mv / v_max_mv sont les plages relevées AU CONNECTEUR P1. Elles ne
// valent que pour KOBOL_OUTPUT_JACK = 0. En sortie jack, seuls le pitch et
// le cutoff sont câblés et utilisent la pleine échelle du DAC.
//
// rest_mv est un repère de calibration, jamais émis.
//
// La table est DÉFINIE dans output.cpp, pas ici : sur AVR un tableau
// `static const` est recopié en RAM, et un `static` dans un header en donne
// une copie par unité de compilation — donc deux fois la même table.
extern const KobolParam PARAMS[];

// Doit valoir le nombre de lignes de PARAMS[] dans output.cpp ; un
// static_assert là-bas vérifie l'accord. Écrit en dur parce que cette valeur
// sert de borne de tableau, ce qu'un `extern const` ne permet pas.
static const uint8_t PARAM_COUNT = 14;

// ─────────────────────────────────────────────────────────────────────
// Contrôleurs sans sortie CV propre
// ─────────────────────────────────────────────────────────────────────

static const uint8_t CC_MODWHEEL       = 1;
static const uint8_t CC_PORTAMENTO_MS  = 5;
static const uint8_t CC_PORTAMENTO_SW  = 65;
static const uint8_t CC_LFO_RATE       = 76;   // PWM sur PIN_LFO_PWM
static const uint8_t CC_VEL_TO_CUT_ON  = 114;
static const uint8_t CC_VEL_TO_CUT_OFF = 115;
static const uint8_t CC_VEL_TO_VCA     = 116;
static const uint8_t CC_GATE_FORCE     = 118;  // CC 100 en v1 : c'est le RPN
                                               // LSB officiel, à ne pas réutiliser

// Pin 2 du P1 = rail -14,5 V. Aucune sortie ne doit y aboutir.
// Pin 13 = VCO1 Frequency, réglage manuel par choix.

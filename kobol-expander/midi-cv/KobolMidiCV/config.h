// config.h — Kobol Expander MIDI/CV
//
// Cible : ESP32-S3 VROOM N16R8 (décision de v2-simple/README.md, avril 2026)
// Sortie : PWM 14 bits -> RC -> LM324 (gain + offset) -> connecteur P1 du Kobol
//
// Toutes les tensions sont en millivolts entiers, mesurées AU CONNECTEUR P1,
// pas en sortie d'ESP32. La conversion mV -> rapport cyclique tient compte de
// l'étage LM324 via cond_min_mv / cond_max_mv.
//
// Sources des plages : docs/measurements/p1_connector_measurements.md
//                      docs/measurements/vco_injection_test.md

#pragma once
#include <Arduino.h>

// ─────────────────────────────────────────────────────────────────────
// PWM
// ─────────────────────────────────────────────────────────────────────

// 14 bits = 16384 pas. Sur la plage pitch de 3108 mV, cela fait 0,19 mV
// par pas — voir CALIBRATION.md pour ce que ça vaut en cents.
static const uint8_t  PWM_BITS = 14;
static const uint16_t PWM_MAX  = (1u << PWM_BITS) - 1;

// Fréquence porteuse. Le filtre RC de l'étage d'adaptation (1 kΩ / 330 nF,
// fc ≈ 480 Hz) doit la rejeter : plus elle est haute, moins il reste
// d'ondulation. L'ESP32-S3 impose freq × 2^bits <= 80 MHz, donc 14 bits
// plafonne à 4882 Hz.
static const uint32_t PWM_FREQ_HZ = 4880;

// ─────────────────────────────────────────────────────────────────────
// Étage d'adaptation LM324
// ─────────────────────────────────────────────────────────────────────
//
// Deux gabarits, selon que la pin visée est unipolaire ou bipolaire.
// Ce sont les tensions obtenues au connecteur pour un rapport cyclique
// de 0 % et de 100 %. À REMESURER après montage de la carte : ces
// valeurs sont les cibles de conception, pas des mesures.

// Gabarit A — unipolaire, pour les pins qui restent positives
static const int16_t COND_A_MIN_MV = -200;
static const int16_t COND_A_MAX_MV = 1100;

// Gabarit B — bipolaire, pour la résonance et les pins négatives
static const int16_t COND_B_MIN_MV = -1600;
static const int16_t COND_B_MAX_MV = 1600;

// ─────────────────────────────────────────────────────────────────────
// Calibration du pitch — pin 11, VCO2
// ─────────────────────────────────────────────────────────────────────
//
// Le test d'injection (vco_injection_test.md) a établi par écoute :
//   point neutre  -22 mV   (le VCO2 ne bouge pas)
//   note grave  -1532 mV
//   note aiguë  +1532 mV
//   réponse EXPONENTIELLE, décrochage vers 10 Hz
//
// Ce qui n'a PAS été mesuré : combien de millivolts par octave. Le
// document en déduit 155,4 mV/oct en supposant « 10 octaves sur 1554 mV »,
// mais 1554 mV n'est que la MOITIÉ de la plage relevée (3108 mV). Si la
// gamme complète tient dans les 3108 mV, on est à 310,8 mV/oct.
//
// Les deux valeurs sont des hypothèses et diffèrent d'un facteur 2 : avec
// la mauvaise, tous les intervalles sont doublés ou divisés par deux.
// -> à régler à l'accordeur, procédure dans CALIBRATION.md.
static const int16_t CAL_NEUTRAL_MV   = -22;
static const uint8_t CAL_NEUTRAL_NOTE = 60;    // Do3 au point neutre
static float         cal_mv_per_octave = 310.8f;  // hypothèse haute, à vérifier

// Butées dures, jamais dépassées quelle que soit la calibration
static const int16_t PITCH_MIN_MV = -1532;
static const int16_t PITCH_MAX_MV =  1532;

// Pitch bend, en demi-tons pour l'excursion maximale
static const float PITCH_BEND_SEMITONES = 2.0f;

// ─────────────────────────────────────────────────────────────────────
// Brochage ESP32-S3
// ─────────────────────────────────────────────────────────────────────
//
// ATTENTION : le LEDC de l'ESP32-S3 n'offre que 8 canaux PWM (4 timers,
// pas de mode grande vitesse, contrairement à l'ESP32 d'origine et à ses
// 16 canaux). v3-advanced/README.md annonce « 16 PWM natifs » — c'est vrai
// pour l'ESP32 classique, faux pour le S3.
//
// Il faut 12 sorties. Les 4 en trop sont marquées OUT_EXTERNAL et
// resteront muettes tant qu'un DAC externe (MCP4822 en chaîne SPI, ou
// PCA9685) n'aura pas été ajouté. output.cpp signale lesquelles au
// démarrage.
static const uint8_t GPIO_UNASSIGNED = 255;   // sortie non câblée

static const uint8_t GPIO_GATE = 4;           // jack Gate face avant

// ─────────────────────────────────────────────────────────────────────
// Table des paramètres
// ─────────────────────────────────────────────────────────────────────

enum ParamState : uint8_t {
  PARAM_OK,        // plage mesurée, pin confirmée
  PARAM_CHECK,     // pin fonctionnelle mais plage à confirmer
  PARAM_BLOCKED,   // en attente d'oscilloscope, jamais émis
};

struct KobolParam {
  uint8_t     cc;           // numéro de Control Change
  uint8_t     p1_pin;       // pin du connecteur P1
  uint8_t     gpio;         // GPIO ESP32, ou GPIO_UNASSIGNED
  int16_t     v_min_mv;     // tension pour CC = 0
  int16_t     v_max_mv;     // tension pour CC = 127
  int16_t     cond_min_mv;  // tension au connecteur à 0 % de rapport cyclique
  int16_t     cond_max_mv;  // tension au connecteur à 100 %
  uint8_t     cc_default;   // valeur appliquée au démarrage
  ParamState  state;
  const char* name;
};

// Ordre = ordre d'initialisation. Les PARAM_OK d'abord : si les canaux
// LEDC manquent, ce sont les moins critiques qui tombent.
static const KobolParam PARAMS[] = {
  // CC   pin  gpio  vmin   vmax   condmin        condmax        def  état          nom
  {  74,  15,   5,      0,   900,  COND_A_MIN_MV, COND_A_MAX_MV,  90, PARAM_CHECK,  "VCF Cutoff"    },
  {  71,  12,   6,   -670,   610,  COND_B_MIN_MV, COND_B_MAX_MV,   0, PARAM_OK,     "VCF Resonance" },
  {  73,   8,   7,   -660, -1150,  COND_B_MIN_MV, COND_B_MAX_MV,   0, PARAM_CHECK,  "VCF Attack"    },
  {  75,   7,  15,  -1350,  -360,  COND_B_MIN_MV, COND_B_MAX_MV,  40, PARAM_OK,     "VCF Decay"     },
  { 102,   4,  16,      0,   920,  COND_A_MIN_MV, COND_A_MAX_MV,  90, PARAM_OK,     "VCF Sustain"   },
  { 103,  10,  17,     50,   600,  COND_A_MIN_MV, COND_A_MAX_MV,  64, PARAM_OK,     "VCF ADS Ctrl"  },
  { 105,   5,  18,   -660,  -280,  COND_B_MIN_MV, COND_B_MAX_MV,   0, PARAM_OK,     "VCA Attack"    },
  { 106,   1,  GPIO_UNASSIGNED, -1410, -1170, COND_B_MIN_MV, COND_B_MAX_MV, 40, PARAM_OK, "VCA Decay" },
  { 107,  16,  GPIO_UNASSIGNED,   -30,   940, COND_A_MIN_MV, COND_A_MAX_MV, 100, PARAM_OK, "VCA Sustain" },
  { 109,   9,  GPIO_UNASSIGNED,     0,   610, COND_A_MIN_MV, COND_A_MAX_MV, 100, PARAM_OK, "VCO2 Volume" },
  { 108,  14,  GPIO_UNASSIGNED,   130,   600, COND_A_MIN_MV, COND_A_MAX_MV, 100, PARAM_CHECK, "VCO1 Volume" },

  // Bloqués — voir MIDI_MAP.md §6. Jamais émis, même si un GPIO est posé.
  { 112,   3,  GPIO_UNASSIGNED, 0, 0, COND_A_MIN_MV, COND_A_MAX_MV, 0, PARAM_BLOCKED, "VCO1 Waveform" },
  { 113,   6,  GPIO_UNASSIGNED, 0, 0, COND_A_MIN_MV, COND_A_MAX_MV, 0, PARAM_BLOCKED, "VCO2 Waveform" },
  {  76, 255,  GPIO_UNASSIGNED, 0, 0, COND_A_MIN_MV, COND_A_MAX_MV, 0, PARAM_BLOCKED, "LFO Rate"      },
};

static const uint8_t PARAM_COUNT = sizeof(PARAMS) / sizeof(PARAMS[0]);

// Pitch : pin 11, traité à part car piloté par les notes et non par un CC.
static const uint8_t GPIO_PITCH = 8;

// ─────────────────────────────────────────────────────────────────────
// Contrôleurs sans sortie matérielle
// ─────────────────────────────────────────────────────────────────────

static const uint8_t CC_MODWHEEL       = 1;    // -> profondeur LFO logiciel
static const uint8_t CC_PORTAMENTO_MS  = 5;    // temps de glide
static const uint8_t CC_PORTAMENTO_SW  = 65;   // glide on/off
static const uint8_t CC_VEL_TO_CUT_ON  = 114;  // vélocité -> cutoff, note on
static const uint8_t CC_VEL_TO_CUT_OFF = 115;  // vélocité -> cutoff, note off
static const uint8_t CC_VEL_TO_VCA     = 116;  // vélocité -> VCA sustain
static const uint8_t CC_GATE_FORCE     = 118;  // gate forcé (CC100 en v1 : c'est
                                               // le RPN LSB officiel, à ne pas
                                               // réutiliser)

static const uint16_t PORTAMENTO_MS_MAX = 1270;  // CC 5 × 10 ms

// Pin 2 = rail -14,5 V. Aucune sortie ne doit y aboutir.
// Pin 13 = VCO1 Frequency, réglage manuel par choix (vco_injection_test.md).

// output.h — couche de sortie CV
//
// Isole le reste du firmware du matériel qui génère les tensions.
// Aujourd'hui : PWM LEDC de l'ESP32-S3. Demain, si les 8 canaux LEDC ne
// suffisent pas, un DAC externe : seul ce fichier et output.cpp changent.

#pragma once
#include <Arduino.h>
#include "config.h"

// Initialise toutes les sorties. Renvoie le nombre de canaux réellement
// obtenus. Les paramètres non servis sont signalés sur le port série et
// leurs écritures deviennent silencieuses.
uint8_t outputBegin();

// Écrit une tension au connecteur P1, en millivolts. Borne à ce que
// l'étage d'adaptation sait produire.
void outputWriteMv(uint8_t param_index, int32_t mv);

// Idem pour le pitch (pin 11), qui n'est pas dans la table PARAMS.
void outputWritePitchMv(int32_t mv);

// Un paramètre a-t-il réellement une sortie câblée et active ?
bool outputIsLive(uint8_t param_index);

// Conversion tension -> rapport cyclique, exposée pour les tests.
uint16_t mvToDuty(int32_t mv, int16_t cond_min_mv, int16_t cond_max_mv);

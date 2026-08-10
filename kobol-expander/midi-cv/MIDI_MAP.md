# Carte MIDI — Kobol Expander MIDI/CV

Liste des contrôleurs MIDI de l'interface. Base : les 16 paramètres du
connecteur CV du Kobol, relevés dans
[`docs/connector_analysis.md`](../docs/connector_analysis.md) et mesurés dans
[`docs/measurements/p1_connector_measurements.md`](../docs/measurements/p1_connector_measurements.md).

Numérotation fondée sur les **Sound Controllers MIDI (CC 70-79)**, prévus pour
ça : un contrôleur du commerce tombe juste sans configuration. Les paramètres
sans équivalent standard vont dans les CC 102-119, officiellement *undefined*
et donc libres.

Canal MIDI : **1** par défaut.

---

## 1. Notes et jeu

| Fonction | Message | Sortie matérielle |
|---|---|---|
| Pitch CV | Note On/Off | MCP4822 canal A |
| Gate | Note On/Off | Teensy pin 6, jack façade |
| Gate forcé | **CC 118** | idem |
| Portamento (glide) | **CC 5** — Portamento Time | interpolé logiciel |
| Portamento on/off | **CC 65** | idem |
| Pitch bend | Pitch Bend | ±2 demi-tons, s'ajoute au pitch |

`CC 100` utilisé par la v1 est en réalité le **RPN LSB** officiel : tout clavier
émettant des RPN déclencherait le gate. À déplacer vers CC 118.

## 2. VCF — filtre

| Paramètre | CC | Nom standard | Pin | Plage mesurée |
|---|---|---|---|---|
| Cutoff | **74** | Brightness | 15 | 0 → +0,90 V |
| Résonance | **71** | Harmonic Content | 12 | −0,67 → +0,61 V |
| Attack | **73** | Attack Time | 8 | ⚠️ non monotone, voir §6 |
| Decay | **75** | Decay Time | 7 | −1,35 → −0,36 V |
| Sustain | **102** | *(libre)* | 4 | 0 → +0,92 V |
| ADS Control | **103** | *(libre)* | 10 | +0,05 → +0,60 V |
| Key follow | **104** | *(libre)* | — | ⚠️ pin inconnue, voir §6 |

## 3. VCA — amplificateur

| Paramètre | CC | Nom standard | Pin | Plage mesurée |
|---|---|---|---|---|
| Attack | **105** | *(libre)* | 5 | −0,66 → −0,28 V |
| Decay | **106** | *(libre)* | 1 | −1,41 → −1,17 V |
| Sustain | **107** | *(libre)* | 16 | −0,03 → +0,94 V |

Les CC 72/73 standard sont réservés au VCF, plus caractéristique du Kobol. Le
VCA passe donc en zone libre.

## 4. VCO 1 et 2

| Paramètre | CC | Pin | Plage mesurée |
|---|---|---|---|
| VCO1 Volume | **108** | 14 | +0,13 → +0,60 V |
| VCO2 Volume | **109** | 9 | 0 → +0,61 V |
| **VCO2 Freq — le pitch** | *piloté par les notes* | 11 | **−1,532 → +1,532 V, exponentiel** ✅ |
| VCO1 Freq | — | 13 | réglage manuel, par choix |
| VCO1 Waveform | **112** | 3 | ⚠️ **0 V constant** — voir §6 |
| VCO2 Waveform | **113** | 6 | ⚠️ **0 V constant** — voir §6 |

Le pitch passe par la **pin 11**, validée au test d'injection : point neutre à
−22 mV, plage ±1532 mV, réponse exponentielle. Elle n'a pas de CC — elle suit
les notes MIDI, le pitch bend et le portamento. Combien de millivolts font une
octave n'a jamais été mesuré : voir [`CALIBRATION.md`](CALIBRATION.md), c'est le
premier réglage à faire.

La **pin 13** (VCO1 Freq) est laissée au potard de façade, choix acté dans
`vco_injection_test.md`. Le CC 110 reste donc libre.

## 5. LFO

| Paramètre | CC | Nom standard | Pin |
|---|---|---|---|
| LFO Rate | **76** | Vibrato Rate | PWM Teensy pin 10 ; ⚠️ pin P1 inconnue, **pas la pin 2** |
| LFO Depth | **77** | Vibrato Depth | — (logiciel) |
| Mod wheel → LFO depth | **1** | Modulation | — (logiciel) |

## 6. Vélocité et modulation logicielle

Ces contrôleurs ne pilotent aucune pin : ils dosent des modulations calculées
par le firmware.

| Fonction | CC | Statut v1 |
|---|---|---|
| Vélocité → Cutoff, note on | CC 14 → **114** | ✅ fait, à renuméroter |
| Vélocité → Cutoff, note off | CC 15 → **115** | ✅ fait, à renuméroter |
| Vélocité → VCA | **116** | ➕ à ajouter |
| LFO → Cutoff | **117** | ➕ à ajouter |

---

## Points bloquants avant câblage

Ce sont des conclusions de **tes mesures**, pas des hypothèses. Trois d'entre
elles contredisent le tableau de `connector_analysis.md`.

### La pin 2 n'est pas LFO Rate — c'est le rail −14,5 V

Mesurée à −14,52 V, constante quelle que soit la position du potard. Le tableau
d'analyse l'annonce comme « LFO Rate, entrée CV externe ». **Y brancher une
sortie de DAC serait un court-circuit sur l'alimentation négative** — DAC
détruit, et possiblement l'alim du Kobol.

Le seul paramètre du firmware v1 qui ne passe pas par le DAC est justement le
LFO Rate (PWM sur `PIN_C7`). À vérifier physiquement sur quelle pin il arrive
avant de reproduire ce câblage.

### Les pins à 0 V : résolu pour le pitch, ouvert pour les waveforms

Les pins 3, 6, 11, 13 mesurent 0 V quel que soit le potard. **Le test
d'injection tranche la question** : ce sont des *entrées* CV à haute impédance,
qui reposent à 0 V tant que rien ne les pilote. Un multimètre ne peut rien y
lire — il faut y injecter une tension et écouter.

C'est ce qui a été fait sur la pin 11, avec une pile variable : elle répond
parfaitement, de −1532 à +1532 mV, en exponentiel. **Le pitch n'est donc pas
bloqué.**

Restent ouvertes les **pins 3 et 6** (waveforms VCO1/VCO2), sur lesquelles le
même test d'injection n'a pas encore été fait. C'est la manipulation à refaire
en priorité, et elle ne demande qu'une pile et un potentiomètre — pas
l'oscilloscope.

La **pin 13** (VCO1 Freq) n'est pas un problème mais un choix : réglage manuel
au potard de façade.

### La pin 8 varie de façon non monotone

VCF Attack mesure −0,66 V au minimum, −1,26 V au milieu et −1,15 V au maximum.
Une valeur médiane hors de l'intervalle des extrêmes ne peut pas venir d'un
simple diviseur : soit une erreur de relevé, soit la pin porte autre chose.

### Deux nomenclatures de connecteur non réconciliées

[`reference/kobol/Plug.txt`](../../reference/kobol/Plug.txt) décrit **deux**
connecteurs, P1 et P7, avec des affectations différentes. Le tableau nommé
« P1 » dans `connector_analysis.md` correspond en fait, ligne pour ligne, à la
liste **P7** de `Plug.txt`.

Les deux listes se recouvrent largement mais dans un ordre permuté : les
paramètres 9 à 16 de P7 sont, à deux positions près, ceux de P1 lus à l'envers.
C'est la signature d'un désaccord de convention de numérotation — DIP
(1-8 en haut, 16-9 en bas) contre IDC nappe (impairs en haut, pairs en bas).
La correspondance ne tombe pas juste pour autant, donc le conflit reste ouvert.

**Une erreur de convention ici décale tout le connecteur** et enverrait le CV du
cutoff sur le rail d'alimentation. À résoudre avant de sertir quoi que ce soit.

### Adaptation de niveau

Les plages utiles font moins de 1 V crête, centrées de manière variable :
certaines pins sont unipolaires positives (4, 9, 10, 14, 15, 16), d'autres
franchement négatives (1, 5, 7, 8), une bipolaire (12, résonance), et le pitch
va de −1,5 à +1,5 V.

Le MCP4822 sort 0 → 4,096 V. Il faut donc par pin un décalage et une
atténuation. Le firmware n'utilise que deux gabarits de conditionnement,
unipolaire et bipolaire (`COND_A_*` / `COND_B_*` dans `config.h`), et rattrape
le reste en logiciel — chaque paramètre connaît sa propre plage utile. Cela
évite un ampli-op réglé finement par canal, mais coûte de la résolution sur les
plages étroites : le VCA Decay n'occupe que 240 mV sur les 3200 mV du gabarit
bipolaire, soit 7 % de la course, donc environ 1200 pas PWM sur 16384.

Reste que l'hypothèse « ±1,5 V uniforme » du README de v2-simple ne tient pas
telle quelle : les plages relevées vont de 0,24 V (VCA Decay) à 1,28 V
(Résonance), un rapport de 5.

### Le matériel actuel n'offre que deux sorties

Le câblage repris de la v1 est un **Teensy 2.0 + un seul MCP4822**, donc deux
canaux CV : le pitch et le cutoff. C'est assez pour CC 74, pas pour les douze
autres paramètres, qui restent décrits dans la table sans sortie.

Pour les servir : un second MCP4822 sur un autre CS (4 canaux), ou l'ESP32-S3
de `v2-simple`. Attention, ce dernier n'a que **8 canaux LEDC** et non 16 —
`v3-advanced/README.md` annonce « 16× PWM natifs », ce qui vaut pour l'ESP32
d'origine mais pas pour le S3 (4 timers, pas de mode grande vitesse).

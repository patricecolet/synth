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

| Fonction | Message | Sortie matérielle | Statut v1 |
|---|---|---|---|
| Pitch CV | Note On/Off | DAC canal A, 1V/oct | ✅ fait |
| Gate | Note On/Off | broche numérique | ✅ fait |
| Gate forcé | CC 100 → **CC 118** | idem | ⚠️ à renuméroter |
| Portamento (glide) | **CC 5** — Portamento Time | interpolé logiciel (`Ramp.h`) | ✅ fait, standard |
| Portamento on/off | **CC 65** | idem | ➕ à ajouter |
| Pitch bend | Pitch Bend | s'ajoute au pitch CV | ➕ à ajouter |

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
| VCO1 Freq | **110** | 13 | ⚠️ **0 V constant** — voir §6 |
| VCO2 Freq | **111** | 11 | ⚠️ **0 V constant** — voir §6 |
| VCO1 Waveform | **112** | 3 | ⚠️ **0 V constant** — voir §6 |
| VCO2 Waveform | **113** | 6 | ⚠️ **0 V constant** — voir §6 |

## 5. LFO

| Paramètre | CC | Nom standard | Pin |
|---|---|---|---|
| LFO Rate | **76** | Vibrato Rate | ⚠️ **pas la pin 2** — voir §6 |
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

### Quatre paramètres mesurent 0 V constant

Pins 3, 6, 11, 13 — VCO1/VCO2 Waveform et VCO1/VCO2 Freq — ne bougent pas quand
on tourne le potard correspondant. Ce sont les paramètres les plus importants
musicalement : sans le pitch, il n'y a pas d'interface MIDI.

Deux lectures possibles, à trancher à l'oscilloscope :

- **Ce sont des entrées CV pures**, à haute impédance, qui reposent à 0 V tant
  que rien ne les pilote. C'est cohérent pour les pins 3 et 6, dont l'analyse du
  schéma ne montre aucune branche vers un potard de façade.
- **L'affectation des pins est fausse.** Les pins 11 et 13 sont censées être
  reliées à un potard (`R78,7 kΩ → Potard Freq`) : elles devraient donc varier.
  La pin 15 est dans le cas inverse — aucun potard au schéma, et pourtant elle
  varie de 0 à 0,9 V.

Tant que ce n'est pas tranché, les CC 110-113 sont réservés mais non câblables.

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
franchement négatives (1, 5, 7, 8), une bipolaire (12, résonance).

Le MCP4822 sort 0 → 4,096 V en gain ×2. Il faut donc par pin un décalage et une
atténuation, soit un ampli-op par canal — 16 au total. C'est la vraie charge
matérielle du projet, et elle contredit l'hypothèse « ±2,5 V uniforme » du
README de v2-simple : les plages relevées vont de 0,24 V (VCA Decay) à 1,28 V
(Résonance), soit un rapport de 5 entre les deux.

---

## Récapitulatif

| Zone CC | Usage |
|---|---|
| 1, 5, 65 | Modulation, portamento — standards |
| 71-77 | Sound Controllers — VCF et LFO |
| 102-113 | Paramètres Kobol sans équivalent standard |
| 114-117 | Profondeurs de modulation logicielle |
| 118 | Gate forcé |

**24 contrôleurs** au total : 20 câblés sur le connecteur, 4 purement logiciels.
Sur les 20, **6 sont bloqués** par les mesures ci-dessus (LFO Rate, les 4 VCO,
VCF Key follow) et 1 est douteux (VCF Attack).

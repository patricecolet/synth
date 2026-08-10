# Calibration

À faire **avant toute utilisation musicale**. Le firmware sort avec une valeur
de pitch théorique, jamais vérifiée sur ta machine.

## Le bug d'échelle de la v1

`v1-first-release/kobolDAC.ino` pose :

```c
noteVoltage = 4000 * (float) note / 120.0;
```

Soit 4000 unités DAC pour 120 demi-tons, c'est-à-dire **33,3 unités par
demi-ton**. Le MCP4822 en gain ×2 sort 1 mV par unité, donc 33,3 mV par
demi-ton, soit **400 mV par octave**.

Or une entrée 1 V/octave demande **1000 mV par octave**.

La v1 jouait donc ses intervalles à 40 % de leur écart : **une octave sonnait
comme une quarte** (400 mV ÷ 1000 = 0,4 octave = 4,8 demi-tons). C'est
probablement passé inaperçu parce qu'on n'entend pas un décalage d'échelle en
jouant une note à la fois — seulement en jouant deux notes distantes.

Le nouveau firmware part sur `cal_mv_per_octave = 1000`, valeur théorique. Reste
à vérifier qu'elle correspond à ton exemplaire.

## Procédure

Il faut un accordeur (ou un accordeur logiciel dans le DAW) et un clavier MIDI.

### 1. Vérifier le repos

Au démarrage, le firmware met le canal A à 0 mV et n'écrit sur aucun autre
paramètre tant qu'un CC n'est pas arrivé. Le Kobol doit sonner **exactement**
comme carte débranchée. Si un réglage saute au branchement, c'est que le CV
n'est pas au repos — vérifier le câblage avant d'aller plus loin.

### 2. Mesurer l'octave réelle

Joue **Do1 (note 24)**, relève la fréquence. Joue **Do2 (note 36)**, relève.

- Rapport **2,0** → `cal_mv_per_octave` est juste.
- Rapport **plus petit** → la constante est trop petite.
- Rapport **plus grand** → trop grande.

Le facteur correctif se lit directement :

```
cal_mv_per_octave_correct = cal_mv_per_octave ÷ log2(f_haut ÷ f_bas)
```

Exemple : Do1 → 65 Hz, Do2 → 86 Hz. Le rapport vaut 1,32, soit log2(1,32) =
0,40 octave au lieu de 1. Il faut donc diviser 1000 par 0,40 → **2500 mV/octave**.
(C'est exactement le symptôme qu'aurait donné l'échelle de la v1.)

Refaire la mesure après correction : deux itérations suffisent en général.

### 3. Vérifier l'étendue

Le DAC sort 0 à 4096 mV. L'étendue jouable vaut donc
`4096 ÷ cal_mv_per_octave` octaves à partir de `CAL_BASE_NOTE`.

| `cal_mv_per_octave` | étendue | depuis Do1 (note 24) |
|---|---|---|
| 1000 | 4,1 octaves | jusqu'à Do5 |
| 2500 | 1,6 octave | jusqu'à Sol2 |

Si l'étendue est trop courte, deux leviers : monter `CAL_BASE_NOTE` pour
recentrer la plage sur ce que tu joues, ou ajouter un étage d'amplification
derrière le DAC.

### 4. Résolution

Le MCP4822 est en 12 bits sur 4096 mV, soit **1 mV par pas**.

| `cal_mv_per_octave` | mV par demi-ton | pas DAC par demi-ton | erreur max |
|---|---|---|---|
| 1000 | 83,3 | 83 | 6 cents |
| 2500 | 208,3 | 208 | 2,4 cents |

6 cents est audible sur un accord tenu, à la limite du perceptible sur une
mélodie. C'est la limite du 12 bits : passer au 16 bits (MCP4922 ou DAC 16
bits) diviserait l'erreur par 16 si un jour ça gêne.

## Calibration des autres paramètres

Avec un seul MCP4822, **seuls le pitch et le cutoff ont une sortie**. Les autres
plages de `PARAMS[]` viennent des relevés au multimètre de
[`../docs/measurements/p1_connector_measurements.md`](../docs/measurements/p1_connector_measurements.md)
et ne servent que le jour où on branchera le connecteur P1.

Deux marquées `check` méritent une vérification à l'oreille :

- **VCF Cutoff** (pin 15) et **VCO1 Volume** (pin 14) — relevées comme variables
  au multimètre, mais listées à 0 V dans
  [`../docs/measurements/analysis_issues.md`](../docs/measurements/analysis_issues.md).
  Les deux documents se contredisent.
- **VCF Attack** (pin 8) — mesures non monotones : −660 mV au minimum,
  −1260 mV au milieu, −1150 mV au maximum. Une médiane hors de l'intervalle
  des extrêmes ne peut pas venir d'un diviseur résistif. À reprendre en
  relevant 5 points.

## Jacks ou connecteur P1 ?

`config.h` porte un commutateur `KOBOL_OUTPUT_JACK`, à **1** par défaut, car
c'est ce que fait le câblage repris de la v1.

| | Échelle | Conditionnement |
|---|---|---|
| **Jacks de façade** | pleine échelle DAC, 0-4096 mV | aucun, le DAC attaque directement |
| **Connecteur P1** | ±1,5 V utiles seulement | **étage d'adaptation obligatoire** |

Passer le commutateur à 0 sans avoir monté l'étage d'adaptation revient à
injecter 4 V sur une entrée qui en attend 1,5.

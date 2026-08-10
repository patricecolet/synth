# Calibration

À faire **avant toute utilisation musicale**. Le firmware sort d'usine avec une
valeur de pitch qui est une hypothèse, pas une mesure.

## Pourquoi c'est nécessaire

Le test d'injection ([`../docs/measurements/vco_injection_test.md`](../docs/measurements/vco_injection_test.md))
a établi par écoute, à la pile variable :

| | |
|---|---|
| Point neutre (VCO2 immobile) | **−22 mV** |
| Note la plus grave | **−1532 mV** |
| Note la plus aiguë | **+1532 mV** |
| Réponse | **exponentielle** |
| Décrochage | vers 10 Hz |

Ce qui n'a **pas** été mesuré : combien de millivolts font une octave. Or c'est
la seule constante qui détermine si l'instrument joue juste.

Le document en déduit 155,4 mV/octave, en posant « 10 octaves sur 1554 mV ».
Mais 1554 mV n'est que la moitié de la plage relevée : de −1532 à +1532 mV, il
y a **3108 mV**. Si la gamme complète tient dans cette plage, on est à
**310,8 mV/octave**.

Les deux valeurs diffèrent d'un facteur 2. Avec la mauvaise, **tous les
intervalles sont doublés ou divisés par deux** — une octave jouée sonne comme
une quinte, ou comme deux octaves. Le firmware part sur 310,8 mV/oct
(`cal_mv_per_octave` dans `config.h`), qui est l'hypothèse la plus cohérente
avec la plage mesurée, mais ça reste à confirmer.

## Procédure

Il faut un accordeur (ou un accordeur logiciel dans le DAW) et un clavier MIDI.

### 1. Vérifier le point neutre

Sans rien envoyer, le firmware place la pin 11 à −22 mV. Le potard Freq2 du
Kobol doit être à zéro. Joue Do3 (note 60) : le VCO2 doit sonner exactement
comme quand rien n'est branché.

Si ce n'est pas le cas, ajuster `CAL_NEUTRAL_MV` dans `config.h` jusqu'à ce que
la note 60 ne déplace pas le VCO2.

### 2. Mesurer l'octave réelle

Joue **Do3 (note 60)**, relève la fréquence à l'accordeur. Joue **Do4 (note
72)**, relève à nouveau.

- Si la seconde est **le double** de la première → `cal_mv_per_octave` est bon.
- Si elle est **quadruple** → la valeur est deux fois trop grande. Diviser par 2.
- Si elle est **1,41× environ** (une quinte) → deux fois trop petite. Multiplier par 2.

Puis affiner : le rapport mesuré donne directement le facteur correctif.

```
cal_mv_per_octave_correct = cal_mv_per_octave × log2(f_haut / f_bas)
```

Exemple : si Do3 → 130 Hz et Do4 → 190 Hz, le rapport est 1,46, soit
log2(1,46) = 0,55 octave au lieu de 1. Il faut donc diviser la constante par
0,55, c'est-à-dire la porter de 310,8 à 565 mV/octave.

### 3. Vérifier l'étendue

Une fois l'octave juste, chercher jusqu'où ça tient. Les butées
`PITCH_MIN_MV` / `PITCH_MAX_MV` bornent à ±1532 mV, donc l'étendue jouable
vaut `3064 / cal_mv_per_octave` octaves autour de Do3.

À 310,8 mV/oct cela fait environ 10 octaves — plus que la plage MIDI. À
565 mV/oct, 5,4 octaves, soit de Sol0 à Ré6 : il faudra probablement décaler
`CAL_NEUTRAL_NOTE` pour centrer la plage sur ce que tu joues.

Vérifier aussi le bas : le test d'injection signale un décrochage vers 10 Hz.
Les notes sous cette limite ne tiendront pas l'accord.

### 4. Résolution obtenue

Le PWM est en 14 bits sur la plage d'adaptation (3200 mV pour le gabarit
bipolaire), soit **0,195 mV par pas**.

| `cal_mv_per_octave` | mV par demi-ton | pas PWM par demi-ton | erreur max |
|---|---|---|---|
| 155,4 | 12,9 | 66 | 0,75 cent |
| 310,8 | 25,9 | 133 | 0,38 cent |
| 565 | 47,1 | 241 | 0,21 cent |

Dans tous les cas la résolution est largement suffisante — l'erreur d'accord
viendra de la stabilité analogique, pas du PWM.

> Le calcul de résolution du document d'injection annonce « 2,8 pas PWM par
> cent ». Il confond deux échelles en route : à 155,4 mV/oct, un cent vaut
> 0,1295 mV, soit **0,66 pas PWM** et non 2,8. La conclusion tenait quand même
> — la résolution suffit — mais avec quatre fois moins de marge qu'annoncé.

## Calibration des autres paramètres

Les plages de `PARAMS[]` dans `config.h` viennent des relevés au multimètre de
[`../docs/measurements/p1_connector_measurements.md`](../docs/measurements/p1_connector_measurements.md).
Elles sont bornées par l'étage d'adaptation, donc une erreur ne peut pas
endommager le Kobol — au pire le paramètre ne couvre pas toute sa course.

Deux marquées `PARAM_CHECK` méritent une vérification à l'oreille :

- **VCF Cutoff** (pin 15) et **VCO1 Volume** (pin 14) — relevées comme
  variables au multimètre, mais listées à 0 V dans
  [`../docs/measurements/analysis_issues.md`](../docs/measurements/analysis_issues.md).
  Les deux documents se contredisent.
- **VCF Attack** (pin 8) — mesures non monotones : −660 mV au minimum,
  −1260 mV au milieu, −1150 mV au maximum. Une médiane hors de l'intervalle
  des extrêmes ne peut pas venir d'un diviseur résistif. Le firmware
  interpole de −660 à −1150 mV en ignorant le point milieu, ce qui est
  probablement faux. À reprendre au multimètre en relevant 5 points.

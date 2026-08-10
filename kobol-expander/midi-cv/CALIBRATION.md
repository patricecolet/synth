# Calibration

Le firmware reprend l'échelle de pitch de la v1, qui tourne dans le Teensy et
sonne juste. Il n'y a donc **rien à régler pour démarrer** — cette page sert à
confirmer l'accord, et à savoir quoi toucher si un jour ça dérive.

## L'échelle vient de la v1, qui fonctionne

`v1-first-release/kobolDAC.ino` tourne dans le Teensy et sonne juste. Il pose :

```c
noteVoltage = 4000 * (float) note / 120.0;
```

Soit 4000 unités DAC pour 120 demi-tons = **33,3 mV par demi-ton**, donc
**400 mV par octave** (le MCP4822 en gain ×2 sort 1 mV par unité).

Ce n'est pas du 1 V/octave, malgré ce qu'annonce le README de la v1. L'entrée
visée est plus sensible d'un facteur 2,5. Le nouveau firmware reprend donc
`cal_mv_per_octave = 400` et `CAL_BASE_NOTE = 0`, ce qui **reproduit la v1 au
millivolt près sur toute la plage MIDI** — vérifié note par note de 0 à 120.

> Ne pas « corriger » cette valeur vers 1000 au motif que l'entrée serait en
> 1 V/oct. C'est une déduction, pas une mesure, et elle est contredite par le
> fait que la v1 sonne juste.

La procédure ci-dessous ne sert donc qu'à **confirmer** l'accord, ou à le
rattraper si ton exemplaire diffère.

## Procédure

Il faut un accordeur (ou un accordeur logiciel dans le DAW) et un clavier MIDI.

### 1. Vérifier le repos

Au démarrage, le firmware met le canal A à 0 mV et n'écrit sur aucun autre
paramètre tant qu'un CC n'est pas arrivé. Le Kobol doit sonner **exactement**
comme carte débranchée. Si un réglage saute au branchement, c'est que le CV
n'est pas au repos — vérifier le câblage avant d'aller plus loin.

### 2. Mesurer l'octave réelle

Joue **Do3 (note 60)**, relève la fréquence. Joue **Do4 (note 72)**, relève.

- Rapport **2,0** → `cal_mv_per_octave` est juste.
- Rapport **plus petit** → la constante est trop petite.
- Rapport **plus grand** → trop grande.

Le facteur correctif se lit directement :

```
cal_mv_per_octave_correct = cal_mv_per_octave ÷ log2(f_haut ÷ f_bas)
```

Exemple : Do1 → 65 Hz, Do2 → 86 Hz. Le rapport vaut 1,32, soit log2(1,32) =
0,40 octave au lieu de 1. Il faut donc diviser 400 par 0,40 → **1000 mV/octave**.

Refaire la mesure après correction : deux itérations suffisent en général.

### 3. Vérifier l'étendue

Le DAC sort 0 à 4096 mV. L'étendue jouable vaut donc
`4096 ÷ cal_mv_per_octave` octaves à partir de `CAL_BASE_NOTE`.

| `cal_mv_per_octave` | étendue | depuis la note 0 |
|---|---|---|
| 400 (v1, actuel) | 10,2 octaves | toute la plage MIDI |
| 1000 | 4,1 octaves | jusqu'à Do5 |

Si l'étendue est trop courte, deux leviers : monter `CAL_BASE_NOTE` pour
recentrer la plage sur ce que tu joues, ou ajouter un étage d'amplification
derrière le DAC.

### 4. Résolution

Le MCP4822 est en 12 bits sur 4096 mV, soit **1 mV par pas**. À 400 mV/octave,
un demi-ton fait 33,3 mV, donc **1 pas DAC = 3 cents**.

| `cal_mv_per_octave` | mV par demi-ton | pas DAC par demi-ton | erreur max |
|---|---|---|---|
| 400 (v1, actuel) | 33,3 | 33 | 1,5 cent |
| 1000 | 83,3 | 83 | 0,6 cent |

1,5 cent d'erreur d'arrondi est inaudible — l'instabilité analogique du VCO sera
d'un tout autre ordre. La résolution n'est pas un sujet ici.

En revanche l'étendue l'est : à 400 mV/octave, les 4096 mV du DAC couvrent
**10 octaves**, soit toute la plage MIDI. C'est confortable, mais cela veut dire
qu'un pas DAC de travers s'entend trois fois plus qu'à 1 V/oct.

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

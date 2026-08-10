# kobol-expander / midi-cv

Interface MIDI → CV pour RSF Kobol Expander. Dossier de travail actif.

Reprend [`../v1-first-release/`](../v1-first-release/), qui reste intouché comme
référence fonctionnelle (Teensy 2.0, 4 paramètres), en changeant de MCU et en
passant à 12 paramètres.

## État

| | |
|---|---|
| **Carte MIDI** | [`MIDI_MAP.md`](MIDI_MAP.md) — 23 CC + le pitch |
| **Calibration** | [`CALIBRATION.md`](CALIBRATION.md) — **à faire avant de jouer** |
| **Firmware** | `KobolMidiCV/` — écrit, jamais compilé ni testé sur cible |
| **Matériel** | ESP32-S3 + LM324, carte non réalisée |

## Matériel visé

ESP32-S3 VROOM N16R8, d'après [`../v2-simple/README.md`](../v2-simple/README.md)
(avril 2026), qui est la décision la plus récente. Les documents de `../docs/`
datent de septembre 2025 et parlent encore de Teensy : ils sont dépassés sur ce
point.

```
ESP32-S3 ─ PWM 14 bits ─ RC 1kΩ/330nF ─ LM324 (gain + offset) ─ P1 du Kobol
         └ GPIO gate ────────────────────────────────────────── jack façade
```

## Structure du firmware

| Fichier | Rôle |
|---|---|
| `KobolMidiCV.ino` | MIDI, pile de notes, portamento, modulations |
| `config.h` | brochage, calibration, **table des paramètres** |
| `output.h` / `.cpp` | couche de sortie CV, isole le choix PWM/DAC |

Tout ce qui se règle est dans `config.h`. La table `PARAMS[]` porte, pour chaque
paramètre : son CC, sa pin P1, son GPIO, sa plage en millivolts et son état
(`PARAM_OK`, `PARAM_CHECK`, `PARAM_BLOCKED`). Ajouter un paramètre = ajouter une
ligne.

`output.cpp` est le seul fichier à toucher si le PWM laisse place à un DAC
externe.

## Ce qui reste à faire

### Avant de brancher quoi que ce soit

1. **Trancher la numérotation du connecteur.** `Plug.txt` et
   `connector_analysis.md` ne décrivent pas le même ordre de pins — le premier
   parle de P1 *et* P7, le second nomme « P1 » ce qui correspond ligne pour
   ligne au P7 du premier. Une erreur ici décale tout le connecteur et envoie un
   CV sur le rail d'alimentation. Détail en §6 de [`MIDI_MAP.md`](MIDI_MAP.md).
2. **Ne jamais câbler la pin 2** : c'est le rail −14,5 V, mesuré.

### Avant de jouer

3. **Calibrer le pitch.** Le nombre de millivolts par octave n'a jamais été
   mesuré, et les deux valeurs plausibles diffèrent d'un facteur 2. Procédure
   dans [`CALIBRATION.md`](CALIBRATION.md).

### Pour compléter

4. **Test d'injection sur les pins 3 et 6** (waveforms). La même manipulation
   qu'en pin 11 — une pile variable et l'oreille, pas besoin d'oscilloscope.
5. **Trouver la vraie pin du LFO Rate.** Ce n'est pas la pin 2.
6. **Quatre sorties sur douze n'ont pas de canal PWM** : le S3 n'en offre que 8.
   Voir §6 de [`MIDI_MAP.md`](MIDI_MAP.md).

## Dépendances

Non versionnées, à installer via le gestionnaire de bibliothèques :

- **Adafruit TinyUSB Library** — USB MIDI natif sur ESP32-S3
- **MIDI Library** (FortySevenEffects)

Dans l'IDE : *Outils > USB Mode > USB-OTG (TinyUSB)*.

Le sous-dossier `KobolMidiCV/` porte le nom du sketch : l'IDE Arduino exige que
le dossier et le `.ino` soient homonymes.

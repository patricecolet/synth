# kobol-expander / midi-cv

Interface MIDI → CV pour RSF Kobol Expander, plus l'outillage autour :
carte MIDI partagée, futur VST, futur séquenceur.

## Cible

**Teensy 2.0 + MCP4822**, câblage repris de
[`../v1-first-release/`](../v1-first-release/), qui est monté et fonctionne. La
v1 reste intouchée comme référence.

L'ESP32-S3 de [`../v2-simple/`](../v2-simple/) reste la cible d'évolution — il
apporterait 12 sorties au lieu de 2 — mais ce n'est pas ce qui est câblé
aujourd'hui.

```
DAW / VST ─ USB MIDI ─ Teensy 2.0 ─ SPI ─ MCP4822 ─┬─ canal A ─ pitch
                            │                      └─ canal B ─ cutoff (CC 74)
                            ├─ pin 6  ─ gate
                            └─ pin 10 ─ PWM LFO rate
```

## État

| | |
|---|---|
| **Carte MIDI, lisible** | [`midi-map.json`](midi-map.json) — source unique firmware + VST |
| **Carte MIDI, lue** | [`MIDI_MAP.md`](MIDI_MAP.md) — 23 CC + le pitch |
| **Cohérence** | `python3 tools/check_map.py` |
| **Calibration** | [`CALIBRATION.md`](CALIBRATION.md) — échelle reprise de la v1, rien à régler |
| **Firmware** | `KobolMidiCV/` — écrit, jamais compilé ni testé sur cible |
| **VST** | pas commencé |
| **Séquenceur** | pas commencé |

## Ce que le câblage actuel permet

Deux sorties CV seulement, puisque le MCP4822 a deux canaux :

- **Pitch** sur le canal A, piloté par les notes, le pitch bend et le portamento
- **Cutoff du filtre** sur le canal B, piloté par **CC 74**

Plus le gate et le PWM du LFO rate. Les douze autres paramètres sont décrits
dans la table mais sans sortie : il faudra un second MCP4822 sur un autre CS,
ou passer à l'ESP32.

## Structure

| Fichier | Rôle |
|---|---|
| `KobolMidiCV.ino` | MIDI, pile de notes, portamento, modulations |
| `config.h` | brochage, calibration, **table des paramètres** |
| `output.h` / `.cpp` | couche de sortie, isole le MCP4822 |
| `midi-map.json` | la même carte, pour le VST |
| `tools/check_map.py` | vérifie que les deux ne divergent pas |

Tout ce qui se règle est dans `config.h`. Ajouter un paramètre = une ligne dans
la table **et** une entrée dans le JSON ; `check_map.py` refuse si l'un des deux
manque.

Le firmware est en **arithmétique entière** : l'ATmega32U4 n'a pas de FPU et
émule le flottant en logiciel, à une dizaine de microsecondes l'opération.

## Ce qui reste à faire

### D'abord

1. **Compiler et téléverser**, puis vérifier que le pitch sonne comme avec la
   v1 : l'échelle en est reprise à l'identique, vérifiée au millivolt près sur
   toute la plage MIDI. Puis contrôler que CC 74 pilote bien le cutoff.

### Si un jour on branche le connecteur P1

2. **Trancher la numérotation.** `Plug.txt` et `connector_analysis.md` ne
   décrivent pas le même ordre de pins. §6 de [`MIDI_MAP.md`](MIDI_MAP.md).
3. **Ne jamais câbler la pin 2** : rail −14,5 V, mesuré.
4. **Monter l'étage d'adaptation** avant de passer `KOBOL_OUTPUT_JACK` à 0.
   Le P1 n'accepte que ±1,5 V, le DAC en sort 4.

### Pour compléter

5. **Test d'injection sur les pins 3 et 6** (waveforms) — une pile et l'oreille
   suffisent, comme pour la pin 11.
6. **Trouver la vraie pin du LFO Rate.** Ce n'est pas la pin 2.

## Dépendances

Aucune bibliothèque externe : `usbMIDI` et `SPI` viennent de Teensyduino.
Dans l'IDE : *Outils > USB Type > MIDI*.

Le sous-dossier `KobolMidiCV/` porte le nom du sketch : l'IDE Arduino exige que
le dossier et le `.ino` soient homonymes.

# kobol-expander / midi-cv

Interface MIDI → CV pour RSF Kobol Expander. Dossier de travail actif.

Reprend [`../v1-first-release/`](../v1-first-release/), qui reste intouché comme
référence fonctionnelle (4 paramètres : pitch, gate, cutoff par vélocité, LFO
rate), pour aller vers le contrôle des 16 paramètres du connecteur CV.

## État

| | |
|---|---|
| **Carte MIDI** | [`MIDI_MAP.md`](MIDI_MAP.md) — 24 contrôleurs définis |
| **Firmware** | `KobolMidiCV/` — pas encore écrit |
| **Matériel** | Teensy 2.0 + MCP4822 (repris de v1), adaptation de niveau à concevoir |

## Avant d'écrire du code

La carte MIDI est posée, mais **6 des 20 paramètres câblés sont bloqués** par
des contradictions entre le schéma relevé et les mesures — dont le pitch des
deux VCO, sans quoi il n'y a pas d'interface MIDI utile. Le détail est en
§6 de [`MIDI_MAP.md`](MIDI_MAP.md). Les trois points à trancher, par ordre
d'urgence :

1. **La convention de numérotation du connecteur.** `Plug.txt` et
   `connector_analysis.md` ne décrivent pas le même ordre de pins. Une erreur
   ici décale tout le connecteur.
2. **La pin 2 est le rail −14,5 V**, pas LFO Rate comme annoncé. Y brancher un
   DAC le détruirait.
3. **Pins 3, 6, 11, 13 à 0 V constant** — les 4 paramètres VCO. Entrées CV
   haute impédance, ou affectation fausse ?

Les 1 et 2 se règlent au multimètre. Le 3 demande l'oscilloscope mentionné dans
[`../docs/connector_analysis.md`](../docs/connector_analysis.md).

## Convention de nommage

Le sous-dossier `KobolMidiCV/` porte le nom du sketch qu'il contient :
l'IDE Arduino exige que le dossier et le `.ino` soient homonymes.

## Dépendances

Non versionnées, à installer via le gestionnaire de bibliothèques :

- **Teensyduino** — pour `usbMIDI` (l'API n'existe pas sur Arduino standard)
- **`Ramp.h`** — interpolation du portamento
- **`MCP48xx`** — pilotage du DAC

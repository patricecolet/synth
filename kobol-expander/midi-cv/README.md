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
| **Cohérence** | `tools/check_map.py` (firmware) et `tools/gen_device_profile.py` (plugin) |
| **Calibration** | [`CALIBRATION.md`](CALIBRATION.md) — échelle reprise de la v1, rien à régler |
| **Firmware** | `KobolMidiCV/` — **téléversé** sur le Teensy (22 % flash, 20 % RAM) |
| **VST** | profil Kobol dans NiDMI Seq — [`INTEGRATION.md`](INTEGRATION.md) |
| **Séquenceur** | pas commencé |

## Ce que le câblage actuel permet

Deux sorties CV seulement, puisque le MCP4822 a deux canaux :

- **Pitch** sur le canal A, piloté par les notes, le pitch bend et le portamento
- **Cutoff du filtre** sur le canal B, piloté par **CC 74**

Plus le gate et le PWM du LFO rate. Les douze autres paramètres sont décrits
dans la table mais sans sortie : il faudra un second MCP4822 sur un autre CS,
ou passer à l'ESP32.

## Réglage par défaut au démarrage

Le firmware applique le `cc_default` de chaque paramètre dès la mise sous
tension, plus le pitch à sa note de base et le LFO à 0. La carte part donc d'un
état connu sans attendre qu'un contrôleur bouge.

Conséquence à connaître : le CV **s'additionne** au potard de façade. Ces
valeurs décalent donc le réglage du Kobol au branchement. Pour qu'un paramètre
reparte de la façade, mettre son `cc_default` à la valeur dont `ccToMv()` tire
0 mV — soit 0 en sortie jack.

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

1. **Téléverser**, puis vérifier que le pitch sonne comme avec la v1 : l'échelle
   en est reprise à l'identique, vérifiée au millivolt près sur toute la plage
   MIDI. Puis contrôler que CC 74 pilote bien le cutoff.

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

### Le nom reste « Teensy MIDI » après un renommage

Le nom affiché ne vient **pas** du descripteur USB mais d'un cache CoreMIDI
indexé sur l'`uniqueID` du périphérique, dans
`~/Library/Preferences/ByHost/com.apple.MIDI.<UUID>.plist`. Changer
`STR_PRODUCT` et téléverser ne suffit donc pas : le système continue d'afficher
l'ancien nom.

Pour le rafraîchir — **coupe brièvement le MIDI de toutes les applications** :

```sh
killall MIDIServer
# puis retirer l'entrée obsolète du plist ByHost (repérée par son uniqueID),
# et rouvrir n'importe quel client MIDI pour que MIDIServer reparte
```

Vérifier avec `swift tools/miditest.swift`, qui liste les ports quand il ne
trouve pas celui qu'il cherche.

### Pas de port série

En USB Type = MIDI, le Teensy 2.0 n'expose **aucun port série USB** : ses
options USB sont exclusives, il n'y a pas de « MIDI + Serial » comme sur les
Teensy 3.x et 4.x. Les `Serial.print` du firmware ne sortent donc nulle part.

Pour observer ce qu'il fait, lui envoyer du MIDI et écouter —
[`tools/miditest.swift`](tools/miditest.swift) le fait sans DAW :

```sh
swift tools/miditest.swift              # liste les ports
swift tools/miditest.swift sweep 74     # balaye le cutoff
swift tools/miditest.swift scale        # gamme chromatique Do3 -> Do4
swift tools/miditest.swift cc 118 127   # force le gate ouvert
```

Deux CC distinguent ce firmware de la v1, qui les ignore : **CC 74** (cutoff)
et **CC 118** (gate force, la v1 utilisait CC 100).

### Compiler et téléverser

Passer par [`build.sh`](build.sh), pas par `arduino-cli` directement :

```sh
./build.sh              # compile
./build.sh upload       # compile, lance Teensy Loader, téléverse
./build.sh clean        # recompile tout
```

Le script existe pour deux raisons qu'un appel nu ne couvre pas.

**Le nom du port MIDI.** Le système affiche `Kobol` et non `Teensy MIDI`.
Attention, macOS met ce nom en cache : après le premier changement il faut
vider l'entrée obsolète, voir plus bas. Ce
nom vient de `STR_PRODUCT`, défini dans le core Teensy
(`cores/usb_midi/usb_private.h`) sous `#ifndef`. Ce fichier est compilé **avec
le core**, pas avec notre code : un `#define` dans `config.h` arriverait trop
tard, il faut un `-D` en ligne de commande. Et il ne peut pas passer par
`compiler.*.extra_flags`, que la plateforme Teensy n'utilise nulle part dans ses
recettes — le seul point d'entrée est `build.flags.defs`, qu'il faut donc
recopier en entier. D'où `CORE_DEFS` dans le script, **à revérifier après une
mise à jour du core** :

```sh
grep '^teensy2.build.flags.defs' ~/Library/Arduino15/packages/teensy/hardware/avr/*/boards.txt
```

**Teensy Loader.** Le core délègue le téléversement à cette application ; si
elle ne tourne pas, l'upload échoue sur *« Unable find Teensy Loader »*. Le
script la lance au besoin.

L'option `usb=midi` du FQBN n'est pas facultative : sans elle `usbMIDI`
n'existe pas et la compilation échoue.

Le sous-dossier `KobolMidiCV/` porte le nom du sketch : l'IDE Arduino exige que
le dossier et le `.ino` soient homonymes.

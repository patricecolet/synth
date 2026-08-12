# Lien avec NiDMI Seq

Le Kobol se pilote depuis **NiDMI Seq**, le séquenceur VST maison. Ce document
dit qui produit quoi, où passe la frontière entre les dépôts, et comment les
deux restent d'accord.

## Pourquoi pas un plugin dédié

NiDMI Seq savait déjà tout faire côté séquencement : son moteur
(`nidmi-sequencer-core`) a des rows d'automation CC (`RowKind::CC`, avec
`ccNumber` par défaut à 74 — le cutoff), des P-locks CC par pas façon Elektron
(`StepData.ccLocks[8]`) et des macros à 8 destinations.

Il manquait une seule chose : **il parlait en numéros de CC, pas en noms de
paramètres**. Écrire un second plugin aurait dupliqué le moteur pour combler ce
seul écart. On a donc ajouté la notion de profil d'appareil.

## Qui fait quoi

```
┌─ dépôt synth (celui-ci) ──────────────────────────────────────┐
│                                                               │
│   midi-map.json          ← FAIT FOI                           │
│        │                                                      │
│        ├── KobolMidiCV/  firmware Teensy, table PARAMS[]      │
│        │                 vérifié par tools/check_map.py       │
│        │                                                      │
│        └── tools/gen_device_profile.py                        │
│                 │  génère                                     │
└─────────────────┼─────────────────────────────────────────────┘
                  ▼
┌─ dépôt nidmi-seq-vst ─────────────────────────────────────────┐
│   Source/DeviceProfile.cpp   bloc entre marqueurs             │
│   Source/DeviceProfile.h     registre des profils             │
│        │                                                      │
│        └── affichage : rows CC, P-locks, encodeurs            │
└───────────────────────────────────────────────────────────────┘
                  │  MIDI USB
                  ▼
        port « Kobol » → Teensy 2.0 → MCP4822 → RSF Kobol Expander
```

`nidmi-sequencer-core` n'est pas touché : il ne connaît pas le Kobol, il émet
des CC. Le profil est purement cosmétique et ne modifie aucun message MIDI.

## La frontière

| | |
|---|---|
| **Ce dépôt possède** | la carte MIDI, le firmware, les mesures électriques, la calibration |
| **NiDMI Seq possède** | le moteur de séquencement, l'UI, le rendu des noms |
| **Ce qui traverse** | `midi-map.json` → bloc généré dans `DeviceProfile.cpp` |
| **Ce qui ne traverse jamais** | les plages en millivolts, les pins P1, les états de mesure. Le plugin n'a aucune raison de les connaître : il envoie des CC 0-127, le firmware fait la conversion. |

## Rester d'accord

La carte a **trois** consommateurs — le firmware, ce dépôt, le plugin — et rien
ne les forçait à s'accorder. En deux jours, cinq divergences étaient déjà
apparues entre `midi-map.json` et `DeviceProfile.cpp` :

- trois noms différents (`Velocity to VCA` contre `Velocite > VCA`) ;
- **LFO Rate marqué non câblé** alors que le firmware le sort en PWM ;
- **Mod Wheel marqué câblé** alors qu'il est explicitement non implémenté.

Les deux derniers sont le genre d'erreur qui coûte une soirée : l'écran affirme
qu'un paramètre est actif, on tourne l'encodeur, rien ne bouge, on soupçonne le
câblage.

D'où deux vérifications, à lancer depuis `kobol-expander/midi-cv/` :

```sh
python3 tools/check_map.py            # JSON  <-> firmware  (PARAMS[])
python3 tools/gen_device_profile.py   # JSON  <-> plugin    (DeviceProfile.cpp)
```

Les deux sortent en 0 si tout concorde, en 1 sinon. Le second se corrige seul :

```sh
python3 tools/gen_device_profile.py --write   # puis recompiler le plugin
```

Le générateur ne réécrit que le bloc entre `// >>> GENERE DEPUIS midi-map.json`
et `// <<< FIN DU BLOC GENERE`. Le reste de `DeviceProfile.cpp` — le registre,
les accesseurs — reste écrit à la main.

## Emplacement du dépôt du plugin

Les trois dépôts sont voisins dans `~/repo/` : `synth`, `nidmi-seq-vst`,
`nidmi-sequencer-core`. `gen_device_profile.py` cherche donc
`~/repo/nidmi-seq-vst`, surchargeable :

```sh
NIDMI_SEQ_VST=/ailleurs/nidmi-seq-vst python3 tools/gen_device_profile.py
```

Si le dépôt est absent, le script le signale et sort en 0 : ce dépôt-ci doit
rester utilisable sans le plugin.

## Le champ `wired`

Il ne veut pas dire « décrit dans la table » mais **« a un effet audible
aujourd'hui »**. Le matériel actuel est un Teensy 2.0 avec un seul MCP4822, donc
deux canaux : le pitch et le cutoff.

Conséquence non évidente : `CC 116` (vélocité → VCA) est `false`, parce qu'il
module le VCA Sustain, qui n'a pas de sortie. Le CC part bien, il ne produit
rien.

Sur 21 contrôleurs, **7 sont audibles** : cutoff, LFO rate, portamento (temps et
commutateur), les deux vélocité → cutoff, et le gate forcé. Plus le pitch, qui
n'a pas de CC puisqu'il suit les notes.

La donnée est dans le profil mais **pas encore exploitée à l'écran**. Griser les
paramètres sans effet serait le prochain pas utile côté plugin.

## Utiliser l'ensemble

1. Téléverser le firmware : `./build.sh upload`
2. Ouvrir NiDMI Seq, page **GLOBAL** → **Profil** → *Kobol Expander*
3. Sortie MIDI du plugin vers le port **`Kobol`**
4. Une row en `kind=Note` pour le pitch et le gate, une row en `kind=CC` sur
   **CC 74** pour le cutoff

Sans DAW, [`tools/miditest.swift`](tools/miditest.swift) envoie notes et CC
directement.

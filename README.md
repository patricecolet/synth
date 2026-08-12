# synth

Atelier synthé analogique : conception de cartes (KiCad), firmwares Arduino,
interfaces Pure Data et documentation de référence.

Le dépôt regroupe plusieurs projets indépendants. Chacun est autonome —
schémas, firmware et patchs vivent dans le même dossier.

## Projets

| Dossier | Quoi |
|---|---|
| [`PS-101/`](PS-101/) | Le synthé principal : carte mère, VCO, VCF, mixeur d'ondes, firmware et UI Pd |
| [`kobol-expander/`](kobol-expander/) | **Interface MIDI → CV pour RSF Kobol Expander.** Firmware Teensy qui tourne, piloté depuis le séquenceur NiDMI Seq |
| [`sequencer/`](sequencer/) | Séquenceur à base de 74HC595 (KiCad, 2018) |
| [`alimentation/`](alimentation/) | Alimentation symétrique LM723 / LM2596 + masse virtuelle |
| [`xva1/`](xva1/) | Patchs Pd de contrôle pour le moteur XVA1 |
| [`experiments/`](experiments/) | Bancs d'essai : DAC MCP4822 / MCP4921, driver TLC5940, LFO |
| [`reference/`](reference/) | Datasheets et schémas glanés — matière première, **non versionnée** (voir ci-dessous) |

## Détail

### PS-101

Le projet le plus abouti. Un VCO numérique (tables d'ondes en ROM → DAC MCP4822)
piloté par Arduino, avec un chemin analogique classique derrière.

```
PS-101/
├── hardware/          # KiCad : carte mère + sous-cartes
│   ├── PS-101.sch/.kicad_pcb   carte principale
│   ├── vco/           VCO (PS-01_VCO)
│   ├── vcf/           filtre Pro-One (AS3320)
│   ├── vco_mixer/     mixeur d'ondes
│   ├── synth.pretty/  empreintes maison
│   └── trace/         sorties PDF
├── firmware/          # sketches Arduino, du plus ancien au plus récent
│   ├── PS-01/         194 l. — 3× MCP4822, 9 formes d'onde, LFO
│   ├── PS01_TEST/     334 l.
│   ├── PS01_TEST2/    376 l. — 2× MCP4822, 5 ondes, mode DEBUG
│   └── EEPROM/        253 l. — persistance des paramètres
└── pd/                # interface Pure Data
    ├── PS01ui.pd      UI principale
    └── PS01_param.pd  abstraction d'un paramètre (utilisée par PS01ui)
```

Les tables d'ondes (`wave/*.h`) sont volontairement dupliquées dans chaque
sketch : l'IDE Arduino exige qu'un sketch soit autonome.

### kobol-expander

Le projet le plus actif. Un Teensy 2.0 reçoit du MIDI USB et sort du CV vers
le Kobol via un MCP4822 — pitch sur le canal A, cutoff du filtre sur le canal B.

```
kobol-expander/
├── midi-cv/            # le travail actif
│   ├── KobolMidiCV/    firmware Teensy (compile, téléversé)
│   ├── midi-map.json   LA carte MIDI — fait foi pour tout le reste
│   ├── build.sh        compile + téléverse, pose le nom USB « Kobol »
│   ├── tools/          vérificateurs de cohérence, test MIDI sans DAW
│   ├── INTEGRATION.md  lien avec le séquenceur NiDMI Seq
│   └── CALIBRATION.md  échelle du pitch, mesurée
├── v1-first-release/   firmware d'origine, référence intouchée
├── v2-simple/          projet KiCad kobol_V2
├── v3/v4/v5            documentation de vision
└── docs/               mesures électriques du connecteur P1
```

**Le pilotage se fait depuis NiDMI Seq**, le séquenceur VST maison (dépôt
voisin, voir plus bas), auquel on a ajouté un profil d'appareil pour qu'il
affiche « VCF Cutoff » au lieu de « CC 74 ». Le détail, la frontière entre les
deux dépôts et les garde-fous contre la dérive sont dans
[`kobol-expander/midi-cv/INTEGRATION.md`](kobol-expander/midi-cv/INTEGRATION.md).

Le brochage du connecteur P1 du Kobol est relevé dans
[`reference/kobol/Plug.txt`](reference/kobol/Plug.txt).

### xva1

`XVA1.pd` et `comUI.pd` sont deux états du même patch (seules diffèrent des
valeurs de sliders sauvegardées). `paramUI.pd` est l'abstraction qu'ils
instancient.

## Documentation tierce

Le dépôt ne contient **que du travail original**. Les datasheets constructeur,
manuels de service et schémas de synthés commerciaux ne sont pas
redistribuables : ils restent sur le disque local mais sont exclus du suivi
git. Concrètement, un clone trouvera vides :

- `reference/datasheets/`, `reference/kobol/`, `reference/analog-synth/`
- les schémas et PDF de `alimentation/`

Chaque README de dossier liste ce qu'il faut retélécharger et auprès de qui —
voir [`reference/README.md`](reference/README.md) et
[`alimentation/README.md`](alimentation/README.md). Mes propres relevés
(`Plug.txt`, `missing.txt`, `CONSOMATION.txt`, `goldpoint_capa.txt`) sont bien
versionnés, eux.

**L'historique a été purgé** avant la publication : `git filter-repo` a retiré
ces 104 fichiers de **tous** les commits, pas seulement de la dernière version.
Le dépôt est passé de 31 Mo à 2,3 Mo. Un commit qui ne faisait que restaurer un
de ces PDF est devenu vide et a disparu.

Mes propres relevés ont été explicitement conservés à travers la réécriture :
`Plug.txt`, `missing.txt`, `CONSOMATION.txt`, `goldpoint_capa.txt`. Les PDF
sortis de mon propre KiCad (`PS-101/hardware/trace/`, `*_schema.pdf`) aussi.

Les fichiers écartés restent **sur le disque local**, seulement plus dans git.

## Dépôts voisins

Ce dépôt ne contient pas tout l'écosystème. Deux voisins comptent :

| Dépôt | Rôle |
|---|---|
| `~/repo/nidmi-seq-vst` | **NiDMI Seq**, séquenceur VST3/AU/Standalone. Porte le profil Kobol |
| `~/repo/nidmi-sequencer-core` | moteur de séquencement, sans lien avec le Kobol |

Le seul fichier qui traverse la frontière est `midi-map.json`, d'où est
**généré** le profil du plugin. Voir
[`kobol-expander/midi-cv/INTEGRATION.md`](kobol-expander/midi-cv/INTEGRATION.md).

## Conventions

- **Pas de chemin absolu** dans les patchs Pd (`declare -path` en nom nu).
- Les sauvegardes KiCad (`*.bak`, `*-bak`, `*-backups/`) et les caches
  (`fp-info-cache`, `*-cache.lib`) sont **suivis volontairement** : ils
  permettent de rouvrir un projet ancien avec les bonnes empreintes.
  Voir les notes en bas de [`.gitignore`](.gitignore).
- Les BOM exportés depuis KiCad portent le suffixe `.bom.csv`.

## Historique

Le commit initial (`Snapshot initial`) capture le dossier **avant** toute
réorganisation. Tout fichier écarté depuis reste récupérable :

```sh
git show <commit>:<ancien/chemin>          # afficher
git checkout <commit> -- <ancien/chemin>   # restaurer
```

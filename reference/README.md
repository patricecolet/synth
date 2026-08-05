# reference

Documentation externe : datasheets, schémas de synthés existants, notes de
mesure. Rien ici n'est de la conception maison — c'est la matière première.

> **Le contenu de ce dossier n'est pas versionné.** Datasheets constructeur,
> manuels de service et schémas de synthés commerciaux ne sont pas
> redistribuables ; ils restent sur le disque local mais sont exclus par le
> [`.gitignore`](../.gitignore). Un clone du dépôt trouvera ces dossiers vides —
> la liste ci-dessous dit quoi retélécharger et auprès de qui.
>
> Seules exceptions, versionnées car ce sont mes propres relevés :
> [`kobol/Plug.txt`](kobol/Plug.txt) et
> [`analog-synth/missing.txt`](analog-synth/missing.txt).

## datasheets/

| Composant | Rôle dans les projets | Source |
|---|---|---|
| AS3310 | Enveloppe ADSR | Alfa RPAR — `alfarzpp.lv` |
| AS3320 | VCF du `PS-101/hardware/vcf/` (clone Pro-One) | Alfa RPAR |
| AS3330 | VCA | Alfa RPAR |
| AS3340 (+ note de tuning) | VCO | Alfa RPAR |
| LM13700 | OTA du `vco_mixer/` et des VCF | Texas Instruments |
| TL07x | Ampli-op générique | Texas Instruments |
| VCA2164 | VCA quadruple | Coolaudio |
| AD5628 / 5648 / 5668 | DAC 8 canaux, piste envisagée | Analog Devices |

Les datasheets constructeur se retrouvent en quelques secondes par référence.
Le fichier `vcf_pro-one.bom.csv` de `PS-101/hardware/vcf/` porte d'ailleurs
l'URL d'origine de l'AS3320 dans sa colonne *Datasheet*.

## kobol/

Schémas d'origine du **RSF Kobol Expander** (marque française, années 80) :
cartes VCO, embase, noise-LFO, et `schemaKobolExpander.jpg`. Circulent sur les
forums de synthés vintage.

**`Plug.txt` est versionné** : c'est mon relevé du brochage du connecteur P1,
base de tout le projet [`kobol-expander/`](../kobol-expander/). Les points
d'interrogation qu'il contient sont des broches non encore identifiées.

## analog-synth/

Schémas de référence relevés sur des machines existantes — servent de modèle
pour les blocs du PS-101 :

- **Korg MS-20** — filtre (`MS20_vcf_920.gif`, `MS20_VCF_CLONE.png`, `MS20Clone2.gif`)
- **Roland SH-101** — manuel de service complet, sub-osc, VCO CEM3340
- **Sequential Pro-One** — filtre CEM3320, repris dans `PS-101/hardware/vcf/`
- divers — wavefolder JH, VCA, générateur de bruit, hard sync, state-variable
  filter, CA3080, VCO no-tempco

**`missing.txt` est versionné** : liste de valeurs de résistances qui me
manquaient lors d'un montage. À recouper avec le schéma concerné avant
commande.

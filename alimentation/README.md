# alimentation

Étude de l'alimentation du synthé : rail symétrique et masse virtuelle.

> **Les schémas et datasheets de ce dossier ne sont pas versionnés** — ce sont
> des documents tiers. Ils restent sur le disque local, exclus par le
> [`.gitignore`](../.gitignore). Seules mes notes de mesure sont dans le dépôt.
>
> La règle d'exclusion porte sur les extensions (`*.pdf *.png *.gif *.jpg`) :
> un schéma d'alimentation de ma main devra être ajouté avec `git add -f`.

## Mes notes (versionnées)

- [`CONSOMATION.txt`](CONSOMATION.txt) — budget de consommation relevé par module
- [`goldpoint_capa.txt`](goldpoint_capa.txt) — références Panasonic des
  condensateurs de la masse virtuelle Goldpoint

## Documents de référence (locaux uniquement)

| Fichier | Quoi | Source |
|---|---|---|
| `ALIM_schematic_v1.2.png` | Bipolar PSU for modular synthesizers, v1.2 (2014) | Dmitry Shtatnov — `modularsynth.ru` |
| `Alim_723_V2.2.pdf`, `Schema_alimentation_variable_LM723.gif`, `LM723-Voltage-Regulator-Circuit-diagram.png` | Alimentation variable à LM723 | schémas de forums |
| `lm2596.pdf` | Datasheet du convertisseur à découpage | Texas Instruments |
| `electronique_alim_symetrique_001.gif` | Principe du rail symétrique | schéma de forum |
| `virtualGround.jpg`, `virtual ground goldpoint vg1 02.jpg`, `Bias2.gif` | Masse virtuelle, approche Goldpoint VG1 | Goldpoint Level Controls |

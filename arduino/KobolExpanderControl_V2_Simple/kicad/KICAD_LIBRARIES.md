# Librairies KiCad à installer – Kobol Expander V2

Ce projet utilise des composants pour lesquels il faut ajouter des librairies KiCad (symboles et emplacements). Voici quoi installer et comment.

---

## 1. ESP32-S3 DevKitC-1 (obligatoire)

Le **ESP32-S3-DevKitC-1** est une carte de développement ; Espressif fournit une librairie KiCad officielle avec symbole et empreinte (footprint).

### Installation via le Plug-in and Content Manager (PCM)

1. **Ouvrir KiCad** (fenêtre principale du projet).
2. **Ouvrir le PCM** : menu **Préférences** → **Gestionnaire de contenu et de plugins** (ou **Plugin and Content Manager**).
3. **Installer la librairie Espressif** :
   - Télécharger le fichier **espressif-kicad-addon.zip** depuis la dernière release :  
     [Releases Espressif KiCad Libraries](https://github.com/espressif/kicad-libraries/releases)  
     (prendre le ZIP prévu pour KiCad 8, ou la version “legacy” si vous êtes en KiCad 6/7).
   - Dans le PCM : **Install from file…** (Installer depuis un fichier…) et sélectionner ce ZIP.  
     **Ne pas dézipper** le fichier : KiCad attend le ZIP.
   - Sur **macOS avec Safari** : désactiver l’option “Ouvrir les fichiers sûrs après téléchargement” pour éviter que Safari ne dézippe automatiquement le fichier.
4. Après installation, la librairie apparaît dans l’onglet **Installed** du PCM. Elle est utilisable sous le nom **PCM_Espressif** (symboles et emplacements).

### Utilisation dans le schéma

- **Symbole** : dans Eeschema, **Ajouter un symbole** (A), chercher **ESP32-S3-DevKitC** (ou **ESP32-S3-DevKitC-1** selon le nom exact dans la librairie). Le symbole se trouve dans la librairie **PCM_Espressif**.
- **Emplacement (footprint)** : après attribution des emplacements, le footprint associé au symbole DevKitC sera proposé (souvent déjà lié dans la librairie Espressif).

### Référence officielle

- [Documentation Espressif KiCad Library](https://documentation.espressif.com/kicad-libraries/main/README.md)  
- Dépôt : [github.com/espressif/kicad-libraries](https://github.com/espressif/kicad-libraries)

---

## 2. Autres composants (souvent déjà dans KiCad)

Les composants suivants sont en général fournis par les librairies standards de KiCad (à vérifier selon votre version) :

| Composant | Librairie typique | Symbole / footprint |
|-----------|-------------------|----------------------|
| Résistances, condensateurs | **Device** | R, C |
| LM324 | **Amplifier_Operational** ou **Linear** | LM324 |
| TLE2426 | **Regulator_Linear** ou librairie Texas Instruments | TLE2426 |
| ICL7660 | **Regulator_Switching** ou **Power** / fabricant | ICL7660 |
| MCP4921 | **Converter_DAC** ou **Microchip** | MCP4921 |
| Connecteurs (headers, USB, jacks) | **Connector** | Selon type (Generic_Connector, USB, etc.) |

Si un symbole ou un footprint manque :

- **Symboles** : PCM → chercher par nom (ex. “LM324”, “MCP4921”, “TL082”) ou ajouter une librairie fabricant (Texas Instruments, Microchip, etc.).
- **Footprints** : idem dans la section Footprints du PCM, ou utiliser des footprints génériques (ex. DIP-14 pour LM324, SOIC-8 pour MCP4921) si la librairie fabricant n’est pas installée.

---

## 3. Récapitulatif

| À installer | Méthode | Usage |
|------------|--------|--------|
| **Espressif KiCad Library** | PCM → Install from file → espressif-kicad-addon.zip | Symbole + footprint **ESP32-S3-DevKitC-1** |
| Autres (R, C, LM324, etc.) | Libs standards KiCad ou PCM selon besoin | Schéma et PCB des 3 cartes |

Une fois la librairie Espressif installée, vous pouvez placer le symbole **ESP32-S3-DevKitC** (ou **ESP32-S3-DevKitC-1**) sur la carte 2 comme indiqué dans **SCHEMATIC_DESIGN.md** (§ 7 et § 10).

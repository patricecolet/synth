# KiCad – Kobol Expander Control V2 Simple

Ce dossier contient le projet KiCad pour le circuit de l’interface MIDI→CV (version simple, non invasive). Le design est prévu en **3 cartes** reliées par **connecteurs** : **Carte 1** = MCU + Alim + DAC (ESP32-S3 DevKitC-1, USB, ICL7660, TLE2426, MCP4921, TL082, Gate) ; **Carte 2** = CV (14 voies PWM→CV, connecteur P1 Kobol) ; **Carte 3** = **option** (switches + multiplexeur pour presets / lecture des potards).

## Contenu

| Fichier / dossier | Description |
|-------------------|-------------|
| **kobol_V2/**     | Projet KiCad (schéma + PCB) – actuellement schéma vide. |
| **SCHEMATIC_DESIGN.md** | Spécification complète : 3 cartes, connecteurs, composants, nets, BOM. |
| **KICAD_LIBRARIES.md** | Librairies à installer (Espressif pour DevKitC-1, etc.) et procédure PCM. |

## Comment utiliser

1. **Ouvrir le projet**  
   - Ouvrir `kobol_V2/kobol_V2.kicad_pro` dans KiCad (ou lancer KiCad et ouvrir ce fichier).

2. **Installer les librairies KiCad**  
   - Suivre **KICAD_LIBRARIES.md** pour installer la librairie **Espressif** (symbole + footprint **ESP32-S3 DevKitC-1**).

3. **Construire le schéma (3 cartes + connecteurs)**  
   - Suivre **SCHEMATIC_DESIGN.md** pour :
     - répartir le circuit en **3 cartes** (Alim, MCU+DAC+Gate, CV P1) et **connecteurs intercartes** ;
     - placer les symboles (alimentation, ICL7660, TLE2426, LM324, MCP4921, TL082, R, C, connecteurs, **ESP32-S3 DevKitC-1**) ;
     - nommer les nets (`+5V`, `-5V`, `GND`, `0V_REF`, `PWM1`…`PWM14`, `CV1`…`CV14`, `PITCH_CV`, `GATE_OUT`, etc.) ;
     - câbler selon les connexions décrites.

4. **Conseils**  
   - Travailler par cartes : Carte 1 (Alim + ESP32 + DAC + Gate) → Carte 2 (14 voies PWM→CV + P1) → Carte 3 option (switches + mux). Utiliser des labels globaux pour les nets entre cartes.  
   - Utiliser des **labels globaux** pour les rails (+5V, -5V, GND, 0V_REF) et les signaux entre blocs.  
   - Pin 2 du connecteur P1 : **ne pas connecter** (danger -14,5 V) ; mettre une pastille NC et une note sur le schéma.

5. **PCB**  
   - Après schéma et attribution des emplacements : **Fichier → Mettre à jour le PCB depuis le schéma**, puis routage dans PCB Editor.

## Référence projet

- Architecture et paramètres P1 : voir le **README principal** du projet (`../README.md`).
- Circuit type : `PWM → R 1kΩ → C 330nF → LM324 → ±1.5V → Pin P1`.

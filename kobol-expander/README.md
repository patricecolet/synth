# KobolExpanderControl - Projet Principal

## Vision du Projet
Interface MIDI USB complète et bidirectionnelle pour RSF Kobol Expander avec système de presets et contrôle total de tous les paramètres.

## Objectifs
- **Contrôle complet** : Tous les paramètres du Kobol via MIDI
- **Système de presets** : Sauvegarde/rappel d'états complets
- **Interface bidirectionnelle** : Lecture + écriture des paramètres
- **Non-invasif** : Utilisation uniquement des connecteurs existants
- **Isolation optique** : Séparation propre analogique/numérique
- **Commercialisable** : Solution plug & play pour tous les Expander

## Architecture Cible

### Matériel
- **MCU** : Teensy 2.0 (prototype) → Teensy 4.x (production)
- **DAC haute résolution** : MCP4822 pour CV critiques
- **PWM 16 canaux** : TLC5940 pour paramètres secondaires
- **Isolation optique** : Séparation galvanique complète
- **Alimentation** : ±18V Kobol → régulateurs isolés

### Fonctionnalités
- **16+ paramètres contrôlés** simultanément
- **Presets** : Capture automatique positions potentiomètres
- **Morphing** : Transitions douces entre presets
- **MIDI complet** : Note, CC, Program Change
- **Interface** : LED status, boutons, écran possible

## Structure du Projet

```
KobolExpanderControl/
├── README.md              # Ce fichier
├── TODO.md                # Plan de développement
├── HARDWARE.md            # Documentation matérielle
├── docs/                  # Documentation technique
│   ├── connector_analysis.md
│   ├── schematics/
│   └── measurements/
├── src/                   # Code source Arduino
│   ├── KobolExpanderControl.ino
│   ├── config.h
│   ├── hardware.h
│   ├── midi_handler.cpp
│   ├── preset_manager.cpp
│   └── libraries/
└── tests/                 # Tests et validation
    ├── bench_tests/
    └── field_tests/
```

## État Actuel
🔄 **Phase 1 Complétée** - Documentation et reverse engineering

### ✅ Terminé
- ✅ **Interface P1** complètement analysée (13/16 paramètres confirmés)
- ✅ **Architecture non-invasive** définie
- ✅ **Choix techniques** validés (LM324 + TLE2426)
- ✅ **Roadmap 5 versions** établie
- ✅ **Concepts avancés** : Modulaire virtuel + Kobol V2

### 🔄 En cours
- 🔄 **Acquisition oscilloscope** pour mesures précises
- 🔄 **Validation pins 1,7,8** via analyse carte synthé
- 🔄 **Pinout définitif** avec nomenclature correcte

### ⏳ À venir
- ⏳ **Prototype V1** : Interface P1 non-invasive
- ⏳ **Tests électriques** complets
- ⏳ **Développement versions** avancées

## Références
- **Projet original** : `../KobolExpanderControl_firstRelease/`
- **Synthé** : RSF Kobol Expander (années 70-80)
- **Communauté** : Forums ModWiggler, Synth-DIY

---
*Projet ambitieux pour la communauté des synthés vintage*

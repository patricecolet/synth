# KobolExpanderControl - Première Release (Version Historique)

## Description
Cette version représente le premier prototype fonctionnel d'interface MIDI pour le RSF Kobol Expander.

## Fonctionnalités Actuelles
- **Interface MIDI USB** via Teensy 2.0
- **Contrôle CV Pitch** : 1V/oct via DAC MCP4822 canal A
- **Contrôle Cutoff** : Via DAC MCP4822 canal B (basé sur vélocité)
- **Gate** : Signal numérique
- **LFO Rate** : Sortie PWM
- **Portamento/Glide** : Transition sinusoïdale entre notes

## Matériel
- Teensy 2.0 (ATmega32U4)
- DAC MCP4822 (dual 12-bit)
- Connexion SPI

## Limitations
- Seulement 4 paramètres contrôlés
- Addition de tensions (potard + DAC)
- Pas de système de presets
- Pas de lecture d'état du Kobol
- Interface basique

## Utilisation
Ce code fonctionne tel quel avec la configuration matérielle décrite. Il s'agit de la base de développement pour les versions futures plus avancées.

---
*Version de référence - Ne pas modifier*
*Base pour le développement du système complet*
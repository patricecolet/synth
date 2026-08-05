# Roadmap Versions - KobolExpanderControl

## 🎯 Stratégie Progressive

### **Version 1 : Non-Invasive Pure**
**Objectif :** Interface P1 uniquement, aucune modification Kobol

#### Fonctionnalités
- **P1 Socket** : 13 paramètres confirmés
- **Interface MIDI→CV** : Modulation des paramètres existants
- **Plug & Play** : Connecteur P1 externe

#### Paramètres Contrôlés
1. **LFO Rate** (Pin 2)
2. **VCO1 Waveform** (Pin 3)  
3. **VCF Sustain** (Pin 4)
4. **VCA Attack** (Pin 5)
5. **VCO2 Waveform** (Pin 6)
6. **VCO2 Volume** (Pin 9)
7. **VCF ADS CTRL** (Pin 10)
8. **VCO2 Frequency** (Pin 11) - 1V/oct
9. **VCF Resonance** (Pin 12)
10. **VCO1 Frequency** (Pin 13) - 1V/oct
11. **VCO1 Volume** (Pin 14)
12. **VCF Cutoff** (Pin 15)
13. **VCA Sustain** (Pin 16)

#### Limitations
- **Pins 1, 7, 8** : Non utilisées
- **Pas de Gate** : Enveloppes non déclenchées
- **Pas de CV pitch** indépendant
- **Contrôle** : Modulation uniquement

#### Matériel
- **Teensy 2.0/4.0**
- **DAC MCP4822** (2 CV précises)
- **TLC5940** (14 paramètres PWM)
- **4× LM324** (adaptation ±2.5V)
- **TLE2426** (virtual ground)
- **Boîtier externe**

---

### **Version 2 : Pins Ajoutées**
**Objectif :** Compléter P1 + ajouter pins manquantes

#### Fonctionnalités Supplémentaires
- **Pin 1** : VCA Decay (à confirmer)
- **Pin 7** : VCF Decay (à confirmer)  
- **Pin 8** : VCF Attack (à confirmer)
- **Interface P1 complète** : 16/16 paramètres

#### Modifications Kobol
- **Analyse carte synthé** pour confirmer pins 1,7,8
- **Ajout résistances** si connexions manquantes
- **Soudures minimales** sur carte synthé

#### Enveloppes Complètes
- **VCA ADS** : Attack (5) + Decay (1) + Sustain (16) ✅
- **VCF ADS** : Attack (8) + Decay (7) + Sustain (4) ✅

#### Matériel
- **Identique V1** + composants pour 3 paramètres
- **Modifications** : Résistances + soudures carte

---

### **Version 3 : Interface Complète**
**Objectif :** Système MIDI→CV complet avec P1 + CV/Gate/LFO

---

### **V4** : **Modulaire Virtuel**
**Objectif :** ADC→DSP→PWM pour modules virtuels

#### Fonctionnalités Révolutionnaires
- **20× ADC** : Inputs CV multiples
- **20× PWM** : Outputs CV multiples  
- **DSP Temps Réel** : Modules virtuels (VCO, VCF, LFO, etc.)
- **Routing Matrix** : Configuration logicielle
- **Interface** : Écran + encodeurs

---

### **V5** : **Kobol V2 - Refonte Complète**
**Objectif :** Nouveau Kobol avec convertisseurs exponentiels numériques

#### Révolution Technique
- **Convertisseur expo DAC** : Remplacement transistors
- **Précision parfaite** : 16-bit = 0.018 cents
- **Tempéraments** : Égal, juste, historiques
- **Stabilité** : Aucune dérive thermique

#### Fonctionnalités Supplémentaires
- **CV Pitch** : 1V/oct indépendant via jack
- **Gate** : Déclenchement enveloppes via jack
- **LFO Externe** : Source LFO externe (optionnel)

#### Sorties Supplémentaires
- **CV Out** → Jack CV IN Kobol (1V/oct)
- **Gate Out** → Jack Gate IN Kobol (0/5V)
- **LFO Out** → Jack LFO IN Kobol (optionnel)

#### Interface Musicale Complète
- **P1** : Expression/modulation (16 paramètres)
- **CV** : Notes et gammes (1V/oct)
- **Gate** : Rythme et déclenchement
- **LFO** : Modulations externes

#### Matériel
- **V2** + circuits supplémentaires :
- **DAC supplémentaire** pour CV pitch
- **Driver Gate** (transistor/buffer)
- **LFO Generator** (PWM + filtrage)
- **Jacks sortie** (3×)

---

## 🚀 Stratégie de Développement

### **Phase 1 : V1 - Validation Concept**
- **Proof of concept** non-invasif
- **Test interface P1**
- **Validation 13 paramètres**
- **Feedback communauté**

### **Phase 2 : V2 - Optimisation**
- **Analyse carte synthé**
- **Complétion interface P1**
- **16 paramètres complets**
- **Documentation technique**

### **Phase 3 : V3 - Système Complet**
- **Interface musicale totale**
- **Séquenceur MIDI→CV professionnel**
- **Commercialisation possible**

## 💡 Avantages Stratégie

### **Progression Sécurisée**
- **V1** : Zéro risque pour le Kobol
- **V2** : Modifications minimales
- **V3** : Système abouti

### **Marché Étagé**
- **V1** : Utilisateurs prudents
- **V2** : Bricoleurs confirmés  
- **V3** : Professionnels/studios

### **Développement Incrémental**
- **Apprentissage** progressif
- **Tests** à chaque étape
- **Amélioration** continue

---

## 📋 Prochaines Actions

1. **Finaliser V1** : Interface P1 non-invasive
2. **Mesures électriques** : Validation tensions
3. **Prototype V1** : Test fonctionnel
4. **Analyse carte synthé** : Préparation V2

---
*Roadmap évolutive - Adaptation selon retours utilisateurs*

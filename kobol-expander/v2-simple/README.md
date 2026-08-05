# KobolExpanderControl V2 - Version Simple

## 🎯 **Version de Production - Non-Invasive**

### **Objectif**
Interface MIDI→CV simple et fiable utilisant uniquement le connecteur P1, sans modification du Kobol.

## 🔧 **Architecture**

### **Matériel**
- **MCU** : ESP32-S3 VROOM N16R8 (WiFi désactivé)
- **Adaptation** : 4× LM324 + TLE2426 (±1.5V)
- **DAC** : MCP4921 (monocanal) + TL082 (étage sortie bipolaire) pour CV pitch externe 1 V/oct
- **Alimentation** : USB 5V + ICL7660 (-5V)

### **Interface**
- **P1** : 14 paramètres contrôlés (éviter Pin 2 = -14.5V)
- **CV Out** : Jack externe pour pitch 1V/oct
- **Gate Out** : Jack externe pour déclenchement
- **USB** : MIDI natif

## 📊 **Paramètres Contrôlés (14/16)**

### **Validés par Mesures :**
1. **Pin 1** : VCA Decay (-1.41V → -1.17V)
2. **Pin 4** : VCF Sustain (0V → 0.92V)  
3. **Pin 5** : VCA Attack (-0.66V → -0.28V)
4. **Pin 7** : VCF Decay (-1.35V → -0.36V)
5. **Pin 8** : VCF Attack (-0.66V → -1.26V)
6. **Pin 9** : VCO2 Volume (0V → 0.61V)
7. **Pin 11** : VCO2 Frequency (-1532mV → +1532mV) ⭐
8. **Pin 12** : VCF Resonance (-0.67V → 0.61V)
9. **Pin 16** : VCA Sustain (-0.03V → 0.94V)

### **À Valider (Oscilloscope) :**
10. **Pin 3** : VCO1 Waveform (0V mesuré - signaux faibles ?)
11. **Pin 6** : VCO2 Waveform (0V mesuré - signaux faibles ?)
12. **Pin 10** : VCF ADS CTRL (0.05V → 0.6V)
13. **Pin 14** : VCO1 Volume (0.13V → 0.6V)
14. **Pin 15** : VCF Cutoff (0V → 0.9V)

### **Non Utilisées :**
- **Pin 2** : V- (-14.5V) - ÉVITER (danger)
- **Pin 13** : VCO1 Frequency - Manuel par choix

## 🔧 **Circuit Standard**

### **Adaptation Uniforme ±1.5V :**
```
PWM → R 1kΩ → C 330nF → LM324 → ±1.5V → Pin P1
```

### **Calibration Logicielle :**
```cpp
// Table par paramètre
struct ParamConfig {
  float scale;   // Facteur échelle  
  int16_t offset; // Décalage mV
};

// Ajustement par pin selon mesures
```

## 💡 **Fonctionnalités**

### **Presets Simples :**
- **Création** : Ajustement son + sauvegarde PWM
- **Rappel** : Application valeurs absolues
- **Interface** : Boutons + LED + écran OLED

### **MIDI Complet :**
- **Note On/Off** : VCO2 Frequency + Gate
- **Control Change** : 14 paramètres P1
- **Program Change** : Sélection presets

## 💰 **Coût Estimé**

### **Composants :**
- **ESP32-S3** : 8€
- **4× LM324** : 4€
- **TLE2426** : 2€
- **MCP4921 + TL082** : ~4€
- **ICL7660** : 2€
- **Passifs** : 15€
- **PCB + Boîtier** : 20€

**Total** : ~54€ pour interface complète

## 🚀 **Avantages V2**

- ✅ **Non-invasif** : Aucune modification Kobol
- ✅ **Plug & Play** : Connecteur P1 standard
- ✅ **Fiable** : Circuit simple et éprouvé
- ✅ **Économique** : Coût raisonnable
- ✅ **Évolutif** : Base pour versions avancées

---
*Version de production recommandée*

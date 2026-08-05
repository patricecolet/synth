# Module Voix - Conception Modulaire

## 🎯 **Concept Module Voix**

### **V4 = Prototype 1 Voix**
```
Module VCO+VCF+VCA complet
├─ Validation architecture
├─ Tests sonores  
├─ Optimisation circuit
└─ Documentation finale
```

### **PolyKobol = 8× Module V4**
```
8× Modules identiques + Contrôle central
├─ Allocation voix intelligente
├─ Layers management
├─ Mixage final
└─ Interface commune
```

## 🔧 **Architecture Module Voix**

### **Carte Module V4 :**
```
Entrées :
├─ CV Pitch (DAC expo)
├─ CV VCF Cutoff  
├─ CV VCA Level
├─ Gate In
├─ Sync In (VCO2)
├─ Alimentation ±15V
└─ GND

Circuit :
├─ 2× VCO (DAC expo + quad transistor + OTA)
├─ 1× VCF (CEM3320 ou AS3320)
├─ 1× VCA (CEM3360 ou AS3360)
├─ 1× Noise (2N3904 avalanche)
└─ Enveloppes ADS (numériques)

Sorties :
├─ Audio Out (mono)
├─ VCO1 Out (pour sync/FM)
├─ VCO2 Out (pour sync/FM)
└─ Envelope Out (CV)
```

### **Dimensions :**
```
PCB : 10cm × 8cm (Eurocard format)
Connecteurs : Headers 2.54mm
Alimentation : Via connecteur
```

## 🎛️ **Contrôle Centralisé**

### **V4 (1 Voix) :**
```
Teensy 4.1 → Module Voix Unique
├─ 4× DAC (2 VCO + VCF + VCA)
├─ 8× PWM (paramètres secondaires)
├─ Interface utilisateur
└─ Plugin VST
```

### **PolyKobol (8 Voix) :**
```
Teensy 4.1 → 8× Modules Voix
├─ 32× DAC (8 voix × 4 CV)
├─ 64× PWM (8 voix × 8 params)
├─ Voice allocation
├─ Layer management
└─ Plugin polyphonique
```

## 🔗 **Interconnexions Modules**

### **Bus Principal :**
```
Control Bus : I2C ou SPI vers tous modules
Audio Bus : Mixage analogique résistif
Sync Bus : Clock + sync entre VCO
Power Bus : ±15V + GND distribués
```

### **Adressage Modules :**
```cpp
// Chaque module = adresse unique
#define VOICE_1 0x10
#define VOICE_2 0x11
// ...
#define VOICE_8 0x17

void setVoiceParameter(uint8_t voice, uint8_t param, uint16_t value) {
  i2c_write(voice, param, value);
}
```

## 🎵 **Validation Progressive**

### **Phase 1 : Module V4**
```
Tests :
├─ Réponse 1V/oct précise
├─ Caractère sonore authentique
├─ Stabilité thermique
├─ Modulations correctes
└─ Interface fonctionnelle
```

### **Phase 2 : Duplication**
```
Fabrication :
├─ 8× PCB identiques
├─ Composants appairés (VCO tracking)
├─ Tests individuels
└─ Intégration système
```

### **Phase 3 : PolyKobol**
```
Système :
├─ Voice allocation
├─ Layers management  
├─ Mixage 8 voix
├─ Interface polyphonique
└─ Validation musicale
```

## 💰 **Économies Développement**

### **R&D Partagée :**
```
V4 Development : 100h
PolyKobol : V4 × 8 + Integration (50h)

vs

PolyKobol direct : 300h

Économie : 150h (50% temps !)
```

### **Production :**
```
Module V4 : Coût unitaire optimisé
PolyKobol : 8× modules + châssis
Évolutivité : Modules remplaçables
```

## 🔧 **Spécifications Module**

### **Connecteur Module :**
```
Pin 1-2 : ±15V alimentation
Pin 3 : GND
Pin 4-7 : CV inputs (Pitch, VCF, VCA, Mod)
Pin 8 : Gate in
Pin 9 : Sync in
Pin 10 : Audio out
Pin 11-12 : VCO outs (sync/FM)
Pin 13 : Envelope out
Pin 14-16 : Réservés expansion
```

### **Interface I2C :**
```cpp
// Commandes module
#define CMD_SET_VCO_FREQ    0x01
#define CMD_SET_VCF_CUTOFF  0x02
#define CMD_SET_VCA_LEVEL   0x03
#define CMD_TRIGGER_ENV     0x04
```

## 🚀 **Avantages Stratégie**

### **Technique :**
- **Validation** : 1 voix avant 8
- **Réutilisation** : Circuit éprouvé
- **Modularité** : Maintenance facilitée

### **Économique :**
- **R&D** : Coût partagé
- **Production** : Économies échelle
- **Évolution** : Modules upgradables

### **Commercial :**
- **V4** : Marché mono (500€)
- **PolyKobol** : Marché poly (3000€)
- **Modules** : Pièces détachées

**Stratégie parfaite !** 🎯

**V4 = Building block pour PolyKobol !** 🧱🚀✨


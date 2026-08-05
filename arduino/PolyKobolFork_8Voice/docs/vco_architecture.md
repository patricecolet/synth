# Architecture VCO - PolyKobol

## 🎯 **VCO Moog/Kobol Classique Modernisé**

### **Circuit Original (Moog/Kobol) :**
```
CV 1V/oct → Convertisseur Expo → Quad Transistor → Wave Shaper → Buffer
              (Analogique)         (2N3904×4)       (Diodes)    (AOP)
                   ↓                    ↓              ↓          ↓
            Dérive thermique      Oscillateur      Formes     Sortie
            Précision limitée     Core analogique   d'ondes    + Sync
```

### **Innovation PolyKobol :**
```
CV 1V/oct → DAC 16-bit → Quad Transistor → Wave Shaper → Buffer
              ↓              ↓              ↓          ↓
         Précision parfaite  Oscillateur   Formes    Sortie
         Stabilité absolue   Core analogique d'ondes  + Sync
```

**Seul changement : Convertisseur expo analogique → DAC numérique**

## 🔧 **Convertisseur Exponentiel DAC**

### **Fonction :**
```
DAC Output = Vref × (2^(CV_Input/Voctave) - 1) / (2^(Vrange/Voctave) - 1)
```

### **Implémentation :**
```cpp
float calculate_expo_voltage(float cv_input) {
  // CV en volts (0-10V pour 10 octaves)
  float octaves = cv_input;  // 1V = 1 octave
  
  // Fonction exponentielle
  float expo_factor = pow(2.0, octaves);
  
  // Conversion vers tension contrôle transistors
  float control_voltage = Vref * (expo_factor - 1.0) / 1023.0;
  
  return control_voltage;
}
```

### **DAC Lookup Table (Optimisé) :**
```cpp
// Pré-calcul pour vitesse
uint16_t expo_table[65536];  // 16-bit resolution

void generate_expo_table() {
  for(int i = 0; i < 65536; i++) {
    float cv = (float)i / 6553.6;  // 0-10V
    expo_table[i] = calculate_expo_dac(cv);
  }
}
```

## ⚡ **Quad Transistor (Inchangé)**

### **Circuit Moog Classique :**
```
Expo Voltage → Q1 (Timing capacitor)
            → Q2 (Current mirror)  
            → Q3 (Current mirror)
            → Q4 (Output buffer)
```

### **Composants :**
- **Transistors** : 2N3904 appairés thermiquement
- **Condensateur** : 1nF timing (fréquence de base)
- **Résistances** : Bias et polarisation

### **Avantages Conservation :**
- ✅ **Son authentique** : Core analogique Moog
- ✅ **Caractère** : Non-linéarités préservées
- ✅ **Harmoniques** : Distorsion analogique naturelle

## 🌊 **Wave Shaper (Inchangé)**

### **Génération Formes d'Ondes (OTA) :**
```
Triangle Core → ┬─ Sine (OTA wave shaper)
                ├─ Square (comparateur)
                ├─ Saw (OTA intégrateur)
                └─ Pulse (OTA PWM control)
```

### **Wave Morphing :**
```
4 Formes → DAC Morphing → Mélange résistif → Sortie finale
```

## 🎵 **Layers - Implémentation**

### **Configuration Layer :**
```cpp
struct Layer {
  uint8_t note;           // Note MIDI
  float detune_cents;     // Détune en cents
  ADSR envelope;          // Enveloppe individuelle
  FilterParams filter;    // VCF individuel
  float volume;           // Niveau layer
  bool mute;              // Mute/Solo
};
```

### **Exemples Layers :**
```cpp
// Accord parfait majeur
layers[0].note = 60;  // Do
layers[1].note = 64;  // Mi  
layers[2].note = 67;  // Sol
layers[3].note = 72;  // Do octave

// Détune pour épaisseur
layers[4].detune = +5.0;   // +5 cents
layers[5].detune = -5.0;   // -5 cents
```

## 🔧 **Contrôle Sync VCO2**

### **Sync Numérique :**
```cpp
void updateSync() {
  for(int voice = 0; voice < 8; voice++) {
    if(sync_enabled[voice]) {
      // Reset VCO2 à chaque cycle VCO1
      vco2_reset_trigger[voice] = vco1_zero_cross[voice];
    }
  }
}
```

### **Hard Sync Analogique :**
```
VCO1 Zero Cross → Transistor Reset → VCO2 Timing Capacitor
```

## 💰 **Économies vs AS3340**

### **Coût Comparatif :**
```
16× AS3340 : 320€
16× DAC+Circuit : 160€ + 80€ = 240€

Économie : 80€ + Fonctionnalités supérieures !
```

## 🎯 **Avantages Architecture**

### **Son :**
- ✅ **Authentique** : Core Moog/Kobol préservé
- ✅ **Caractère** : Analogique pur après DAC
- ✅ **Harmoniques** : Non-linéarités naturelles

### **Technique :**
- ✅ **Précision** : DAC 16-bit parfait
- ✅ **Stabilité** : Aucune dérive
- ✅ **Évolutivité** : Algorithmes updates

### **Musical :**
- ✅ **Layers** : 8 voix superposables
- ✅ **Tempéraments** : Historiques + exotiques
- ✅ **Micro-tuning** : Précision impossible avant

**Architecture parfaite !** 🎯

**Son Moog/Kobol + Précision moderne + Layers = RÉVOLUTION !** 🚀✨

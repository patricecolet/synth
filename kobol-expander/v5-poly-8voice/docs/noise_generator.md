# Générateur de Bruit - PolyKobol

## 🎯 **Noise Generator - Essentiel Synthé**

### **Types de Bruit :**
- **Bruit Blanc** : Toutes fréquences égales (percussions, effets)
- **Bruit Rose** : -3dB/octave (plus musical, naturel)
- **Bruit Rouge** : -6dB/octave (graves accentués)

### **Applications Musicales :**
- **Percussions** : Snare, hi-hat, cymbales
- **Effets** : Wind, ocean, textures
- **Modulation** : Source aléatoire pour S&H
- **Mixage** : Épaisseur sonore

## 🔧 **Implémentation Hardware**

### **Option 1 : Noise IC Dédié**
```
MM5837 (Digital Noise) → Filtrage → Amplification
└─ Shift register → Pseudo-random → Audio noise
```

### **Option 2 : Transistor Avalanche**
```
2N3904 en avalanche → Amplification → Filtrage
└─ Bruit vrai analogique → Caractère vintage
```

### **Option 3 : Numérique (Teensy)**
```cpp
// Générateur pseudo-aléatoire
class NoiseGenerator {
  uint32_t lfsr = 0xACE1;  // Linear Feedback Shift Register
  
  float generateWhite() {
    lfsr = (lfsr >> 1) ^ (-(lfsr & 1) & 0xB400);
    return (float)lfsr / 65536.0 - 0.5;  // ±0.5
  }
  
  float generatePink() {
    // Algorithme Paul Kellett
    static float b0, b1, b2, b3, b4, b5, b6;
    float white = generateWhite();
    
    b0 = 0.99886 * b0 + white * 0.0555179;
    b1 = 0.99332 * b1 + white * 0.0750759;
    // ... filtrage rose
    return b0 + b1 + b2 + b3 + b4 + b5 + b6 + white * 0.5362;
  }
};
```

## 🎛️ **Contrôles Noise**

### **Paramètres :**
- **Level** : Volume bruit (0-100%)
- **Color** : Blanc ↔ Rose ↔ Rouge
- **Filter** : Passe-haut/bas pour texture
- **Modulation** : LFO/ENV → Level/Color

### **Interface Plugin :**
```
Noise Section :
┌─────────────────┐
│ White ●─────○ Pink │
│ Level [████░░░░] │
│ Filter [██████░] │
│ Mod → VCF ●     │
└─────────────────┘
```

## 🔧 **Architecture par Version**

### **V2 Simple :**
```
ESP32-S3 → PWM → RC Filter → Noise Out
         → Algorithme numérique simple
```

### **V3-V5 Avancées :**
```
Option A : MM5837 → Filtrage analogique → VCA contrôlé
Option B : Transistor avalanche → Authentique vintage
Option C : DSP sophistiqué → Formes multiples
```

### **PolyKobol :**
```
8× Noise generators indépendants :
- Par voix ou global selon configuration
- Mixage : Résistif ou numérique
- Filtrage : Analogique (caractère)
```

## 🎵 **Noise dans Kobol Original**

### **Circuit Probable :**
```
Transistor Avalanche → Amplification → VCA → Mixer
                    → Filtrage passe-haut → Texture
```

### **Intégration Mixer :**
```
VCO1 ──┐
VCO2 ──┼── Mixer → VCF
Noise ─┘
```

### **Contrôle :**
- **Noise Level** : Potentiomètre ou CV
- **Dans P1** : Probablement une des pins non identifiées

## 🔧 **Implémentation Recommandée**

### **V2 Simple - Numérique :**
```cpp
// Dans loop principal (1kHz)
float noise_sample = noise_generator.generatePink();
uint16_t noise_pwm = (noise_sample + 1.0) * 8191;  // 0-16383
ledcWrite(NOISE_CHANNEL, noise_pwm);
```

### **V3+ - Analogique :**
```
MM5837 → TL074 (amplification) → PWM VCA → Sortie
       → RC filtering → Blanc/Rose sélectionnable
```

### **PolyKobol - Hybride :**
```
Teensy → DAC → MM5837 control → Noise analogique
       → PWM → VCA control → Level par voix
```

## 🎯 **Intégration P1**

### **Noise dans P1 :**
**Probablement une des pins 0V mesurées !**
- **Pin 3** : Noise Level ?
- **Pin 6** : Noise Color ?
- **Pin 14/15** : Noise modulation ?

### **Test Validation :**
```
Injection tension → Pin 3,6,14,15
Écouter : Changement niveau/couleur bruit ?
```

## 💡 **Noise + Modulations**

### **Sources Modulation :**
```
Noise → S&H → Random CV
Noise → Envelope Follower → Dynamic CV
Noise → Bandpass → Filtered modulation
```

### **Applications Créatives :**
```cpp
// Noise modulé par LFO
float modulated_noise = noise * (1.0 + lfo * 0.5);

// Noise quantifié  
float quantized_noise = quantize(noise, pentatonic_scale);

// Noise filtré par enveloppe
float shaped_noise = noise * envelope_output;
```

## 🚀 **Architecture Complète**

### **Signal Path :**
```
VCO1 ──┐
VCO2 ──┼── Mixer → VCF → VCA → Output
Noise ─┘     ↑      ↑     ↑
            LFO    ENV   ENV
             ↑      ↑     ↑
         Mod Matrix ┴─────┘
```

### **Contrôle Numérique :**
```
Teensy → ┬─ VCO CV (DAC expo)
         ├─ VCF CV (PWM)
         ├─ VCA CV (PWM)  
         ├─ Noise Level (PWM)
         └─ Modulations (PWM)
```

## 🎯 **Conclusion**

**Noise Generator = Indispensable !**
- **Percussions** : Impossible sans bruit
- **Textures** : Épaisseur sonore
- **Modulations** : Source aléatoire
- **Authenticité** : Synthé complet

### **Prochains Tests :**
1. **Identifier** pin noise dans P1
2. **Tester** injection sur pins 0V
3. **Valider** contrôle niveau/couleur

**Excellente observation !** 🎵

**Noise = Composant manquant crucial** pour synthé complet ! ✨🔊


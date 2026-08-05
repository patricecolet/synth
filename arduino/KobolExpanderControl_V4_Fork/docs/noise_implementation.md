# Noise Generator - Kobol V2 (V4)

## 🎯 **Noise dans Kobol V2**

### **Architecture Authentique**
Reproduire le caractère du noise generator Kobol original avec contrôle numérique moderne.

## 🔧 **Implémentation V4**

### **Option 1 : Transistor Avalanche (Authentique)**
```
2N3904 en avalanche → TL074 → VCA (PWM controlled) → Mixer
      ↓                ↓           ↓                   ↓
Bruit vrai        Amplification  Niveau variable    VCO+Noise
analogique        + filtrage     numérique          → VCF
```

**Avantages :**
- ✅ **Son authentique** : Bruit analogique vrai
- ✅ **Caractère vintage** : Non-linéarités naturelles
- ✅ **Contrôle moderne** : Niveau par PWM

### **Option 2 : MM5837 + Filtrage**
```
MM5837 → Filtrage analogique → VCA → Mixer
   ↓           ↓                ↓      ↓
Noise IC   Blanc→Rose       Niveau  Mixage
digital    (RC networks)    PWM     VCO+Noise
```

**Avantages :**
- ✅ **Stabilité** : Pas de réglage avalanche
- ✅ **Reproductibilité** : Identique entre unités
- ✅ **Fiabilité** : IC dédié

### **Option 3 : Hybride Numérique/Analogique**
```
Teensy → DAC → Filtrage analogique → VCA → Mixer
   ↓       ↓         ↓               ↓      ↓
Algo    Noise     Caractère        Niveau  Mixage
sophistiqué     analogique       contrôlé
```

## 🎵 **Contrôles Noise**

### **Paramètres Kobol V2 :**
- **Noise Level** : 0-100% (PWM controlled)
- **Noise Color** : Blanc ↔ Rose (filtrage variable)
- **Noise Filter** : Passe-haut pour texture
- **Noise Mod** : LFO/ENV → Level dynamique

### **Interface Plugin :**
```
Noise Generator :
┌─────────────────────┐
│ Level  [██████░░░░] │
│ Color  White ●──○ Pink │
│ Filter [████████░░] │
│ Mod    LFO1 → Level │
└─────────────────────┘
```

## 🔧 **Circuit Recommandé V4**

### **Transistor Avalanche (Authentique) :**
```
+15V ──[R 1MΩ]── 2N3904 Collector
                    │
                    ├── C 100pF ── TL074 (buffer)
                    │                │
                   GND              VCA (PWM)
                                     │
                                   Mixer
```

### **Réglage Avalanche :**
- **Résistance** : 1MΩ variable pour réglage avalanche
- **Condensateur** : 100pF filtrage HF
- **Buffer** : TL074 isolation + amplification

### **Contrôle Niveau :**
```
PWM → RC Filter → VCA control → Noise level variable
```

## 🎛️ **Filtrage Blanc/Rose**

### **Circuit Filtrage :**
```
Noise blanc → ┬─ Direct (blanc)
              ├─ RC -3dB/oct (rose)  
              └─ RC -6dB/oct (rouge)
              
Sélection : PWM → VCA mix → Couleur variable
```

### **Algorithme Couleur :**
```cpp
void setNoiseColor(float color) {
  // color : 0.0 = blanc, 1.0 = rose
  float white_amount = 1.0 - color;
  float pink_amount = color;
  
  setVCA(NOISE_WHITE, white_amount);
  setVCA(NOISE_PINK, pink_amount);
}
```

## 🎯 **Intégration Signal Path**

### **Mixer Kobol :**
```
VCO1 ──[Level]──┐
VCO2 ──[Level]──┼── Mixer → VCF → VCA → Output
Noise ─[Level]──┘
```

### **Modulations Noise :**
```
LFO1 → Noise Level (tremolo bruit)
ENV1 → Noise Color (évolution texture)
Velocity → Noise Level (expressivité)
```

## 🔊 **Applications Musicales**

### **Percussions :**
```
Noise + ENV rapide = Snare drum
Noise + Filtre HF = Hi-hat
Noise + Filtre BF = Kick texture
```

### **Textures :**
```
Noise + LFO lent = Wind effect
Noise + VCF modulation = Sweep texture
Noise + S&H = Random steps
```

### **Modulation :**
```
Noise → S&H → Random CV → VCO pitch (chaos)
Noise → Envelope follower → Dynamic modulation
```

## 💰 **Coût par Version**

### **V2 Simple :**
- **Numérique** : 0€ (algorithme ESP32)
- **Filtrage** : 2€ (RC + VCA)

### **V4 Authentique :**
- **2N3904** : 0.50€
- **TL074** : 1€
- **VCA** : 2€ (PWM controlled)
- **Passifs** : 1€
**Total** : ~4.50€

## 🚀 **Conclusion**

**Noise Generator = Indispensable !**
- **V2** : Implémentation numérique simple
- **V4** : Transistor avalanche authentique
- **V5** : 8× générateurs indépendants

**Transforme** l'interface en **vrai synthé complet** !

**Percussions + textures + modulations** = Expressivité totale ! 🎵✨


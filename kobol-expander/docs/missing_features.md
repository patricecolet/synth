# Fonctionnalités Manquantes - Toutes Versions

## 🎯 **Éléments Cruciaux à Ajouter**

### **1. Matrice de Connexion (Modulation Matrix)**
### **2. Modulateurs Internes**  
### **3. Gestion Portamento**

---

## 🔗 **Matrice de Connexion**

### **Principe :**
```
Sources → Matrice → Destinations
   ↓        ↓         ↓
LFO1    [Amount]   VCF Cutoff
ENV1    [Amount]   VCO2 Pitch  
VCO1    [Amount]   VCO2 PWM
Wheel   [Amount]   VCF Resonance
```

### **Implémentation par Version :**

#### **V2 Simple :**
```
Matrice 4×4 basique :
- 2× LFO → 4× Destinations
- 2× ENV → 4× Destinations  
- Interface : 4 potards + 4 boutons
```

#### **V3-V5 Avancées :**
```
Matrice 16×16 complète :
- 8× Sources → 16× Destinations
- Interface : Écran + encodeurs
- Sauvegarde : Par preset
```

### **Sources Modulation :**
- **LFO1/LFO2** : Formes d'ondes multiples
- **ENV1/ENV2** : Enveloppes ADS
- **Velocity** : Vélocité note
- **Aftertouch** : Pression clavier
- **Mod Wheel** : Contrôleur MIDI
- **VCO1** : Auto-modulation

### **Destinations :**
- **VCO1/VCO2 Pitch** : Vibrato, FM
- **VCO1/VCO2 PWM** : Modulation largeur
- **VCF Cutoff** : Modulation filtre
- **VCF Resonance** : Modulation Q
- **VCA Level** : Tremolo
- **Pan** : Modulation stéréo

---

## 🌊 **Modulateurs Internes**

### **LFO (Low Frequency Oscillators)**

#### **LFO1/LFO2 Caractéristiques :**
```cpp
class LFO {
  enum Waveform { SINE, TRIANGLE, SAW_UP, SAW_DOWN, SQUARE, 
                  RANDOM, SAMPLE_HOLD, CHAOS };
  
  float frequency;    // 0.01Hz - 100Hz
  float amplitude;    // 0-100%
  float phase;        // 0-360°
  bool sync_to_key;   // Reset sur note
  bool sync_to_tempo; // Sync MIDI clock
};
```

#### **Formes d'Ondes Avancées :**
- **Sine** : Vibrato classique
- **Triangle** : Modulation douce
- **Random** : Variations aléatoires
- **S&H** : Steps quantifiés
- **Chaos** : Modulation complexe

### **Enveloppes Multiples**

#### **ENV1/ENV2 (ADS) :**
```cpp
class Envelope {
  float attack;     // 1ms - 10s
  float decay;      // 1ms - 10s  
  float sustain;    // 0-100%
  
  enum Curve { LINEAR, EXPONENTIAL, LOGARITHMIC, S_CURVE };
  Curve attack_curve;
  Curve decay_curve;
};
```

#### **Enveloppes Spécialisées :**
- **ENV1** : VCF (filter envelope)
- **ENV2** : VCA (amplitude envelope)
- **ENV3** : Modulation (assignable)

---

## 🎹 **Gestion Portamento**

### **Types Portamento :**

#### **Portamento Simple :**
```cpp
class Portamento {
  float time;          // 0-10s
  bool legato_only;    // Seulement si notes liées
  enum Curve { LINEAR, EXPONENTIAL, S_CURVE };
  Curve glide_curve;
};
```

#### **Portamento par Version :**

**V2 Simple :**
```
Portamento global :
- Time : 1 potard (0-5s)
- Curve : Exponentielle fixe
- Mode : Toujours actif
```

**V3-V5 Avancées :**
```
Portamento sophistiqué :
- Time : Variable par voix
- Curve : Sélectionnable  
- Mode : Legato/Always/Off
- Destination : Pitch + autres paramètres
```

### **Implémentation :**
```cpp
void updatePortamento() {
  for(int voice = 0; voice < num_voices; voice++) {
    if(voices[voice].portamento_active) {
      float current = voices[voice].current_pitch;
      float target = voices[voice].target_pitch;
      float step = (target - current) * portamento_rate;
      
      voices[voice].current_pitch += step;
      updateVCO_Pitch(voice, voices[voice].current_pitch);
    }
  }
}
```

---

## 🎛️ **Interface Modulation**

### **V2 Simple - Interface Basique :**
```
Hardware :
- 4× Potards : LFO1/2 Rate + Amount
- 4× Boutons : Destinations LFO
- 2× Potards : ENV Attack/Decay
- 1× Potard : Portamento Time
```

### **V3-V5 - Interface Avancée :**
```
Hardware :
- Écran TFT : Matrice visuelle
- 4× Encodeurs : Navigation
- 16× Boutons : Sélection rapide
- Mod Matrix : Page dédiée écran
```

### **Écran Matrice :**
```
     │VCF│VCA│VCO│PWM│
─────┼───┼───┼───┼───┤
LFO1 │ ● │   │ ● │   │
ENV1 │ ● │ ● │   │   │  
VEL  │   │ ● │   │ ● │
WHEEL│ ● │   │ ● │   │
```

---

## 🚀 **Roadmap Intégration**

### **Phase 1 : V2 Simple**
- **LFO basique** : 2× LFO avec formes standard
- **Matrice 4×4** : Connexions essentielles
- **Portamento** : Global simple

### **Phase 2 : V3-V5**
- **LFO avancés** : Formes complexes + sync
- **Matrice 16×16** : Contrôle total
- **Portamento** : Par voix + courbes

### **Phase 3 : PolyKobol**
- **Matrice par layer** : 8× matrices individuelles
- **Modulations croisées** : Layer → Layer
- **Portamento polyphonique** : Glide indépendant

---

## 🎯 **Impact sur Architecture**

### **Pins Supplémentaires :**
- **LFO outputs** : 2-4 pins PWM
- **Matrix control** : I2C vers DAC/PWM
- **Portamento** : Calcul software

### **CPU Supplémentaire :**
- **LFO generation** : Temps réel
- **Matrix calculation** : 16×16 multiplications
- **Portamento** : Interpolation continue

### **Mémoire :**
- **Matrix presets** : 16×16×16 bits
- **LFO wavetables** : Formes complexes
- **Portamento curves** : Tables interpolation

**Ces fonctionnalités transforment** l'interface en **vrai synthé complet !** 🎵

**Excellente observation** - elles sont **indispensables** pour une expérience musicale complète ! 🎯✨

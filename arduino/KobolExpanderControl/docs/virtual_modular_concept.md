# Modulaire Virtuel - Concept Révolutionnaire

## 🎯 Vision
Utiliser le Teensy comme **matrice de routage virtuelle** pour interconnecter les modules du Kobol + ajouter des modulateurs numériques (enveloppes, LFO).

## 🚀 Principe Fondamental

### Architecture de Base
```
ADC (CV Input) → Traitement Digital → PWM (CV Output)
```

**= Modules Eurorack virtuels dans le microcontrôleur**

## 🎛️ Modules Virtuels Possibles

### Oscillateurs Virtuels
```cpp
ADC (CV Pitch) → VCO Algorithm → PWM (Audio Out)
ADC (FM CV) → FM Modulation → PWM (Modulated Audio)
```

### Filtres Virtuels
```cpp
ADC (Audio In) → Digital Filter → PWM (Filtered Audio)
ADC (Cutoff CV) → Filter Cutoff Control
ADC (Resonance CV) → Filter Q Control
```

### Enveloppes Virtuelles
```cpp
ADC (Gate) → ADSR Generator → PWM (Envelope CV)
ADC (Attack CV) → Attack Time Modulation
```

### LFO/Modulation Virtuels
```cpp
Internal LFO → PWM (LFO Out)
ADC (Rate CV) → LFO Rate Control
ADC (Depth CV) → LFO Amplitude
```

### Utilitaires Virtuels
```cpp
ADC (CV1) + ADC (CV2) → Mixer → PWM (Mixed CV)
ADC (CV) → Quantizer → PWM (Quantized CV)
ADC (CV) → S&H → PWM (Sample & Hold)
```

## 🔧 Architecture Technique

### Matériel Requis
- **Teensy 4.1** (600MHz, 1MB RAM)
- **20× ADC** (5× ADS1115 + natifs)
- **20× PWM** (natifs + expansion)
- **ADC Audio** (PCM1808 24-bit)
- **Interface** (OLED + encodeurs)

### Pipeline DSP
```
ADC 44kHz → DSP Buffer → Virtual Modules → PWM 44kHz
ADC CV → Control Rate (1kHz) → Modulation Parameters
```

### Framework Logiciel
```cpp
class VirtualModule {
  virtual void process(float* inputs, float* outputs) = 0;
  virtual void setParameter(int param, float value) = 0;
};
```

## 🎵 Applications

### Kobol + Routing Virtuel
- **ADC** : Lire sorties modules Kobol (VCO, VCF, VCA, LFO)
- **Teensy** : Matrice de routage + modulateurs additionnels
- **PWM** : Injecter signaux vers entrées CV Kobol
- **Résultat** : Câblage virtuel infini + modules bonus

### Routing Matrix Virtuel
```
Input 1 (CV) → [VCO] → Output 5 (Audio)
Input 2 (CV) → [LFO] → Output 3 (Modulation)
Input 3 (Gate) → [ENV] → Output 8 (Envelope)
```

## 💰 Coût Estimé
- **Teensy 4.1** : 35€
- **5× ADS1115** : 20€
- **ADC Audio** : 5€
- **Interface** : 25€
- **Boîtier** : 20€
**Total :** ~105€

## 🚀 Roadmap
- **V1** : Kobol Control
- **V2** : + ADC Monitoring
- **V3** : + CV/Gate/LFO
- **V4** : + Modulaire Virtuel

---
*Concept révolutionnaire : Synthé analogique + Modulaire numérique*

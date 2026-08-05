# Mesures Connecteur P1 - RSF Kobol Expander

## 📋 Informations Session

**Date :** ___________  
**Heure :** ___________  
**Conditions :** ___________  
**Multimètre :** ___________  
**Kobol S/N :** ___________  

## 📐 Phase 1 : Identification Physique

### Pinout Vérifié
```
P1 - Socket femelle 2×8 pins (vue côté composant)
 1  2  3  4  5  6  7  8
16 15 14 13 12 11 10  9

Pin 1 localisée : □ Encoche □ Marquage □ Autre: _______
Orientation confirmée : □ Oui □ Non - Correction: _______
```

## ⚡ Phase 2 : Mesures Tension (Kobol ALLUMÉ)

### ⚠️ SÉCURITÉ : ±18V présent !

### Tensions au Repos (Potards au centre)
| Pin | Tension (V) | Type Signal | Notes |
|-----|-------------|-------------|-------|
| 1   | -0.09       | CV          | Proche de 0V |
| 2   | -14.52      | Référence   | Égal V- (-14.5V) |
| 3   | 0.00        | CV          | Référence 0V |
| 4   | +0.44       | CV          | Signal positif |
| 5   | -0.50       | CV          | Signal négatif |
| 6   | 0.00        | CV          | Référence 0V |
| 7   | -1.27       | CV          | Signal négatif |
| 8   | -0.52       | CV          | Signal négatif |
| 9   | +0.60       | CV          | Signal positif |
| 10  | +0.59       | CV          | Signal positif |
| 11  | 0.00        | CV          | Référence 0V |
| 12  | +0.60       | CV          | Signal positif |
| 13  | +0.58       | CV          | Signal positif |
| 14  | 0.00        | CV          | Référence 0V |
| 15  | 0.00        | CV          | Référence 0V |
| 16  | +0.46       | CV          | Signal positif |

### Variations avec Potentiomètres

#### Pin 2 - Référence V-
**Fonction :** RÉFÉRENCE -14.5V (égal V-, pas LFO Rate !)
- Position MIN : -14.5 V
- Position MID : -14.5 V  
- Position MAX : -14.5 V
- **Plage totale :** 0 V (constante)
- **Confirmé :** ✅ Référence V- (-14.5V)

#### Pin 3 - VCO1 Waveform (Hypothèse)
**Potard Waveform1 :**
- Position MIN :0 V
- Position MID : 0 V
- Position MAX : 0 V
- **Plage totale :** _______ V
- **Confirmé :** □ VCO1 Wave □ Autre: _______

#### Pin 4 - VCF Sustain (Hypothèse)
**Potard VCF Sustain :**
- Position MIN :0 V
- Position MID :0.44 V
- Position MAX : 0.92 V
- **Plage totale :** 0.92 V
- **Confirmé :** □ VCF Sustain □ Autre: _______

#### Pin 5 - VCA Attack (Hypothèse)
**Potard VCA Attack :**
- Position MIN : -0.66 V
- Position MID : -0.5 V
- Position MAX : -0.28 V
- **Plage totale :** _______ V
- **Confirmé :** □ VCA Attack □ Autre: _______

#### Pin 6 - VCO2 Waveform (Hypothèse)
**Potard Waveform2 :**
- Position MIN : 0V
- Position MID :0 V
- Position MAX : 0 V
- **Plage totale :** _______ V
- **Confirmé :** □ VCO2 Wave □ Autre: _______

#### Pin 9 - VCO2 Volume (Hypothèse)
**Potard Volume2 :**
- Position MIN : 0 V
- Position MID : 0.59 V
- Position MAX : 0.61 V
- **Plage totale :** _______ V
- **Confirmé :** □ VCO2 Volume □ Autre: _______

#### Pin 10 - VCF ADS CTRL (Hypothèse)
**Potard VCF ADS :**
- Position MIN : 0.05 V
- Position MID : 0.59 V
- Position MAX : 0.6 V
- **Plage totale :** _______ V
- **Confirmé :** □ VCF ADS □ Autre: _______

#### Pin 11 - VCO2 Frequency (Hypothèse)
**Potard Freq2 :**
- Position MIN : 0 V
- Position MID : 0 V
- Position MAX : 0 V
- **Plage totale :** _______ V
- **1V/oct :** □ Oui □ Non
- **Confirmé :** □ VCO2 Freq □ Autre: _______

#### Pin 12 - VCF Resonance (Hypothèse)
**Potard Resonance :**
- Position MIN : -0.67 V
- Position MID : _______ V
- Position MAX : 0.61 V
- **Plage totale :** _______ V
- **Confirmé :** □ Resonance □ Autre: _______

#### Pin 13 - VCO1 Frequency (Hypothèse)
**Potard Freq1 :**
- Position MIN : 0 V
- Position MID : _______ V
- Position MAX : 0 V
- **Plage totale :** _______ V
- **1V/oct :** □ Oui □ Non
- **Confirmé :** □ VCO1 Freq □ Autre: _______

#### Pin 14 - VCO1 Volume (Hypothèse)
**Potard Volume1 :**
- Position MIN : 0.13 V
- Position MID : _______ V
- Position MAX : 0.6 V
- **Plage totale :** _______ V
- **Confirmé :** □ VCO1 Volume □ Autre: _______

#### Pin 15 - VCF Cutoff (Hypothèse)
**Potard Cutoff :**
- Position MIN : 0 V
- Position MID : _______ V
- Position MAX : 0.9 V
- **Plage totale :** _______ V
- **Confirmé :** □ VCF Cutoff □ Autre: _______

#### Pin 16 - VCA Sustain (Hypothèse)
**Potard VCA Sustain :**
- Position MIN : -0.03 V
- Position MID : _______ V
- Position MAX : 0.94 V
- **Plage totale :** _______ V
- **Confirmé :** □ VCA Sustain □ Autre: _______

### Pins Mystérieuses

#### Pin 1 - VCA Decay ✅
**Potard VCA Decay :**
- Position MIN : -1.41 V
- Position MID : -1.29 V
- Position MAX : -1.17 V
- **Plage totale :** 0.24 V
- **Confirmé :** ✅ VCA Decay

#### Pin 7 - VCF Decay ✅
**Potard VCF Decay :**
- Position MIN : -1.35 V
- Position MID : -0.52 V
- Position MAX : -0.36 V
- **Plage totale :** 0.99 V
- **Confirmé :** ✅ VCF Decay

#### Pin 8 - VCF Attack ✅
**Potard VCF Attack :**
- Position MIN : -0.66 V
- Position MID : -1.26 V
- Position MAX : -1.15 V
- **Plage totale :** 0.60 V
- **Confirmé :** ✅ VCF Attack

## 📊 Phase 3 : Continuité (Kobol ÉTEINT)

### Résistances Série
| Pin | Vers Jack | Résistance | Vers Potard | Résistance | Notes |
|-----|-----------|------------|-------------|------------|-------|
| 2   | LFO Rate  | _____ Ω    | LFO Rate    | _____ Ω    | R100k? |
| 3   | Waveform1 | _____ Ω    | Waveform1   | _____ Ω    | R82k+100k? |
| ... | ...       | ...        | ...         | ...        | ... |

### Alimentations
- **Pin vers +18V :** Pin ___ = _____ Ω
- **Pin vers -18V :** Pin ___ = _____ Ω  
- **Pin vers +14.5V :** Pin ___ = _____ Ω
- **Pin vers -14.5V :** Pin ___ = _____ Ω
- **Pin vers GND :** Pin ___ = _____ Ω

## 🎯 Conclusions

### Paramètres Confirmés
1. _________________ (Pin ___)
2. _________________ (Pin ___)
3. _________________ (Pin ___)
...

### Corrections Nécessaires
- **Pinout :** _________________
- **Fonctions :** _________________  
- **Tensions :** _________________

### Spécifications Interface
- **Plage CV :** ±_____ V
- **Impédance :** _____ Ω
- **Linéarité :** □ Oui □ Non

---
*Mesures définitives pour conception interface*

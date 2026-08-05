# Problèmes Identifiés - Mesures P1

## ⚠️ **Problèmes Majeurs Découverts**

### **1. Pin 2 = -14.5V - DANGER !**

#### Problématique
- **Pin 2** : -14.5V constant
- **Notre interface** : Alimentation USB 5V (0V référence)
- **Risque** : Court-circuit si connexion directe !

#### Solutions
```
Option A : Éviter Pin 2 complètement
Option B : Isolation avec opto-coupleur  
Option C : Adapter circuit pour gérer -14.5V
```

#### Impact Projet
- **13 pins** utilisables au lieu de 14
- **Pas d'alimentation** depuis P1
- **Circuit** plus complexe si isolation

### **2. VCO Pins = 0V - Signaux Faibles**

#### Pins Problématiques
- **Pin 3,6** : Waveforms VCO1/VCO2 = 0V
- **Pin 11,13** : Frequencies VCO1/VCO2 = 0V
- **Pin 14,15** : Volume1, Cutoff = 0V

#### Hypothèses
1. **Signaux millivolts** : Multimètre pas assez précis
2. **Signaux dynamiques** : Besoin oscilloscope
3. **Activation** : Mode séquenceur requis ?
4. **Vraiment non connectés** : Pins réservées

#### Tests Nécessaires (Oscilloscope)
- **Sensibilité** : Mode mV
- **AC Coupling** : Signaux dynamiques
- **Trigger** : Capture signaux variables
- **FFT** : Analyse fréquentielle

### **3. Analyse Schéma VCO Nécessaire**

#### Objectifs
- **Comprendre** pourquoi VCO pins = 0V
- **Identifier** vrais points de contrôle
- **Localiser** CV pitch (1V/oct)
- **Trouver** LFO Rate réel

## 🔧 **Solutions Techniques**

### **Pour Pin 2 (-14.5V)**

#### Option A : Évitement
```
Connecteur P1 : 15 pins utiles (éviter Pin 2)
Pin 2 : Laisser non connectée
```

#### Option B : Référence Négative
```
Pin 2 (-14.5V) → Diviseur résistif → Référence -2.5V
Pour AOP si besoin tension négative
```

#### Option C : Isolation
```
Pin 2 → Opto-coupleur → Signal logique uniquement
```

### **Pour Signaux Faibles**

#### Attendre Oscilloscope
- **Sensibilité mV** : Détecter signaux faibles
- **Mode AC** : Variations dynamiques
- **Analyse spectrale** : Identifier LFO/enveloppes

#### Tests d'Injection
```
Générateur → Pin 3 → Écouter changement waveform
Confirms si pin fonctionnelle
```

## 🎯 **Révision Architecture**

### **Interface Révisée V1**
```
ESP32-S3 → ┬─ 12× PWM → Pins fonctionnelles P1
           ├─ MCP4822 → CV pitch via jacks face avant
           └─ Pin 2 : NON CONNECTÉE (danger)
```

### **Pins Utilisables Confirmées (12/16)**
1. Pin 1 : VCA Decay
4. Pin 4 : VCF Sustain
5. Pin 5 : VCA Attack  
7. Pin 7 : VCF Decay
8. Pin 8 : VCF Attack
9. Pin 9 : VCO2 Volume
10. Pin 10 : VCF ADS CTRL
12. Pin 12 : VCF Resonance
16. Pin 16 : VCA Sustain

**Éviter :** Pin 2 (-14.5V)
**À investiguer :** Pins 3,6,11,13,14,15 (oscilloscope)

## 🚀 **Prochaines Actions**

### **Immédiat**
1. **Réviser** conception pour éviter Pin 2
2. **Planifier** tests oscilloscope
3. **Adapter** interface pour 12 pins confirmées

### **Avec Oscilloscope**
1. **Analyser** pins 0V (signaux faibles ?)
2. **Localiser** LFO Rate et CV Pitch
3. **Finaliser** pinout complet

### **Sécurité**
- **JAMAIS** connecter Pin 2 à notre circuit 0V !
- **Isolation** obligatoire si utilisation Pin 2

---
*Problèmes identifiés - Solutions en cours*

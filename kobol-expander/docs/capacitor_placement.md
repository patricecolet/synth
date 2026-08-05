# Placement Condensateurs - KobolExpanderControl

## 🎯 **Objectifs Condensateurs**

### **Fonctions Critiques :**
- **Découplage** alimentation (stabilité)
- **Filtrage PWM** (conversion PWM → DC)
- **Anti-parasites** (isolation RF)
- **Stabilité AOP** (oscillations)

## ⚡ **Alimentation - Découplage**

### **Chaque Circuit Intégré :**
```
VCC Pin → [100nF céramique] → GND (proche du CI)
        → [10µF électrolytique] → GND (local)
```

#### **ESP32-S3 :**
- **100nF** sur chaque pin VCC (3.3V)
- **10µF** + **100µF** sur alimentation principale
- **Ferrites** sur câble USB (anti-RF)

#### **4× LM324 :**
- **100nF** sur pin 4 (VCC) de chaque LM324
- **10µF** partagé pour les 4 puces
- **Placement** : Au plus près des pins VCC

#### **TLE2426 (Virtual Ground) :**
- **10µF** sur sortie 2.5V
- **100nF** en parallèle (hautes fréquences)

## 🔄 **Filtrage PWM → DC**

### **Chaque Sortie PWM :**
```
PWM → [R filtrage] → [C filtrage] → AOP
```

#### **Calcul Filtrage :**
```
PWM ESP32 : ~5kHz (14-bit)
Fréquence coupure : fc = 1/(2πRC)

Pour fc = 500Hz (10× sous PWM) :
R = 1kΩ, C = 330nF
ou
R = 3.3kΩ, C = 100nF
```

#### **Composants par Canal :**
```
PWM Pin → R 1kΩ → C 330nF → LM324+
                    ↓
                   GND
```

## 🛡️ **Anti-Parasites**

### **Isolation Numérique/Analogique :**
```
Alimentation numérique → [Ferrite] → Alimentation analogique
Masse numérique → [R 10Ω + C 100nF] → Masse analogique
```

### **Câbles :**
```
Câble USB → Ferrite (anti-RF)
Câble P1 → Blindage + ferrites
```

### **PCB :**
```
Plan de masse → Séparation numérique/analogique
Condensateurs → Placement stratégique
```

## 🔧 **Stabilité AOP**

### **Chaque LM324 :**
```
Sortie AOP → [R 100Ω] → Pin P1 (isolation)
Feedback → [C 22pF] → Compensation (si oscillation)
```

### **Boucle de Masse :**
```
GND Analogique → [C 100µF] → Châssis (si métallique)
```

## 📋 **Liste Condensateurs**

### **Découplage (par fonction) :**
- **ESP32-S3** : 5× 100nF + 2× 10µF + 1× 100µF
- **4× LM324** : 4× 100nF + 1× 10µF
- **TLE2426** : 1× 100nF + 1× 10µF
- **Alimentation** : 2× 100µF + 4× 100nF

### **Filtrage PWM :**
- **16× 330nF** (filtrage PWM → DC)
- **16× 1kΩ** (résistances série)

### **Anti-Parasites :**
- **4× 100nF** (isolation masses)
- **2× Ferrites** (câbles)

## 💰 **Coût Condensateurs**

### **Estimation :**
```
100nF céramique × 25 : 2.50€
10µF électrolytique × 8 : 2.00€  
330nF céramique × 16 : 1.60€
100µF électrolytique × 4 : 2.00€
Ferrites × 2 : 1.00€

Total : ~9€
```

## 📐 **Placement PCB**

### **Règles :**
1. **100nF** : Au plus près des pins VCC
2. **10µF** : Proche des circuits, pas forcément collé
3. **Filtrage PWM** : Entre PWM et AOP
4. **Masses** : Plan continu, vias multiples

### **Zones :**
```
Zone Numérique : ESP32 + découplage
Zone Analogique : AOP + filtrage
Zone Interface : Connecteurs + protection
```

## 🎯 **Priorités Placement**

### **Critiques :**
1. **100nF ESP32** : Stabilité MCU
2. **330nF PWM** : Filtrage parasites
3. **100nF LM324** : Stabilité AOP

### **Importantes :**
1. **10µF TLE2426** : Virtual ground stable
2. **100µF alimentation** : Réserve énergie
3. **Ferrites** : Isolation RF

### **Optionnelles :**
1. **22pF compensation** : Si oscillation AOP
2. **100µF châssis** : Si boîtier métallique

---

## 🚀 **Résumé**

**Condensateurs = Fondation stabilité !**
- **Découplage** : Chaque CI protégé
- **Filtrage** : PWM → DC propre
- **Anti-parasites** : Isolation RF
- **Coût** : ~9€ pour stabilité totale

**Placement soigné = Interface fiable !** 🎯✨

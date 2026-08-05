# Sélection AOP - Adaptation Tension

## 🎯 Cahier des Charges

### Fonction
**Adaptation 0-5V → ±2.5V** pour interface P1

### Spécifications
- **Entrée** : 0-5V (DAC MCP4822 / TLC5940 PWM filtré)
- **Sortie** : ±2.5V (interface P1 Kobol)
- **Alimentation** : 5V USB + Virtual Ground
- **Nombre de canaux** : 16 (pour tous les paramètres)

## 🔧 Circuit d'Adaptation

### Configuration de base
```
Gain = -1, Offset = -2.5V

Vin (0-5V) → AOP → Vout = -(Vin - 2.5V) = -Vin + 2.5V
```

**Résultat :**
- Vin = 0V → Vout = +2.5V
- Vin = 2.5V → Vout = 0V  
- Vin = 5V → Vout = -2.5V

## 🎛️ AOP Recommandés

### Option 1 : TL074 (Quad)
**Avantages :**
- ✅ **4 canaux** par boîtier
- ✅ **Standard** audio/synthé
- ✅ **Faible bruit**
- ✅ **±15V** compatible
- ✅ **Économique**

**Inconvénients :**
- ❌ **4 boîtiers** nécessaires (16 canaux)

### Option 2 : LM324 (Quad)
**Avantages :**
- ✅ **Très économique**
- ✅ **Single supply** possible
- ✅ **Robuste**

**Inconvénients :**
- ❌ **Performances** limitées
- ❌ **4 boîtiers** nécessaires

### Option 3 : TL072 (Dual)
**Avantages :**
- ✅ **Excellentes** performances audio
- ✅ **Standard** synthé
- ✅ **Faible bruit**

**Inconvénients :**
- ❌ **8 boîtiers** nécessaires

### Option 4 : AOP Single (TL071)
**Avantages :**
- ✅ **Flexibilité** max
- ✅ **Performances** optimales

**Inconvénients :**
- ❌ **16 boîtiers** nécessaires
- ❌ **Coût** et **place**

## 🎯 Recommandation Finale

### **LM324 (Quad) + TLE2426 - OPTIMAL**

**Pourquoi :**
- **Single supply** 5V USB (pas besoin ±V)
- **Très économique** (~4€ pour 16 canaux)
- **Robuste** et disponible partout
- **Virtual ground** TLE2426 = ±2.5V parfait
- **Suffisant** pour adaptation CV

**Configuration :**
```
USB 5V → TLE2426 → Virtual GND (2.5V)
       → 4× LM324 → 16 canaux adaptation
```

### Circuit type par canal
```
Vin (0-5V) ──[R1]──┬─── AOP+ 
                    │
              [R2]──┴─── Virtual GND (2.5V)
                    
AOP- ──[R3]──┬─── Vout (±2.5V)
             │
        [R4]─┴─── Virtual GND (2.5V)

TLE2426: USB 5V → Virtual GND 2.5V
```

**Valeurs typiques :**
- R1 = R2 = R3 = R4 = 10kΩ
- TLE2426 fournit référence stable 2.5V
- Gain = -1, Offset = +2.5V

## 💡 Alternatives

### **Option Premium : MCP6004**
- **Rail-to-rail** entrée/sortie
- **Meilleures performances** que LM324
- **Plus cher** mais qualité supérieure

### **Option Budget : LM358**
- **8 boîtiers** dual au lieu de 4 quad
- **Encore plus économique**
- **Performances similaires** LM324

## 📋 Composants Finaux

**Liste de courses V1 :**
- **4× LM324** (16 canaux) ~4€
- **1× TLE2426** (virtual ground) ~2€
- **Résistances 1%** 10kΩ pour précision
- **Condensateurs** découplage 100nF
- **Alimentation** : USB 5V uniquement

**Total :** ~10€ pour 16 canaux d'adaptation !

## 🎯 Validation Technique

**LM324 Specs :**
- **Alimentation** : 3-32V (5V USB ✅)
- **Swing sortie** : Rail-to-rail -1.5V (±2.5V ✅)
- **Slew rate** : 0.5V/µs (suffisant CV ✅)
- **Offset** : <7mV (négligeable ✅)

---
*Choix optimal économique et fiable pour CV*

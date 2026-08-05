# Étude Convertisseur Exponentiel - Remplacement DAC

## 🎯 Objectif
Remplacer le convertisseur exponentiel analogique du Kobol par un système DAC numérique haute précision.

## ⚡ Problématique Résolution Exponentielle

### Fonction Exponentielle
```
f(V) = f₀ × 2^V
df/dV = f₀ × ln(2) × 2^V = f × ln(2)
```

**La sensibilité est PROPORTIONNELLE à la fréquence !**

### Paradoxe Résolution
- **Graves** : Beaucoup de pas DAC pour peu de Hz
- **Aigus** : Peu de pas DAC pour beaucoup de Hz
- **Résultat** : Résolution musicale constante !

## 📊 Calculs de Résolution

### Formule Générale
```
Pas nécessaires par cent = 0.0006 × 65536 / ln(2) = 56.6 pas/cent
```

**Résolution constante sur toute la gamme !**

### Validation par Octave

#### DAC 16-bit sur 10 octaves (0-10V)

**Octave 1 (40-80Hz) :**
- Pas DAC = 0.42 mHz
- 1 cent = 24 mHz  
- Résolution = **57 pas/cent** ✅

**Octave 10 (10240-20480Hz) :**
- Pas DAC = 0.108 Hz
- 1 cent = 6 Hz
- Résolution = **56 pas/cent** ✅

### Comparaison Résolutions

| DAC | Pas/Cent | Résolution | Qualité |
|-----|----------|------------|---------|
| 12-bit | 3.5 | 0.3 cents | ❌ Insuffisant |
| 14-bit | 14 | 0.07 cents | ⚠️ Limite |
| 16-bit | 56 | 0.018 cents | ✅ Excellent |
| 18-bit | 226 | 0.004 cents | ⭐ Overkill |

## 🎵 Seuils Audibles

- **5 cents** : Seuil détection humaine
- **1 cent** : Musicien professionnel  
- **0.1 cent** : Instruments précision
- **0.01 cent** : Limite physique

**DAC 16-bit = 0.018 cents = Précision professionnelle !**

## 🔧 Architecture Technique

### Chaîne de Conversion
```
CV Input → ADC 16-bit → Calcul Expo → DAC 16-bit → VCO
```

### Algorithme Critique
```cpp
float cv_volt = adc_reading * (10.0/65536.0);  // 0-10V
float frequency = f0 * pow(2.0, cv_volt);      // Exponentiel
uint16_t dac_value = freq_to_dac_table[cv_volt]; // Lookup optimisé
```

### Optimisations
- **Lookup table** : Calcul pré-calculé
- **Interpolation** : Précision entre points
- **Calibration** : Compensation VCO individuel

## 🎯 DAC Recommandés

### DAC8831 (16-bit)
- **Résolution** : 0.018 cents ✅
- **Prix** : ~10€
- **Interface** : SPI simple
- **Linéarité** : ±1 LSB

### AD5791 (20-bit)  
- **Résolution** : 0.004 cents (overkill)
- **Prix** : ~50€
- **Performances** : Exceptionnelles

### MCP4728 (12-bit)
- **Résolution** : 0.3 cents ❌
- **Prix** : ~3€  
- **Insuffisant** pour pitch précis

## 🚀 Validation Expérimentale

### Test Résolution
```cpp
// Vérifier cohérence résolution
for(int octave = 1; octave <= 10; octave++) {
  float freq = 40.0 * pow(2, octave-1);
  float cent_step = freq * 0.0006;
  float dac_step = freq * ln(2) / 65536;
  float resolution = cent_step / dac_step;
  // Doit être ~56 pour toutes octaves
}
```

### Mesures Nécessaires
- **Linéarité** DAC sur 10V
- **Stabilité** température
- **Bruit** résiduel
- **Précision** 1V/oct

## 🎵 Impact Musical

### Tempéraments Programmables
```cpp
float just_ratio[] = {1.0, 16/15, 9/8, 6/5, 5/4, 4/3...};
float equal_temp = pow(2.0, semitone/12.0);
```

### Micro-Tuning
- **Gammes** arabes, indiennes
- **Accordages** historiques
- **Justesse** harmonique parfaite

## 🔬 Recherche Future

### Analyse Kobol Original
- **Schéma** convertisseur expo actuel
- **Caractéristiques** VCO response
- **Points** d'injection CV

### Développement Algorithmes
- **Lookup tables** optimisées
- **Interpolation** haute précision
- **Calibration** automatique

---
*Révolution : Précision numérique + Caractère analogique*

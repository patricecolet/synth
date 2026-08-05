# Protocole de Mesures - Connecteur P1

## 🎯 Objectif
Mesurer systématiquement le connecteur P1 pour valider notre analyse et créer le pinout définitif.

## ⚠️ SÉCURITÉ PREMIÈRE

### Précautions
- **Kobol éteint** pour mesures continuité
- **Kobol allumé** pour mesures tension (attention ±18V !)
- **Multimètre** en bon état
- **Sondes** propres et isolées

## 📐 Phase 1 : Identification Physique

### Pinout Définitif
```
P1 - Socket femelle 2×8 pins (vue côté composant)
 1  2  3  4  5  6  7  8
16 15 14 13 12 11 10  9

⚠️ CRITICAL : Vérifier Pin 1 avec marquage/encoche !
```

### Checklist Physique
- [ ] **Pin 1** : Localiser marquage/encoche
- [ ] **Orientation** : Confirmer sens horaire
- [ ] **Accessibilité** : Toutes pins accessibles ?
- [ ] **État** : Pins propres, pas d'oxydation

## 📊 Phase 2 : Mesures Continuité (Kobol ÉTEINT)

### Protocole
```
Pour chaque pin P1 :
├─ Continuité vers jacks face avant
├─ Continuité vers potentiomètres  
├─ Continuité vers alimentations
└─ Résistance vers masse
```

### Tableau de Mesures
| Pin | Jack Face Avant | Potard | Alim | Résistance | Notes |
|-----|-----------------|--------|------|------------|-------|
| 1   | ?               | ?      | ?    | ?          | À mesurer |
| 2   | ?               | ?      | ?    | ?          | À mesurer |
| ... | ...             | ...    | ...  | ...        | ... |

## ⚡ Phase 3 : Mesures Tension (Kobol ALLUMÉ)

### ⚠️ ATTENTION : ±18V présent !

### Protocole Sécurisé
```
1. Multimètre → Mode DC Voltage, calibre 20V
2. Sonde noire → Masse Kobol (jack sleeve)
3. Sonde rouge → Pin P1 (avec précaution)
4. Noter tension repos
5. Varier potard correspondant
6. Noter tension min/max
```

### Tableau Tensions
| Pin | V Repos | V Min | V Max | Delta | Paramètre Confirmé |
|-----|---------|-------|-------|-------|-------------------|
| 1   | ?       | ?     | ?     | ?     | ?                 |
| 2   | ?       | ?     | ?     | ?     | LFO Rate ?        |
| ... | ...     | ...   | ...   | ...   | ...               |

## 🔍 Phase 4 : Validation Analyse

### Correspondances à Vérifier
```
Pin 2 → LFO Rate (R 100kΩ)
Pin 3 → VCO1 Waveform (R 82kΩ + 100kΩ)
Pin 4 → VCF Sustain (R 68kΩ + 82kΩ)
Pin 5 → VCA Attack (R 82kΩ + 68kΩ)
Pin 6 → VCO2 Waveform (R 100kΩ)
Pin 9 → VCO2 Volume (R 27kΩ + 33kΩ)
Pin 10 → VCF ADS CTRL (R 27kΩ + 33kΩ)
Pin 11 → VCO2 Frequency (R 78.7kΩ + 100kΩ)
Pin 12 → VCF Resonance (R 10kΩ + 10kΩ)
Pin 13 → VCO1 Frequency (R 78.7kΩ + 100kΩ)
Pin 14 → VCO1 Volume (R 27kΩ + 33kΩ)
Pin 15 → VCF Cutoff (R 100kΩ)
Pin 16 → VCA Sustain (R 68kΩ + 82kΩ)
```

### Tests Fonctionnels
- [ ] **Varier potard** → **Tension P1 change** ?
- [ ] **Plage tension** → **±2.5V confirmé** ?
- [ ] **Linéarité** → **Réponse proportionnelle** ?

## 📋 Phase 5 : Documentation Finale

### Pinout Vérifié
```
Pin X → Fonction → Plage → Résistances → Status
```

### Corrections Nécessaires
- **Fonctions** mal identifiées
- **Numérotation** incorrecte  
- **Tensions** différentes d'estimé

## 🎯 Conseils Pratiques

### Ordre de Mesures
1. **Continuité d'abord** (sécurité)
2. **Tensions repos** (vue d'ensemble)
3. **Variations potards** (confirmation fonctions)
4. **Validation croisée** (cohérence)

### Points d'Attention
- **Pin 1** : Vraiment libre ou cachée ?
- **Pins 7,8** : Connexions vers carte synthé
- **Alimentations** : ±18V, ±14.5V présentes ?
- **Masses** : Continuité système

## 🔧 Matériel Nécessaire
- **Multimètre** précis
- **Sondes** fines pour pins
- **Carnet** pour notes
- **Appareil photo** pour documentation

## 🚀 Résultats Attendus

### Validation
- **13 paramètres** confirmés
- **3 pins mystérieuses** élucidées
- **Plages tensions** précises
- **Pinout définitif** pour connecteur mâle

### Corrections Possibles
- **Renommage** pins selon mesures
- **Révision** résistances
- **Ajustement** estimations tensions

---

## 📝 Notes de Mesures

**Date :** ___________
**Conditions :** ___________
**Multimètre :** ___________

**Observations importantes :**
- 
- 
- 

---
*Protocole pour validation définitive interface P1*

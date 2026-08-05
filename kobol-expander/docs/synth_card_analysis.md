# Analyse Carte Synthé - RSF Kobol Expander

## 🎯 Objectif
Analyser la carte synthé pour confirmer les connexions des pins mystérieuses de P1.

## 🔍 Pins à Élucider

### Pin 1 - VCA Decay (Hypothèse)
- **Position** : À côté du circuit VCA Decay sur carte synthé
- **À vérifier** : Connexion, résistances, jack/potard associés

### Pin 7 - VCF Decay (Hypothèse)  
- **Position** : À côté du circuit VCF Decay sur carte synthé
- **À vérifier** : Connexion, résistances, jack/potard associés

### Pin 8 - VCF Attack (Hypothèse)
- **Position** : À côté du circuit VCF Attack sur carte synthé  
- **À vérifier** : Connexion, résistances, jack/potard associés

## 📋 Méthodologie

### Analyse Visuelle
- [ ] Identifier circuits enveloppes sur carte synthé
- [ ] Tracer connexions depuis P1 pins 1, 7, 8
- [ ] Noter composants et valeurs résistances
- [ ] Identifier jacks/potards associés

### Documentation
- [ ] Schéma bloc enveloppes VCA/VCF
- [ ] Confirmation fonctions pins P1
- [ ] Architecture complète enveloppes

## 📊 Résultats Attendus

**Si confirmé :**
- Interface P1 = **16 paramètres complets**
- Enveloppes **ADS** complètes VCA + VCF (pas de Release)
- Documentation technique complète

**Architecture ADS typique années 70 :**
- **VCA** : Attack (Pin 5) + Decay (Pin 1) + Sustain (Pin 16)
- **VCF** : Attack (Pin 8) + Decay (Pin 7) + Sustain (Pin 4)

---
*À remplir lors de l'analyse physique de la carte synthé*

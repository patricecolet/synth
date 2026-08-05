# Analyse Connecteur Séquenceur RSF Kobol

## 📋 État de l'Analyse
**Statut** : 🔄 En attente oscilloscope  
**Dernière mise à jour** : Septembre 2025

## 🎯 Objectifs
Documenter complètement le connecteur séquenceur pour interface bidirectionnelle non-invasive.

## 📐 Analyse Physique

### Type de Connecteur
- [x] **Identifié** : Socket femelle 2.54mm (0.1")
- [x] **Nombre de pins** : 2×8 = 16 pins total
- [x] **Espacement pins** : 2.54mm standard
- [x] **Configuration** : Double rangée, séparation 3 pas (7.62mm)
- [ ] **Marquage/Numérotation** : À identifier

### Localisation
- [ ] **Position** sur l'Expander
- [ ] **Accessibilité** pour câblage externe
- [ ] **Mécanisme** de verrouillage

## ⚡ Mesures Électriques

### Équipement Nécessaire
- ✅ **Multimètre** : Disponible
- ⏳ **Oscilloscope** : À acquérir
- [ ] **Générateur signaux** : Si nécessaire pour tests

### Protocole de Mesures

#### Phase 1 : Mesures Statiques (Multimètre)
```
Pour chaque pin :
├─ Tension au repos (potentiomètres à zéro)
├─ Tension maximale (potentiomètres à fond)
├─ Points intermédiaires (25%, 50%, 75%)
├─ Impédance (résistance série)
└─ Correspondance avec paramètre audible
```

#### Phase 2 : Mesures Dynamiques (Oscilloscope)
```
Pour signaux variables :
├─ Forme d'onde LFO
├─ Enveloppes ADSR
├─ Signaux gate/trigger
├─ Fréquence et amplitude
└─ Timing et synchronisation
```

## 📊 Résultats de Mesures

### Tableau Pinout P1 (2×8 pins)
| Pin | Fonction | Connexion Schéma | V Min | V Max | Type | Notes |
|-----|----------|------------------|-------|-------|------|-------|
| 1   | VCA Decay (prob.) | À confirmer par analyse carte synthé | ?     | ?     | CV In | Contrôle Decay enveloppe VCA |
| 2   | LFO Rate | R 100kΩ → Jack + LFO card | ?     | ?     | CV In | Entrée CV externe |
| 3   | VCO1 Waveform | R 82kΩ → (R 100kΩ → Jack) + carte synthé | ?     | ?     | CV In | Morphing forme d'onde VCO1 |
| 4   | VCF Sustain | Triple destination: R68kΩ→Potard + R82kΩ→Jack + Direct carte | ?     | ?     | CV In | Contrôle Sustain enveloppe VCF |
| 5   | VCA Attack | R82kΩ→(Jack + R68kΩ→Potard) + Direct carte | ?     | ?     | CV In | Contrôle Attack enveloppe VCA |
| 6   | VCO2 Waveform | R 100kΩ → Jack + Direct carte synthé | ?     | ?     | CV In | Contrôle forme d'onde VCO2 |
| 7   | VCF Decay (prob.) | À confirmer par analyse carte synthé | ?     | ?     | CV In | Contrôle Decay enveloppe VCF |
| 8   | VCF Attack (prob.) | À confirmer par analyse carte synthé | ?     | ?     | CV In | Contrôle Attack enveloppe VCF |
| 9   | VCO2 Volume | R27kΩ→Potard + R33kΩ→Jack + Direct carte | ?     | ?     | CV In | Contrôle niveau VCO2 |
| 10  | VCF ADS CTRL | R27kΩ→Potard + R33kΩ→Jack + Direct carte | ?     | ?     | CV In | Contrôle global enveloppe VCF |
| 11  | VCO2 Freq | R78.7kΩ→Potard + R100kΩ→Jack + Direct carte | ?     | ?     | CV In | Contrôle fréquence VCO2 (1V/oct) |
| 12  | VCF Resonance | R10kΩ→Potard + R10kΩ→Jack + Direct carte | ?     | ?     | CV In | Contrôle résonance filtre |
| 13  | VCO1 Freq | R78.7kΩ→Potard + R100kΩ→Jack + Direct carte | ?     | ?     | CV In | Contrôle fréquence VCO1 (1V/oct) |
| 14  | VCO1 Volume | R27kΩ→Potard + R33kΩ→Jack + Direct carte | ?     | ?     | CV In | Contrôle niveau VCO1 |
| 15  | VCF Cutoff | R 100kΩ → Jack + Direct carte | ?     | ?     | CV In | Contrôle fréquence coupure VCF |
| 16  | VCA Sustain | R68kΩ→Potard + R82kΩ→Jack + Direct carte | ?     | ?     | CV In | Contrôle Sustain enveloppe VCA |

### Description Schéma (d'après observation)

**Architecture générale des potentiomètres :**
```
Potentiel + (haut) ────┐
                       │
Signal de contrôle ────┼──── Curseur potard
                       │
Potentiel - (bas) ─────┘

Valeurs des potentiels : À déterminer par mesures
IMPORTANT: Socket P1 connecté APRÈS les résistances de protection
→ Plage CV probablement réduite (±2.5V estimé)
```

**Configuration P1 confirmée :**
```
P1 - Socket femelle 2×8 pins
Séparation : 3 pas (7.62mm)
Espacement : 2.54mm

NOMENCLATURE STANDARD DIP (côté composant = sens horaire) :
Vue côté composant (socket femelle) :
 1  2  3  4  5  6  7  8
16 15 14 13 12 11 10  9

CRITICAL : Cette nomenclature détermine le connecteur mâle !
```

**Connexions par pin :**

**Pin 1** : → Probablement VCA Decay (à côté sur carte synthé)
**Pin 2** : → R 100kΩ → Jack input "LFO Rate" + connexion vers carte LFO
**Pin 3** : → R 82kΩ → se divise en 2 branches :
           ├─ R 100kΩ → Jack "Waveform1" 
           └─ Direct vers carte synthé (circuit wave morphing)
**Pin 4** : → Se divise en 2 branches principales :
           ├─ Se subdivise en :
           │  ├─ R 68kΩ → Potard VCF Sustain (panneau)
           │  └─ R 82kΩ → Jack "VCF Sustain"
           └─ Direct vers carte synthé (enveloppe VCF)
**Pin 5** : → Se divise en 2 branches principales :
           ├─ R 82kΩ → se subdivise en :
           │  ├─ Jack "VCA Attack"
           │  └─ R 68kΩ → Potard Attack (panneau)
           └─ Direct vers carte synthé (enveloppe VCA)
**Pin 6** : → Se divise en 2 branches :
           ├─ R 100kΩ → Jack "VCO2 Waveform"
           └─ Direct vers carte synthé
**Pin 7** : → Probablement VCF Decay (à côté sur carte synthé)
**Pin 8** : → Probablement VCF Attack (à côté sur carte synthé)
**Pin 9** : → Se divise en 2 branches principales :
           ├─ Se subdivise en :
           │  ├─ R 27kΩ → Potard Volume2 (panneau)
           │  └─ R 33kΩ → Jack "Volume2"
           └─ Direct vers carte synthé
**Pin 10** : → Se divise en 3 branches :
            ├─ Direct vers carte synthé
            ├─ R 33kΩ → Jack "VCF ADS CTRL"
            └─ R 27kΩ → Potard VCF ADS CTRL (panneau)
**Pin 11** : → Se divise en 3 branches :
            ├─ Direct vers carte synthé
            ├─ R 100kΩ → Jack "Freq2"
            └─ R 78.7kΩ → Potard Freq2 (panneau)
**Pin 12** : → Se divise en 3 branches :
            ├─ Direct vers carte synthé
            ├─ R 10kΩ → Jack "Resonance"
            └─ R 10kΩ → Potard Resonance (panneau)
**Pin 13** : → Se divise en 3 branches :
            ├─ Direct vers carte synthé
            ├─ R 100kΩ → Jack "Freq1"
            └─ R 78.7kΩ → Potard Freq1 (panneau)
**Pin 14** : → Se divise en 3 branches :
            ├─ Direct vers carte synthé
            ├─ R 33kΩ → Jack "Volume1"
            └─ R 27kΩ → Potard Volume1 (panneau)
**Pin 15** : → Se divise en 2 branches :
            ├─ Direct vers carte synthé
            └─ R 100kΩ → Jack "VCF Freq"
**Pin 16** : → Se divise en 3 branches :
            ├─ Direct vers carte synthé
            ├─ R 82kΩ → Jack "VCA Sustain"
            └─ R 68kΩ → Potard VCA Sustain (panneau)

### Correspondances Paramètres
| Paramètre Kobol | Pin | Plage Tension | Linéarité | Inversé ? |
|-----------------|-----|---------------|-----------|-----------|
| VCO1 Pitch      | ?   | ?             | ?         | ?         |
| VCO2 Pitch      | ?   | ?             | ?         | ?         |
| Filter Cutoff   | ?   | ?             | ?         | ?         |
| Filter Res      | ?   | ?             | ?         | ?         |
| ... | ... | ... | ... | ... |

### Signaux Dynamiques
| Signal | Pin | Forme | Fréquence | Amplitude | Notes |
|--------|-----|-------|-----------|-----------|-------|
| LFO    | ?   | ?     | 0.1-20Hz  | ?         | À mesurer |
| Env    | ?   | ?     | Variable  | ?         | À mesurer |
| Gate   | ?   | ?     | Note dep. | ?         | À mesurer |

## 🔧 Analyse Technique

### Interface Non-Invasive
- **P1** : Connecteur dédié séquenceur externe
- **Signaux CV** : Entrées après résistances de protection
- **Plage estimée** : ±2.5V (atténuée par résistances)
- **Addition** : CV externe + potentiomètre = contrôle hybride

### Niveaux de Tension
- **Alimentation Kobol** : ±18V, ±14.5V disponibles
- **Interface P1** : ±2.5V estimé (après résistances)
- **Notre DAC/PWM** : 0-5V → adaptation simple vers ±2.5V

### Architecture Simplifiée
```
Teensy → DAC/TLC5940 → Adaptation ±2.5V → P1 → Kobol
         (0-5V)        (AOP simple)        (CV)
```

**Avantages :**
- **Aucune modification** du Kobol
- **Plug & Play** via P1
- **Circuit simple** et fiable

## 📝 Notes de Terrain

### Observations
- Tensions "faibles" observées (à quantifier)
- Connecteur prévu pour séquenceur externe
- Interface conçue pour contrôle bidirectionnel

### Questions à Résoudre
1. **Type exact** du connecteur ?
2. **Pinout officiel** disponible ?
3. **Compatibilité** entre versions Expander ?
4. **Séquenceur original** : spécifications ?

### Ressources Identifiées
- Forum ModWiggler : discussions techniques
- Scribd : schémas partiels Expander II
- Communauté Synth-DIY : expérience utilisateurs

---

## 🚀 Prochaines Étapes

### Phase 1 : Validation
1. **Mesures électriques** P1 (multimètre + oscilloscope)
2. **Confirmation pins 1, 7, 8** via analyse carte synthé
3. **Test plages de tension** acceptables

### Phase 2 : Développement
1. **Circuit d'adaptation** 0-5V → ±2.5V
2. **Code Teensy étendu** pour 16 paramètres
3. **Interface MIDI complète**

### Phase 3 : Intégration
1. **Boîtier externe** avec P1 connecté
2. **Tests fonctionnels** complets
3. **Documentation utilisateur**

---
*Document vivant - Mise à jour au fur et à mesure des découvertes*

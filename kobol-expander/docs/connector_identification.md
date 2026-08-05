# Guide d'Identification Connecteur Séquenceur Kobol

## 🎯 Objectif
Identifier précisément le type de connecteur pour choisir l'interface appropriée.

## 🔍 Types de Connecteurs Vintage (années 70)

### DIN (Deutsches Institut für Normung)
**Caractéristiques :**
- **Circulaire** avec broches disposées en arc
- **Tailles courantes** : 3, 5, 7, 8 broches
- **MIDI** utilise DIN 5 broches (mais MIDI = années 80)
- **Verrouillage** par rotation ou clip

**Identification :**
```
    3     5     7     8
   ●●●   ●●●●●  ●●●●●●● ●●●●●●●●
    ●     ●●●    ●●●   ●●●●
```

### Edge Connector (Connecteur à Peigne)
**Caractéristiques :**
- **PCB inséré** dans connecteur femelle
- **Contacts dorés** sur bord de carte
- **Espacement** : 2.54mm (0.1") ou 1.27mm
- **Courant** pour ordinateurs/cartes d'extension

**Identification :**
```
Mâle (sur PCB):     Femelle (sur boîtier):
================    ┌─────────────────┐
||||||||||||||||    │                 │
================    │ ≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡ │
                    └─────────────────┘
```

### D-Sub (D-Subminiature)
**Caractéristiques :**
- **Forme trapézoïdale** en D
- **Tailles** : DB-9, DB-15, DB-25
- **Vis de fixation** sur les côtés
- **Utilisé** pour RS-232, vidéo

**Identification :**
```
DB-9:        DB-15:       DB-25:
●●●●●        ●●●●●●●●     ●●●●●●●●●●●●●
 ●●●●         ●●●●●●●      ●●●●●●●●●●●●
```

### Connecteurs Propriétaires
**Caractéristiques :**
- **Spécifiques** au fabricant
- **Formes diverses** : rectangulaire, circulaire
- **Détrompeur** pour éviter mauvaises connexions
- **Souvent** avec verrouillage spécial

## 📋 Checklist d'Identification

### Étape 1 : Observation Visuelle
- [ ] **Forme générale** : Circulaire / Rectangulaire / Trapézoïdale / Autre ?
- [ ] **Nombre de contacts** : Compter précisément
- [ ] **Disposition contacts** : En ligne / En arc / Matrice ?
- [ ] **Taille approximative** : Mesurer diamètre ou dimensions
- [ ] **Couleur** : Plastique noir / blanc / métal ?

### Étape 2 : Marquages
- [ ] **Étiquettes** sur le boîtier près du connecteur
- [ ] **Sérigraphie** sur le PCB
- [ ] **Numéros** ou lettres sur les pins
- [ ] **Symboles** ou icônes

### Étape 3 : Mécanisme
- [ ] **Verrouillage** : Vis / Clip / Rotation / Friction ?
- [ ] **Orientation** : Détrompeur visible ?
- [ ] **Accessibilité** : Facilement accessible ou encastré ?

### Étape 4 : Contexte
- [ ] **Autres connecteurs** sur l'Expander (pour comparaison)
- [ ] **Époque** : Standards années 70 vs modernes
- [ ] **Fonction** : Audio / Contrôle / Alimentation / Données

## 📸 Documentation Recommandée

### Photos à Prendre
1. **Vue d'ensemble** : Connecteur dans son contexte
2. **Gros plan frontal** : Disposition des pins
3. **Vue de profil** : Mécanisme de verrouillage
4. **Marquages** : Étiquettes et sérigraphie
5. **Comparaison taille** : Avec objet de référence (pièce, règle)

### Mesures à Effectuer
- **Diamètre externe** (si circulaire)
- **Dimensions** L×l×h (si rectangulaire)
- **Espacement pins** (centre à centre)
- **Diamètre pins** (si accessible)
- **Profondeur insertion**

## 🔧 Types Probables pour Kobol

### Hypothèse 1 : DIN 5 ou 8 Broches
**Probabilité** : ⭐⭐⭐⭐
- Standard européen années 70
- Utilisé pour contrôles analogiques
- Robuste et fiable

### Hypothèse 2 : Connecteur Propriétaire
**Probabilité** : ⭐⭐⭐
- RSF était fabricant français
- Interface spécifique séquenceur
- Optimisé pour leur système

### Hypothèse 3 : Edge Connector
**Probabilité** : ⭐⭐
- Économique à produire
- Beaucoup de contacts possibles
- Utilisé sur cartes d'extension

## 🛠️ Solutions d'Interface

### Si DIN Standard
- **Connecteurs** : Disponibles facilement
- **Câblage** : Direct
- **Adaptateurs** : Possibles si besoin

### Si Propriétaire
- **Reverse engineering** : Analyser mécaniquement
- **Fabrication custom** : PCB ou connecteur 3D
- **Adaptation** : Câbles volants

### Si Edge Connector
- **PCB interface** : Créer carte d'adaptation
- **Connecteur femelle** : Standard industriel
- **Fiabilité** : Attention aux contacts

## 📝 Formulaire d'Identification

**Remplir après examen physique :**

```
Date : ___________
Observateur : ___________

FORME :
□ Circulaire  □ Rectangulaire  □ Trapézoïdale  □ Autre: _______

TAILLE :
Diamètre/Longueur : _____ mm
Largeur : _____ mm
Hauteur : _____ mm

CONTACTS :
Nombre total : _____
Disposition : □ Ligne  □ Arc  □ Matrice  □ Autre: _______
Espacement : _____ mm

MARQUAGE :
Étiquette : _______________
Sérigraphie : _______________
Numérotation : _______________

MÉCANISME :
□ Vis  □ Clip  □ Rotation  □ Friction  □ Autre: _______

IDENTIFICATION PROBABLE :
□ DIN __ broches
□ D-Sub DB-__
□ Edge Connector __ contacts
□ Propriétaire
□ Autre: _______________
```

---

## 🚀 Prochaines Actions

1. **Examen physique** avec ce guide
2. **Documentation photo** complète
3. **Recherche** avec identification précise
4. **Choix interface** en conséquence

---
*Guide pour identification précise du connecteur séquenceur*

# TODO - KobolExpanderControl

## 📋 Plan de Développement

### Phase 1 : Reverse Engineering & Documentation
**🎯 Objectif** : Comprendre complètement l'interface Kobol

#### En attente matériel
- [ ] **Acquisition oscilloscope** 
  - Nécessaire pour analyse signaux dynamiques
  - Budget : ~200-500€ pour modèle basique suffisant

#### Documentation connecteur séquenceur
- [ ] **Analyse physique**
  - [ ] Type connecteur (DIN, edge, autre ?)
  - [ ] Nombre de pins/broches
  - [ ] Marquages éventuels
  
- [ ] **Mesures électriques** (multimètre)
  - [ ] Tensions repos sur chaque pin
  - [ ] Variations avec potentiomètres
  - [ ] Identification entrées vs sorties
  - [ ] Mesure impédances
  
- [ ] **Analyse dynamique** (avec oscilloscope)
  - [ ] Formes d'ondes LFO
  - [ ] Enveloppes ADSR
  - [ ] Signaux gate/trigger
  - [ ] Timing et synchronisation

#### Livrables Phase 1
- [ ] **Datasheet connecteur** complet
- [ ] **Pinout vérifié** avec fonctions identifiées
- [ ] **Plages de tensions** documentées
- [ ] **Correspondance paramètres** ↔ pins

---

### Phase 2 : Conception Circuit Étendu
**🎯 Objectif** : Schéma complet du système

#### Architecture électronique
- [ ] **Schéma global**
  - [ ] Teensy 2.0 + périphériques
  - [ ] Routage SPI partagé (DAC + TLC5940)
  - [ ] Multiplexeur ADC pour lecture
  - [ ] Isolation optique complète

- [ ] **Alimentation isolée**
  - [ ] Régulateurs ±18V → ±15V, ±12V, +5V
  - [ ] Calcul dissipation thermique
  - [ ] Protection et filtrage
  - [ ] Masses séparées

- [ ] **Interface analogique**
  - [ ] Conditionnement signaux TLC5940
  - [ ] Filtrage RC sorties PWM
  - [ ] Amplification signaux faibles
  - [ ] Adaptation niveaux

#### Sélection composants
- [ ] **Opto-coupleurs**
  - [ ] Signaux numériques : 6N137, PC817
  - [ ] Signaux analogiques : HCNR200, IL300
  - [ ] Calcul performances requises

- [ ] **Multiplexeur ADC**
  - [ ] CD4067 (16 canaux) ou CD4051 (8 canaux)
  - [ ] Timing et séquencement
  - [ ] Résolution vs vitesse

#### Livrables Phase 2
- [ ] **Schéma électronique** complet
- [ ] **Liste composants** avec références
- [ ] **PCB layout** préliminaire
- [ ] **Calculs dimensionnement**

---

### Phase 3 : Développement Logiciel
**🎯 Objectif** : Code complet et robuste

#### Architecture logicielle
- [ ] **Structure modulaire**
  - [ ] Gestionnaire MIDI
  - [ ] Contrôleur DAC/TLC5940
  - [ ] Gestionnaire ADC/lecture
  - [ ] Système presets
  - [ ] Interface utilisateur

- [ ] **Gestion matérielle**
  - [ ] Driver TLC5940 optimisé
  - [ ] SPI partagé avec arbitrage
  - [ ] ADC multiplexé séquentiel
  - [ ] Calibration automatique

#### Fonctionnalités avancées
- [ ] **Système presets**
  - [ ] Capture état complet
  - [ ] Stockage EEPROM/Flash
  - [ ] Recall avec anti-jump
  - [ ] Morphing entre presets

- [ ] **Interface MIDI étendue**
  - [ ] Mapping CC configurable
  - [ ] Program Change → Presets
  - [ ] SysEx pour configuration
  - [ ] MIDI Learn automatique

#### Livrables Phase 3
- [ ] **Code source** documenté
- [ ] **Bibliothèques** réutilisables
- [ ] **Tests unitaires**
- [ ] **Documentation API**

---

### Phase 4 : Intégration & Tests
**🎯 Objectif** : Validation complète

#### Tests fonctionnels
- [ ] **Validation paramètres**
  - [ ] Précision CV (1V/oct)
  - [ ] Linéarité autres paramètres
  - [ ] Temps de réponse
  - [ ] Stabilité long terme

- [ ] **Tests système**
  - [ ] Isolation galvanique
  - [ ] Immunité parasites
  - [ ] Stress test MIDI
  - [ ] Comportement limites

#### Compatibilité
- [ ] **Versions Kobol**
  - [ ] Test sur différents Expander
  - [ ] Vérification connecteurs
  - [ ] Adaptations nécessaires

- [ ] **Environnement MIDI**
  - [ ] DAW populaires
  - [ ] Contrôleurs MIDI
  - [ ] Chaînage MIDI

#### Livrables Phase 4
- [ ] **Prototype fonctionnel**
- [ ] **Rapport tests** complet
- [ ] **Manuel utilisateur**
- [ ] **Guide dépannage**

---

### Phase 5 : Commercialisation
**🎯 Objectif** : Produit fini

#### Finalisation produit
- [ ] **Boîtier externe**
  - [ ] Design mécanique
  - [ ] Interface utilisateur (LED, boutons)
  - [ ] Connectique professionnelle
  - [ ] Fabrication PCB final

- [ ] **Certification**
  - [ ] Tests EMC si nécessaire
  - [ ] Documentation technique
  - [ ] Manuel installation

#### Stratégie commerciale
- [ ] **Étude marché**
  - [ ] Enquête communauté Kobol
  - [ ] Pricing compétitif
  - [ ] Canaux distribution

- [ ] **Support**
  - [ ] Site web produit
  - [ ] Documentation complète
  - [ ] Support technique

#### Livrables Phase 5
- [ ] **Produit commercialisable**
- [ ] **Documentation utilisateur**
- [ ] **Stratégie commerciale**
- [ ] **Support client**

---

## 🚀 Prochaines Actions Immédiates

1. **Acquisition oscilloscope** (priorité haute)
2. **Documentation connecteur** avec mesures multimètre
3. **Étude datasheet TLC5940** approfondie
4. **Conception schéma bloc** système complet

## 📝 Notes de Développement

### Questions techniques ouvertes
- Résolution ADC suffisante pour matching presets ?
- Fréquence PWM TLC5940 optimale ?
- Stratégie anti-jump : pickup vs morphing ?

### Idées d'améliorations
- Interface graphique PC/Mac
- Support autres synthés RSF
- Modules d'expansion
- Séquenceur intégré

---
*Dernière mise à jour : Septembre 2025*

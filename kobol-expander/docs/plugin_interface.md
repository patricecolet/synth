> **Document de septembre 2025, en partie dépassé.** L'interface plugin existe
> désormais, mais pas sous la forme prévue ici : au lieu d'un plugin dédié, un
> **profil d'appareil** a été ajouté au séquenceur NiDMI Seq, qui savait déjà
> émettre des CC et faire des P-locks. Voir
> [`../midi-cv/INTEGRATION.md`](../midi-cv/INTEGRATION.md).
>
> Reste valable : les maquettes d'interface, la réflexion sur JUCE, et l'idée
> de retour bidirectionnel (non implémentée).

# Interface Plugin - KobolExpanderControl

## 🎯 **Concept Plugin**

### **Vision :**
Interface graphique moderne sous forme de plugin VST/AU pour contrôler le Kobol depuis la DAW.

### **Avantages Plugin :**
- ✅ **Interface riche** : Graphiques, animations, visualisations
- ✅ **Intégration DAW** : Automation, presets, recall
- ✅ **Évolutivité** : Updates interface sans hardware
- ✅ **Multi-plateforme** : Windows, Mac, Linux
- ✅ **Presets cloud** : Partage communauté

## 🖥️ **Architecture Plugin**

### **Communication :**
```
DAW ←→ Plugin VST ←→ USB ←→ ESP32-S3 ←→ Kobol
  ↓        ↓         ↓        ↓         ↓
Audio   Interface  MIDI   Hardware   Synthé
```

### **Protocole :**
```
Plugin → MIDI CC → ESP32 → PWM → Kobol
Plugin ← MIDI Feedback ← ESP32 ← ADC ← Kobol (V3+)
```

## 🎛️ **Interface Graphique**

### **V2 Simple - Interface Basique :**
```
┌─────────────────────────────────────┐
│ KobolExpanderControl V2             │
├─────────────────────────────────────┤
│ VCO2 ┌─────┐ VCF ┌─────┐ VCA ┌─────┐│
│ Freq │ ● ● │ Cut │ ● ● │ Att │ ● ● ││
│      └─────┘     └─────┘     └─────┘│
│                                     │
│ LFO1 ┌─────┐ ENV1 ┌────┐ Presets   │
│ Rate │ ● ● │ Att  │ ●● │ [1][2][3] │
│      └─────┘      └────┘           │
└─────────────────────────────────────┘
```

### **V3-V5 - Interface Avancée :**
```
┌─────────────────────────────────────────────────┐
│ PolyKobol - 8 Voice Polyphonic                  │
├─────────────────────────────────────────────────┤
│ Voice 1 │ Voice 2 │ Voice 3 │ Voice 4 │ Master │
├─────────┼─────────┼─────────┼─────────┼─────────┤
│ [VCO]   │ [VCO]   │ [VCO]   │ [VCO]   │ [LFO1] │
│ [VCF]   │ [VCF]   │ [VCF]   │ [VCF]   │ [LFO2] │
│ [VCA]   │ [VCA]   │ [VCA]   │ [VCA]   │ [ENV1] │
│ [ENV]   │ [ENV]   │ [ENV]   │ [ENV]   │ [ENV2] │
├─────────┴─────────┴─────────┴─────────┼─────────┤
│ Modulation Matrix                     │ Effects │
│ ┌─────────────────────────────────────┐│ [Reverb]│
│ │ LFO1→VCF1 ●  LFO2→VCA ●            ││ [Delay] │
│ │ ENV1→VCF2 ●  WHEEL→RES ●           ││ [Chorus]│
│ └─────────────────────────────────────┘│         │
└─────────────────────────────────────────────────┘
```

## 💻 **Technologies Plugin**

### **Framework Recommandé :**

#### **JUCE (C++) :**
- ✅ **Multi-format** : VST2/3, AU, AAX
- ✅ **Multi-plateforme** : Win/Mac/Linux
- ✅ **Performance** : Temps réel optimisé
- ✅ **MIDI** : Gestion complète
- ✅ **Graphiques** : Interface moderne

#### **Alternative Web (Electron) :**
- ✅ **Développement** : HTML/CSS/JS
- ✅ **Interface** : Moderne et flexible
- ✅ **Déploiement** : Rapide
- ❌ **Performance** : Moins optimisé

### **Communication MIDI :**
```cpp
// Plugin → Hardware
void sendParameter(int param_id, float value) {
  int cc_number = param_mapping[param_id];
  int midi_value = value * 127;
  sendMIDI_CC(cc_number, midi_value);
}

// Hardware → Plugin (V3+)
void onMIDI_Feedback(int cc, int value) {
  int param_id = reverse_mapping[cc];
  updateGUI_Parameter(param_id, value / 127.0);
}
```

## 🎨 **Fonctionnalités Plugin**

### **Visualisations :**
- **Oscilloscope** : Formes d'ondes temps réel
- **Spectrum** : Analyse fréquentielle
- **Enveloppes** : Courbes ADSR animées
- **Matrix** : Routage visuel

### **Presets Avancés :**
- **Banques** : Organisation hiérarchique
- **Tags** : Recherche par style/genre
- **Morphing** : Transition visuelle
- **Randomize** : Génération automatique

### **Automation DAW :**
```cpp
// Tous paramètres automatisables
registerParameter("VCF_Cutoff", 0.0f, 1.0f);
registerParameter("VCO2_Frequency", -1.0f, 1.0f);
registerParameter("LFO1_Rate", 0.01f, 100.0f);
```

## 🔧 **Développement par Version**

### **V2 Simple :**
```
Plugin basique :
- 14 contrôles (sliders/knobs)
- Presets simples (load/save)
- Interface épurée
- Développement : 2-3 mois
```

### **V3-V5 Avancées :**
```
Plugin sophistiqué :
- Interface multi-pages
- Matrice modulation visuelle
- Visualisations temps réel
- Développement : 6-12 mois
```

## 💡 **Avantages Plugin**

### **Utilisateur :**
- **Interface familière** : Dans la DAW
- **Automation** : Enregistrement mouvements
- **Presets** : Sauvegarde avec projet
- **Visualisation** : Feedback visuel

### **Développeur :**
- **Updates** : Interface sans hardware
- **Distribution** : Téléchargement simple
- **Support** : Logs et diagnostics
- **Monétisation** : Modèle SaaS possible

## 🚀 **Roadmap Plugin**

### **V2 Simple :**
1. **MIDI mapping** : CC → Paramètres
2. **Interface basique** : 14 contrôles
3. **Presets** : Load/save simple

### **V3+ Avancées :**
1. **Bidirectionnel** : Feedback hardware
2. **Matrice** : Modulation visuelle  
3. **Visualisations** : Temps réel
4. **AI** : Génération presets

**Plugin = Interface du futur !** 🖥️

**Évolutivité maximale** sans changer hardware ! 🎯✨

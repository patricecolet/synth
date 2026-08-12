# KobolExpanderControl - Guide des Versions

> **Ce guide décrit un plan de 2025.** Le travail actif est aujourd'hui dans
> [`midi-cv/`](midi-cv/) : firmware Teensy 2.0 qui tourne, carte MIDI partagée
> avec le séquenceur NiDMI Seq, calibration mesurée. Les cinq versions
> ci-dessous restent la feuille de route à long terme, mais seules **v1** (qui
> fonctionne) et **v2** (schémas KiCad) contiennent quelque chose.
>
> | Dossier | Contenu réel |
> |---|---|
> | [`midi-cv/`](midi-cv/) | **firmware actif**, carte MIDI, outils, intégration VST |
> | [`v1-first-release/`](v1-first-release/) | le firmware d'origine, référence intouchée |
> | [`v2-simple/`](v2-simple/) | projet KiCad `kobol_V2`, pas de code |
> | `v3` / `v4` / `v5` | documentation de vision uniquement |

## 📁 **Structure Projet**

### **5 Versions Distinctes :**

```
📁 KobolExpanderControl_firstRelease/     # V1 - Version Historique
📁 KobolExpanderControl_V2_Simple/        # V2 - Version Production  
📁 KobolExpanderControl_V3_Advanced/      # V3 - Version Professionnelle
📁 KobolExpanderControl_V4_Fork/          # V4 - Kobol V2 (Fork)
📁 PolyKobolFork_8Voice/                  # V5 - PolyKobol 8 Voix 🚀
📁 KobolExpanderControl/                  # Documentation Générale
```

## 🎯 **Comparaison Versions**

| Aspect | V1 First | V2 Simple | V3 Advanced | V4 Fork | V5 Poly |
|--------|----------|-----------|-------------|---------|---------|
| **Invasivité** | Non | Non | Minimale | Complète | Révolutionnaire |
| **Complexité** | Basique | Simple | Complexe | Révolutionnaire | Extrême |
| **Coût** | 15€ | 54€ | 180€ | 500€+ | 3000€+ |
| **Temps Dev** | Fait | 2 mois | 12 mois | 24 mois | 36 mois |
| **Marché** | Prototype | Grand public | Pro | Collectionneurs | Boutique |
| **Voix** | Mono | Mono | Mono | Mono | **8 Poly** |

## 🚀 **V1 - firstRelease (Historique)**
- **Status** : ✅ Fonctionnel
- **Usage** : Base de développement
- **Modification** : ❌ Interdite (référence)

## 🎛️ **V2 - Simple (Production)**
- **Status** : 🔄 En développement
- **Objectif** : Interface P1 non-invasive
- **Paramètres** : 14/16 (éviter Pin 2)
- **Circuit** : ±1.5V uniforme + calibration logicielle

## 🎵 **V3 - Advanced (Professionnelle)**
- **Status** : 📋 Planifiée
- **Objectif** : ADC bidirectionnel + presets automatiques
- **Complexité** : Modifications Kobol minimales
- **Innovation** : Modulaire virtuel

## 🇫🇷 **V4 - Fork (Révolutionnaire)**
- **Status** : 🔬 Recherche
- **Objectif** : Nouveau Kobol avec DAC expo
- **Innovation** : Tempéraments programmables
- **Impact** : Révolution synthèse analogique

## 🎵 **V5 - PolyKobol 8 Voix (Ultime)**
- **Status** : 💭 Vision
- **Objectif** : Kobol polyphonique 8 voix
- **Innovation** : Synthé analogique polyphonique moderne
- **Impact** : Synthé boutique révolutionnaire

---

## 🎯 **Stratégie Développement**

### **Progression Logique :**
```
V1 (Base) → V2 (Production) → V3 (Pro) → V4 (Innovation)
    ↓           ↓               ↓           ↓
  Fait      2 mois          12 mois     24 mois
```

### **Validation Étapes :**
- **V2** valide le concept non-invasif
- **V3** explore les limites techniques  
- **V4** révolutionne la synthèse

### **Marché Segmenté :**
- **V2** : Communauté Kobol large
- **V3** : Studios professionnels
- **V4** : Collectionneurs et innovateurs

---

## 📋 **Documentation par Version**

### **Chaque Dossier Contient :**
- `README.md` - Vision et objectifs
- `docs/` - Documentation technique
- `src/` - Code source (si applicable)
- `hardware/` - Schémas et PCB
- `tests/` - Validation et mesures

### **Documentation Commune :**
- `KobolExpanderControl/docs/` - Analyses générales
- `README_VERSIONS.md` - Ce guide

---

## 🎵 **Vision Globale**

**Du contrôleur simple au synthé révolutionnaire :**
- **Préservation** : Caractère Kobol authentique
- **Innovation** : Technologies modernes
- **Progression** : Validation étape par étape
- **Impact** : Renaissance synthèse française 🇫🇷

**Chaque version a sa place dans l'écosystème !** 🚀✨

---
*Guide de navigation - Versions KobolExpanderControl*

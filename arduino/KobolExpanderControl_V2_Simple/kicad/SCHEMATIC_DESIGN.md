# Schéma électronique – Kobol Expander Control V2 Simple

Ce document décrit le circuit à reproduire dans KiCad. Utilisez-le comme référence pour placer les symboles et les connexions.

---

## 0. Architecture : 3 cartes + connecteurs

Le projet est découpé en **3 cartes** reliées par **connecteurs**. Les **cartes 1 et 2** sont nécessaires au fonctionnement de base ; la **carte 3 est optionnelle** (presets / lecture des potards).

| Carte | Rôle | Contenu principal | Connecteurs |
|-------|------|-------------------|-------------|
| **Carte 1 – MCU + Alim + DAC** | Cœur : alimentation, processeur, pitch/gate | USB, ICL7660, TLE2426, **ESP32-S3 DevKitC-1**, **MCP4921** (DAC 1 canal), **TL082** (étage sortie DAC), sortie Gate, jacks CV Out / Gate Out | Vers Carte 2 : +5V, -5V, GND, 0V_REF, PWM1…14, GATE, GND. Vers Carte 3 (option) : alimentation, GPIO (switches, mux), ADC_IN, GND. |
| **Carte 2 – CV** | 14 voies PWM→CV et sortie Kobol | 4× LM324, 14× filtre RC, R de protection, **connecteur P1** (vers Kobol) | Vers Carte 1 : alim + signaux (PWM, GATE). Vers Carte 3 (option) : les 14 lignes CV/P1 à lire (pour le multiplexeur). |
| **Carte 3 – Option : switches + multiplexeur** | Interface presets + lecture des potards | **Switches** (boutons / sélection presets), **multiplexeur analogique** (lecture des tensions P1 pour « learn preset »), évent. level shifter vers ADC | Vers Carte 1 : +3V3/5V, GND, signaux switches (GPIO), adresse mux (GPIO), **ADC_IN** (sortie mux → ADC ESP32). Vers Carte 2 : les 14 lignes CV (pour brancher en entrée du mux). |

**Carte 3 = option** : on peut monter uniquement les cartes 1 et 2 pour avoir l’expander MIDI→CV ; ajouter la carte 3 permet la gestion des presets et la lecture des potards analogiques du Kobol (learn preset).

**Connecteurs intercartes**  
- Pas 2,54 mm (strip ou header). Repère pin 1, détrompage.  
- Nommer les nets sur le schéma (ex. `J_PWR`, `J_SIGNAL`, `J_OPTION` pour la carte 3).

**ESP32-S3** : **DevKitC-1** sur la carte 1. Voir § 7 et **KICAD_LIBRARIES.md**.

---

## 1. Vue d’ensemble des blocs (3 cartes)

```
[ Carte 1 – MCU + Alim + DAC ]
USB 5V ──► ICL7660 ──► +5V, -5V    ESP32-S3 DevKitC-1 ──► PWM1..14 ──┐
         TLE2426 ──► 0V_REF        SPI ──► MCP4921 ──► TL082 ──► Pitch CV │
              │                    Gate ──► Jack Gate                 │
              └── connecteur vers Carte 2 (+5V,-5V,0V_REF, PWM, Gate)┘
              └── connecteur option vers Carte 3 (GPIO, ADC_IN)

[ Carte 2 – CV ]
PWM1..14, +5V, -5V, 0V_REF ◄── connecteur Carte 1
     │
     ▼
Filtre RC ──► LM324 x14 ──► CV1..14 ──► Connecteur P1 (Kobol)
                              │
                              └── (option) connecteur vers Carte 3 (14 lignes pour mux)

[ Carte 3 – OPTION : Switches + Multiplexeur ]
Switches (GPIO) ──► vers Carte 1
14 lignes CV (depuis Carte 2) ──► Multiplexeur ──► Level shifter ──► ADC_IN ──► Carte 1 (ADC ESP32)
Adresse mux (GPIO Carte 1)
```

---

## 2. Alimentation (feuille ou bloc Power)

### 2.1 Entrée USB

| Référence | Composant      | Valeur / type     | Connexions                    |
|-----------|----------------|-------------------|------------------------------|
| J_USB     | Connecteur USB | USB-B ou micro   | Pin 1 → +5V, Pin 2 → GND    |
| C_USB_IN  | Condensateur   | 22 µF électrolytique | Entre +5V et GND         |
| C_USB_100n| Condensateur   | 100 nF            | Entre +5V et GND, proche J_USB |

**Nets** : `+5V`, `GND`

### 2.2 ICL7660 (–5 V)

| Référence | Composant | Brochage (DIP-8) | Connexions |
|-----------|-----------|-------------------|------------|
| U_7660    | ICL7660   | 8 → +5V, 4 → GND, 5 → -5V | Suivre datasheet pour C1, C2 (transfert), C_out |
| C_7660_C1 | 10 µF     | Cap transfert (broches 2–3 selon DS) | |
| C_7660_C2 | 10 µF     | Cap transfert       | |
| C_7660_OUT| 22 µF + 100 nF | Sortie -5V        | Entre -5V et GND |

**Nets** : `+5V`, `GND`, `-5V`

### 2.3 TLE2426 (référence 0 V)

| Référence  | Composant | Connexions |
|------------|-----------|------------|
| U_TLE2426  | TLE2426   | In+ → +5V, In- → -5V, Out → 0V_REF |
| C_TLE_100n | 100 nF    | Entre 0V_REF et GND (optionnel) |

**Nets** : `+5V`, `-5V`, `GND`, `0V_REF`

### 2.4 Découplage rails

- **+5V** : 100 nF au plus près de chaque circuit intégré (LM324 sur Carte 2 ; MCP4921, TL082 sur Carte 1).
- **-5V** : 100 nF au plus près de chaque LM324.

---

## 3. Voie PWM → CV (répéter 14 fois)

Une voie = **filtre RC** + **1/4 LM324** + **résistance de protection**.

### 3.1 Filtre (par voie)

| Référence   | Composant | Valeur | Connexions |
|-------------|-----------|--------|------------|
| R_PWM_n     | Résistance| 1 kΩ   | PWM_n (ESP32) → noeud commun |
| C_PWM_n     | Condensateur | 330 nF | Noeud commun → GND |

Noeud commun = entrée (+) de l’AOP de la voie n.

### 3.2 Étage AOP (LM324)

- **4× LM324** : chaque LM324 contient 4 AOP → 16 AOP au total, 14 utilisés (2 en spare).
- Alimentation : V+ → +5V, V- → -5V (GND commun avec 0V_REF).
- Montage par AOP : **ampli non-inverseur avec offset** pour sortie dans la plage ±1,5 V (ou selon plage par pin). Référence d’offset = 0V_REF (ou diviseur depuis 0V_REF / -5V / +5V selon besoin).
- **Découplage** : 100 nF entre V+ et GND, 100 nF entre V- et GND, au plus près de chaque boîtier LM324.

### 3.3 Résistance de protection (sortie)

| Référence | Composant | Valeur  | Connexions   |
|-----------|-----------|---------|--------------|
| R_OUT_n   | Résistance| 100–330 Ω | Sortie AOP → CV_n (vers P1) |

### 3.4 Mapping des 14 voies

| Voie n | Net PWM  | Net CV   | Pin P1 | Paramètre (rappel README) |
|--------|----------|----------|--------|---------------------------|
| 1      | PWM1     | CV1      | Pin 1  | VCA Decay                 |
| 2      | PWM2     | CV2      | Pin 3  | VCO1 Waveform             |
| 3      | PWM3     | CV3      | Pin 4  | VCF Sustain               |
| 4      | PWM4     | CV4      | Pin 5  | VCA Attack                 |
| 5      | PWM5     | CV5      | Pin 6  | VCO2 Waveform             |
| 6      | PWM6     | CV6      | Pin 7  | VCF Decay                  |
| 7      | PWM7     | CV7      | Pin 8  | VCF Attack                 |
| 8      | PWM8     | CV8      | Pin 9  | VCO2 Volume                |
| 9      | PWM9     | CV9      | Pin 10 | VCF ADS CTRL               |
| 10     | PWM10    | CV10     | Pin 11 | VCO2 Frequency            |
| 11     | PWM11    | CV11     | Pin 12 | VCF Resonance              |
| 12     | PWM12    | CV12     | Pin 14 | VCO1 Volume                |
| 13     | PWM13    | CV13     | Pin 15 | VCF Cutoff                 |
| 14     | PWM14    | CV14     | Pin 16 | VCA Sustain                |

**Pin 2** : NE PAS CONNECTER (V- -14,5 V du Kobol).  
**Pin 13** : VCO1 Frequency – laissé manuel par choix (non connecté à une sortie CV dans ce design).

---

## 4. Connecteur P1

- **Type** : connecteur 2×8 (16 broches), pas 2,54 mm.
- **Mapping** : Pin 1 → CV1, Pin 2 → **NC** (étiquette « -14.5V DO NOT DRIVE »), Pin 3 → CV2, … Pin 16 → CV14.
- **Pin 13** : non connecté à une sortie (ou étiquette « VCO1 Freq – manual »).

---

## 5. MCP4921 (DAC – pitch 1 V/oct)

DAC **monocanal** 12 bits, SPI. Sortie unique (Vout) pour la CV pitch externe.

| Référence   | Composant | Connexions |
|-------------|-----------|------------|
| U_MCP4921   | MCP4921   | VDD → +5V (ou +3V3 selon DS), GND, **VREF** → voir ci‑dessous |
| SPI         | CS, SCK, MOSI | Vers ESP32-S3 (broches détaillées en **§ 7**, sous-section SPI) |
| C_MCP_100n  | 100 nF    | VDD–GND au plus près |
| C_MCP_10u   | 10 µF (optionnel) | VDD–GND |

**VREF** : ne pas laisser flottant. Soit **VREF → VDD** (+5 V) : sortie DAC 0–5 V, adapter l’étage AOP (un seul soustracteur). Soit **VREF → référence 2,048 V** (ex. MCP1541, REF3020) : sortie 0–2,048 V, garder le circuit 2× AOP actuel.

La sortie **Vout** du MCP4921 (0–2,048 V si VREF = 2,048 V, ou 0–5 V si VREF = VDD) alimente l’étage **TL082** (2 AOP) ci‑dessous ; le net final `PITCH_CV` part vers une résistance de protection puis le jack **CV Out**.

---

### 5.2 Circuit complet : DAC → 2 AOP → CV bipolaire (–5 V à +5 V)

**Objectif** : sortie jack **–5 V à +5 V** (1 V/oct, 0 V = note de référence) pour accorder le synthé. Alimentation des AOP : **+5 V / –5 V** (rails carte 1).

**Étape 1 – AOP1 (pré‑gain)**  
Transforme 0–2,048 V (OUTA) en environ 0–5,1 V.

- Montage : **ampli non-inverseur**.
- Entrée + : Vout du MCP4921 (évent. filtre 100 nF vers GND sur ce nœud).
- R1_DAC entre **sortie AOP1** et **entrée –** ; R2_DAC entre **entrée –** et **GND**.
- Gain \(G_1 = 1 + R1\_DAC / R2\_DAC\). Avec R2_DAC = 10 kΩ, R1_DAC = 15 kΩ → \(G_1 = 2{,}5\) → V1 ≈ 0–5,12 V.

**Étape 2 – AOP2 (offset autour de 0 V)**  
Transforme V1 (≈ 0–5,12 V) en **–5 V à +5 V**.

- Référence **Vref** : tension milieu de la plage V1, par ex. **2,56 V**, obtenue par diviseur résistif depuis +5 V (ou 0V_REF si adapté). Ex. R_ref_A = 10 kΩ (vers +5 V), R_ref_B = 10 kΩ (vers GND) → 2,5 V ; ou trimmer pour ajuster le 0 V.
- Montage : **ampli différentiel** (soustracteur) : \(V_\text{out} = G_2 \cdot (V_1 - V_\text{ref})\), avec \(G_2 = 2\).
- Soustracteur à 4 résistances (AOP2) : **Entrée +** : V1 via R_a (10 kΩ) vers le nœud +, et ce nœud vers **GND** via R_b (20 kΩ) → donc V+ = V1 × R_b/(R_a+R_b) = (2/3)·V1. **Entrée –** : Vref via R_c (10 kΩ) vers le nœud –, et ce nœud vers **sortie AOP2** via R_d (20 kΩ) → V– = (Vref·R_d + Vout·R_c)/(R_c+R_d). L’AOP impose V+ = V–, d’où Vout = 2·(V1 − Vref). **Important** : Vref ne va pas sur l’entrée + ; V1 est sur + (avec partiteur vers GND), Vref est sur – (avec contre‑réaction depuis Vout).

**Sortie** : sortie AOP2 → **R_PROT** 100 Ω → net **PITCH_CV** → jack **CV Out**.

#### Schéma ASCII (logique)

```
 MCP4921 Vout ───●──────────────► Entrée + AOP1 (TL082-1)
               │
             [100 nF]
               │
              GND

AOP1 (TL082, 1/2, alimenté en +5 / -5)
- Entrée + : reliée au ● (OUTA)
- Entrée - : reliée
    ↑ à la sortie AOP1 via R1_DAC = 15 kΩ
    ↓ à GND via R2_DAC = 10 kΩ
- Sortie : V1 ≈ 0..5,1 V

  Soustracteur AOP2 (TL082, 2/2 ; Vout = 2·(V1 − Vref)) :

  Entrée + :  V1 ──[R_a 10k]───●──[R_b 20k]── GND    →  V+ = (2/3)·V1
                               │
                               └── entrée + AOP2

  Entrée − :  Vref ──[R_c 10k]───●──[R_d 20k]── Vout  →  V− = (2·Vref + Vout)/3
                               │
                               └── entrée − AOP2
  (AOP impose V+ = V−  ⇒  Vout = 2·V1 − 2·Vref)

Sortie AOP2 ──[R_PROT 100Ω]──► PITCH_CV ─► jack CV Out
```

#### Valeurs à utiliser (carte 1)

| Composant   | Valeur  | Rôle |
|-------------|---------|------|
| R1_DAC      | 15 kΩ   | Contre-réaction AOP1 (gain 2,5) |
| R2_DAC      | 10 kΩ   | Entrée – AOP1 vers GND |
| R_ref_A     | 10 kΩ   | Diviseur Vref (vers +5 V) |
| R_ref_B     | 10 kΩ   | Diviseur Vref (vers GND) ; ou trimmer pour régler 0 V |
| R_a         | 10 kΩ   | V1 → entrée + AOP2 |
| R_b         | 20 kΩ   | Entrée + AOP2 → GND (partiteur avec R_a : V+ = (2/3)·V1) |
| R_c         | 10 kΩ   | Vref → entrée – AOP2 |
| R_d         | 20 kΩ   | Sortie AOP2 → entrée – AOP2 (contre-réaction) |
| R_PROT      | 100 Ω   | Protection sortie → jack |
| U_TL082     | TL082 (dual) | AOP1 + AOP2 ; alimentation +5 V / –5 V |

Ajustement fin du **0 V** : envoyer la valeur DAC milieu (ex. 2048 pour 12 bits) et régler le trimmer (ou R_ref_B) pour mesurer 0 V au multimètre sur le jack.

---

## 6. Sortie Gate

- **GPIO** ESP32 (net `GATE_OUT`) → **résistance série** 330 Ω → jack **Gate Out**.
- Option : buffer 3,3 V → 5 V si le synthé attend 5 V en gate.

---

## 7. ESP32-S3 DevKitC-1 (carte 1)

- **Module utilisé** : **ESP32-S3-DevKitC-1** (carte de développement avec USB, régulateur 3,3 V intégré).
- **Emplacement** : **Carte 1** (MCU + Alim + DAC).
- **Alimentation** : 5 V USB ou 5 V sur la carte → régulateur intégré sur la DevKit → 3,3 V pour le SoC.
- **GPIO** : PWM1…PWM14 (vers Carte 2), Gate, SPI (MCP4921), USB (MIDI). Si Carte 3 présente : adresse mux (ex. MUX_S0…S2), lecture ADC (ADC_IN), signaux switches (SW_*).
- **Symboles / emplacements KiCad** : librairie **Espressif** (PCM). Voir **KICAD_LIBRARIES.md**.
- Découplage 3,3 V : déjà sur la DevKit ; 100 nF au connecteur d’alim sur la carte si besoin.

### Broches SPI (MCP4921) – ESP32-S3 DevKitC-1

Sur l’ESP32-S3 il n’y a pas de VSPI ; on utilise **FSPI** ou **HSPI**. Pour le MCP4921 il faut MOSI, SCK et CS (pas de MISO).

**FSPI (SPI2) – recommandé :**

| Signal | GPIO | Net schéma |
|--------|------|------------|
| MOSI   | 11   | SPI_MOSI   |
| SCK    | 12   | SPI_SCK    |
| CS     | 9    | SPI_CS     |

**HSPI (SPI3) – alternative :**

| Signal | GPIO | Net schéma |
|--------|------|------------|
| MOSI   | 15   | SPI_MOSI   |
| SCK    | 17   | SPI_SCK    |
| CS     | Libre (ex. 10, 14) | SPI_CS |

**À éviter** sur DevKitC-1 N8R8 : **GPIO 35, 36, 37** (réservées PSRAM octal). Vérifier le pinout pour les broches déjà utilisées (LED, BOOT, etc.).

---

## 7bis. Carte 3 (option) : switches + multiplexeur

**Carte 3 est optionnelle** : elle ajoute la gestion des presets et la lecture des potards du Kobol (learn preset).

### Switches (boutons)

- **Rôle** : sélection de presets, validation « learn », etc.
- **Câblage** : un côté vers GND (ou +3V3), l’autre vers une résistance pull-up/down et vers un **GPIO** de l’ESP32 (Carte 1). Nets typiques : `SW_1`, `SW_2`, … (ou `SW_PRESET_1`, `SW_LEARN`, etc.).
- **Connecteur Carte 3 → Carte 1** : ces signaux GPIO + GND, alimentation +3V3 pour la carte 3.

### Multiplexeur analogique

- **Rôle** : lire la tension sur une des 14 lignes CV/P1 (quand la Carte 2 met ses sorties en haute Z, ces lignes portent la tension des potards du Kobol).
- **Composant** : multiplexeur analogique bipolaire (ex. **CD4051** x2 ou **CD4052**, alimentation ±5 V ou 0/5 V selon type). Entrées : les **14 lignes** CV1…CV14 (provenant de la Carte 2 via un connecteur). Sortie commune : une seule ligne analogique.
- **Sélection** : 3 ou 4 bits d’adresse (GPIO Carte 1) → nets `MUX_S0`, `MUX_S1`, `MUX_S2` (et évent. `MUX_S3`).
- **Sortie mux** : tension bipolaire (ex. -1,5 V à +1,5 V) → **level shifter** (un AOP, ex. LM324) pour ramener en 0–3,3 V → net **`ADC_IN`** vers l’**ADC** de l’ESP32 (Carte 1).
- **Connecteurs** :  
  - Carte 3 ↔ Carte 1 : +3V3, GND, MUX_S0…S2, ADC_IN, SW_* (switches).  
  - Carte 3 ↔ Carte 2 : les 14 lignes CV1…CV14 (à relier aux entrées du mux).

### Résumé Carte 3

| Élément      | Composants / nets                    | Vers Carte 1      | Vers Carte 2   |
|-------------|---------------------------------------|-------------------|----------------|
| Switches    | Boutons + R pull-up/down              | GPIO (SW_*), GND   | —              |
| Multiplexeur| CD4051/4052, alimentation             | MUX_S0…S2, ADC_IN | CV1…CV14       |
| Level shifter | 1 AOP (bipolaire → 0–3,3 V)        | ADC_IN            | —              |

---

## 8. Noms de nets à utiliser

- Alimentation : `+5V`, `-5V`, `GND`, `0V_REF`, `+3V3`
- PWM : `PWM1` … `PWM14`
- CV vers P1 : `CV1` … `CV14`
- DAC / Gate : `PITCH_CV`, `GATE_OUT`
- SPI : `SPI_CS`, `SPI_SCK`, `SPI_MOSI`
- **Carte 3 (option)** : `MUX_S0`, `MUX_S1`, `MUX_S2`, `ADC_IN`, `SW_1`, `SW_2`, …

---

## 9. BOM simplifié (à affiner selon boîtiers)

| Catégorie      | Référence / type        | Quantité |
|----------------|-------------------------|----------|
| MCU            | ESP32-S3 DevKitC-1      | 1        |
| Connecteurs    | Intercartes (2,54 mm)   | Selon 3 cartes |
| AOP            | LM324 (DIP-14 ou SOIC)  | 4        |
| Référence      | TLE2426                 | 1        |
| Convertisseur  | ICL7660                 | 1        |
| DAC            | MCP4921                 | 1        |
| AOP pitch (Carte 1) | TL082 (dual)        | 1        |
| R 1 kΩ         | Filtre PWM              | 14       |
| C 330 nF       | Filtre PWM              | 14       |
| R protection   | 100–330 Ω               | 14 + 1 (DAC) + 1 (Gate) |
| Condensateurs  | 100 nF, 10–22 µF        | Selon schéma |
| Connecteurs    | USB, P1 2×8, Jacks 3,5 mm, **intercartes** (2,54 mm) | 1, 1, 2, selon schéma 3 cartes |
| **Carte 3 (option)** | Switches (tactiles), CD4051/4052, 1× AOP (level shifter) | Selon interface choisie |

---

## 10. Ordre de placement suggéré dans KiCad (par carte)

- **Carte 1 – MCU + Alim + DAC** : Alimentation (USB, ICL7660, TLE2426, découplages), **ESP32-S3 DevKitC-1**, **MCP4921** + **TL082** (étage pitch bipolaire), Gate, jacks CV Out / Gate Out, **connecteur(s) vers Carte 2** (+5V, -5V, GND, 0V_REF, PWM1…14, GATE, GND). Si option Carte 3 : **connecteur(s) vers Carte 3** (+3V3, GND, MUX_S0…S2, ADC_IN, SW_*).
- **Carte 2 – CV** : Connecteur(s) depuis Carte 1 (alim + PWM, GATE), 14 voies PWM→CV (filtre RC + LM324 + R_out), connecteur **P1** (Pin 2 = NC). Si option Carte 3 : **connecteur vers Carte 3** (les 14 lignes CV1…CV14 pour le mux).
- **Carte 3 – Option (switches + multiplexeur)** : Connecteur(s) depuis Carte 1 (alim, GPIO mux, ADC_IN, switches) et depuis Carte 2 (CV1…CV14), switches, multiplexeur (CD4051/4052), level shifter → ADC_IN.

Ensuite : annotations, ERC, **Assign PCB footprints**, **Update PCB** pour chaque carte. Le projet peut être dessiné avec 3 feuilles (sheets) = 3 PCB, la Carte 3 pouvant être omise pour la version de base.

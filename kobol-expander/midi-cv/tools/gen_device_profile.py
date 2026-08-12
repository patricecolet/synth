#!/usr/bin/env python3
"""Genere le profil Kobol du plugin NiDMI Seq depuis midi-map.json.

La carte MIDI a plusieurs consommateurs : le firmware, ce depot, et le plugin
qui vit dans un autre depot. Recopier a la main a deja produit cinq
divergences en deux jours — dont deux `wired` faux. Elle est donc generee.

Deux sorties, tenues d'accord :

  1. <plugin>/Profiles/kobol-expander.json — le fichier que l'utilisateur
     copie pour decrire un autre synthe.
  2. la chaine kKobolSeedJson de DeviceProfile.cpp, entre marqueurs, ecrite
     dans ~/Documents/NiDMI/Profiles/ au premier lancement pour que le dossier
     ne soit pas vide.

Le reste de DeviceProfile.cpp est laisse intact.

    python3 tools/gen_device_profile.py            verifie et signale
    python3 tools/gen_device_profile.py --write    applique

Chemin du plugin : $NIDMI_SEQ_VST, sinon ~/repo/nidmi-seq-vst.
Si le depot est absent, le script le dit et sort en 0 : ce depot doit rester
utilisable sans lui.
"""

import json
import os
import sys
from pathlib import Path

BEGIN = "// >>> GENERE DEPUIS midi-map.json — NE PAS EDITER A LA MAIN"
END = "// <<< FIN DU BLOC GENERE"

ROOT = Path(__file__).resolve().parent.parent
JSON_PATH = ROOT / "midi-map.json"

VST_ROOT = Path(os.environ.get("NIDMI_SEQ_VST", Path.home() / "repo" / "nidmi-seq-vst"))
TARGET = VST_ROOT / "Source" / "DeviceProfile.cpp"
PROFILE = VST_ROOT / "Profiles" / "kobol-expander.json"


def entries(data):
    """Toutes les entrees du profil, dans l'ordre d'affichage."""
    return data["parameters"] + data["modulation"]


def build_json(data):
    """Le profil tel que le plugin le lira. Format v2 : panneau + positions +
    learn, en plus du nommage des CC."""
    import json as _json

    out = {
        "schema": 2,
        "name": "Kobol Expander",
        "manufacturer": "RSF",
        "panel": data["panel"],
        "pitch": data["pitch"],
        "parameters": [],
    }
    for e in entries(data):
        out["parameters"].append({
            "cc": int(e["cc"]),
            "short": e["short"],
            "name": e["name"],
            "group": e["group"],
            "section": e["section"],
            "type": e["type"],
            "pos": e["pos"],
            "size": e["size"],
            "wired": bool(e["wired"]),
            "learn": e["learn"],
        })
    return _json.dumps(out, ensure_ascii=False, indent=2)


def build_block(data):
    """Le meme JSON, encapsule en litteral C++ brut."""
    return (
        BEGIN + "\n"
        "static const char* kKobolSeedJson = R\"JSON(" + build_json(data) + ")JSON\";\n"
        + END
    )


def main():
    if not TARGET.exists():
        print(f"Plugin absent ({TARGET}) — rien a faire.")
        print("Definir NIDMI_SEQ_VST si le depot est ailleurs.")
        return 0

    data = json.loads(JSON_PATH.read_text(encoding="utf-8"))
    text = TARGET.read_text(encoding="utf-8")

    if BEGIN not in text or END not in text:
        print("Marqueurs absents de DeviceProfile.cpp. Encadrer la graine par :")
        print(f"  {BEGIN}\n  ...\n  {END}")
        return 1

    block = build_block(data)
    head = text[: text.index(BEGIN)]
    tail = text[text.index(END) + len(END) :]
    updated = head + block + tail

    body = build_json(data) + "\n"
    stale_seed = updated != text
    stale_file = (not PROFILE.exists()) or PROFILE.read_text(encoding="utf-8") != body

    if not stale_seed and not stale_file:
        print(f"OK — profil du plugin a jour ({len(entries(data))} controleurs).")
        return 0

    if "--write" in sys.argv:
        if stale_seed:
            TARGET.write_text(updated, encoding="utf-8")
            print("DeviceProfile.cpp : graine regeneree (recompiler le plugin).")
        if stale_file:
            PROFILE.parent.mkdir(parents=True, exist_ok=True)
            PROFILE.write_text(body, encoding="utf-8")
            print(f"{PROFILE} : ecrit.")
        return 0

    if stale_seed:
        print("La graine de DeviceProfile.cpp a DIVERGE de midi-map.json.")
    if stale_file:
        print(f"{PROFILE} a DIVERGE de midi-map.json.")
    print("Regenerer avec :  python3 tools/gen_device_profile.py --write")
    return 1


if __name__ == "__main__":
    sys.exit(main())

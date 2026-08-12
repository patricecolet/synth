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


def rows(data):
    out = []
    for e in data["parameters"] + data["modulation"]:
        out.append((int(e["cc"]), e["short"], e["name"], e["group"], bool(e["wired"])))
    return out


def build_json(data):
    """Le profil tel que le plugin le lira."""
    rs = rows(data)
    w_cc = max(len(str(r[0])) for r in rs)
    w_sh = max(len(f'"{r[1]}",') for r in rs)
    w_nm = max(len(f'"{r[2]}",') for r in rs)

    lines = ["{", '  "name": "Kobol Expander",', '  "parameters": [']
    for i, (cc, short, name, group, wired) in enumerate(rs):
        comma = "," if i < len(rs) - 1 else ""
        lines.append(
            '    {{ "cc": {cc:>{wc}}, "short": {sh:<{ws}} "name": {nm:<{wn}} '
            '"group": "{gr}", "wired": {wd:<5} }}{c}'.format(
                cc=cc, wc=w_cc,
                sh=f'"{short}",', ws=w_sh,
                nm=f'"{name}",', wn=w_nm,
                gr=group,
                wd="true" if wired else "false",
                c=comma,
            )
        )
    lines += ["  ]", "}"]
    return "\n".join(lines)


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
        print(f"OK — profil du plugin a jour ({len(rows(data))} controleurs).")
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

#!/usr/bin/env python3
"""Genere la table Kobol de DeviceProfile.cpp (plugin NiDMI Seq) depuis midi-map.json.

La carte MIDI a trois consommateurs : le firmware, ce depot, et le plugin
NiDMI Seq qui vit dans un autre depot. Recopier la table a la main entre les
deux a deja produit cinq divergences en deux jours — noms differents, et deux
`wired` faux. Elle est donc generee.

Le script reecrit uniquement le bloc entre les deux marqueurs de
DeviceProfile.cpp ; tout le reste du fichier est laisse intact.

    python3 tools/gen_device_profile.py              verifie et signale
    python3 tools/gen_device_profile.py --write      applique

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


def build_block(data):
    rows = []
    for entry in data["parameters"] + data["modulation"]:
        rows.append(
            (
                int(entry["cc"]),
                entry["short"],
                entry["name"],
                entry["group"],
                bool(entry["wired"]),
            )
        )

    w_short = max(len(f'"{r[1]}",') for r in rows)
    w_name = max(len(f'"{r[2]}",') for r in rows)

    lines = [
        BEGIN,
        "//",
        "//   cd <depot synth>/kobol-expander/midi-cv",
        "//   python3 tools/gen_device_profile.py --write",
        "//",
        "// La source est midi-map.json, qui fait foi pour le firmware aussi.",
        "// wired = le parametre a-t-il un effet AUDIBLE aujourd'hui.",
        "",
        "static const DeviceParam kKobolParams[] = {",
        "    //  cc   court      nom complet                groupe  cable",
    ]
    for cc, short, name, group, wired in rows:
        lines.append(
            "    {{ {cc:>3}, {short:<{ws}} {name:<{wn}} {group:<8} {wired:<5} }},".format(
                cc=cc,
                short=f'"{short}",',
                ws=w_short,
                name=f'"{name}",',
                wn=w_name,
                group=f'"{group}",',
                wired="true" if wired else "false",
            )
        )
    lines.append("};")
    lines.append(END)
    return "\n".join(lines)


def main():
    if not TARGET.exists():
        print(f"Plugin absent ({TARGET}) — rien a faire.")
        print("Definir NIDMI_SEQ_VST si le depot est ailleurs.")
        return 0

    data = json.loads(JSON_PATH.read_text(encoding="utf-8"))
    block = build_block(data)
    text = TARGET.read_text(encoding="utf-8")

    if BEGIN not in text or END not in text:
        print("Marqueurs absents de DeviceProfile.cpp. Encadrer la table par :")
        print(f"  {BEGIN}\n  ...\n  {END}")
        return 1

    head = text[: text.index(BEGIN)]
    tail = text[text.index(END) + len(END) :]
    updated = head + block + tail

    if updated == text:
        n = len(data["parameters"]) + len(data["modulation"])
        print(f"OK — DeviceProfile.cpp est a jour ({n} controleurs).")
        return 0

    if "--write" in sys.argv:
        TARGET.write_text(updated, encoding="utf-8")
        print(f"DeviceProfile.cpp regenere depuis midi-map.json.")
        print("Ne pas oublier de recompiler le plugin.")
        return 0

    print("DeviceProfile.cpp a DIVERGE de midi-map.json.")
    print("Regenerer avec :  python3 tools/gen_device_profile.py --write")
    return 1


if __name__ == "__main__":
    sys.exit(main())

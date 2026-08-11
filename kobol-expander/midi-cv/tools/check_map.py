#!/usr/bin/env python3
"""Vérifie que midi-map.json et KobolMidiCV/config.h décrivent la même chose.

Le JSON est lu par le VST, config.h par le firmware. Les deux dérivent des
mêmes mesures et doivent rester d'accord — sinon le plugin envoie un CC que
le firmware range ailleurs, et le symptôme est un paramètre qui bouge tout
seul, très pénible à diagnostiquer.

Usage :  python3 tools/check_map.py
Sortie :  0 si tout concorde, 1 sinon.
"""

import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
JSON_PATH = ROOT / "midi-map.json"
CONFIG_PATH = ROOT / "KobolMidiCV" / "output.cpp"   # la table PARAMS[] y est definie

STATE_MAP = {"PARAM_OK": "ok", "PARAM_CHECK": "check", "PARAM_BLOCKED": "blocked"}

# { 74, 15, DAC_CH_CUTOFF, 0, 900, 0, 90, PARAM_CHECK, "VCF Cutoff" },
ROW = re.compile(
    r"\{\s*(\d+)\s*,\s*(\d+)\s*,\s*(\w+)\s*,"
    r"\s*(-?\d+)\s*,\s*(-?\d+)\s*,\s*(-?\d+)\s*,\s*(\d+)\s*,"
    r"\s*(PARAM_\w+)\s*,\s*\"([^\"]+)\"\s*\}"
)


def parse_config(text):
    body = text[text.index("const KobolParam PARAMS[] = {"):]
    body = body[: body.index("\n};")]
    out = {}
    for m in ROW.finditer(body):
        cc, pin, dac, vmin, vmax, rest, dflt, state, name = m.groups()
        out[int(cc)] = {
            "name": name,
            "p1_pin": int(pin),
            "dac": dac,
            "mv": {"min": int(vmin), "max": int(vmax), "rest": int(rest)},
            "default": int(dflt),
            "state": STATE_MAP[state],
        }
    return out


def main():
    errors = []

    data = json.loads(JSON_PATH.read_text(encoding="utf-8"))
    conf = parse_config(CONFIG_PATH.read_text(encoding="utf-8"))
    js = {p["cc"]: p for p in data["parameters"]}

    only_json = sorted(set(js) - set(conf))
    only_conf = sorted(set(conf) - set(js))
    for cc in only_json:
        errors.append(f"CC {cc} ({js[cc]['name']}) : dans le JSON, absent de config.h")
    for cc in only_conf:
        errors.append(f"CC {cc} ({conf[cc]['name']}) : dans config.h, absent du JSON")

    for cc in sorted(set(js) & set(conf)):
        j, c = js[cc], conf[cc]
        if j["name"] != c["name"]:
            errors.append(f"CC {cc} : nom \"{j['name']}\" vs \"{c['name']}\"")
        # p1_pin vaut 255 dans config.h quand il n'y a pas de pin, null en JSON
        jp = j["p1_pin"] if j["p1_pin"] is not None else 255
        if jp != c["p1_pin"]:
            errors.append(f"CC {cc} ({j['name']}) : pin P1 {jp} vs {c['p1_pin']}")
        if j["state"] != c["state"]:
            errors.append(f"CC {cc} ({j['name']}) : etat {j['state']} vs {c['state']}")
        for k in ("min", "max", "rest"):
            if j["mv"][k] != c["mv"][k]:
                errors.append(
                    f"CC {cc} ({j['name']}) : mv.{k} {j['mv'][k]} vs {c['mv'][k]}"
                )
        if j.get("default") != c["default"]:
            errors.append(
                f"CC {cc} ({j['name']}) : defaut {j.get('default')} vs {c['default']}"
            )
        j_has_dac = j["dac_channel"] is not None
        c_has_dac = c["dac"] != "DAC_CH_NONE"
        if j_has_dac != c_has_dac:
            errors.append(
                f"CC {cc} ({j['name']}) : canal DAC {j['dac_channel']} vs {c['dac']}"
            )

    # Un CC ne doit jamais servir deux fois, sinon un mouvement du VST en
    # déplace deux à la fois.
    seen = {}
    for entry in data["parameters"] + data["modulation"]:
        cc = entry["cc"]
        if cc in seen:
            errors.append(f"CC {cc} attribue deux fois : {seen[cc]} et {entry['name']}")
        seen[cc] = entry["name"]

    if errors:
        print(f"{len(errors)} desaccord(s) entre midi-map.json et output.cpp :\n")
        for e in errors:
            print(f"  - {e}")
        return 1

    print(f"OK — {len(js)} parametres et {len(data['modulation'])} "
          f"controleurs de modulation concordent.")
    return 0


if __name__ == "__main__":
    sys.exit(main())

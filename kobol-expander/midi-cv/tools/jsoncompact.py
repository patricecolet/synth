"""Écriture JSON indentée, mais tableaux de scalaires et objets de paramètre
gardés EN LIGNE.

Un profil doit rester copiable et modifiable à la main — c'est tout l'intérêt
du format. `json.dumps(indent=2)` éclate chaque `rect` sur quatre lignes et
chaque paramètre sur quatorze, ce qui triple la longueur du fichier sans rien
apporter à la lecture.
"""

import json


def dumps(obj, indent=2):
    def enc(o, lvl):
        pad, pad2 = " " * (indent * lvl), " " * (indent * (lvl + 1))

        if isinstance(o, dict):
            # Un paramètre ou une section : assez court pour tenir sur une ligne,
            # et bien plus lisible en colonnes qu'éclaté.
            if {"cc", "name"} <= set(o) or {"id", "rect"} <= set(o):
                inner = ", ".join(
                    f"{json.dumps(k, ensure_ascii=False)}: {json.dumps(v, ensure_ascii=False)}"
                    for k, v in o.items()
                )
                return "{ " + inner + " }"
            if not o:
                return "{}"
            items = [
                f"{pad2}{json.dumps(k, ensure_ascii=False)}: {enc(v, lvl + 1)}"
                for k, v in o.items()
            ]
            return "{\n" + ",\n".join(items) + f"\n{pad}}}"

        if isinstance(o, list):
            if not o:
                return "[]"
            if all(not isinstance(x, (dict, list)) for x in o):
                return "[" + ", ".join(json.dumps(x, ensure_ascii=False) for x in o) + "]"
            return "[\n" + ",\n".join(f"{pad2}{enc(x, lvl + 1)}" for x in o) + f"\n{pad}]"

        return json.dumps(o, ensure_ascii=False)

    return enc(obj, 0) + "\n"

#!/usr/bin/env bash
#
# Compilation et téléversement de KobolMidiCV.
#
# Ce script existe pour une raison précise : le nom USB du port MIDI ne peut
# pas être posé depuis le sketch. Il vient de STR_PRODUCT, défini dans le core
# Teensy (cores/usb_midi/usb_private.h) sous #ifndef, et ce fichier est compilé
# avec le core, pas avec notre code. Un #define dans config.h arriverait trop
# tard : il faut un -D en ligne de commande.
#
# Et il ne peut pas passer par compiler.*.extra_flags : la plateforme Teensy
# n'utilise aucune de ces propriétés dans ses recettes. Le seul point d'entrée
# est build.flags.defs, qu'il faut donc recopier en entier — d'où la
# duplication de -DTEENSYDUINO et -DARDUINO_ARCH_AVR ci-dessous.
#
#   ./build.sh              compile
#   ./build.sh upload       compile puis téléverse
#   ./build.sh clean        recompile tout

set -euo pipefail
cd "$(dirname "$0")"

SKETCH=KobolMidiCV
FQBN='teensy:avr:teensy2:usb=midi,speed=16'

# Nom affiché par le système comme port MIDI.
USB_NAME='Kobol'

# Doit refléter teensy2.build.flags.defs de boards.txt, sinon on efface des
# défines dont le core a besoin. À revérifier après une mise à jour du core :
#   grep '^teensy2.build.flags.defs' ~/Library/Arduino15/packages/teensy/hardware/avr/*/boards.txt
CORE_DEFS='-DTEENSYDUINO=159 -DARDUINO_ARCH_AVR'

DEFS="build.flags.defs=${CORE_DEFS} -DSTR_PRODUCT=L\"${USB_NAME}\""

CLEAN=""
[[ "${1:-}" == "clean" ]] && CLEAN="--clean"

echo "→ compilation (${FQBN}, port MIDI « ${USB_NAME} »)"
arduino-cli compile $CLEAN -b "$FQBN" --build-property "$DEFS" "$SKETCH"

if [[ "${1:-}" == "upload" ]]; then
    # Le core Teensy délègue à l'application Teensy Loader ; sans elle, l'upload
    # échoue sur « Unable find Teensy Loader ».
    if ! pgrep -qf 'teensy.app/Contents/MacOS/teensy'; then
        LOADER=$(ls -d ~/Library/Arduino15/packages/teensy/tools/teensy-tools/*/teensy.app 2>/dev/null | tail -1)
        if [[ -n "$LOADER" ]]; then
            echo "→ lancement de Teensy Loader"
            open "$LOADER"
            sleep 3
        else
            echo "!! Teensy Loader introuvable — lance-le à la main" >&2
        fi
    fi

    PORT=$(arduino-cli board list 2>/dev/null | awk '/teensy/ {print $1; exit}')
    if [[ -z "$PORT" ]]; then
        echo "!! aucune carte Teensy detectee" >&2
        exit 1
    fi
    echo "→ televersement sur $PORT"
    arduino-cli upload -b "$FQBN" -p "$PORT" "$SKETCH"
    echo "→ fait. Verifier le nom du port :  swift tools/miditest.swift"
fi

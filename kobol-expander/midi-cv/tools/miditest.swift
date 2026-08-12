// miditest.swift — envoi MIDI vers l'interface Kobol, sans DAW
//
// Le firmware tourne en USB Type = MIDI : sur Teensy 2.0 il n'existe alors
// aucun port série USB, donc aucune trace de débogage. Le seul moyen de
// savoir ce qu'il fait est de lui envoyer du MIDI et d'écouter.
//
//   swift tools/miditest.swift                       liste les ports
//   swift tools/miditest.swift cc 74 100             un Control Change
//   swift tools/miditest.swift sweep 74              balayage 0->127->0
//   swift tools/miditest.swift note 60 100 500       note, vélocité, durée ms
//   swift tools/miditest.swift scale                 gamme chromatique
//
// Port visé par défaut : « Kobol », nom pose par build.sh via STR_PRODUCT.
// Surcharger avec la variable d'environnement KOBOL_MIDI_PORT.

import CoreMIDI
import Foundation

let portName = ProcessInfo.processInfo.environment["KOBOL_MIDI_PORT"] ?? "Kobol"
let channel: UInt8 = 0   // canal MIDI 1

func displayName(_ obj: MIDIObjectRef) -> String {
    var p: Unmanaged<CFString>?
    if MIDIObjectGetStringProperty(obj, kMIDIPropertyDisplayName, &p) == noErr,
       let s = p?.takeRetainedValue() { return s as String }
    return "?"
}

func findDestination(_ name: String) -> MIDIEndpointRef? {
    for i in 0 ..< MIDIGetNumberOfDestinations() {
        let d = MIDIGetDestination(i)
        if displayName(d) == name { return d }
    }
    return nil
}

var client = MIDIClientRef()
MIDIClientCreate("kobol-miditest" as CFString, nil, nil, &client)
var outPort = MIDIPortRef()
MIDIOutputPortCreate(client, "out" as CFString, &outPort)

func send(_ bytes: [UInt8]) {
    guard let dest = findDestination(portName) else { return }
    var packet = MIDIPacketList()
    let p = MIDIPacketListInit(&packet)
    _ = MIDIPacketListAdd(&packet, 1024, p, 0, bytes.count, bytes)
    MIDISend(outPort, dest, &packet)
}

func cc(_ number: UInt8, _ value: UInt8) { send([0xB0 | channel, number, value]) }
func noteOn(_ n: UInt8, _ v: UInt8)      { send([0x90 | channel, n, v]) }
func noteOff(_ n: UInt8)                 { send([0x80 | channel, n, 0]) }

let args = Array(CommandLine.arguments.dropFirst())

guard findDestination(portName) != nil else {
    print("Port « \(portName) » introuvable. Destinations disponibles :")
    for i in 0 ..< MIDIGetNumberOfDestinations() {
        print("  -", displayName(MIDIGetDestination(i)))
    }
    exit(1)
}

if args.isEmpty {
    print("Port trouve : « \(portName) »")
    print("Usage : cc <n> <val> | sweep <n> | note <n> <vel> <ms> | scale")
    exit(0)
}

switch args[0] {
case "cc" where args.count >= 3:
    let n = UInt8(args[1]) ?? 74, v = UInt8(args[2]) ?? 0
    cc(n, v)
    print("CC \(n) = \(v)")

case "sweep":
    let n = args.count >= 2 ? (UInt8(args[1]) ?? 74) : 74
    print("Balayage CC \(n) : 0 -> 127 -> 0")
    for v in stride(from: 0, through: 127, by: 1) {
        cc(n, UInt8(v)); usleep(12_000)
    }
    for v in stride(from: 127, through: 0, by: -1) {
        cc(n, UInt8(v)); usleep(12_000)
    }
    print("termine")

case "note" where args.count >= 4:
    let n = UInt8(args[1]) ?? 60, v = UInt8(args[2]) ?? 100
    let ms = UInt32(args[3]) ?? 500
    noteOn(n, v); print("Note \(n) vel \(v) pendant \(ms) ms")
    usleep(ms * 1000)
    noteOff(n)

case "scale":
    print("Gamme chromatique Do3 -> Do4 (notes 60 a 72)")
    for n in UInt8(60) ... UInt8(72) {
        noteOn(n, 100); usleep(220_000); noteOff(n); usleep(40_000)
    }

default:
    print("Commande inconnue. cc | sweep | note | scale")
    exit(1)
}

usleep(120_000)   // laisse partir les derniers paquets

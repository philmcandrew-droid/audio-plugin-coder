# MyReverb - Style Guide v2

## Changes from v1
- Deeper rack chassis with inset faceplate and worn-metal texture
- Knob housings with engraved tick scales and stronger 3D caps
- Classic **needle VU** (arc scale) replacing segmented bar
- Period typography: serif stamped title, engraved control labels

## Color Palette

| Role | Hex | Usage |
|------|-----|--------|
| Chassis | `#3A3228` | Outer rack shell |
| Chassis edge | `#2A241C` | Drop shadow / depth |
| Panel cream | `#EDE6D6` | Inset faceplate (warmer than v1) |
| Panel wear | `#E0D8C8` | Noise overlay tint |
| Tobacco | `#4F3F32` | Knob skirts, label plates |
| Charcoal | `#1F1F1F` | Ticks, needle, title shadow |
| Copper | `#A66B35` | Rules, EMI stripe |
| EMI red | `#7A2230` | VU overload arc |
| Knob cap hi | `#E8E2D8` | Highlight |
| Knob cap lo | `#6B5A4A` | Skirt shadow |
| VU cream glass | `#D8D0C0` | Meter background |
| VU needle | `#1A1A1A` | Needle bar |

## Typography

- **Title:** `Georgia, 'Times New Roman', serif` — 20px, 700, uppercase, 0.14em tracking
- **Engraved effect:** `text-shadow: 0 1px 0 #fff, 0 -1px 0 #8A8070`
- **Subtitle:** 9px sans, copper, 0.14em tracking — "EMI PLATE & CHAMBER · LONDON"
- **Control labels:** 10px sans, uppercase, on mini **label plates** (inset rects)
- **Values:** 9px tabular, tobacco on cream

## Hardware Depth

- **Chassis:** 8px outer pad, 4px radius, dual box-shadow
- **Faceplate inset:** 6px margin inside chassis, inset shadow 4px
- **VU bezel:** Raised lip 3px, inner glass gradient, 48px arc height
- **Screws:** Slot-head style (cross slot via CSS linear-gradient)

## Knobs (v2)

| Size | Diameter | Ticks | Cap |
|------|----------|-------|-----|
| Large | 84px | 11 ticks, −135° to +135° | Domed radial + rim ring |
| Medium | 68px | 9 ticks | Same, scaled |

Tick ring: static `#knob-ticks` canvas per housing.

## VU Meter (v2)

- Canvas 400×72 — semicircular arc, −40 to +3 VU-style scale (cosmetic)
- Black needle with copper pivot dot
- Red overload marks above 0 dB region on arc
- Preview animates needle from `mix` parameter

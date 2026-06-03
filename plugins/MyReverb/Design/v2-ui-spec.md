# MyReverb - UI Specification v2

## Iteration Goals (from user feedback)
1. **More vintage hardware** — chassis, inset faceplate, worn metal, slot screws
2. **Knobs** — tick scales, stronger 3D, clearer pointer
3. **VU** — classic needle meter (1960s rack style)
4. **Typography** — stamped serif title, engraved labels on plates

## Layout (unchanged footprint)
- **Window:** 500 × 400 px
- Sections: Chassis → Faceplate → Header → VU bezel → Space row → Tone row

## Controls
Same parameter map as v1 — IDs unchanged for JUCE binding.

| Parameter | Label | Row | Widget |
|-----------|-------|-----|--------|
| `size` | SIZE | 1 | Large knob + tick ring |
| `decay` | DECAY | 1 | Large knob + tick ring |
| `mix` | MIX | 1 | Large knob + tick ring |
| `predelay` | PRE-DLY | 2 | Medium knob + tick ring |
| `damping` | DAMP | 2 | Medium knob + tick ring |
| `width` | WIDTH | 2 | Medium knob + tick ring |

## New Elements
- `#rack-chassis` — outer shell
- `#faceplate` — inset worn panel
- `.vu-bezel` — meter housing with glass lip
- `#wetMeter` — 400×72 needle VU canvas
- `.knob-housing` + `.knob-ticks` canvas per control
- `.label-plate` — recessed label backing

## v1 → v2 Visual Diff
| Area | v1 | v2 |
|------|----|----|
| Shell | Flat cream panel | Dark chassis + inset faceplate |
| VU | Horizontal segments | Arc needle meter |
| Knobs | Plain gradient disc | Tick ring + domed cap + white pointer |
| Title | Sans uppercase | Georgia engraved stamp |
| Screws | Round dots | Slot-head detail |

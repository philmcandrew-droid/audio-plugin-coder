# MyReverb - UI Specification v4

## Iteration Goals
1. **VU meter** — larger, clearer scale, painted red overload sector
2. **Window size** — **580 × 600 px** (fits stereo VU + knob rows)

## VU (v4 — stereo)
- **Dual meters:** `wetMeterL` and `wetMeterR`, each **234 × 118 px**
- Shared scale legend below both units
- Production: drive from stereo wet L/R levels
- Arc span: ~220° (wider than v3)
- Scale: −20, −10, −7, −5, −3, 0, +1, +2, +3 VU
- **Red zone:** arc fill from 0 to +3 (overload)
- **Green zone:** subtle tint −20 to −3
- Silkscreen "VU" + "WET" labels on bezel

## Window
| Version | Size |
|---------|------|
| v1–v2 | 500 × 400 |
| v3 | 540 × 440 |
| v4 | **580 × 600** (stereo VU) |

Knob sizes unchanged from v3 (88 / 72 px).

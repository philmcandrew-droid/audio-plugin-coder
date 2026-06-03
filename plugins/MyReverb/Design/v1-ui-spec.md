# MyReverb - UI Specification v1

## Design Source
- **Source:** Creative brief (Dream phase) — Abbey Road 1960s rack aesthetic
- **Framework:** WebView (HTML/CSS/Canvas)
- **Version:** v1

## Layout

- **Window:** 500 × 400 px
- **Structure:**
  1. Header — plugin name + subtitle
  2. Wet VU meter (canvas)
  3. **Space row** — Size, Decay, Mix (large knobs)
  4. **Tone row** — Pre-delay, Damping, Width (medium knobs)

```
┌──────────────────────────────────────────────┐
│  MY REVERB          Abbey Road Style         │
│  ───────────── copper rule ─────────────     │
│  ┌────────────────────────────────────────┐  │
│  │▓▓▓▓▓▓▓░░░░░░  WET  (canvas VU)       │  │
│  └────────────────────────────────────────┘  │
│     (SIZE)      (DECAY)       (MIX)          │
│   [  80px ]     [  80px ]     [  80px ]      │
│                                              │
│   (PRE-DLY)     (DAMP)        (WIDTH)        │
│   [  64px ]     [  64px ]     [  64px ]      │
└──────────────────────────────────────────────┘
```

## Controls

| Parameter ID | Label | Type | Position | Range | Default | Display |
|--------------|-------|------|----------|-------|---------|---------|
| `size` | SIZE | Large rotary | Row 1, col 1 | 0.0 – 1.0 | 0.35 | 0–100% |
| `decay` | DECAY | Large rotary | Row 1, col 2 | 0.1 – 20.0 s | 2.5 s | seconds |
| `mix` | MIX | Large rotary | Row 1, col 3 | 0.0 – 1.0 | 0.25 | 0–100% |
| `predelay` | PRE-DLY | Medium rotary | Row 2, col 1 | 0 – 200 ms | 20 ms | ms |
| `damping` | DAMP | Medium rotary | Row 2, col 2 | 0.0 – 1.0 | 0.45 | 0–100% |
| `width` | WIDTH | Medium rotary | Row 2, col 3 | 0.0 – 1.0 | 0.70 | 0–100% |

## Element IDs (JUCE relay binding)

Each control uses IDs matching `parameter-spec.md`:

- `size-control`, `size-knob`, `size-value`
- `decay-control`, `decay-knob`, `decay-value`
- `mix-control`, `mix-knob`, `mix-value`
- `predelay-control`, `predelay-knob`, `predelay-value`
- `damping-control`, `damping-knob`, `damping-value`
- `width-control`, `width-knob`, `width-value`
- `wetMeter` (canvas)

## Color Palette
- Background: `#F5F0E6`
- Primary text: `#2B2B2B`
- Accent: `#B87333` (copper)
- Highlight: `#8B2635` (EMI red, title stripe only)

## Style Notes
- Rack-mount faceplate with cosmetic corner screws
- Knobs drawn in CSS (gradients) — no bitmap assets in v1
- Canvas VU shows wet signal; subtle segment lines for 1960s meter look
- Preview HTML is self-contained; production copies to `Source/ui/public/index.html` at `/impl`

## Out of Scope (v1 UI)
- Preset browser
- Mode selector
- Spectrum analyzer

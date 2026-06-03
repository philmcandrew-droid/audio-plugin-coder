# MyReverb - Style Guide v1

## Theme
**Retro 1960s Abbey Road rack unit** — warm cream faceplate, brushed-metal knobs, restrained EMI-era branding. Utility-first, no neon or modern flat DAW chrome.

## Color Palette

| Role | Hex | Usage |
|------|-----|--------|
| Panel cream | `#F5F0E6` | Main faceplate background |
| Panel shadow | `#E8E0D4` | Inner bezel gradient edge |
| Tobacco brown | `#5C4A3A` | Knob bodies, section dividers |
| Charcoal | `#2B2B2B` | Labels, tick marks, title |
| Copper accent | `#B87333` | Brand stripe, VU peak tint |
| EMI red | `#8B2635` | Subtle logo / accent line (sparingly) |
| Knob cap | `#D4CFC4` | Knob top highlight |
| Knob rim | `#3D3D3D` | Knob outer ring |
| VU green | `#4A7C59` | Wet meter fill (vintage VU) |
| VU background | `#C9C4B8` | Meter trough |

## Typography

- **Family:** `'Franklin Gothic Medium', 'Arial Narrow', 'Segoe UI', sans-serif`
- **Plugin title:** 18px, 600 weight, uppercase, 0.12em letter-spacing, charcoal
- **Subtitle:** 10px, 400 weight, copper, uppercase, 0.08em letter-spacing
- **Knob labels (large):** 11px, 600 weight, uppercase, charcoal
- **Knob labels (medium):** 10px, 600 weight, uppercase, charcoal
- **Value readouts:** 10px, tabular-nums, tobacco brown

## Spacing & Layout

- **Window:** 500 × 400 px (fixed, `overflow: hidden`)
- **Outer padding:** 16px
- **Header height:** 48px (title + subtitle)
- **VU meter:** 420 × 36 px, centered
- **Row gap:** 24px between VU and controls, 20px between knob rows
- **Knob row padding:** 12px horizontal inset
- **Knob gap:** 28px (large row), 24px (medium row)

## Controls

### Large rotaries (top row)
- **Diameter:** 80px
- **Travel:** 270° (−135° to +135°)
- **Cap:** Radial gradient (knob cap → tobacco)
- **Indicator:** 3px × 22px charcoal line

### Medium rotaries (bottom row)
- **Diameter:** 64px
- Same travel and indicator proportions scaled

### Interaction
- Relative vertical drag, 0.45°/px sensitivity
- Double-click resets to default
- No hover glow (period hardware)

## Decorative Elements

- **Top copper rule:** 2px × 60% width, centered under title
- **Panel screws:** Four 6px circles at corners (#9A9088 fill, #6B655C stroke) — cosmetic
- **Bezel:** 1px `#D9D2C6` inset shadow on main panel

## Canvas (VU Meter)

- 2D canvas, `#wetMeter`
- Horizontal bar, vintage segmented appearance (drawn rects)
- Displays normalized wet level 0–1 (preview: animated demo; production: C++ relay)

## CSS Variables

```css
:root {
  --bg-panel: #F5F0E6;
  --bg-shadow: #E8E0D4;
  --brown: #5C4A3A;
  --charcoal: #2B2B2B;
  --copper: #B87333;
  --emi-red: #8B2635;
  --knob-cap: #D4CFC4;
  --vu-fill: #4A7C59;
  --vu-trough: #C9C4B8;
}
```

## Accessibility

- Minimum label contrast 4.5:1 on cream (charcoal text)
- Hit targets ≥ 44px (knob diameter satisfies)

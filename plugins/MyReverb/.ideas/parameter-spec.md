# MyReverb - Parameter Specification

## Core Parameters

All six controls are always visible — two groups of three as defined in ideation.

### Space

| ID | Name | Type | Range | Default | Unit | Description |
|:---|:---|:---|:---|:---|:---|:---|
| `size` | Size | Float | 0.0 - 1.0 | 0.35 | normalized | Room scale: low = tight plate; high = diffuse chamber |
| `decay` | Decay | Float | 0.1 - 20.0 | 2.5 | s | Reverb tail length (logarithmic mapping) |
| `mix` | Mix | Float | 0.0 - 1.0 | 0.25 | normalized | Wet/dry blend (equal-power crossfade) |

### Tone & Image

| ID | Name | Type | Range | Default | Unit | Description |
|:---|:---|:---|:---|:---|:---|:---|
| `predelay` | Pre-delay | Float | 0.0 - 200.0 | 20.0 | ms | Delay before reverb onset |
| `damping` | Damping | Float | 0.0 - 1.0 | 0.45 | normalized | High-frequency decay on tail (0 = bright, 1 = dark) |
| `width` | Width | Float | 0.0 - 1.0 | 0.70 | normalized | Stereo spread of wet signal (0.5 = mono wet, 1.0 = wide) |

## Parameter Behavior

### Size

- **Range:** 0.0 to 1.0 (normalized)
- **Scaling:** Linear
- **Default:** 0.35 (compact plate-ish space)
- **Behavior:** Modulates early reflection density and late-field diffusion; lower values emphasize plate-like density, higher values chamber-like bloom
- **Smoothing:** Required (≈20–50 ms ramp)

### Decay

- **Range:** 0.1 s to 20.0 s
- **Scaling:** Logarithmic (musical steps per decade)
- **Default:** 2.5 s (classic vocal plate territory)
- **Behavior:** RT60-style tail; capped to prevent runaway feedback in self-contained use
- **Smoothing:** Required (≈50–100 ms ramp)

### Mix

- **Range:** 0.0 to 1.0 (dry at 0, wet at 1)
- **Scaling:** Equal-power crossfade (sin/cos law) to maintain perceived level
- **Default:** 0.25 (mix-ready send level)
- **Smoothing:** Required (≈10–30 ms ramp)

### Pre-delay

- **Range:** 0 ms to 200 ms
- **Scaling:** Linear in milliseconds; optional fine resolution below 50 ms
- **Default:** 20 ms (vocal separation without audible gap)
- **Smoothing:** Required (≈10–30 ms ramp)

### Damping

- **Range:** 0.0 to 1.0 (normalized)
- **Scaling:** Linear control → low-pass or HF shelf on wet tail only
- **Default:** 0.45 (gentle Abbey Road–style treble soften)
- **Behavior:** At 0, full bandwidth tail; at 1, strongly darkened tail (chamber/distant plate)
- **Smoothing:** Required (≈20–50 ms ramp)

### Width

- **Range:** 0.0 to 1.0 (normalized)
- **Scaling:** Linear mapped to wet stereo width via M/S matrix
- **Default:** 0.70 (modest stereo spread)
- **Behavior:** 0.5 = mono-compatible wet; 1.0 = maximum safe spread without phase issues on mono sum
- **Smoothing:** Required (≈10–30 ms ramp)

## Technical Requirements

### Real-Time Safety

- All parameters smoothed on the audio thread
- No allocation in `processBlock`
- Lock-free atomic parameter reads

### Parameter Persistence

- Full state recall with DAW sessions
- Automation on all six continuous parameters
- Preset support deferred to implementation phase

## UI Mapping (Retro 1960s)

| Control | Suggested widget | Label style |
|---------|------------------|-------------|
| Size | Large rotary | SIZE |
| Decay | Large rotary | DECAY |
| Mix | Large rotary | MIX |
| Pre-delay | Medium rotary | PRE-DLY |
| Damping | Medium rotary | DAMP |
| Width | Medium rotary | WIDTH |

Knobs arranged in two rows: **Size | Decay | Mix** top; **Pre-delay | Damping | Width** bottom. Period-appropriate caps and cream panel per creative brief.

## Future Expansion (Not v1)

- Plate / Chamber / Room mode switch
- Input high-pass (low-cut on reverb send)
- Synced pre-delay to host tempo
- Preset factory bank ("Studio 2", "EMT Plate", etc.)

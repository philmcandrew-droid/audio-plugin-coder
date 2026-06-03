# MyReverb - DSP Architecture Specification

## Design Goals

Deliver a **vintage plate/chamber stereo reverb** in one engine: warm tails, musical decay, HF damping in the feedback path, and mono-safe stereo width — without convolution or multi-mode switching in v1.

## Core Components

| Component | Purpose |
|-----------|---------|
| **Input conditioner** | Stereo dry tap; optional soft clip on wet send (fixed threshold, not user-facing) |
| **Pre-delay line** | Stereo `juce::dsp::DelayLine` (max 200 ms + headroom) |
| **FDN reverb core** | 8-channel feedback delay network with Hadamard mixing matrix |
| **Size morph module** | Interpolates delay lengths, diffusion depth, and input diffusion between plate and chamber targets |
| **Feedback damping filter** | One-pole LPF per FDN channel in feedback path (Damping parameter) |
| **Modulation (subtle)** | Low-rate LFO on select delay lines for plate “shimmer” without obvious chorus |
| **Wet tone shaper** | Gentle HF shelf on wet bus post-FDN (reinforces Damping at extremes) |
| **Width processor** | M/S matrix on wet only; dry path untouched |
| **Mix stage** | Equal-power dry/wet crossfade |

## Processing Chain

```
                    ┌─────────────────────────────────────────┐
  Input L/R ────────┤ Dry tap ────────────────────────────────┼──┐
                    │                                         │  │
                    └─► Pre-delay L/R ─► Input diffusion AP ─► FDN │  │
                              │              (Size-morphed)    │  │
                              │         ┌──────┴──────┐         │  │
                              │         │ 8× delay  │◄────────┤  │
                              │         │ + LPF fb  │  Damping │  │
                              │         │ + matrix  │          │  │
                              │         └──────┬──────┘         │  │
                              │                ▼                │  │
                              │         Wet tone shelf          │  │
                              │                ▼                │  │
                              │         Width (M/S wet)         │  │
                              └────────────────┬────────────────┘  │
                                               ▼                   │
                                         Mix (equal-power) ◄───────┘
                                               ▼
                                         Output L/R
```

## FDN Core (v1)

### Topology

- **8 delay lines** with lengths derived from host sample rate and `size` (log-spaced base delays, scaled plate ↔ chamber)
- **Hadamard 8×8** feedback matrix (fixed, orthonormal — stable energy)
- **Input injection:** Dry (post pre-delay) summed into all lines with Size-dependent gain weights (plate: peaked center lines; chamber: more even spread)
- **Output sum:** Weighted sum of delay outputs → stereo via complementary L/R pan weights on line groups

### Plate vs Chamber (Size morph)

| Size region | Character | Delay scale | Diffusion | Mod depth |
|-------------|-----------|-------------|-----------|-----------|
| 0.0 – 0.35 | Tight EMT plate | 0.6× base | Light (1 AP stage) | Low |
| 0.35 – 0.65 | Plate + room | 1.0× base | Medium (2 AP) | Medium |
| 0.65 – 1.0 | Echo chamber | 1.4× base | Heavy (3 AP) | Low–medium |

Interpolation is continuous across `size` (no mode switch).

### Decay mapping

- User `decay` (0.1–20 s) maps to **feedback gain** per line: \( g = \exp(-6.9 \cdot T_{line} / T_{60}) \) where \(T_{line}\) is per-line delay in seconds
- Global cap: \( g_{max} = 0.98 \) to prevent runaway in self-contained use
- Smoothed on parameter change (50–100 ms)

### Damping mapping

- `damping` 0→1 maps to LPF cutoff 12 kHz → 800 Hz in feedback path (per channel)
- Additional gentle shelf on wet bus: −0 to −6 dB at 8 kHz for final darken at high damping

### Pre-delay

- 0–200 ms linear; fractional delay with linear interpolation
- Latency reporting: `predelay_samples + FDN_max_delay` for PDC

### Width

- Convert wet L/R → M/S
- `width` 0.5 → S gain 0 (mono wet); 1.0 → S gain +3 dB (capped for mono compatibility)
- Limit stereo enhancement: \( |S| \leq 0.9|M| \) after scaling

### Mix

- Equal-power: `dryGain = cos(mix * π/2)`, `wetGain = sin(mix * π/2)`
- Dry taken from input tap **before** pre-delay; wet from width processor

## Parameter Mapping

| Parameter | Component | Function | Range |
|-----------|-----------|----------|-------|
| `size` | Size morph, FDN lengths, diffusion, input weights | Plate ↔ chamber character | 0.0 – 1.0 |
| `decay` | FDN feedback gains | Tail length (T60) | 0.1 – 20.0 s |
| `mix` | Mix stage | Dry/wet balance | 0.0 – 1.0 |
| `predelay` | Pre-delay line | Time before reverb | 0 – 200 ms |
| `damping` | Feedback LPF + wet shelf | HF loss on tail | 0.0 – 1.0 |
| `width` | M/S wet processor | Stereo spread | 0.0 – 1.0 |

## Smoothing (Audio Thread)

| Parameter | Ramp time |
|-----------|-----------|
| size | 30 ms |
| decay | 80 ms |
| mix | 20 ms |
| predelay | 25 ms |
| damping | 40 ms |
| width | 20 ms |

All via `juce::SmoothedValue<float>` or `juce::dsp::Ballistics` — no allocations in `processBlock`.

## Memory & Performance

- Pre-delay buffer: ~200 ms × 2 ch × float
- FDN: 8 lines × max ~120 ms each at 192 kHz (allocate at `prepareToPlay`)
- Allpass diffusion: 3 stages × 8 lines (short, fixed coefficients)
- Target CPU: < 5% @ 48 kHz, 512 buffer, modern desktop

## Complexity Assessment

**Score: 3**

**Rationale:**
- Multi-component chain (delay, FDN, filtering, M/S, crossfade) with coupled parameter mapping
- Size morph adds interpolated DSP state (not a single static reverb preset)
- Real-time stability requires capped feedback and tested matrix scaling
- Simpler than nonlinear analog modeling (score 4) or granular/spectral engines (score 4–5); more complex than a gain/filter plugin (score 1–2)

## Out of Scope (DSP v1)

- Convolution IRs
- Separate plate/chamber algorithms (morph only)
- Nonlinear drive on wet (optional v1.1)
- Synced tempo pre-delay

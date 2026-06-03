# MyReverb - Implementation Plan

## Complexity Score: 3

## Implementation Strategy: Phased

Reverb FDN + six smoothed parameters + Size morph exceeds single-pass comfort; implement in three code phases with testable milestones.

### Phase 2.1.1: Core Signal Path

- [ ] Plugin shell: `AudioProcessor` / `AudioProcessorEditor`, bus layout stereo in/out
- [ ] APVTS with six parameters per `parameter-spec.md`
- [ ] Pre-delay stereo delay lines (max 200 ms)
- [ ] Basic FDN (8 lines, fixed matrix) with `decay` → feedback only
- [ ] Dry/wet equal-power mix
- [ ] `prepareToPlay` buffer sizing; report latency

### Phase 2.1.2: Character & Controls

- [ ] Size morph: delay scale, diffusion AP count/strength, input weights
- [ ] Damping: feedback one-pole LPF per line + wet HF shelf
- [ ] Subtle modulation on 2–4 delay lines (plate zone only)
- [ ] Width: M/S wet matrix with mono-safe limiting
- [ ] Parameter smoothing on all six controls
- [ ] Sanity presets (hard-coded): “EMT Plate”, “Studio 2” as debug factory values

### Phase 2.1.3: Polish & Validation

- [ ] Denormal protection and DC-safe feedback
- [ ] Automation / state save-restore
- [ ] pluginval smoke pass
- [ ] A/B level match at default mix (no loudness jump)
- [ ] CPU profiling at 48/96 kHz

## UI Framework Decision

**Decision: webview**

**Rationale:**
- Creative brief mandates a **specific retro 1960s rack aesthetic** (cream panel #F5F0E6, tobacco brown, EMI copper accents, beveled metal knobs, period typography) — faster and more faithful with HTML/CSS/Canvas than native Visage styling from scratch
- Six static rotaries + optional VU meter: moderate control count, **high visual fidelity** requirement (WebView sweet spot per APC guidelines)
- No real-time spectrum or shader graph — Visage performance advantage is less critical than look-and-feel
- Matches successful APC plugins with custom visual identity (e.g. nf_gnarly)

**Alternative considered:** Visage — viable if WebView build tooling is unavailable; would simplify deployment but sacrifice period UI detail unless custom drawing is heavily invested.

## UI Implementation Notes (for Design phase)

- Canvas or CSS-based rotary controls with cream panel background
- Layout: 500×400 logical — top row SIZE / DECAY / MIX (large), bottom PRE-DLY / DAMP / WIDTH (medium)
- Subtle wet-level VU (optional, canvas bar)
- `juce::WebBrowserComponent` + `juce_audio_processors` WebView integration

## Dependencies

### Required JUCE Modules

- `juce_audio_basics`
- `juce_audio_processors`
- `juce_dsp` (DelayLine, IIR, ProcessContext)
- `juce_gui_extra` (WebBrowserComponent)

### APC / Build

- Standard APC plugin CMake template
- WebView assets in `plugins/MyReverb/Design/`

### Optional

- `juce_audio_utils` (level metering helpers)

## Risk Assessment

### High Risk

- **FDN stability** at long decay + low damping — mitigate with \(g_{max}\), matrix normalization, and test vectors at 20 s decay
- **Zipper noise** on Size/Decay — mandatory smoothing and crossfade when changing delay line lengths (re-init strategy: crossfade old/new FDN states over 50 ms)

### Medium Risk

- **Pre-delay + PDC** — host latency compensation must include max delay + FDN depth
- **Width mono compatibility** — verify mono sum on bus without phase cancellation
- **WebView2 runtime** on Windows target machines

### Low Risk

- Mix crossfade
- APVTS parameter binding
- Damping as one-pole LPF

## Testing Checklist

- [ ] Silence in → silence out (no DC offset growth)
- [ ] Impulse response length tracks Decay
- [ ] Damping darkens tail without killing early energy
- [ ] Pre-delay audible gap 0–200 ms
- [ ] Size audible shift plate → chamber
- [ ] Automation smooth on all parameters
- [ ] State recall restores sound

## Next Phase

Run **`/design MyReverb`** to produce WebView UI mockup and `Design/index.html` specification aligned with retro 1960s brief.

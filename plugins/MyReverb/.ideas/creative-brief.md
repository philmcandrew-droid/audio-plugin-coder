# MyReverb - Creative Brief

## Hook
**"The warmth of Abbey Road — legendary plate and chamber reverbs from the 1960s, in one plugin."**

MyReverb captures the dense, musical character of classic EMI-era studio spaces: EMT plate shimmer, echo-chamber depth, and the smooth high-frequency roll-off that defined Beatles-era records. Not a sterile algorithmic hall — a living, vintage tail that sits behind the mix instead of washing over it.

## Vision

A focused stereo reverb plugin inspired by Abbey Road Studios' iconic reverbs of the 1960s. The sound should feel **warm, forward, and tape-friendly**: short-to-long decays with natural damping, optional pre-delay for clarity, and adjustable stereo width for period-correct imaging or modern spread.

### Sonic Character

- **Plate heritage:** Bright attack, smooth sustain, characteristic HF damping — the EMT 140 family tone without harsh digital tails
- **Chamber depth:** Larger "Size" settings evoke Studio 2-style echo chambers: diffuse, enveloping, slightly darker than plate
- **Musical decay:** Decay times that reward musical settings; self-oscillation avoided in favor of controlled, mix-ready tails
- **Mix discipline:** Strong default wet/dry balance philosophy — reverb as glue, not flood

### Target Use Cases

1. **Vocal beds:** Classic 60s vocal ambience (Beatles, early Floyd, contemporary retro productions)
2. **Drums & percussion:** Room glue and plate sheen on snare and overheads
3. **Guitar & keys:** Short plate on rhythm; longer chamber on leads and pads
4. **Retro mixing:** Period-authentic ambience when paired with tape/saturation chains
5. **Modern vintage:** Lo-fi or indie productions wanting "recorded at Abbey Road" color without convolution libraries

## UX Principles

### Control Layout

Six primary knobs in two logical groups — no menu diving:

| Group | Controls | Role |
|-------|----------|------|
| **Space** | Size, Decay, Mix | Room scale, tail length, wet/dry |
| **Tone & Image** | Pre-delay, Damping, Width | Separation, HF roll-off, stereo field |

### Retro 1960s Aesthetic

- **Visual era:** Mid-century studio hardware — brushed metal, cream/off-white panels, simple typography (Helvetica-era or period sans-serif)
- **Layout:** Symmetric rack-style panel; large rotary controls with subtle shadow and beveled edges
- **Color palette:** Warm cream (#F5F0E6), tobacco brown accents, muted EMI red or copper highlights, dark charcoal labels
- **Feedback:** Minimal animation — gentle knob glow or VU-style wet meter; no neon or cyberpunk chrome
- **Scale:** Medium plugin window (~500×400 logical px); readable at 100% and 150% UI scale

### Simplicity First

- All parameters visible at once
- Sensible defaults for instant "Abbey Road plate" character on insert
- No hidden modes in v1 — one algorithm path, tuned for vintage plate/chamber blend via Size

## Inspiration References

### Hardware & Studios

- **EMT 140 plate reverb** — Abbey Road's workhorse; bright plate with adjustable damping
- **Abbey Road echo chambers** — Studio 2 and related spaces; diffuse, larger-than-plate character
- **REDD / TG console era** — Reverb returns blended on desks; slightly compressed, cohesive wet signal

### Sonic References

- Beatles vocal and drum ambience (1965–1969)
- Pink Floyd early albums — spacious but not icy
- Contemporary retro: artists seeking "analog room" without IR convolution weight

### Visual References

- 1960s EMI / Abbey Road equipment photography
- Neve/EMT rack units: simple legending, physical knobs, restrained branding
- Avoid: DAW-native flat gray, skeuomorphic wood grain, or futuristic HUD overlays

## Technical Philosophy (Ideation Only)

- Single high-quality stereo reverb engine with **Size** morphing early reflection density and late-field character (plate ↔ chamber)
- **Damping** as one-knob HF shelf on the tail (plate-style treble soften)
- **Pre-delay** 0–200 ms for source separation before bloom
- **Width** as M/S wet widening/narrowing without mono collapse at center
- Real-time safe parameter smoothing on all continuous controls
- Details of algorithm choice deferred to `/plan` phase

## Out of Scope (v1)

- Multiple reverb algorithms or IR loading
- Synced tempo pre-delay
- EQ section beyond Damping
- Preset browser (may follow in later phases)

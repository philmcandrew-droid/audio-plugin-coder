# MyReverb - UI Specification v3

## Iteration Goals
1. **Larger knobs** — easier grab targets
2. **Layout & spacing** — less cramped; 540×440 window
3. **Stronger branding** — model + serial metal plates, studio lineage in header

## Layout

```
┌────────────────────────────────────────────────────────┐
│ [TYPE 140A plate]     MY REVERB      [No. 64-LDN]      │
│                  Abbey Road Style                       │
│ ─────────────────── EMI stripe ─────────────────────    │
│              ┌── Needle VU (wider) ──┐                  │
│              SIZE    DECAY    MIX    (large knobs)        │
│              PRE-DLY  DAMP   WIDTH  (medium knobs)      │
└────────────────────────────────────────────────────────┘
```

## Controls
Unchanged parameter IDs from v1/v2.

## New DOM Elements
- `.model-plate` — left header badge
- `.serial-plate` — right header badge
- `.header-band` — flex row for plates + title

import * as Juce from "./juce/index.js";

const MIN_DEG = -135;
const MAX_DEG = 135;
const SPAN = MAX_DEG - MIN_DEG;
const VU_DB = [-20, -10, -7, -5, -3, 0, 1, 2, 3];

const PARAM_IDS = ["size", "decay", "mix", "predelay", "damping", "width"];
const defaults = {
  size: 0.35,
  decay: decayToNorm(2.5),
  mix: 0.25,
  predelay: 0.1,
  damping: 0.45,
  width: 0.70
};

const states = {};
let vuL = 0.32;
let vuR = 0.38;

function decayToNorm(sec) {
  return Math.log(sec / 0.1) / Math.log(200);
}

function normToDecay(n) {
  return 0.1 * Math.pow(200, n);
}

function normToDeg(n) {
  return MIN_DEG + n * SPAN;
}

function degToNorm(deg) {
  return Math.max(0, Math.min(1, (deg - MIN_DEG) / SPAN));
}

function dbToAngle(db) {
  const min = VU_DB[0];
  const max = VU_DB[VU_DB.length - 1];
  const t = (db - min) / (max - min);
  const a0 = Math.PI * 1.05;
  const a1 = Math.PI * 1.95;
  return a0 + t * (a1 - a0);
}

function formatValue(id, norm) {
  if (id === "decay") return normToDecay(norm).toFixed(1) + " s";
  if (id === "predelay") return Math.round(norm * 200) + " ms";
  return Math.round(norm * 100) + "%";
}

function setKnobVisual(id, norm) {
  const ind = document.getElementById(id + "-indicator");
  const val = document.getElementById(id + "-value");
  if (ind) ind.style.transform = `translateX(-50%) rotate(${normToDeg(norm)}deg)`;
  if (val) val.textContent = formatValue(id, norm);
}

function drawNeedleVu(canvas, level) {
  if (!canvas) return;
  const vctx = canvas.getContext("2d");
  const w = canvas.width;
  const h = canvas.height;
  const cx = w / 2;
  const cy = h - 12;
  const r = Math.min(w, h) * 0.48;

  vctx.fillStyle = "#E8E4DA";
  vctx.fillRect(0, 0, w, h);

  vctx.fillStyle = "rgba(90, 138, 98, 0.14)";
  vctx.beginPath();
  vctx.moveTo(cx, cy);
  vctx.arc(cx, cy, r - 4, dbToAngle(-20), dbToAngle(-3), false);
  vctx.closePath();
  vctx.fill();

  vctx.fillStyle = "rgba(158, 42, 50, 0.32)";
  vctx.beginPath();
  vctx.moveTo(cx, cy);
  vctx.arc(cx, cy, r - 4, dbToAngle(0), dbToAngle(3), false);
  vctx.closePath();
  vctx.fill();

  const a0 = Math.PI * 1.05;
  const a1 = Math.PI * 1.95;
  vctx.strokeStyle = "#2B2B2B";
  vctx.lineWidth = 2.5;
  vctx.beginPath();
  vctx.arc(cx, cy, r, a0, a1, false);
  vctx.stroke();

  VU_DB.forEach((db) => {
    const a = dbToAngle(db);
    const overload = db >= 0;
    const inner = r - (overload ? 18 : 14);
    vctx.strokeStyle = overload ? "#9E2A32" : "#4F3F32";
    vctx.lineWidth = db === 0 || db === 3 ? 2.5 : 1.5;
    vctx.beginPath();
    vctx.moveTo(cx + Math.cos(a) * inner, cy + Math.sin(a) * inner);
    vctx.lineTo(cx + Math.cos(a) * r, cy + Math.sin(a) * r);
    vctx.stroke();

    if (db === -20 || db === -10 || db === 0 || db === 3) {
      vctx.fillStyle = overload ? "#9E2A32" : "#1F1F1F";
      vctx.font = db === 3 ? "bold 9px sans-serif" : "8px sans-serif";
      vctx.textAlign = "center";
      vctx.fillText(
        db > 0 ? "+" + db : String(db),
        cx + Math.cos(a) * (r - 28),
        cy + Math.sin(a) * (r - 28) + 3
      );
    }
  });

  const dbNeedle = -20 + level * 22;
  const na = dbToAngle(Math.min(3, dbNeedle));
  vctx.strokeStyle = "#141414";
  vctx.lineWidth = 2.5;
  vctx.beginPath();
  vctx.moveTo(cx, cy);
  vctx.lineTo(cx + Math.cos(na) * (r - 8), cy + Math.sin(na) * (r - 8));
  vctx.stroke();

  vctx.fillStyle = "#A66B35";
  vctx.beginPath();
  vctx.arc(cx, cy, 5, 0, Math.PI * 2);
  vctx.fill();
  vctx.strokeStyle = "#2A241C";
  vctx.stroke();

  const grd = vctx.createLinearGradient(0, 0, 0, h * 0.35);
  grd.addColorStop(0, "rgba(255,255,255,0.3)");
  grd.addColorStop(1, "transparent");
  vctx.fillStyle = grd;
  vctx.fillRect(0, 0, w, h * 0.4);
}

function drawVuMeters() {
  drawNeedleVu(document.getElementById("wetMeterL"), vuL);
  drawNeedleVu(document.getElementById("wetMeterR"), vuR);
}

window.updateVuMeters = (left, right) => {
  vuL = Math.max(0, Math.min(1, left));
  vuR = Math.max(0, Math.min(1, right));
  drawVuMeters();
};

function initTickRings() {
  document.querySelectorAll(".knob-ticks").forEach((canvas) => {
    const ctx = canvas.getContext("2d");
    const w = canvas.width;
    const h = canvas.height;
    const cx = w / 2;
    const cy = h / 2;
    const isLarge = canvas.dataset.size === "large";
    const outer = isLarge ? 52 : 42;
    const count = isLarge ? 13 : 11;
    const start = (-135 * Math.PI) / 180;
    const end = (135 * Math.PI) / 180;
    ctx.clearRect(0, 0, w, h);
    for (let i = 0; i < count; i++) {
      const t = i / (count - 1);
      const a = start + t * (end - start);
      const major = i === 0 || i === count - 1 || i === Math.floor(count / 2);
      const len = major ? 8 : 5;
      ctx.strokeStyle = major ? "#1F1F1F" : "#5C4A3A";
      ctx.lineWidth = major ? 2 : 1;
      ctx.beginPath();
      ctx.moveTo(cx + Math.sin(a) * (outer - len), cy - Math.cos(a) * (outer - len));
      ctx.lineTo(cx + Math.sin(a) * outer, cy - Math.cos(a) * outer);
      ctx.stroke();
    }
  });
}

function bindKnob(paramId, state) {
  const knob = document.querySelector(`.knob[data-param="${paramId}"]`);
  if (!knob) return;

  const indicator = knob.querySelector(".knob-indicator");
  let dragging = false;
  let lastY = 0;
  let rotation = normToDeg(state.getNormalisedValue());

  const syncFromHost = () => {
    const norm = state.getNormalisedValue();
    rotation = normToDeg(norm);
    indicator.style.transform = `translateX(-50%) rotate(${rotation}deg)`;
    setKnobVisual(paramId, norm);
  };

  indicator.style.transform = `translateX(-50%) rotate(${rotation}deg)`;
  syncFromHost();

  state.valueChangedEvent.addListener(syncFromHost);

  knob.addEventListener("mousedown", (e) => {
    dragging = true;
    lastY = e.clientY;
    state.sliderDragStarted();
    e.preventDefault();
  });

  knob.addEventListener("dblclick", () => {
    state.setNormalisedValue(defaults[paramId]);
    syncFromHost();
  });

  document.addEventListener("mousemove", (e) => {
    if (!dragging) return;
    rotation += (lastY - e.clientY) * 0.45;
    rotation = Math.max(MIN_DEG, Math.min(MAX_DEG, rotation));
    lastY = e.clientY;
    state.setNormalisedValue(degToNorm(rotation));
    indicator.style.transform = `translateX(-50%) rotate(${rotation}deg)`;
    setKnobVisual(paramId, state.getNormalisedValue());
  });

  document.addEventListener("mouseup", () => {
    if (dragging) {
      dragging = false;
      state.sliderDragEnded();
    }
  });
}

document.addEventListener("DOMContentLoaded", () => {
  initTickRings();
  drawVuMeters();

  const juceAvailable = typeof window.__JUCE__ !== "undefined";

  if (juceAvailable) {
    PARAM_IDS.forEach((id) => {
      states[id] = Juce.getSliderState(id);
      bindKnob(id, states[id]);
    });
  } else {
    const local = { ...defaults };
    PARAM_IDS.forEach((id) => setKnobVisual(id, local[id]));
    document.querySelectorAll(".knob").forEach((knob) => {
      const paramId = knob.dataset.param;
      const indicator = knob.querySelector(".knob-indicator");
      let dragging = false;
      let lastY = 0;
      let rotation = normToDeg(local[paramId]);
      indicator.style.transform = `translateX(-50%) rotate(${rotation}deg)`;
      knob.addEventListener("mousedown", (e) => {
        dragging = true;
        lastY = e.clientY;
        e.preventDefault();
      });
      knob.addEventListener("dblclick", () => {
        local[paramId] = defaults[paramId];
        rotation = normToDeg(local[paramId]);
        setKnobVisual(paramId, local[paramId]);
      });
      document.addEventListener("mousemove", (e) => {
        if (!dragging) return;
        rotation += (lastY - e.clientY) * 0.45;
        rotation = Math.max(MIN_DEG, Math.min(MAX_DEG, rotation));
        lastY = e.clientY;
        local[paramId] = degToNorm(rotation);
        indicator.style.transform = `translateX(-50%) rotate(${rotation}deg)`;
        setKnobVisual(paramId, local[paramId]);
        vuL = Math.min(1, 0.15 + local.mix * 0.6);
        vuR = Math.min(1, 0.15 + local.mix * 0.55);
        drawVuMeters();
      });
      document.addEventListener("mouseup", () => { dragging = false; });
    });
    setInterval(() => {
      const t = Date.now() / 850;
      vuL = Math.min(1, 0.18 + local.mix * 0.58 + Math.sin(t) * 0.07);
      vuR = Math.min(1, 0.18 + local.mix * 0.58 + Math.sin(t + 0.4) * 0.07);
      drawVuMeters();
    }, 40);
  }

  console.log("MyReverb UI ready", juceAvailable ? "(JUCE)" : "(standalone)");
});

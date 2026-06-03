// Development utility to verify JUCE native integration
export function checkNativeInterop() {
    console.log("=== JUCE Native Interop Check ===");

    if (typeof window.__JUCE__ !== 'undefined') {
        console.log("✓ JUCE native backend detected");
        console.log("Available methods:", Object.keys(window.__JUCE__));
        return true;
    } else {
        console.warn("✗ JUCE native backend NOT detected");
        console.warn("Running in standalone browser mode");
        return false;
    }
}

// Auto-run check on load
checkNativeInterop();

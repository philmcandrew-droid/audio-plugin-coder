#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
constexpr float kMinDecay = 0.1f;
constexpr float kMaxDecay = 20.0f;

float decayToRoomSize (float sizeNorm, float decaySec)
{
    const float decayNorm = std::log (juce::jmax (kMinDecay, decaySec) / kMinDecay)
                          / std::log (kMaxDecay / kMinDecay);
    return juce::jlimit (0.05f, 0.98f, 0.12f + sizeNorm * 0.38f + decayNorm * 0.48f);
}
} // namespace

//==============================================================================
MyReverbAudioProcessor::MyReverbAudioProcessor()
    : AudioProcessor (BusesProperties()
                          .withInput ("Input", juce::AudioChannelSet::stereo(), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, juce::Identifier ("MyReverbParams"), createParameterLayout())
{
}

MyReverbAudioProcessor::~MyReverbAudioProcessor() = default;

juce::AudioProcessorValueTreeState::ParameterLayout MyReverbAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "size", 1 }, "Size",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.35f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "decay", 1 }, "Decay",
        juce::NormalisableRange<float> (kMinDecay, kMaxDecay, 0.01f, 0.35f), 2.5f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "mix", 1 }, "Mix",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.25f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "predelay", 1 }, "Pre-delay",
        juce::NormalisableRange<float> (0.0f, 200.0f, 0.1f), 20.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "damping", 1 }, "Damping",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.45f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "width", 1 }, "Width",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.70f));

    return layout;
}

void MyReverbAudioProcessor::updateReverbParameters (float sizeNorm, float decaySec, float dampingNorm, float widthNorm)
{
    juce::dsp::Reverb::Parameters params;
    params.roomSize = decayToRoomSize (sizeNorm, decaySec);
    params.damping = dampingNorm;
    params.wetLevel = 1.0f;
    params.dryLevel = 0.0f;
    params.width = juce::jlimit (0.0f, 1.0f, (widthNorm - 0.5f) * 2.0f);
    params.freezeMode = 0.0f;
    reverb.setParameters (params);
}

//==============================================================================
const juce::String MyReverbAudioProcessor::getName() const { return JucePlugin_Name; }
bool MyReverbAudioProcessor::acceptsMidi() const { return false; }
bool MyReverbAudioProcessor::producesMidi() const { return false; }
bool MyReverbAudioProcessor::isMidiEffect() const { return false; }
double MyReverbAudioProcessor::getTailLengthSeconds() const { return kMaxDecay; }
int MyReverbAudioProcessor::getNumPrograms() { return 1; }
int MyReverbAudioProcessor::getCurrentProgram() { return 0; }
void MyReverbAudioProcessor::setCurrentProgram (int) {}
const juce::String MyReverbAudioProcessor::getProgramName (int) { return {}; }
void MyReverbAudioProcessor::changeProgramName (int, const juce::String&) {}

bool MyReverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto out = layouts.getMainOutputChannelSet();
    if (out != juce::AudioChannelSet::mono() && out != juce::AudioChannelSet::stereo())
        return false;
    return layouts.getMainInputChannelSet() == out;
}

void MyReverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    maxPredelaySamples = (int) std::ceil (sampleRate * 0.2) + 4;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = (juce::uint32) samplesPerBlock;
    spec.numChannels = 2;

    delayLineL.prepare (spec);
    delayLineR.prepare (spec);
    delayLineL.setMaximumDelayInSamples (maxPredelaySamples);
    delayLineR.setMaximumDelayInSamples (maxPredelaySamples);

    reverb.prepare (spec);
    wetBuffer.setSize (2, samplesPerBlock);

    mixSmoothed.reset (sampleRate, 0.03);
    predelaySmoothed.reset (sampleRate, 0.025);
    mixSmoothed.setCurrentAndTargetValue (0.25f);
    predelaySmoothed.setCurrentAndTargetValue (20.0f);
}

void MyReverbAudioProcessor::releaseResources()
{
    delayLineL.reset();
    delayLineR.reset();
    reverb.reset();
}

void MyReverbAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ignoreUnused (midi);
    juce::ScopedNoDenormals noDenormals;

    const auto numSamples = buffer.getNumSamples();
    const auto numCh = juce::jmin (2, buffer.getNumChannels());

    if (numSamples == 0 || numCh == 0)
        return;

    const float sizeNorm = apvts.getRawParameterValue ("size")->load();
    const float decaySec = apvts.getRawParameterValue ("decay")->load();
    const float mixTarget = apvts.getRawParameterValue ("mix")->load();
    const float predelayMs = apvts.getRawParameterValue ("predelay")->load();
    const float dampingNorm = apvts.getRawParameterValue ("damping")->load();
    const float widthNorm = apvts.getRawParameterValue ("width")->load();

    mixSmoothed.setTargetValue (mixTarget);
    predelaySmoothed.setTargetValue (predelayMs);
    updateReverbParameters (sizeNorm, decaySec, dampingNorm, widthNorm);

    wetBuffer.setSize (numCh, numSamples, false, false, true);
    for (int ch = 0; ch < numCh; ++ch)
        wetBuffer.copyFrom (ch, 0, buffer, ch, 0, numSamples);

    predelaySmoothed.skip (numSamples - 1);
    const float predelaySamples = (float) (predelaySmoothed.getCurrentValue() * 0.001 * currentSampleRate);
    delayLineL.setDelay (predelaySamples);
    delayLineR.setDelay (predelaySamples);

    for (int i = 0; i < numSamples; ++i)
    {
        if (numCh > 0)
        {
            auto* L = wetBuffer.getWritePointer (0);
            delayLineL.pushSample (0, L[i]);
            L[i] = delayLineL.popSample (0);
        }
        if (numCh > 1)
        {
            auto* R = wetBuffer.getWritePointer (1);
            delayLineR.pushSample (0, R[i]);
            R[i] = delayLineR.popSample (0);
        }
    }

    juce::dsp::AudioBlock<float> wetBlock (wetBuffer);
    juce::dsp::ProcessContextReplacing<float> wetContext (wetBlock);
    reverb.process (wetContext);

    if (numCh > 1)
    {
        const float sideGain = juce::jlimit (0.0f, 1.2f, (widthNorm - 0.5f) * 2.4f);
        auto* L = wetBuffer.getWritePointer (0);
        auto* R = wetBuffer.getWritePointer (1);
        for (int i = 0; i < numSamples; ++i)
        {
            const float mid = 0.5f * (L[i] + R[i]);
            float side = 0.5f * (L[i] - R[i]) * sideGain;
            const float maxSide = std::abs (mid) * 0.9f;
            side = juce::jlimit (-maxSide, maxSide, side);
            L[i] = mid + side;
            R[i] = mid - side;
        }
    }

    float peakL = 0.0f;
    float peakR = 0.0f;
    for (int i = 0; i < numSamples; ++i)
    {
        const float mix = mixSmoothed.getNextValue();
        const float dryG = std::cos (mix * juce::MathConstants<float>::halfPi);
        const float wetG = std::sin (mix * juce::MathConstants<float>::halfPi);

        for (int ch = 0; ch < numCh; ++ch)
        {
            const float dry = buffer.getSample (ch, i);
            const float wet = wetBuffer.getSample (ch, i);
            buffer.setSample (ch, i, dry * dryG + wet * wetG);

            if (ch == 0)
                peakL = juce::jmax (peakL, std::abs (wet * wetG));
            else if (ch == 1)
                peakR = juce::jmax (peakR, std::abs (wet * wetG));
        }
    }

    const float decayCoeff = 0.88f;
    wetPeakL.store (juce::jmax (peakL, wetPeakL.load() * decayCoeff));
    wetPeakR.store (juce::jmax (peakR, wetPeakR.load() * decayCoeff));
}

bool MyReverbAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* MyReverbAudioProcessor::createEditor()
{
    return new MyReverbAudioProcessorEditor (*this);
}

void MyReverbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto xml = apvts.copyState().createXml())
        copyXmlToBinary (*xml, destData);
}

void MyReverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        if (xml->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MyReverbAudioProcessor();
}

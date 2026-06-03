#include "PluginEditor.h"
#include "BinaryData.h"

//==============================================================================
MyReverbAudioProcessorEditor::MyReverbAudioProcessorEditor (MyReverbAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    sizeAttachment = std::make_unique<juce::WebSliderParameterAttachment> (
        *audioProcessor.apvts.getParameter ("size"), sizeRelay, nullptr);
    decayAttachment = std::make_unique<juce::WebSliderParameterAttachment> (
        *audioProcessor.apvts.getParameter ("decay"), decayRelay, nullptr);
    mixAttachment = std::make_unique<juce::WebSliderParameterAttachment> (
        *audioProcessor.apvts.getParameter ("mix"), mixRelay, nullptr);
    predelayAttachment = std::make_unique<juce::WebSliderParameterAttachment> (
        *audioProcessor.apvts.getParameter ("predelay"), predelayRelay, nullptr);
    dampingAttachment = std::make_unique<juce::WebSliderParameterAttachment> (
        *audioProcessor.apvts.getParameter ("damping"), dampingRelay, nullptr);
    widthAttachment = std::make_unique<juce::WebSliderParameterAttachment> (
        *audioProcessor.apvts.getParameter ("width"), widthRelay, nullptr);

    webView = std::make_unique<SinglePageBrowser> (createWebOptions (*this));
    addAndMakeVisible (*webView);
    webView->goToURL (juce::WebBrowserComponent::getResourceProviderRoot());

    setSize (580, 600);
    startTimerHz (30);
}

MyReverbAudioProcessorEditor::~MyReverbAudioProcessorEditor()
{
    stopTimer();
}

void MyReverbAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff141210));
}

void MyReverbAudioProcessorEditor::resized()
{
    if (webView != nullptr)
        webView->setBounds (getLocalBounds());
}

void MyReverbAudioProcessorEditor::timerCallback()
{
    if (webView == nullptr || ! webView->isVisible())
        return;

    const float l = audioProcessor.wetPeakL.load();
    const float r = audioProcessor.wetPeakR.load();
    const float levelL = juce::jlimit (0.0f, 1.0f, std::sqrt (l) * 2.5f);
    const float levelR = juce::jlimit (0.0f, 1.0f, std::sqrt (r) * 2.5f);

    const juce::String js = "if (typeof window.updateVuMeters === 'function') { window.updateVuMeters("
                        + juce::String (levelL, 4) + "," + juce::String (levelR, 4) + "); }";
    webView->evaluateJavascript (js);
}

juce::WebBrowserComponent::Options MyReverbAudioProcessorEditor::createWebOptions (MyReverbAudioProcessorEditor& editor)
{
    return juce::WebBrowserComponent::Options{}
        .withBackend (juce::WebBrowserComponent::Options::Backend::webview2)
        .withWinWebView2Options (
            juce::WebBrowserComponent::Options::WinWebView2{}
                .withUserDataFolder (juce::File::getSpecialLocation (juce::File::tempDirectory)
                                        .getChildFile ("NPS_MyReverb")))
        .withNativeIntegrationEnabled()
        .withKeepPageLoadedWhenBrowserIsHidden()
        .withResourceProvider ([&editor] (const juce::String& url) { return editor.getResource (url); })
        .withOptionsFrom (editor.sizeRelay)
        .withOptionsFrom (editor.decayRelay)
        .withOptionsFrom (editor.mixRelay)
        .withOptionsFrom (editor.predelayRelay)
        .withOptionsFrom (editor.dampingRelay)
        .withOptionsFrom (editor.widthRelay);
}

std::optional<juce::WebBrowserComponent::Resource> MyReverbAudioProcessorEditor::getResource (const juce::String& url)
{
    const auto makeResource = [] (const char* data, int size, const char* mime)
    {
        return juce::WebBrowserComponent::Resource {
            std::vector<std::byte> (reinterpret_cast<const std::byte*> (data),
                                    reinterpret_cast<const std::byte*> (data) + size),
            juce::String (mime)
        };
    };

    if (url.isEmpty() || url == "/" || url == "/index.html")
        return makeResource (MyReverb_BinaryData::index_html,
                             MyReverb_BinaryData::index_htmlSize,
                             "text/html");

    return std::nullopt;
}

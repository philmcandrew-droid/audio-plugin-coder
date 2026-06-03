#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include "PluginProcessor.h"

//==============================================================================
class MyReverbAudioProcessorEditor : public juce::AudioProcessorEditor,
                                     public juce::Timer
{
public:
    explicit MyReverbAudioProcessorEditor (MyReverbAudioProcessor&);
    ~MyReverbAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    MyReverbAudioProcessor& audioProcessor;

    // Relays → WebView → Attachments (destruction order)
    juce::WebSliderRelay sizeRelay { "size" };
    juce::WebSliderRelay decayRelay { "decay" };
    juce::WebSliderRelay mixRelay { "mix" };
    juce::WebSliderRelay predelayRelay { "predelay" };
    juce::WebSliderRelay dampingRelay { "damping" };
    juce::WebSliderRelay widthRelay { "width" };

    struct SinglePageBrowser : juce::WebBrowserComponent
    {
        using WebBrowserComponent::WebBrowserComponent;
        bool pageAboutToLoad (const juce::String& newURL) override
        {
            return newURL == getResourceProviderRoot();
        }
    };

    std::unique_ptr<SinglePageBrowser> webView;

    std::unique_ptr<juce::WebSliderParameterAttachment> sizeAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> decayAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> mixAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> predelayAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> dampingAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> widthAttachment;

    std::optional<juce::WebBrowserComponent::Resource> getResource (const juce::String& url);
    static juce::WebBrowserComponent::Options createWebOptions (MyReverbAudioProcessorEditor& editor);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyReverbAudioProcessorEditor)
};

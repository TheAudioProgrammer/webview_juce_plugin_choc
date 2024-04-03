#pragma once

#include "PluginProcessor.h"
#include <juce_gui_extra/juce_gui_extra.h>
#include "gui/choc_WebView.h"
#include "Data.h"

//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    AudioPluginAudioProcessor& processorRef;

    // Platform specific containers
#if JUCE_MAC
    std::unique_ptr<juce::NSViewComponent> webViewContainer;
#elif JUCE_WINDOWS
    std::unique_ptr<juce::HWNDComponent> webViewContainer;
#endif

    // Create choc webview
    std::unique_ptr<choc::ui::WebView> webView;

    // How we can pass the value between the UI and processor in a thread safe way...
    std::unique_ptr<juce::ParameterAttachment> gainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};

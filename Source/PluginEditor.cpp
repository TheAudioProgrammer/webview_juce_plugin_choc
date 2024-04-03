#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    // Create some options to configure the webview
    choc::ui::WebView::Options options;
    options.enableDebugMode = true;
    options.acceptsFirstMouseClick = true;

    // Initialize the webview...
    webView = std::make_unique<choc::ui::WebView>(options);

    // Platform specific container initialization, then set our webview...
#if JUCE_MAC
    webViewContainer = std::make_unique<juce::NSViewComponent>();
    webViewContainer->setView (webView->getViewHandle());
#elif JUCE_WINDOWS
    webViewContainer = std::make_unique<juce::HWNDComponent>();
    webViewContainer->setHWND (webView->getViewHandle());
#endif

    addAndMakeVisible (*webViewContainer);

    // If the user changes the gain value from the DAW, we need to pass the slider to the JS function receiveGainValue
    auto receiveGainValue = [this](float newGainValue)
    {
        std::string jsCode = "receiveGainValue(" + std::to_string(newGainValue) + ");";
        webView->evaluateJavascript (jsCode, nullptr);
    };

    // Associate the GAIN parameter in the processor with the UI component we need to update when GAIN is changed (in JS)
    gainAttachment = std::make_unique<juce::ParameterAttachment>(*processorRef.getParamList().getParameter ("GAIN"), receiveGainValue);

    /* Lambda to parse through the JS JavaScript and pass it to the processor.
    This is a "proof of concept" where the only param is our gain.  A true implementation would identify what param
    was changed, then update accordingly.  I suppose you could alternatively have a separate callback for each param. */
    auto onGainSliderChanged = [this](const choc::value::ValueView& args) ->choc::value::Value
    {
        auto json = juce::JSON::parse (choc::json::toString(args));

        if (json.isArray())
        {
            float gainVal = json[0].toString().getFloatValue();
            gainAttachment->setValueAsCompleteGesture (gainVal);
        }
        else
        {
            return choc::value::Value(-1);
        }

        return choc::value::Value(0);
    };

    /* Here, we are looking for a callback in our JS called gainValueCallback(). This is invoked on the web view's
     * slider change.  Then we call our C++ lambda onGainSliderChanged() to pass on the gain value */
    webView->bind ("gainValueCallback", onGainSliderChanged);

    // Find our html file from our binary data
    const auto htmlData = juce::String::createStringFromData (data::gain_view_html, data::gain_view_htmlSize);

    // If valid, show the HTML
    if (htmlData.isNotEmpty())
        webView->setHTML (htmlData.toStdString());
    else
        jassertfalse;

    setSize (400, 300);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void AudioPluginAudioProcessorEditor::resized()
{
    webViewContainer->setBounds (getLocalBounds());
}

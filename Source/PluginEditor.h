#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class MistyAudioProcessorEditor :
	public juce::AudioProcessorEditor,
	private juce::Slider::Listener
{
public:
    MistyAudioProcessorEditor(MistyAudioProcessor&);
    ~MistyAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
	void sliderValueChanged(juce::Slider* slider) override;

    MistyAudioProcessor& audioProcessor;

    juce::Slider midiVolume;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MistyAudioProcessorEditor)
};

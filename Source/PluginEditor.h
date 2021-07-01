#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#define GREY juce::Colours::grey

class MistyAudioProcessorEditor :
	public juce::AudioProcessorEditor
{
public:
    MistyAudioProcessorEditor(MistyAudioProcessor&);
    ~MistyAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
	int menuBarHeight = 25;

	std::unique_ptr<juce::FileChooser> fileChooser;

	juce::TextButton openButton;
	juce::ShapeButton resetButton {"reset", GREY, GREY.brighter(), GREY};
	juce::ShapeButton playButton  {"play",  GREY, GREY.brighter(), GREY};
	juce::String fileLoaded;
	juce::String statusMessage = "Select a MIDI file to play";

	juce::Path resetButtonShape;
	juce::Path playButtonShape;
	juce::Path pauseButtonShape;

	MistyAudioProcessor& audioProcessor;

	void setFileLoaded(juce::String);

    void openButtonClicked();
	void resetButtonClicked();
	void playButtonClicked();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MistyAudioProcessorEditor)
};

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MidiFileHolder.h"

#define GREY juce::Colours::grey

class MistyAudioProcessorEditor :
	public juce::AudioProcessorEditor
{
public:
    MistyAudioProcessorEditor(MistyAudioProcessor&);
    ~MistyAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    bool keyPressed(const juce::KeyPress&) override;

private:
    friend MidiFileHolder;

    int menuBarHeight = 25;

	std::unique_ptr<juce::FileChooser> fileChooser;
    bool fileLoaded = false;

	juce::TextButton openButton;
    juce::ToggleButton followButton {"Follow"};
    juce::ShapeButton resetButton {"reset", GREY, GREY.brighter(), GREY};
    juce::ShapeButton playButton  {"play",  GREY, GREY.brighter(), GREY};
    juce::String statusMessage = "Select a MIDI file to play";

	juce::Path resetButtonShape;
	juce::Path playButtonShape;
	juce::Path pauseButtonShape;

	MistyAudioProcessor& audioProcessor;
	MidiFileHolder midiFileHolder;

	std::unique_ptr<juce::LookAndFeel_V4> lookAndFeel {new juce::LookAndFeel_V4};

	void setLoadedInterface(bool);

    void openButtonClicked();
    void followButtonClicked();
	void resetButtonClicked();
	void playButtonClicked();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MistyAudioProcessorEditor)
};

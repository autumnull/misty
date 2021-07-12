#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MidiFileHolder.h"

#define GREY juce::Colours::grey

class MistyAudioProcessorEditor :
	public juce::AudioProcessorEditor,
    private juce::Timer
{
public:
    const static int menuBarHeight = 25;

    MistyAudioProcessorEditor(MistyAudioProcessor&);
    ~MistyAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    bool keyPressed(const juce::KeyPress&) override;

private:
    friend MidiFileHolder; // so it can fiddle with the buttons

    bool openingFile = false;

	juce::TextButton openButton {"Open"};
    juce::ToggleButton followButton {"Follow"};
    juce::ShapeButton resetButton {"Reset", GREY, GREY.brighter(), GREY};
    juce::ShapeButton playButton  {"Play",  GREY, GREY.brighter(), GREY};
    juce::String statusMessage = "Select a MIDI file to play";

	juce::Path resetButtonShape;
	juce::Path playButtonShape;
	juce::Path pauseButtonShape;

	MistyAudioProcessor& p;
	MidiFileHolder midiFileHolder;

	std::unique_ptr<juce::FileChooser> fileChooser;

	std::unique_ptr<juce::LookAndFeel_V4> lookAndFeel {new juce::LookAndFeel_V4};

	void timerCallback() override;

	void setLoadedInterface(bool);

    void openButtonClicked();
    void followButtonClicked();
	void resetButtonClicked();
	void playButtonClicked();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MistyAudioProcessorEditor)
};

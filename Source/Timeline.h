#pragma once

#include <JuceHeader.h>

class MidiFileHolder;
class MistyAudioProcessor;

class Timeline  : public juce::Component
{
public:
	const static int barHeight = 32;
    float cursorPosition = 0;

	Timeline(MidiFileHolder&, MistyAudioProcessor&);
    ~Timeline() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent&) override;
    void mouseWheelMove(const juce::MouseEvent&, const juce::MouseWheelDetails&) override;

private:
	MidiFileHolder& midiFileHolder;
	MistyAudioProcessor& p;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Timeline)
};

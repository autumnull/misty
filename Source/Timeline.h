#pragma once

#include <JuceHeader.h>

class MidiFileHolder;

class Timeline  : public juce::Component
{
public:
	float cursorPosition = 0;
	float offset = 0;
	float maxtime = 0;

	Timeline(int, MidiFileHolder&);
    ~Timeline() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent&) override;
    void mouseWheelMove(const juce::MouseEvent&, const juce::MouseWheelDetails&) override;

private:
	int height;
	MidiFileHolder& midiFileHolder;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Timeline)
};

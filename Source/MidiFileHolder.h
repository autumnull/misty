#pragma once

#include <JuceHeader.h>
#include "MidiTrack.h"
#include "TracksHolder.h"
#include "Timeline.h"

class MistyAudioProcessor;

class MidiFileHolder :
	public juce::Component,
	private juce::Timer
{
public:
    MidiFileHolder(MistyAudioProcessor*);
    ~MidiFileHolder() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    juce::String getFilename();
	juce::Result loadMidiFile(juce::File&);

private:
	int timelineHeight = 32;
	Timeline timeline;

	MistyAudioProcessor* audioProcessor;

	juce::MidiFile midiFile;
	juce::String filename = "";

	juce::Viewport tracksViewport;
	TracksHolder* tracksHolder;

	void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiFileHolder)
};

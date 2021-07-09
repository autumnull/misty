#pragma once

#include <JuceHeader.h>
#include "Timeline.h"
#include "TracksViewport.h"
#include "TracksHolder.h"
#include "MidiTrack.h"

class MistyAudioProcessor;
class MistyAudioProcessorEditor;

class MidiFileHolder :
	public juce::Component,
	private juce::Timer
{
public:
    MidiFileHolder(MistyAudioProcessorEditor*, MistyAudioProcessor*);
    ~MidiFileHolder() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTimePosition(float);

    juce::String getFilename();
	juce::Result loadMidiFile(juce::File&);
	void setFollowPlayback(bool);
	void viewportScrolledByUser();

private:
	int timelineHeight = 32;
	Timeline timeline;
	bool followPlayback = false;

	MistyAudioProcessorEditor* editor;
	MistyAudioProcessor* audioProcessor;

	juce::MidiFile midiFile;
	juce::String filename = "";

	TracksViewport tracksViewport;
	TracksHolder* tracksHolder;

	void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiFileHolder)
};

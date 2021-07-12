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
    MidiFileHolder(MistyAudioProcessorEditor&, MistyAudioProcessor&);
    ~MidiFileHolder() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTimePosition(float);

	juce::Result loadMidiFile(juce::File&);
	void viewportScrolledByUser();
	void resetView();
	void limitEditorSize();

    TracksViewport tracksViewport;
private:
    Timeline timeline;

	MistyAudioProcessorEditor& editor;
	MistyAudioProcessor& p;

    TracksHolder* tracksHolder = nullptr;

	void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiFileHolder)
};

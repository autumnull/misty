#pragma once

#include <JuceHeader.h>
#include "MidiTrack.h"
#include "TracksViewport.h"

class MidiFileHolder;
class MistyAudioProcessor;

class TracksHolder : public juce::Component
{
public:
    const static int trackHeight = 200;
    const static int trackSpacing = 3;

    TracksHolder(MidiFileHolder&, MistyAudioProcessor&);

	~TracksHolder() override;
    void paint (juce::Graphics&) override;

    void resized() override;
    void loadTracks(const juce::MidiFile&, int);

    void updateRenderArea(juce::Rectangle<int>);
private:

    MidiFileHolder& midiFileHolder;
	MistyAudioProcessor& p;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TracksHolder)
};

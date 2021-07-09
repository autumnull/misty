#pragma once

#include <JuceHeader.h>
#include "MidiTrack.h"
#include "TracksViewport.h"

class MidiFileHolder;

class TracksHolder : public juce::Component
{
public:
    TracksHolder(TracksViewport&);
	~TracksHolder() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void loadTracks(const juce::MidiFile&, int);
    juce::Rectangle<int> getRenderArea(int);

private:
	int trackHeight = 200;
	int trackSpacing = 3;

	juce::OwnedArray<MidiTrack> tracks;

	TracksViewport& viewport;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TracksHolder)
};

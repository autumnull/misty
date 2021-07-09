#pragma once

#include <JuceHeader.h>

class MidiFileHolder;

class TracksViewport :
    public juce::Viewport
{
public:
    TracksViewport();
    TracksViewport(MidiFileHolder&);

    void scrollBarMoved (juce::ScrollBar*, double newRangeStart) override;

private:
    MidiFileHolder& midiFileHolder;
};

#include <JuceHeader.h>
#include "TracksViewport.h"
#include "MidiFileHolder.h"

TracksViewport::TracksViewport(MidiFileHolder& midiFileHolder) :
    midiFileHolder (midiFileHolder)
{

}

void TracksViewport::scrollBarMoved (juce::ScrollBar* scrollBarThatHasMoved, double newRangeStart)
{
    auto newRangeStartInt = juce::roundToInt (newRangeStart);

    if (scrollBarThatHasMoved == &getHorizontalScrollBar())
    {
        setViewPosition (newRangeStartInt, getViewPositionY());
    }
    else if (scrollBarThatHasMoved == &getVerticalScrollBar())
    {
        setViewPosition (getViewPositionX(), newRangeStartInt);
    }

    if (getHorizontalScrollBar().isMouseButtonDown(true))
        midiFileHolder.viewportScrollbarMoved();
}
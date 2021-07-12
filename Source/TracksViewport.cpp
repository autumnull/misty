#include <JuceHeader.h>
#include "TracksViewport.h"
#include "MidiFileHolder.h"

TracksViewport::TracksViewport(MidiFileHolder& midiFileHolder) :
    midiFileHolder (midiFileHolder)
{
    setInterceptsMouseClicks(false, true);
}

void TracksViewport::scrollBarMoved (juce::ScrollBar* scrollBarThatHasMoved, double newRangeStart)
{
    Viewport::scrollBarMoved(scrollBarThatHasMoved, newRangeStart);

    if (getHorizontalScrollBar().isMouseButtonDown(true))
        midiFileHolder.viewportScrolledByUser();

    ((TracksHolder*)getViewedComponent())->updateRenderArea(getViewArea());
}

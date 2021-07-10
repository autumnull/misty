#include <JuceHeader.h>
#include "Timeline.h"
#include "MidiTrack.h"
#include "MidiFileHolder.h"

Timeline::Timeline(int height, MidiFileHolder& midiFileHolder) :
	height (height),
	midiFileHolder (midiFileHolder)
{
    setMouseCursor(juce::MouseCursor::IBeamCursor);
}

Timeline::~Timeline()
{

}

void Timeline::paint (juce::Graphics& g)
{
	// header
	g.setColour(juce::Colours::black);
	g.fillRect(0, 0, getWidth(), height);

	// ticks
	g.setColour(juce::Colours::whitesmoke);
	for (int t = 0; t < maxtime; t++) {
		int x = MidiTrack::margin + t*MidiTrack::xScale - offset;
		if (t % 5 == 0) {
			g.fillRect(x-1, height-10, 1, 10);
			if (t % 2 == 0) {
				auto m = juce::String(t/60);
				auto s = juce::String(t%60);
				g.drawFittedText(
					m=="0" ? s : m+":"+s.paddedLeft('0', 2),
					x-15, 0, 30, height-10,
					juce::Justification::centred, 1);
			}
		} else {
			g.fillRect(x-1, height-5, 1, 5);
		}
	}

	// cursor
	g.setColour(juce::Colours::black);
	g.drawLine(
		cursorPosition-offset,
		height,
		cursorPosition-offset,
		getHeight());
}

void Timeline::resized()
{

}

void Timeline::mouseDown(const juce::MouseEvent& event)
{
    auto x = event.getMouseDownX();
    auto t = fmax(0, (x + offset - MidiTrack::margin)/MidiTrack::xScale);

    if (t < maxtime)
        midiFileHolder.setTimePosition(t);
}

void Timeline::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel)
{
    auto viewport = &midiFileHolder.tracksViewport;
    viewport->mouseWheelMove(event.getEventRelativeTo(viewport), wheel);
    midiFileHolder.viewportScrolledByUser();
}
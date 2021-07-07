#include <JuceHeader.h>
#include "Timeline.h"
#include "MidiTrack.h"

Timeline::Timeline(int height) :
	height (height)
{

}

Timeline::~Timeline()
{

}

void Timeline::paint (juce::Graphics& g)
{
	// header
	g.setColour(juce::Colours::black);
	g.fillRect((int)MidiTrack::margin, 0, getWidth(), height);

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

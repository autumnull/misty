#include <JuceHeader.h>
#include "Timeline.h"
#include "MidiTrack.h"

Timeline::Timeline(int height) :
	height (height)
{
	setMouseCursor(juce::MouseCursor::IBeamCursor);
	setInterceptsMouseClicks(false, false);
}

Timeline::~Timeline()
{

}

void Timeline::paint (juce::Graphics& g)
{
	// header
	g.setColour(juce::Colours::black);
	g.fillRect(0, 0, getWidth(), height);

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

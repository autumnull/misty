#include <JuceHeader.h>
#include "Timeline.h"
#include "MidiTrack.h"
#include "MidiFileHolder.h"
#include "PluginProcessor.h"

Timeline::Timeline(MidiFileHolder& midiFileHolder, MistyAudioProcessor& p) :
    midiFileHolder (midiFileHolder),
    p (p)
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
	g.fillRect(0, 0, getWidth(), barHeight);

	// ticks
	g.setColour(juce::Colours::whitesmoke);
	auto start = floor((p.viewportPosition.x - 15)/MidiTrack::xScale);
	auto end = ceil((p.viewportPosition.x+15+getWidth())/MidiTrack::xScale);
	for (int t = start; t < end; t++) {
		int x = MidiTrack::margin + t*MidiTrack::xScale - p.viewportPosition.x;
		if (t % 5 == 0) {
			g.fillRect(x-1, barHeight-10, 1, 10);
			if (t % 2 == 0) {
				auto m = juce::String(t/60);
				auto s = juce::String(t%60);
				g.drawFittedText(
					m=="0" ? s : m+":"+s.paddedLeft('0', 2),
					x-15, 0, 30, barHeight-10,
					juce::Justification::centred, 1);
			}
		} else {
			g.fillRect(x-1, barHeight-5, 1, 5);
		}
	}

	// cursor
    float t = p.samplesPlayed/p.currentSampleRate;
    cursorPosition = MidiTrack::margin + t*MidiTrack::xScale;
	g.setColour(juce::Colours::black);
	g.drawLine(
		cursorPosition-p.viewportPosition.x,
		barHeight,
		cursorPosition-p.viewportPosition.x,
		getHeight());
}

void Timeline::resized()
{

}

void Timeline::mouseDown(const juce::MouseEvent& event)
{
    auto x = event.getMouseDownX();
    auto t = fmax(0, (x+p.viewportPosition.x-MidiTrack::margin)/MidiTrack::xScale);

    if (t < p.maxtime)
        midiFileHolder.setTimePosition(t);
}

void Timeline::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel)
{
    auto viewport = &midiFileHolder.tracksViewport;
    auto viewportX = viewport->getViewPositionX();

    viewport->mouseWheelMove(event.getEventRelativeTo(viewport), wheel);

    if (viewportX != viewport->getViewPositionX())
        midiFileHolder.viewportScrolledByUser();
}

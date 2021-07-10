#include <JuceHeader.h>
#include "MidiTrack.h"
#include "TracksHolder.h"

const float MidiTrack::margin = 10.0f;
float MidiTrack::xScale = 24.0f;

MidiTrack::MidiTrack(const juce::MidiMessageSequence& midiTrack, int trackNum, TracksHolder* parent) :
	track (midiTrack),
	trackNum (trackNum),
	parent (parent)
{
	juce::Array<MidiNote*> noteStates;
	noteStates.insertMultiple(0, nullptr, 128);
	for (int jEvent = 0; jEvent<track.getNumEvents(); jEvent++)
	{
		auto message = track.getEventPointer(jEvent)->message;
		int noteNumber = message.getNoteNumber();
		if (message.isNoteOn())
		{
			if (noteStates[noteNumber] != nullptr) {
				noteStates[noteNumber]->x1 = message.getTimeStamp();
			}
			notes.add(new MidiNote(
				message.getTimeStamp(),
				INFINITY,
				noteNumber/127.0f,
				pow(message.getVelocity()/127.0f, 2),
				false));
			noteStates.setUnchecked(noteNumber, notes.getLast());
		}
		else if (message.isNoteOff())
		{
			if (noteStates[noteNumber] != nullptr) {
				noteStates[noteNumber]->x1 = message.getTimeStamp();
				noteStates[noteNumber]->off = true;
				noteStates.setUnchecked(noteNumber, nullptr);
			}
		}
	}
}

MidiTrack::~MidiTrack()
{

}

void MidiTrack::paint (juce::Graphics& g)
{
	g.fillAll(juce::Colours::white);

	float height = getHeight(), width = getWidth();
	auto renderArea = parent->getRenderArea(trackNum);

	if (renderArea.getBottom() < 0 || renderArea.getY() > height)
		return;

	float r = 2.5; // dot radius
	float w = 1.0; // line radius
	float space = 5.0f; // space above and below notes
	float lim = 0.1; // minimum possible alpha;
	for (auto note : notes)
	{
		float x0 = margin + note->x0*xScale;
		float x1 = margin + note->x1*xScale;
		float y = r + space + (height - 2*space) * (1 - note->y);

		if (renderArea.contains(x0, y)
		 || renderArea.contains(x1, y))
		{
			g.setColour(
				juce::Colours::black
				.withBrightness((1-note->v)*(1-lim))
				);
			g.fillEllipse(x0-r, y-r, 2*r, 2*r);
			g.fillRect(x0, y-w, x1-x0, 2*w);
			if (note->off)
				g.fillRect(x1-w, y-r, 2*w, 2*r);
		}
	}

	// start and end lines
	float d = 5.0f; // line separation
	g.setColour(juce::Colours::black);
	g.drawLine(margin, 0, margin, height);
	g.drawLine(width-margin  , 0, width-margin  , height);
	g.drawLine(width-margin+d, 0, width-margin+d, height);
}

void MidiTrack::resized()
{

}

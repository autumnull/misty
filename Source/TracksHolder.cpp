#include "TracksHolder.h"
#include "TracksViewport.h"

TracksHolder::TracksHolder(TracksViewport& viewport)
: viewport (viewport)
{
	setSize(1000,1000);
}

TracksHolder::~TracksHolder()
{

}

void TracksHolder::paint (juce::Graphics& g)
{
	g.fillAll(juce::Colours::dimgrey);
}

void TracksHolder::resized()
{
	for (int jTrack = 0; jTrack < tracks.size(); jTrack++)
		tracks[jTrack]->setBounds(
			0,
			jTrack*(trackHeight+trackSpacing)-trackSpacing,
			getWidth(),
			trackHeight);
}

void TracksHolder::loadTracks(const juce::MidiFile& midiFile, int midiFileType) {
	tracks.clear();
	for (int jTrack = midiFileType; jTrack < midiFile.getNumTracks(); jTrack++)
	{
		auto track = *midiFile.getTrack(jTrack);
		tracks.add(new MidiTrack(track, jTrack-midiFileType, this));
		addAndMakeVisible(tracks.getLast());
	}
	auto newWidth =	midiFile.getLastTimestamp()*MidiTrack::xScale + 2*MidiTrack::margin;
	auto newHeight = (midiFile.getNumTracks()-midiFileType)*trackHeight;
	setSize(newWidth, newHeight);
}

juce::Rectangle<int> TracksHolder::getRenderArea(int trackNum) {
	auto pos = tracks[trackNum]->getBoundsInParent().getTopLeft();
	return viewport.getViewArea().translated(-pos.x, -pos.y).expanded(5,5);
}

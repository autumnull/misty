#include "TracksHolder.h"
#include "TracksViewport.h"
#include "PluginProcessor.h"

TracksHolder::TracksHolder(MidiFileHolder& midiFileHolder, MistyAudioProcessor& p) :
    midiFileHolder (midiFileHolder),
    p (p)
{
    if (p.tracks.isEmpty()) {
        setSize(600,600);
    }
    else {
        for (auto track : p.tracks)
            addAndMakeVisible(track);

        auto newWidth =	p.maxtime*MidiTrack::xScale + 2*MidiTrack::margin;
        auto newHeight = p.tracks.size()*trackHeight;
        setSize(newWidth, newHeight);
    }
}

TracksHolder::~TracksHolder()
{
    for (auto track : p.tracks)
        removeChildComponent(track);
}

void TracksHolder::paint (juce::Graphics& g)
{
	g.fillAll(juce::Colours::dimgrey);
}

void TracksHolder::resized()
{
	for (int jTrack = 0; jTrack < p.tracks.size(); jTrack++)
		p.tracks[jTrack]->setBounds(
			0,
			jTrack*(trackHeight+trackSpacing) - trackSpacing,
			getWidth(),
			trackHeight);
}

void TracksHolder::loadTracks(const juce::MidiFile& midiFile, int midiFileType) {
	p.tracks.clear();
	for (int jTrack = midiFileType; jTrack < midiFile.getNumTracks(); jTrack++)
	{
		auto track = *midiFile.getTrack(jTrack);
		p.tracks.add(new MidiTrack(track, jTrack-midiFileType));
		addAndMakeVisible(p.tracks.getLast());
	}
	auto newWidth =	p.maxtime*MidiTrack::xScale + 2*MidiTrack::margin;
	auto newHeight = p.tracks.size()*trackHeight;
	setSize(newWidth, newHeight);
}

void TracksHolder::updateRenderArea(juce::Rectangle<int> viewArea) {
	for (int jTrack = 0; jTrack < p.tracks.size(); jTrack++) {
        auto pos = p.tracks[jTrack]->getBoundsInParent().getTopLeft();
        p.tracks[jTrack]->renderArea = viewArea.translated(-pos.x, -pos.y).expanded(5,5);
    }
}

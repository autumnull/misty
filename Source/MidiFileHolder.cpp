#include <JuceHeader.h>
#include "MidiFileHolder.h"
#include "PluginProcessor.h"

MidiFileHolder::MidiFileHolder(MistyAudioProcessor* audioProcessor) :
	audioProcessor (audioProcessor),
	timeline(timelineHeight)
{
	tracksViewport.setViewedComponent(new TracksHolder(tracksViewport));
	tracksHolder = (TracksHolder*)tracksViewport.getViewedComponent();
	addAndMakeVisible(tracksViewport);

	addAndMakeVisible(timeline);
}

MidiFileHolder::~MidiFileHolder()
{

}

void MidiFileHolder::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::dimgrey);

	g.setColour(juce::Colours::black);
	g.fillRect(0, 0, getWidth(), timelineHeight);
}

void MidiFileHolder::resized()
{
	auto indent = tracksViewport.getScrollBarThickness();
	auto timelineArea = getLocalBounds();
	timelineArea.removeFromBottom(indent);
	timelineArea.removeFromRight(indent);
	timeline.setBounds(timelineArea);

	auto maxSize = juce::Point<int>(tracksHolder->getWidth(), tracksHolder->getHeight());
	tracksViewport.setBounds(0, timelineHeight, getWidth(), getHeight()-timelineHeight);
}

juce::String MidiFileHolder::getFilename() {
	return filename;
}

juce::Result MidiFileHolder::loadMidiFile(juce::File &file) {

	std::unique_ptr<juce::FileInputStream> fileStream (new juce::FileInputStream(file));

	if (fileStream->openedOk())
	{
		int midiFileType;
		auto result = midiFile.readFrom(*fileStream, false, &midiFileType);

		if (midiFileType == 2)
			return juce::Result::fail("Cannot read format 2 midi files");

		midiFile.convertTimestampTicksToSeconds();
		tracksHolder->loadTracks(midiFile, midiFileType);

		audioProcessor->midiBuffer.clear();
		for (int t = 0; t < midiFile.getNumTracks(); t++)
		{
			const juce::MidiMessageSequence* track = midiFile.getTrack(t);
			for (int i = 0; i < track->getNumEvents(); i++)
			{
				juce::MidiMessage& m = track->getEventPointer(i)->message;
				int sampleOffset = (int)(audioProcessor->currentSampleRate * m.getTimeStamp());
				audioProcessor->midiBuffer.addEvent(m, sampleOffset);
			}
		}
		audioProcessor->samplesPlayed = 0;
		tracksViewport.setViewPosition(0,0);
		startTimerHz(20);
		filename = file.getFileNameWithoutExtension();
		tracksHolder->resized();
	}

	return fileStream->getStatus();
}

void MidiFileHolder::timerCallback()
{
	float t = audioProcessor->samplesPlayed/audioProcessor->currentSampleRate;
	timeline.cursorPosition = t*MidiTrack::xScale + MidiTrack::margin;
	timeline.offset = tracksViewport.getViewPositionX();
	timeline.repaint();
}
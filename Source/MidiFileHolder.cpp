#include <JuceHeader.h>
#include "MidiFileHolder.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"

MidiFileHolder::MidiFileHolder(MistyAudioProcessorEditor& editor,
                               MistyAudioProcessor& p) :
    editor (editor),
    p (p),
    timeline(*this, p),
    tracksViewport(*this)
{
	tracksViewport.setViewedComponent(new TracksHolder(*this, p));
	tracksHolder = (TracksHolder*)tracksViewport.getViewedComponent();
	tracksViewport.setViewPosition(p.viewportPosition);
	addAndMakeVisible(tracksViewport);

	addAndMakeVisible(timeline);

    startTimerHz(20);
}

MidiFileHolder::~MidiFileHolder()
{

}

void MidiFileHolder::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::dimgrey);

	g.setColour(juce::Colours::black);
	g.fillRect(0, 0, getWidth(), timeline.barHeight);
}

void MidiFileHolder::resized()
{
	auto indent = tracksViewport.getScrollBarThickness();
	auto timelineArea = getLocalBounds();
	timelineArea.removeFromBottom(indent);
	timelineArea.removeFromRight(indent);
	timeline.setBounds(timelineArea);

	tracksViewport.setBounds(0, timeline.barHeight, getWidth(), getHeight()-timeline.barHeight);
	tracksHolder->updateRenderArea(tracksViewport.getViewArea());
}

void MidiFileHolder::setTimePosition(float time)
{
    auto samples = time*p.currentSampleRate;

    switch (p.state) {
    case MistyAudioProcessor::Started:
        p.samplesPlayed = samples;
        p.state = MistyAudioProcessor::JumpingStarted;
        break;
    case MistyAudioProcessor::Paused:
    case MistyAudioProcessor::Stopped:
        p.samplesPlayed = samples;
        p.state = MistyAudioProcessor::JumpingPaused;
    default:
        break;
    }
}

juce::Result MidiFileHolder::loadMidiFile(juce::File &file)
{
    if (file.getFileExtension() != ".mid" && file.getFileExtension() != ".MID")
        return juce::Result::fail("Not a MIDI file (.mid)");

	std::unique_ptr<juce::FileInputStream> fileStream (new juce::FileInputStream(file));

	if (fileStream->openedOk())
	{
        int midiFileType;
        auto midiFile = juce::MidiFile();
		auto result = midiFile.readFrom(*fileStream, false, &midiFileType);

		if (! result)
		    return juce::Result::fail("Failed to read MIDI data");

		if (midiFileType == 2)
			return juce::Result::fail("Cannot read format 2 MIDI files");

		midiFile.convertTimestampTicksToSeconds();
        p.maxtime = midiFile.getLastTimestamp();
		tracksHolder->loadTracks(midiFile, midiFileType);
		limitEditorSize();
        tracksViewport.setViewPosition(0,0);
		tracksHolder->updateRenderArea(tracksViewport.getViewArea());

		p.midiBuffer.clear();
		for (int t = 0; t < midiFile.getNumTracks(); t++)
		{
			const juce::MidiMessageSequence* track = midiFile.getTrack(t);
			for (int i = 0; i < track->getNumEvents(); i++)
			{
				juce::MidiMessage& m = track->getEventPointer(i)->message;
				int sampleOffset = (int)(p.currentSampleRate * m.getTimeStamp());
				p.midiBuffer.addEvent(m, sampleOffset);
			}
		}
        p.samplesPlayed = 0;
        p.fileLoaded = file.getFileNameWithoutExtension();
	}

	return fileStream->getStatus();
}

void MidiFileHolder::viewportScrolledByUser()
{
    p.viewportPosition = tracksViewport.getViewPosition();
    editor.followButton.setToggleState(false, juce::sendNotification);
    timeline.offset = tracksViewport.getViewPositionX();
    timeline.repaint();
}

void MidiFileHolder::resetView()
{
    tracksViewport.setViewPosition(0, tracksViewport.getViewPositionY());
    timeline.offset = 0;
    timeline.repaint();
}

void MidiFileHolder::limitEditorSize()
{
    if (tracksHolder != nullptr)
        editor.setResizeLimits(
            600, 265,
            fmin(1400, tracksHolder->getWidth()),
            fmin(800, tracksHolder->getHeight()
                + editor.menuBarHeight
                + timeline.barHeight
                + tracksViewport.getScrollBarThickness())
        );
}

void MidiFileHolder::timerCallback()
{
	float t = p.samplesPlayed/p.currentSampleRate;
	if (t > p.maxtime) {
	    t = p.maxtime;
        p.state = MistyAudioProcessor::Pausing;
        editor.playButtonClicked();
	}
	timeline.cursorPosition = MidiTrack::margin + t*MidiTrack::xScale;
	if (p.following) {
		auto y = tracksViewport.getViewPositionY();
		auto viewWidth = tracksViewport.getViewWidth();
		auto x =
			fmin(tracksHolder->getWidth() - viewWidth,
				fmax(
					0,
                    timeline.cursorPosition - viewWidth/2
				));
		tracksViewport.setViewPosition(x, y);
        timeline.offset = x;
    }
	timeline.repaint();
}

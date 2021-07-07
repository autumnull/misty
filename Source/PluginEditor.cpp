#include "PluginProcessor.h"
#include "PluginEditor.h"

MistyAudioProcessorEditor::MistyAudioProcessorEditor(MistyAudioProcessor& p) :
	AudioProcessorEditor(&p),
	audioProcessor(p),
	midiFileHolder(&p)
{
	setResizable(true, true);
	setResizeLimits(600, 265, 1200, 800);

	openButton.setButtonText("Open");
	openButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentWhite);
	openButton.onClick = [this] { openButtonClicked(); };
	addAndMakeVisible(&openButton);

	resetButtonShape.addRectangle(2, 2, 4, 23);
	resetButtonShape.addTriangle(6, 12.5, 23, 23, 23, 2);

	resetButton.setShape(resetButtonShape, false, true, false);
	resetButton.onClick = [this] { resetButtonClicked(); };
	addChildComponent(resetButton);	// invisible until file loaded

	pauseButtonShape.addRectangle(4, 2, 6, 23);
	pauseButtonShape.addRectangle(15, 2, 6, 23);
	playButtonShape.addTriangle(3, 23, 3, 2, 22, 12.5);

	playButton.setShape(playButtonShape, false, true, false);
	playButton.onClick = [this] { playButtonClicked(); };
	playButton.setClickingTogglesState(true);
	auto light = GREY.brighter(0.5);
	playButton.setOnColours(light, light.brighter(0.5), light);
	playButton.shouldUseOnColours(true);
	addChildComponent(playButton);	// invisible until file loaded

	addChildComponent(midiFileHolder);

    setSize(600, 265);
}

MistyAudioProcessorEditor::~MistyAudioProcessorEditor()
{

}

void MistyAudioProcessorEditor::paint(juce::Graphics& g)
{
	// background
    g.fillAll(juce::Colours::dimgrey);

    // menubar background
    g.setColour(juce::Colour::fromRGBA(0,0,0,80));
    g.fillRect(0, 0, getWidth(), menuBarHeight);

	// file info text
	g.setColour(juce::Colours::lightgrey);
	g.setFont(15.0f);
	if (fileLoaded) {
		auto filenameArea = getLocalBounds()
			.removeFromTop(menuBarHeight)
			.removeFromRight((getWidth()-menuBarHeight)/2);
		g.drawFittedText(
			midiFileHolder.getFilename(),
			filenameArea,
			juce::Justification::centred, 1);
	}
	else {
		g.drawFittedText(
			statusMessage,
			0, menuBarHeight,
			getWidth(), getHeight()-menuBarHeight,
			juce::Justification::centred, 1);
	}
}

void MistyAudioProcessorEditor::resized()
{
	openButton.setBounds(0, 0, 60, menuBarHeight);

	auto playBounds = juce::Rectangle<int>(getWidth()/2-menuBarHeight/2, 0, menuBarHeight, menuBarHeight);
	auto resetBounds = playBounds.translated(-menuBarHeight, 0);
	playBounds.reduce(4, 4); resetBounds.reduce(4, 4);
	resetButton.setBounds(resetBounds);
	playButton.setBounds(playBounds);
	midiFileHolder.setBounds(0, menuBarHeight, getWidth(), getHeight()-menuBarHeight);
}

void MistyAudioProcessorEditor::setLoadedInterface(bool isLoaded)
{
	if (isLoaded) {
		playButton.setVisible(true);
		resetButton.setVisible(true);
		midiFileHolder.setVisible(true);
	}
	else {
		playButton.setVisible(false);
		resetButton.setVisible(false);
		midiFileHolder.setVisible(false);
	}
	fileLoaded = isLoaded;
}

void MistyAudioProcessorEditor::openButtonClicked()
{
	juce::MessageManagerLock mmLock(juce::Thread::getCurrentThread());
	if (!mmLock.lockWasGained())
		return;

	if (playButton.getToggleState()) {
		audioProcessor.state = MistyAudioProcessor::Stopping;
		playButton.setToggleState(false, juce::sendNotification);
	}

	fileChooser = std::make_unique<juce::FileChooser>(
		"Select a MIDI file to play...",
		juce::File::getSpecialLocation(juce::File::userMusicDirectory),
		"*.mid");

	fileChooser->launchAsync(
		juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
		[this](const juce::FileChooser& chooser) {
			juce::File file = chooser.getResult();
			juce::Result status = midiFileHolder.loadMidiFile(file);
			if (status.wasOk())
			{
				statusMessage = "";
				setLoadedInterface(true);
			}
			else {
				statusMessage = "Couldn't open file: " + status.getErrorMessage();
				setLoadedInterface(false);
			}
			repaint();
		}
	);
}

void MistyAudioProcessorEditor::resetButtonClicked()
{
	if (! fileLoaded)
		return;

	switch (audioProcessor.state) {
	case MistyAudioProcessor::Started:
	case MistyAudioProcessor::Paused:
		audioProcessor.state = MistyAudioProcessor::Stopping;
		playButton.setToggleState(false, juce::sendNotification);
	default:
		break;
	}
}

void MistyAudioProcessorEditor::playButtonClicked()
{
	if (! fileLoaded)
		return;

	switch (audioProcessor.state) {
	case MistyAudioProcessor::Stopped:
	case MistyAudioProcessor::Paused:
		audioProcessor.state = MistyAudioProcessor::Starting;
		playButton.setShape(pauseButtonShape, false, true, false);
		break;
	case MistyAudioProcessor::Started:
		audioProcessor.state = MistyAudioProcessor::Pausing;
	case MistyAudioProcessor::Stopping:	// happens when reset button clicked
		playButton.setShape(playButtonShape, false, true, false);
	default:
		break;
	}

	repaint();
}

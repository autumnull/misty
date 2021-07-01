#include "PluginProcessor.h"
#include "PluginEditor.h"

MistyAudioProcessorEditor::MistyAudioProcessorEditor(MistyAudioProcessor& p) :
	AudioProcessorEditor(&p),
	audioProcessor(p)
{
	openButton.setButtonText("Open");
	openButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentWhite);
	openButton.onClick = [this] { openButtonClicked(); };
	addAndMakeVisible(&openButton);

	resetButtonShape.addRectangle(2, 2, 4, 23);
	resetButtonShape.addTriangle(6, 12.5, 23, 23, 23, 2);

	resetButton.setShape(resetButtonShape, false, true, false);
	resetButton.onClick = [this] { resetButtonClicked(); };
	addChildComponent(resetButton);

	pauseButtonShape.addRectangle(4, 2, 6, 23);
	pauseButtonShape.addRectangle(15, 2, 6, 23);
	playButtonShape.addTriangle(3, 23, 3, 2, 22, 12.5);

	playButton.setShape(playButtonShape, false, true, false);
	playButton.onClick = [this] { playButtonClicked(); };
	playButton.setClickingTogglesState(true);
	auto light = GREY.brighter(0.5);
	playButton.setOnColours(light, light.brighter(0.5), light);
	playButton.shouldUseOnColours(true);
	addChildComponent(playButton);

    setSize(600, 150);
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
	if (fileLoaded == "") {
		g.drawFittedText(
			statusMessage,
			0, menuBarHeight,
			getWidth(), getHeight()-menuBarHeight,
			juce::Justification::centred, 1);
	}
	else {
		auto filenameArea = getLocalBounds()
			.removeFromTop(menuBarHeight)
			.removeFromRight((getWidth()-menuBarHeight)/2);
		g.drawFittedText(
			fileLoaded,
			filenameArea,
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
}

void MistyAudioProcessorEditor::setFileLoaded(juce::String filename)
{
	fileLoaded = filename;
	if (filename == "") {
		playButton.setVisible(false);
		resetButton.setVisible(false);
	}
	else {
		playButton.setVisible(true);
		resetButton.setVisible(true);
	}
}

void MistyAudioProcessorEditor::openButtonClicked()
{
	juce::MessageManagerLock mmLock(juce::Thread::getCurrentThread());
	if (!mmLock.lockWasGained())
		return;

	if (playButton.getToggleState())
		playButton.setToggleState(false, juce::sendNotification);

	fileChooser = std::make_unique<juce::FileChooser>(
		"Select a MIDI file to play...",
		juce::File::getSpecialLocation(juce::File::userMusicDirectory),
		"*.mid");

	fileChooser->launchAsync(
		juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
		[this](const juce::FileChooser& chooser) {
			juce::File file = chooser.getResult();
			juce::Result status = audioProcessor.loadMidiFile(file);
			if (status.wasOk())
			{
				statusMessage = "";
				setFileLoaded(file.getFileNameWithoutExtension());
			}
			else {
				statusMessage = "Couldn't open file: " + status.getErrorMessage();
				setFileLoaded("");
			}
			repaint();
		}
	);
}

void MistyAudioProcessorEditor::resetButtonClicked()
{
	if (fileLoaded == "")
		return;

	playButton.setToggleState(false, juce::sendNotification);
}

void MistyAudioProcessorEditor::playButtonClicked()
{
	if (fileLoaded == "")
		return;

	playButton.setShape(
		playButton.getToggleState() ? pauseButtonShape : playButtonShape,
		false, true, false);
	repaint();
}

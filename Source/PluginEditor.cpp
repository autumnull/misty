#include "PluginProcessor.h"
#include "PluginEditor.h"

MistyAudioProcessorEditor::MistyAudioProcessorEditor(MistyAudioProcessor& p) :
    AudioProcessorEditor(&p),
    p(p),
    midiFileHolder(*this, p)
{
    lookAndFeel->setColourScheme(juce::LookAndFeel_V4::getMidnightColourScheme());
    setLookAndFeel(lookAndFeel.get());

	openButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentWhite);
	openButton.onClick = [this] { openButtonClicked(); };
	addAndMakeVisible(&openButton);

	followButton.onClick = [this] { followButtonClicked(); };
	followButton.setToggleState(p.following, juce::dontSendNotification);
	addChildComponent(followButton);

	resetButtonShape.addRectangle(2, 2, 4, 23);
	resetButtonShape.addTriangle(6, 12.5, 23, 23, 23, 2);

	resetButton.setShape(resetButtonShape, false, true, false);
	resetButton.onClick = [this] { resetButtonClicked(); };
	addChildComponent(resetButton);

	pauseButtonShape.addRectangle(4, 2, 6, 23);
	pauseButtonShape.addRectangle(15, 2, 6, 23);
	playButtonShape.addTriangle(3, 23, 3, 2, 22, 12.5);

	auto playing = p.state == MistyAudioProcessor::Started;
	playButton.setShape(playing ? pauseButtonShape : playButtonShape, false, true, false);
	playButton.setToggleState(playing, juce::dontSendNotification);
	playButton.onClick = [this] { playButtonClicked(); };
	playButton.setClickingTogglesState(true);
	auto light = GREY.brighter(0.8);
	playButton.setOnColours(light, light.brighter(0.5), light);
	playButton.shouldUseOnColours(true);
	addChildComponent(playButton);

	addChildComponent(midiFileHolder);

	if (p.fileLoaded != "")
        setLoadedInterface(true);

    setResizable(true, true);
    if (p.tracks.isEmpty())
        setResizeLimits(600, 265, 1400, 800);
    else
        midiFileHolder.limitEditorSize();
    setSize(600, 265);
    startTimer(100);
}

MistyAudioProcessorEditor::~MistyAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
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
	if (p.fileLoaded != "") {
		auto filenameArea = getLocalBounds()
			.removeFromTop(menuBarHeight)
			.removeFromRight((getWidth()-menuBarHeight)/2);
		g.drawFittedText(
			p.fileLoaded,
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
	followButton.setBounds(60, 0, 70, menuBarHeight);

	auto playBounds = juce::Rectangle<int>(getWidth()/2-menuBarHeight/2, 0, menuBarHeight, menuBarHeight);
	auto resetBounds = playBounds.translated(-menuBarHeight, 0);
	playBounds.reduce(4, 4); resetBounds.reduce(4, 4);
	resetButton.setBounds(resetBounds);
	playButton.setBounds(playBounds);
	midiFileHolder.setBounds(0, menuBarHeight, getWidth(), getHeight()-menuBarHeight);
}

bool MistyAudioProcessorEditor::keyPressed(const juce::KeyPress& key)
{
    if (key == juce::KeyPress::backspaceKey) {
        resetButton.triggerClick();
        return true;
    }

    switch (key.getTextCharacter()){
    case ' ':
        playButton.triggerClick();
        return true;
    case 'f':
        followButton.triggerClick();
        return true;
    case 'o':
        openButton.triggerClick();
        return true;
    default:
        return false;
    }
}

void MistyAudioProcessorEditor::timerCallback()
{
    if (isShowing() || isOnDesktop())
    {
        grabKeyboardFocus();
        stopTimer();
    }
}

void MistyAudioProcessorEditor::setLoadedInterface(bool isLoaded)
{
    followButton.setVisible(isLoaded);
    playButton.setVisible(isLoaded);
    resetButton.setVisible(isLoaded);
    midiFileHolder.setVisible(isLoaded);
}

void MistyAudioProcessorEditor::openButtonClicked()
{
	juce::MessageManagerLock mmLock(juce::Thread::getCurrentThread());
	if (!mmLock.lockWasGained())
		return;

	fileChooser = std::unique_ptr<juce::FileChooser>(new juce::FileChooser(
		"Select a MIDI file to play...",
		juce::File::getSpecialLocation(juce::File::userMusicDirectory),
		"*.mid;*.MID"));

	int flags = juce::FileBrowserComponent::openMode
	          | juce::FileBrowserComponent::canSelectFiles
	          | juce::FileBrowserComponent::filenameBoxIsReadOnly
	          | juce::FileBrowserComponent::doNotClearFileNameOnRootChange;
	fileChooser->launchAsync(
	    flags,
	    [this](const juce::FileChooser& chooser) {
            auto file = chooser.getResult();

            if (file.getFileName() != "")
            {
                if (playButton.getToggleState()) {
                    p.state = MistyAudioProcessor::Stopping;
                    playButton.setToggleState(false, juce::dontSendNotification);
                    playButtonClicked();
                }

                juce::Result status = midiFileHolder.loadMidiFile(file);
                if (status.wasOk()) {
                    statusMessage = "";
                    setLoadedInterface(true);
                } else {
                    statusMessage = "Couldn't open file: " + status.getErrorMessage();
                    setLoadedInterface(false);
                    p.fileLoaded = "";
                }
            }
            else {
                statusMessage = "No MIDI file selected";
            }
            grabKeyboardFocus();
            repaint();
	    });
}

void MistyAudioProcessorEditor::followButtonClicked()
{
	p.following = followButton.getToggleState();
}

void MistyAudioProcessorEditor::resetButtonClicked()
{
	switch (p.state)
	{
	case MistyAudioProcessor::Started:
	case MistyAudioProcessor::Paused:
        p.state = MistyAudioProcessor::Stopping;
		playButton.setToggleState(false, juce::sendNotification);
	default:
        midiFileHolder.resetView();
		break;
	}
}

void MistyAudioProcessorEditor::playButtonClicked()
{
	switch (p.state)
	{
	case MistyAudioProcessor::Stopped:
	case MistyAudioProcessor::Paused:
        p.state = MistyAudioProcessor::Starting;
		playButton.setShape(pauseButtonShape, false, true, false);
		break;
	case MistyAudioProcessor::Started:
        p.state = MistyAudioProcessor::Pausing;
	case MistyAudioProcessor::Stopping:	// happens when reset button clicked
		playButton.setShape(playButtonShape, false, true, false);
		playButton.setVisible(true);
		break;
	case MistyAudioProcessor::Pausing: // happens when end of file reached
	    playButton.setVisible(false);
	    playButton.setShape(playButtonShape, false, true, false);
	default:
		break;
	}

	repaint();
}

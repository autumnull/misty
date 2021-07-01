#include "PluginProcessor.h"
#include "PluginEditor.h"

MistyAudioProcessorEditor::MistyAudioProcessorEditor(MistyAudioProcessor& p) :
	AudioProcessorEditor(&p),
	audioProcessor(p)
{
	midiVolume.setSliderStyle(juce::Slider::LinearBarVertical);
	midiVolume.setRange(0.0, 127.0, 1.0);
	midiVolume.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
	midiVolume.setPopupDisplayEnabled(true, false, this);
	midiVolume.setTextValueSuffix(" Volume");
	midiVolume.setValue(1.0);
	addAndMakeVisible(&midiVolume);
	midiVolume.addListener(this);

    setSize(200, 200);
}

MistyAudioProcessorEditor::~MistyAudioProcessorEditor()
{

}

void MistyAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::white);

    g.setColour(juce::Colours::black);
    g.setFont(15.0f);
    g.drawFittedText("Midi Volume", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
}

void MistyAudioProcessorEditor::resized()
{
	midiVolume.setBounds(40, 30, 20, getHeight()-60);
}

void MistyAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
	audioProcessor.noteOnVel = midiVolume.getValue();
}
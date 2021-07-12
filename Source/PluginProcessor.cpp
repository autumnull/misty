#include "PluginProcessor.h"
#include "PluginEditor.h"

MistyAudioProcessor::MistyAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor (BusesProperties()
					#if ! JucePlugin_IsMidiEffect
					 #if ! JucePlugin_IsSynth
					  .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
					 #endif
					  .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
					#endif
						)
#endif
{

}

MistyAudioProcessor::~MistyAudioProcessor()
{

}

const juce::String MistyAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MistyAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MistyAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MistyAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MistyAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MistyAudioProcessor::getNumPrograms()
{
    return 1;
}

int MistyAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MistyAudioProcessor::setCurrentProgram(int index)
{

}

const juce::String MistyAudioProcessor::getProgramName(int index)
{
    return {};
}

void MistyAudioProcessor::changeProgramName(int index, const juce::String& newName)
{

}

void MistyAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	currentSampleRate = sampleRate;
	currentSamplesPerBlock = samplesPerBlock;
}

void MistyAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MistyAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MistyAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	buffer.clear(); // clear audio channels just in case ?

	juce::MidiBuffer processedMidi;

	int sampleDeltaToAdd = -samplesPlayed;

	switch (state)
	{
	case Started:
		processedMidi.addEvents(midiBuffer, samplesPlayed, currentSamplesPerBlock, sampleDeltaToAdd);
		samplesPlayed += currentSamplesPerBlock;
		break;
	case Stopping:
		samplesPlayed = 0;
	case Starting:
	case Pausing:
        sendAllNotesOff(&processedMidi);
		state = (TransportState)(state+1);
		break;
	case JumpingStarted:
        sendAllNotesOff(&processedMidi);
        state = Started;
        break;
	case JumpingPaused:
        sendAllNotesOff(&processedMidi);
	    state = Paused;
	default:
		break;
	}

	midiMessages.swapWith(processedMidi);
}

bool MistyAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* MistyAudioProcessor::createEditor()
{
    return new MistyAudioProcessorEditor(*this);
}

void MistyAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MistyAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void MistyAudioProcessor::sendAllNotesOff(juce::MidiBuffer* buffer)
{
    for (int c = 0; c < 0x10; c++) {
        auto message = juce::MidiMessage(0xB0+c, 0x7b, 0x40);
        buffer->addEvent(message, c);
    }
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MistyAudioProcessor();
}

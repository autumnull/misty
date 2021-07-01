#include "PluginProcessor.h"
#include "PluginEditor.h"

MistyAudioProcessor::MistyAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor(BusesProperties())
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
    return false;
}

bool MistyAudioProcessor::producesMidi() const
{
    return true;
}

bool MistyAudioProcessor::isMidiEffect() const
{
    return true;
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

}

void MistyAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MistyAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    juce::ignoreUnused (layouts);
    return true;
}
#endif

void MistyAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	buffer.clear();

	juce::MidiBuffer processedMidi;

	for (const auto metadata : midiMessages)
	{
		auto message = metadata.getMessage();
		const auto time = metadata.samplePosition;

		if (message.isNoteOn())
		{
			message = juce::MidiMessage::noteOn (message.getChannel(),
				message.getNoteNumber(),
				(juce::uint8) noteOnVel);
		}

		processedMidi.addEvent (message, time);
	}

	midiMessages.swapWith (processedMidi);
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

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MistyAudioProcessor();
}

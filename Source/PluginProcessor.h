#pragma once

#include <JuceHeader.h>
#include "MidiFileHolder.h"
#include "MidiTrack.h"

class MistyAudioProcessor :
	public juce::AudioProcessor
{
public:
	enum TransportState
	{
		Starting,
		Started,
		Pausing,
        Paused,
        Stopping,
        Stopped,
        JumpingStarted,
        JumpingPaused
    };

	MistyAudioProcessor();
    ~MistyAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

	double currentSampleRate = 44100; // set an initial value to stop divide by zero
    int currentSamplesPerBlock;
    int samplesPlayed = 0;
    TransportState state = Stopped;

    juce::MidiBuffer midiBuffer;

	// store all these here so they're preserved when the editor is closed.
    juce::String fileLoaded = "";
    float maxtime = 0;
    bool following = false;
    juce::Point<int> viewportPosition {0, 0};
    juce::OwnedArray<MidiTrack> tracks;

private:
    void sendAllNotesOff(juce::MidiBuffer*);

	MidiFileHolder* midiFileHolder;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MistyAudioProcessor)
};

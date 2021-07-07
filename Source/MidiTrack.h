#pragma once

#include <JuceHeader.h>

// forward declaration
class TracksHolder;

class MidiTrack  : public juce::Component
{
public:
	const static float margin; // margin around notes
	static float xScale; // scale factor for pixels/seconds
	MidiTrack(const juce::MidiMessageSequence&, int, TracksHolder*);

    ~MidiTrack() override;
	void paint (juce::Graphics&) override;

    void resized() override;
private:

	struct MidiNote {
		MidiNote(float x0, float x1, float y, float v, bool off)
			: x0(x0), x1(x1), y(y), v(v), off(off)
		{}
		float x0;
		float x1;
		float y;
		float v;
		bool off; // whether or not the note has a noteoff at the end
	};

	int trackNum;
	const juce::MidiMessageSequence& track;
	juce::OwnedArray<MidiNote> notes;

	TracksHolder* parent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiTrack)
};

#pragma once

#include <JuceHeader.h>


class Timeline  : public juce::Component
{
public:
	float cursorPosition = 0;
	float offset = 0;

	Timeline(int);
    ~Timeline() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
	int height;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Timeline)
};

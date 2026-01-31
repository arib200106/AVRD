#pragma once
#include <JuceHeader.h>
#include "Colors.h"

class TransportPanel : public juce::Component
{
public:
    TransportPanel()
    {
        // Let's add a dummy "Play" button to see the style
        addAndMakeVisible(playButton);
        playButton.setButtonText("PLAY");
        
        addAndMakeVisible(stopButton);
        stopButton.setButtonText("STOP");
    }

    void paint(juce::Graphics& g) override
    {
        // Give the header a slightly lighter background than the main window
        g.fillAll(AVRDColors::panel);
        
        // Add a subtle bottom border to separate it from the timeline
        g.setColour(AVRDColors::accent);
        g.drawHorizontalLine(getHeight() - 1, 0.0f, (float)getWidth());
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(10);
        auto buttonWidth = 80;
        
        // Position buttons on the left
        playButton.setBounds(area.removeFromLeft(buttonWidth).reduced(2));
        stopButton.setBounds(area.removeFromLeft(buttonWidth).reduced(2));
    }

private:
    juce::TextButton playButton, stopButton;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TransportPanel)
};
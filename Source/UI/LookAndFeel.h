#pragma once
#include <JuceHeader.h>
#include "Colors.h"

class AVRDLookAndFeel : public juce::LookAndFeel_V4
{
public:
    AVRDLookAndFeel()
    {
        // 1. Set the window colors
        setColour(juce::ResizableWindow::backgroundColourId, AVRDColors::background);
        
        // 2. Style the Buttons globally
        setColour(juce::TextButton::buttonColourId, AVRDColors::widget);
        setColour(juce::TextButton::buttonOnColourId, AVRDColors::accent);
        setColour(juce::TextButton::textColourOffId, AVRDColors::text);
        setColour(juce::TextButton::textColourOnId, juce::Colours::white);
        
        // 3. Style the Labels (Text)
        setColour(juce::Label::textColourId, AVRDColors::text);
    }

    // You can override specific drawing methods here later (e.g., drawButtonBackground)
    // for a more "custom" industrial look.
};
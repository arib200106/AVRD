#pragma once
#include <JuceHeader.h>

namespace AVRDColors
{
    // Professional DAW Palette (Charcoal & Slate)
    const juce::Colour background     = juce::Colour(0xff121212); // Main Workspace
    const juce::Colour panel          = juce::Colour(0xff1e1e1e); // Sidebars / Bottom Panel
    const juce::Colour widget         = juce::Colour(0xff2d2d2d); // Buttons / Sliders background
    
    const juce::Colour accent         = juce::Colour(0xff4a90e2); // Electric Blue (Playheads/Selections)
    const juce::Colour text           = juce::Colour(0xffcccccc); // Soft White / Light Grey text
    const juce::Colour textDimmed     = juce::Colour(0xff777777); // Disabled/Low-priority text
    
    const juce::Colour trackRed       = juce::Colour(0xffcf6679); // Recording / Error
    const juce::Colour trackGreen     = juce::Colour(0xff03dac6); // Success / Active MIDI
}
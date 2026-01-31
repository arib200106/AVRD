#pragma once
#include <JuceHeader.h>
#include "Colors.h"

class BrowserPanel : public juce::Component
{
public:
    BrowserPanel() {}

    void paint(juce::Graphics& g) override
    {
        auto area = getLocalBounds().toFloat();
        g.setColour(juce::Colour(0xFFF5F5F7));
        g.fillRect(area);
        g.setColour(juce::Colours::black.withAlpha(0.1f));
        g.drawRect(area, 1.0f);
        auto headerArea = area.removeFromTop(40.0f).reduced(10.0f, 0.0f);
        g.setColour(juce::Colour(0xFF1A1A1A));
        g.setFont(juce::Font(14.0f, juce::Font::bold));
        g.drawText("BROWSER", headerArea, juce::Justification::centredLeft);
    }
};
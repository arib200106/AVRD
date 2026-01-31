#pragma once
#include <JuceHeader.h>

class TrackLane : public juce::Component
{
public:
    TrackLane(int trackIndex) : index(trackIndex) {}

    // We add a way to tell the lane what the zoom is
    void setZoom(float newZoomX) { 
        zoomX = newZoomX; 
        repaint(); 
    }

    void paint(juce::Graphics& g) override
    {
        auto area = getLocalBounds().toFloat();
        float headerWidth = 140.0f;

        // 1. Background
        auto basePink = juce::Colour(0xFFFF1493);
        g.setColour(index % 2 == 0 ? basePink.darker(0.1f) : basePink.darker(0.05f));
        g.fillRect(area);

        // 2. THE GRID (PASTE THIS HERE)
        // We multiply the 40px beat by zoomX so the grid stretches with the lane
        g.setColour(juce::Colours::black.withAlpha(0.1f));
        for (float x = headerWidth; x < getWidth(); x += (40.0f * zoomX)) 
        {
            g.drawVerticalLine((int)x, 0.0f, (float)getHeight());
        }

        // 3. Header Section (Left)
        auto headerArea = area.removeFromLeft(headerWidth);
        g.setColour(juce::Colours::black.withAlpha(0.2f));
        g.fillRect(headerArea);

        // 4. Label
        g.setColour(juce::Colours::white);
        g.setFont(juce::Font(14.0f, juce::Font::bold));
        g.drawText("TRACK " + juce::String(index + 1), headerArea.reduced(10, 0), juce::Justification::centredLeft);

        // 5. Divider
        g.setColour(juce::Colours::black.withAlpha(0.2f));
        g.drawHorizontalLine(getHeight() - 1, 0.0f, (float)getWidth());
    }

private:
    int index;
    float zoomX = 1.0f; // Default zoom
};
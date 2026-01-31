#pragma once
#include <JuceHeader.h>

class TimelineHeader : public juce::Component
{
public:
    void setZoom(float newZoomX) { zoomX = newZoomX; repaint(); }
    
    // This allows the PlaylistPanel to tell the header how far we've scrolled
    void setScrollOffset(int newOffset) { scrollOffset = newOffset; repaint(); }

    void paint(juce::Graphics& g) override{
        auto area = getLocalBounds();
        float headerWidth = 140.0f;
        g.setColour(juce::Colour(0xFF1A1A1A));
        g.fillRect(area);
        float barWidth = 160.0f * zoomX;
        for (int bar = 1; bar <= 200; ++bar){
            float xPos = headerWidth + ((bar - 1) * barWidth) - scrollOffset;
            if (xPos > getWidth()) break;
            if (xPos < headerWidth - barWidth) continue;
            if (xPos >= headerWidth - 5){
                g.setColour(juce::Colours::white.withAlpha(0.8f));
                g.setFont(juce::Font(13.0f, juce::Font::bold));
                g.drawText(juce::String(bar), (int)xPos + 5, 0, 40, getHeight(), juce::Justification::centredLeft);
                g.setColour(juce::Colours::white.withAlpha(0.3f));
                g.drawVerticalLine((int)xPos, 0.0f, (float)getHeight());
                g.setGradientFill(juce::ColourGradient(juce::Colours::black.withAlpha(0.3f), 0, (float)getHeight(),juce::Colours::transparentBlack, 0, (float)getHeight() - 10, false));
                g.fillRect(area);
            }
        }
    }

private:
    float zoomX = 1.0f;
    int scrollOffset = 0; // New variable!
};
#pragma once
#include <JuceHeader.h>
#include "Colors.h"
#include "TransportPanel.h"
#include "BrowserPanel.h"
#include "PlaylistPanel.h"

class MainComponent : public juce::Component
{
public:
    MainComponent()
    {
        addAndMakeVisible(transportPanel);
        addAndMakeVisible(browserPanel);
        addAndMakeVisible(playlistPanel);
        setSize(1200, 800);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(AVRDColors::background);
        g.setColour(AVRDColors::accent);
        g.drawRect(getLocalBounds(), 2);
    }

    void resized() override {
        auto area = getLocalBounds();
        area.reduce(5, 5);
        transportPanel.setBounds(area.removeFromTop(60));
        area.removeFromTop(5);
        browserPanel.setBounds(area.removeFromLeft(250));
        area.removeFromLeft(5);
        playlistPanel.setBounds(area);
    }

private:
    TransportPanel transportPanel;
    PlaylistPanel playlistPanel;
    BrowserPanel browserPanel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
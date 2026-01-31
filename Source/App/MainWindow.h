#pragma once
#include <JuceHeader.h>
#include "MainComponent.h"

class MainWindow : public juce::DocumentWindow
{
public:
    MainWindow(
        juce::String name,
        tracktion_engine::Engine& engine,
        tracktion_engine::Edit& edit
    )
        : DocumentWindow(
            name,
            juce::Colours::black,
            juce::DocumentWindow::allButtons
        )
    {
        setUsingNativeTitleBar(true);
        setResizable(true, true);

        setContentOwned(new MainComponent(engine, edit), true);

        centreWithSize(1200, 800);
        setVisible(true);
    }

    void closeButtonPressed() override
    {
        juce::JUCEApplication::getInstance()->systemRequestedQuit();
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};

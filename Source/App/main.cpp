#include <JuceHeader.h>
#include "MainWindow.h"
#include "LookAndFeel.h"

class AVRDApplication : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override    { return "AVRD"; }
    const juce::String getApplicationVersion() override { return "0.0.1"; }
    bool moreThanOneInstanceAllowed() override          { return true; }

    void initialise(const juce::String&) override
    {
        juce::LookAndFeel::setDefaultLookAndFeel(&avrStyle);
        mainWindow = std::make_unique<MainWindow>(getApplicationName());

    }

    void shutdown() override
    {
        juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
        mainWindow = nullptr;
    }

    void systemRequestedQuit() override { quit(); }
    void anotherInstanceStarted(const juce::String&) override {}

private:
    AVRDLookAndFeel avrStyle;
    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(AVRDApplication)
#include <JuceHeader.h>
#include <tracktion_engine/tracktion_engine.h>
#include "MainWindow.h"

namespace te = tracktion_engine;

class AVRDApplication : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override    { return "AVRD"; }
    const juce::String getApplicationVersion() override { return "1.0.0"; }
    bool moreThanOneInstanceAllowed() override          { return true; }

    void initialise (const juce::String&) override
    {
        // Create engine
        engine = std::make_unique<te::Engine>(getApplicationName());

        // Get device manager and set up
        auto& deviceManager = engine->getDeviceManager().deviceManager;
        
        // Initialize JUCE AudioDeviceManager directly
        juce::String audioError;
        auto setup = deviceManager.getAudioDeviceSetup();
        setup.sampleRate = 44100.0;
        setup.bufferSize = 512;
        
        deviceManager.initialise(0, 2, nullptr, true, juce::String(), &setup);
        
        if (audioError.isNotEmpty())
        {
            DBG("Audio error: " + audioError);
        }

        // Create empty edit - SIMPLIFIED
        juce::File editFile; // Empty file for new project
        edit = te::createEmptyEdit(*engine, editFile);

        for (int i = 0; i < 4; ++i){
            edit->ensureNumberOfAudioTracks(i + 1);
        }
        
        // Create main window
        mainWindow = std::make_unique<MainWindow>(
            getApplicationName(),
            *engine,
            *edit
        );
    }

    void shutdown() override
    {
        mainWindow.reset();
        edit.reset();
        engine.reset();
    }

    void systemRequestedQuit() override { quit(); }

private:
    std::unique_ptr<te::Engine> engine;
    std::unique_ptr<te::Edit> edit;
    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION (AVRDApplication)
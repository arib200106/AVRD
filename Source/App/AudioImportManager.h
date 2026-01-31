#pragma once
#include <JuceHeader.h>
#include <tracktion_engine/tracktion_engine.h>


namespace te = tracktion_engine;

class AudioImportManager
{
public:
    AudioImportManager(te::Edit& ed, te::Engine& eng)
        : edit(ed), engine(eng)
    {
    }

    void importAudioFile()
    {
        chooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File::getSpecialLocation(juce::File::userHomeDirectory),
            "*.wav;*.mp3;*.aif"
        );

        auto flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

        chooser->launchAsync(flags, [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file.existsAsFile())
                importFileToTrack(file);
        });
    }

    void importFileToTrack(const juce::File& audioFile, double startTimeInSeconds = 0.0)
    {
        auto audioTracks = te::getAudioTracks(edit);
        
        if (audioTracks.size() > 0)
        {
            auto* track = audioTracks[0];

            // 1. Clear existing clips
            auto clips = track->getClips();
            for (int i = clips.size(); --i >= 0;)
                clips[i]->removeFromParent();

            // 2. Use the "tracktion" namespace for time types (not te::)
            // Note: 'te' is an alias for 'tracktion_engine', but these live in 'tracktion'
            auto startPos = tracktion::TimePosition::fromSeconds(startTimeInSeconds);
            auto duration = tracktion::TimeDuration::fromSeconds(10.0);
            
            // 3. Modern ClipPosition uses 'time' instead of 'timeRange'
            te::ClipPosition position;
            position.time = { startPos, startPos + duration }; // This is a tracktion::TimeRange
            position.offset = tracktion::TimeDuration::fromSeconds(0.0);

            // 4. Native insertion
            track->insertWaveClip(audioFile.getFileNameWithoutExtension(), 
                                audioFile, 
                                position, 
                                false);

            lastImportedFile = audioFile;

            if (onTrackAdded)
                onTrackAdded();
        }
    }

    std::function<void()> onTrackAdded;
    juce::File getLastImportedFile() const { return lastImportedFile; }

private:
    te::Edit& edit;
    te::Engine& engine;
    
    juce::File lastImportedFile;
    std::unique_ptr<juce::FileChooser> chooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioImportManager)
};
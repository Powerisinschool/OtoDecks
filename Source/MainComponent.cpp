#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize(1000, 750);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired(RuntimePermissions::recordAudio)
        && !RuntimePermissions::isGranted(RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request(RuntimePermissions::recordAudio,
                                    [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(0, 2);
    }

    // juce_setAudioEngineInstance(this);
    // juce_setupRemoteCommandCenter();

    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // player1.prepareToPlay (samplesPerBlockExpected, sampleRate);
    // player2.prepareToPlay (samplesPerBlockExpected, sampleRate);
    mixerSource.prepareToPlay (samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    // juce_updateNowPlayingInfo();
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock (bufferToFill);
}

// void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
// {
//     auto *leftChan = bufferToFill.buffer->getWritePointer(0,
//                                                                 bufferToFill.startSample);
//     auto *rightChan = bufferToFill.buffer->getWritePointer(0,
//                                                                 bufferToFill.startSample);
//     for (auto i = 0; i < bufferToFill.numSamples; ++i)
//     {
//         // const float sample = rand.nextFloat() * 0.04;
//         // const double sample = fmod(phase, 0.2);
//         const double sample = sin(phase) * 0.1;
//         leftChan[i] = sample;
//         rightChan[i] = sample;
//
//         phase += dphase;
//     }
//     // bufferToFill.clearActiveBufferRegion();
// }

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
    // cancelToken.store (false, std::memory_order_relaxed);
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    deckGUI1.setBounds (getLocalBounds().withWidth(getWidth()/2));
    deckGUI2.setBounds (getLocalBounds().withWidth(getWidth()/2).withX(getWidth()/2));
}

// void MainComponent::play()
// {
//     player1.start();
// }
//
// void MainComponent::pause()
// {
//     player1.stop();
//     player2.stop();
// }

// void MainComponent::togglePlayPause()
// {
//     player1.togglePlayPause();
//     player2.togglePlayPause();
// }

// void MainComponent::seekTo(const double timeInSeconds)
// {
//     player1.setPosition(timeInSeconds);
//     player2.setPosition(timeInSeconds);
// }

// double MainComponent::getTrackDuration()
// {
//     if (player1.isPlaying())
//         return player1.getTrackDuration();
//     return player2.getTrackDuration();
// }

// double MainComponent::getPlaybackTime()
// {
//     if (player1.isPlaying())
//         return player1.getPosition();
//     return player2.getPosition();
// }
//
// bool MainComponent::isPlaying()
// {
//     return player1.isPlaying() || player2.isPlaying();
// }

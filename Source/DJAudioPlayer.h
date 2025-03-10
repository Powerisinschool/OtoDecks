/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 27 Feb 2025 9:18:01pm
    Author:  Tolulope Olagunju

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "IntervalThread.h"

using namespace juce;

class DJAudioPlayer : public AudioSource {
public:

  explicit DJAudioPlayer(std::string name, AudioFormatManager &_formatManager);
  ~DJAudioPlayer() override;

  //==============================================================================
  void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
  void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
  void releaseResources() override;

  void loadURL(const URL& audioURL);
  // void loadTags(const char *filepath);
  void setGain(double gain);
  void setSpeed(double ratio);
  void setPosition(double position);
  void setPositionRelative(double position);
  void skipTrackBySeconds(int seconds);

  double getGain() const;
  double getSpeed() const;
  // Return the current position in seconds
  double getPosition() const;
  // Return the current position scaled from 0 to 1
  double getPositionRelative() const;
  // Return the total length of the track
  double getTrackDuration() const;
  // const char* getTrackTitle() const;
  const char *getActiveDeckName() const;
  const char *getFilePath() const;

  void start();
  void stop();
  void togglePlayPause();

  bool isPlaying() const;

  bool isTrackFinished() const;

private:
  std::string deckName;
  AudioFormatManager& formatManager;
  std::unique_ptr<AudioFormatReaderSource> readerSource;
  AudioTransportSource transportSource;
  ResamplingAudioSource resampleSource{&transportSource, false, 2};

  // std::atomic_bool cancelToken{false};
  // IntervalThread macUpdateUIThread;

  // std::string filePath;

  // std::string trackTitle{"Loading..."};
};

extern "C"
{
  void juce_setAudioEngineInstance(DJAudioPlayer* engine);
  void juce_updateNowPlayingInfo();
  // void juce_updateNowPlayingInfo(const char* title, const char* artist, double duration, double elapsedTime, float playbackRate);
  void juce_setupRemoteCommandCenter();
}

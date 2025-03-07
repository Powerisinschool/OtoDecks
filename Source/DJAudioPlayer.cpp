/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 27 Feb 2025 9:18:01pm
    Author:  Tolulope Olagunju

  ==============================================================================
*/

#include "DJAudioPlayer.h"

#include <utility>

DJAudioPlayer::DJAudioPlayer(std::string name) : deckName(std::move(name)), macUpdateUIThread(&cancelToken, [this]()
{
  // if (isPlaying())
  //   juce_updateNowPlayingInfo();
}) {}
DJAudioPlayer::~DJAudioPlayer()
{
  cancelToken.store (false, std::memory_order_relaxed);
};

//==============================================================================
void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
  formatManager.registerBasicFormats();
  transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
  resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
  // juce_updateNowPlayingInfo();
}

void DJAudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
  resampleSource.getNextAudioBlock (bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
  transportSource.releaseResources();
  resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(const URL& audioURL)
{
  auto *reader = formatManager.createReaderFor(audioURL.createInputStream(false));
  if (reader != nullptr) // good file
  {
    // DBG ("Track Title: " << trackTitle);
    if (const String trackTitle = reader->metadataValues["Title"]; !trackTitle.isEmpty())
      this->trackTitle = trackTitle.toUTF8();
    else
      this->trackTitle = audioURL.getFileName().toUTF8();
    // loadTags(audioURL.getSubPath().toStdString().c_str());

    // load image data
    // filePath = audioURL.getSubPath().toUTF8();

    std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
    transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
    readerSource = std::move(newSource);
    juce_setAudioEngineInstance(this);
    juce_setupRemoteCommandCenter();
    juce_updateNowPlayingInfo();
  }
}

// void DJAudioPlayer::loadTags(const char* filepath)
// {
//   std::cout << "fn: " << filepath << std::endl;
//   const auto file = TagLib::FileRef{filepath};
//   std::cout << file.isNull() << std::endl;
//   std::cout << file.tag() << std::endl;
//   if (!file.isNull() && file.tag())
//   {
//     const TagLib::Tag *tag = file.tag();
//
//     std::cout << tag->title() << std::endl;
//     std::cout << tag->artist() << std::endl;
//   }
// }

void DJAudioPlayer::setGain(double gain)
{
  if (gain < 0 || gain > 2.0)
    DBG("DJAudioPlayer::setGain gain should be between 0 and 2");
  else
    transportSource.setGain(static_cast<float>(gain));
}

void DJAudioPlayer::setSpeed(const double ratio)
{
  if (ratio < 0.0 || ratio > 100.0)
    DBG("DJAudioPlayer::setSpeed ratio should be between 0 and 100");
  else
    resampleSource.setResamplingRatio(ratio);
}

void DJAudioPlayer::setPosition(const double position)
{
  transportSource.setPosition(position);
  juce_updateNowPlayingInfo();
}

void DJAudioPlayer::setPositionRelative(const double position)
{
  if (position < 0.0 || position > 1.0)
    DBG("DJAudioPlayer::setPositionRelative position should be between 0 and 1");
  else
  {
    double positionInSeconds = transportSource.getLengthInSeconds() * position;
    setPosition(positionInSeconds);
  }
}

void DJAudioPlayer::skipTrackBySeconds(const int seconds)
{
  const double newPosition = getPosition() + seconds;
  setPosition(newPosition);
}

double DJAudioPlayer::getGain() const
{
  return transportSource.getGain();
}

double DJAudioPlayer::getSpeed() const
{
  return resampleSource.getResamplingRatio();
}

double DJAudioPlayer::getPosition() const
{
  return transportSource.getCurrentPosition();
}

double DJAudioPlayer::getPositionRelative() const
{
  return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

double DJAudioPlayer::getTrackDuration() const
{
  // std::cout << transportSource.getLengthInSeconds() << std::endl;
  return transportSource.getLengthInSeconds();
}

const char* DJAudioPlayer::getTrackTitle() const
{
  // std::cout << "t: " << trackTitle << std::endl;
  // std::cout << "t: " << trackTitle.c_str() << std::endl;
  return trackTitle.c_str();
}

const char* DJAudioPlayer::getActiveDeckName() const
{
  static char buf[100];  // Static buffer persists after function returns
  snprintf(buf, sizeof(buf), "OtoDecks - %s", deckName.c_str());
  return buf;
}

const char* DJAudioPlayer::getFilePath() const
{
  static char buf[100];  // Static buffer persists after function returns
  snprintf(buf, sizeof(buf), "%s", filePath.c_str());
  return buf;
}


void DJAudioPlayer::start()
{
  if (!transportSource.isPlaying())
    transportSource.start();
  juce_setAudioEngineInstance(this);
  juce_updateNowPlayingInfo();
}

void DJAudioPlayer::stop()
{
  if (transportSource.isPlaying())
    transportSource.stop();
  juce_updateNowPlayingInfo();
}

void DJAudioPlayer::togglePlayPause()
{
  if (transportSource.isPlaying())
    transportSource.stop();
  else
    transportSource.start();
  juce_updateNowPlayingInfo();
}

bool DJAudioPlayer::isPlaying() const
{
  return transportSource.isPlaying();
}

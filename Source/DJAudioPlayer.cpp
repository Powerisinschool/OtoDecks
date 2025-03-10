/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 27 Feb 2025 9:18:01pm
    Author:  Tolulope Olagunju

  ==============================================================================
*/

#include "DJAudioPlayer.h"

#include <utility>

DJAudioPlayer::DJAudioPlayer(std::string name, AudioFormatManager &_formatManager) : deckName(std::move(name)), formatManager(_formatManager) {}
DJAudioPlayer::~DJAudioPlayer()
{
}

//==============================================================================
void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
  transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
  resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
  // for (int i = 0; i < formatManager.getNumKnownFormats(); i++)
  // {
  //   const auto format = formatManager.getKnownFormat(i);
  //   DBG("Supported format: " << format->getFormatName());
  // }
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
  // for (int i = 0; i < formatManager.getNumKnownFormats(); i++)
  // {
  //   const auto format = formatManager.getKnownFormat(i);
  //   DBG("Supported format: " << format->getFormatName());
  // }
  URL::InputStreamOptions options{URL::ParameterHandling{URL::ParameterHandling::inAddress}};
  if (auto *reader = formatManager.createReaderFor(audioURL.createInputStream(options.withConnectionTimeoutMs(5000).withNumRedirectsToFollow(3))); reader != nullptr) // good file
  {
    // DBG ("Track Title: " << trackTitle);
    // if (const String trackTitle = reader->metadataValues["Title"]; !trackTitle.isEmpty())
    //   this->trackTitle = trackTitle.toUTF8();
    // else
    //   this->trackTitle = audioURL.getFileName().toUTF8();
    // loadTags(audioURL.getSubPath().toStdString().c_str());

    // load image data
    // filePath = audioURL.getSubPath().toUTF8();

    std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
    transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
    readerSource = std::move(newSource);
    // juce_setAudioEngineInstance(this);
    // juce_setupRemoteCommandCenter();
    // juce_updateNowPlayingInfo();
  } else {
    DBG("Audio URL: " << audioURL.toString(false));
    if (! audioURL.isLocalFile())
    {
      DBG("Error: Not a local file");
    }
    else if (! audioURL.getLocalFile().existsAsFile())
    {
      DBG("Error: File does not exist");
    }
    else if (! audioURL.getLocalFile().hasReadAccess())
    {
      DBG("Error: No read access");
    }

    if (auto inputStream = audioURL.createInputStream(options); inputStream == nullptr)
    {
      DBG("Failed to create input stream!");
    }
    else
    {
      auto *_reader = formatManager.createReaderFor(std::move(inputStream));
      if (_reader == nullptr)
      {
        DBG("Failed to create reader!");
      }
    }

    DBG("DJAudioPlayer::loadURL bad file");
  }
}

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
  // juce_updateNowPlayingInfo();
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

// const char* DJAudioPlayer::getTrackTitle() const
// {
//   // std::cout << "t: " << trackTitle << std::endl;
//   // std::cout << "t: " << trackTitle.c_str() << std::endl;
//   return trackTitle.c_str();
// }

const char* DJAudioPlayer::getActiveDeckName() const
{
  static char buf[100];  // Static buffer persists after function returns
  snprintf(buf, sizeof(buf), "OtoDecks - %s", deckName.c_str());
  return buf;
}

const char* DJAudioPlayer::getFilePath() const
{
  static char buf[100];  // Static buffer persists after function returns
  // snprintf(buf, sizeof(buf), "%s", filePath.c_str());
  return buf;
}


void DJAudioPlayer::start()
{
  if (!transportSource.isPlaying())
    transportSource.start();
  // juce_setAudioEngineInstance(this);
  // juce_updateNowPlayingInfo();
}

void DJAudioPlayer::stop()
{
  if (transportSource.isPlaying())
    transportSource.stop();
  // juce_updateNowPlayingInfo();
}

void DJAudioPlayer::togglePlayPause()
{
  if (transportSource.isPlaying())
    transportSource.stop();
  else
    transportSource.start();
  // juce_updateNowPlayingInfo();
}

bool DJAudioPlayer::isPlaying() const
{
  return transportSource.isPlaying();
}

bool DJAudioPlayer::isTrackFinished() const {
  return transportSource.hasStreamFinished();
}

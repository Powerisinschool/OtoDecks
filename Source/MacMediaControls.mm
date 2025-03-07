// File to add controls to Mac's native UI

#import <AppKit/AppKit.h>
#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import <MediaPlayer/MediaPlayer.h>
#import <QuickLook/QuickLook.h>  // Add QuickLook framework
#include "DJAudioPlayer.h"

static DJAudioPlayer* juceAudioEngine = nullptr;  // Store a pointer to your engine

void setAudioEngineInstance(DJAudioPlayer* engine)
{
    juceAudioEngine = engine;  // Store the passed reference
}

void updateNowPlayingInfo()
{
    @autoreleasepool {
        if (!juceAudioEngine)
            return;

        MPNowPlayingInfoCenter *infoCenter = [MPNowPlayingInfoCenter defaultCenter];
        NSMutableDictionary *nowPlayingInfo = [NSMutableDictionary dictionary];

        // Set track title and artist
        nowPlayingInfo[MPMediaItemPropertyTitle] = @(juceAudioEngine->getTrackTitle());   // Replace with dynamic title
        nowPlayingInfo[MPMediaItemPropertyArtist] = @(juceAudioEngine->getActiveDeckName());   // Replace with DJ name or app name

        // Set track duration and playback time
        nowPlayingInfo[MPMediaItemPropertyPlaybackDuration] = @(juceAudioEngine->getTrackDuration());  // Duration in seconds
        nowPlayingInfo[MPNowPlayingInfoPropertyElapsedPlaybackTime] = @(juceAudioEngine->getPosition());  // Current position

        // Set playback rate (1.0 for normal speed, 0.0 if paused)
        nowPlayingInfo[MPNowPlayingInfoPropertyDefaultPlaybackRate] = @1.0;
        nowPlayingInfo[MPNowPlayingInfoPropertyPlaybackRate] = juceAudioEngine->isPlaying() ? @1.0 : @0.0;

        // Set Album art
        /*NSString *path = @(juceAudioEngine->getFilePath());
        NSURL *fileURL = [NSURL fileURLWithPath:path];

        CGSize iconSize = CGSizeMake(512, 512);  // Adjust size as needed
        NSDictionary *options = @{ (__bridge NSString *)kQLThumbnailOptionIconModeKey: @NO };
        CGImageRef thumbnail = QLThumbnailImageCreate(kCFAllocatorDefault, (__bridge CFURLRef)fileURL, iconSize, (__bridge CFDictionaryRef)options);

        if (!thumbnail) {
            NSLog(@"❌ Failed to get QuickLook thumbnail for: %s", path);
            return;
        }

        // Convert CGImage to NSImage
        NSImage *thumbnailImage = [[NSImage alloc] initWithCGImage:thumbnail size:NSZeroSize];
        CFRelease(thumbnail); // Release CGImage to prevent memory leaks

        if (!thumbnailImage) {
            NSLog(@"❌ Failed to create NSImage from thumbnail.");
            return;
        }

        MPMediaItemArtwork *artwork = [[MPMediaItemArtwork alloc] initWithBoundsSize:thumbnailImage.size requestHandler:^NSImage * _Nonnull(CGSize size) {
            return thumbnailImage;  // Return the NSImage for Now Playinh
        }];

        nowPlayingInfo[MPMediaItemPropertyArtwork] = artwork;*/

        /*const char *imagePath = @(juceAudioEngine->getArtworkPath());
        NSString *path = [NSString stringWithUTF8String:imagePath];
        UIImage *albumArt = [UIImage imageWithContentsOfFile:path];

        // ✅ Add artwork
        if (albumArt) {
            MPMediaItemArtwork *artwork = [[MPMediaItemArtwork alloc] initWithBoundsSize:albumArt.size requestHandler:^UIImage * _Nonnull(CGSize size) {
                return albumArt;
            }];
            nowPlayingInfo[MPMediaItemPropertyArtwork] = artwork;
        }*/

        // Explicitly tell macOS that the Touch Bar should show skip controls
        nowPlayingInfo[MPNowPlayingInfoPropertyIsLiveStream] = @NO;

        // Update Now Playing bar
        infoCenter.nowPlayingInfo = nowPlayingInfo;
    }
}

// Function to update Now Playing info
//void updateNowPlayingInfo(NSString* title, NSString* artist, double duration, double elapsedTime, float playbackRate)
//{
//    MPNowPlayingInfoCenter *center = [MPNowPlayingInfoCenter defaultCenter];
//    NSMutableDictionary *info = [NSMutableDictionary dictionary];
//
//    info[MPMediaItemPropertyTitle] = title;
//    info[MPMediaItemPropertyArtist] = artist;
//    info[MPMediaItemPropertyPlaybackDuration] = @(duration);
//    info[MPNowPlayingInfoPropertyElapsedPlaybackTime] = @(elapsedTime);
//    info[MPNowPlayingInfoPropertyPlaybackRate] = @(playbackRate);
//
//    center.nowPlayingInfo = info;
//}

void setupRemoteCommandCenter()
{
    MPRemoteCommandCenter *commandCenter = [MPRemoteCommandCenter sharedCommandCenter];

    [commandCenter.playCommand addTargetWithHandler:^MPRemoteCommandHandlerStatus(MPRemoteCommandEvent *event) {
        if (juceAudioEngine) juceAudioEngine->start();
        updateNowPlayingInfo();  // 🔹 Update Now Playing Bar
        return MPRemoteCommandHandlerStatusSuccess;
    }];

    [commandCenter.pauseCommand addTargetWithHandler:^MPRemoteCommandHandlerStatus(MPRemoteCommandEvent *event) {
        if (juceAudioEngine) juceAudioEngine->stop();
        updateNowPlayingInfo();  // 🔹 Update Now Playing Bar
        return MPRemoteCommandHandlerStatusSuccess;
    }];

    [commandCenter.togglePlayPauseCommand addTargetWithHandler:^MPRemoteCommandHandlerStatus(MPRemoteCommandEvent *event) {
        if (juceAudioEngine) juceAudioEngine->togglePlayPause();
        updateNowPlayingInfo();  // 🔹 Update Now Playing Bar
        return MPRemoteCommandHandlerStatusSuccess;
    }];

    // Explicitly enable seek forward/backward on Touch Bar
    commandCenter.togglePlayPauseCommand.enabled = YES;
    commandCenter.playCommand.enabled = YES;
    commandCenter.pauseCommand.enabled = YES;
    commandCenter.changePlaybackPositionCommand.enabled = YES;

    // Skip forward 5 seconds
    commandCenter.skipForwardCommand.enabled = YES;
    commandCenter.skipForwardCommand.preferredIntervals = @[@5]; // 5-second skip
    [commandCenter.skipForwardCommand addTargetWithHandler:^MPRemoteCommandHandlerStatus(MPRemoteCommandEvent *event) {
        if (juceAudioEngine) juceAudioEngine->skipTrackBySeconds(5);
        return MPRemoteCommandHandlerStatusSuccess;
    }];

    // Skip backward 5 seconds
    commandCenter.skipBackwardCommand.enabled = YES;
    commandCenter.skipBackwardCommand.preferredIntervals = @[@5]; // 5-second rewind
    [commandCenter.skipBackwardCommand addTargetWithHandler:^MPRemoteCommandHandlerStatus(MPRemoteCommandEvent *event) {
        if (juceAudioEngine) juceAudioEngine->skipTrackBySeconds(-5);
        return MPRemoteCommandHandlerStatusSuccess;
    }];

    // Seek Command (Scrubbing)
    [commandCenter.changePlaybackPositionCommand addTargetWithHandler:^MPRemoteCommandHandlerStatus(MPRemoteCommandEvent *event) {
        auto *positionEvent = (MPChangePlaybackPositionCommandEvent *)event;
        double newTime = positionEvent.positionTime; // New position in seconds
        if (juceAudioEngine) juceAudioEngine->setPosition(newTime); // Implement this function to update playback
        return MPRemoteCommandHandlerStatusSuccess;
    }];
}

extern "C"
{
    void juce_setAudioEngineInstance(DJAudioPlayer* engine)
    {
        setAudioEngineInstance(engine);
    }

    // void juce_updateNowPlayingInfo(const char* title, const char* artist, double duration, double elapsedTime, float playbackRate)
    void juce_updateNowPlayingInfo()
    {
        updateNowPlayingInfo();
        /*updateNowPlayingInfo([NSString stringWithUTF8String:title],
                             [NSString stringWithUTF8String:artist],
                             duration, elapsedTime, playbackRate);*/
    }

    void juce_setupRemoteCommandCenter()
    {
        setupRemoteCommandCenter();
    }
}

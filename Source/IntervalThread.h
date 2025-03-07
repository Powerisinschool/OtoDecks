/*
  ==============================================================================

    IntervalThread.h
    Created: 4 Mar 2025 9:06:01am
    Author:  Tolulope Olagunju

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class IntervalThread : Thread
{
public:
  explicit IntervalThread(std::atomic_bool *cancelToken, const std::function<void()>& runnerFn);

  // Delete copy operations
  IntervalThread(const IntervalThread&) = delete;
  IntervalThread& operator=(const IntervalThread&) = delete;

  // Allow move operations
  IntervalThread(IntervalThread&&) noexcept = default;
  IntervalThread& operator=(IntervalThread&&) noexcept = default;

  void run() override;

private:
  std::atomic_bool *mCancelToken;
  std::function<void()> mFn;
};

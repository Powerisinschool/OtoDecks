/*
  ==============================================================================

    IntervalThread.cpp
    Created: 4 Mar 2025 9:06:01am
    Author:  Tolulope Olagunju

  ==============================================================================
*/

#include "IntervalThread.h"

IntervalThread::IntervalThread(std::atomic_bool* cancelToken, const std::function<void()>& runnerFn) : Thread("IntervalThread"),
                                                                                                       mCancelToken(cancelToken),
                                                                                                       mFn(runnerFn)
{
  startThread(Priority::low);
}

void IntervalThread::run()
{
  while (!mCancelToken->load())
  {
    MessageManager::callAsync([this]()
    {
        mFn(); // Runs UI updates on the message thread
    });
    sleep(100);
  }
  // stopThread(100);
}

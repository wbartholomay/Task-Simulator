//
//  ECSimTask2.cpp
//  
//
//

#include "ECSimTask2.h"
#include <iostream>

//################ECMultiIntervalTask#####################

ECMultiIntervalsTask::ECMultiIntervalsTask(const std::string &tid):ECSimTask(tid), intervals({{}}) {}

bool ECMultiIntervalsTask::IsReadyToRun(int tick) const{
    //if tick falls within any of the intervals, run that interval
    for(auto k : intervals)
        if(tick >= k.first && tick <= k.second) return true;

    return false;
}

bool ECMultiIntervalsTask::IsFinished(int tick) const{
    //if tick is beyond the last interval, Task is done
    return tick > intervals.back().second;
}

void ECMultiIntervalsTask::AddInterval(int a, int b){
    //add interval (a, b) to vector of intervals
    std::pair<int, int> interval(a, b);
    intervals.push_back(interval);
}

//################ECHardIntervalTaslk#####################

ECHardIntervalTask::ECHardIntervalTask(const std::string &tid, int start, int end):ECSimTask(tid), tmStart(start), tmEnd(end),
hasStarted(false){}

bool ECHardIntervalTask::IsReadyToRun(int tick) const{
    //only starts on interval start
    return hasStarted && tick <= tmEnd || tick == tmStart;
}

bool ECHardIntervalTask::IsFinished(int tick) const{
    //ends after end of interval passed, or if the start interval has passed and it is not running
    return (tick > tmEnd) || ((tick > tmStart) && !hasStarted);
}

void ECHardIntervalTask::Run(int tick, int duration){
    ECSimTask::Run(tick, duration);
    hasStarted = true;
}

void ECHardIntervalTask::Wait(int tick, int duration){
    ECSimTask::Wait(tick, duration);
}


//################ECConsecutiveIntervalTask#####################

ECConsecutiveIntervalTask::ECConsecutiveIntervalTask(const std::string &tid, int start, int end):ECSimTask(tid), tmStart(start),
tmEnd(end), isRunning(false), wasInterrupted(false){}

bool ECConsecutiveIntervalTask::IsReadyToRun(int tick) const{
    //start if time is within interval, and the task has not been previously started and interrupted
    return tick >= tmStart && tick <= tmEnd && !wasInterrupted;
}

bool ECConsecutiveIntervalTask::IsFinished(int tick) const{
    //finish if the interval has passed, or the task has been started and interrupted
    return tick > tmEnd || wasInterrupted;
}

void ECConsecutiveIntervalTask::Run(int tick, int duration){
    ECSimTask::Run(tick, duration);
    isRunning = true;
}

void ECConsecutiveIntervalTask::Wait(int tick, int duration){
    ECSimTask::Wait(tick, duration);
    //if task is being told to wait after starting, it is being interrupted
    if(isRunning){
        wasInterrupted = true;
    }
}


//################ECConsecutiveIntervalTask#####################

ECPeriodicTask::ECPeriodicTask(const std::string &tid, int start, int run, int sleep):ECSimTask(tid), tmStart(start), runLen(run),
sleepLen(sleep){}

bool ECPeriodicTask::IsReadyToRun(int tick) const{
    int cycleLen = runLen + sleepLen;
    //if it is on a run interval, run
    return tick >= tmStart && ((tick - tmStart) % cycleLen) < runLen;
}

bool ECPeriodicTask::IsFinished(int tick) const{
    //is never finished, always has another interval coming up
    return false;
}
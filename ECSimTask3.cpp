//
//  ECSimTask3.cpp
//  
//

#include "ECSimTask3.h"
#include <iostream>
using namespace std;

//***********************************************************
// Basic task
//***********************************************************

//***********************************************************
// Interval task: a single interval.
// YW: you shouldn't need to change this class!

ECSimIntervalTask :: ECSimIntervalTask(const std::string &tidIn, int tmStartIn, int tmEndIn) : tid(tidIn), tmStart(tmStartIn), tmEnd(tmEndIn), tmTotWait(0), tmTotRun(0)
{
}

// Is task ready to run at certain time? tick: the current clock time (in simulation unit)
bool ECSimIntervalTask :: IsReadyToRun(int tick) const
{
    return tick >= tmStart && tick <= tmEnd;
}

// Is task complete at certain time? If so, scheduler may remove it from the list. tick the current clock time (in simulation unit)
bool ECSimIntervalTask :: IsFinished(int tick) const
{
    return tick > tmEnd;
}


//***********************************************************
// Consecutive task: a task that can early abort

ECSimConsecutiveTask :: ECSimConsecutiveTask(ECSimTask *pTask) 
{
    task = pTask;
    isAborted = false;

}

// your code here
void ECSimConsecutiveTask::Run(int tick, int duration){
    task->Run(tick, duration);
    isRunning = true;
}

void ECSimConsecutiveTask::Wait(int tick, int duration){
    task->Wait(tick, duration);
    if(isRunning) isAborted = true;
}

bool ECSimConsecutiveTask::IsAborted(int tick) const{
    //abort if interrupted
    return task->IsAborted(tick) || isAborted;
}

bool ECSimConsecutiveTask::IsReadyToRun(int tick) const{
    //do not run if aborted
    return task->IsReadyToRun(tick) && !isAborted;
}

//***********************************************************
// Periodic task: a task that can early abort

ECSimPeriodicTask :: ECSimPeriodicTask(ECSimTask *pTask, int ls) 
{
    task = pTask;
    sleepLen = ls;
    runTime = 0;
    startTime = 0;
    numCycles = 0;
    firstCycleStarted = false;
    firstCycleEnded = false;
}


bool ECSimPeriodicTask :: IsReadyToRun(int tick) const{
    //use the run time found from the first cycle
    int cycleLen = runTime + sleepLen;
    //if task has not yet ran first cycle, runTime will not have been measured
    if(!firstCycleEnded){
        return task->IsReadyToRun(tick);
    }
    else{
        int adjustedTick = tick - (cycleLen * numCycles);
        // std::cout <<"running "<<"tick: "<<tick<<" adjusted Tick: "<<adjustedTick<<" numCycles: "<<numCycles<<std::endl;
        return task->IsReadyToRun(adjustedTick);
    }
}

bool ECSimPeriodicTask :: IsFinished(int tick) const{
    int cycleLen = runTime + sleepLen;
    if(!firstCycleEnded){
        return task->IsFinished(tick);
    }
    else{
        int adjustedTick = tick - (cycleLen * numCycles);
        // std::cout <<"finished "<<"tick: "<<tick<<" adjusted Tick: "<<adjustedTick<<" numCycles: "<<numCycles<<std::endl;
        return task->IsFinished(adjustedTick);
    }
}

void ECSimPeriodicTask :: Run(int tick, int duration){
    //block needs to be in Run and Wait instead of isReadyToRun, as it makes changes to class members
    //responsible for tracking run time length
    if(!firstCycleStarted){
        //if the first cycle has not yet started, log the start time
        startTime = tick;
        firstCycleStarted = true;
    }
    if(IsFinished(tick+1)){
        //if the first cycle is going to end at the next tick, log the cycle length
        if(!firstCycleEnded){
            runTime = tick - startTime + 1;
            firstCycleEnded = true;
        }
        numCycles++;
    }
    // std::cout<<task->GetId()<<" running at tick "<<tick<<std::endl;
    task->Run(tick, duration);
}

void ECSimPeriodicTask :: Wait(int tick, int duration){
    //block needs to be in Run and Wait instead of isReadyToRun, as it makes changes to class members
    //responsible for tracking run time length
    if(!firstCycleStarted){
        //if the first cycle has not yet started, log the start time
        startTime = tick;
        firstCycleStarted = true;
    }
    if(IsFinished(tick+1)){
        //if the first cycle is going to end at the next tick, log the cycle length
        if(!firstCycleEnded){
            runTime = tick - startTime + 1;
            firstCycleEnded = true;
        }
        numCycles++;
    }
    // std::cout<<task->GetId()<<" waiting at tick "<<tick<<std::endl;

    task->Wait(tick, duration);
}

//***********************************************************
// Task with a deadline to start: a task that must start by some time; otherwise terminate

ECSimStartDeadlineTask :: ECSimStartDeadlineTask(ECSimTask *pTask, int tmStartDeadlineIn)
{
    task = pTask;
    startDeadline = tmStartDeadlineIn;
    isAborted = false;
    hasStarted = false;

}

// your code here

void ECSimStartDeadlineTask :: Run(int tick, int duration){
    if(!hasStarted) hasStarted = true;
    task->Run(tick, duration);
}

void ECSimStartDeadlineTask :: Wait(int tick, int duration){
    if(tick >= startDeadline && !hasStarted)
            isAborted = true;

    task->Wait(tick, duration);
}

bool ECSimStartDeadlineTask :: IsAborted(int tick) const{
    return isAborted || task->IsAborted(tick);
}

bool ECSimStartDeadlineTask :: IsFinished(int tick) const{
    return isAborted || task->IsFinished(tick);
}

//***********************************************************
// Task must end by some fixed time click: this is useful e.g. when a task is periodic

ECSimEndDeadlineTask :: ECSimEndDeadlineTask(ECSimTask *pTask, int tmEndDeadlineIn) 
{
    task = pTask;
    endDeadline = tmEndDeadlineIn;
}

// your code here

bool ECSimEndDeadlineTask :: IsAborted(int tick) const{
    return tick > endDeadline || task->IsAborted(tick);
}

bool ECSimEndDeadlineTask :: IsFinished(int tick) const{
    return tick > endDeadline || task->IsFinished(tick);
}

//***********************************************************
// Composite task: contain multiple sub-tasks

ECSimCompositeTask :: ECSimCompositeTask(const std::string &tidIn){
    tid = tidIn;
    numTasks = 0;
    tmTotRun = 0;
    tmTotWait = 0;
}


void ECSimCompositeTask :: AddSubtask(ECSimTask *pt){
    tasks.push_back(pt);

    //update cycleLen
    // int minStart = tasks[0]->tmStart;
    // int maxEnd = tasks[0]->tmEnd;
    // for(ECSimTask* task : tasks){
    //     if(task->tmStart < minStart) minStart = task->tmStart;
    //     if(task->tmEnd > maxEnd) maxEnd = task->tmEnd;
    // }

    // cycleLen = maxEnd - minStart;
}

void ECSimCompositeTask :: Run(int tick, int duration){
    //tasks not ready will still wait
    for(ECSimTask* task : tasks){
        if(task->IsReadyToRun(tick))
            task->Run(tick, duration);
    }
    tmTotRun++;
}

void ECSimCompositeTask :: Wait(int tick, int duration){
    for(ECSimTask* task : tasks){
        task->Wait(tick, duration);
    }
    tmTotWait++;
}

bool ECSimCompositeTask :: IsAborted(int tick) const{
    for(ECSimTask* task : tasks){
        if(!task->IsAborted(tick))
            return false;
    }
    return true;
}

bool ECSimCompositeTask :: IsReadyToRun(int tick) const{
    for(ECSimTask* task : tasks){
        if(task->IsReadyToRun(tick))
            //std::cout << "Task: " << GetId() << " Ready at tick: " << tick << endl;
            return true;
    }
    return false;
}

bool ECSimCompositeTask :: IsFinished(int tick) const{
    for(ECSimTask* task : tasks){
        if(!task->IsFinished(tick))
            return false;
    }
    return true;
}

int ECSimCompositeTask :: GetTotRunTime() const{
    return tmTotRun;
}

int ECSimCompositeTask :: GetTotWaitTime() const{
    return tmTotWait;
}


// your code here
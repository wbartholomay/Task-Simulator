//
//  ECSimTask3.h
//  
//
//  Flexible task classes, with the following attribues
//  that you can choose any subsets:
//  (i) consequitive: if asked to wait while it wants to run before finish, terminates before completing all the planned time
//  (ii) delayable (can be delayed; e.g. [1,3] can become [4,5] (delayed by 3 time ticks)
//  (iii) deadline to start (must start by certain time)
//  (iv) deadline to finish (must end by some time)
//  (v) periodic: this task runs restarts after it finishes its current iteration
//  (vi) can contain sub-tasks: if any subtask aborts, this task is finished; otherwise it finishes when all subtasks finishes.

#ifndef ECSimTask3_h
#define ECSimTask3_h

#include <vector>
#include <string>
#include <iostream>

//***********************************************************
// Generic simulation task
//***********************************************************

class ECSimTask
{
public:
    virtual ~ECSimTask() {}
    
    // Get the task id
    virtual std::string GetId() const = 0;
    
    // Is task ready to run at certain time? tick: the current clock time (in simulation unit)
    virtual bool IsReadyToRun(int tick) const = 0;
    
    // Is task complete at certain time? If so, scheduler may remove it from the list. tick the current clock time (in simulation unit)
    virtual bool IsFinished(int tick) const = 0;
    
    // Is task early abort? There can be various reasons for abort: e.g., missed deadline
    virtual bool IsAborted(int tick) const = 0;
    
    // Run the task for some duration (usually 1, but can be more) starting from time tick
    virtual void Run(int tick, int duration) = 0;
    
    // Wait for some duration (usually 1, but can be more), starting from time tick
    virtual void Wait(int tick, int duration) = 0;
    
    // How much total time does the task has to wait to get its turn so far?
    virtual int GetTotWaitTime() const = 0;
    
    // Get total run-time (so far)
    virtual int GetTotRunTime() const = 0;
};


//***********************************************************
// Basic task: you shouldn't need to change this class!
//***********************************************************

//***********************************************************
// Interval task: a single interval

class ECSimIntervalTask : public ECSimTask
{
public:
    ECSimIntervalTask(const std::string &tid, int tmStart, int tmEnd);
    
    // Get the task id
    virtual std::string GetId() const { return this->tid; }
    
    // Is task ready to run at certain time? tick: the current clock time (in simulation unit)
    virtual bool IsReadyToRun(int tick) const;
    
    // Is task complete at certain time? If so, scheduler may remove it from the list. tick the current clock time (in simulation unit)
    virtual bool IsFinished(int tick) const;
    
    // Is task early abort? There can be various reasons for abort: e.g., missed deadline
    virtual bool IsAborted(int tick) const { return false; }
    
    // Run the task for some duration (usually 1, but can be more) starting from time tick
    virtual void Run(int tick, int duration) { tmTotRun += duration; }
    
    // Wait for some duration (usually 1, but can be more), starting from time tick
    virtual void Wait(int tick, int duration) { tmTotWait += duration; }
    
    // How much total time does the task has to wait to get its turn so far?
    virtual int GetTotWaitTime() const { return tmTotWait; }
    
    // Get total run-time (so far)
    virtual int GetTotRunTime() const { return tmTotRun; }
    
private:
    std::string tid;
    int tmTotWait;
    int tmTotRun;
    int tmStart;
    int tmEnd;
};


//***********************************************************
// Consecutive task: a task that can early abort
// This class modifies the passed-in task to have the consecutive property

//decorator
class ECSimConsecutiveTask : public ECSimTask
{
public:
  ECSimConsecutiveTask(ECSimTask *pTask);
  std::string GetId() const {return task->GetId();}

  void Run(int tick, int duration);
  void Wait(int tick, int duration);

  bool IsAborted(int tick) const;
  bool IsReadyToRun(int tick) const;
  bool IsFinished(int tick) const {return task->IsFinished(tick);}

  int GetTotRunTime() const {return task->GetTotRunTime();}
  int GetTotWaitTime() const {return task->GetTotWaitTime();}


private:
  ECSimTask* task;
  bool started;
  bool aborted;
};

//***********************************************************
// Periodic task: a task that can early abort
// This class modifies the passed-in task to have the early abort property

//also inherit to allow for polymorphism
class ECSimPeriodicTask : public ECSimTask
{
public:
  ECSimPeriodicTask(ECSimTask *pTask, int lenSleep);

  std::string GetId() const {return task->GetId();}

  void Run(int tick, int duration);
  void Wait(int tick, int duration);

  bool IsAborted(int tick) const {return false;}
  bool IsReadyToRun(int tick) const;
  bool IsFinished(int tick) const;

  int GetTotRunTime() const {return task->GetTotRunTime();}
  int GetTotWaitTime() const {return task->GetTotWaitTime();}

private:
  int sleepLen;

  int runTime;
  int startTime;

  bool firstCycleEnded;
  ECSimTask* task;
};

//***********************************************************
// Task with a deadline to start: a task that must start by some time; otherwise terminate
// This class modifies the passed-in task to have a deadline to start 

class ECSimStartDeadlineTask : public ECSimTask
{
public:
  ECSimStartDeadlineTask(ECSimTask *pTask, int tmStartDeadline);

  // your code here

  std::string GetId() const {return task->GetId();}

  void Run(int tick, int duration);
  void Wait(int tick, int duration);

  bool IsAborted(int tick) const;
  bool IsReadyToRun(int tick) const {return task->IsReadyToRun(tick);}
  bool IsFinished(int tick) const;

  int GetTotRunTime() const {return task->GetTotRunTime();}
  int GetTotWaitTime() const {return task->GetTotWaitTime();}

private:
  ECSimTask* task;
  int startDeadline;
  bool started;
  bool aborted;
};

//***********************************************************
// Task must end by some fixed time click: this is useful e.g. when a task is periodic
// This class modifies the passed-in task to have a deadline to end 

class ECSimEndDeadlineTask : public ECSimTask
{
public:
  ECSimEndDeadlineTask(ECSimTask *pTask, int tmEndDeadline);

  // your code here

  std::string GetId() const {return task->GetId();}

  void Run(int tick, int duration) {task->Run(tick, duration);}
  void Wait(int tick, int duration) {task->Wait(tick, duration);}

  bool IsAborted(int tick) const;
  bool IsReadyToRun(int tick) const {return task->IsReadyToRun(tick);}
  bool IsFinished(int tick) const;

  int GetTotRunTime() const {return task->GetTotRunTime();}
  int GetTotWaitTime() const {return task->GetTotWaitTime();}

private:
  ECSimTask* task;
  int endDeadline;
};

//***********************************************************
// Composite task: contain multiple sub-tasks

class ECSimCompositeTask : public ECSimTask
{
public:
  int cycleLen;

  ECSimCompositeTask(const std::string &tidIn); 

  virtual std::string GetId() const {return tid;};
    
  // Add subtask
  void AddSubtask(ECSimTask *pt);

  // your code ehre
  void Run(int tick, int duration);
  void Wait(int tick, int duration);

  bool IsAborted(int tick) const;
  bool IsReadyToRun(int tick) const;
  bool IsFinished(int tick) const;

  int GetTotRunTime() const;
  int GetTotWaitTime() const;
    
private:
  std::string tid;
  std::vector<ECSimTask*> tasks;
  int tmTotRun;
  int tmTotWait;
};

#endif /* ECSimTask3_h */

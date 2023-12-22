Part 1: ECSimTask and ECSimTaskScheduler were given as template code. 
In ECSimTask2 and ECSimTaskScheduler2 is my code, which implements new task types and scheduler types through an inheritance hierarchy. 
These tasks and schedulers are tested in ECSimTaskTests.

Part2: ECSimTaskScheduler3 as well as the ECSimTask and ECSimIntervalTask classes are template code.
The decorators for ECSimTask objects in ECSimTask3 were created by me.
These decorators are tested in ECSimTaskTests3.




Project Description:

This course project is about task scheduling simulation. Task scheduling is very common in different
applications, e.g., operating systems or production line. Here, we want to implement a simpler task
scheduler that simulates the behavior of task scheduling. Here is a brief overview of several properties
of this simulation. Please read the provided starter code to understand better of this system.

1. Time. We consider time as a whole integer. You can view time like a clock ticking. Time starts from
1. That is, once simulation starts, the first time tick is 1.
   
2. Task. A task is represented by one interval [a, b] (start and end time) or multiple intervals. This task
wants to run during these intervals. There are different kinds of tasks. We have provided a base
class, ECSimTask, and one concrete task, ECSoftIntervalTask. Some more details below.

a) There are three states of tasks: (i) idle (not running, either because it is not ready to run at  
this time or it is not chosen by the scheduler to run); (ii) running (scheduled by the scheduler),
and (iii) finished (it will no longer run, either because its time has passed or it is early
terminated by some condition).

b) Task is preemptive. That is, a running task can be put to idle by the scheduler.

c) A task would keep track of how long it has run (only count the actual running time, not
including idling time).

d) A task also keeps track of waiting time. Waiting time is the number of ticks when the task is
ready to run but has to wait since the scheduler chose another task to run.

3. Scheduler. A scheduler keeps track a list of active tasks and picks a task to run at each time tick.
Exactly how it decides what task to choose is decided by the specific scheduler, which implements
different scheduling algorithms.

a) A simulator keeps track of time (which initially is zero and the first simulation time tick is 1). It
also keeps track of which task (if any) is running at the current time.

b) Only a single task can be chosen to run at any time tick. That is, there is noparallelization in
this system.
c) The key interface function of scheduler is “Simulate”. This function takes a time duration and
will run the simulation up to this time duration (it can early terminate if there is nothing
more to run; the return value is equal to the number of time ticksthe simulator actually runs).

d) A concrete type of scheduler, ECSimFIFOTaskScheduler, is given. This scheduler is first‐come‐
first‐serve (or first‐in‐first‐out, FIFO).

1 Project Part‐I  
Read the starter files for the exact function signatures you should implement.

1. Read the provided starter code in ECSimTask.h, ECSimTask.cpp, ECSimTaskScheduler.h,
ECSimTaskScheduler.cpp and ECSimTaskTests.cpp. Don’t change these starter files.  These starter
code should work in the first test case. Understand how the starter code works.  

2. There is a class hierarchy. Make sure you use your new classes fit into the class hierarchy.

3. Implement several additional types of tasks in ECSimTasks2.h/cpp.

(a) ECMultiIntervalsTask. This type of task consists multiple intervals. It has a method
AddInterval(int a, int b) to add an interval [a,b] to it. Other than this, it behaves the same as
the single (soft) interval.

(b) ECHardIntervalTask. This is different from the given ECSoftIntervalTask in that it must start at
the time it requested; otherwise, this task would finish. Once it starts on time, it behaves just
like ECSoftIntervalTask, which can run intermittently (i.e., sometimes running and sometimes
idling).

(c) ECConsecutiveIntervalTask. This type of task must run in consecutive interval. That is, once
interrupted, it finishes.

(d) ECPeriodicTask. This type of task is recurring: it requests to run periodically of fixed length
and then idle for fixed time, after a certain starting time.

4. Implement several additional types of schedulers in ECSimTaskScheduler2.h/cpp. These
schedulers implement different algorithms about choosing what tasks to schedule.    In all the
following scheduling algorithms, when there is a tie, prefer the request that comes in the first (i.e,
FIFO). This has been implemented in the starter code.

(a) ECSimLWTFTaskScheduler. Longest waiting time first: choose the task that has waited for the
longest time.

(b) ECSimPriorityScheduler. Schedule the task with highest priority (recall lower priority value
has higher priority; this is consistent with Unix/Linux priority).

(c) ECSimRoundRobinTaskScheduler. Schedule the task that has been scheduled the least
number of times (count each time unit).

2 Project Part‐II

This second part of the project revisits the above task scheduling simulation from the perspective of
design patterns. In this part, we focus on the task, not scheduler (we will only use one simple scheduler,
FIFO). The main issue here is the diversity of different kinds of tasks.

1. You are provided with the implementation of a basic kind of task called ECSimIntervalTask. This
task behaves like a task in Part I: it runs in an interval of time (e.g., from time 6 to time 10). Other
than this, this vanilla task has no other special properties. Your task is to modify this simple task to
have more features as described in the following. Please don’t change the ECSimIntervalTask class.

2. Variations in properties of tasks.
(a) A task can be required to run consecutively. That is, once a task is run, it must run to the

time it wants to stop; otherwise the task finishes.
(b) A task may be periodic. That is, between runs, a task may sleep for a fixed number of time
ticks. A little more details about the periodic task:

i. A periodic task restarts after the sleeping time at the first time it runs last time. That is,
you should ignore the initial waiting time. For example, suppose a periodic task first runs
from tick 7 to 8, and the wait time is 3. Then the next time it runs at 12 (instead of
3+6=9).

ii. You should make periodic task behave like copying itself as time runs. Suppose a
periodic task runs from 7 to 9 (when it aborts due to say ending deadline at 9), and wait
time is 2. Then the next time it runs should be from 12 to 14 (it is like ending deadline
now is 14). Note here you would abort at 14 because periodic task would like to repeat
itself.

(c) A task may have a start deadline: it must start by certain time; otherwise it finishes.

(d) A task may have an ending deadline: it must finish by certain time (this can be an issue for
e.g., periodic task).

3. Structures. Multiple tasks may be grouped together. That is, a task can contain sub‐tasks.
There may be even more kinds of variations (e.g., a task may be delayed: its interval is [1,3], and it is OK to
run as [5,7] if needed). In Part I, we address this situation by creating subclasses of ECTask. However, as
you should know by now, a main issue for thisis the potentially large number of subclasses when you want
to combine variations. For example, you may want a task that is consecutive, has a start deadline and is
periodic. We want you to implement the Decorator pattern to address this issue of variations. To address
the situation that a task having subtasks, you should apply Composite pattern.
Read the starter files for the exact function signatures you should implement.
1. Implement the following variations of ECSimTask in ECSimTasks3.h/cpp. Note these variations
(decorations) can be combined. Refer to the starter code for its usages (check out the provided test
code).

(a) ECSimConsecutiveTask: this kind of task cannot be put to wait once started.

(b) ECPeriodicTask. This type of task is recurring: it requests to run periodically of fixed length
and then sleep for fixed time.

(c) ECSimStartDeadlineTask: must start by certain time. Otherwise it aborts.

(d) ECSimEndDeadlineTask: must end by certain time.

You should ensure the above variations can be combined. Note: the order of decoration may matter.
To see this, consider a task of interval [3, 5]. Suppose we first make it periodic and then end by time
4. This modified task can only run from 3 to 4. But if we modify first the ending time to be 4 and
then make it periodic, this task would run from 3 to 4 and then repeat sometime afterwards. The
rule is, you would modify the task that is passed in; the current modification is on top of the
modifications you have made before.

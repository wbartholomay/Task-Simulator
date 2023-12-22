//
//  ECSimTaskScheduler2.cpp
//  
//
//

#include <vector>
#include <iostream>
#include <climits>

using namespace std;

#include "ECSimTaskScheduler2.h"
#include "ECSimTask.h"

ECSimLWTFTaskScheduler::ECSimLWTFTaskScheduler(){}

ECSimTask *ECSimLWTFTaskScheduler::ChooseTaskToSchedule(const std::vector<ECSimTask *> &listReadyTasks) const{
    int maxWaitTime = -1;
    ECSimTask* taskToRun = nullptr;

    //return null pointer if there are no tasks ready
    if(listReadyTasks.size() == 0) return taskToRun;

    //find the task with the longest wait time, and return
    for(ECSimTask* task : listReadyTasks){
        if(task->GetTotWaitTime() > maxWaitTime){
            maxWaitTime = task->GetTotWaitTime();
            taskToRun = task;
        }
    }

    return taskToRun;
}



ECSimPriorityScheduler::ECSimPriorityScheduler(){}

ECSimTask *ECSimPriorityScheduler::ChooseTaskToSchedule(const std::vector<ECSimTask *> &listReadyTasks) const{
    int minPriority = INT_MAX;
    ECSimTask* taskToRun = nullptr;

    if(listReadyTasks.size() == 0) return taskToRun;

    //find the task with the lowest priority, and return
    for(ECSimTask* task : listReadyTasks){
        if(task->GetPriority() < minPriority){
            minPriority = task->GetPriority();
            taskToRun = task;
        }
    }

    return taskToRun;
}


ECSimRoundRobinTaskScheduler::ECSimRoundRobinTaskScheduler(){}

ECSimTask *ECSimRoundRobinTaskScheduler::ChooseTaskToSchedule(const std::vector<ECSimTask *> &listReadyTasks) const{
    int minRunTime = INT_MAX;
    ECSimTask* taskToRun = nullptr;

    if(listReadyTasks.size() == 0) return taskToRun;

    //find the task with the lowest priority, and return
    for(ECSimTask* task : listReadyTasks){
        // std::cout<<"Task ID "<<task->GetId()<<" Run Time "<<task->GetTotRunTime()<<" Wait Time "<<task->GetTotWaitTime()<< std::endl;
        if(task->GetTotRunTime() < minRunTime){
           minRunTime = task->GetTotRunTime();
            taskToRun = task;
        }
    }

    return taskToRun;
}
#include "ThreadPool.hpp"

typedef ThreadPool::Task Task;

void ThreadPool::initialize(const unsigned int threadsCount)
{
    mTasksQueue.clear();
    for (auto iter = 0; threadsCount > iter; ++iter)
    {
        mThreads.emplace_back(ScopedThread(&ThreadPool::workerThread));
    }
}

void ThreadPool::uninitialize()
{
    for (auto iter = 0; mThreads.size() > iter; ++iter)
    {
        mTasksQueue.emplace(std::make_tuple(TaskPriority::Realtime, Task(), Task()));
    }
    mThreads.clear();
}

void ThreadPool::submit(TaskPriority priority, Task taskCallback, Task taskDoneCallback)
{
    mTasksQueue.emplace(std::make_tuple(priority, taskCallback, taskDoneCallback));
}

void ThreadPool::workerThread()
{
    for (;;)
    {
        ThreadTaskData taskData;

        mTasksQueue.pop(taskData);
        auto taskCallback = std::get<1>(taskData);
        auto taskDoneCallback = std::get<2>(taskData);

        if (taskCallback)
        {
            taskCallback();
            if (taskDoneCallback)
            {
                taskDoneCallback();
            }
        }
        else
        {
            return;
        }
    }
}

ThreadSafePriorityQueue< ThreadPool::ThreadTaskData, std::priority_queue< ThreadPool::ThreadTaskData, std::vector<ThreadPool::ThreadTaskData>, ThreadPool::ThreadTaskDataComparator > > ThreadPool::mTasksQueue;
std::vector< ScopedThread > ThreadPool::mThreads;

#include "ActiveObject.hpp"

typedef ActiveObject::Task Task;

ActiveObject::ActiveObject()
    : mThread(Task())
{
    mThread = std::move(ScopedThread(&ActiveObject::workerThread, this));
}

ActiveObject::~ActiveObject()
{

}

void ActiveObject::send(Task taskCallback, Task taskDoneCallback)
{
    mTasksQueue.emplace(std::make_pair(taskCallback, taskDoneCallback));
}

void ActiveObject::workerThread()
{
    for (;;)
    {
        std::pair<Task, Task> tasks;

        mTasksQueue.pop(tasks);

        if (tasks.first)
        {
            tasks.first();
            if (tasks.second)
            {
                tasks.second();
            }
        }
        else
        {
            return;
        }
    }
}

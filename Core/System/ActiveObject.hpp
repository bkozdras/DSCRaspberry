#pragma once

#include <functional>
#include <future>
#include <memory>
#include <queue>
#include <thread>
#include "Utilities/ScopedThread.hpp"
#include "Utilities/ThreadSafeQueue.hpp"

class ActiveObject
{
    public:

        typedef std::function<void()> Task;

        ActiveObject();
        ~ActiveObject();

        void send(Task taskCallback, Task taskDoneCallback = std::function<void()>());

        template<class F>
        static auto sendAsync(F function) -> std::future<decltype(function())>
        {
            using ResultType = decltype(function());
            auto task = std::make_shared< std::packaged_task<ResultType()> >(function);
            std::future<ResultType> result = task->get_future();
            send([task]() { (*task)(); });
            return result;
        }

    private:

        ThreadSafeSimpleQueue< std::pair<Task, Task> > mTasksQueue;
        ScopedThread mThread;

        void workerThread();
};
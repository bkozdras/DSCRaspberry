#pragma once

#include <functional>
#include <future>
#include <memory>
#include <tuple>
#include "Types/TaskPriority.hpp"
#include "Utilities/ScopedThread.hpp"
#include "Utilities/ThreadSafeQueue.hpp"

class ThreadPool
{
    public :

        typedef std::function<void()> Task;

        static void initialize(const unsigned int threadsCount = std::thread::hardware_concurrency());
        static void uninitialize();

        static void submit(TaskPriority priority, Task taskCallback, Task taskDoneCallback = std::function<void()>());

        template<class F>
        static auto submitAsync(TaskPriority priority, F function) -> std::future<decltype(function())>
        {
            using ResultType = decltype(function());
            auto task = std::make_shared< std::packaged_task<ResultType()> >(function);
            std::future<ResultType> result = task->get_future();
            submit(priority, [task]() { (*task)(); });
            return result;
        }

    private :

        typedef std::tuple<TaskPriority, Task, Task> ThreadTaskData;
        class ThreadTaskDataComparator
        {
            public :
                bool operator() (const ThreadTaskData & a, const ThreadTaskData & b)
                {
                    return Comparator::compare(std::get<0>(a), std::get<0>(b));
                }
        };

        static ThreadSafePriorityQueue< ThreadTaskData, std::priority_queue< ThreadTaskData, std::vector<ThreadTaskData>, ThreadTaskDataComparator > > mTasksQueue;
        static std::vector< ScopedThread > mThreads;

        static void workerThread();
};

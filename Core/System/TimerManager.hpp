#pragma once

#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <functional>
#include <chrono>
#include <unordered_map>
#include <set>
#include <cstdint>

class TimerManager
{
    public:

        typedef uint64_t TimerId;
        typedef std::function<void()> HandlerType;

    private:

        typedef std::unique_lock<std::mutex> ScopedLock;

        static std::mutex mSyncMutex;
        static std::condition_variable mCondVar;

    private:

        typedef std::chrono::steady_clock Clock;
        typedef std::chrono::time_point<Clock> Timestamp;
        typedef std::chrono::milliseconds Duration;

        struct Instance
        {
            Instance(TimerId id = 0) : id(id), running(false)
            {}

            template<typename Tfunction>
            Instance(TimerId id, Timestamp next, Duration period, Tfunction&& handler) noexcept
                : id(id),
                  next(next),
                  period(period),
                  handler(std::forward<Tfunction>(handler)),
                  running(false)
            {}

            Instance(const Instance &) = delete;

            Instance(Instance && r) noexcept
                : id(r.id),
                  next(r.next),
                  period(r.period),
                  handler(std::move(r.handler)),
                  running(r.running)
            {}

            Instance& operator=(const Instance & ) = delete;

            Instance& operator=(Instance && r)
            {
                if (this != &r)
                {
                    id = r.id;
                    next = r.next;
                    period = r.period;
                    handler = std::move(r.handler);
                    running = r.running;
                }
                return *this;
            }

            TimerId id;
            Timestamp next;
            Duration period;
            HandlerType handler;
            bool running;
        };

        typedef std::unordered_map<TimerId, Instance> InstanceMap;
        static TimerId nextId;
        static InstanceMap active;

        // Comparison functor to sort the timer "mQueue" by Instance::next
        struct NextActiveComparator
        {
            bool operator()(const Instance &a, const Instance &b) const
            {
                return a.next < b.next;
            }
        };
        static NextActiveComparator comparator;

        // Queue is a set of references to Instance objects, sorted by next
        typedef std::reference_wrapper<Instance> QueueValue;
        typedef std::multiset<QueueValue, NextActiveComparator> Queue;
        static std::unique_ptr<Queue> mQueue;

        // Thread and exit flag
        static std::thread mWorkerThread;
        static bool mIsThreadDone;

        static void threadStart();

    public:

        static void initialize();
        static void uninitialize();

        static TimerId create(uint64_t when, uint64_t period, const HandlerType & handler);
        static TimerId create(uint64_t when, uint64_t period, HandlerType && handler);

    private:

        static TimerId createImpl(Instance && item);

    public:

        static bool destroy(TimerId id);
        static bool exists(TimerId id);
};

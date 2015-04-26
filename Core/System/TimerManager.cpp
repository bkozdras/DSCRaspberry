#include "TimerManager.hpp"

void TimerManager::threadStart()
{
    ScopedLock lock(mSyncMutex);

    while (!mIsThreadDone)
    {
        if (mQueue->empty())
        {
            // Wait (forever) for work
            mCondVar.wait(lock);
        }
        else
        {
            auto firstInstance = mQueue->begin();
            Instance& instance = *firstInstance;
            auto now = Clock::now();
            if (now >= instance.next)
            {
                mQueue->erase(firstInstance);

                // Mark it as running to handle racing destroy
                instance.running = true;

                // Call the handler
                lock.unlock();
                instance.handler();
                lock.lock();

                if (mIsThreadDone)
                {
                    break;
                }
                else if (!instance.running)
                {
                    // Running was set to false, destroy was called
                    // for this Instance while the callback was in progress
                    // (this thread was not holding the lock during the callback)
                    active.erase(instance.id);
                }
                else
                {
                    instance.running = false;

                    // If it is periodic, schedule a new one
                    if (instance.period.count() > 0)
                    {
                        instance.next = instance.next + instance.period;
                        mQueue->insert(instance);
                    }
                    else {
                        active.erase(instance.id);
                    }
                }
            }
            else {
                // Wait until the timer is ready or a timer creation notifies
                mCondVar.wait_until(lock, instance.next);
            }
        }
    }
}

void TimerManager::initialize()
{
    nextId = 1;
    mQueue = std::make_unique<Queue>(comparator);
    mIsThreadDone = false;
    ScopedLock lock(mSyncMutex);
    mWorkerThread = std::thread(std::bind((void(*)())&TimerManager::threadStart));
}

void TimerManager::uninitialize()
{
    ScopedLock lock(mSyncMutex);
    mIsThreadDone = true;
    mCondVar.notify_all();
    lock.unlock();
    mWorkerThread.join();
}

TimerManager::TimerId TimerManager::create(uint64_t msFromNow, uint64_t msPeriod,
    const std::function<void()> &handler)
{
    return createImpl(Instance((TimerId)0,
        Clock::now() + Duration(msFromNow), Duration(msPeriod),
        handler));
}

TimerManager::TimerId TimerManager::create(uint64_t msFromNow, uint64_t msPeriod,
    std::function<void()>&& handler)
{
    return createImpl(Instance(0,
        Clock::now() + Duration(msFromNow), Duration(msPeriod),
        std::move(handler)));
}

TimerManager::TimerId TimerManager::createImpl(Instance&& item)
{
    ScopedLock lock(mSyncMutex);
    item.id = nextId++;
    auto iter = active.emplace(item.id, std::move(item));
    mQueue->insert(iter.first->second);
    mCondVar.notify_all();
    return item.id;
}

bool TimerManager::destroy(TimerId id)
{
    ScopedLock lock(mSyncMutex);
    auto i = active.find(id);
    if (i == active.end())
        return false;
    else if (i->second.running)
    {
        // A callback is in progress for this Instance,
        // so flag it for deletion in the mWorkerThread
        i->second.running = false;
    }
    else
    {
        mQueue->erase(std::ref(i->second));
        active.erase(i);
    }

    mCondVar.notify_all();
    return true;
}

bool TimerManager::exists(TimerId id)
{
    ScopedLock lock(mSyncMutex);
    return active.find(id) != active.end();
}

std::mutex TimerManager::mSyncMutex;
std::condition_variable TimerManager::mCondVar;
TimerManager::TimerId TimerManager::nextId;
TimerManager::InstanceMap TimerManager::active;
TimerManager::NextActiveComparator TimerManager::comparator;
std::unique_ptr<TimerManager::Queue> TimerManager::mQueue;
std::thread TimerManager::mWorkerThread;
bool TimerManager::mIsThreadDone;
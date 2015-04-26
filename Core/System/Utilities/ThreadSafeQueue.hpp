#pragma once

#include <mutex>
#include <queue>
#include <condition_variable>

template < class Item, class QueueType = std::queue<Item> >
class ThreadSafeQueueBase
{
    public:

        void clear()
        {
            std::lock_guard<std::mutex> lockGuard(mMutex);
            QueueType emptyQueue;
            std::swap(mQueue, emptyQueue);
        }

        void emplace(Item && elem)
        {
            std::lock_guard<std::mutex> lockGuard(mMutex);
            mQueue.emplace(elem);
            mCondVar.notify_one();
        }

        void pop(Item & elem)
        {
            std::unique_lock<std::mutex> uniqueLock(mMutex);
            mCondVar.wait
            (
                uniqueLock,
                [this]()
                {
                    return !mQueue.empty();
                }
            );
            elem = takeFirstElementFromQueue();
            mQueue.pop();
        }

        bool popNoWait(Item & elem)
        {
            std::lock_guard<std::mutex> lockGuard(mMutex);
            if (!mQueue.empty())
            {
                elem = takeFirstElementFromQueue();
                mQueue.pop();
                return true;
            }
            else
            {
                return false;
            }
        }

        void push(Item & elem)
        {
            std::lock_guard<std::mutex> lockGuard(mMutex);
            mQueue.push(elem);
            mCondVar.notify_one();
        }

        unsigned int size()
        {
            std::lock_guard<std::mutex> lockGuard(mMutex);
            return mQueue.size();
        }

    protected:

        QueueType mQueue;
        std::mutex mMutex;
        std::condition_variable mCondVar;

        virtual const Item & takeFirstElementFromQueue() = 0;
};

template < class Item, class QueueType = std::queue<Item> >
class ThreadSafeSimpleQueue : public ThreadSafeQueueBase<Item, QueueType>
{
    protected :

        const Item & takeFirstElementFromQueue() override
        {
            return this->template mQueue.front();
        }
};

template < class Item, class QueueType = std::priority_queue<Item> >
class ThreadSafePriorityQueue : public ThreadSafeQueueBase<Item, QueueType>
{
protected:

    const Item & takeFirstElementFromQueue() override
    {
        return this->template mQueue.top();
    }
};

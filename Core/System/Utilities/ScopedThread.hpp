#pragma once

#include <thread>

class ScopedThread
{
    public:

        template<class ThreadCallback, class... Arguments>
        explicit ScopedThread(ThreadCallback && threadCallback, Arguments && ... arguments)
            : mThread(threadCallback, std::forward<Arguments>(arguments) ...)
        {}

        ScopedThread(ScopedThread && rValue)
            : mThread(std::move(rValue.mThread))
        {}

        ~ScopedThread()
        {
            if (mThread.joinable())
            {
                mThread.join();
            }
        }

        ScopedThread & operator= (ScopedThread && thread)
        {
            mThread = std::move(thread.mThread);
            return *this;
        }

        std::thread & get()
        {
            return mThread;
        }

    private:

        std::thread mThread;

        ScopedThread(const ScopedThread &) = delete;
        ScopedThread & operator= (const ScopedThread &) = delete;
};

#pragma once

#include <map>

enum class TaskPriority
{
    Low,
    Normal,
    High,
    Realtime
};

namespace Comparator
{
    static bool compare(const TaskPriority a, const TaskPriority b)
    {
        static std::map< TaskPriority, int > priorityToInt =
        {
            { TaskPriority::Realtime, 0 },
            { TaskPriority::High, 1 },
            { TaskPriority::Normal, 2 },
            { TaskPriority::Low, 3 }
        };

        return (priorityToInt[a] < priorityToInt[b]);
    }
};

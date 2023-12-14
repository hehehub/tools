#pragma once
#include "Vector.h"
#include <string>
using namespace std;

class task
{
public:
    int level;
    string name;

    task() : level(0), name("") {}

    task(int _level, string _name) : level(_level), name(_name) {}

    bool operator<(task &another)
    {
        if (level < another.level)
            return true;
        else if (level == another.level)
            return name < another.name;
        else
            return false;
    }
};

class priorqueue
{
    Vector<task> queue;
    int numCounts;

    void percolateUp(int index)
    {
        task _task = queue[index];
        int parentIndex = (index - 1) / 2;
        while (index > 0 && _task < queue[parentIndex])
        {
            queue[index] = queue[parentIndex]; // ?
            index = parentIndex;
            parentIndex = (parentIndex - 1) / 2;
        }
        queue[index] = _task;
    }

    void percolateDown(int index) // 下滤
    {
        task _task = queue[index];
        int child = 2 * index + 1; // 左子节点
        while (child < numCounts)
        {
            // 选出左右子节点中较小的一个
            if (child + 1 < numCounts && queue[child + 1] < queue[child])
            {
                child++;
            }

            // 如果Task小于或等于较小的子节点，下滤结束
            if (_task < queue[child])
            {
                break;
            }

            // 否则，将较小的子节点向上移动
            queue[index] = queue[child];
            index = child;
            child = 2 * index + 1;
        }
        queue[index] = _task;
    }

    void build()
    {
        if (numCounts < 2)
            return;
        // 第一个需要调整的子树的根节点地址
        int parent = numCounts / 2 - 1;
        while (1)
        {
            percolateDown(parent);
            if (parent == 0)
                return; // 到达根节点，返回
            --parent;
        }
    }

public:
    priorqueue(Vector<task> _queue) : queue(_queue)
    {
        numCounts = queue.size();
        build();
    }

    void push(task _task)
    {
        queue.push_back(_task);
        numCounts++;
        percolateUp(numCounts - 1);
    }

    void pop()
    {
        if (numCounts <= 0)
            return; // 如果队列为空，则不执行任何操作

        if (numCounts == 1)
        {
            // 如果队列中只有一个元素，直接移除它
            queue.pop_back();
            numCounts--;
            return;
        }

        // 将最后一个元素移动到顶部
        queue[0] = queue[numCounts - 1];
        // 移除最后一个元素
        queue.pop_back();
        numCounts--;

        // 恢复堆的性质
        percolateDown(0);
    }

    task &top()
    {
        return queue[0];
    }

    int size() const
    {
        return numCounts;
    }

    bool empty()
    {
        return numCounts == 0;
    }
};
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
        if (name < another.name)
            return true;
        return false;
    }
};

class priorqueue
{
    Vector<task> queue;
    int numCounts;

    void percolateUp(int index, task _task)
    {
        int parentIndex = (index - 1) / 2;
        while (index > 0 && _task < queue[parentIndex]) //
        {
            queue[index] = queue[parentIndex];
            index = parentIndex;
            parentIndex = (parentIndex - 1) / 2;
        }
        queue[index] = _task;
    }

    void percolateDown(int childTree, task Task) // 下滤
    {
        int index = childTree;          // 目标节点索引
        int maxChild = 2 * (index + 1); // 目标节点右子节点
        bool godown = true;             // 循环下滤退出标志
        while (godown && maxChild < numCounts)
        {
            godown = false;
            if (queue[maxChild - 1] < queue[maxChild]) //
                --maxChild;                            // 若左孩子大，则更新为左孩子
            if (queue[maxChild] < Task)                //
            {                                          // 若父节点小于孩子，则下滤
                godown = true;
                queue[index] = queue[maxChild]; // 令较大值为交换值
                index = maxChild;               // 该交换节点索引下移
                maxChild = 2 * (maxChild + 1);  // 重新计算交换节点右子节点
            }
        }
        if (maxChild == numCounts)
        {
            if (queue[maxChild - 1] < Task)
            { // 令左子节点值为交换值
                queue[index] = queue[maxChild - 1];
                index = maxChild - 1;
            }
        }
        queue[index] = Task; // 将调整值赋予交换节点
    }

    void build()
    {
        if (numCounts < 2)
            return;
        // 第一个需要调整的子树的根节点地址
        int parent = numCounts / 2 - 1;
        while (1)
        {
            percolateDown(parent, queue[parent]);
            if (0 == parent)
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
        percolateUp(numCounts - 1, queue[numCounts - 1]);
    }

    void pop()
    {
        task _task = queue[numCounts - 1];
        queue[numCounts - 1] = queue[0];
        numCounts--;
        percolateDown(0, _task); // 下滤
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
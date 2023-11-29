#pragma once
#include <math.h> // std::max

#define DEFAULT_CAPACITY 10 // 默认的容量

template <class T>
class Queue
{
private:
    int _front;    // 队列头部索引
    int _size;     // 队列大小
    int _capacity; // 队列容量
    T *_elem;      // 动态数组

    // 扩展队列容量
    void expand()
    {
        if (_size < _capacity)
            return;
        _capacity = fmax(DEFAULT_CAPACITY, _capacity * 2);
        T *oldElem = _elem;
        _elem = new T[_capacity];
        for (int i = 0; i < _size; ++i)
        {
            _elem[(_front + i) % _capacity] = oldElem[(_front + i) % _capacity]; // 使用新容量
        }
        _front = 0; // 重置 _front
        delete[] oldElem;
    }

public:
    // 构造函数
    Queue(int cap = DEFAULT_CAPACITY) : _front(0), _size(0), _capacity(cap)
    {
        _elem = new T[_capacity];
    }

    // 入队
    void push(const T &e)
    {
        expand(); // 确保有足够的空间
        _elem[(_front + _size) % _capacity] = e;
        ++_size;
    }

    // 出队
    void pop()
    {
        if (_size == 0)
            return; // 防止空队列操作
        _front = (_front + 1) % _capacity;
        --_size;
    }

    // 获取队列头部元素
    T &front()
    {
        return _elem[_front];
    }

    // 获取队列元素数量
    int size() const
    {
        return _size;
    }

    // 检查队列是否为空
    bool empty() const
    {
        return _size == 0;
    }

    // 析构函数
    ~Queue()
    {
        delete[] _elem;
    }
};
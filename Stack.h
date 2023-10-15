#pragma once

#include <math.h>

#define DEFAULT_CAPACITY 3

template <class T>
class Stack
{
    int _size;
    int _capacity;
    T *_elem;

public:
    Stack(int s = 0, T v = 0)
    {
        _capacity = fmax(DEFAULT_CAPACITY, s);
        _elem = new T[_capacity];
        for (_size = 0; _size < s; _elem[_size++] = v)
            ;
    }

    void push()
    {
        if (_size >= _capacity)
        {
            expand();
        }
        _elem[_size++] = e;
    }

    void pop()
    {
        if (_size > 0)
        {
            _size--;
        }
    }

    T &top() const
    {
        return _elem[_size - 1];
    }
};
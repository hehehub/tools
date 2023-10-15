#pragma once

#include <math.h>

#define DEFAULT_CAPACITY 3

template <class T>
class Stack
{
    int _size;
    int _capacity;
    T *_elem;

    void expand()
    {
        if (_size < _capacity)
            return;
        _capacity = (_capacity < DEFAULT_CAPACITY) ? DEFAULT_CAPACITY : _capacity * 2;
        T *oldElem = _elem;
        _elem = new T[_capacity];
        for (int i = 0; i < _size; i++)
            _elem[i] = oldElem[i];
        delete[] oldElem;
    }

public:
    Stack(int s = 0, T v = 0)
    {
        _capacity = fmax(DEFAULT_CAPACITY, s);
        _elem = new T[_capacity];
        for (_size = 0; _size < s; _elem[_size++] = v)
            ;
    }

    void push(T const &e)
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

    int size() const
    {
        return _size;
    }

    ~Stack() { delete[] _elem; }
};
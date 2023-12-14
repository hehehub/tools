#pragma once
#include <cstdlib>
#include <utility>
#include <math.h>
#include <initializer_list>

#define DEFAULT_CAPACITY 3

template <class T>
class Vector
{
    long long inverse = 0;
    int _size;
    int _capacity;
    T *_elem;

    class Iterator
    {
        T *ptr;

    public:
        Iterator(T *ptr) : ptr(ptr) {}

        Iterator &operator++()
        {
            ++ptr;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator temp = *this;
            ++*this;
            return temp;
        }

        bool operator!=(const Iterator &other) const
        {
            return ptr != other.ptr;
        }

        bool operator==(const Iterator &other) const
        {
            return ptr == other.ptr;
        }

        T &operator*() const
        {
            return *ptr;
        }

        T *operator->() const
        {
            return ptr;
        }

        Iterator operator+(int n) const
        {
            return Iterator(ptr + n);
        }

        Iterator operator-(int n) const
        {
            return Iterator(ptr - n);
        }

        int operator-(const Iterator &other) const
        {
            return ptr - other.ptr;
        }

        static int distance(Iterator first, Iterator last)
        {
            return last - first; // 直接返回两个迭代器的差值
        }
    };

    void copyFrom(T const *A, int lo, int hi) // 以数组区间A[lo, hi)为蓝本复制向量
    {
        _elem = (T *)::operator new((_capacity = 2 * (hi - lo)) * sizeof(T)); // 使用placement new分配内存
        _size = 0;

        while (lo < hi)
        {
            new (_elem + _size) T(A[lo]); // 使用placement new在已分配的内存中构造对象
            ++_size;
            ++lo;
        }
    }

    void expand() // 向量空间不足时扩容
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

    void shrink() // 有必要时缩容
    {
        if (_size < _capacity / 2) // 判断是否需要缩容，这里我们假设当大小小于容量的一半时进行缩容
        {
            T *newElem = new T[_size];
            for (int i = 0; i < _size; i++)
            {
                newElem[i] = _elem[i];
            }

            delete[] _elem;
            _elem = newElem;

            _capacity = _size;
        }
    }

    static int comparePoints(const void *a, const void *b)
    {
        T va = *(const T *)a;
        T vb = *(const T *)b;
        return va - vb;
        // return *(T *)a - *(T *)b;
    }

public:
    Vector(std::initializer_list<T> list) : _size(0), _capacity(DEFAULT_CAPACITY)
    {
        if (list.size() > DEFAULT_CAPACITY)
            _capacity = list.size();
        _elem = new T[_capacity];
        for (const auto &elem : list)
        {
            if (_size >= _capacity)
            {
                expand(); // Expand the vector if necessary.
            }
            _elem[_size++] = elem;
        }
    }

    Vector(int s = 0)
    {
        _capacity = fmax(DEFAULT_CAPACITY, s);
        _elem = new T[_capacity];
        for (_size = 0; _size < s; _size++)
        {
            _elem[_size] = T(); // 使用 T 的默认构造函数
        }
    }

    // 构造函数，指定大小和默认值
    Vector(int s, T v)
    {
        _capacity = fmax(DEFAULT_CAPACITY, s);
        _elem = new T[_capacity];
        for (_size = 0; _size < s; _size++)
        {
            _elem[_size] = v;
        }
    }

    Vector(T const *A, int n) // 数组整体复制
    {
        copyFrom(A, 0, n);
    }

    Vector(const Vector<T> &V)
    {
        _elem = new T[V._capacity];
        _size = V._size;
        _capacity = V._capacity;
        for (int i = 0; i < _size; ++i)
        {
            _elem[i] = V._elem[i];
        }
    }

    // 移动构造函数
    Vector(Vector<T> &&V) noexcept
        : _size(V._size), _capacity(V._capacity), _elem(V._elem)
    {
        V._elem = nullptr;
        V._size = 0;
        V._capacity = 0;
    }

    // 拷贝赋值运算符
    Vector<T> &operator=(const Vector<T> &V)
    {
        if (this != &V)
        {
            delete[] _elem;
            _elem = new T[V._capacity];
            _size = V._size;
            _capacity = V._capacity;
            for (int i = 0; i < _size; ++i)
            {
                _elem[i] = V._elem[i];
            }
        }
        return *this;
    }

    // 移动赋值运算符
    Vector<T> &operator=(Vector<T> &&V) noexcept
    {
        if (this != &V)
        {
            delete[] _elem;
            _elem = V._elem;
            _size = V._size;
            _capacity = V._capacity;

            V._elem = nullptr;
            V._size = 0;
            V._capacity = 0;
        }
        return *this;
    }

    template <class Iter>
    Vector(Iter begin, Iter end)
    {
        _size = 0;
        _capacity = Iterator::distance(begin, end);
        _capacity = fmax(_capacity, DEFAULT_CAPACITY); // 确保_capacity至少为DEFAULT_CAPACITY
        _elem = new T[_capacity];
        for (Iter it = begin; it != end; ++it)
        {
            _elem[_size++] = *it;
        }
    }

    T &operator[](int r)
    {
        return _elem[r];
    }

    T const &operator[](int r) const
    {
        return _elem[r];
    }

    bool operator==(Vector<T> const &equal)
    {
        if (_size != equal.size())
            return false;
        for (int i = 0; i < _size; i++)
        {
            if (_elem[i] != equal[i])
                return false;
        }
        return true;
    }

    Iterator begin()
    {
        return Iterator(_elem);
    }

    Iterator end()
    {
        return Iterator(_elem + _size);
    }

    void push_back(T const &e)
    {
        if (_size >= _capacity)
        {
            expand(); // Expand the vector if necessary.
        }
        _elem[_size++] = e; // Add the element to the end of the vector.
    }

    void pop_back()
    {
        if (_size > 0)
        {
            _size--;
        }
    }

    T &back() const
    {
        return _elem[_size - 1];
    }

    int size() const
    {
        return _size;
    }

    bool empty()
    {
        return _size == 0;
    }

    ~Vector() { delete[] _elem; } // 释放内部空间
};
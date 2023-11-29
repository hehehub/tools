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
    };

    void copyFrom(T const *A, int lo, int hi) // 以数组区间A[lo, hi)为蓝本复制向量
    {
        // 分配空间，规模清零
        _elem = new T[_capacity = 2 * (hi - lo)];
        _size = 0;

        while (lo < hi) // A[lo, hi)内的元素逐一
        {
            _elem[_size++] = A[lo++]; // 复制至_elem[0, hi - lo)
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

    void merge(int lo, int mi, int hi, T *temp)
    {
        int i = lo, j = mi, k = 0;

        while (i < mi && j < hi)
        {
            if (_elem[i] <= _elem[j])
            {
                temp[k++] = _elem[i++];
            }
            else
            {
                inverse += mi - i;
                temp[k++] = _elem[j++];
            }
        }

        while (i < mi)
            temp[k++] = _elem[i++];
        while (j < hi)
            temp[k++] = _elem[j++];

        for (int i = 0, j = lo; j < hi;)
            _elem[j++] = temp[i++];
    }

    void mergeSortRecursively(int lo, int hi, T *temp) // 真正执行递归调用的函数，它采用外部传入的临时数组temp进行归并操作，而不是每次都新建一个
    {
        if (hi - lo < 2)
            return;
        int mi = (lo + hi) / 2;
        mergeSortRecursively(lo, mi, temp);
        mergeSortRecursively(mi, hi, temp);
        merge(lo, mi, hi, temp);
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

    Vector(int s = 0, T v = 0)
    {
        _capacity = fmax(DEFAULT_CAPACITY, s);
        _elem = new T[_capacity];
        for (_size = 0; _size < s; _elem[_size++] = v)
            ;
    }

    Vector(T const *A, int n) // 数组整体复制
    {
        copyFrom(A, 0, n);
    }

    Vector(const Vector<T> &V) // 拷贝构造
    {
        copyFrom(V._elem, 0, V._size);
    }

    Vector(Vector<T> &&V) noexcept // 移动构造
    {
        _elem = V._elem;         // 接管V的内部数组
        _size = V._size;         // 接管V的大小
        _capacity = V._capacity; // 接管V的容量

        V._elem = nullptr; // 确保V不再指向内部数组
        V._size = 0;       // 将V的大小设为0
        V._capacity = 0;   // 将V的容量设为0
    }

    Vector<T> &operator=(Vector<T> const &V) // 拷贝
    {
        if (_elem)
        {
            delete[] _elem; // 释放原有内容
        }
        copyFrom(V._elem, 0, V.size()); // 整体复制
        return *this;                   // 返回当前对象的引用，以便链式赋值
    }

    Vector<T> &operator=(Vector<T> &&V) noexcept // 移动
    {
        if (this != &V) // 防止自赋值
        {
            delete[] _elem;  // 释放当前对象的资源
            _elem = V._elem; // 接管V的资源
            _size = V._size;
            _capacity = V._capacity;

            V._elem = nullptr;
            V._size = 0;
            V._capacity = 0;
        }
        return *this;
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

    int erase(int position)
    {
        if (position < 0 || position >= _size)
        {
            // 位置无效，返回0表示没有元素被删除
            return 0;
        }

        for (int i = position; i < _size - 1; i++)
        {
            // 将后续元素逐一前移覆盖当前位置的元素
            _elem[i] = _elem[i + 1];
        }

        // 更新向量的大小
        _size--;

        return 1; // 返回1表示成功删除了一个元素
    }

    int insert(int r, T const &e)
    {
        expand(); // 若有必要，扩容
        for (int i = _size; i > r; i--)
        {
            _elem[i] = _elem[i - 1]; // 自后向前，后继元素顺次后移一个单元
        }
        _elem[r] = e;
        _size++;  // 置入新元素并更新容量
        return r; // 返回秩
    }

    int remove(int lo, int hi) // 删除区间[lo, hi)
    {
        if (lo < 0 || lo >= _size || hi < lo || hi > _size)
            return 0;

        if (lo == hi)
            return 0;

        while (hi < _size)
        {
            _elem[lo++] = _elem[hi++]; // [hi, _size)顺次前移hi - lo个单元
        }
        _size = lo; // 更新规模，直接丢弃尾部[lo, _size = hi)区间
        shrink();
        return hi - lo; // 返回被删除元素的数目
    }

    void sort()
    {
        qsort(_elem, _size, sizeof(T), comparePoints);
    }

    int deduplicate() // 去重
    {
        int oldSize = _size;             // 记录原规模
        int i = 1;                       // 从_elem[1]开始
        while (i < _size)                // 自前向后逐一考查各元素_elem[i]
            (find(_elem[i], 0, i) < 0) ? // 在其前缀中找与之雷同者（至多一个）
                i++
                                       : remove(i, i + 1); // 若无雷同则继续考查其后继，否则删除雷同者
        return oldSize - _size;                            // 向量规模变化量，即被删除元素总数
    }

    int find(T const &e, int lo, int hi) const // 无序向量的顺序查找：返回最后一个元素e的位置；失败时，返回lo - 1
    {
        // assert: 0 <= lo < hi <= _size
        while ((lo < hi--) && (e != _elem[hi]))
            ;      // 从后向前，顺序查找
        return hi; // 若hi < lo，则意味着失败；否则hi即命中元素的秩
    }

    void permute(Vector<T> &V) // 置乱器
    {
        for (int i = V.size(); i > 0; i--) // 自后向前
        {
            std::swap(V[i - 1], V[rand() % i]); // V[i - 1]与V[0, i)中某一随机元素交换
        }
    }

    void mergeSort(int lo, int hi)
    {
        if (hi - lo < 2)
            return;
        T *temp = new T[hi - lo]; // 这里的temp传入mergeSortRecursively函数，只分配一次临时内存，而不是总新分配B
        mergeSortRecursively(lo, hi, temp);
        delete[] temp;
    }

    long long getinv()
    {
        return inverse;
    }

    int binSearch(T const &e)
    {
        int lo = 0, hi = _size;
        while (lo < hi)
        {                                            // 每步迭代仅需做一次比较判断，有两个分支
            int mi = (lo + hi) >> 1;                 // 以中点为轴点
            (e < _elem[mi]) ? hi = mi : lo = mi + 1; // 经比较后确定深入[lo, mi)或(mi, hi)
        }                                            // 成功查找不能提前终止
        return --lo;                                 // 循环结束时，lo为大于e的元素的最小秩，故lo - 1即不大于e的元素的最大秩
    }                                                // 有多个命中元素时，总能保证返回秩最大者；查找失败时，能够返回失败的位置

    ~Vector() { delete[] _elem; } // 释放内部空间
};
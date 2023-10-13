#pragma once

#include <cstdlib>
#include <utility>
#include <math.h>
#include <initializer_list>

#define DEFAULT_CAPACITY 3

template <class T>
class Vector
{
protected:
    int _size;
    int _capacity;
    T *_elem;

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
            return; // 尚未满员时，不必扩容
        if (_capacity < DEFAULT_CAPACITY)
        {
            _capacity = DEFAULT_CAPACITY; // 不低于最小容量
        }
        T *oldElem = _elem;
        _elem = new T[_capacity <<= 1]; // 容量加倍
        for (int i = 0; i < _size; i++)
        {
            _elem[i] = oldElem[i]; // 复制原向量内容（T为基本类型，或已重载赋值操作符'='）
        }
        delete[] oldElem; // 释放原空间
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

    void merge(int lo, int mi, int hi)
    {
        T *A = _elem + lo; // 合并后的向量A[0, hi - lo) = _elem[lo, hi)
        int lb = mi - lo;
        T *B = new T[lb]; // 前子向量B[0, lb) = _elem[lo, mi)
        for (int i = 0; i < lb; B[i] = A[i++])
            ; // 复制前子向量
        int lc = hi - mi;
        T *C = _elem + mi; // 后子向量C[0, lc) = _elem[mi, hi)
        for (int i = 0, j = 0, k = 0; (j < lb) || (k < lc);)
        {
            // B[j]和C[k]中的小者续至A末尾
            if ((j < lb) && (!(k < lc) || (B[j] <= C[k])))
                A[i++] = B[j++];
            if ((k < lc) && (!(j < lb) || (C[k] < B[j])))
                A[i++] = C[k++];
        }
        delete[] B; // 释放临时空间B
    }

public:
    Vector(std::initializer_list<T> list) : _size(0), _capacity(DEFAULT_CAPACITY)
    {
        _elem = new T[_capacity];
        for (const auto &elem : list)
        {
            expand();
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

    Vector(T const *A, int n) { copyFrom(A, 0, n); } // 数组整体复制

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

    T &operator[](int r) const
    {
        return _elem[r];
    }

    void push_back(T const &e)
    {
        if (_size >= _capacity)
        {
            expand(); // Expand the vector if necessary.
        }
        _elem[_size++] = e; // Add the element to the end of the vector.
    }

    int size()
    {
        return _size;
    }

    bool empty()
    {
        return _size == 0;
    }

    void pop_back()
    {
        if (_size > 0)
        {
            _size--;
        }
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
            return;             // 单元素区间自然有序，否则...
        int mi = (lo + hi) / 2; // 以中点为界
        mergeSort(lo, mi);
        mergeSort(mi, hi); // 分别排序
        merge(lo, mi, hi); // 归并
    }

    ~Vector() { delete[] _elem; } // 释放内部空间
};
// 列表节点模板类（以双向链表形式实现）
#pragma once

template <typename T>
struct ListNode
{
    T data;
    ListNode<T> *pred;
    ListNode<T> *succ; // 数值、前驱、后继
    // 构造函数
    ListNode() {} // 针对header和trailer的构造
    ListNode(T e, ListNode<T> *p = NULL, ListNode<T> *s = NULL)
        : data(e), pred(p), succ(s) {} // 默认构造器

    // 操作接口
    ListNode<T> *insertAsPred(T const &e) // 紧靠当前节点之前插入新节点
    {
        ListNode<T> *x = new ListNode(e, pred, this); // 创建新节点
        pred->succ = x;
        pred = x; // 设置正向链接
        return x; // 返回新节点的位置
    }

    ListNode<T> *insertAsSucc(T const &e) // 紧随当前节点之后插入新节点
    {
    }
};

template <typename T>
class List
{
private:
    // 规模、头哨兵、尾哨兵
    int _size;
    ListNode<T> *header;
    ListNode<T> *trailer;

protected:
    void init() // 列表创建时的初始化
    {
        header = new ListNode<T>;  // 创建头哨兵节点
        trailer = new ListNode<T>; // 创建尾哨兵节点
        header->succ = trailer;
        header->pred = NULL;
        trailer->pred = header;
        trailer->succ = NULL;
        _size = 0; // 记录规模
    }

    int clear() // 清除所有节点
    {
    }

    void copyNodes(ListNode<T> *p, int n) // 复制列表中自位置p起的n项
    {
    }

    void merge(ListNode<T> *&p, int n, List<T> &L, ListNode<T> *q, int m)
    {
        ListNode<T> *pp = p->pred;
        // 借助前驱（可能是header），以便返回前 ...
        while (0 < m) // 在q尚未移出区间之前
            if ((0 < n) && (p->data <= q->data))
            // 若p仍在区间内且v(p) <= v(q)，则
            {
                if (q == (p = p->succ))
                    break;
                n--;
            }
            // p归入合并的列表，并替换为其直接后继
            else // 若p已超出右界或v(q) < v(p)，则
            {
                insertB(p, L.remove((q = q->succ)->pred));
                m--;
            }
        // 将q转移至p之前
        p = pp->succ; // 确定归并后区间的（新）起点
    }

public:
    T &List<T>::operator[](int r) const
    {
        ListNode<T> *p = first(); // 从首节点出发
        while (0 < r--)
            p = p->succ; // 顺数第r个节点即是
        return p->data;  // 目标节点，返回其中所存元素
    }

    // 构造函数
    List() { init(); } // 默认

    List(List<T> const &L) // 整体复制列表L
    {
    }

    List(List<T> const &L, int r, int n) // 复制列表L中自第r项起的n项
    {
    }

    List(ListNode<T> *p, int n) // 复制列表中自位置p起的n项
    {
    }

    ListNode<T> *insertAsFirst(T const &e) // e当作首节点插入
    {
        _size++;
        return header->insertAsSucc(e);
    }

    ListNode<T> *insertAsLast(T const &e) // e当作末节点插入
    {
        _size++;
        return trailer->insertAsPred(e);
    }

    ListNode<T> *insertA(ListNode<T> *p, T const &e) // e当作p的后继插入（After）
    {
        _size++;
        return p->insertAsSucc(e);
    }

    ListNode<T> *insertB(ListNode<T> *p, T const &e) // e当作p的前驱插入（Before）
    {
        _size++;
        return p->insertAsPred(e);
    }

    T remove(ListNode<T> *p)
    {
        T e = p->data; // 备份待删除节点的数值（假定T类型可直接赋值）
        p->pred->succ = p->succ;
        p->succ->pred = p->pred; // 后继、前驱
        delete p;
        _size--;  // 释放节点，更新规模
        return e; // 返回备份的数值
    }

    // 在无序列表内节点p（可能是trailer）的n个（真）前驱中，找到等于e的最后者
    ListNode<T> *find(T const &e, int n, ListNode<T> *p) const
    {
        while (0 < n--) // 对于p的最近的n个前驱，从右向左
            if (e == (p = p->pred)->data)
                return p; // 逐个比对直至命中或越界
        return NULL;      // p越出左边界意味着区间内不含e，查找失败
    }                     // 失败时，返回NULL

    ListNode<T> *search(T const &e, int n, ListNode<T> *p) const
    {
        while (0 <= n--) // 对于p的最近的n个前驱，从右向左逐个比较
            if (((p = p->pred)->data) <= e)
                break; // assert: 至此位置p必符合输出语义约定——尽管此前最后一次关键码比较可能没有意义（等效于与-inf比较）
        return p;      // 返回查找终止的位置
    }                  // 失败时，返回区间左边界的前驱（可能是header),可通过valid()判断成功与否

    int List<T>::uniquify()
    { // 成批剔除重复元素，效率更高
        if (_size < 2)
            return 0;        // 平凡列表自然无重复
        int oldSize = _size; // 记录原规模
        ListNode<T> *p = first();
        ListNode<T> *q;                  // p为各区段起点，q为其后继
        while (trailer != (q = p->succ)) // 反复考查紧邻的节点对(p, q)
            if (p->data != q->data)
                p = q; // 若互异，则转向下一区段
            else
                remove(q);      // 否则（雷同），删除后者
        return oldSize - _size; // 列表规模变化量，即被删除元素总数
    }

    void insertionSort(ListNode<T> *p, int n) // 插入排序：对起始于位置p的n个元素排序
    {
        for (int r = 0; r < n; r++)
        { // 逐一为各节点
            insertA(search(p->data, r, p), p->data);
            // 查找适当的位置并插入
            p = p->succ;
            remove(p->pred); // 转向下一节点
        }
    }

    void selectionSort(ListNode<T> *p, int n) // 选择排序：对起始于位置p的n个元素排序
    {
        ListNode<T> *head = p->pred;
        ListNode<T> *tail = p;
        for (int i = 0; i < n; i++)
            tail = tail->succ;
        // 待排序区间为(head, tail)
        while (1 < n)
        { // 在至少还剩两个节点之前，在待排序区间内
            ListNode<T> *max = selectMax(head->succ, n);
            // 找出最大者（歧义时后者优先）
            insertB(tail, remove(max));
            // 将其移至无序区间末尾（作为有序区间新的首元素）
            tail = tail->pred;
            n--;
        }
    }

    void mergeSort(ListNode<T> *&p, int n) // 归并排序：对起始于位置p的n个元素排序
    {
        if (n < 2)
            return;     // 若待排序范围已足够小，则直接返回；否则...
        int m = n >> 1; // 以中点为界
        ListNode<T> *q = p;
        for (int i = 0; i < m; i++)
            q = q->succ; // 均分列表
        mergeSort(p, m);
        mergeSort(q, n - m);          // 对前、后子列表分别排序
        merge(p, m, *this, q, n - m); // 归并
    }                                 // 注意：排序后，p依然指向归并后区间的（新）起点

    // 析构函数
    ~List() // 释放（包含头、尾哨兵在内的）所有节点
    {
        int oldSize = _size;
        while (_size > 0)
            remove(header->succ);
        // 反复删除首节点，直至列表变空
        delete header;
        delete trailer;
    }
};
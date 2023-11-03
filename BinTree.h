#pragma once
#include "Queue.h"
#define stature(p) ((p) ? (p)->height : -1)
// 节点高度（宏定义封装，使得实现简便，
// 与“空树高度为-1”的约定相统一）

template <class T>
struct BinNode
{
    T data = 0;         // 数值
    BinNode<T> *parent; // 父节点
    BinNode<T> *lc;     // 左、右孩子
    BinNode<T> *rc;
    int height; // 高度

    BinNode(T const &e, BinNode<T> *p = nullptr)
        : data(e), parent(p), lc(nullptr), rc(nullptr), height(0) {}

    BinNode<T> *insertAsLC(T const &e) // 将e作为当前节点的左孩子插入二叉树
    {
        return lc = new BinNode(e, this);
    }

    BinNode<T> *insertAsRC(T const &e) // 作为当前节点的右孩子插入新节点
    {
        return rc = new BinNode(e, this);
    }

    BinNode<T> *succ() // 当前节点直接后继
    {
        return nullptr;
    }

    bool HasLChild()
    {
        return this->lc != nullptr;
    }

    bool HasRChild()
    {
        return this->rc != nullptr;
    }

    void visit(T data)
    {
    }

    template <class VST>
    void travLevel(VST &visit) // 二叉树层次遍历算法
    {
        Queue<BinNode<T> *> Q; // 辅助队列
        Q.push(this);          // 根节点入队
        while (!Q.empty())     // 在队列再次变空之前，反复迭代
        {
            BinNode<T> *x = Q.front();
            Q.pop();
            visit(x->data); // 取出队首节点并访问之
            if (x->HasLChild())
                Q.push(x->lc); // 左孩子入队
            if (x->HasRChild())
                Q.push(x->rc); // 右孩子入队
        }
    }

    int size() // 以当前节点为根的子树规模
    {
        int s = 1; // 计入本身
        if (lc)
        {
            s += lc->size(); // 递归计入左子树规模
        }
        if (rc)
        {
            s += rc->size(); // 递归计入右子树规模
        }
        return s;
    }

    int getHeight()
    {
        if (!this)
            return -1;
        else
            return 1 + fmax(height(lc), height(rc));
    }
};

template <class T, class VST>
class BinTree
{
    int _size;         // 规模
    int current;       // 对吗？
    BinNode<T> *_root; // 根节点
    T a[4000000];

    virtual int updateHeight(BinNode<T> *x) // 更新节点x高度
    {
        return x->height = 1 + max(stature(x->lc), stature(x->rc));
    }

    void updateHeightAbove(BinNode<T> *x) // 更新节点x及其祖先高度
    {
        while (x) // 从x出发，覆盖历代祖先
        {
            updateHeight(x);
            x = x->parent;
        }
    }

    static int removeAt(BinNode<T> *x) // 释放被摘除节点，并返回删除节点总数
    {
        if (!x)
            return 0;                                  // 递归基：空树
        int n = 1 + removeAt(x->lc) + removeAt(x->rc); // 递归释放左右子树
        delete (x);
        return n;
    }

    void visit(T data)
    {
        if (data)
        {
            a[current] = data;
            current++;
        }
    }

public:
    BinTree() : _size(0), current(0)
    {
        _root = new BinNode<T>; // 创建一个空的根节点
    }

    int size() const { return _size; } // 规模

    bool empty() const { return !_root; } // 判空

    BinNode<T> *root() const { return _root; } // 树根

    BinNode<T> *insertAsLC(BinNode<T> *x, T const &e) // e插入为x的左孩子
    {
        _size++;
        x->insertAsLC(e);
        updateHeightAbove(x);
        return x->lc;
    }

    BinNode<T> *insertAsRC(BinNode<T> *x, T const &e) // e插入为x的左孩子
    {
        _size++;
        x->insertAsRC(e);
        updateHeightAbove(x);
        return x->rc;
    }

    void travPre(BinNode<T> *x, VST &visit) // 前序遍历
    {
        if (x == nullptr)
            return;
        visit(x->data);
        travPre(x->lc, visit);
        travPre(x->rc, visit);
    }

    void travIn_R(BinNode<T> *x, VST &visit) // 中序遍历
    {
        if (x == nullptr)
            return;
        travIn_R(x->lc, visit);
        visit(x->data);
        travIn_R(x->rc, visit);
    }

    void travPost_R(BinNode<T> *x, VST &visit) // 后序遍历
    {
        if (x == nullptr)
            return;
        travPost_R(x->lc, visit);
        travPost_R(x->rc, visit);
        visit(x->data);
    }

    int height(BinNode<T> *x)
    {
        if (!x)
            return -1;
        else
            return 1 + max(height(x->lc), height(x->rc));
    }

    int remove(BinNode<T> *x) // 删除子树x，更新规模，返回删除节点总数
    {
        if (x->parent) // 切断来自父结点的指针
        {
            if (x->parent->lc == x)
                x->parent->lc = nullptr;
            if (x->parent->rc == x)
                x->parent->rc = nullptr;
        }
        updateHeightAbove(x->parent); // 更新祖先高度
        int n = removeAt(x);
        _size -= n;
        return n;
    }

    void print()
    {
        for (int i = 0; i < current; i++)
        {
            printf("%d ", a[i]);
        }
    }

    ~BinTree() // 析构函数
    {
        if (0 < _size)
            remove(_root);
    }
};
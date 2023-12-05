#pragma once

template <class T>
struct Node
{
    T data;
    Node<T> *next;
};

template <class T>
class Queue
{
private:
    int _size;
    Node<T> *head;
    Node<T> *tail;

public:
    Queue() : _size(0)
    {
        head = new Node<T>;
        head->next = nullptr;
        tail = head;
    }

    void push(const T &_data)
    {
        _size++;
        auto node = new Node<T>;
        node->data = _data;
        tail->next = node;
        tail = node;
        node->next = nullptr;
    }

    void pop()
    {
        _size--;
        if (head->next == tail)
        {
            delete tail;
            tail = head;
        }
        else
        {
            Node<T> *p = head->next->next;
            delete head->next;
            head->next = p;
        }
    }

    void clear()
    {
        for (int i = 0; i < _size; i++)
            pop();
    }

    int size()
    {
        return _size;
    }

    bool empty()
    {
        return _size == 0;
    }

    T &front()
    {
        return head->next->data;
    }

    ~Queue()
    {
        clear();
        delete head;
    }
};
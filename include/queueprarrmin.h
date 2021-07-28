#pragma once
#ifndef ENCODERDECODER_QUEUEPRARRMIN_H
#define ENCODERDECODER_QUEUEPRARRMIN_H

#include <node.h>

#include <utility>

/**
\brief �������� ������, ������������ ������� � ����������� (���������� �� ���. ����) �� �������� ����
*/

class QueuePrArrMin {
public:
    typedef std::pair<int, Node*> KeyValType;
public:
    QueuePrArrMin();
    QueuePrArrMin(const QueuePrArrMin& q);
    QueuePrArrMin& operator=(const QueuePrArrMin& q);
    ~QueuePrArrMin() {
        while (!isEmpty()) {
            pop();
        }
    }
    /// ���������� true, ������ ���� ������� �����
    bool isEmpty() const noexcept;
    /// �������� � ������� ���� (std::pair<int, float>) ����-�������� �� O(log(n))
    void push(const KeyValType& kv);
    /// ��������� ������� � ����������� ������ �� O(log(n))
    void pop() noexcept;
    /// ���������� ������ �� ������� � ����������� ������ �� O(1)
    const KeyValType& top() const;
private:
    void resize(size_t sz);

    void siftUp(size_t i);
    void siftDown(size_t i);

    KeyValType* data_;
    size_t capacity_;
    size_t size_;
};

#endif // #ifndef ENCODERDECODER_QUEUEPRARRMIN_H
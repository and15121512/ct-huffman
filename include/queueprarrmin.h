#pragma once
#ifndef ENCODERDECODER_QUEUEPRARRMIN_H
#define ENCODERDECODER_QUEUEPRARRMIN_H

#include <node.h>

#include <utility>

/**
\brief Описание класса, реализующего очередь с приоритетом (извлечение на мин. ключ) на двоичной куче
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
    /// Возвращает true, только если очередь пуста
    bool isEmpty() const noexcept;
    /// Помещает в очередь пару (std::pair<int, float>) ключ-значение за O(log(n))
    void push(const KeyValType& kv);
    /// Извлекает элемент с минимальным ключом за O(log(n))
    void pop() noexcept;
    /// Возвращает ссылку на элемент с минимальным ключом за O(1)
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
#include <queueprarrmin.h>

#include <stdexcept>

void QueuePrArrMin::siftUp(size_t i) {
    _ASSERT(i < size_);
    for (size_t curr = i;
        (curr != 0) && (data_[curr].first < data_[(curr - 1) / 2].first);
        curr = (curr - 1) / 2) {
        std::swap(data_[curr], data_[(curr - 1) / 2]);
    }
}

void QueuePrArrMin::siftDown(size_t i) {
    _ASSERT(i < size_);
    for (size_t c = i; ; ) {
        size_t to_swap = c;
        size_t l = 2 * c + 1;
        size_t r = 2 * c + 2;
        if (l < size_ && data_[l].first < data_[c].first) {
            to_swap = l;
        }
        if (r < size_ && data_[r].first < data_[to_swap].first) {
            to_swap = r;
        }
        if (c != to_swap) {
            std::swap(data_[c], data_[to_swap]);
            c = to_swap;
        }
        else {
            break;
        }
    }
}

bool QueuePrArrMin::isEmpty() const noexcept {
    return 0 == size_;
}

void QueuePrArrMin::resize(size_t new_cap) {
    _ASSERT(nullptr != data_);
    if (new_cap <= capacity_) {
        return;
    }

    KeyValType* new_data_ = new KeyValType[new_cap];
    for (size_t i = 0; i < capacity_; ++i) {
        new_data_[i] = data_[i];
    }
    delete[] data_;
    data_ = new_data_;
    capacity_ = new_cap;
}

void QueuePrArrMin::push(const KeyValType& kv) {
    _ASSERT(nullptr != data_);
    if (size_ == capacity_) {
        resize(2 * capacity_);
    }
    data_[size_] = kv;
    ++size_;

    siftUp(size_ - 1);
}

void QueuePrArrMin::pop() noexcept {
    _ASSERT(nullptr != data_);
    if (size_ == 0) {
        return;
    }
    --size_;
    if (size_ > 0) {
        data_[0] = data_[size_];
        siftDown(0);
    }
}

const QueuePrArrMin::KeyValType& QueuePrArrMin::top() const {
    _ASSERT(nullptr != data_);
    if (size_ == 0) {
        throw std::exception("QueuePrBinHeap::top(): queue is empty");
    }
    return data_[0];
}

QueuePrArrMin& QueuePrArrMin::operator=(const QueuePrArrMin& q) {
    _ASSERT(nullptr != q.data_ && nullptr != data_);
    if (this == &q) {
        return *this;
    }
    if (q.size_ > capacity_) {
        resize(q.capacity_);
    }
    for (size_t i = 0; i < q.size_; ++i) {
        data_[i] = q.data_[i];
    }
    size_ = q.size_;
    return *this;
}

QueuePrArrMin::QueuePrArrMin() {
    const size_t INIT_CAP = 5;
    data_ = new KeyValType[INIT_CAP];
    capacity_ = INIT_CAP;
    size_ = 0;
}

QueuePrArrMin::QueuePrArrMin(const QueuePrArrMin& q) {
    const size_t INIT_CAP = 5;
    data_ = new KeyValType[INIT_CAP];
    capacity_ = INIT_CAP;
    size_ = 0;
    resize(q.capacity_);
    for (size_t i = 0; i < q.size_; ++i) {
        data_[i] = q.data_[i];
    }
    size_ = q.size_;
}

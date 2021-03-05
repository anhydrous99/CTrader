//
// Created by Armando Herrera on 3/1/21.
//

#ifndef CTRADER_MAXQUEUE_H
#define CTRADER_MAXQUEUE_H

#include <deque>
#include <shared_mutex>

template<typename T>
class MaxQueue {
    std::deque<T> queue;
    std::shared_mutex mut;

public:
    typedef typename std::deque<T>::iterator iterator;
    typedef typename std::deque<T>::const_iterator const_iterator;
    typedef typename std::deque<T>::value_type value_type;
    typedef typename std::deque<T>::size_type size_type;
    typedef typename std::deque<T>::reference reference;
    typedef typename std::deque<T>::const_reference const_reference;

    explicit MaxQueue(size_type n);

    void push(const value_type& value);
    void push(value_type&& value);
    void clear();
    value_type pop();
    size_type size();
    const_reference front();
    const_reference back();

    reference operator[](size_type pos);
    const_reference operator[](size_type pos) const;

private:
    size_type max_size;
};

template<typename T>
void MaxQueue<T>::push(const MaxQueue::value_type &value) {
    if (queue.size() > max_size)
        queue.pop_front();
    {
        std::unique_lock lock(mut);
        queue.push_back(value);
    }
}

template<typename T>
void MaxQueue<T>::push(MaxQueue::value_type &&value) {
    if (queue.size() > max_size)
        queue.pop_front();
    {
        std::unique_lock lock(mut);
        queue.push_back(value);
    }
}

template<typename T>
MaxQueue<T>::MaxQueue(MaxQueue::size_type n) : max_size(n) {}

template<typename T>
typename MaxQueue<T>::value_type MaxQueue<T>::pop() {
    value_type v = queue.front();
    {
        std::unique_lock lock(mut);
        queue.pop_front();
    }
    return v;
}

template<typename T>
typename MaxQueue<T>::size_type MaxQueue<T>::size() {
    std::shared_lock lock(mut);
    return queue.size();
}

template<typename T>
typename MaxQueue<T>::const_reference MaxQueue<T>::front() {
    std::shared_lock lock(mut);
    return queue.front();
}

template<typename T>
typename MaxQueue<T>::const_reference MaxQueue<T>::back() {
    std::shared_lock lock(mut);
    return queue.back();
}

template<typename T>
typename MaxQueue<T>::reference MaxQueue<T>::operator[](MaxQueue::size_type pos) {
    std::unique_lock lock(mut);
    return queue[pos];
}

template<typename T>
typename MaxQueue<T>::const_reference MaxQueue<T>::operator[](MaxQueue::size_type pos) const {
    std::shared_lock lock(mut);
    return queue[pos];
}

template<typename T>
void MaxQueue<T>::clear() {
    std::unique_lock lock(mut);
    queue.clear();
}

#endif //CTRADER_MAXQUEUE_H

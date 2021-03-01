//
// Created by Armando Herrera on 3/1/21.
//

#ifndef CTRADER_MAXQUEUE_H
#define CTRADER_MAXQUEUE_H

#include <queue>
#include <deque>

template<typename T>
class SuperQueue : public std::queue<T, Container> {
public:
    typedef typename Container::iterator iterator;
    typedef typename Container::const_iterator const_iterator;

    iterator begin() { return c.begin(); }
    iterator end() { return c.end(); }
    const_iterator cbegin() { return c.cbegin(); }
    const_iterator cend() { return c.cend(); }
};

template<typename T>
class MaxQueue {
    SuperQueue<T> queue;

public:
    typedef typename SuperQueue<T>::iterator iterator;
    typedef typename SuperQueue<T>::const_iterator const_iterator;
    typedef typename SuperQueue<T>::value_type value_type;
    typedef typename SuperQueue<T>::size_type size_type;
    typedef typename SuperQueue<T>::reference reference;
    typedef typename SuperQueue<T>::const_reference const_reference;

    explicit MaxQueue(size_type n);

    iterator begin() { return queue.begin(); }
    iterator end() { return queue.end(); }
    const_iterator cbegin() { return queue.cbegin(); }
    const_iterator cend() { return queue.cend(); }

    void push(const value_type& value);
    void push(value_type&& value);
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
    if (queue.size() >= max_size)
        queue.pop();
    queue.push(value);
}

template<typename T>
void MaxQueue<T>::push(MaxQueue::value_type &&value) {
    if (queue.size() >= max_size)
        queue.pop();
    queue.push(value);
}

template<typename T>
MaxQueue<T>::MaxQueue(MaxQueue::size_type n) : max_size(n) {}

template<typename T>
MaxQueue::value_type MaxQueue<T>::pop() {
    value_type v = queue.front();
    queue.pop();
    return v;
}

template<typename T>
MaxQueue::size_type MaxQueue<T>::size() {
    return queue.size();
}

template<typename T>
MaxQueue::const_reference MaxQueue<T>::front() {
    return queue.front();
}

template<typename T>
MaxQueue::const_reference MaxQueue<T>::back() {
    return queue.back();
}

template<typename T>
MaxQueue::reference MaxQueue<T>::operator[](MaxQueue::size_type pos) {
    return *(begin() + pos);
}

template<typename T>
MaxQueue::const_reference MaxQueue<T>::operator[](MaxQueue::size_type pos) const {
    return *(begin() + pos);
}

#endif //CTRADER_MAXQUEUE_H

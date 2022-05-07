//
// Created by niels on 5/7/22.
//

#ifndef STOCK_EXCHANGE_ITERABLEQUEUE_H
#define STOCK_EXCHANGE_ITERABLEQUEUE_H

#include <deque>
#include "queue"

template<typename T, typename Container=std::deque<T> >
class IterableQueue : public std::queue<T,Container>
{
public:
    typename Container::iterator begin() { return this->c.begin(); }
    typename Container::iterator end() { return this->c.end(); }
    typename Container::const_iterator begin() const { return this->c.begin(); }
    typename Container::const_iterator end() const { return this->c.end(); }
};


#endif //STOCK_EXCHANGE_ITERABLEQUEUE_H

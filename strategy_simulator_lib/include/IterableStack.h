#ifndef STOCK_EXCHANGE_ITERABLESTACK_H
#define STOCK_EXCHANGE_ITERABLESTACK_H

#include <stack>
#include <stdexcept>

template<typename T, typename Container=std::deque<T> >
class IterableStack : public std::stack<T,Container>
{
public:
    typename Container::iterator begin() { return this->c.begin(); }
    typename Container::iterator end() { return this->c.end(); }
    typename Container::const_iterator begin() const { return this->c.begin(); }
    typename Container::const_iterator end() const { return this->c.end(); }

    T& operator[](int index) {
        if (index >= this->c.size()) {
            throw std::out_of_range("Stack index out of bound, exiting");
        }
        return this->c[index];
    }
};


#endif //STOCK_EXCHANGE_ITERABLESTACK_H

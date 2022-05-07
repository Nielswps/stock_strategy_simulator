//
// Created by niels on 5/7/22.
//

#ifndef STOCK_EXCHANGE_VISITOR_H
#define STOCK_EXCHANGE_VISITOR_H


#include "Data.h"

class Visitor {
public:
    virtual void visitData(const Data *data) const = 0;
};


#endif //STOCK_EXCHANGE_VISITOR_H

//
// Created by niels on 5/7/22.
//

#include "../include/HistoricData.h"

void HistoricData::accept(Strategy *visitor) {
    visitor->visitData(this);
}

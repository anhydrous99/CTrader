//
// Created by constexpr_dog on 3/2/21.
//

#include "libCTrader/types.h"

bool libCTrader::Product::operator==(const libCTrader::Product &product) const {
    return id == product.id;
}

bool libCTrader::Product::operator<(const libCTrader::Product &product) const {
    return id < product.id;
}

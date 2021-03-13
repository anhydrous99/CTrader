//
// Created by constexpr_dog on 3/7/21.
//

#include "libCTrader/Decimal.h"
#include <iostream>

[[maybe_unused]] libCTrader::Decimal::Decimal(const std::string &value, const std::string &quote_tick_size) {
    if (quote_tick_size == "0.01" || quote_tick_size == "0.01000000") {
        precision = 2;
        dec_2 = dec::decimal<2>(value);
    } else if (quote_tick_size == "0.001" || quote_tick_size == "0.00100000") {
        precision = 3;
        dec_3 = dec::decimal<3>(value);
    } else if (quote_tick_size == "0.0001" || quote_tick_size == "0.00010000") {
        precision = 4;
        dec_4 = dec::decimal<4>(value);
    } else if (quote_tick_size == "0.00001" || quote_tick_size == "0.00001000") {
        precision = 5;
        dec_5 = dec::decimal<5>(value);
    } else if (quote_tick_size == "0.000001" || quote_tick_size == "0.00000100") {
        precision = 6;
        dec_6 = dec::decimal<6>(value);
    } else if (quote_tick_size == "0.0000001" || quote_tick_size == "0.00000010") {
        precision = 7;
        dec_7 = dec::decimal<7>(value);
    } else if (quote_tick_size == "0.00000001") {
        precision = 8;
        dec_8 = dec::decimal<8>(value);
    }
}

[[maybe_unused]] libCTrader::Decimal::Decimal(const std::string &value, int prec) {
    precision = prec;
    if (prec == 2)
        dec_2 = dec::decimal<2>(value);
    else if (prec == 3)
        dec_3 = dec::decimal<3>(value);
    else if (prec == 4)
        dec_4 = dec::decimal<4>(value);
    else if (prec == 5)
        dec_5 = dec::decimal<5>(value);
    else if (prec == 6)
        dec_6 = dec::decimal<6>(value);
    else if (prec == 7)
        dec_7 = dec::decimal<7>(value);
    else if (prec == 8)
        dec_8 = dec::decimal<8>(value);
}

bool libCTrader::Decimal::operator==(const Decimal& rhs) const {
    if (precision == 2)
        return dec_2 == rhs.dec_2;
    else if (precision == 3)
        return dec_3 == rhs.dec_3;
    else if (precision == 4)
        return dec_4 == rhs.dec_4;
    else if (precision == 5)
        return dec_5 == rhs.dec_5;
    else if (precision == 6)
        return dec_6 == rhs.dec_6;
    else if (precision == 7)
        return dec_7 == rhs.dec_7;
    else
        return dec_8 == rhs.dec_8;
}

bool libCTrader::Decimal::operator<(const Decimal& rhs) const {
    if (precision == 2)
        return dec_2 < rhs.dec_2;
    else if (precision == 3)
        return dec_3 < rhs.dec_3;
    else if (precision == 4)
        return dec_4 < rhs.dec_4;
    else if (precision == 5)
        return dec_5 < rhs.dec_5;
    else if (precision == 6)
        return dec_6 < rhs.dec_6;
    else if (precision == 7)
        return dec_7 < rhs.dec_7;
    else
        return dec_8 < rhs.dec_8;
}

bool libCTrader::Decimal::operator<=(const Decimal& rhs) const {
    if (precision == 2)
        return dec_2 <= rhs.dec_2;
    else if (precision == 3)
        return dec_3 <= rhs.dec_3;
    else if (precision == 4)
        return dec_4 <= rhs.dec_4;
    else if (precision == 5)
        return dec_5 <= rhs.dec_5;
    else if (precision == 6)
        return dec_6 <= rhs.dec_6;
    else if (precision == 7)
        return dec_7 <= rhs.dec_7;
    else
        return dec_8 <= rhs.dec_8;
}

bool libCTrader::Decimal::operator>(const Decimal& rhs) const {
    if (precision == 2)
        return dec_2 > rhs.dec_2;
    else if (precision == 3)
        return dec_3 > rhs.dec_3;
    else if (precision == 4)
        return dec_4 > rhs.dec_4;
    else if (precision == 5)
        return dec_5 > rhs.dec_5;
    else if (precision == 6)
        return dec_6 > rhs.dec_6;
    else if (precision == 7)
        return dec_7 > rhs.dec_7;
    else
        return dec_8 > rhs.dec_8;
}

bool libCTrader::Decimal::operator>=(const Decimal& rhs) const {
    if (precision == 2)
        return dec_2 >= rhs.dec_2;
    else if (precision == 3)
        return dec_3 >= rhs.dec_3;
    else if (precision == 4)
        return dec_4 >= rhs.dec_4;
    else if (precision == 5)
        return dec_5 >= rhs.dec_5;
    else if (precision == 6)
        return dec_6 >= rhs.dec_6;
    else if (precision == 7)
        return dec_7 >= rhs.dec_7;
    else
        return dec_8 >= rhs.dec_8;
}

bool libCTrader::Decimal::operator!=(const Decimal& rhs) const {
    if (precision == 2)
        return dec_2 != rhs.dec_2;
    else if (precision == 3)
        return dec_3 != rhs.dec_3;
    else if (precision == 4)
        return dec_4 != rhs.dec_4;
    else if (precision == 5)
        return dec_5 != rhs.dec_5;
    else if (precision == 6)
        return dec_6 != rhs.dec_6;
    else if (precision == 7)
        return dec_7 != rhs.dec_7;
    else
        return dec_8 != rhs.dec_8;
}

float libCTrader::Decimal::get_flt() const {
    if (precision == 2)
        return static_cast<float>(dec_2.getAsDouble());
    else if (precision == 3)
        return static_cast<float>(dec_3.getAsDouble());
    else if (precision == 4)
        return static_cast<float>(dec_4.getAsDouble());
    else if (precision == 5)
        return static_cast<float>(dec_5.getAsDouble());
    else if (precision == 6)
        return static_cast<float>(dec_6.getAsDouble());
    else if (precision == 7)
        return static_cast<float>(dec_7.getAsDouble());
    else
        return static_cast<float>(dec_8.getAsDouble());
}

[[maybe_unused]] double libCTrader::Decimal::get_dbl() const {
    if (precision == 2)
        return dec_2.getAsDouble();
    else if (precision == 3)
        return dec_3.getAsDouble();
    else if (precision == 4)
        return dec_4.getAsDouble();
    else if (precision == 5)
        return dec_5.getAsDouble();
    else if (precision == 6)
        return dec_6.getAsDouble();
    else if (precision == 7)
        return dec_7.getAsDouble();
    else
        return dec_8.getAsDouble();
}

std::string libCTrader::Decimal::get_str() const {
    std::stringstream ss;
    if (precision == 2)
        ss << dec_2;
    else if (precision == 3)
        ss << dec_3;
    else if (precision == 4)
        ss << dec_4;
    else if (precision == 5)
        ss << dec_5;
    else if (precision == 6)
        ss << dec_6;
    else if (precision == 7)
        ss << dec_7;
    else
        ss << dec_8;
    return ss.str();
}
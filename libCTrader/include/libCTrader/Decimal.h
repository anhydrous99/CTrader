//
// Created by constexpr_dog on 3/7/21.
//

#ifndef CTRADER_DECIMAL_H
#define CTRADER_DECIMAL_H

#include <string>
#include <decimal.h>

namespace libCTrader {
    struct Decimal {
        int precision = 2;
        dec::decimal<2> dec_2;
        dec::decimal<3> dec_3;
        dec::decimal<4> dec_4;
        dec::decimal<5> dec_5;
        dec::decimal<6> dec_6;
        dec::decimal<7> dec_7;
        dec::decimal<8> dec_8;

        [[maybe_unused]] Decimal(const std::string &value, const std::string &quote_tick_size);
        [[maybe_unused]] Decimal(const std::string &value, int prec);

        template<typename T>
        bool operator==(const T &rhs) const;
        template<typename T>
        bool operator<(const T &rhs) const;
        template<typename T>
        bool operator<=(const T &rhs) const;
        template<typename T>
        bool operator>(const T &rhs) const;
        template<typename T>
        bool operator>=(const T &rhs) const;
        template<typename T>
        bool operator!=(const T &rhs) const;

        bool operator==(const Decimal &rhs) const;
        bool operator<(const Decimal &rhs) const;
        bool operator<=(const Decimal &rhs) const;
        bool operator>(const Decimal &rhs) const;
        bool operator>=(const Decimal &rhs) const;
        bool operator!=(const Decimal &rhs) const;

        [[nodiscard]] float get_flt() const;
        [[nodiscard]] double get_dbl() const;
        [[nodiscard]] std::string get_str() const;
    };

    // Template Implementations

    template<typename T>
    bool Decimal::operator==(const T& rhs) const {
        if (precision == 2)
            return dec_2 == rhs;
        else if (precision == 3)
            return dec_3 == rhs;
        else if (precision == 4)
            return dec_4 == rhs;
        else if (precision == 5)
            return dec_5 == rhs;
        else if (precision == 6)
            return dec_6 == rhs;
        else if (precision == 7)
            return dec_7 == rhs;
        else
            return dec_8 == rhs;
    }

    template<typename T>
    bool Decimal::operator<(const T& rhs) const {
        if (precision == 2)
            return dec_2 < rhs;
        else if (precision == 3)
            return dec_3 < rhs;
        else if (precision == 4)
            return dec_4 < rhs;
        else if (precision == 5)
            return dec_5 < rhs;
        else if (precision == 6)
            return dec_6 < rhs;
        else if (precision == 7)
            return dec_7 < rhs;
        else
            return dec_8 < rhs;
    }

    template<typename T>
    bool Decimal::operator<=(const T& rhs) const {
        if (precision == 2)
            return dec_2 <= rhs;
        else if (precision == 3)
            return dec_3 <= rhs;
        else if (precision == 4)
            return dec_4 <= rhs;
        else if (precision == 5)
            return dec_5 <= rhs;
        else if (precision == 6)
            return dec_6 <= rhs;
        else if (precision == 7)
            return dec_7 <= rhs;
        else
            return dec_8 <= rhs;
    }

    template<typename T>
    bool Decimal::operator>(const T& rhs) const {
        if (precision == 2)
            return dec_2 > rhs;
        else if (precision == 3)
            return dec_3 > rhs;
        else if (precision == 4)
            return dec_4 > rhs;
        else if (precision == 5)
            return dec_5 > rhs;
        else if (precision == 6)
            return dec_6 > rhs;
        else if (precision == 7)
            return dec_7 > rhs;
        else
            return dec_8 > rhs;
    }

    template<typename T>
    bool Decimal::operator>=(const T& rhs) const {
        if (precision == 2)
            return dec_2 >= rhs;
        else if (precision == 3)
            return dec_3 >= rhs;
        else if (precision == 4)
            return dec_4 >= rhs;
        else if (precision == 5)
            return dec_5 >= rhs;
        else if (precision == 6)
            return dec_6 >= rhs;
        else if (precision == 7)
            return dec_7 >= rhs;
        else
            return dec_8 >= rhs;
    }

    template<typename T>
    bool Decimal::operator!=(const T& rhs) const {
        if (precision == 2)
            return dec_2 != rhs;
        else if (precision == 3)
            return dec_3 != rhs;
        else if (precision == 4)
            return dec_4 != rhs;
        else if (precision == 5)
            return dec_5 != rhs;
        else if (precision == 6)
            return dec_6 != rhs;
        else if (precision == 7)
            return dec_7 != rhs;
        else
            return dec_8 != rhs;
    }
}

#endif //CTRADER_DECIMAL_H

//
// Created by constexpr_dog on 4/5/21.
//

#ifndef CTRADER_MOCK_AUTH_H
#define CTRADER_MOCK_AUTH_H

#include <gmock/gmock.h>
#include <libCTrader/Auth.h>

class MockAuth : public libCTrader::IAuth {
public:
    MOCK_METHOD4(Sign, std::string(const std::string&, const std::string&, const std::string&, const std::string&));
    MOCK_METHOD3(Sign, std::string(const std::string&, const std::string&, const std::string&));
};

#endif //CTRADER_MOCK_AUTH_H

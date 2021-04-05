//
// Created by constexpr_dog on 4/5/21.
//

#ifndef CTRADER_MOCK_CLIENT_H
#define CTRADER_MOCK_CLIENT_H

#include <gmock/gmock.h>
#include <libCTrader/Client.h>

class MockClient : public libCTrader::IClient {
public:
    MOCK_METHOD4(call, std::string(const std::string&, bool, const std::string&, const std::string&));
    MOCK_METHOD3(call, std::string(const std::string&, bool, const std::string&));
};

#endif //CTRADER_MOCK_CLIENT_H

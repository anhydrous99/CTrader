//
// Created by constexpr_dog on 4/5/21.
//

#include <gtest/gtest.h>
#include <libCTrader/Api.h>
#include "mock_Auth.h"
#include "mock_Client.h"

namespace {
    using ::testing::TestWithParam;
    using ::testing::Values;

    struct ApiTest : public ::testing::Test {
        libCTrader::Api api;
    protected:
        void SetUp() override {

        }
    };
}
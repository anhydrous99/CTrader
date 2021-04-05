//
// Created by constexpr_dog on 4/5/21.
//

#include <gtest/gtest.h>
#include <libCTrader/Api.h>
#include "mock_Client.h"

namespace {
    using ::testing::TestWithParam;
    using ::testing::Values;

    struct ApiTest : public ::testing::Test {
        libCTrader::Client *client;
        libCTrader::Api *api;
    protected:
        void SetUp() override {
            client = reinterpret_cast<libCTrader::Client *>(new MockClient());
            api = new libCTrader::Api(client);
        }

        void TearDown() override {
            delete api;
            delete client;
        }
    };

    TEST_F(ApiTest, accounts) {

    }

    TEST_F(ApiTest, account) {

    }

    TEST_F(ApiTest, account_ledger) {

    }

    TEST_F(ApiTest, account_holds) {

    }

    TEST_F(ApiTest, place_market_order) {

    }

    TEST_F(ApiTest, place_limit_order) {

    }

    TEST_F(ApiTest, place_stop_order) {

    }

    TEST_F(ApiTest, cancel_all_orders) {

    }

    TEST_F(ApiTest, list_orders) {

    }

    TEST_F(ApiTest, get_order) {

    }

    TEST_F(ApiTest, list_fills) {

    }

    TEST_F(ApiTest, exchange_limits) {

    }

    TEST_F(ApiTest, get_products) {

    }

    TEST_F(ApiTest, get_product) {

    }

    TEST_F(ApiTest, get_order_book) {

    }

    TEST_F(ApiTest, get_lvl3_order_book) {

    }

    TEST_F(ApiTest, get_product_ticker) {

    }

    TEST_F(ApiTest, list_latest_trades) {

    }

    TEST_F(ApiTest, get_historical_candles) {

    }

    TEST_F(ApiTest, get_historical_candles2) {

    }

    TEST_F(ApiTest, get_latest_historical_candles) {

    }

    TEST_F(ApiTest, get_24hr_stats) {

    }
}
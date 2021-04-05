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
        MockClient *client;
        libCTrader::Api *api;
    protected:
        void SetUp() override {
            client = new MockClient();
            api = new libCTrader::Api(reinterpret_cast<libCTrader::Client*>(client));
        }

        void TearDown() override {
            delete api;
            delete client;
        }
    };

    TEST_F(ApiTest, accounts) { // NOLINT
        EXPECT_CALL(*client, call("GET", true, "/accounts")).WillRepeatedly(::testing::Return(R"(
[
    {
        "id": "71452118-efc7-4cc4-8780-a5e22d4baa53",
        "currency": "BTC",
        "balance": "0.0000000000000000",
        "available": "0.0000000000000000",
        "hold": "0.0000000000000000",
        "profile_id": "75da88c5-05bf-4f54-bc85-5c775bd68254",
        "trading_enabled": true
    },
    {
        "id": "e316cb9a-0808-4fd7-8914-97829c1925de",
        "currency": "USD",
        "balance": "80.2301373066930000",
        "available": "79.2266348066930000",
        "hold": "1.0035025000000000",
        "profile_id": "75da88c5-05bf-4f54-bc85-5c775bd68254",
        "trading_enabled": true
    }
])"));
        auto accounts = api->accounts();
        EXPECT_EQ(accounts.size(), 2);
        EXPECT_EQ(accounts[0].id, "71452118-efc7-4cc4-8780-a5e22d4baa53");
        EXPECT_EQ(accounts[1].id, "e316cb9a-0808-4fd7-8914-97829c1925de");
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
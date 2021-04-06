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
        std::string account_id = "a1b2c3d4";
        EXPECT_CALL(*client, call("GET", true, "/accounts/" + account_id)).WillRepeatedly(::testing::Return(R"(
{
    "id": "a1b2c3d4",
    "balance": "1.100",
    "holds": "0.100",
    "available": "1.00",
    "currency": "USD"
})"));
        auto account = api->account(account_id);
        EXPECT_EQ(account.id, account_id);
    }

    TEST_F(ApiTest, account_ledger) {
        std::string account_id = "a1b2c3d4";
        EXPECT_CALL(*client, call("GET", true, "/accounts/" + account_id + "/ledger"))
        .WillRepeatedly(::testing::Return(R"([
    {
        "id": "100",
        "created_at": "2014-11-07T08:19:27.028459Z",
        "amount": "0.001",
        "balance": "239.669",
        "type": "fee",
        "details": {
            "order_id": "d50ec984-77a8-460a-b958-66f114b0de9b",
            "trade_id": "74",
            "product_id": "BTC-USD"
        }
    }
])"));
        auto ledger = api->account_ledger(account_id);
        EXPECT_EQ(ledger[0].id, "100");
    }

    TEST_F(ApiTest, account_holds) {
        std::string account_id = "a1b2c3d4";
        EXPECT_CALL(*client, call("GET", true, "/accounts/" + account_id + "/holds"))
        .WillRepeatedly(::testing::Return(R"([
    {
        "id": "82dcd140-c3c7-4507-8de4-2c529cd1a28f",
        "account_id": "e0b3f39a-183d-453e-b754-0c13e5bab0b3",
        "created_at": "2014-11-06T10:34:47.123456Z",
        "updated_at": "2014-11-06T10:40:47.123456Z",
        "amount": "4.23",
        "type": "order",
        "ref": "0a205de4-dd35-4370-a285-fe8fc375a273"
    }
])"));
        auto holds = api->account_holds(account_id);
    }

    TEST_F(ApiTest, place_market_order) {
        EXPECT_CALL(*client, call("POST", true, "/orders", "{\"product_id\":\"BTC-USD\",\"side\":\"buy\",\"size\":\"0.01000000\",\"type\":\"market\"}"))
        .WillRepeatedly(::testing::Return(R"({
    "id": "d0c5340b-6d6c-49d9-b567-48c4bfca13d2",
    "price": "0.10000000",
    "size": "0.01000000",
    "product_id": "BTC-USD",
    "side": "buy",
    "stp": "dc",
    "type": "market",
    "time_in_force": "GTC",
    "post_only": false,
    "created_at": "2016-12-08T20:02:28.53864Z",
    "fill_fees": "0.0000000000000000",
    "filled_size": "0.00000000",
    "executed_value": "0.0000000000000000",
    "status": "pending",
    "settled": false
})"));
        auto order = api->place_market_order("BTC-USD", "buy", "0.01000000", "");
        EXPECT_EQ(order.id, "d0c5340b-6d6c-49d9-b567-48c4bfca13d2");
    }

    TEST_F(ApiTest, place_limit_order) {
        EXPECT_CALL(*client, call("POST", true, "/orders", "{\"post_only\":true,\"price\":\"0.10000000\",\"product_id\":\"BTC-USD\",\"side\":\"buy\",\"size\":\"0.01000000\",\"type\":\"limit\"}"))
        .WillRepeatedly(::testing::Return(R"({
    "id": "d0c5340b-6d6c-49d9-b567-48c4bfca13d2",
    "price": "0.10000000",
    "size": "0.01000000",
    "product_id": "BTC-USD",
    "side": "buy",
    "stp": "dc",
    "type": "limit",
    "time_in_force": "GTC",
    "post_only": false,
    "created_at": "2016-12-08T20:02:28.53864Z",
    "fill_fees": "0.0000000000000000",
    "filled_size": "0.00000000",
    "executed_value": "0.0000000000000000",
    "status": "pending",
    "settled": false
})"));
        auto order = api->place_limit_order("BTC-USD", "buy", "0.10000000", "0.01000000");
        EXPECT_EQ(order.id, "d0c5340b-6d6c-49d9-b567-48c4bfca13d2");
    }

    TEST_F(ApiTest, cancel_all_orders) {
        EXPECT_CALL(*client, call("DELETE", true, "/orders"));
        api->cancel_all_orders();
    }

    TEST_F(ApiTest, list_orders) {
        EXPECT_CALL(*client, call("GET", true, "/orders"))
        .WillRepeatedly(::testing::Return(R"([
    {
        "id": "d0c5340b-6d6c-49d9-b567-48c4bfca13d2",
        "price": "0.10000000",
        "size": "0.01000000",
        "product_id": "BTC-USD",
        "side": "buy",
        "stp": "dc",
        "type": "limit",
        "time_in_force": "GTC",
        "post_only": false,
        "created_at": "2016-12-08T20:02:28.53864Z",
        "fill_fees": "0.0000000000000000",
        "filled_size": "0.00000000",
        "executed_value": "0.0000000000000000",
        "status": "open",
        "settled": false
    },
    {
        "id": "8b99b139-58f2-4ab2-8e7a-c11c846e3022",
        "price": "1.00000000",
        "size": "1.00000000",
        "product_id": "BTC-USD",
        "side": "buy",
        "stp": "dc",
        "type": "limit",
        "time_in_force": "GTC",
        "post_only": false,
        "created_at": "2016-12-08T20:01:19.038644Z",
        "fill_fees": "0.0000000000000000",
        "filled_size": "0.00000000",
        "executed_value": "0.0000000000000000",
        "status": "open",
        "settled": false
    }
])"));
        auto order_list = api->list_orders();
        EXPECT_EQ(order_list[0].id, "d0c5340b-6d6c-49d9-b567-48c4bfca13d2");
        EXPECT_EQ(order_list[1].id, "8b99b139-58f2-4ab2-8e7a-c11c846e3022");
    }

    TEST_F(ApiTest, get_order) {
        std::string order_id = "68e6a28f-ae28-4788-8d4f-5ab4e5e5ae08";
        EXPECT_CALL(*client, call("GET", true, "/orders/" + order_id))
        .WillRepeatedly(::testing::Return(R"(
{
    "id": "68e6a28f-ae28-4788-8d4f-5ab4e5e5ae08",
    "size": "1.00000000",
    "product_id": "BTC-USD",
    "side": "buy",
    "stp": "dc",
    "funds": "9.9750623400000000",
    "specified_funds": "10.0000000000000000",
    "type": "market",
    "post_only": false,
    "created_at": "2016-12-08T20:09:05.508883Z",
    "done_at": "2016-12-08T20:09:05.527Z",
    "done_reason": "filled",
    "fill_fees": "0.0249376391550000",
    "filled_size": "0.01291771",
    "executed_value": "9.9750556620000000",
    "status": "done",
    "settled": true
})"));
        auto order = api->get_order(order_id);
        EXPECT_EQ(order.id, order_id);
    }

    TEST_F(ApiTest, list_fills) {
        EXPECT_CALL(*client, call("GET", true, "/fills"))
        .WillRepeatedly(::testing::Return(R"([
    {
        "trade_id": 74,
        "product_id": "BTC-USD",
        "price": "10.00",
        "size": "0.01",
        "order_id": "d50ec984-77a8-460a-b958-66f114b0de9b",
        "created_at": "2014-11-07T22:19:28.578544Z",
        "liquidity": "T",
        "fee": "0.00025",
        "settled": true,
        "side": "buy"
    }
])"));
        auto fills = api->list_fills();
        EXPECT_EQ(fills[0].trade_id, 74);
    }

    TEST_F(ApiTest, get_products) {
        EXPECT_CALL(*client, call("GET", false, "/products"))
        .WillRepeatedly(::testing::Return(R"([
    {
        "id": "BTC-USD",
        "display_name": "BTC/USD",
        "base_currency": "BTC",
        "quote_currency": "USD",
        "base_increment": "0.00000001",
        "quote_increment": "0.01000000",
        "base_min_size": "0.00100000",
        "base_max_size": "280.00000000",
        "min_market_funds": "5",
        "max_market_funds": "1000000",
        "status": "online",
        "status_message": "",
        "cancel_only": false,
        "limit_only": false,
        "post_only": false,
        "trading_disabled": false
    }
])"));
        auto products = api->get_products();
        EXPECT_EQ(products[0].id, "BTC-USD");
    }

    TEST_F(ApiTest, get_product) {
        std::string product_id = "BTC-USD";
        EXPECT_CALL(*client, call("GET", false, "/products/" + product_id))
        .WillRepeatedly(::testing::Return(R"({
    "id": "BTC-USD",
    "display_name": "BTC/USD",
    "base_currency": "BTC",
    "quote_currency": "USD",
    "base_increment": "0.00000001",
    "quote_increment": "0.01000000",
    "base_min_size": "0.00100000",
    "base_max_size": "280.00000000",
    "min_market_funds": "5",
    "max_market_funds": "1000000",
    "status": "online",
    "status_message": "",
    "cancel_only": false,
    "limit_only": false,
    "post_only": false,
    "trading_disabled": false
})"));
        auto product = api->get_product(product_id);
        EXPECT_EQ(product.id, product_id);
    }

    TEST_F(ApiTest, get_product_ticker) {
        std::string product_id = "BTC-USD";
        EXPECT_CALL(*client, call("GET", false, "/products/" + product_id + "/ticker"))
        .WillRepeatedly(::testing::Return(R"({
  "trade_id": 4729088,
  "price": "333.99",
  "size": "0.193",
  "bid": "333.98",
  "ask": "333.99",
  "volume": "5957.11914015",
  "time": "2015-11-14T20:46:03.511254Z"
})"));
        auto ticker = api->get_product_ticker(product_id);
        EXPECT_EQ(ticker.trade_id, 4729088);
    }

    TEST_F(ApiTest, list_latest_trades) {
        std::string product_id = "BTC-USD";
        EXPECT_CALL(*client, call("GET", false, "/products/" + product_id + "/trades"))
        .WillRepeatedly(::testing::Return(R"([{
    "time": "2014-11-07T22:19:28.578544Z",
    "trade_id": 74,
    "price": "10.00000000",
    "size": "0.01000000",
    "side": "buy"
}, {
    "time": "2014-11-07T01:08:43.642366Z",
    "trade_id": 73,
    "price": "100.00000000",
    "size": "0.01000000",
    "side": "sell"
}])"));
        auto trades = api->list_latest_trades(product_id);
        EXPECT_EQ(trades[0].time, "2014-11-07T22:19:28.578544Z");
        EXPECT_EQ(trades[1].time, "2014-11-07T01:08:43.642366Z");
    }
}
//
// Created by Armando Herrera on 3/1/21.
//

#ifndef CTRADER_API_H
#define CTRADER_API_H

#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>
#include <vector>
#include <map>
#include "Auth.h"
#include "types.h"

namespace libCTrader {
    using ptime = boost::posix_time::ptime;

    //! Takes care of communicating with Coinbase Pro REST API
    class Api {
        //! Our auth class for request signatures
        Auth *auth;
        //! The uri of the Coinbase Pro REST API
        std::string uri;

        /*!
         * Makes the HTTP/S request to Coinbase Pro's API
         *
         * @param method The HTTP method
         * @param authed Whether we need to create our signature
         * @param path The HTTP path
         * @param body The body of our request
         * @return The request response
         */
        std::string call(const std::string &method, bool authed, const std::string &path, const std::string &body);

        /*!
         * Makes the HTTP/S request to Coinbase Pro's API
         *
         * @param method The HTTP method
         * @param authed Whether we need to create our signature
         * @param path The HTTP path
         * @return The request response
         */
        std::string call(const std::string &method, bool authed, const std::string &path);

        /*!
         * Creates a UNIX Timestamp (number of seconds from Epoch Time)
         *
         * @return Our timestamp
         */
        static std::string GetTimestamp();

        /*!
         * Creates url tags
         *
         * @param args The fields & values
         * @return The formatted url tags
         */
        static std::string build_url_args(const std::map<std::string, std::string> &args);

    public:
        Api(std::string uri, Auth *auth);
        ~Api();

        /*!
         * Change the authentication object
         *
         * @param au
         */
        void set_auth(Auth *au);

        /*!
         * Change the REST API uri
         *
         * @param u
         */
        void set_uri(std::string u);

        /*!
         * Gets all Coinbase Pro Accounts
         *
         * @return A vector of accounts, info
         */
        std::vector<Account> accounts();

        /*!
         * Gets a Coinbase Pro Account
         *
         * @param account_id The account id
         * @return The account info
         */
        Account account(const std::string& account_id);

        /*!
         * Gets an account ledger
         *
         * @param account_id The account id
         * @return The account history
         */
        std::vector<Account_Ledger_Entry> account_ledger(const std::string& account_id);

        /*!
         * Gets the holds on an account
         *
         * @param account_id The account id
         * @return The holds on an account, as a string
         */
        std::string account_holds(const std::string& account_id);

        /*!
         * Places a market order
         *
         * @param product_id The product id for the market order
         * @param side The side, "buy" or "sell"
         * @param size The size, how much btc or eth
         * @param funds The funds, how much usd
         * @return The returned order details
         */
        Order place_market_order(const std::string& product_id, const std::string& side, const std::string &size,
                                 const std::string &funds);

        /*!
         * Places a limit order
         *
         * @param product_id The product id for the limit order
         * @param side The side, "buy" or "sell"
         * @param price The price for our limit order
         * @param size The size, btc, eth, ...
         * @return The returned order details
         */
        Order place_limit_order(const std::string& product_id, const std::string& side, const std::string &price,
                                const std::string &size);

        /*!
         * Places a stop order
         *
         * @param product_id The product id for the stop order
         * @param side The side, "buy" or "sell"
         * @param price The price for our stop order
         * @param size The size of our stop order
         * @param funds The amount of funds available for our stop order
         * @return The returned order details
         */
        Order place_stop_order(const std::string& product_id, const std::string& side, const std::string &price,
                               const std::string &size, const std::string &funds);

        /*!
         * Cancel all orders
         *
         * @param product_id (If specified), cancel all orders for a particular product
         */
        void cancel_all_orders(const std::string *product_id=nullptr);

        /*!
         * List all orders
         *
         * @param status (If specified), list orders with this status
         * @param product_id (If specified), list orders with this product id
         * @return The list of orders
         */
        std::vector<Order> list_orders(const std::string *status=nullptr, const std::string *product_id=nullptr);

        /*!
         * Gets an order
         *
         * @param order_id The id for said order
         * @return The returned order details
         */
        Order get_order(const std::string &order_id);

        /*!
         * Gets a list of fills
         *
         * @param order_id (If specified), list of fills with this order id
         * @param product_id (If specified), list of fills with this product id
         * @return A list of fills
         */
        std::vector<Fill> list_fills(const std::string* order_id=nullptr, const std::string* product_id=nullptr);

        /*!
         * Returns your current exchange limits
         *
         * @return
         */
        std::string exchange_limits();

        // Public API
        /*!
         * Gets all available product
         *
         * @return A list of products
         */
        std::vector<Product> get_products();

        /*!
         * Gets a particular product details
         *
         * @param product_id The product id
         * @return The product
         */
        Product get_product(const std::string &product_id);

        /*!
         * Get an, either level 1 or level 2 order book. Use get_lvl3_order_book for a level 3 order book.
         *
         * @param product_id The product id
         * @param level The level, either 1 for level 1 or 2 for level 2
         * @return The order book
         */
        std::map<std::string, Page> get_order_book(const std::string &product_id, int level=1);

        /*!
         * Get the level 3 order book, it is recommended to use either a level 1 or level 2
         *
         * @param product_id The product id
         * @return The level 3 order book
         */
        std::map<std::string, PageLVL3> get_lvl3_order_book(const std::string &product_id);

        /*!
         * Get a product ticker
         *
         * @param product_id The desired product's id
         * @return The latest ticker of the product
         */
        Ticker get_product_ticker(const std::string &product_id);

        /*!
         * Get a list of the latest trade for a product
         *
         * @param product_id The product id
         * @return A list of the latest trades for a product
         */
        std::vector<Trade> list_latest_trades(const std::string &product_id);

        /*!
         * Get the historical price of a product
         *
         * @param product_id The product id
         * @param start The start ISO 8601 datetime stamp
         * @param end The end ISO 8601 datetime stamp
         * @param granularity The granularity in seconds
         * @return The historical candles
         */
        std::vector<Candle> get_historical_candles(const std::string &product_id, const std::string &start,
                                                 const std::string &end, int granularity);

        /*!
         * Get the historical price of a product
         *
         * @param product_id The product id
         * @param start The start ptime boost object
         * @param end The end ptime boost object
         * @param granularity The granularity in seconds
         * @return The historical candles
         */
        std::vector<Candle> get_historical_candles(const std::string &product_id, const ptime &start, const ptime &end,
                                                   int granularity);

        std::vector<Candle> get_latest_historical_candles(const std::string &product_id, int granularity);

        /*!
         * Get the 24 stats for a product
         *
         * @param product_id The product's id
         * @return The 24 stats
         */
        std::string get_24hr_stats(const std::string &product_id);
    };
}


#endif //CTRADER_API_H

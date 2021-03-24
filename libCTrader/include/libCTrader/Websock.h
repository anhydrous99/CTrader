//
// Created by constexpr_dog on 3/3/21.
//

#ifndef CTRADER_WEBSOCK_H
#define CTRADER_WEBSOCK_H

#include <cpprest/ws_client.h>
#undef U

#include <boost/signals2.hpp>
#include <shared_mutex>
#include <functional>
#include <map>
#include "types.h"

namespace libCTrader {
    //! Ingests the Coinbase Pro's websocket feed
    class Websock {
        //! Cpprestsdk websocket client
        web::web_sockets::client::websocket_callback_client client;

        /*!
         * Handles incoming websocket messages
         *
         * @param msg The incoming websocket message
         */
        void message_handler(const std::string &msg);

        /*!
         * Sends a message to websocket api
         *
         * @param msg The outgoing websocket message
         */
        void send_message(const std::string &msg);

        //! Keeps track of channel product pairs and their users
        std::map<std::pair<std::string, Product>, std::vector<int>> channel_product_ids;
        //! Signal for incoming ticker message
        boost::signals2::signal<void(const WSTicker &)> on_ticker_signal;
        //! Signal for incoming snapshot message, for level 2 order book
        boost::signals2::signal<void(const LVL2Snapshot &)> on_lvl2_book_snapshot_signal;
        //! Signal for incoming lvl2update message
        boost::signals2::signal<void(const LVL2Update &)> on_lvl2_book_update_signal;
        //! Websocket uri
        std::string uri;
        //! Whether we are connected to the websocket feed
        bool connected = false;

    public:
        explicit Websock(std::string uri);
        ~Websock();

        /*!
         * Connect to the websocket feed, also subscribes to the channel product pairs
         */
        void Connect();

        /*!
         * Disconnects from the websocket feed, also unsubs from the channel product pairs
         */
        void Disconnect();

        /*!
         * Adds a channel product pair, subscribes if connected
         *
         * @param channel The channel to subscribe to
         * @param product The product to subscribe to
         * @param id An id to distinguish from other channel product pairs
         */
        void add_channel_product_pair(const std::string &channel, const Product &product, int id=0);

        /*!
         * Removes a channel product pair, unsubscribes if connected
         *
         * @param channel The channel to unsubscribe from
         * @param product The product to unsubscribe from
         * @param id An id to distinguish from other channel product pairs
         */
        void remove_channel_product_pair(const std::string &channel, const Product &product, int id=0);

        /*!
         * A channel to subscribe to, with a list of products
         *
         * @param channel The channel to subscribe to
         * @param products The list of products to subscribe
         * @param id An id to distinguish
         */
        void add_channel(const std::string &channel, const std::vector<Product> &products, int id=0);

        /*!
         * Removes and unsubscribes, if connected, from a channel, all products
         *
         * @param channel The channel
         */
        void remove_channel(const std::string &channel);

        /*!
         * Adds a handler for when a new ticker comes in, watch out for thread safety
         *
         * @param handler A function with the WSTicker class as an input
         */
        void on_new_ticker(const std::function<void(const WSTicker&)> &handler);

        /*!
         * Adds a handler for when a snapshot comes in, watch out for thread safety
         *
         * @param handler A function with the LVL2Snapshot class as an input
         */
        void on_lvl2_snapshot(const std::function<void(const LVL2Snapshot&)> &handler);

        /*!
         * Adds a handler for when an lvl2 update comes in, watch out for thread safety
         *
         * @param handler A function with the LVL2Update class as an input
         */
        void on_lvl2_update(const std::function<void(const LVL2Update&)> &handler);

        /*!
         * Checks if a channel-product pair is connected
         *
         * @param channel The channel to check
         * @param product The product to check
         * @return Whether it is connected
         */
        bool is_connected(const std::string &channel, const Product &product);

        /*!
         * Change the uri
         *
         * @param u The uri to change to
         */
        void set_uri(const std::string &u);
    };
}

#endif //CTRADER_WEBSOCK_H

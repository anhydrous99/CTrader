//
// Created by constexpr_dog on 4/5/21.
//

#ifndef CTRADER_CLIENT_H
#define CTRADER_CLIENT_H

#include <utility>

#include "libCTrader/Auth.h"

namespace libCTrader {
    struct IClient {
        virtual ~IClient() = default;
        virtual std::string call(const std::string&, bool, const std::string&, const std::string&) = 0;
        virtual std::string call(const std::string&, bool, const std::string&) = 0;
    };

    class Client : IClient {
        //! Our auth class for request signatures
        Auth *auth;
        //! The uri of the Coinbase Pro REST API
        std::string uri;

    public:
        Client(Auth *auth, std::string uri);
        ~Client() override;

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
         * Makes the HTTP/S request to Coinbase Pro's API
         *
         * @param method The HTTP method
         * @param authed Whether we need to create our signature
         * @param path The HTTP path
         * @param body The body of our request
         * @return The request response
         */
        std::string call(const std::string &method, bool authed, const std::string &path, const std::string &body) override;

        /*!
         * Makes the HTTP/S request to Coinbase Pro's API
         *
         * @param method The HTTP method
         * @param authed Whether we need to create our signature
         * @param path The HTTP path
         * @return The request response
         */
        std::string call(const std::string &method, bool authed, const std::string &path) override;

        /*!
         * Creates a UNIX Timestamp (number of seconds from Epoch Time)
         *
         * @return Our timestamp
         */
        template<typename T>
        static T get_timestamp();
        static std::string get_timestamp();
    };

    template<typename T>
    T Client::get_timestamp() {
        return time(nullptr);
    }
}

#endif //CTRADER_CLIENT_H

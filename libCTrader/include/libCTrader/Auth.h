//
// Created by Armando Herrera on 3/1/21.
//

#ifndef CTRADER_AUTH_H
#define CTRADER_AUTH_H

#include <string>

namespace libCTrader {
    struct IAuth {
        [[nodiscard]] virtual std::string Sign(const std::string&, const std::string&, const std::string&,const std::string&) const = 0;
        [[nodiscard]] virtual std::string Sign(const std::string &, const std::string &, const std::string&) const = 0;
        virtual ~IAuth() = default;
    };

    //! Take care of our authentication
    class Auth : IAuth {
    public:
        //! This is the API key
        std::string Key;
        //! This is the provided secret
        std::string Secret;
        //! This is your passphrase
        std::string Passphrase;

        /*!
         * Decodes a base-64 string
         *
         * @param input A base-64 string
         * @return A decoded string
         */
        static std::string b64_decode(const std::string &input);

        /*!
         * Encodes a base-64 string
         *
         * @param input A string to encode
         * @return The encoded string
         */
        static std::string b64_encode(const std::string &input);

        /*!
         * Creates an HMAC SHA256 Signature for coinbase pro's REST API
         *
         * @param time_stamp A UNIX Time timestamp - (The number of seconds since Epoch)
         * @param method GET, POST, DELETE, ... - Your http methods
         * @param path The URL path to the http request
         * @param body The http body
         * @return The HMAC SHA256 Signature
         */
        [[nodiscard]] std::string
        Sign(const std::string &time_stamp, const std::string &method, const std::string &path,
             const std::string &body) const override;

        /*!
         * Creates an HMAC SHA256 Signature for coinbase pro's REST API
         *
         * @param time_stamp A UNIX Time timestamp - (The number of seconds since Epoch)
         * @param method GET, POST, DELETE, ... - Your http methods
         * @param path The URL path to the http request
         * @return The HMAC SHA256 Signature
         */
        [[nodiscard]] std::string
        Sign(const std::string &time_stamp, const std::string &method, const std::string &path) const override;

        Auth() = default;

        /*!
         * Creates an Auth class
         *
         * @param key The cryptographic key
         * @param secret The given secret string
         * @param passphrase The created passphrase
         */
        Auth(std::string key, std::string secret, std::string passphrase);

        /*!
         * Change the key
         *
         * @param key
         */
        void set_key(const std::string &key);

        /*!
         * Change the secret
         *
         * @param secret
         */
        void set_secret(const std::string &secret);

        /*!
         * Change the passphrase
         *
         * @param passphrase
         */
        void set_passphrase(const std::string &passphrase);
    };
}


#endif //CTRADER_AUTH_H

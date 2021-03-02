//
// Created by Armando Herrera on 3/1/21.
//

#ifndef CTRADER_AUTH_H
#define CTRADER_AUTH_H

#include <string>

class Auth {
    static std::string b64_decode(const std::string &input);
    static std::string b64_encode(const std::string &input);

public:
    std::string Key;
    std::string Secret;
    std::string Passphrase;

    [[nodiscard]] std::string Sign(const std::string& time_stamp, const std::string& method, const std::string& path, const std::string& body) const;
    [[nodiscard]] std::string Sign(const std::string& time_stamp, const std::string& method, const std::string& path) const;
    Auth() = default;
    Auth(std::string key, std::string secret, std::string passphrase);

    void set_key(const std::string &key);
    void set_secret(const std::string &secret);
    void set_passphrase(const std::string &passphrase);
};


#endif //CTRADER_AUTH_H

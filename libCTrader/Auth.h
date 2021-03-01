//
// Created by Armando Herrera on 3/1/21.
//

#ifndef CTRADER_AUTH_H
#define CTRADER_AUTH_H

#include <string>

class Auth {
public:
    std::string Key;
    std::string Secret;
    std::string Passphrase;

    std::string Sign(const std::string& time_stamp, const std::string& method, const std::string& path, const std::string& body) const;
    std::string Sign(const std::string& time_stamp, const std::string& method, const std::string& path) const;
    Auth() = default;
    Auth(std::string key, std::string secret, std::string passphrase);

    static std::string b64_decode(std::string &input);
    static std::string b64_encode(std::string &input);
};


#endif //CTRADER_AUTH_H

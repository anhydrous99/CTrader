//
// Created by Armando Herrera on 3/1/21.
//

#include "Auth.h"

#include <utility>
#include <iostream>
#include <openssl/evp.h>

std::string Auth::Sign(const std::string &time_stamp, const std::string &method, const std::string &path,
                       const std::string &body) const {
    std::string plain = time_stamp + method + path = body;
    return plain;
}

std::string Auth::Sign(const std::string &time_stamp, const std::string &method, const std::string &path) const {
    return std::string();
}

Auth::Auth(std::string key, std::string secret, std::string passphrase) : Key(std::move(key)),
    Secret(std::move(secret)), Passphrase(std::move(passphrase)) {}

std::string Auth::b64_decode(std::string &input) {
    const auto pl = 3 * input.size() / 4;
    auto output = reinterpret_cast<unsigned char *>(calloc(pl+1, 1));
    const auto ol = EVP_DecodeBlock(output, reinterpret_cast<const unsigned char *>(input.c_str()), input.size());
    if (pl != ol)
        std::cerr << "Error, decode prediced " << pl << " but we got " << ol << std::endl;
    return std::string(reinterpret_cast<const char *>(output));
}

std::string Auth::b64_encode(std::string &input) {
    const auto pl = 4 * ((input.size() + 2) / 3);
    auto output = reinterpret_cast<char *>(calloc(pl + 1, 1));
    const auto ol = EVP_EncodeBlock(reinterpret_cast<unsigned char *>(output),
                                    reinterpret_cast<const unsigned char *>(input.c_str()), input.size());
    if (pl != ol)
        std::cerr << "Error: encode predicted" << pl << " but we got " << ol << std::endl;
    return std::string(output);
}
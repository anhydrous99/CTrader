//
// Created by Armando Herrera on 3/1/21.
//

#include "Auth.h"

#include <utility>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/hmac.h>

std::string Auth::Sign(const std::string &time_stamp, const std::string &method, const std::string &path,
                       const std::string &body) const {
    std::string message = time_stamp + method + path + body;
    std::string secret = b64_decode(Secret);
    unsigned int result_len;
    unsigned char result[EVP_MAX_MD_SIZE];
    HMAC(EVP_sha256(), reinterpret_cast<void *>(&secret[0]), secret.length(),
         reinterpret_cast<const unsigned char *>(message.c_str()), message.length(), result, &result_len);
    std::string signature(reinterpret_cast<char *>(result), result_len);
    return b64_encode(signature);
}

std::string Auth::Sign(const std::string &time_stamp, const std::string &method, const std::string &path) const {
    return Sign(time_stamp, method, path, "");
}

Auth::Auth(std::string key, std::string secret, std::string passphrase) : Key(std::move(key)),
    Secret(std::move(secret)), Passphrase(std::move(passphrase)) {}

std::string Auth::b64_decode(const std::string &input) {
    const auto pl = 3 * input.size() / 4;
    auto output = reinterpret_cast<unsigned char *>(calloc(pl+1, 1));
    const auto ol = EVP_DecodeBlock(output, reinterpret_cast<const unsigned char *>(input.c_str()), input.size());
    if (pl != ol)
        std::cerr << "Error, decode prediced " << pl << " but we got " << ol << std::endl;
    return std::string(reinterpret_cast<const char *>(output));
}

std::string Auth::b64_encode(const std::string &input) {
    const auto pl = 4 * ((input.size() + 2) / 3);
    auto output = reinterpret_cast<char *>(calloc(pl + 1, 1));
    const auto ol = EVP_EncodeBlock(reinterpret_cast<unsigned char *>(output),
                                    reinterpret_cast<const unsigned char *>(input.c_str()), input.size());
    if (pl != ol)
        std::cerr << "Error: encode predicted" << pl << " but we got " << ol << std::endl;
    return std::string(output);
}
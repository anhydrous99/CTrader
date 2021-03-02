//
// Created by Armando Herrera on 3/1/21.
//

#ifndef CTRADER_API_H
#define CTRADER_API_H

#include <string>
#include "Auth.h"

class Api {
    Auth auth;
    std::string uri;

    std::string call(const std::string &method, bool authed, const std::string &path, const std::string &body);
    std::string call(const std::string &method, bool authed, const std::string &path);
    static std::string GetTimestamp();

public:
    Api(std::string uri, Auth auth);
    ~Api();
};


#endif //CTRADER_API_H

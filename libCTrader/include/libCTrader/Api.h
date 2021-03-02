//
// Created by Armando Herrera on 3/1/21.
//

#ifndef CTRADER_API_H
#define CTRADER_API_H

#include <string>
#include <vector>
#include "Auth.h"
#include "types.h"

namespace libCTrader {
    class Api {
        Auth *auth;
        std::string uri;

        std::string call(const std::string &method, bool authed, const std::string &path, const std::string &body);
        std::string call(const std::string &method, bool authed, const std::string &path);
        static std::string GetTimestamp();

    public:
        Api(std::string uri, Auth *auth);
        ~Api();

        void set_auth(Auth *au);
        void set_uri(std::string u);

        std::vector<Account> accounts();
    };
}


#endif //CTRADER_API_H

//
// Created by Armando Herrera on 3/1/21.
//

#ifndef CTRADER_API_H
#define CTRADER_API_H

#include <string>

class Api {
    std::string call(const std::string &method, bool authed, const std::string &path, const std::string &body);
    std::string call(const std::string &method, bool authed, const std::string &path);

};


#endif //CTRADER_API_H

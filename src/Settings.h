//
// Created by constexpr_dog on 3/2/21.
//

#ifndef CTRADER_SETTINGS_H
#define CTRADER_SETTINGS_H

#include <Auth.h>
#include <Api.h>

class Settings {
    bool first = true;
    bool changed = false;
    std::string api_uri;
    std::string ws_uri;
    std::string fix_uri;
    libCTrader::Auth auth;
    libCTrader::Api api;
    void load();

public:
    Settings();

    std::string get_api_uri();
    std::string get_websocket_uri();
    std::string get_fix_uri();
    libCTrader::Auth *get_authentication();
    libCTrader::Api *get_api();

    bool create_settings_window();
    bool settings_have_changed();
};


#endif //CTRADER_SETTINGS_H

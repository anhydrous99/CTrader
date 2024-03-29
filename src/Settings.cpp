//
// Created by constexpr_dog on 3/2/21.
//

#include <yaml-cpp/yaml.h>
#include <fstream>
#include <cstring>
#include "imgui.h"
#include "Settings.h"

Settings::Settings() : api("", nullptr), websock("") {
    load();
     api.set_uri(api_uri);
     api.set_auth(&auth);
    websock.set_uri(ws_uri);
}

std::string Settings::get_api_uri() {
    return api_uri;
}

std::string Settings::get_websocket_uri() {
    return ws_uri;
}

std::string Settings::get_fix_uri() {
    return fix_uri;
}

libCTrader::Auth *Settings::get_authentication() {
    return &auth;
}

libCTrader::Api *Settings::get_api() {
    return &api;
}

bool Settings::create_settings_window() {
    bool close = false;
    static char rest_api[128];
    static char ws_feed[128];
    static char fix_api[128];
    static char key_char[128];
    static char secret_char[128];
    static char passphrase_char[128];

    if (first) {
        std::strcpy(rest_api, api_uri.c_str());
        std::strcpy(ws_feed, ws_uri.c_str());
        std::strcpy(fix_api, fix_uri.c_str());
        std::strcpy(key_char, auth.Key.c_str());
        std::strcpy(secret_char, auth.Secret.c_str());
        std::strcpy(passphrase_char, auth.Passphrase.c_str());
        first = false;
    }

    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::BeginGroup();
    ImGui::Text("Servers");
    ImGui::InputText("Rest API", rest_api, IM_ARRAYSIZE(rest_api));
    ImGui::InputText("Websocket Feed", ws_feed, IM_ARRAYSIZE(fix_api));
    ImGui::InputText("Fix API", fix_api, IM_ARRAYSIZE(fix_api));
    ImGui::EndGroup();
    ImGui::BeginGroup();
    ImGui::Text("Authentication");
    ImGui::InputText("Key", key_char, IM_ARRAYSIZE(key_char));
    ImGui::InputText("Secret", secret_char, IM_ARRAYSIZE(secret_char));
    ImGui::InputText("Passphrase", passphrase_char, IM_ARRAYSIZE(passphrase_char));
    ImGui::EndGroup();
    if (ImGui::Button("Submit")) {
        api_uri = std::string(rest_api);
        ws_uri = std::string(ws_feed);
        fix_uri = std::string(fix_api);
        auth.set_key(std::string(key_char));
        auth.set_secret(std::string(secret_char));
        auth.set_passphrase(std::string(passphrase_char));

        api.set_uri(api_uri);

        YAML::Node settings = YAML::LoadFile("settings.yaml");
        settings["Servers"]["Rest API"] = api_uri;
        settings["Servers"]["Websocket Feed"] = ws_uri;
        settings["Servers"]["Fix API"] = fix_uri;
        settings["Authentication"]["Key"] = auth.Key;
        settings["Authentication"]["Secret"] = auth.Secret;
        settings["Authentication"]["Passphrase"] = auth.Passphrase;

        std::ofstream settings_file("settings.yaml");
        settings_file << settings;
        settings_file.close();
        close = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset"))
        load();
    ImGui::SameLine();
    if (ImGui::Button("Close"))
        close = true;
    ImGui::End();
    return close;
}

bool Settings::settings_have_changed() {
    if (changed) {
        changed = false;
        return true;
    }
    return false;
}

void Settings::load() {
    YAML::Node settings = YAML::LoadFile("settings.yaml");
    auto servers = settings["Servers"];
    api_uri = servers["Rest API"].as<std::string>();
    ws_uri = servers["Websocket Feed"].as<std::string>();
    fix_uri = servers["Fix API"].as<std::string>();

    api.set_uri(api_uri);

    auto authentication = settings["Authentication"];
    auth.set_key(authentication["Key"].as<std::string>());
    auth.set_secret(authentication["Secret"].as<std::string>());
    auth.set_passphrase(authentication["Passphrase"].as<std::string>());

    first = true;
}

libCTrader::Websock *Settings::get_websock() {
    return &websock;
}

#pragma once
#include <map>
#include "logger.h"

class GlobalVar {
public:
    static GlobalVar* instance() {
        DEBUGLOG("GlobalVar::instance||init instance");
        static GlobalVar gv;
        return &gv;
    }

    bool get_value(std::string key) {
        if (var.find(key) == var.end())
        {
            ERRORLOG("GlobalVar::get_value||key not found||key={}", key);
            return false;
        }
        else
        {
            bool value = var[key];
            INFOLOG("GlobalVar::get_value||key={}||value={}", key, value);
            return value;
        }
    }

    void set_value(std::string key, bool value) {
        INFOLOG("GlobalVar::set_value||key={}||value={}", key, value);
        var[key] = value;
    }

private:
    GlobalVar() {
        var["exit_game"] = false;
    }

    std::map<std::string, bool> var;
};

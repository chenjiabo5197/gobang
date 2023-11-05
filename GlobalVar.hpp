#pragma once
#include <map>
#include "logger.h"

typedef enum {
    RESULT_FLAG_DEFAULT = 0,
    PLAYER_WIN = 1,      // 玩家赢 
    PLAYER_LOSE = 2,      // 玩家输
    RESULT_DRAW = 3,      // 平局
    BLACK_WIN = 4,      // 黑方胜
    WHITE_WIN = 5      // 白方胜
}result_flag;

class GlobalVar {
public:
    static GlobalVar* instance() 
    {
        DEBUGLOG("GlobalVar::instance||init instance");
        static GlobalVar gv;
        return &gv;
    }

    bool getValue(std::string key) 
    {
        if (var.find(key) == var.end())
        {
            ERRORLOG("GlobalVar::getValue||key not found||key={}", key);
            return false;
        }
        else
        {
            bool value = var[key];
            INFOLOG("GlobalVar::getValue||key={}||value={}", key, value);
            return value;
        }
    }

    void setValue(std::string key, bool value) 
    {
        INFOLOG("GlobalVar::setValue||key={}||value={}", key, value);
        var[key] = value;
    }

    void setResultFlag(result_flag kind)
    {
        resultFlag = kind;
    }

    result_flag getResultFlag()
    {
        return resultFlag;
    }

private:
    GlobalVar() 
    {
        var["exitGame"] = false;
        resultFlag = RESULT_FLAG_DEFAULT;
    }

    result_flag resultFlag;

    std::map<std::string, bool> var;
};

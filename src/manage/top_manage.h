#pragma once

#include "../logger/logger.h"
#include "../utils/config.h"
#include "../render/render.h"

class TopManage
{
private:
    Render* render;
    
public:
    TopManage(const Config& config);
    ~TopManage();

    void start();
};



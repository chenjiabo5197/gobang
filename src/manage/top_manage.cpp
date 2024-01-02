#include "top_manage.h"

TopManage::TopManage(const Config& config)
{
    this->render = new Render(config);
    INFOLOG("TopManage construct success");
}

TopManage::~TopManage()
{
    delete render;
    INFOLOG("~TopManage success");
}

void TopManage::start()
{
    this->render->setRendererType(PLAYCHESS_INTERFACE);
    this->render->renderer();
}


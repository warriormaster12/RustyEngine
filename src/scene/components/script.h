#pragma once

#include "component.h"


class Script : public Component {
public:
    virtual void on_ready()override;
    virtual void update(const double& deltaT)override;
};

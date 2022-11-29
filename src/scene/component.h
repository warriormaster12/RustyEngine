#pragma once

#include <iostream>


class Component {
public:
    const char* type_name;
    virtual void on_ready()=0;
    virtual void update(const double& deltaT)=0;
};

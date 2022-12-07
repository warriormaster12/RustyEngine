#pragma once

#include <iostream>

class Entity;

class Component {
public:
    const char* type_name;
    Entity* entity = nullptr;
    virtual void on_ready()=0;
    virtual void update(const double& deltaT)=0;
};

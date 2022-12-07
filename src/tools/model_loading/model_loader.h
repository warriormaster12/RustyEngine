#pragma  once

#include <iostream>

#include <vector>

#include "entity.h"


class ModelLoader{
public:
    static std::vector<Entity> LoadFile(const std::string& file);

};

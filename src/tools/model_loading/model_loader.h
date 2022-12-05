#pragma  once 

#include <iostream>

#include <vector>

#include "entity.h"


class ModelLoader{
public:
    static void LoadFile(const std::string& file, std::vector<Entity>& outEntities);

};
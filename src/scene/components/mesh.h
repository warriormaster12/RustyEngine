#pragma once

#include "component.h"
#include <vector>

#include "glm.hpp"

#include "VkInit.h"


struct Vertex {
    glm::vec3 position;
    glm::vec3 color = glm::vec3(1.0f);
    glm::vec3 normals;
};


class Mesh : public Component {
public:
    virtual void on_ready()override;
    virtual void update(const double& deltaT)override;

    std::vector<uint32_t> indicies = {};
    std::vector<Vertex> vertices = {};
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    Pipeline* pipeline = nullptr;
    VkTools::AllocatedBuffer vertexBuffer;
    VkTools::AllocatedBuffer indexBuffer;
    VkTools::AllocatedBuffer modelBuffer;
};

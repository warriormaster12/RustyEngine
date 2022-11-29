#pragma once

#include "component.h"
#include "glm/glm.hpp"


class Transform : public Component {
public:
    virtual void on_ready()override;
    virtual void update(const double& deltaT)override;

    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    glm::mat4 GetModelMatrix();
};

#pragma once

#include "component.h"
#include "glm.hpp"


class Transform : public Component {
public:
    virtual void on_ready()override;
    virtual void update(const double& deltaT)override;

    void SetPosition(const glm::vec3& value);
    void SetRotation(const glm::vec3& value);
    void SetScale(const glm::vec3& value);
    void SetTransformMatrix(const glm::mat4& value);

    const glm::vec3& GetPosition() {return position;}
    const glm::mat4& GetTransformMatrix() {return transformMatrix;}

private:
    glm::mat4 GetModelMatrix();
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    glm::mat4 transformMatrix = glm::mat4(1.0f);
};

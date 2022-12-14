#pragma once
#include <iostream>
#include "component.h"
#include "glm.hpp"
#include "VkInit.h"

class Transform;

class Camera : public Component{
public:
    void on_ready() override;
    void update(const double& delta) override;

    glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetRotationMatrix() const;

    glm::mat4 renderMatrix;


    float fov = 90.0f;
    float zNear = 0.1f;
	float zFar = 100.0f;
    float aspect = 0.0f;
    
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;

    VkTools::AllocatedBuffer cameraBuffer;

    Transform* transform;
};

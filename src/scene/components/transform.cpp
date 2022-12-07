#include "transform.h"
#include "gtx/euler_angles.hpp"

void Transform::on_ready() {

}


void Transform::update(const double &delta){

}


void Transform::SetPosition(const glm::vec3& value){
    position = value;
    transformMatrix = GetModelMatrix();
}

void Transform::SetRotation(const glm::vec3& value){
    rotation = value;
    transformMatrix = GetModelMatrix();
}

void Transform::SetScale(const glm::vec3& value){
    scale = value;
    transformMatrix = GetModelMatrix();
}

void Transform::SetTransformMatrix(const glm::mat4& value){
    position = value[3];
    scale.x = glm::length(glm::vec3(value[0]));
    scale.y = glm::length(glm::vec3(value[1]));
    scale.z = glm::length(glm::vec3(value[2]));

    auto T1 = glm::atan2(value[2][1], value[2][2]);
    auto C2 = glm::sqrt(value[0][0]*value[0][0] + value[1][0]*value[1][0]);
    auto T2 = glm::atan2(-value[2][0], C2);
    auto S1 = glm::sin(T1);
    auto C1 = glm::cos(T1);
    auto T3 = glm::atan2(S1*value[0][2] - C1*value[0][1], C1*value[1][1] - S1*value[1][2  ]);
    rotation = glm::vec3(-T1,-T2,-T3);

    transformMatrix = GetTransformMatrix();
}

glm::mat4 Transform::GetModelMatrix() {
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);
    return glm::mat4{
    {
        scale.x * (c1 * c3 + s1 * s2 * s3),
        scale.x * (c2 * s3),
        scale.x * (c1 * s2 * s3 - c3 * s1),
        0.0f,
    },
    {
        scale.y * (c3 * s1 * s2 - c1 * s3),
        scale.y * (c2 * c3),
        scale.y * (c1 * c3 * s2 + s1 * s3),
        0.0f,
    },
    {
        scale.z * (c2 * s1),
        scale.z * (-s2),
        scale.z * (c1 * c2),
        0.0f,
    },
    {position.x, position.y, position.z, 1.0f}};
}

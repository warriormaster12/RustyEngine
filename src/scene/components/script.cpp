#include "script.h"
#include "transform.h"
#include "camera.h"
#include "window.h"

#include "GLFW/glfw3.h"
#include "entity.h"

float speed = 3.0f;
glm::vec3 position = glm::vec3(0.0f);
glm::vec3 velocity = glm::vec3(0.0f);

void Script::on_ready() {
    if (entity->GetComponent<Transform>() != nullptr){
        position = entity->GetComponent<Transform>()->GetPosition();
    }
}


void Script::update(const double &delta){
    glm::vec3 inputAxis = glm::vec3(0.0f);
    if(Window::GetKEvents(GLFW_KEY_W) == GLFW_PRESS)
    {
        inputAxis.z -= 1.f;
    }
    if(Window::GetKEvents(GLFW_KEY_S) == GLFW_PRESS)
    {
        inputAxis.z += 1.f;
    }

    if(Window::GetKEvents(GLFW_KEY_D) == GLFW_PRESS)
    {
        inputAxis.x += 1.f;
    }
    if(Window::GetKEvents(GLFW_KEY_A) == GLFW_PRESS)
    {
        inputAxis.x -= 1.f;
    }

    if(Window::GetKEvents(GLFW_KEY_Q) == GLFW_PRESS)
    {
        inputAxis.y -= 1.f;
    }
    if(Window::GetKEvents(GLFW_KEY_E) == GLFW_PRESS)
    {
        inputAxis.y += 1.f;
    }

    if(Window::GetKEvents(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        speed = 6.0f;
    }
    else {
        speed = 3.0f;
    }

    inputAxis = glm::clamp(inputAxis, { -1.0,-1.0,-1.0 }, { 1.0,1.0,1.0 });
    if (entity->GetComponent<Camera>() != nullptr){
        glm::mat4 cam_rot = entity->GetComponent<Camera>()->GetRotationMatrix();

        glm::vec3 forward = { 0,0,1.0f };
	    glm::vec3 right = { 1.0f,0,0 };
        glm::vec3 up = { 0,1.0f,0 };

        forward = cam_rot * glm::vec4(forward, 0.f);
        right = cam_rot * glm::vec4(right, 0.f);

        entity->GetComponent<Camera>()->forward = forward;
        entity->GetComponent<Camera>()->right = right;
        entity->GetComponent<Camera>()->up = up;

        

        velocity = inputAxis.z * forward + inputAxis.x * right + inputAxis.y * up;

	    velocity *= speed * delta;

        position += velocity;
        if (entity->GetComponent<Transform>() != nullptr){
            entity->GetComponent<Transform>()->SetPosition(position);
        }
    }
}

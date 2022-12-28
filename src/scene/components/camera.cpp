#include "camera.h"
#include "gtc/matrix_transform.hpp"
#include "entity.h"
#include "components/transform.h"



void Camera::on_ready() {
	cameraBuffer.AllocateBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, sizeof(glm::mat4));
}


void Camera::update(const double &delta){
	if(entity != nullptr) {
		if(entity->GetComponent<Transform>() != nullptr) {
			transform = entity->GetComponent<Transform>();
		}
	}
	cameraBuffer.UploadBufferData(&renderMatrix);
}

glm::mat4 Camera::GetRotationMatrix() const
{
	float yaw = 0.0f;
	float pitch = 0.0f;
	if(entity->GetComponent<Transform>() != nullptr){
		yaw = entity->GetComponent<Transform>()->GetRotation().x;
		pitch = entity->GetComponent<Transform>()->GetRotation().y;
	}

	glm::mat4 yaw_rot = glm::rotate(glm::mat4{ 1 }, yaw, { 0,1,0 });
	glm::mat4 pitch_rot = glm::rotate(glm::mat4{ yaw_rot }, pitch, { -1,0,0 });

	return pitch_rot;
}


glm::mat4 Camera::GetViewMatrix() const
{
	glm::mat4 cam_rot = GetRotationMatrix();
	glm::mat4 view;
	if(transform != nullptr) {
		view = glm::translate(glm::mat4{ 1 }, transform->GetPosition()) * cam_rot;
	}else {
		view = glm::translate(glm::mat4{ 1 }, glm::vec3(0.0f)) * cam_rot;
	}

	//we need to invert the camera matrix
	view = glm::inverse(view);

	return view;
}

glm::mat4 Camera::GetProjectionMatrix()
{
	aspect = (float)VulkanDevice::GetSwapchainExtent().width/(float)VulkanDevice::GetSwapchainExtent().height;
    glm::mat4 pro = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
    pro[1][1] *= -1;
    return pro;
}

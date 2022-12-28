#include "frustum.h"
#include "entity.h"
#include "components/camera.h"
#include "components/transform.h"
#include "components/mesh.h"

Frustum Frustum::createFrustumFromCamera(Entity& camEntity) {
    Frustum frustum;
    if(camEntity.GetComponent<Camera>() != nullptr && camEntity.GetComponent<Transform>() != nullptr){
        auto* camera = camEntity.GetComponent<Camera>();
        auto* transform = camEntity.GetComponent<Transform>();
        const float halfVSide = camera->zFar * tanf(glm::radians(camera->fov) * .5f);
        const float halfHSide = halfVSide * camera->aspect;
        const glm::vec3 frontMultFar = camera->zFar * camera->forward;

        frustum.nearFace = { transform->GetPosition() +  camera->zNear * camera->forward, camera->forward };
        frustum.farFace = { transform->GetPosition() + frontMultFar, -camera->forward };
        frustum.rightFace = { transform->GetPosition(),
                                glm::cross(camera->up,frontMultFar + camera->right * halfHSide) };
        frustum.leftFace = { transform->GetPosition(),
                                glm::cross(frontMultFar - camera->right * halfHSide, camera->up) };
        frustum.topFace = { transform->GetPosition(),
                                glm::cross(camera->right, frontMultFar - camera->up * halfVSide) };
        frustum.bottomFace = { transform->GetPosition(),
                                glm::cross(frontMultFar + camera->up * halfVSide, camera->right) };
    }

    return frustum;
}

bool Sphere::isOnFrustum(const Frustum &camFrustum, Entity& entity)const {
    //Get global scale thanks to our transform
    if(entity.GetComponent<Transform>() != nullptr && entity.GetComponent<Mesh>() != nullptr){
        auto* transform = entity.GetComponent<Transform>();
        auto* mesh = entity.GetComponent<Mesh>();
        const glm::vec3 globalScale = transform->GetScale();

        //Get our global center with process it with the global model matrix of our transform
        const glm::vec3 globalCenter{ mesh->modelMatrix * glm::vec4(center, 1.f) };

        //To wrap correctly our shape, we need the maximum scale scalar.
        const float maxScale = std::max(std::max(globalScale.x, globalScale.y), globalScale.z);

        //Max scale is assuming for the diameter. So, we need the half to apply it to our radius
        Sphere globalSphere(globalCenter, radius * (maxScale * 0.5f));

        //Check Firstly the result that have the most chance to faillure to avoid to call all functions.
        return (globalSphere.isOnOrForwardPlan(camFrustum.leftFace) &&
            globalSphere.isOnOrForwardPlan(camFrustum.rightFace) &&
            globalSphere.isOnOrForwardPlan(camFrustum.farFace) &&
            globalSphere.isOnOrForwardPlan(camFrustum.nearFace) &&
            globalSphere.isOnOrForwardPlan(camFrustum.topFace) &&
            globalSphere.isOnOrForwardPlan(camFrustum.bottomFace));
    }
    return false;
}

AABB AABB::generateAABB(Entity& entity)
{
    if(entity.GetComponent<Mesh>() != nullptr) {
        glm::vec3 minAABB = glm::vec3(std::numeric_limits<float>::max());
        glm::vec3 maxAABB = glm::vec3(std::numeric_limits<float>::min());
        for (auto&& vertex : entity.GetComponent<Mesh>()->vertices)
        {
            minAABB.x = std::min(minAABB.x, vertex.position.x);
            minAABB.y = std::min(minAABB.y, vertex.position.y);
            minAABB.z = std::min(minAABB.z, vertex.position.z);

            maxAABB.x = std::max(maxAABB.x, vertex.position.x);
            maxAABB.y = std::max(maxAABB.y, vertex.position.y);
            maxAABB.z = std::max(maxAABB.z, vertex.position.z);
        }
        return AABB(minAABB, maxAABB);
    }
    return AABB(glm::vec3(0.0f), glm::vec3(0.0f));
}

bool AABB::isOnFrustum(const Frustum& camFrustum, Entity& entity) const 
{
    if(entity.GetComponent<Transform>() != nullptr && entity.GetComponent<Mesh>() != nullptr){
        auto* transform = entity.GetComponent<Transform>();
        auto* mesh = entity.GetComponent<Mesh>();
		//Get global scale thanks to our transform
		const glm::vec3 globalCenter{ mesh->modelMatrix * glm::vec4(center, 1.f) };

		// Scaled orientation
		const glm::vec3 right = mesh->modelMatrix[0] * extents.x;
		const glm::vec3 up = mesh->modelMatrix[1] * extents.y;
		const glm::vec3 forward = -mesh->modelMatrix[2] * extents.z;

		const float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
			std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
			std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

		const float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
			std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
			std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

		const float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
			std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
			std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

		const AABB globalAABB(globalCenter, newIi, newIj, newIk);

		return (globalAABB.isOnOrForwardPlan(camFrustum.leftFace) &&
			globalAABB.isOnOrForwardPlan(camFrustum.rightFace) &&
			globalAABB.isOnOrForwardPlan(camFrustum.topFace) &&
			globalAABB.isOnOrForwardPlan(camFrustum.bottomFace) &&
			globalAABB.isOnOrForwardPlan(camFrustum.nearFace) &&
			globalAABB.isOnOrForwardPlan(camFrustum.farFace));
    }
    return false;
};
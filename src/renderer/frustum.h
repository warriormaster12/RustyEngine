#pragma once 

#include "glm.hpp"
#include <array>


struct Plan
{
    // unit vector
    glm::vec3 normal = { 0.f, 1.f, 0.f };

    // distance from origin to the nearest point in the plan
    float distance = 0.f;

    Plan() = default;

	Plan(const glm::vec3& p1, const glm::vec3& norm)
		: normal(glm::normalize(norm)),
		distance(glm::dot(normal, p1))
	{}

    float getSignedDistanceToPlan(const glm::vec3& point) const
	{
		return glm::dot(normal, point) - distance;
	}    

};


class Entity;

struct Frustum
{
    Plan topFace;
    Plan bottomFace;

    Plan rightFace;
    Plan leftFace;

    Plan farFace;
    Plan nearFace;
    Frustum createFrustumFromCamera(Entity& camEntity);
};


struct BoundingVolume
{
	virtual bool isOnFrustum(const Frustum& camFrustum, Entity& entity) const = 0;

	virtual bool isOnOrForwardPlan(const Plan& plan) const = 0;

	bool isOnFrustum(const Frustum& camFrustum) const
	{
		return (isOnOrForwardPlan(camFrustum.leftFace) &&
			isOnOrForwardPlan(camFrustum.rightFace) &&
			isOnOrForwardPlan(camFrustum.topFace) &&
			isOnOrForwardPlan(camFrustum.bottomFace) &&
			isOnOrForwardPlan(camFrustum.nearFace) &&
			isOnOrForwardPlan(camFrustum.farFace));
	};
};

struct Sphere : public BoundingVolume
{
	glm::vec3 center{ 0.f, 0.f, 0.f };
	float radius{ 0.f };

	Sphere(const glm::vec3& inCenter, float inRadius)
		: BoundingVolume{}, center{ inCenter }, radius{ inRadius }
	{}

	bool isOnOrForwardPlan(const Plan& plan) const final
	{
		return plan.getSignedDistanceToPlan(center) > -radius;
	}

	bool isOnFrustum(const Frustum& camFrustum, Entity& entity) const final;
};

struct AABB : public BoundingVolume
{
	
	glm::vec3 center{ 0.f, 0.f, 0.f };
	glm::vec3 extents{ 0.f, 0.f, 0.f };

	AABB(const glm::vec3& min, const glm::vec3& max)
		: BoundingVolume{}, center{ (max + min) * 0.5f }, extents{ max.x - center.x, max.y - center.y, max.z - center.z }
	{}

	AABB(const glm::vec3& inCenter, float iI, float iJ, float iK)
		: BoundingVolume{}, center{ inCenter }, extents{ iI, iJ, iK }
	{}

	std::array<glm::vec3, 8> getVertice() const
	{
		std::array<glm::vec3, 8> vertice;
		vertice[0] = { center.x - extents.x, center.y - extents.y, center.z - extents.z };
		vertice[1] = { center.x + extents.x, center.y - extents.y, center.z - extents.z };
		vertice[2] = { center.x - extents.x, center.y + extents.y, center.z - extents.z };
		vertice[3] = { center.x + extents.x, center.y + extents.y, center.z - extents.z };
		vertice[4] = { center.x - extents.x, center.y - extents.y, center.z + extents.z };
		vertice[5] = { center.x + extents.x, center.y - extents.y, center.z + extents.z };
		vertice[6] = { center.x - extents.x, center.y + extents.y, center.z + extents.z };
		vertice[7] = { center.x + extents.x, center.y + extents.y, center.z + extents.z };
		return vertice;
	}

	static AABB generateAABB(Entity& entity);

	//see https://gdbooks.gitbooks.io/3dcollisions/content/Chapter2/static_aabb_plan.html
	bool isOnOrForwardPlan(const Plan& plan) const final
	{
		// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
		const float r = extents.x * std::abs(plan.normal.x) + extents.y * std::abs(plan.normal.y) +
			extents.z * std::abs(plan.normal.z);

		return -r <= plan.getSignedDistanceToPlan(center);
	}

	bool isOnFrustum(const Frustum& camFrustum, Entity& entity) const final;
};
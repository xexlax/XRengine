#pragma once
#include <glm/glm.hpp>
namespace XRE {
	struct PhysicsMotion {
		bool needUpdate = false;
		glm::vec3 linear_velocity = glm::vec3(0.0f);
		glm::vec3 linear_acceleration = glm::vec3(0.0f);
		glm::vec3 angular_velocity = glm::vec3(0.0f);
		glm::vec3 angular_acceleration = glm::vec3(0.0f);

	};
}
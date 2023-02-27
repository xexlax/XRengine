#pragma once
#include"pch.h"

#include <glm\glm.hpp>
namespace XRE {
	glm::vec3 getPosition(const glm::mat4& trans) {
		glm::vec3 pos = {
			trans[0][3],trans[1][3],trans[2][3]
		};
		return pos;
	}

	glm::vec3 getPosition2(const glm::mat4& trans) {
		glm::vec3 pos = {
			trans[3][0],trans[3][1],trans[3][2]
		};
		return pos;
	}

	
}
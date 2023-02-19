#pragma once
#include <glm/glm.hpp>

class Color {
	Color(float r, float g, float b, float a = 1.0f);
	~Color() {};
	
public:
	glm::vec3 toVec3() { return glm::vec3(r, g, b); }
	glm::vec3 toVec4() { return glm::vec4(r, g, b, a ); }
	float r, g, b, a;

};
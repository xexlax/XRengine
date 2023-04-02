#include "JoltPhysicsScene.h"

JPH::Mat44 XRE::toMat44(const glm::mat4& m)
{
	JPH::Vec4 cols[4];
	for (int i = 0; i < 4; i++)
	{
		cols[i] = JPH::Vec4(m[0][i], m[1][i], m[2][i], m[3][i]);
	}

	return { cols[0], cols[1], cols[2], cols[3] };
}

glm::mat4 XRE::toMat44(const JPH::Mat44& m)
{
	glm::vec4 cols[4];
	for (int i = 0; i < 4; i++)
	{
		cols[i] = toVec4(m.GetColumn4(i));
	}

	return glm::transpose(glm::mat4(cols[0], cols[1], cols[2], cols[3]));
}

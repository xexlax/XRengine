#pragma once

#include <glm/glm.hpp>

namespace XRE {

    struct PhysicsHitInfo
    {
        glm::vec3 m_HitPos;
        glm::vec3 m_HitNormal;
        float m_Distance{ 0.f };
        uint32_t m_HitBodyID;
    };


}
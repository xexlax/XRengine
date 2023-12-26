#pragma once
#include "glm\glm.hpp"
namespace XRE {

    struct UniformBufferObject {
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

    struct d_light {
        glm::vec4 dir;
        glm::vec3 color;
        float intensity;
    };

    struct p_light {
        glm::vec4 pos;
        glm::vec3 color;
        float intensity;
    };

    struct LightingUBO {
        d_light d;
        p_light p[4];
        size_t plightCount;
    };

    struct MaterialUBO {
        int useAlbedo;
        int useSpecMap;
        int useNormalMap;
        float metallic;
        float roughness;
    };
}
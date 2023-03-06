#include "pch.h"
#include "stb_image.h"
#include "glad\glad.h"

namespace XRE {

	void CreateCubeMap(uint32_t& texture, std::vector<std::string>& faces) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

        int width, height, nrChannels;
        unsigned char* data;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            stbi_set_flip_vertically_on_load(0);
            data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
      
            XRE_CORE_ASSERT(data, "Failed to load image!");

            GLenum internalFormat = 0, dataFormat = 0;
            if (nrChannels == 4)
            {
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
            }
            else if (nrChannels == 3)
            {
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
            }
            if (data) {
                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_RGB, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data
                );
            }
            else {
                XRE_CORE_ERROR("立方体贴图无法加载:{0}", faces[i]);
                stbi_image_free(data);
            }

        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
}
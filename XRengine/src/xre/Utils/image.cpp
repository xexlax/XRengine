#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "image.h"

unsigned char* load_image( std::string path, int* width, int* height, int* channels, int desirechannels, bool flip) {
	if (flip == true)
		stbi_set_flip_vertically_on_load(1);
	return stbi_load(path.c_str(), width, height, channels, 0);
	//XRE_CORE_ASSERT(data, "Failed to load image!");
}

void image_free(unsigned char*& data) {
	stbi_image_free(data);
}

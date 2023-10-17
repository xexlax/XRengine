#pragma once


unsigned char* load_image(std::string path, int* width, int* height, int* channels, int desirechannels, bool flip);

void image_free(unsigned char*& data);
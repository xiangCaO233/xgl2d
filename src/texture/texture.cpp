#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "texture.h"

std::vector<GLuint> Texture::texunits;

Texture::Texture(const char *texpath) {}

Texture::~Texture() {}

#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "texture.h"

std::vector<Texture *> Texture::texunits;

Texture::Texture(const char *texpath) {
  // 使用前绑定对应mesh
  int twidth, theight;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(texpath, &twidth, &theight, &nrChannels, 0);
  width = twidth;
  height = theight;
  // 初始化纹理
  glGenTextures(1, &texture);
  // 0号纹理槽留白
  glActiveTexture(GL_TEXTURE0 + texunits.size() % 15 + 1);
  texid = texunits.size() + 1;
  glBindTexture(GL_TEXTURE_2D, texture);
  // 为当前绑定的纹理对象设置环绕、过滤方式
  // 重复绘制
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // 缩小使用临近过滤
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  // 放大使用线性过滤
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // 上传纹理数据
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);
  texunits.push_back(this);
  // 释放纹理源数据
  stbi_image_free(data);
}
Texture::Texture() {
  width = 0;
  height = 0;
  texid = 0;
}

Texture::~Texture() {}

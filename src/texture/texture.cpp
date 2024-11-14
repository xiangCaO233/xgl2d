#include <iostream>
#include <string>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "texture.h"

std::vector<Texture *> Texture::textures;

void Texture::activatetexunits() {
  if (textures.size() <= 15) {
    for (int i = 0; i < textures.size(); i++) {
      glActiveTexture(GL_TEXTURE1 + i);
    }
  } else {
    // 大于15个纹理，激活全部纹理槽
    for (int i = 0; i < 15; i++) {
      glActiveTexture(GL_TEXTURE1 + i);
    }
  }
};

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
  texid = textures.size() + 1;
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
  textures.push_back(this);
  // 释放纹理源数据
  stbi_image_free(data);
}
Texture::Texture() {
  // 1*1白色纹理
  width = 1;
  height = 1;
  texid = 0;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  std::cout << "deftexture obj:" << std::to_string(texture) << std::endl;
  // 创建一个 1x1 像素的白色纹理数据
  unsigned char whitePixel[3] = {255, 255, 255}; // RGB 全部设为 255，表示白色
  // 将数据加载到纹理中
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE,
               whitePixel);
  // 设置纹理参数
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // 初始化完成,解绑
  glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {}

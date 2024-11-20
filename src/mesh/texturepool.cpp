#include "texturepool.h"
#include "stb_image.h"
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

Texturepool::Texturepool(int max_tex_count) : _max_tex_count(max_tex_count) {
  glGenBuffers(1, &TBO);
  glGenTextures(1, &texture_atlas);
  stbi_set_flip_vertically_on_load(true);
}
Texturepool::Texturepool(const char *texturedir) {
  glGenBuffers(1, &TBO);
  glGenTextures(1, &texture_atlas);
  glBindTexture(GL_TEXTURE_2D, texture_atlas);
  // 为当前绑定的纹理对象设置环绕、过滤方式
  // 重复绘制
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // 缩小使用临近过滤
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  // 放大使用线性过滤
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // 反转y轴,合乎gl礼
  stbi_set_flip_vertically_on_load(true);
  loadtexture(texturedir);
}
Texturepool::~Texturepool() {
  glDeleteTextures(1, &texture_atlas);
  glDeleteBuffers(1, &TBO);
  // 释放分配的meta
  for (auto &meta : _texmetas) {
    delete meta.second;
  };
}
void Texturepool::bind() { glBindTexture(GL_TEXTURE_2D, TBO); };
void Texturepool::unbind() { glBindTexture(GL_TEXTURE_2D, 0); };

void Texturepool::loadtexture(const char *texturedir) {
  if (!std::filesystem::exists(texturedir)) {
    // 非法路径
    throw std::runtime_error("illegal texture file");
  } else if (std::filesystem::is_regular_file(texturedir)) {
    std::cout << "读取纹理: " << std::filesystem::absolute(texturedir);
    int twidth, theight, nrChannels;
    unsigned char *data =
        stbi_load(std::filesystem::absolute(texturedir).c_str(), &twidth,
                  &theight, &nrChannels, 0);
    // 读取到表中
    auto meta = new TextureMeta();
    meta->name = texturedir;
    meta->width = twidth;
    meta->height = theight;
    meta->woffset = 0;
    meta->hoffset = 0;
    meta->texid = _texmetas.size();
    _texmetas[texturedir] = meta;
    _texdatas[meta] = data;
  } else if (std::filesystem::is_directory(texturedir)) {
    // 遍历目录中的所有文件和文件夹
    for (const auto &entry :
         std::filesystem::recursive_directory_iterator(texturedir)) {
      if (std::filesystem::is_regular_file(entry)) {
        // 获取文件的绝对路径
        std::cout << "读取纹理: " << std::filesystem::absolute(entry.path())
                  << std::endl;
        int twidth, theight, nrChannels;
        unsigned char *data =
            stbi_load(std::filesystem::absolute(entry.path()).c_str(), &twidth,
                      &theight, &nrChannels, 0);
        // 读取到表中
        auto meta = new TextureMeta();
        meta->name = entry.path();
        meta->width = twidth;
        meta->height = theight;
        meta->woffset = 0;
        meta->hoffset = 0;
        meta->texid = _texmetas.size();
        _texmetas[entry.path().c_str()] = meta;
        _texdatas[meta] = data;
      }
    }
  } else {
    std::cout << "dir empty" << std::endl;
  }
}
void Texturepool::creatatlas() {
  is_done = true;
  for (const auto &pair : _texmetas) {
    // 将图像合并
    insert(pair.second);
  }
  // 释放纹理数据
  for (auto &pair : _texmetas) {
    std::cout << pair.first
              << " respos:[x:" + std::to_string(pair.second->woffset) +
                     ",y:" + std::to_string(pair.second->hoffset) + "]"
              << std::endl;
    stbi_image_free(_texdatas[pair.second]);
  }
};

// 插入新纹理
bool Texturepool::insert(TextureMeta *result) { return true; }

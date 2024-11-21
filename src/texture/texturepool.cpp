#include "texturepool.h"
#include "MaxRectsBinPack.h"
#include "stb_image.h"
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

Texturepool::Texturepool(std::string &texturedir, Shader *shader)
    : _shader(shader) {
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

  // 生成默认白色纹理
  // RGBA(255, 255, 255, 255)
  unsigned char whitePixel[4] = {255, 255, 255, 255};
  _defmeta = std::make_shared<TextureMeta>("defmeta", 1, 1, 0, 0);
  _texmetas[_defmeta->name] = _defmeta;
  _texdatas[_defmeta] = whitePixel;

  // 加载纹理文件夹
  loadtexture(texturedir);
}
Texturepool::~Texturepool() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glDeleteTextures(1, &texture_atlas);
  glDeleteBuffers(1, &TBO);
}
void Texturepool::bind() {
  glBindTexture(GL_TEXTURE_2D, texture_atlas);
  glActiveTexture(GL_TEXTURE0);
  _shader->set_sampler("sampler", 0);
};
void Texturepool::unbind() { glBindTexture(GL_TEXTURE_2D, 0); };

void Texturepool::loadtexture(std::string &texturedir) {
  if (!is_done) {
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
      auto meta = std::make_shared<TextureMeta>();
      meta->name = texturedir;
      meta->width = twidth;
      meta->height = theight;
      meta->woffset = 0;
      meta->hoffset = 0;
      meta->metaid = _texmetas.size();
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
              stbi_load(std::filesystem::absolute(entry.path()).c_str(),
                        &twidth, &theight, &nrChannels, 0);
          // 读取到表中
          auto meta = std::make_shared<TextureMeta>();
          meta->name = entry.path();
          meta->width = twidth;
          meta->height = theight;
          meta->woffset = 0;
          meta->hoffset = 0;
          meta->metaid = _texmetas.size();
          _texmetas[entry.path().c_str()] = meta;
          _texdatas[meta] = data;
        }
      }
    } else {
      std::cout << "dir empty" << std::endl;
    }
  } else {
    std::cout << "generate is already done, load failed" << std::endl;
  }
}
void Texturepool::creatatlas() {
  is_done = true;
  MaxRectsBinPack pack = MaxRectsBinPack();
  for (const auto &pair : _texmetas) {
    // 将图像合并
    pack.Insert(pair.second, MaxRectsBinPack::RectBottomLeftRule);
  }
  // 生成atlas图像
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pack.binWidth, pack.binHeight, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  _vwidth = pack.binWidth;
  _vheight = pack.binHeight;
  // 填充并释放纹理数据
  for (auto &pair : _texmetas) {
    std::cout << pair.first
              << "res tex pos:[x:" + std::to_string(pair.second->woffset) +
                     ",y:" + std::to_string(pair.second->hoffset) + "]"
              << std::endl;
    // 放置纹理到atlas
    glTexSubImage2D(GL_TEXTURE_2D, 0, pair.second->woffset,
                    pair.second->hoffset, pair.second->width,
                    pair.second->height, GL_RGBA, GL_UNSIGNED_BYTE,
                    _texdatas[pair.second]);
  }
  std::cout << "res atlas size:[" + std::to_string(pack.binWidth) + "x" +
                   std::to_string(pack.binHeight) + "]"
            << std::endl;
  std::cout << "usage:[" + std::to_string(pack.Occupancy()) + "]" << std::endl;
  std::cout << "generate done" << std::endl;
  is_done = true;
};

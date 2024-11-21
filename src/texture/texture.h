#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../include/core/glcore.h"

#include <vector>
class Texture {
  // 纹理单元集
  static std::vector<Texture *> textures;
  static void activatetexunits();

  friend class Mesh;

public:
  GLuint texture;
  // 构造Texture
  Texture();
  Texture(const char *texpath);
  // 析构Texture
  virtual ~Texture();
  // 纹理id(GPU用于索引的id),颜色通道数
  int texid, nrChannels;
  float width, height;
};

#endif /* TEXTURE_H */

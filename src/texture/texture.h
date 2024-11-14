#ifndef TEXTURE_H
#define TEXTURE_H
#include "../../include/core/glcore.h"

#include <vector>
class Texture {
  GLuint texture;
  // 纹理单元集
  static std::vector<Texture *> textures;
  static void activatetexunits();

  friend class Xmesh;
  friend class XquadMesh;
  friend class Quad;

public:
  // 构造Texture
  Texture();
  Texture(const char *texpath);
  // 析构Texture
  virtual ~Texture();
  // 纹理id,颜色通道数
  int texid, nrChannels;
  float width, height;
};

#endif /* TEXTURE_H */

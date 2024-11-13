#ifndef TEXTURE_H
#define TEXTURE_H
#include "../../include/core/glcore.h"

#include <vector>
class Texture {
  // 纹理单元集
  static std::vector<GLuint> texunits;

public:
  // 构造Texture
  Texture(const char *texpath);
  // 析构Texture
  virtual ~Texture();
  int texid;
  float width, height;
};

#endif /* TEXTURE_H */

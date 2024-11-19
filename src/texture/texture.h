#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../include/core/glcore.h"

#include <vector>
enum TexType {
  // 直接填充纹理
  FILL,
  // 直接重复纹理(默认的左下角起点)
  REAPEAT,
  // 直接重复纹理(以中心点扩散)
  REAPEAT_BY_CENTER,
  // 直接适应宽度并纵向重复(同样默认左下角)
  FIT_WIDTH_AND_REPEAT,
  // 直接适应高度并横向重复(同样默认左下角)
  FIT_HEIGHT_AND_REPEAT,
  // 直接适应宽度并纵向重复(以中心点扩散)
  FIT_WIDTH_AND_REPEAT_BY_CENTER,
  // 直接适应高度并横向重复(以中心点扩散)
  FIT_HEIGHT_AND_REPEAT_BY_CENTER
};
class Texture {
  // 纹理单元集
  static std::vector<Texture *> textures;
  static void activatetexunits();

  friend class Xmesh;
  friend class XquadMesh;
  friend class Xovalmesh;
  friend class XQuad;
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

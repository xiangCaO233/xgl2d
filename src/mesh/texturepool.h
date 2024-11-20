#ifndef TEXTUREPOOL_H
#define TEXTUREPOOL_H

#include "../../include/core/glcore.h"
#include <cstdint>
#include <unordered_map>
#include <vector>
struct Rect {
  int x, y, width, height;

  Rect(int x, int y, int width, int height)
      : x(x), y(y), width(width), height(height) {}
};
struct TextureMeta {
  // 纹理文件名
  const char *name;
  // 纹理尺寸
  int width, height;
  // 纹理宽高的偏移
  int woffset, hoffset;
  // 纹理id
  float texid;
  bool operator==(const TextureMeta &other) const {
    return name == other.name && width == other.width &&
           height == other.height && woffset == other.woffset &&
           hoffset == other.hoffset && texid == other.texid;
  }
};

// 纹理图集
class Texturepool {
  // 纹理缓冲对象
  GLuint TBO, texture_atlas;
  // 虚拟纹理尺寸
  uint32_t _vwidth, _vheight;
  // 是否加载完成
  bool is_done{false};
  // 纹理元数据集
  std::unordered_map<const char *, TextureMeta *> _texmetas;
  // 纹理数据集
  std::unordered_map<TextureMeta *, unsigned char *> _texdatas;
  std::vector<Rect> freeDimensions;
  // maxrectangl算法
  bool insert(int width, int height, TextureMeta *result);
  bool canInsert(int width, int height);
  void mergeFreeRectangles();
  bool canMerge(const Rect &r1, const Rect &r2);
  Rect merge(const Rect &r1, const Rect &r2);
  void expandAtlas();

public:
  // 构造Texturepool
  Texturepool(uint32_t vwidth = 4096, uint32_t vheight = 4096);
  // 直接读取纹理文件夹
  Texturepool(const char *texturedir);
  // 析构Texturepool
  virtual ~Texturepool();

  // 绑定纹理池
  void bind();
  void unbind();

  // 加载纹理文件或文件夹
  void loadtexture(const char *texturedir);

  void creatatlas();
};

#endif /* TEXTUREPOOL_H */

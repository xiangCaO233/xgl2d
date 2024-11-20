#ifndef TEXTUREPOOL_H
#define TEXTUREPOOL_H

#include "../../include/core/glcore.h"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
struct Rect {
  int x, y, width, height;

  Rect(int x, int y, int width, int height)
      : x(x), y(y), width(width), height(height) {}
};
struct TextureMeta {
  // 纹理文件名
  std::string name;
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
  uint32_t _vwidth{1024}, _vheight{1024};
  int _max_tex_count{128};
  // 是否加载完成
  bool is_done{false};
  // 纹理元数据集
  std::unordered_map<std::string, TextureMeta *> _texmetas;
  // 纹理数据集
  std::unordered_map<TextureMeta *, unsigned char *> _texdatas;
  // 合并纹理集算法
  std::vector<Rect> freeDimensions;
  bool insert(TextureMeta *tex);
  void expandAtlas();

public:
  // 构造Texturepool
  Texturepool(int max_tex_count);
  // 直接读取纹理文件夹
  Texturepool(const char *texturedir);
  // 析构Texturepool
  virtual ~Texturepool();

  // 绑定纹理池
  void bind();
  void unbind();

  // 加载纹理文件或文件夹
  void loadtexture(const char *texturedir);

  // 构造图集
  void creatatlas();
};

#endif /* TEXTUREPOOL_H */

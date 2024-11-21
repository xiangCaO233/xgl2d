#ifndef TEXTUREPOOL_H
#define TEXTUREPOOL_H

#include "../../include/core/glcore.h"
#include "shader/shader.h"
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

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
  // 对应着色器
  Shader *_shader;
  // 虚拟纹理尺寸
  uint32_t _vwidth, _vheight;
  // 是否加载完成
  bool is_done{false};
  // 纹理元数据集
  std::unordered_map<std::string, std::shared_ptr<TextureMeta>> _texmetas;
  // 纹理数据集
  std::unordered_map<std::shared_ptr<TextureMeta>, unsigned char *> _texdatas;

public:
  // 构造Texturepool
  // 直接读取纹理文件夹
  Texturepool(const char *texturedir, Shader *shader);
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

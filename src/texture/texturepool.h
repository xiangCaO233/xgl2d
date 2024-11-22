#ifndef TEXTUREPOOL_H
#define TEXTUREPOOL_H

#include "../../include/core/glcore.h"
#include "shader/shader.h"
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

enum TexType {
  // 直接填充纹理[0]
  FILL,
  // 直接重复纹理(默认的左下角起点)[1]
  REAPEAT,
  // 直接重复纹理(以中心点扩散)[2]
  REAPEAT_BY_CENTER,
  // 直接适应宽度并纵向重复(同样默认左下角)[3]
  FIT_WIDTH_AND_REPEAT,
  // 直接适应高度并横向重复(同样默认左下角)[4]
  FIT_HEIGHT_AND_REPEAT,
  // 直接适应宽度并纵向重复(以中心点扩散)[5]
  FIT_WIDTH_AND_REPEAT_BY_CENTER,
  // 直接适应高度并横向重复(以中心点扩散)[6]
  FIT_HEIGHT_AND_REPEAT_BY_CENTER
};

struct TextureMeta {
  // 纹理文件名
  std::string name;
  // 纹理尺寸
  int width, height;
  // 纹理宽高的偏移
  int woffset, hoffset;
  // 纹理id
  float metaid;
  // 在TBO中的偏移
  bool operator==(const TextureMeta &other) const {
    return name == other.name && width == other.width &&
           height == other.height && woffset == other.woffset &&
           hoffset == other.hoffset && metaid == other.metaid;
  }
};

// 纹理图集
class Texturepool {
  // 纹理缓冲对象
  GLuint UTBO{}, texture_atlas{};
  // 对应着色器
  Shader *_shader;
  // 虚拟纹理尺寸
  uint32_t _vwidth{}, _vheight{};
  // 是否加载完成
  bool is_done{false};
  std::shared_ptr<TextureMeta> _defmeta;
  // 纹理元数据集
  std::unordered_map<std::string, std::shared_ptr<TextureMeta>> _texmetas;
  std::unordered_map<int, std::shared_ptr<TextureMeta>> _texmetas_by_index;
  std::vector<std::shared_ptr<TextureMeta>> _metalist;
  // 纹理数据集
  std::unordered_map<std::shared_ptr<TextureMeta>, unsigned char *> _texdatas;

  friend class Mesh;

public:
  // 构造Texturepool
  // 直接读取纹理文件夹
  Texturepool(std::string &texturedir, Shader *shader);
  // 析构Texturepool
  virtual ~Texturepool();

  // 绑定纹理池
  void bind();
  void unbind();

  // 加载纹理文件或文件夹
  void loadtexture(std::string &texturedir);

  // 构造图集
  void creatatlas();

  std::shared_ptr<TextureMeta> operator[](const char *name) {
    std::string metaname = std::string(name);
    auto it = _texmetas.find(metaname);
    if (it != _texmetas.end()) {
      // std::cout << "找到meta:[" << name << "]" << std::endl;
      return it->second;
    } else {
      std::cout << "无法找到[" << name << "]" << std::endl;
      return _defmeta;
    }
  }
  std::shared_ptr<TextureMeta> operator[](int index) {
    auto it = _texmetas_by_index.find(index);
    if (it != _texmetas_by_index.end()) {
      // std::cout << "找到meta:[" << it->second->name << "]" << std::endl;
      return it->second;
    } else {
      std::cout << "无法找到[" << index << "]" << std::endl;
      return _defmeta;
    }
  }
};

#endif /* TEXTUREPOOL_H */

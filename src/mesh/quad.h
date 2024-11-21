#ifndef QUAD_H
#define QUAD_H

#include "../texture/texture.h"
#include "texture/texturepool.h"
#include <memory>

class Quad {

  // 基本数据
  glm::vec2 _cp;
  float _w, _h;
  float _rotation{0.0f};
  glm::vec4 _color;
  std::shared_ptr<Texture> _tex;
  std::shared_ptr<TextureMeta> _texmeta;
  // 纹理变换矩阵
  glm::mat3 _uvtransform = glm::mat3(1.0f);
  TexType _textype;

  // 绘制顺序
  uint32_t _draw_order{0};

  // 在模型缓冲区中的偏移
  uint32_t model_data_offset{0};

  friend class Mesh;

public:
  // 构造Quad
  Quad(glm::vec2 &cp, float w, float h, float rotation, glm::vec4 &color,
       std::shared_ptr<Texture> &tex, TexType texture_type)
      : _cp(cp), _w(w), _h(h), _rotation(rotation), _color(color), _tex(tex) {
    settextyppe(texture_type);
  };
  // 析构Quad
  virtual ~Quad() = default;

  // 重新设置纹理填充方式
  void settextyppe(TexType texture_type) {
    _textype = texture_type;
    switch (_textype) {
    case FILL: {
      _uvtransform = glm::mat3(1.0f);
      break;
    }
    default:
      break;
    }
  }
  void rotate(float rotate) { _rotation = rotate; }
  // 重写==运算符
  inline bool operator==(const Quad &q) const {
    return _cp == q._cp && _w == q._w && _h == q._h &&
           _rotation == q._rotation && _color == q._color && _tex == q._tex &&
           _textype == q._textype;
  };
  inline bool equals(glm::vec2 &cp, float w, float h, float rotation,
                     glm::vec4 &color, std::shared_ptr<Texture> &tex,
                     TexType texture_type) {
    return _cp == cp && _w == w && _h == h && _rotation == rotation &&
           _color == color && _tex == tex && _textype == texture_type;
  }
};
#endif /* QUAD_H */

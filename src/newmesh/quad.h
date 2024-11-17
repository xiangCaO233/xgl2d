#ifndef QUAD_H
#define QUAD_H

#include "../texture/texture.h"
#include <memory>

class Quad {

  // 基本数据
  glm::vec2 _cp;
  float w, h;
  glm::vec4 _color;
  std::shared_ptr<Texture> _tex;
  TexType _textype;

  // 是否需要更新的标识
  bool _is_update;

  // 绘制顺序
  int _draw_order;

  // 在模型缓冲区中的偏移
  uint32_t model_data_offset[4]{0, 0, 0, 0};

  friend class Mesh;

public:
  // 构造Quad
  Quad(glm::vec2 &cp, float w, float h, glm::vec4 &color, Texture &tex,
       TexType texture_type);
  // 析构Quad
  virtual ~Quad();

  // 重写==运算符
  inline bool operator==(const Quad &q) const {
    return _cp == q._cp && _color == q._color && _tex == q._tex &&
           _textype == q._textype;
  };
  // 判断属性是否相同
  inline bool iscpequal(const Quad &q) const { return _cp == q._cp; }
  inline bool iswhequal(const Quad &q) const { return w == q.w && h == q.h; }
  inline bool iscolorequal(const Quad &q) const { return _color == q._color; }
  inline bool istexequal(const Quad &q) const { return _tex == q._tex; }
  inline bool isuvequal(const Quad &q) const { return _textype == q._textype; }
  inline bool iscpequal(glm::vec2 &__cp) const { return _cp == __cp; }
  inline bool iswhequal(float &__w, float &__h) const {
    return w == __w && h == __h;
  }
  inline bool iscolorequal(glm::vec4 &__color) const {
    return _color == __color;
  }
  inline bool istexequal(std::shared_ptr<Texture> &__tex) const {
    return _tex == __tex;
  }
  inline bool isuvequal(TexType &__textype) const {
    return _textype == __textype;
  }
};
#endif /* QUAD_H */

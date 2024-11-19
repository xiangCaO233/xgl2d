#ifndef QUAD_H
#define QUAD_H

#include "../texture/texture.h"
#include <memory>

class Quad {

  // 基本数据
  glm::vec2 _cp;
  float _w, _h;
  float _rotation{0.0f};
  glm::vec4 _color;
  std::shared_ptr<Texture> _tex;
  // 左下到左上逆时针顺序存储纹理坐标
  glm::vec2 _uv1;
  glm::vec2 _uv2;
  glm::vec2 _uv3;
  glm::vec2 _uv4;
  TexType _textype;

  // 绘制顺序
  uint32_t _draw_order{0};

  // 在模型缓冲区中的偏移
  uint32_t model_data_offset[7]{0, 0, 0, 0, 0, 0, 0};

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
      _uv1.x = 0.0f;
      _uv1.y = 0.0f;
      _uv2.x = 1.0f;
      _uv2.y = 0.0f;
      _uv3.x = 1.0f;
      _uv3.y = 1.0f;
      _uv4.x = 0.0f;
      _uv4.y = 1.0f;
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
  // 判断属性是否相同
  [[nodiscard]] inline bool isrotationequal(const Quad &q) const {
    return _rotation == q._rotation;
  }
  [[nodiscard]] inline bool iscpequal(const Quad &q) const {
    return _cp == q._cp;
  }
  [[nodiscard]] inline bool iswhequal(const Quad &q) const {
    return _w == q._w && _h == q._h;
  }
  [[nodiscard]] inline bool iscolorequal(const Quad &q) const {
    return _color == q._color;
  }
  [[nodiscard]] inline bool istexequal(const Quad &q) const {
    return _tex == q._tex;
  }
  [[nodiscard]] inline bool isuvequal(const Quad &q) const {
    return _textype == q._textype;
  }
  [[nodiscard]] inline bool iscpequal(glm::vec2 &_cp_) const {
    return _cp == _cp_;
  }
  inline bool iswhequal(float &_w_, float &_h_) const {
    return _w == _w_ && _h == _h_;
  }
  [[nodiscard]] inline bool isrotationequal(float _rotation_) const {
    return _rotation == _rotation_;
  }
  inline bool iscolorequal(glm::vec4 &_color_) const {
    return _color == _color_;
  }
  inline bool istexequal(std::shared_ptr<Texture> &_tex_) const {
    return _tex == _tex_;
  }
  inline bool isuvequal(TexType &_textype_) const {
    return _textype == _textype_;
  }
};
#endif /* QUAD_H */

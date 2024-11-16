#ifndef QUAD_H
#define QUAD_H

#include "../../texture/texture.h"
#include "Shape.h"
#include <memory>

class Quad : public Shape {

  // 矩形宽高
  float width, height;
  // 矩形的对应变换矩阵
  glm::mat4 _translation = glm::mat4(1.0f);

  friend class XquadMesh;

public:
  Quad(glm::vec2 &cp, float width, float height);
  Quad(glm::vec2 &&cp, float width, float height);
  Quad(glm::vec2 &cp, float width, float height, glm::vec4 &color);
  Quad(glm::vec2 &cp, float width, float height,
       const std::shared_ptr<Texture> &texture, TexType texture_type);
  Quad(glm::vec2 &cp, float width, float height, glm::vec4 &color,
       std::shared_ptr<Texture> texture, TexType texture_type);
  virtual ~Quad();
};

class Linestrip : public Quad {
  // 长度
  float linelength{};
  // 线宽
  float linewidth{};

public:
  // 构造Linestrip
  Linestrip(glm::vec2 &p1, glm::vec2 &p2, float width);
  Linestrip(glm::vec2 &p1, glm::vec2 &p2, float width, glm::vec4 &color);
  Linestrip(glm::vec2 &sp, float length, float degrees, float width);
  Linestrip(glm::vec2 &sp, float length, float degrees, float width,
            glm::vec4 &color);
  // 析构Linestrip
  virtual ~Linestrip();
};
#endif /* QUAD_H */

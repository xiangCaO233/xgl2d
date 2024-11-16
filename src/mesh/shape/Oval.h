#ifndef OVAL_H
#define OVAL_H

#include "Shape.h"
#include <memory>

class Oval : public Shape {
  // 割数
  int _segment;
  // x半径和y半径
  int _rx, _ry;

public:
  // 构造Oval
  Oval(glm::vec2 &cp, float rx, float ry, int segment, glm::vec4 &color,
       std::shared_ptr<Texture> texture, TexType texture_type);
  // 析构Oval
  virtual ~Oval() = default;
};
#endif /* OVAL_H */

#include "Oval.h"

Oval::Oval(glm::vec2 &cp, float rx, float ry, int segment, glm::vec4 &color,
           std::shared_ptr<Texture> texture, TexType texture_type)
    : Shape(2 + segment), _segment(segment), _rx(rx), _ry(ry) {
  // 开头两个顶点在中间
  _vertices[0].position = glm::vec3(cp, 0.0f);
  _vertices[0].texid = texture->texid;
  _vertices[0].color = color;
  tex = texture;
  switch (texture_type) {
  case FILL: {
    _vertices[0].uv = {0.5f, 0.5f};
    break;
  }
  default: {
    _vertices[0].uv = {0.5f, 0.5f};
    break;
  }
  }
  for (int i = 0; i <= segment; i++) {
    float angle = 2.0f * M_PI * float(i) / float(segment);
    _vertices[1 + i].position = {cp.x + cos(angle) * rx, cp.y + sin(angle) * ry,
                                 0.0f};
    _vertices[1 + i].texid = texture->texid;
    _vertices[1 + i].color = color;
    switch (texture_type) {
    case FILL: {
      _vertices[1 + i].uv = {cos(angle) + 0.5f, sin(angle) + 0.5f};
      break;
    }
    default: {
      _vertices[1 + i].uv = {0.5f, 0.5f};
      break;
    }
    }
  }
}

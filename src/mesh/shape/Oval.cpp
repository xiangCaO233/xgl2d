#include "Oval.h"

Oval::Oval(glm::vec2 &cp, float rx, float ry, int segment, glm::vec4 &color)
    : Shape(1 + segment), _segment(segment), _rx(rx), _ry(ry) {
  _vertices[0].position = glm::vec3(cp, 0.0f);
}

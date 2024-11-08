#ifndef XMESH_H
#define XMESH_H

#include "../../include/core/glcore.h"
#include <vector>

class Xmesh {
  // 缓冲对象
  GLuint VBO, VAO, EBO, FBO;
  // mesh数据
  std::vector<glm::vec3> _positions;
  std::vector<glm::vec4> _colors;
  std::vector<glm::vec2> _uvs;

public:
  // 构造Xmesh
  Xmesh();
  // 析构Xmesh
  virtual ~Xmesh();

  void load(std::vector<std::vector<float>> &vertices_data);
};

#endif /* XMESH_H */

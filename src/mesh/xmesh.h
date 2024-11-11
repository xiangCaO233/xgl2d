#ifndef XMESH_H
#define XMESH_H

#include "../../include/core/glcore.h"
#include "../texture/texture.h"
#include <cstdint>
#include <vector>

struct Vertex {
  // 顶点元数据
  glm::vec3 position{0.0f, 0.0f, 0.0f};
  glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec2 uv{0.0f, 0.0f};
  // 顶点材质
  float texid{0.0f};
  std::vector<float> dump() {
    std::vector<float> data;
    data.push_back(position.x);
    data.push_back(position.y);
    data.push_back(position.z);
    data.push_back(color.r);
    data.push_back(color.g);
    data.push_back(color.b);
    data.push_back(color.a);
    data.push_back(uv.x);
    data.push_back(uv.y);
    data.push_back(texid);
    return data;
  }
};

class Xmesh {
protected:
  // 缓冲对象
  // VertexBufferObject VertexArrayObject
  // ElementBufferObject
  GLuint VBO, VAO, EBO;
  // mesh顶点数量分配
  uint32_t _vcount_size;
  // mesh顶点缓冲对象实际分配大小(VBO)(byte)
  uint64_t _vbuffer_size;
  // mesh顶点索引数量分配
  uint32_t _velecount_size;
  // mesh顶点索引缓冲对象实际分配大小(EBO)(byte)
  uint64_t _velebuffer_size;

public:
  // 构造Xmesh
  Xmesh(uint32_t vcount_size = 1024);
  // 析构Xmesh
  virtual ~Xmesh();

  // 绑定mesh缓冲
  void bind();
  // 解绑mesh缓冲
  void unbind();
};

#endif /* XMESH_H */

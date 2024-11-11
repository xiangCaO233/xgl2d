#ifndef XMESH_H
#define XMESH_H

#include "../../include/core/glcore.h"
#include "../texture/texture.h"
#include <cstdint>

struct Vertex {
  // 顶点元数据
  glm::vec3 position;
  glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec2 uv{0.0f, 0.0f};
  // 顶点材质
  Texture *tex;
};

class Xmesh {
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

  // 创建矩形
  void creatquad(float x, float y, float width, float height, glm::vec4 &color,
                 float texid = -1);
};

#endif /* XMESH_H */

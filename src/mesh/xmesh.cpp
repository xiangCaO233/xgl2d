#include "xmesh.h"
#include <cstddef>

Xmesh::Xmesh(uint32_t vcount_size)
    : _vcount_size(vcount_size), _vbuffer_size(vcount_size * sizeof(Vertex)),
      _velecount_size(_vcount_size * 2),
      _velebuffer_size(_velecount_size * sizeof(uint32_t)) {
  // 在初始化Xmesh之前确保gl已初始化
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &VAO);
  glGenBuffers(1, &EBO);

  // 先绑定
  bind();

  // 初始化VBO大小(使用空指针初始化数据)
  glBufferData(GL_VERTEX_ARRAY, _vbuffer_size, nullptr, GL_DYNAMIC_DRAW);
  // 初始化EBO大小
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, _velebuffer_size, nullptr,
               GL_DYNAMIC_DRAW);

  // 顶点描述
  // 启用layout0
  glEnableVertexAttribArray(0);
  // 描述lauout0为顶点坐标
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (const void *)offsetof(Vertex, position));
  // 启用layout1
  glEnableVertexAttribArray(1);
  // 描述lauout1为顶点颜色
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (const void *)offsetof(Vertex, color));
  // 启用layout2
  glEnableVertexAttribArray(2);
  // 描述lauout2为顶点uv
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (const void *)offsetof(Vertex, uv));
  // 启用layout3
  glEnableVertexAttribArray(2);
  // 描述lauout3为顶点uv
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (const void *)offsetof(Vertex, texid));

  // 初始化操作完成，解绑
  unbind();
}

Xmesh::~Xmesh() {
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &VAO);
}

void Xmesh::bind() {
  // 绑定VAO，VBO
  glBindVertexArray(VAO);
  glBindBuffer(GL_VERTEX_ARRAY, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}
void Xmesh::unbind() {
  // 绑定VAO，VBO
  glBindVertexArray(0);
  glBindBuffer(GL_VERTEX_ARRAY, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

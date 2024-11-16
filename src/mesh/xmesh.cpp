#include "xmesh.h"
#include "shape/Shape.h"
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>

Xmesh::Xmesh(Shader *shader, int max_texture_unit, uint32_t vcount_size)
    : _vcount_size(vcount_size), _vbuffer_size(vcount_size * sizeof(Vertex)),
      _velecount_size(_vcount_size * 2),
      _velebuffer_size(_velecount_size * sizeof(uint32_t)), _shader(shader) {
  // 在初始化Xmesh之前确保gl已初始化
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // 先绑定
  glBindVertexArray(VAO);

  deftexture = std::make_shared<Texture>();
  // 默认纹理绑定
  glBindTexture(GL_TEXTURE_2D, deftexture->texture);
  // 激活纹理单元0
  glActiveTexture(GL_TEXTURE0);
  // 在着色器中设置纹理单元的索引
  shader->set_unfm1i("samplers[0]", deftexture->texid);

  // 初始化VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // 初始化VBO大小(使用空指针初始化数据)
  glBufferData(GL_ARRAY_BUFFER, _vbuffer_size, nullptr, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
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
  glEnableVertexAttribArray(3);
  // 描述lauout3为顶点变换矩阵索引
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (const void *)offsetof(Vertex, matindex));
  // 启用layout4
  glEnableVertexAttribArray(4);
  // 描述lauout3为顶点材质索引
  glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (const void *)offsetof(Vertex, texid));

  // 初始化操作完成，解绑
}

Xmesh::~Xmesh() {
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &VAO);
}

void Xmesh::bind() {
  // 绑定VAO，VBO
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}
void Xmesh::unbind() {
  // 绑定VAO，VBO
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

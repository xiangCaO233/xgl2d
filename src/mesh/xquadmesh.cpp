#include "xquadmesh.h"
#include <vector>

XquadMesh::XquadMesh(uint32_t qcount)
    : Xmesh(qcount * 4), _qcount_size(qcount) {
  // 初始化矩形的绘制索引表
  bind();
  auto indicies = std::vector<uint32_t>();
  for (int i = 0; i < qcount; i++) {
    int offset = i * 4;
    indicies.push_back(offset);
    indicies.push_back(offset + 1);
    indicies.push_back(offset + 2);
    indicies.push_back(offset + 2);
    indicies.push_back(offset + 3);
    indicies.push_back(offset);
  }
  // 预填充quad索引数据
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                  indicies.size() * sizeof(uint32_t), indicies.data());

  // 初始化变换矩阵缓冲对象(共qcount个四阶矩阵)
  glGenBuffers(1, &TBO);
  std::vector<glm::mat4> matdata(qcount, glm::mat4(1.0f));
  glBufferData(GL_ARRAY_BUFFER, qcount * sizeof(glm::mat4), matdata.data(),
               GL_DYNAMIC_DRAW);
  // 配置矩阵属性
  for (int i = 0; i < 4; i++) {
    glEnableVertexAttribArray(4 + i);
    glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (void *)(i * sizeof(glm::vec4)));
    glVertexAttribDivisor(4 + i, 1);
  }

  unbind();
}

XquadMesh::~XquadMesh() {
  Xmesh::~Xmesh();
  glDeleteBuffers(1, &TBO);
}

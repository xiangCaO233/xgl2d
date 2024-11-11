#include "xquadmesh.h"
#include <vector>

Quad::Quad(float width, float height) {
  bottom_left = new Vertex;
  bottom_left->position = {-width / 2, -height / 2, 0.0f};
  bottom_right = new Vertex;
  bottom_right->position = {width / 2, -height / 2, 0.0f};
  top_right = new Vertex;
  top_right->position = {width / 2, height / 2, 0.0f};
  top_left = new Vertex;
  top_left->position = {-width / 2, height / 2, 0.0f};
}
Quad::Quad(float width, float height, glm::vec4 &color) : Quad(width, height) {
  bottom_left->color = color;
  bottom_right->color = color;
  top_right->color = color;
  top_left->color = color;
};
Quad::Quad(float width, float height, glm::vec2 &uv, Texture *texture)
    : Quad(width, height) {
  bottom_left->uv = uv;
  bottom_left->tex = texture;
  bottom_right->uv = uv;
  bottom_right->tex = texture;
  top_right->uv = uv;
  top_right->tex = texture;
  top_left->uv = uv;
  top_left->tex = texture;
};

Quad::~Quad() {}

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
// 使用左下角为基准构建矩形
void XquadMesh::newquad(float x, float y, float width, float height,
                        glm::vec4 &color) {
  auto quad = new Quad(width, height, color);
  quad->translate(glm::translate(glm::mat4(1.0f), {}));
  _quads.push_back(quad);
};
void XquadMesh::newquad(float x, float y, float width, float height,
                        glm::vec2 &uv, Texture *texture) {
  auto quad = new Quad(width, height, uv, texture);
};

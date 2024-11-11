#include "xquadmesh.h"
#include <iostream>
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
Quad::Quad(float width, float height, glm::vec2 &uv, int texid)
    : Quad(width, height) {
  bottom_left->uv = uv;
  bottom_left->texid = texid;
  bottom_right->uv = uv;
  bottom_right->texid = texid;
  top_right->uv = uv;
  top_right->texid = texid;
  top_left->uv = uv;
  top_left->texid = texid;
};

Quad::~Quad() {}
std::vector<float> Quad::dump() {
  std::vector<float> data;
  auto bldata = bottom_left->dump();
  auto brdata = bottom_right->dump();
  auto trdata = top_right->dump();
  auto tldata = top_left->dump();
  data.insert(data.end(), bldata.begin(), bldata.end());
  data.insert(data.end(), brdata.begin(), brdata.end());
  data.insert(data.end(), trdata.begin(), trdata.end());
  data.insert(data.end(), tldata.begin(), tldata.end());
  return data;
};
// 应用变换
void Quad::scale(float scalerate) {
  auto res = *_translation * scalerate;
  _translation = &res;
};
void Quad::translate(glm::vec3 &trans) {
  auto res = *_translation * glm::translate(glm::mat4(1.0f), trans);
  _translation = &res;
};
void Quad::rotate(float rad, glm::vec3 &axis){};

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
  bind();
  auto quad = new Quad(width, height, color);
  glm::vec3 movement = {x, y, 0.0f};
  quad->translate(movement);
  // 同步显存
  // 顶点数据
  glBufferSubData(GL_ARRAY_BUFFER, _quads.size() * 4 * sizeof(Vertex),
                  4 * sizeof(Vertex), quad->dump().data());
  // 矩阵变换数据
  glBindBuffer(GL_ARRAY_BUFFER, TBO);
  /*
glBufferSubData(GL_ARRAY_BUFFER, _quads.size() * 4 * sizeof(glm::vec4),
            4 * sizeof(glm::vec4), quad->_translation);
                                                                  */
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  _quads.push_back(quad);
  unbind();
};
void XquadMesh::newquad(float x, float y, float width, float height,
                        glm::vec2 &uv, int texid) {
  bind();
  auto quad = new Quad(width, height, uv, texid);
  glm::vec3 movement = {x, y, 0.0f};
  quad->translate(movement);
  // 同步显存
  glBufferSubData(GL_ARRAY_BUFFER, _quads.size() * 4 * sizeof(Vertex),
                  4 * sizeof(Vertex), quad->dump().data());
  // 矩阵变换数据
  glBindBuffer(GL_ARRAY_BUFFER, TBO);
  glBufferSubData(GL_ARRAY_BUFFER, _quads.size() * 4 * sizeof(glm::vec4),
                  4 * sizeof(glm::vec4), quad->_translation);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  _quads.push_back(quad);
  unbind();
};

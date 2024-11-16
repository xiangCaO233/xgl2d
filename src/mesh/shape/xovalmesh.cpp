#include "xovalmesh.h"

Xovalmesh::Xovalmesh(Shader *shader, int max_texture_unit, int ovalcount,
                     int defsegment)
    : Xmesh(shader, max_texture_unit, ovalcount * (1 + defsegment)),
      _defsegment(defsegment) {}

Xovalmesh::~Xovalmesh() = default;

// 绘制椭圆
void Xovalmesh::drawoval(glm::vec2 &cp, float rx, float ry, glm::vec4 &color,
                         glm::vec2 &screensize) {
  drawoval(cp, rx, ry, color, deftexture, TexType::FILL, screensize);
};
void Xovalmesh::drawoval(glm::vec2 &cp, float rx, float ry, glm::vec4 &&color,
                         glm::vec2 &screensize) {
  drawoval(cp, rx, ry, color, screensize);
};
void Xovalmesh::drawoval(glm::vec2 &&cp, float rx, float ry, glm::vec4 &color,
                         glm::vec2 &screensize) {
  drawoval(cp, rx, ry, color, screensize);
};
void Xovalmesh::drawoval(glm::vec2 &&cp, float rx, float ry, glm::vec4 &&color,
                         glm::vec2 &screensize) {
  drawoval(cp, rx, ry, color, screensize);
};
void Xovalmesh::drawoval(glm::vec2 &cp, float rx, float ry,
                         std::shared_ptr<Texture> texture, TexType textype,
                         glm::vec2 &screensize) {
  drawoval(cp, rx, ry, {1.0f, 1.0f, 1.0f, 1.0f}, texture, textype, screensize);
};
void Xovalmesh::drawoval(glm::vec2 &&cp, float rx, float ry,
                         std::shared_ptr<Texture> texture, TexType textype,
                         glm::vec2 &screensize) {
  drawoval(cp, rx, ry, texture, textype, screensize);
};
void Xovalmesh::drawoval(glm::vec2 &cp, float rx, float ry, glm::vec4 &color,
                         std::shared_ptr<Texture> texture, TexType textype,
                         glm::vec2 &screensize){
    // 实现绘制
    // 生成顶点数据

};
void Xovalmesh::drawoval(glm::vec2 &cp, float rx, float ry, glm::vec4 &&color,
                         std::shared_ptr<Texture> texture, TexType textype,
                         glm::vec2 &screensize) {
  drawoval(cp, rx, ry, color, texture, textype, screensize);
};
void Xovalmesh::drawoval(glm::vec2 &&cp, float rx, float ry, glm::vec4 &color,
                         std::shared_ptr<Texture> texture, TexType textype,
                         glm::vec2 &screensize) {

  drawoval(cp, rx, ry, color, texture, textype, screensize);
};
void Xovalmesh::drawoval(glm::vec2 &&cp, float rx, float ry, glm::vec4 &&color,
                         std::shared_ptr<Texture> texture, TexType textype,
                         glm::vec2 &screensize) {
  drawoval(cp, rx, ry, color, texture, textype, screensize);
};

// 绘制圆
void Xovalmesh::drawcircle(glm::vec2 &cp, float r, glm::vec4 &color,
                           glm::vec2 &screensize) {
  drawoval(cp, r, r, color, screensize);
};
void Xovalmesh::drawcircle(glm::vec2 &cp, float r, glm::vec4 &&color,
                           glm::vec2 &screensize) {
  drawcircle(cp, r, color, screensize);
};
void Xovalmesh::drawcircle(glm::vec2 &&cp, float r, glm::vec4 &color,
                           glm::vec2 &screensize) {
  drawcircle(cp, r, color, screensize);
};
void Xovalmesh::drawcircle(glm::vec2 &&cp, float r, glm::vec4 &&color,
                           glm::vec2 &screensize) {
  drawcircle(cp, r, color, screensize);
};
void Xovalmesh::drawcircle(glm::vec2 &cp, float r,
                           std::shared_ptr<Texture> texture, TexType textype,
                           glm::vec2 &screensize) {
  drawoval(cp, r, r, texture, textype, screensize);
};
void Xovalmesh::drawcircle(glm::vec2 &&cp, float r,
                           std::shared_ptr<Texture> texture, TexType textype,
                           glm::vec2 &screensize) {
  drawcircle(cp, r, texture, textype, screensize);
};
void Xovalmesh::drawcircle(glm::vec2 &cp, float r, glm::vec4 &color,
                           std::shared_ptr<Texture> texture, TexType textype,
                           glm::vec2 &screensize) {
  drawoval(cp, r, r, color, texture, textype, screensize);
};
void Xovalmesh::drawcircle(glm::vec2 &cp, float r, glm::vec4 &&color,
                           std::shared_ptr<Texture> texture, TexType textype,
                           glm::vec2 &screensize) {
  drawcircle(cp, r, color, texture, textype, screensize);
};
void Xovalmesh::drawcircle(glm::vec2 &&cp, float r, glm::vec4 &color,
                           std::shared_ptr<Texture> texture, TexType textype,
                           glm::vec2 &screensize) {
  drawcircle(cp, r, color, texture, textype, screensize);
};
void Xovalmesh::drawcircle(glm::vec2 &&cp, float r, glm::vec4 &&color,
                           std::shared_ptr<Texture> texture, TexType textype,
                           glm::vec2 &screensize) {
  drawcircle(cp, r, color, texture, textype, screensize);
};
void Xovalmesh::finish(){};

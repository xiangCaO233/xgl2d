#ifndef XQUADMESH_H
#define XQUADMESH_H

#include "../xmesh.h"
#include <vector>

class Quad {
  // 矩形四顶点
  Vertex *bottom_left;
  Vertex *bottom_right;
  Vertex *top_right;
  Vertex *top_left;
  // 矩形在当前meshVBO中的显存偏移
  uint32_t meshOffset;
  // 矩形的对应变换矩阵(默认单位矩阵)
  glm::mat4 *_translation = new glm::mat4(1.0f);

public:
  Quad(float width, float height);
  Quad(float width, float height, glm::vec4 &color);
  Quad(float width, float height, glm::vec2 &uv, Texture *texture);
  ~Quad();

  // 设置颜色
  void setfill(glm::vec4 &color);
  // 设置矩形材质(默认拉伸)
  void settexture(Texture *texture);
  // 重置材质uv(自定义贴图坐标)
  void setquaduv(glm::vec2 &bottom_left, glm::vec2 &bottom_right,
                 glm::vec2 &top_right, glm::vec2 &top_left);

  // 应用变换
  void scale(float scalerate);
  void translate(glm::vec3 &trans);
  void rotate(float rad, glm::vec3 &axis);
};

class XquadMesh : public Xmesh {
  // 矩阵缓冲对象
  GLuint TBO;
  uint32_t _qcount_size;
  // quadmesh中的全部quad(矩形)
  std::vector<Quad *> _quads;

public:
  // 构造XquadMesh
  XquadMesh(uint32_t qcount = 256);
  // 析构XquadMesh
  ~XquadMesh() override;

  // 使用左下角为基准构建矩形
  void newquad(float x, float y, float width, float height, glm::vec4 &color);
  void newquad(float x, float y, float width, float height, glm::vec2 &uv,
               Texture *texture);
};
#endif /* XQUADMESH_H */

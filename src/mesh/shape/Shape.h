#ifndef SHAPE_H
#define SHAPE_H

#include "../../../include/core/glcore.h"
#include "../../texture/texture.h"
#include <memory>

struct Vertex {
  // 顶点元数据
  glm::vec3 position{0.0f, 0.0f, 0.0f};
  glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec2 uv{0.0f, 0.0f};
  // 矩阵变换索引
  float matindex{0};
  // 顶点材质
  float texid{0.0f};
  float *dump() const {
    float *data = new float[11];
    data[0] = position.x;
    data[1] = position.y;
    data[2] = position.z;
    data[3] = color.r;
    data[4] = color.g;
    data[5] = color.b;
    data[6] = color.a;
    data[7] = uv.x;
    data[8] = uv.y;
    data[9] = matindex;
    data[10] = texid;
    return data;
  }
  // dump出变换后的顶点数据
  float *dump(glm::mat4 transform) const {
    float *data = new float[11];
    glm::vec4 position4 = glm::vec4(position, 1.0f);
    // 变换顶点
    position4 = transform * position4;
    data[0] = position4.x;
    data[1] = position4.y;
    data[2] = position4.z;
    data[3] = color.r;
    data[4] = color.g;
    data[5] = color.b;
    data[6] = color.a;
    data[7] = uv.x;
    data[8] = uv.y;
    data[9] = matindex;
    data[10] = texid;
    return data;
  }
  bool operator==(const Vertex &v) const {
    return position == v.position && color == v.color && uv == v.uv &&
           matindex == v.matindex && texid == v.texid;
  };
};

class Shape {
  // 顶点集
  Vertex *_vertices;
  int _vcount;
  // 变换矩阵(默认单位矩阵)
  glm::mat4 _transform = glm::mat4(1.0f);
  // 形状绑定的纹理
  std::shared_ptr<Texture> tex;
  // 绘制顺序
  uint32_t draworder{0};

  friend class Oval;
  friend class Quad;
  friend class XquadMesh;
  friend class Xovalmesh;
  friend class Linestrip;

public:
  // 构造Shape
  Shape(int vcount);
  // 析构Shape
  virtual ~Shape();
  // 导出数据(src是否经过变换)
  float *dump(bool src);

  void setmatindex(unsigned int index);
  // 设置颜色
  void setfill(glm::vec4 &color);
  // 设置矩形材质(默认拉伸)
  void settexture(const std::shared_ptr<Texture> &texture);
  // 重置材质uv(自定义贴图坐标)
  void setquaduv(glm::vec2 &bottom_left, glm::vec2 &bottom_right,
                 glm::vec2 &top_right, glm::vec2 &top_left);

  // 应用变换
  void scale(float scalerate);
  void translate(glm::vec3 &trans);
  void translate(glm::vec3 &&trans);
  void rotate(float degrees, glm::vec3 &axis);
  void rotate(float degrees, glm::vec3 &&axis);

  bool screen_is_contain(glm::vec2 &screen) const;

  bool operator==(const Shape &quad);
};

#endif /* SHAPE_H */

#include "Shape.h"

Shape::Shape(int vcount) : _vcount(vcount), _vertices(new Vertex[vcount]) {}

Shape::~Shape() { delete[] _vertices; };

bool Shape::operator==(const Shape &shape) {
  return _vertices == shape._vertices;
};
float *Shape::dump(bool src) {
  float *data = new float[_vcount * 11];
  if (src) {
    // dump源顶点数据
    for (int i = 0; i < _vcount; i++) {
      float *tempdata = _vertices[i].dump();
      std::copy(tempdata, tempdata + 11, data + 11 * i);
      delete[] tempdata;
    }
  } else {
    // dump应用变换后的顶点数据
    for (int i = 0; i < _vcount; i++) {
      float *tempdata = _vertices[i].dump(_transform);
      std::copy(tempdata, tempdata + 11, data + 11 * i);
      delete[] tempdata;
    }
  }
  return data;
};
void Shape::setmatindex(unsigned int index) {
  _vertices[0].matindex = index;
  _vertices[1].matindex = index;
  _vertices[2].matindex = index;
  _vertices[3].matindex = index;
};
// 设置颜色
void Shape::setfill(glm::vec4 &color) {
  _vertices[0].color = color;
  _vertices[1].color = color;
  _vertices[2].color = color;
  _vertices[3].color = color;
};
// 设置矩形材质(默认拉伸)
void Shape::settexture(const std::shared_ptr<Texture> &texture) {
  tex = texture;
  _vertices[0].texid = texture->texid;
  _vertices[1].texid = texture->texid;
  _vertices[2].texid = texture->texid;
  _vertices[3].texid = texture->texid;
};
// 重置材质uv(自定义贴图坐标)
void Shape::setquaduv(glm::vec2 &bottom_leftuv, glm::vec2 &bottom_rightuv,
                      glm::vec2 &top_rightuv, glm::vec2 &top_leftuv) {
  _vertices[0].uv = bottom_leftuv;
  _vertices[1].uv = bottom_rightuv;
  _vertices[2].uv = top_rightuv;
  _vertices[3].uv = top_leftuv;
};
// 应用变换
// 放缩
void Shape::scale(float scalerate) { _transform = _transform * scalerate; };
// 平移
void Shape::translate(glm::vec3 &trans) {
  _transform = _transform * glm::translate(glm::mat4(1.0f), trans);
};
void Shape::translate(glm::vec3 &&trans) { translate(trans); }
// 旋转
void Shape::rotate(float degrees, glm::vec3 &axis) {
  // 角度转换为弧度
  float angle = glm::radians(degrees);
  // 创建旋转矩阵
  glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, axis);
  _transform = _transform * rotationMatrix;
};
void Shape::rotate(float degrees, glm::vec3 &&axis) { rotate(degrees, axis); }
bool Shape::screen_is_contain(glm::vec2 &screen) const { return false; };

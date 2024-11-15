#ifndef XQUADMESH_H
#define XQUADMESH_H

#include "../xmesh.h"
#include <memory>
#include <unordered_map>
#include <vector>
enum TexType { FILL_TO_QUAD, REAPEAT_TO_QUAD };
class Quad {
  // 矩形四顶点
  Vertex *bottom_left;
  Vertex *bottom_right;
  Vertex *top_right;
  Vertex *top_left;
  float width, height;
  // 矩形的对应变换矩阵(默认单位矩阵)
  glm::mat4 _translation = glm::mat4(1.0f);
  // 矩形的纹理
  std::shared_ptr<Texture> quadTex;
  uint32_t draworder;

  friend class XquadMesh;

public:
  Quad(glm::vec2 &cp, float width, float height);
  Quad(glm::vec2 &&cp, float width, float height);
  Quad(glm::vec2 &cp, float width, float height, glm::vec4 &color);
  Quad(glm::vec2 &cp, float width, float height,
       std::shared_ptr<Texture> texture, TexType texture_type);
  Quad(glm::vec2 &cp, float width, float height, glm::vec4 &color,
       std::shared_ptr<Texture> texture, TexType texture_type);
  ~Quad();

  // 导出数据(src是否经过变换)
  std::vector<float> dump(bool src);

  void setmatindex(int index);
  // 设置颜色
  void setfill(glm::vec4 &color);
  // 设置矩形材质(默认拉伸)
  void settexture(std::shared_ptr<Texture> texture);
  // 重置材质uv(自定义贴图坐标)
  void setquaduv(glm::vec2 &bottom_left, glm::vec2 &bottom_right,
                 glm::vec2 &top_right, glm::vec2 &top_left);

  // 应用变换
  void scale(float scalerate);
  void translate(glm::vec3 &trans);
  void translate(glm::vec3 &&trans);
  void rotate(float degrees, glm::vec3 &axis);
  void rotate(float degrees, glm::vec3 &&axis);

  bool isequaln(const Quad &quad);
  bool operator==(const Quad &quad);
};

class Linestrip : public Quad {
  // 长度
  float linelength;
  // 线宽
  float linewidth;

public:
  // 构造Linestrip
  Linestrip(glm::vec2 &p1, glm::vec2 &p2, float width);
  Linestrip(glm::vec2 &p1, glm::vec2 &p2, float width, glm::vec4 &color);
  Linestrip(glm::vec2 &sp, float length, float degrees, float width);
  Linestrip(glm::vec2 &sp, float length, float degrees, float width,
            glm::vec4 &color);
  // 析构Linestrip
  virtual ~Linestrip();
};

class XquadMesh : public Xmesh {
  uint32_t _qcount_size;

  int _max_texid{0};
  // quadmesh中的全部quad(矩形本身)
  std::vector<Quad *> _quads;
  // 将要绘制的矩形
  std::vector<Quad *> _should_draw_quads;
  // 矩形在列表中的索引(快速查找在绘制长宽比相同的矩形数据哈希表)
  std::unordered_map<Quad *, int> _quad_indicies;
  // 矩形绘制权重(权重为0时，将矩形列表末尾的矩形移交到此0权矩形处,包括显存空间哈希表)
  std::unordered_map<Quad *, int> _quad_draw_weight;

  bool screencontainquad(float x, float y, float width, float height,
                         glm::vec2 &screensize);

public:
  // 构造XquadMesh
  XquadMesh(Shader *shader, uint32_t qcount = 256);
  // 析构XquadMesh
  virtual ~XquadMesh() override;

  // 使用前绑定本mesh
  // 矩形中心点+尺寸(仅填充颜色)
  void drawquad(glm::vec2 &cp, float width, float height, glm::vec4 &color,
                glm::vec2 &screensize);
  void drawquad(glm::vec2 &cp, float width, float height, glm::vec4 &&color,
                glm::vec2 &screensize);
  void drawquad(glm::vec2 &&cp, float width, float height, glm::vec4 &color,
                glm::vec2 &screensize);
  void drawquad(glm::vec2 &&cp, float width, float height, glm::vec4 &&color,
                glm::vec2 &screensize);
  // 仅纹理
  void drawquad(glm::vec2 &cp, float width, float height,
                std::shared_ptr<Texture> texture, TexType texture_type,
                glm::vec2 &screensize);
  void drawquad(glm::vec2 &&cp, float width, float height,
                std::shared_ptr<Texture> texture, TexType texture_type,
                glm::vec2 &screensize);
  // 矩形中心点+尺寸(颜色+纹理混合)
  void drawquad(glm::vec2 &cp, float width, float height, glm::vec4 &color,
                std::shared_ptr<Texture> texture, TexType texture_type,
                glm::vec2 &screensize);
  void drawquad(glm::vec2 &cp, float width, float height, glm::vec4 &&color,
                std::shared_ptr<Texture> texture, TexType texture_type,
                glm::vec2 &screensize);
  void drawquad(glm::vec2 &&cp, float width, float height, glm::vec4 &color,
                std::shared_ptr<Texture> texture, TexType texture_type,
                glm::vec2 &screensize);
  void drawquad(glm::vec2 &&cp, float width, float height, glm::vec4 &&color,
                std::shared_ptr<Texture> texture, TexType texture_type,
                glm::vec2 &screensize);
  // 两点确定线段
  void drawlinestrip(glm::vec2 &p1, glm::vec2 &p2, float width,
                     glm::vec4 &color, glm::vec2 &screensize);

  void drawlinestrip(glm::vec2 &p1, glm::vec2 &&p2, float width,
                     glm::vec4 &&color, glm::vec2 &screensize);
  void drawlinestrip(glm::vec2 &&p1, glm::vec2 &p2, float width,
                     glm::vec4 &&color, glm::vec2 &screensize);
  void drawlinestrip(glm::vec2 &&p1, glm::vec2 &&p2, float width,
                     glm::vec4 &color, glm::vec2 &screensize);

  void drawlinestrip(glm::vec2 &p1, glm::vec2 &p2, float width,
                     glm::vec4 &&color, glm::vec2 &screensize);
  void drawlinestrip(glm::vec2 &p1, glm::vec2 &&p2, float width,
                     glm::vec4 &color, glm::vec2 &screensize);
  void drawlinestrip(glm::vec2 &&p1, glm::vec2 &p2, float width,
                     glm::vec4 &color, glm::vec2 &screensize);

  void drawlinestrip(glm::vec2 &&p1, glm::vec2 &&p2, float width,
                     glm::vec4 &&color, glm::vec2 &screensize);

  // 射线
  void drawlinestrip(glm::vec2 &sp, float length, float degrees, float width,
                     glm::vec4 &color, glm::vec2 &screensize);

  void drawlinestrip(glm::vec2 &sp, float length, float degrees, float width,
                     glm::vec4 &&color, glm::vec2 &screensize);
  void drawlinestrip(glm::vec2 &&sp, float length, float degrees, float width,
                     glm::vec4 &color, glm::vec2 &screensize);
  void drawlinestrip(glm::vec2 &&sp, float length, float degrees, float width,
                     glm::vec4 &&color, glm::vec2 &screensize);

  // 完成此次绘制(实际绘制,包括传输顶点数据)
  void finish();

  inline unsigned int size() { return _quads.size(); }

  // 使用左下角为基准构建矩形
  void newquad(Quad *quad);
};
#endif /* XQUADMESH_H */

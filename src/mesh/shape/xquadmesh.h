#ifndef XQUADMESH_H
#define XQUADMESH_H

#include "../xmesh.h"
#include "Quad.h"
#include <vector>

class QuadBatch {
  // 该批次中的全部矩形
  std::vector<std::shared_ptr<Quad>> batch;
  // 此批次中的纹理批次索引
  int texture_batch_index{0};

  friend class XquadMesh;

public:
  // 构造QuadBatch
  QuadBatch(){};
  // 析构QuadBatch
  virtual ~QuadBatch() = default;
};

class XquadMesh : public Xmesh {
  uint32_t _qcount_size;

  // 全部批次
  std::vector<std::shared_ptr<QuadBatch>> _all_batchs;
  int _max_texid{0};
  // 将要绘制的矩形
  std::vector<std::shared_ptr<Quad>> _should_draw_quads;
  // 判断屏幕中是否包含矩形
  static bool screencontainquad(float x, float y, float width, float height,
                                glm::vec2 &screensize);

public:
  // 构造XquadMesh
  XquadMesh(Shader *shader, int max_texture_unit, uint32_t qcount = 256);
  // 析构XquadMesh
  ~XquadMesh() override = default;

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
                const std::shared_ptr<Texture> &texture, TexType texture_type,
                glm::vec2 &screensize);
  void drawquad(glm::vec2 &&cp, float width, float height,
                const std::shared_ptr<Texture> &texture, TexType texture_type,
                glm::vec2 &screensize);
  // 矩形中心点+尺寸(颜色+纹理混合)
  void drawquad(glm::vec2 &cp, float width, float height, glm::vec4 &color,
                const std::shared_ptr<Texture> &texture, TexType texture_type,
                glm::vec2 &screensize);
  void drawquad(glm::vec2 &cp, float width, float height, glm::vec4 &&color,
                const std::shared_ptr<Texture> &texture, TexType texture_type,
                glm::vec2 &screensize);
  void drawquad(glm::vec2 &&cp, float width, float height, glm::vec4 &color,
                const std::shared_ptr<Texture> &texture, TexType texture_type,
                glm::vec2 &screensize);
  void drawquad(glm::vec2 &&cp, float width, float height, glm::vec4 &&color,
                const std::shared_ptr<Texture> &texture, TexType texture_type,
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
  void finish() override;
};
#endif /* XQUADMESH_H */

#ifndef XOVALMESH
#define XOVALMESH

#include "../xmesh.h"
#include "Oval.h"

class OvalBatch {
  // 该批次中的全部矩形
  std::vector<std::shared_ptr<Oval>> batch;
  // 此批次中的纹理批次索引
  int texture_batch_index{0};

  friend class Xovalmesh;

public:
  // 构造OvalBatch
  OvalBatch(){};
  // 析构OvalBatch
  virtual ~OvalBatch() = default;
};

class Xovalmesh : public Xmesh {
  // 实例缓冲区(单独绘制每个椭圆)
  GLuint instanceVBO;
  // 默认割数
  int _defsegment;

  // 全部批次
  std::vector<std::shared_ptr<OvalBatch>> _all_batchs;
  int _max_texid{0};
  // 将要绘制的椭圆
  std::vector<std::shared_ptr<Oval>> _should_draw_ovals;

  // 判断屏幕中是否包含椭圆
  static bool screencontainoval(float x, float y, float rx, float ry,
                                glm::vec2 &screensize);

public:
  // 构造Xovalmesh
  Xovalmesh(Shader *shader, int max_texture_unit, int ovalcount,
            int defsegment = 8);
  // 析构Xovalmesh
  ~Xovalmesh() override;

  // 绘制椭圆
  void drawoval(glm::vec2 &cp, float rx, float ry, glm::vec4 &color,
                glm::vec2 &screensize);
  void drawoval(glm::vec2 &cp, float rx, float ry, glm::vec4 &&color,
                glm::vec2 &screensize);
  void drawoval(glm::vec2 &&cp, float rx, float ry, glm::vec4 &color,
                glm::vec2 &screensize);
  void drawoval(glm::vec2 &&cp, float rx, float ry, glm::vec4 &&color,
                glm::vec2 &screensize);
  void drawoval(glm::vec2 &cp, float rx, float ry,
                std::shared_ptr<Texture> texture, TexType textype,
                glm::vec2 &screensize);
  void drawoval(glm::vec2 &&cp, float rx, float ry,
                std::shared_ptr<Texture> texture, TexType textype,
                glm::vec2 &screensize);
  void drawoval(glm::vec2 &cp, float rx, float ry, glm::vec4 &color,
                std::shared_ptr<Texture> texture, TexType textype,
                glm::vec2 &screensize);
  void drawoval(glm::vec2 &cp, float rx, float ry, glm::vec4 &&color,
                std::shared_ptr<Texture> texture, TexType textype,
                glm::vec2 &screensize);
  void drawoval(glm::vec2 &&cp, float rx, float ry, glm::vec4 &color,
                std::shared_ptr<Texture> texture, TexType textype,
                glm::vec2 &screensize);
  void drawoval(glm::vec2 &&cp, float rx, float ry, glm::vec4 &&color,
                std::shared_ptr<Texture> texture, TexType textype,
                glm::vec2 &screensize);
  // 绘制圆
  void drawcircle(glm::vec2 &cp, float r, glm::vec4 &color,
                  glm::vec2 &screensize);
  void drawcircle(glm::vec2 &cp, float r, glm::vec4 &&color,
                  glm::vec2 &screensize);
  void drawcircle(glm::vec2 &&cp, float r, glm::vec4 &color,
                  glm::vec2 &screensize);
  void drawcircle(glm::vec2 &&cp, float r, glm::vec4 &&color,
                  glm::vec2 &screensize);
  void drawcircle(glm::vec2 &cp, float r, std::shared_ptr<Texture> texture,
                  TexType textype, glm::vec2 &screensize);
  void drawcircle(glm::vec2 &&cp, float r, std::shared_ptr<Texture> texture,
                  TexType textype, glm::vec2 &screensize);
  void drawcircle(glm::vec2 &cp, float r, glm::vec4 &color,
                  std::shared_ptr<Texture> texture, TexType textype,
                  glm::vec2 &screensize);
  void drawcircle(glm::vec2 &cp, float r, glm::vec4 &&color,
                  std::shared_ptr<Texture> texture, TexType textype,
                  glm::vec2 &screensize);
  void drawcircle(glm::vec2 &&cp, float r, glm::vec4 &color,
                  std::shared_ptr<Texture> texture, TexType textype,
                  glm::vec2 &screensize);
  void drawcircle(glm::vec2 &&cp, float r, glm::vec4 &&color,
                  std::shared_ptr<Texture> texture, TexType textype,
                  glm::vec2 &screensize);

  void finish() override;
};

#endif /* XOVALMESH */

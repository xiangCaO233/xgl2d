#ifndef XOVALMESH
#define XOVALMESH

#include "../xmesh.h"

class Xovalmesh : public Xmesh {
  int _defsegment;

public:
  // 构造Xovalmesh
  Xovalmesh(Shader *shader, int max_texture_unit, int ovalcount,
            int defsegment);
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

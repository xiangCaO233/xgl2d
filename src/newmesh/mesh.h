#ifndef MESHBASE
#define MESHBASE

#include "../../include/core/glcore.h"
#include "quad.h"
#include "shader/shader.h"
#include <cstdint>
#include <memory>
#include <vector>

enum DataType { POSITION, SIZE, ROTATION, COLOR, UV, TEXID };

class QuadBatch {
  // 该批次中的全部矩形
  std::vector<std::shared_ptr<Quad>> batch;
  // 此批次中的纹理批次索引
  int texture_batch_index{0};
  friend class Mesh;

public:
  // 构造QuadBatch
  QuadBatch(){};
  // 析构QuadBatch
  virtual ~QuadBatch() = default;
};

class OvalBatch {
  // 该批次中的全部椭圆
  std::vector<std::shared_ptr<Quad>> batch;
  // 此批次中的纹理批次索引
  int texture_batch_index{0};
  friend class Mesh;

public:
  // 构造OvalBatch
  OvalBatch(){};
  // 析构OvalBatch
  virtual ~OvalBatch() = default;
};
class Mesh {
  // 缓冲对象
  // VBO:顶点数据缓冲对象
  // VAO:顶点数组缓冲对象
  // FBO:帧缓冲对象
  // TBO:纹理贴图缓冲对象
  // (
  // quad_instanceVBOs[0]:x,y中心坐标,
  // quad_instanceVBOs[1]:尺寸
  // quad_instanceVBOs[2]:旋转角度,
  // quad_instanceVBOs[3]:r,g,b,a颜色,
  // quad_instanceVBOs[4]:u,v贴图坐标,
  // quad_instanceVBOs[5]:texid贴图索引
  // )
  GLuint VBO{0}, VAO{0}, quad_instanceVBOs[6]{0}, FBO{0}, TBO{0};
  int max_quad_count;
  Shader *_shader;
  // 矩形批次
  std::vector<std::shared_ptr<QuadBatch>> _all_batchs;
  int _max_texid{0};
  int _max_texture_unit;
  // 包含的图形
  std::vector<std::shared_ptr<Quad>> _quads;
  // 每一段要更新的连续的矩形和数据
  std::vector<std::vector<std::shared_ptr<Quad>>> _update_consequent_quads_poss;
  std::vector<std::vector<float>> _update_consequent_quads_pos_datas;
  std::vector<std::vector<std::shared_ptr<Quad>>>
      _update_consequent_quads_sizes;
  std::vector<std::vector<float>> _update_consequent_quads_size_datas;
  std::vector<std::vector<std::shared_ptr<Quad>>>
      _update_consequent_quads_rotations;
  std::vector<std::vector<float>> _update_consequent_quads_rotation_datas;
  std::vector<std::vector<std::shared_ptr<Quad>>>
      _update_consequent_quads_colors;
  std::vector<std::vector<float>> _update_consequent_quads_color_datas;
  std::vector<std::vector<std::shared_ptr<Quad>>> _update_consequent_quads_uvs;
  std::vector<std::vector<float>> _update_consequent_quads_uv_datas;
  std::vector<std::vector<std::shared_ptr<Quad>>>
      _update_consequent_quads_texids;
  std::vector<std::vector<float>> _update_consequent_quads_texid_datas;
  // 当前正在处理的矩形下标
  uint32_t _current_handle_index{0};
  // 默认1*1白色纹理
  std::shared_ptr<Texture> _deftexture;

  void updatequadbatch(std::shared_ptr<Quad> &quad);

  void updateinstanceoffset(uintptr_t instance_offset = 0);

public:
  // 构造Mesh
  explicit Mesh(Shader *shader, int max_texture_unit,
                int initial_quad_count = 4096);
  // 析构Mesh
  virtual ~Mesh();

  // 绑定mesh缓冲
  void bind();
  // 解绑mesh缓冲
  void unbind();

  // 绘制矩形(材质默认混合颜色并填充到矩形)
  void drawquad(glm::vec2 &cp, float w, float h, float rotation,
                glm::vec4 &color, std::shared_ptr<Texture> &texture,
                TexType texture_type, glm::vec2 &screen_size);
  void drawquad(glm::vec2 &&cp, float w, float h, float rotation,
                glm::vec4 &color, std::shared_ptr<Texture> &texture,
                TexType texture_type, glm::vec2 &screen_size);
  void drawquad(glm::vec2 &cp, float w, float h, float rotation,
                glm::vec4 &&color, std::shared_ptr<Texture> &texture,
                TexType texture_type, glm::vec2 &screen_size);
  void drawquad(glm::vec2 &&cp, float w, float h, float rotation,
                glm::vec4 &&color, std::shared_ptr<Texture> &texture,
                TexType texture_type, glm::vec2 &screen_size);
  // 绘制矩形(仅填充颜色)
  void drawquad(glm::vec2 &cp, float w, float h, float rotation,
                glm::vec4 &color, glm::vec2 &screen_size);
  void drawquad(glm::vec2 &&cp, float w, float h, float rotation,
                glm::vec4 &color, glm::vec2 &screen_size);
  void drawquad(glm::vec2 &cp, float w, float h, float rotation,
                glm::vec4 &&color, glm::vec2 &screen_size);
  void drawquad(glm::vec2 &&cp, float w, float h, float rotation,
                glm::vec4 &&color, glm::vec2 &screen_size);
  // 绘制矩形(材质默认填充到矩形)
  void drawquad(glm::vec2 &cp, float w, float h, float rotation,
                std::shared_ptr<Texture> &texture, TexType texture_type,
                glm::vec2 &screen_size);
  void drawquad(glm::vec2 &&cp, float w, float h, float rotation,
                std::shared_ptr<Texture> &texture, TexType texture_type,
                glm::vec2 &screen_size);
  // 更新连续区
  void update_consecutive(
      std::vector<std::vector<std::shared_ptr<Quad>>> &update_consequent_list,
      std::vector<std::shared_ptr<Quad>> *current_consequent_list,
      std::vector<std::vector<float>> &update_consequent_data_list,
      std::vector<float> *current_consequent_data, DataType dataType,
      std::shared_ptr<Quad> &handle_quad) const;
  void drawlinestrip();
  void drawoval();
  void drawcircle();

  void finish();
};
#endif /* MESHBASE */

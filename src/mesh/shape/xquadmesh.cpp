#include "xquadmesh.h"
#include <memory>
#include <string>
#include <vector>

Quad::Quad(glm::vec2 &cp, float width, float height)
    : width(width), height(height) {
  bottom_left = std::make_shared<Vertex>();
  bottom_left->position = {-width / 2, -height / 2, 0.0f};
  bottom_right = std::make_shared<Vertex>();
  bottom_right->position = {width / 2, -height / 2, 0.0f};
  top_right = std::make_shared<Vertex>();
  top_right->position = {width / 2, height / 2, 0.0f};
  top_left = std::make_shared<Vertex>();
  top_left->position = {-width / 2, height / 2, 0.0f};
  translate({cp.x, cp.y, 0.0f});
}
Quad::Quad(glm::vec2 &&cp, float width, float height)
    : Quad(cp, width, height){};
Quad::Quad(glm::vec2 &cp, float width, float height, glm::vec4 &color)
    : Quad(cp, width, height) {
  bottom_left->color = color;
  bottom_right->color = color;
  top_right->color = color;
  top_left->color = color;
};
Quad::Quad(glm::vec2 &cp, float width, float height,
           std::shared_ptr<Texture> texture, TexType texture_type)
    : Quad(cp, width, height) {
  quadTex = texture;
  switch (texture_type) {
  case TexType::FILL_TO_QUAD: {
    // 直接填充整个矩形(尺寸不一则变形)
    bottom_left->uv = {0.0f, 0.0f};
    bottom_right->uv = {1.0f, 0.0f};
    top_right->uv = {1.0f, 1.0f};
    top_left->uv = {0.0f, 1.0f};
    break;
  }
  case TexType::REAPEAT_TO_QUAD: {
    // 重复绘制材质到矩形(根据quad尺寸自动重复绘制或直接裁剪(左下角为起点))
    bottom_left->uv = {0.0f, 0.0f};
    bottom_right->uv = {width / texture->width, 0.0f};
    top_right->uv = {width / texture->width, height / texture->height};
    top_left->uv = {0.0f, height / texture->height};
  }
  }
  bottom_left->texid = texture->texid;
  bottom_right->texid = texture->texid;
  top_right->texid = texture->texid;
  top_left->texid = texture->texid;
};

Quad::Quad(glm::vec2 &cp, float width, float height, glm::vec4 &color,
           std::shared_ptr<Texture> texture, TexType texture_type)
    : Quad(cp, width, height, texture, texture_type) {
  bottom_left->color = color;
  bottom_right->color = color;
  top_right->color = color;
  top_left->color = color;
};
Quad::~Quad() {}
bool Quad::isequaln(const Quad &quad) {
  return width / height == quad.width / quad.height;
};
bool Quad::operator==(const Quad &quad) {
  return *bottom_left == *quad.bottom_left &&
         *bottom_right == *quad.bottom_right && *top_right == *quad.top_right &&
         *top_left == *quad.top_left;
};
std::vector<float> Quad::dump(bool src) {
  std::vector<float> data;
  std::vector<float> bldata;
  std::vector<float> brdata;
  std::vector<float> trdata;
  std::vector<float> tldata;
  if (src) {
    // dump源顶点数据
    bldata = bottom_left->dump();
    brdata = bottom_right->dump();
    trdata = top_right->dump();
    tldata = top_left->dump();
  } else {
    // dump应用变换的顶点数据
    bldata = bottom_left->dump(_translation);
    brdata = bottom_right->dump(_translation);
    trdata = top_right->dump(_translation);
    tldata = top_left->dump(_translation);
  }
  data.insert(data.end(), bldata.begin(), bldata.end());
  data.insert(data.end(), brdata.begin(), brdata.end());
  data.insert(data.end(), trdata.begin(), trdata.end());
  data.insert(data.end(), tldata.begin(), tldata.end());
  return data;
};
void Quad::setmatindex(int index) {
  bottom_left->matindex = index;
  bottom_right->matindex = index;
  top_right->matindex = index;
  top_left->matindex = index;
};
// 设置颜色
void Quad::setfill(glm::vec4 &color) {
  bottom_left->color = color;
  bottom_right->color = color;
  top_right->color = color;
  top_left->color = color;
};
// 设置矩形材质(默认拉伸)
void Quad::settexture(std::shared_ptr<Texture> texture) {
  quadTex = texture;
  bottom_left->texid = texture->texid;
  bottom_right->texid = texture->texid;
  top_right->texid = texture->texid;
  top_left->texid = texture->texid;
};
// 重置材质uv(自定义贴图坐标)
void Quad::setquaduv(glm::vec2 &bottom_leftuv, glm::vec2 &bottom_rightuv,
                     glm::vec2 &top_rightuv, glm::vec2 &top_leftuv) {
  bottom_left->uv = bottom_leftuv;
  bottom_right->uv = bottom_rightuv;
  top_right->uv = top_rightuv;
  top_left->uv = top_leftuv;
};
// 应用变换
// 放缩
void Quad::scale(float scalerate) { _translation = _translation * scalerate; };
// 平移
void Quad::translate(glm::vec3 &trans) {
  _translation = _translation * glm::translate(glm::mat4(1.0f), trans);
};
void Quad::translate(glm::vec3 &&trans) { translate(trans); }
// 旋转
void Quad::rotate(float degrees, glm::vec3 &axis) {
  // 角度转换为弧度
  float angle = glm::radians(degrees);
  // 创建旋转矩阵
  glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, axis);
  _translation = _translation * rotationMatrix;
};
void Quad::rotate(float degrees, glm::vec3 &&axis) { rotate(degrees, axis); }

// 线段构造
Linestrip::Linestrip(glm::vec2 &p1, glm::vec2 &p2, float width)
    : linelength(sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2))),
      linewidth(width),
      Quad({(p2.x + p1.x) / 2.0f, (p2.y - p1.y) / 2.0f},
           sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2)), width) {
  // 旋转线段
  rotate(atan((p2.y - p1.y) / (p2.x - p1.x)) * 180.0f / M_PI,
         {0.0f, 0.0f, 1.0f});
}

Linestrip::Linestrip(glm::vec2 &p1, glm::vec2 &p2, float width,
                     glm::vec4 &color)
    : Linestrip(p1, p2, width) {
  setfill(color);
};

Linestrip::Linestrip(glm::vec2 &sp, float length, float degrees, float width)
    : Quad({sp.x + length * cos(degrees / 180.0f * M_PI),
            sp.y + length * sin(degrees / 180.0f * M_PI)},
           length, width) {}

Linestrip::Linestrip(glm::vec2 &sp, float length, float degrees, float width,
                     glm::vec4 &color)
    : Linestrip(sp, length, degrees, width) {
  setfill(color);
};

Linestrip::~Linestrip() = default;

XquadMesh::XquadMesh(Shader *shader, int max_texture_unit, uint32_t qcount)
    : Xmesh(shader, qcount * 4), _qcount_size(qcount) {}

XquadMesh::~XquadMesh() { Xmesh::~Xmesh(); }

bool XquadMesh::screencontainquad(float x, float y, float width, float height,
                                  glm::vec2 &screensize) {
  // 计算屏幕的半宽和半高，因为是以窗口中心为原点的坐标系，所以要考虑到这一点
  float screenHalfWidth = screensize.x / 2;
  float screenHalfHeight = screensize.y / 2;
  // 矩形的四个顶点坐标
  float topLeftX = x - width / 2;
  float topLeftY = y + height / 2;
  float topRightX = x + width / 2;
  float topRightY = y + height / 2;
  float bottomLeftX = x - width / 2;
  float bottomLeftY = y - height / 2;
  float bottomRightX = x + width / 2;
  float bottomRightY = y - height / 2;

  // 检查是否有顶点在屏幕内
  if ((topLeftX >= -screenHalfWidth && topLeftX <= screenHalfWidth &&
       topLeftY >= -screenHalfHeight && topLeftY <= screenHalfHeight) ||
      (topRightX >= -screenHalfWidth && topRightX <= screenHalfWidth &&
       topRightY >= -screenHalfHeight && topRightY <= screenHalfHeight) ||
      (bottomLeftX >= -screenHalfWidth && bottomLeftX <= screenHalfWidth &&
       bottomLeftY >= -screenHalfHeight && bottomLeftY <= screenHalfHeight) ||
      (bottomRightX >= -screenHalfWidth && bottomRightX <= screenHalfWidth &&
       bottomRightY >= -screenHalfHeight && bottomRightY <= screenHalfHeight)) {
    return true;
  }

  return false;
};
// 使用前绑定本mesh
void XquadMesh::drawquad(glm::vec2 &cp, float width, float height,
                         glm::vec4 &color, glm::vec2 &screensize) {
  drawquad(cp, width, height, color, deftexture, TexType::REAPEAT_TO_QUAD,
           screensize);
};
void XquadMesh::drawquad(glm::vec2 &cp, float width, float height,
                         glm::vec4 &&color, glm::vec2 &screensize) {
  drawquad(cp, width, height, color, screensize);
};
void XquadMesh::drawquad(glm::vec2 &&cp, float width, float height,
                         glm::vec4 &color, glm::vec2 &screensize) {
  drawquad(cp, width, height, color, screensize);
};
void XquadMesh::drawquad(glm::vec2 &&cp, float width, float height,
                         glm::vec4 &&color, glm::vec2 &screensize) {
  drawquad(cp, width, height, color, screensize);
};
void XquadMesh::drawquad(glm::vec2 &cp, float width, float height,
                         std::shared_ptr<Texture> texture, TexType texture_type,
                         glm::vec2 &screensize) {
  drawquad(cp, width, height, {1.0f, 1.0f, 1.0f, 1.0f}, texture, texture_type,
           screensize);
};
void XquadMesh::drawquad(glm::vec2 &&cp, float width, float height,
                         std::shared_ptr<Texture> texture, TexType texture_type,
                         glm::vec2 &screensize) {
  drawquad(cp, width, height, texture, texture_type, screensize);
}
void XquadMesh::drawquad(glm::vec2 &cp, float width, float height,
                         glm::vec4 &color, std::shared_ptr<Texture> texture,
                         TexType texture_type, glm::vec2 &screensize) {
  bool should_draw = screencontainquad(cp.x, cp.y, width, height, screensize);
  if (!should_draw)
    return;
  //  std::cout << "add quad" << std::endl;
  auto quad =
      std::make_shared<Quad>(cp, width, height, color, texture, texture_type);
  if (texture->texid > _max_texid) {
    _max_texid = texture->texid;
  }
  quad->draworder = _should_draw_quads.size();
  _should_draw_quads.push_back(quad);
  // todo
  // 临时矩形列表
  std::shared_ptr<QuadBatch> tempbatch;
  if (_all_batchs.size() == 0) {
    // 所有批数为0(直接新建批次)
    tempbatch = std::make_shared<QuadBatch>();
    tempbatch->batch.push_back(quad);
    // 将新批次加入全部批次
    _all_batchs.push_back(tempbatch);
  } else {
    // 已有批次

    // 计算此quad绑定的texture应处的纹理批次
  }
};

void XquadMesh::drawquad(glm::vec2 &cp, float width, float height,
                         glm::vec4 &&color, std::shared_ptr<Texture> texture,
                         TexType texture_type, glm::vec2 &screensize) {
  drawquad(cp, width, height, color, texture, texture_type, screensize);
}
void XquadMesh::drawquad(glm::vec2 &&cp, float width, float height,
                         glm::vec4 &color, std::shared_ptr<Texture> texture,
                         TexType texture_type, glm::vec2 &screensize) {
  drawquad(cp, width, height, color, texture, texture_type, screensize);
}
void XquadMesh::drawquad(glm::vec2 &&cp, float width, float height,
                         glm::vec4 &&color, std::shared_ptr<Texture> texture,
                         TexType texture_type, glm::vec2 &screensize) {
  drawquad(cp, width, height, color, texture, texture_type, screensize);
};

void XquadMesh::drawlinestrip(glm::vec2 &p1, glm::vec2 &p2, float width,
                              glm::vec4 &color, glm::vec2 &screensize) {
  bool should_draw = screencontainquad(
      (p2.x + p1.x) / 2.0f, (p2.y + p1.y) / 2.0f,
      sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2)), width, screensize);
  //  std::cout << "add quad" << std::endl;
  auto line = std::make_shared<Linestrip>(p1, p2, width, color);
  line->settexture(deftexture);
  line->draworder = _should_draw_quads.size();
  _should_draw_quads.push_back(line);
}

// 绘制线段
void XquadMesh::drawlinestrip(glm::vec2 &p1, glm::vec2 &&p2, float width,
                              glm::vec4 &&color, glm::vec2 &screensize) {
  drawlinestrip(p1, p2, width, color, screensize);
};
void XquadMesh::drawlinestrip(glm::vec2 &&p1, glm::vec2 &p2, float width,
                              glm::vec4 &&color, glm::vec2 &screensize) {
  drawlinestrip(p1, p2, width, color, screensize);
};
void XquadMesh::drawlinestrip(glm::vec2 &&p1, glm::vec2 &&p2, float width,
                              glm::vec4 &color, glm::vec2 &screensize) {
  drawlinestrip(p1, p2, width, color, screensize);
};

void XquadMesh::drawlinestrip(glm::vec2 &p1, glm::vec2 &p2, float width,
                              glm::vec4 &&color, glm::vec2 &screensize) {
  drawlinestrip(p1, p2, width, color, screensize);
};
void XquadMesh::drawlinestrip(glm::vec2 &p1, glm::vec2 &&p2, float width,
                              glm::vec4 &color, glm::vec2 &screensize) {
  drawlinestrip(p1, p2, width, color, screensize);
};
void XquadMesh::drawlinestrip(glm::vec2 &&p1, glm::vec2 &p2, float width,
                              glm::vec4 &color, glm::vec2 &screensize) {
  drawlinestrip(p1, p2, width, color, screensize);
};

void XquadMesh::drawlinestrip(glm::vec2 &&p1, glm::vec2 &&p2, float width,
                              glm::vec4 &&color, glm::vec2 &screensize) {
  drawlinestrip(p1, p2, width, color, screensize);
};

// 绘制射线
void XquadMesh::drawlinestrip(glm::vec2 &sp, float length, float degrees,
                              float width, glm::vec4 &color,
                              glm::vec2 &screensize) {
  drawlinestrip(sp,
                {sp.x + length * cos(degrees / 180.0f * M_PI),
                 sp.y + length * sin(degrees / 180.f * M_PI)},
                width, color, screensize);
};

void XquadMesh::drawlinestrip(glm::vec2 &sp, float length, float degrees,
                              float width, glm::vec4 &&color,
                              glm::vec2 &screensize) {
  drawlinestrip(sp, length, degrees, width, color, screensize);
};
void XquadMesh::drawlinestrip(glm::vec2 &&sp, float length, float degrees,
                              float width, glm::vec4 &color,
                              glm::vec2 &screensize) {
  drawlinestrip(sp, length, degrees, width, color, screensize);
};
void XquadMesh::drawlinestrip(glm::vec2 &&sp, float length, float degrees,
                              float width, glm::vec4 &&color,
                              glm::vec2 &screensize) {
  drawlinestrip(sp, length, degrees, width, color, screensize);
};

// 构建矩形数据到gpu
void XquadMesh::newquad(std::shared_ptr<Quad> quad) {
  uint32_t quadcount = size();
  // std::cout << "绘制数量:" << std::to_string(quadcount) << std::endl;
  //  设置模型矩阵索引
  quad->setmatindex(quadcount);
  // 同步显存
  // 顶点数据
  glBufferSubData(GL_ARRAY_BUFFER, quadcount * 4 * sizeof(Vertex),
                  4 * sizeof(Vertex), quad->dump(false).data());
  // 更新顶点索引数据
  std::vector<unsigned int> indicies_data = {
      4 * quadcount,     4 * quadcount + 1, 4 * quadcount + 2,
      4 * quadcount + 2, 4 * quadcount + 3, 4 * quadcount};
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, quadcount * 6 * sizeof(uint32_t),
                  6 * sizeof(uint32_t), indicies_data.data());
  _quads.push_back(quad);
};

void XquadMesh::finish() {
  // 根据纹理划分渲染批次(至少1批)
  int batch_count = _max_texid / 15 + 1;
  auto batchs = std::vector<std::vector<std::shared_ptr<Quad>>>(batch_count);
  for (auto &quad : _should_draw_quads) {
    // 将矩形放入对应批次
    batchs[quad->quadTex->texid / 15].push_back(quad);
  }
  // 逐批次绘制
  for (int i = 0; i < batchs.size(); i++) {
    for (int j = 0; j < batchs[i].size(); j++) {
      auto quad = batchs[i][j];
      // 上传矩形数据
      newquad(quad);
    }
    // 0号纹理槽绑定
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, deftexture->texture);

    //  此批次纹理数量
    auto size = Texture::textures.size();
    int currentbatchtexturecount = size < 15             ? size
                                   : size < (i + 1) * 15 ? size - i * 15
                                                         : 15;
    // std::cout << "此批纹理数:[" + std::to_string(currentbatchtexturecount) +
    // "]"
    //           << std::endl;
    //  循环绑定此批次矩形所需的材质
    for (int j = 0; j < currentbatchtexturecount; ++j) {
      glActiveTexture(GL_TEXTURE1 + j);
      glBindTexture(GL_TEXTURE_2D, Texture::textures[j + i * 15]->texture);
      _shader->set_sampler(("samplers[" + std::to_string(j + 1) + "]").c_str(),
                           j + 1);
    }
    glDrawElements(GL_TRIANGLES, batchs[i].size() * 6, GL_UNSIGNED_INT,
                   (void *)0);
    _quads.clear();
    // std::cout << "绘制第[" + std::to_string(i + 1) + "]批完成" << std::endl;
  }
  // std::cout << "QuadMesh绘制完成" << std::endl;
  // 清除全部的绘制批
  _should_draw_quads.clear();
}

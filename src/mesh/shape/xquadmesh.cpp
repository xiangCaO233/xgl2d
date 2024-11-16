#include "xquadmesh.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

XquadMesh::XquadMesh(Shader *shader, int max_texture_unit, uint32_t qcount)
    : Xmesh(shader, max_texture_unit, qcount * 4), _qcount_size(qcount) {}

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
  drawquad(cp, width, height, color, deftexture, TexType::REAPEAT, screensize);
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
                         const std::shared_ptr<Texture> &texture,
                         TexType texture_type, glm::vec2 &screensize) {
  drawquad(cp, width, height, {1.0f, 1.0f, 1.0f, 1.0f}, texture, texture_type,
           screensize);
};
void XquadMesh::drawquad(glm::vec2 &&cp, float width, float height,
                         const std::shared_ptr<Texture> &texture,
                         TexType texture_type, glm::vec2 &screensize) {
  drawquad(cp, width, height, texture, texture_type, screensize);
}
void XquadMesh::drawquad(glm::vec2 &cp, float width, float height,
                         glm::vec4 &color,
                         const std::shared_ptr<Texture> &texture,
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
  // 临时矩形列表
  std::shared_ptr<QuadBatch> tempbatch;
  if (_all_batchs.empty()) {
    // 所有批数为空(直接新建批次)
    tempbatch = std::make_shared<QuadBatch>();
    tempbatch->batch.push_back(quad);
    tempbatch->texture_batch_index = quad->tex->texid / _max_texture_unit;
    // 将新批次加入全部批次
    _all_batchs.push_back(tempbatch);
  } else {
    // 已有批次
    int last_texture_batch_index =
        _all_batchs.at(_all_batchs.size() - 1)->texture_batch_index;
    int this_quad_texture_batch_index = quad->tex->texid / _max_texture_unit;
    if (this_quad_texture_batch_index == last_texture_batch_index)
      // 和上一批texture_batch_index相同
      _all_batchs.at(_all_batchs.size() - 1)->batch.push_back(quad);
    else {
      tempbatch = std::make_shared<QuadBatch>();
      tempbatch->batch.push_back(quad);
      tempbatch->texture_batch_index = this_quad_texture_batch_index;
      _all_batchs.push_back(tempbatch);
    }
  }
};

void XquadMesh::drawquad(glm::vec2 &cp, float width, float height,
                         glm::vec4 &&color,
                         const std::shared_ptr<Texture> &texture,
                         TexType texture_type, glm::vec2 &screensize) {
  drawquad(cp, width, height, color, texture, texture_type, screensize);
}
void XquadMesh::drawquad(glm::vec2 &&cp, float width, float height,
                         glm::vec4 &color,
                         const std::shared_ptr<Texture> &texture,
                         TexType texture_type, glm::vec2 &screensize) {
  drawquad(cp, width, height, color, texture, texture_type, screensize);
}
void XquadMesh::drawquad(glm::vec2 &&cp, float width, float height,
                         glm::vec4 &&color,
                         const std::shared_ptr<Texture> &texture,
                         TexType texture_type, glm::vec2 &screensize) {
  drawquad(cp, width, height, color, texture, texture_type, screensize);
};

void XquadMesh::drawlinestrip(glm::vec2 &p1, glm::vec2 &p2, float width,
                              glm::vec4 &color, glm::vec2 &screensize) {
  bool should_draw = screencontainquad(
      (p2.x + p1.x) / 2.0f, (p2.y + p1.y) / 2.0f,
      sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2)), width, screensize);
  if (!should_draw)
    return;
  //  std::cout << "add quad" << std::endl;
  auto line = std::make_shared<Linestrip>(p1, p2, width, color);
  line->settexture(deftexture);
  line->draworder = _should_draw_quads.size();
  _should_draw_quads.push_back(line);
  // 临时矩形列表
  std::shared_ptr<QuadBatch> tempbatch;
  if (_all_batchs.empty()) {
    // 所有批数为空(直接新建批次)
    tempbatch = std::make_shared<QuadBatch>();
    tempbatch->batch.push_back(line);
    tempbatch->texture_batch_index = line->tex->texid / _max_texture_unit;
    // 将新批次加入全部批次
    _all_batchs.push_back(tempbatch);
  } else {
    // 已有批次
    int last_texture_batch_index =
        _all_batchs.at(_all_batchs.size() - 1)->texture_batch_index;
    int this_quad_texture_batch_index = line->tex->texid / _max_texture_unit;
    if (this_quad_texture_batch_index == last_texture_batch_index)
      // 和上一批texture_batch_index相同
      _all_batchs.at(_all_batchs.size() - 1)->batch.push_back(line);
    else {
      tempbatch = std::make_shared<QuadBatch>();
      tempbatch->batch.push_back(line);
      tempbatch->texture_batch_index = this_quad_texture_batch_index;
      _all_batchs.push_back(tempbatch);
    }
  }
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

void XquadMesh::finish() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, deftexture->texture);
  for (auto &quadBatch : _all_batchs) {
    int texture_batch_index = quadBatch->texture_batch_index;
    auto size = Texture::textures.size();
    auto currentbatchtexturecount = size < 15 ? size
                                    : size < (texture_batch_index + 1) * 15
                                        ? size - texture_batch_index * 15
                                        : 15;
    // 激活绑定此批次的全部纹理单元
    for (int j = 0; j < currentbatchtexturecount; j++) {
      glActiveTexture(GL_TEXTURE1 + j);
      glBindTexture(GL_TEXTURE_2D,
                    Texture::textures[j + texture_batch_index * 15]->texture);
      _shader->set_sampler(("samplers[" + std::to_string(j + 1) + "]").c_str(),
                           j + 1);
    }
    // 上传矩形数据
    auto batchsize = quadBatch->batch.size();
    auto *data = new float[batchsize * 44];
    auto *indicies_data = new uint32_t[batchsize * 6];
    for (uint32_t i = 0; i < batchsize; i++) {
      // 设置模型矩阵索引
      quadBatch->batch[i]->setmatindex(i);
      float *quaddata = quadBatch->batch[i]->dump(false);
      // 顶点数据
      std::copy(quaddata, quaddata + 44, data + i * 44);
      delete[] quaddata;
      // 顶点索引数据
      uint32_t quad_indicies_data[6] = {4 * i,     4 * i + 1, 4 * i + 2,
                                        4 * i + 2, 4 * i + 3, 4 * i};
      std::copy(quad_indicies_data, quad_indicies_data + 6,
                indicies_data + i * 6);
    }
    // 上传顶点索引数据
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                    batchsize * 6 * sizeof(uint32_t), indicies_data);
    // 上传顶点数据
    glBufferSubData(GL_ARRAY_BUFFER, 0, batchsize * 4 * sizeof(Vertex), data);

    // 批量绘制矩形
    glDrawElements(GL_TRIANGLES, (quadBatch->batch.size()) * 6, GL_UNSIGNED_INT,
                   nullptr);

    // 清理缓存
    delete[] indicies_data;
    delete[] data;
  }
  _should_draw_quads.clear();
  _all_batchs.clear();
}

#include "xovalmesh.h"
#include <iostream>
#include <string>

Xovalmesh::Xovalmesh(Shader *shader, int max_texture_unit, int ovalcount,
                     int defsegment)
    : Xmesh(shader, max_texture_unit, ovalcount * (1 + defsegment)),
      _defsegment(defsegment) {
  // 初始化实例化缓冲区
  glGenBuffers(1, &instanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  float instanceData[ovalcount * (3 + _defsegment)];
  for (int i = 0; i < 3 + _defsegment; i++) {
    instanceData[i] = i;
  }
  glBufferData(GL_ARRAY_BUFFER, sizeof(instanceData), instanceData,
               GL_DYNAMIC_DRAW);
  // 配置实例数据的位置属性
  bind();
  glEnableVertexAttribArray(5);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void *)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // 每个实例使用不同的索引
  glVertexAttribDivisor(5, 1);
  unbind();
}

Xovalmesh::~Xovalmesh() { glDeleteBuffers(1, &instanceVBO); };

// 绘制椭圆
void Xovalmesh::drawoval(glm::vec2 &cp, float rx, float ry, glm::vec4 &color,
                         glm::vec2 &screensize) {
  drawoval(cp, rx, ry, color, deftexture, TexType::FILL, screensize);
};
void Xovalmesh::drawoval(glm::vec2 &cp, float rx, float ry, glm::vec4 &&color,
                         glm::vec2 &screensize) {
  drawoval(cp, rx, ry, color, screensize);
};
void Xovalmesh::drawoval(glm::vec2 &&cp, float rx, float ry, glm::vec4 &color,
                         glm::vec2 &screensize) {
  drawoval(cp, rx, ry, color, screensize);
};
void Xovalmesh::drawoval(glm::vec2 &&cp, float rx, float ry, glm::vec4 &&color,
                         glm::vec2 &screensize) {
  drawoval(cp, rx, ry, color, screensize);
};
void Xovalmesh::drawoval(glm::vec2 &cp, float rx, float ry,
                         std::shared_ptr<Texture> texture, TexType textype,
                         glm::vec2 &screensize) {
  drawoval(cp, rx, ry, {1.0f, 1.0f, 1.0f, 1.0f}, texture, textype, screensize);
};
void Xovalmesh::drawoval(glm::vec2 &&cp, float rx, float ry,
                         std::shared_ptr<Texture> texture, TexType textype,
                         glm::vec2 &screensize) {
  drawoval(cp, rx, ry, texture, textype, screensize);
};
void Xovalmesh::drawoval(glm::vec2 &cp, float rx, float ry, glm::vec4 &color,
                         std::shared_ptr<Texture> texture, TexType textype,
                         glm::vec2 &screensize) {
  // 实现绘制
  // 生成顶点数据
  // bool should_draw = screencontainoval(cp.x, cp.y, rx, ry, screensize);
  // if (!should_draw)
  //  return;
  auto oval =
      std::make_shared<Oval>(cp, rx, ry, _defsegment, color, texture, textype);
  if (texture->texid > _max_texid) {
    _max_texid = texture->texid;
  }
  oval->draworder = _should_draw_ovals.size();

  _should_draw_ovals.push_back(oval);
  // 临时矩形列表
  std::shared_ptr<OvalBatch> tempbatch;
  if (_all_batchs.empty()) {
    // 所有批数为空(直接新建批次)
    tempbatch = std::make_shared<OvalBatch>();
    tempbatch->batch.push_back(oval);
    tempbatch->texture_batch_index = oval->tex->texid / _max_texture_unit;
    // 将新批次加入全部批次
    _all_batchs.push_back(tempbatch);
  } else {
    // 已有批次
    int last_texture_batch_index =
        _all_batchs.at(_all_batchs.size() - 1)->texture_batch_index;
    int this_oval_texture_batch_index = oval->tex->texid / _max_texture_unit;
    if (this_oval_texture_batch_index == last_texture_batch_index)
      // 和上一批texture_batch_index相同
      _all_batchs.at(_all_batchs.size() - 1)->batch.push_back(oval);
    else {
      tempbatch = std::make_shared<OvalBatch>();
      tempbatch->batch.push_back(oval);
      tempbatch->texture_batch_index = this_oval_texture_batch_index;
      _all_batchs.push_back(tempbatch);
    }
  }
};
void Xovalmesh::drawoval(glm::vec2 &cp, float rx, float ry, glm::vec4 &&color,
                         std::shared_ptr<Texture> texture, TexType textype,
                         glm::vec2 &screensize) {
  drawoval(cp, rx, ry, color, texture, textype, screensize);
};
void Xovalmesh::drawoval(glm::vec2 &&cp, float rx, float ry, glm::vec4 &color,
                         std::shared_ptr<Texture> texture, TexType textype,
                         glm::vec2 &screensize) {

  drawoval(cp, rx, ry, color, texture, textype, screensize);
};
void Xovalmesh::drawoval(glm::vec2 &&cp, float rx, float ry, glm::vec4 &&color,
                         std::shared_ptr<Texture> texture, TexType textype,
                         glm::vec2 &screensize) {
  drawoval(cp, rx, ry, color, texture, textype, screensize);
};

// 绘制圆
void Xovalmesh::drawcircle(glm::vec2 &cp, float r, glm::vec4 &color,
                           glm::vec2 &screensize) {
  drawoval(cp, r, r, color, screensize);
};
void Xovalmesh::drawcircle(glm::vec2 &cp, float r, glm::vec4 &&color,
                           glm::vec2 &screensize) {
  drawcircle(cp, r, color, screensize);
};
void Xovalmesh::drawcircle(glm::vec2 &&cp, float r, glm::vec4 &color,
                           glm::vec2 &screensize) {
  drawcircle(cp, r, color, screensize);
};
void Xovalmesh::drawcircle(glm::vec2 &&cp, float r, glm::vec4 &&color,
                           glm::vec2 &screensize) {
  drawcircle(cp, r, color, screensize);
};
void Xovalmesh::drawcircle(glm::vec2 &cp, float r,
                           std::shared_ptr<Texture> texture, TexType textype,
                           glm::vec2 &screensize) {
  drawoval(cp, r, r, texture, textype, screensize);
};
void Xovalmesh::drawcircle(glm::vec2 &&cp, float r,
                           std::shared_ptr<Texture> texture, TexType textype,
                           glm::vec2 &screensize) {
  drawcircle(cp, r, texture, textype, screensize);
};
void Xovalmesh::drawcircle(glm::vec2 &cp, float r, glm::vec4 &color,
                           std::shared_ptr<Texture> texture, TexType textype,
                           glm::vec2 &screensize) {
  drawoval(cp, r, r, color, texture, textype, screensize);
};
void Xovalmesh::drawcircle(glm::vec2 &cp, float r, glm::vec4 &&color,
                           std::shared_ptr<Texture> texture, TexType textype,
                           glm::vec2 &screensize) {
  drawcircle(cp, r, color, texture, textype, screensize);
};
void Xovalmesh::drawcircle(glm::vec2 &&cp, float r, glm::vec4 &color,
                           std::shared_ptr<Texture> texture, TexType textype,
                           glm::vec2 &screensize) {
  drawcircle(cp, r, color, texture, textype, screensize);
};
void Xovalmesh::drawcircle(glm::vec2 &&cp, float r, glm::vec4 &&color,
                           std::shared_ptr<Texture> texture, TexType textype,
                           glm::vec2 &screensize) {
  drawcircle(cp, r, color, texture, textype, screensize);
};
void Xovalmesh::finish() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, deftexture->texture);
  for (auto &ovalBatch : _all_batchs) {
    int texture_batch_index = ovalBatch->texture_batch_index;
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

    // 上传椭圆数据
    auto batchsize = ovalBatch->batch.size();
    int indexcount = 2 + _defsegment;
    auto *data = new float[batchsize * 11 * indexcount];
    auto *indicies_data = new uint32_t[batchsize * indexcount];
    for (uint32_t i = 0; i < batchsize; i++) {
      // 设置模型矩阵索引
      ovalBatch->batch[i]->setmatindex(i);
      // 顶点数据
      float *ovaldata = ovalBatch->batch[i]->dump(false);
      std::copy(ovaldata, ovaldata + 11 * indexcount,
                data + i * (11 * indexcount));
      delete[] ovaldata;
    }
    // 上传顶点数据
    glBufferSubData(GL_ARRAY_BUFFER, 0, batchsize * indexcount * sizeof(Vertex),
                    data);
    // 绘制椭圆(instanced实例化防止粘连)
    glDrawArrays(GL_TRIANGLE_FAN, 0, batchsize * indexcount);
    // glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, indexcount, batchsize);
    // std::cout << "batchsize :" << std::to_string(batchsize) << std::endl;
    // glDrawElementsInstanced(GL_TRIANGLE_FAN, batchsize * indexcount,
    //                         GL_UNSIGNED_INT, nullptr, indexcount);

    // 清理缓存
    delete[] data;
    delete[] indicies_data;
  }
  _should_draw_ovals.clear();
  _all_batchs.clear();
};

#include "mesh.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

Mesh::Mesh(Shader *shader, int max_texture_unit, int initial_quad_count)
    : _shader(shader), _max_texture_unit(max_texture_unit),
      max_quad_count(initial_quad_count) {
  // 初始化缓冲对象
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &instanceVBO);
  // glGenBuffers(1, &FBO);
  // 变换前原始矩形(1x1像素)
  // 初始化默认纹理
  _deftexture = std::make_shared<Texture>();
  float basic_quad_data[20] = {
      -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // v1
      1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // v2
      1.0f,  1.0f,  0.0f, 1.0f, 1.0f, // v3
      -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, // v4
  };

  // 绑定顶点数组缓冲对象
  glBindVertexArray(VAO);
  // 绑定顶点数据缓冲对象
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // 上传原始矩形数据到缓冲区(VBO中)
  glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), basic_quad_data,
               GL_STATIC_DRAW);

  // 描述location0 顶点缓冲0~2float为float类型数据(用vec3接收)
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);

  // 描述location2 顶点缓冲3~4float为float类型数据(用vec2接收为默认uv坐标)
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));

  // 实例数据
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  glBufferData(GL_ARRAY_BUFFER, max_quad_count * 19 * sizeof(float), nullptr,
               GL_DYNAMIC_DRAW);
  // 描述location3 为2*float
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 19 * sizeof(float), nullptr);
  // 每个实例变化一次
  glVertexAttribDivisor(2, 1);

  // 绑定尺寸缓冲(x和y的缩放倍率)
  // 描述location3 为2*float
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 19 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  // 每个实例变化一次
  glVertexAttribDivisor(3, 1);

  // 绑定旋转角缓冲
  // 描述location4 为float
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 19 * sizeof(float),
                        (void *)(4 * sizeof(float)));
  // 每个实例变化一次
  glVertexAttribDivisor(4, 1);

  // 绑定颜色缓冲
  // 描述location5 为4*float
  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 19 * sizeof(float),
                        (void *)(5 * sizeof(float)));
  // 每个实例变化一次
  glVertexAttribDivisor(5, 1);

  // 绑定纹理索引缓冲
  // 描述location6 为float
  glEnableVertexAttribArray(6);
  glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, 19 * sizeof(float),
                        (void *)(9 * sizeof(float)));
  // 每个实例变化一次
  glVertexAttribDivisor(6, 1);

  // 绑定纹理坐标缓冲
  // 绑定纹理坐标缓冲 (拆分 mat3 为 3 个 vec3)
  // 描述 location 7 为 3 * float (第 1 列)
  glEnableVertexAttribArray(7);
  glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 19 * sizeof(float),
                        (void *)(10 * sizeof(float)));
  glVertexAttribDivisor(7, 1);

  // 描述 location 8 为 3 * float (第 2 列)
  glEnableVertexAttribArray(8);
  glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, 19 * sizeof(float),
                        (void *)(13 * sizeof(float)));
  glVertexAttribDivisor(8, 1);

  // 描述 location 9 为 3 * float (第 3 列)
  glEnableVertexAttribArray(9);
  glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, 19 * sizeof(float),
                        (void *)(16 * sizeof(float)));
  glVertexAttribDivisor(9, 1);

  // 绑定默认1*1白色纹理到纹理槽0
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _deftexture->texture);

  // 解绑结束构造
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

Mesh::~Mesh() {
  unbind();
  if (FBO)
    glDeleteBuffers(1, &FBO);
  glDeleteBuffers(1, &instanceVBO);
  glDeleteVertexArrays(1, &VAO);
}
void Mesh::bind() {
  // 绑定VAO，VBO
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
}
void Mesh::unbind() {
  // 绑定VAO，VBO
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Mesh::updateinstanceoffset(int instance_offset) {
  uintptr_t offset = 19 * sizeof(float) * instance_offset;

  // 描述location3 为2*float
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 19 * sizeof(float),
                        (void *)offset);

  // 绑定尺寸缓冲(x和y的缩放倍率)
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 19 * sizeof(float),
                        (void *)(2 * sizeof(float) + offset));

  // 绑定旋转角缓冲
  glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 19 * sizeof(float),
                        (void *)(4 * sizeof(float) + offset));

  // 绑定颜色缓冲
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 19 * sizeof(float),
                        (void *)(5 * sizeof(float) + offset));

  // 绑定纹理索引缓冲
  glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, 19 * sizeof(float),
                        (void *)(9 * sizeof(float) + offset));

  // 绑定纹理缓冲
  glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 19 * sizeof(float),
                        (void *)(10 * sizeof(float) + offset));
  glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, 19 * sizeof(float),
                        (void *)(13 * sizeof(float) + offset));
  glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, 19 * sizeof(float),
                        (void *)(16 * sizeof(float) + offset));
}

void Mesh::updatequadbatch(std::shared_ptr<Quad> &quad) {
  // 批处理部分
  // 临时矩形列表
  std::shared_ptr<QuadBatch> tempbatch;
  if (_all_batchs.empty()) {
    // 所有批数为空(直接新建批次)
    tempbatch = std::make_shared<QuadBatch>();
    tempbatch->batch.push_back(quad);
    tempbatch->texture_batch_index = quad->_tex->texid / _max_texture_unit;
    // 将新批次加入全部批次
    _all_batchs.push_back(tempbatch);
  } else {
    // 已有批次
    int last_texture_batch_index =
        _all_batchs.at(_all_batchs.size() - 1)->texture_batch_index;
    int this_quad_texture_batch_index = quad->_tex->texid / _max_texture_unit;
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
}

// 绘制矩形
void Mesh::drawquad(glm::vec2 &cp, float w, float h, float rotation,
                    glm::vec4 &color, std::shared_ptr<Texture> &texture,
                    TexType texture_type, glm::vec2 &screen_size) {
  // debug
  // std::cout << "---------------------------------------------------"
  //          << std::endl;
  // 记录下每一段需要更新的连续的内存,也可以只有一份数据
  std::vector<std::shared_ptr<Quad>> *consequent_quads = nullptr;
  std::vector<float> *consequent_quads_data = nullptr;
  // 判断当前缓存矩形列表是否到达末尾
  if (_current_handle_index == _quads.size()) {
    // 初始化此矩形
    std::shared_ptr<Quad> quad = std::make_shared<Quad>(
        cp, w, h, rotation, color, texture, texture_type);
    quad->_draw_order = _current_handle_index;
    // 更新最大纹理id
    if (_max_texid < quad->_tex->texid) {
      _max_texid = quad->_tex->texid;
    }
    // 设置模型数据偏移点
    // 内存偏移
    quad->model_data_offset = _current_handle_index * 19 * sizeof(float);
    // 直接加入更新列表和缓存
    _quads.push_back(quad);

    // 更新数据
    update_consecutive(_update_consequent_quads, consequent_quads,
                       _update_consequent_quads_datas, consequent_quads_data,
                       quad);
    updatequadbatch(quad);
  } else {
    // 当前没有处理到结尾处
    // 获取当前位置矩形
    std::shared_ptr<Quad> _handle_quad = _quads[_current_handle_index];
    // 检查当前绘制到的矩形内容是否一致
    if (!_handle_quad->equals(cp, w, h, rotation, color, texture,
                              texture_type)) {
      // 不一致,更新矩形数据
      _handle_quad->_cp = cp;
      _handle_quad->_tex = texture;
      _handle_quad->_rotation = rotation;
      _handle_quad->_color = color;
      _handle_quad->settextyppe(texture_type);
      update_consecutive(_update_consequent_quads, consequent_quads,
                         _update_consequent_quads_datas, consequent_quads_data,
                         _handle_quad);
    };
    updatequadbatch(_handle_quad);
  }
  _current_handle_index++;
};
void Mesh::drawquad(glm::vec2 &&cp, float w, float h, float rotation,
                    glm::vec4 &color, std::shared_ptr<Texture> &texture,
                    TexType texture_type, glm::vec2 &screen_size) {
  drawquad(cp, w, h, rotation, color, texture, texture_type, screen_size);
};
void Mesh::drawquad(glm::vec2 &cp, float w, float h, float rotation,
                    glm::vec4 &&color, std::shared_ptr<Texture> &texture,
                    TexType texture_type, glm::vec2 &screen_size) {
  drawquad(cp, w, h, rotation, color, texture, texture_type, screen_size);
};
void Mesh::drawquad(glm::vec2 &&cp, float w, float h, float rotation,
                    glm::vec4 &&color, std::shared_ptr<Texture> &texture,
                    TexType texture_type, glm::vec2 &screen_size) {
  drawquad(cp, w, h, rotation, color, texture, texture_type, screen_size);
};
void Mesh::drawquad(glm::vec2 &cp, float w, float h, float rotation,
                    glm::vec4 &color, glm::vec2 &screen_size) {
  drawquad(cp, w, h, rotation, color, _deftexture, FILL, screen_size);
};
void Mesh::drawquad(glm::vec2 &&cp, float w, float h, float rotation,
                    glm::vec4 &color, glm::vec2 &screen_size) {
  drawquad(cp, w, h, rotation, color, screen_size);
};
void Mesh::drawquad(glm::vec2 &cp, float w, float h, float rotation,
                    glm::vec4 &&color, glm::vec2 &screen_size) {
  drawquad(cp, w, h, rotation, color, screen_size);
};
void Mesh::drawquad(glm::vec2 &&cp, float w, float h, float rotation,
                    glm::vec4 &&color, glm::vec2 &screen_size) {
  drawquad(cp, w, h, rotation, color, screen_size);
};
void Mesh::drawquad(glm::vec2 &cp, float w, float h, float rotation,
                    std::shared_ptr<Texture> &texture, TexType texture_type,
                    glm::vec2 &screen_size) {
  drawquad(cp, w, h, rotation, {1.0f, 1.0f, 1.0f, 1.0f}, texture, texture_type,
           screen_size);
};
void Mesh::drawquad(glm::vec2 &&cp, float w, float h, float rotation,
                    std::shared_ptr<Texture> &texture, TexType texture_type,
                    glm::vec2 &screen_size) {
  drawquad(cp, w, h, rotation, texture, texture_type, screen_size);
};

// 通用的连续更新处理函数
void Mesh::update_consecutive(
    std::vector<std::vector<std::shared_ptr<Quad>>> &update_consequent_list,
    std::vector<std::shared_ptr<Quad>> *current_consequent_list,
    std::vector<std::vector<float>> &update_consequent_data_list,
    std::vector<float> *current_consequent_data,
    std::shared_ptr<Quad> &handle_quad) const {
  // 检查并更新此矩形到正确位置
  if (!update_consequent_list.empty()) {
    // 获取最后一个连续更新段
    auto &last_consequent = update_consequent_list.back();
    auto &last_consequent_data = update_consequent_data_list.back();
    // std::cout << "last_consequent:" << &last_consequent << std::endl;
    if (last_consequent.back()->_draw_order == _current_handle_index - 1) {
      // 连续
      current_consequent_list = &last_consequent;
      current_consequent_data = &last_consequent_data;
    } else {
      // 不连续,创建新区段
      update_consequent_list.emplace_back();
      update_consequent_data_list.emplace_back();

      // 引用新创建的区段
      current_consequent_list = &update_consequent_list.back();
      current_consequent_data = &update_consequent_data_list.back();
    }
  } else {
    // 如果列表为空，创建新的区段
    update_consequent_list.emplace_back();
    update_consequent_data_list.emplace_back();

    // 引用新创建的区段
    current_consequent_list = &update_consequent_list.back();
    current_consequent_data = &update_consequent_data_list.back();
  }
  // 加入此连续区
  current_consequent_list->push_back(handle_quad);
  // std::cout << "current_consequent_list:" << current_consequent_list
  //           << std::endl;
  //  更新数据(按照VAO布局更新数据)
  current_consequent_data->push_back(handle_quad->_cp.x);
  current_consequent_data->push_back(handle_quad->_cp.y);
  current_consequent_data->push_back(handle_quad->_w);
  current_consequent_data->push_back(handle_quad->_h);
  current_consequent_data->push_back(handle_quad->_rotation);
  current_consequent_data->push_back(handle_quad->_color.r);
  current_consequent_data->push_back(handle_quad->_color.g);
  current_consequent_data->push_back(handle_quad->_color.b);
  current_consequent_data->push_back(handle_quad->_color.a);
  current_consequent_data->push_back(handle_quad->_tex->texid);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      current_consequent_data->push_back(handle_quad->_uvtransform[i][j]);
    }
  }
}

void Mesh::finish() {
  //  处理显存
  if (_current_handle_index < _quads.size() - 1) {
    // 上一帧还有残余数据
    // 擦除后面部分的矩形缓存
    _quads.erase(_quads.begin() + _current_handle_index, _quads.end());
  }
  // std::cout << "debug:" << std::endl;
  // std::cout << "gpu data:" << std::endl;
  // auto data = std::vector<float>(_quads.size() * 19);
  // glGetBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * 4, data.data());
  // for (auto d : data) {
  //   std::cout << std::to_string(d) << ",";
  // }
  //   更新连续的矩形数据
  if (!_update_consequent_quads.empty()) {
    // 需要更新矩形数据
    for (int i = 0; i < _update_consequent_quads.size(); i++) {
      auto quads = _update_consequent_quads[i];
      // 计算连续显存的起始偏移
      uint32_t consequent_data_start_offset = quads.front()->model_data_offset;
      // 计算连续的显存大小
      uint32_t consequent_data_size = quads.back()->model_data_offset -
                                      consequent_data_start_offset +
                                      19 * sizeof(float);
      // 更新那一段显存
      glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
      glBufferSubData(GL_ARRAY_BUFFER, consequent_data_start_offset,
                      consequent_data_size,
                      _update_consequent_quads_datas[i].data());
    }
  }
  if (_max_texid == 0) {
    // 都是默认纹理
    // 直接全部绘制
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, (int)(_quads.size()));
  } else {
    // 有非默认纹理绑定,批处理多纹理
    // std::cout << "批数:" << std::to_string(_all_batchs.size()) << std::endl;
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
        _shader->set_sampler(
            ("samplers[" + std::to_string(j + 1) + "]").c_str(), j + 1);
      }

      updateinstanceoffset(quadBatch->batch.front()->_draw_order);
      // 批绘制矩形
      glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4,
                            (int)(quadBatch->batch.size()));
    }
    updateinstanceoffset();
  }
  //  恢复处理索引
  _current_handle_index = 0;
  // 清空批次
  _all_batchs.clear();
  // 清除需要更新的内容
  _update_consequent_quads.clear();
  _update_consequent_quads_datas.clear();
}

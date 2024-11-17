#include "mesh.h"
#include <memory>
#include <vector>

Mesh::Mesh() {
  // 初始化缓冲对象
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(4, instanceVBOs);
  // glGenBuffers(1, &FBO);
  // glGenBuffers(1, &TBO);
  // 变换前原始矩形(1x1像素)
  float basic_quad_data[60] = {
      -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, // v1
      1.0f,  -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, // v2
      1.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  1.0f,  0.0f, // v3
      1.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  1.0f,  0.0f, // v3
      -1.0f, 1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f,  0.0f, // v4
      -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, // v1
  };

  // 绑定顶点数组缓冲对象
  glBindVertexArray(VAO);
  // 绑定顶点数据缓冲对象
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // 上传原始矩形数据到缓冲区(VBO中)
  glBufferData(GL_ARRAY_BUFFER, 40 * sizeof(float), basic_quad_data,
               GL_STATIC_DRAW);

  // 描述location0 顶点缓冲0~2float为float类型数据(用vec3接收)
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float),
                        (void *)0);

  // 描述location1 顶点缓冲3~6float为float类型数据(用vec4接收)
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float),
                        (void *)(3 * sizeof(float)));

  // 描述location2 顶点缓冲7~9float为float类型数据(用vec2接收为uv坐标)
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float),
                        (void *)(7 * sizeof(float)));

  // 描述location3 顶点缓冲第10个float为float类型数据(用float接收为texid)
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float),
                        (void *)(9 * sizeof(float)));

  // 绑定坐标缓冲(矩形中心)
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBOs[0]);
  // 描述location4 为2*float
  glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(4);
  // 每个实例变化一次
  glVertexAttribDivisor(4, 1);

  // 绑定颜色缓冲
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBOs[1]);
  // 描述location5 为4*float
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(5);
  // 每个实例变化一次
  glVertexAttribDivisor(5, 1);

  // 绑定纹理坐标缓冲
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBOs[2]);
  // 描述location6 为2*float
  glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(6);
  // 每个实例变化一次
  glVertexAttribDivisor(6, 1);

  // 绑定纹理索引缓冲
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBOs[3]);
  // 描述location7 为float
  glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(7);
  // 每个实例变化一次
  glVertexAttribDivisor(7, 1);

  // 解绑结束构造
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

Mesh::~Mesh() {
  if (FBO)
    glDeleteBuffers(1, &FBO);
  if (TBO)
    glDeleteBuffers(1, &TBO);
  glDeleteBuffers(4, instanceVBOs);
  glDeleteVertexArrays(1, &VAO);
}

// 绘制矩形
void Mesh::drawquad(glm::vec2 &cp, float w, float h, glm::vec4 &color,
                    std::shared_ptr<Texture> &texture, TexType texture_type,
                    glm::vec2 &screen_size) {
  // 记录下每一段需要更新的连续的内存,也可以只有一份数据
  std::vector<std::shared_ptr<Quad>> consequent_quads_pos;
  std::vector<float> consequent_quads_pos_data;
  std::vector<std::shared_ptr<Quad>> consequent_quads_color;
  std::vector<float> consequent_quads_color_data;
  std::vector<std::shared_ptr<Quad>> consequent_quads_uv;
  std::vector<float> consequent_quads_uv_data;
  std::vector<std::shared_ptr<Quad>> consequent_quads_texid;
  std::vector<float> consequent_quads_texid_data;
  // 判断当前缓存矩形列表是否到达末尾
  if (_current_handle_index == _quads.size()) {
    // 初始化此矩形
    std::shared_ptr<Quad> quad =
        std::make_shared<Quad>(cp, w, h, color, texture, texture);
    quad->_draw_order = _current_handle_index;
    // 设置模型数据偏移点
    // 2d坐标
    quad->model_data_offset[0] = _current_handle_index * sizeof(glm::vec2);
    // 颜色
    quad->model_data_offset[1] = _current_handle_index * sizeof(glm::vec4);
    // uv
    quad->model_data_offset[2] = _current_handle_index * sizeof(glm::vec2);
    // texid
    quad->model_data_offset[3] = _current_handle_index * sizeof(float);
    // 直接加入更新列表和缓存
    _quads.push_back(quad);

    // 各自更新
    // 更新相关连续区段
    update_consecutive(_update_consequent_quads_poss, consequent_quads_pos,
                       _update_consequent_quads_pos_datas,
                       consequent_quads_pos_data, POSITION, quad);
    update_consecutive(_update_consequent_quads_colors, consequent_quads_color,
                       _update_consequent_quads_color_datas,
                       consequent_quads_color_data, COLOR, quad);
    update_consecutive(_update_consequent_quads_uvs, consequent_quads_uv,
                       _update_consequent_quads_uv_datas,
                       consequent_quads_uv_data, UV, quad);
    update_consecutive(_update_consequent_quads_texids, consequent_quads_texid,
                       _update_consequent_quads_texid_datas,
                       consequent_quads_texid_data, TEXID, quad);
  } else {
    // 当前没有处理到结尾处
    // 获取当前位置矩形
    std::shared_ptr<Quad> _handle_quad = _quads[_current_handle_index];
    // 检查当前绘制到的矩形内容是否一致
    bool is_update{false};
    if (!_handle_quad->iscpequal(cp)) {
      // 坐标不同，需要更新
      _handle_quad->_cp = cp;
      // 检查当前绘制到的矩形内容是否一致
      if (!_handle_quad->_is_update) {
        if (!_handle_quad->iscpequal(cp)) {
          _handle_quad->_cp = cp;
          update_consecutive(_update_consequent_quads_poss,
                             consequent_quads_pos,
                             _update_consequent_quads_pos_datas,
                             consequent_quads_pos_data, POSITION, _handle_quad);
        }
        if (!_handle_quad->iscolorequal(color)) {
          _handle_quad->_color = color;
          update_consecutive(_update_consequent_quads_colors,
                             consequent_quads_color,
                             _update_consequent_quads_color_datas,
                             consequent_quads_color_data, COLOR, _handle_quad);
        }
        if (!_handle_quad->isuvequal(texture_type)) {
          _handle_quad->_textype = texture_type;
          update_consecutive(_update_consequent_quads_uvs, consequent_quads_uv,
                             _update_consequent_quads_uv_datas,
                             consequent_quads_uv_data, UV, _handle_quad);
        }
        if (!_handle_quad->istexequal(texture)) {
          _handle_quad->_tex = texture;
          update_consecutive(_update_consequent_quads_texids,
                             consequent_quads_texid,
                             _update_consequent_quads_texid_datas,
                             consequent_quads_texid_data, TEXID, _handle_quad);
        }
      }
    }
    _current_handle_index++;
  }
};
void Mesh::drawquad(glm::vec2 &&cp, float w, float h, glm::vec4 &color,
                    std::shared_ptr<Texture> &texture, TexType texture_type,
                    glm::vec2 &screen_size) {
  drawquad(cp, w, h, color, texture, texture_type, screen_size);
};
void Mesh::drawquad(glm::vec2 &cp, float w, float h, glm::vec4 &&color,
                    std::shared_ptr<Texture> &texture, TexType texture_type,
                    glm::vec2 &screen_size) {
  drawquad(cp, w, h, color, texture, texture_type, screen_size);
};
void Mesh::drawquad(glm::vec2 &&cp, float w, float h, glm::vec4 &&color,
                    std::shared_ptr<Texture> &texture, TexType texture_type,
                    glm::vec2 &screen_size) {
  drawquad(cp, w, h, color, texture, texture_type, screen_size);
};
void Mesh::drawquad(glm::vec2 &cp, float w, float h, glm::vec4 &color,
                    glm::vec2 &screen_size) {
  drawquad(cp, w, h, color, _deftexture, FILL, screen_size);
};
void Mesh::drawquad(glm::vec2 &&cp, float w, float h, glm::vec4 &color,
                    glm::vec2 &screen_size) {
  drawquad(cp, w, h, color, screen_size);
};
void Mesh::drawquad(glm::vec2 &cp, float w, float h, glm::vec4 &&color,
                    glm::vec2 &screen_size) {
  drawquad(cp, w, h, color, screen_size);
};
void Mesh::drawquad(glm::vec2 &&cp, float w, float h, glm::vec4 &&color,
                    glm::vec2 &screen_size) {
  drawquad(cp, w, h, color, screen_size);
};
void Mesh::drawquad(glm::vec2 &cp, float w, float h,
                    std::shared_ptr<Texture> &texture, TexType texture_type,
                    glm::vec2 &screen_size) {
  drawquad(cp, w, h, {1.0f, 1.0f, 1.0f, 1.0f}, texture, texture_type,
           screen_size);
};
void Mesh::drawquad(glm::vec2 &&cp, float w, float h,
                    std::shared_ptr<Texture> &texture, TexType texture_type,
                    glm::vec2 &screen_size) {
  drawquad(cp, w, h, texture, texture_type, screen_size);
};

// 通用的连续更新处理函数
void Mesh::update_consecutive(
    std::vector<std::vector<std::shared_ptr<Quad>>> &update_consequent_list,
    std::vector<std::shared_ptr<Quad>> &current_consequent_list,
    std::vector<std::vector<float>> &update_consequent_data_list,
    std::vector<float> &current_consequent_data, DataType dataType,
    std::shared_ptr<Quad> &handle_quad) {
  // 检查并更新此矩形到正确位置
  if (!update_consequent_list.empty()) {
    // 获取最后一个连续更新段
    auto &last_consequent = update_consequent_list.back();
    auto &last_consequent_data = update_consequent_data_list.back();
    if (last_consequent.back()->_draw_order == _current_handle_index - 1) {
      // 连续
      current_consequent_list = last_consequent;
      current_consequent_data = last_consequent_data;
    }
  }
  // 加入此连续区
  current_consequent_list.push_back(handle_quad);
  // 更新数据
  switch (dataType) {
  case POSITION: {
    current_consequent_data.push_back(handle_quad->_cp.x);
    current_consequent_data.push_back(handle_quad->_cp.y);
    break;
  }
  case COLOR: {
    current_consequent_data.push_back(handle_quad->_color.r);
    current_consequent_data.push_back(handle_quad->_color.g);
    current_consequent_data.push_back(handle_quad->_color.b);
    current_consequent_data.push_back(handle_quad->_color.a);
    break;
  }
  case UV: {
    // 需按textype实现
    current_consequent_data.push_back(0.0f);
    current_consequent_data.push_back(0.0f);
    break;
  }
  case TEXID: {
    current_consequent_data.push_back(handle_quad->_tex->texid);
    break;
  }
  }
  // 添加更新数据
  handle_quad->_is_update = true;
}

void Mesh::finish() {
  // 处理显存
  if (_current_handle_index < _quads.size() - 1) {
    // 上一帧还有残余数据
    // 擦除后面部分的矩形缓存
    _quads.erase(_quads.begin() + _current_handle_index, _quads.end());
  }
  // 更新连续的位置数据
  if (!_update_consequent_quads_poss.empty()) {
    // 需要更新位置数据
    for (int i = 0; i < _update_consequent_quads_poss.size(); i++) {
      auto quads_pos = _update_consequent_quads_poss[i];
      // 计算连续显存的起始偏移
      uint32_t consequent_pos_data_start_offset =
          quads_pos.front()->model_data_offset[0];
      // 计算连续的显存大小
      uint32_t consequent_pos_data_size =
          quads_pos.back()->model_data_offset[0] -
          consequent_pos_data_start_offset + sizeof(glm::vec2);
      // 更新那一段显存
      glBindBuffer(GL_ARRAY_BUFFER, instanceVBOs[0]);
      glBufferSubData(GL_ARRAY_BUFFER, consequent_pos_data_size,
                      consequent_pos_data_size,
                      _update_consequent_quads_pos_datas[i].data());
    }
  }
  // 颜色
  if (!_update_consequent_quads_colors.empty()) {
    for (int i = 0; i < _update_consequent_quads_colors.size(); i++) {
      auto quads_color = _update_consequent_quads_colors[i];
      uint32_t consequent_color_data_start_offset =
          quads_color.front()->model_data_offset[0];
      uint32_t consequent_color_data_size =
          quads_color.back()->model_data_offset[0] -
          consequent_color_data_start_offset + sizeof(glm::vec2);
      glBindBuffer(GL_ARRAY_BUFFER, instanceVBOs[0]);
      glBufferSubData(GL_ARRAY_BUFFER, consequent_color_data_size,
                      consequent_color_data_size,
                      _update_consequent_quads_color_datas[i].data());
    }
  }
  // uv
  if (!_update_consequent_quads_uvs.empty()) {
    for (int i = 0; i < _update_consequent_quads_uvs.size(); i++) {
      auto quads_uv = _update_consequent_quads_uvs[i];
      uint32_t consequent_uv_data_start_offset =
          quads_uv.front()->model_data_offset[0];
      uint32_t consequent_uv_data_size = quads_uv.back()->model_data_offset[0] -
                                         consequent_uv_data_start_offset +
                                         sizeof(glm::vec2);
      glBindBuffer(GL_ARRAY_BUFFER, instanceVBOs[0]);
      glBufferSubData(GL_ARRAY_BUFFER, consequent_uv_data_size,
                      consequent_uv_data_size,
                      _update_consequent_quads_uv_datas[i].data());
    }
  }
  // texid
  if (!_update_consequent_quads_texids.empty()) {
    for (int i = 0; i < _update_consequent_quads_texids.size(); i++) {
      auto quads_texid = _update_consequent_quads_texids[i];
      uint32_t consequent_texid_data_start_offset =
          quads_texid.front()->model_data_offset[0];
      uint32_t consequent_texid_data_size =
          quads_texid.back()->model_data_offset[0] -
          consequent_texid_data_start_offset + sizeof(glm::vec2);
      glBindBuffer(GL_ARRAY_BUFFER, instanceVBOs[0]);
      glBufferSubData(GL_ARRAY_BUFFER, consequent_texid_data_size,
                      consequent_texid_data_size,
                      _update_consequent_quads_texid_datas[i].data());
    }
  }

  glDrawArraysInstanced(GL_TRIANGLES, 0, 6, _quads.size());
  // 恢复处理索引
  _current_handle_index = 0;
}

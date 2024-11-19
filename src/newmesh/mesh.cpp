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
  glGenBuffers(6, quad_instanceVBOs);
  // glGenBuffers(1, &FBO);
  // glGenBuffers(1, &TBO);
  // 变换前原始矩形(1x1像素)
  // 初始化默认纹理
  _deftexture = std::make_shared<Texture>();
  float basic_quad_data[30] = {
      -1.0f, -1.0f, 0.0f, -1.0f, -1.0f, // v1
      1.0f,  -1.0f, 0.0f, 1.0f,  -1.0f, // v2
      1.0f,  1.0f,  0.0f, 1.0f,  1.0f,  // v3
      1.0f,  1.0f,  0.0f, 1.0f,  1.0f,  // v3
      -1.0f, 1.0f,  0.0f, -1.0f, 1.0f,  // v4
      -1.0f, -1.0f, 0.0f, -1.0f, -1.0f, // v1
  };

  // 绑定顶点数组缓冲对象
  glBindVertexArray(VAO);
  // 绑定顶点数据缓冲对象
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // 上传原始矩形数据到缓冲区(VBO中)
  glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(float), basic_quad_data,
               GL_STATIC_DRAW);

  // 描述location0 顶点缓冲0~2float为float类型数据(用vec3接收)
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);

  // 描述location2 顶点缓冲3~4float为float类型数据(用vec2接收为uv坐标)
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));

  // 绑定坐标缓冲(矩形中心)
  glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, max_quad_count * 2 * sizeof(float), nullptr,
               GL_DYNAMIC_DRAW);
  // 描述location3 为2*float
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  // 每个实例变化一次
  glVertexAttribDivisor(2, 1);

  // 绑定尺寸缓冲(x和y的缩放倍率)
  glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[1]);
  glBufferData(GL_ARRAY_BUFFER, max_quad_count * 2 * sizeof(float), nullptr,
               GL_DYNAMIC_DRAW);
  // 描述location3 为2*float
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  // 每个实例变化一次
  glVertexAttribDivisor(3, 1);

  // 绑定旋转角缓冲
  glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[2]);
  glBufferData(GL_ARRAY_BUFFER, max_quad_count * sizeof(float), nullptr,
               GL_DYNAMIC_DRAW);
  // 描述location4 为float
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void *)0);
  // 每个实例变化一次
  glVertexAttribDivisor(4, 1);

  // 绑定颜色缓冲
  glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[3]);
  glBufferData(GL_ARRAY_BUFFER, max_quad_count * 4 * sizeof(float), nullptr,
               GL_DYNAMIC_DRAW);
  // 描述location5 为4*float
  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  // 每个实例变化一次
  glVertexAttribDivisor(5, 1);

  // 绑定纹理坐标缓冲
  glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[4]);
  glBufferData(GL_ARRAY_BUFFER, max_quad_count * 2 * 6 * sizeof(float), nullptr,
               GL_DYNAMIC_DRAW);
  // 描述location6 为2*float
  glEnableVertexAttribArray(6);
  glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  // 每个顶点变化一次
  glVertexAttribDivisor(6, 0);

  // 绑定纹理索引缓冲
  glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[5]);
  glBufferData(GL_ARRAY_BUFFER, max_quad_count * sizeof(float), nullptr,
               GL_DYNAMIC_DRAW);
  // 描述location7 为float
  glEnableVertexAttribArray(7);
  glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void *)0);
  // 每个实例变化一次
  glVertexAttribDivisor(7, 1);

  updateinstanceoffset();

  // 解绑结束构造
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

Mesh::~Mesh() {
  unbind();
  if (FBO)
    glDeleteBuffers(1, &FBO);
  if (TBO)
    glDeleteBuffers(1, &TBO);
  glDeleteBuffers(6, quad_instanceVBOs);
  glDeleteVertexArrays(1, &VAO);
}
void Mesh::bind() {
  // 绑定VAO，VBO
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[0]);
}
void Mesh::unbind() {
  // 绑定VAO，VBO
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Mesh::updateinstanceoffset(uintptr_t instance_offset) {
  glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[0]);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                        (void *)(2 * 4 * instance_offset));

  glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[1]);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                        (void *)(2 * 4 * instance_offset));

  glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[2]);
  glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float),
                        (void *)(4 * instance_offset));

  glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[3]);
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(4 * 4 * instance_offset));

  glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[4]);
  glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                        (void *)(2 * 6 * 4 * instance_offset));

  glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[5]);
  glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float),
                        (void *)(4 * instance_offset));
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
  // (quad_instanceVBOs[0]:x,y中心坐标,
  // quad_instanceVBOs[1]:尺寸
  // quad_instanceVBOs[2]:旋转角度,
  // quad_instanceVBOs[3]:r,g,b,a颜色,
  // quad_instanceVBOs[4]:u,v贴图坐标,
  // quad_instanceVBOs[5]:texid贴图索引)
  // std::cout << "当前显存数据:" << std::endl;
  // auto gldata = std::vector<float>(_quads.size());
  // std::cout << "center point:" << std::endl;
  // glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[0]);
  // gldata = std::vector<float>(_quads.size() * 2);
  // glGetBufferSubData(GL_ARRAY_BUFFER, 0, gldata.size() * 4, gldata.data());
  // for (auto data : gldata) {
  //  std::cout << (data) << ",";
  //}
  // std::cout << std::endl;
  // std::cout << "quad size:" << std::endl;
  // glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[1]);
  // glGetBufferSubData(GL_ARRAY_BUFFER, 0, gldata.size() * 4, gldata.data());
  // for (auto data : gldata) {
  //  std::cout << (data) << ",";
  //}
  // std::cout << std::endl;
  // std::cout << "quad rotation:" << std::endl;
  // glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[2]);
  // gldata = std::vector<float>(_quads.size());
  // glGetBufferSubData(GL_ARRAY_BUFFER, 0, gldata.size() * 4, gldata.data());
  // for (auto data : gldata) {
  //  std::cout << (data) << ",";
  //}
  // std::cout << std::endl;
  // std::cout << "quad color:" << std::endl;
  // glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[3]);
  // gldata = std::vector<float>(_quads.size() * 4);
  // glGetBufferSubData(GL_ARRAY_BUFFER, 0, gldata.size() * 4, gldata.data());
  // for (auto data : gldata) {
  //  std::cout << (data) << ",";
  //}
  // std::cout << std::endl;
  // std::cout << "quad uv:" << std::endl;
  // glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[4]);
  // gldata = std::vector<float>(_quads.size() * 6 * 2);
  // glGetBufferSubData(GL_ARRAY_BUFFER, 0, gldata.size() * 4, gldata.data());
  // for (int i = 0; i < gldata.size(); i++) {
  //  std::cout << gldata[i] << ",";
  //  if ((i + 1) % 2 == 0) {
  //    std::cout << '\t';
  //  }
  //  if ((i + 1) % 12 == 0) {
  //    std::cout << std::endl;
  //  }
  //}
  // std::cout << std::endl;
  // std::cout << "quad texid:" << std::endl;
  // glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[5]);
  // gldata = std::vector<float>(_quads.size());
  // glGetBufferSubData(GL_ARRAY_BUFFER, 0, gldata.size() * 4, gldata.data());
  // for (auto data : gldata) {
  //  std::cout << (data) << ",";
  //}
  // std::cout << std::endl;
  // std::cout << "---------------------------------------------------"
  //          << std::endl;

  // 记录下每一段需要更新的连续的内存,也可以只有一份数据
  std::vector<std::shared_ptr<Quad>> *consequent_quads_pos = nullptr;
  std::vector<float> *consequent_quads_pos_data = nullptr;
  std::vector<std::shared_ptr<Quad>> *consequent_quads_size = nullptr;
  std::vector<float> *consequent_quads_size_data = nullptr;
  std::vector<std::shared_ptr<Quad>> *consequent_quads_rotation = nullptr;
  std::vector<float> *consequent_quads_rotation_data = nullptr;
  std::vector<std::shared_ptr<Quad>> *consequent_quads_color = nullptr;
  std::vector<float> *consequent_quads_color_data = nullptr;
  std::vector<std::shared_ptr<Quad>> *consequent_quads_uv = nullptr;
  std::vector<float> *consequent_quads_uv_data = nullptr;
  std::vector<std::shared_ptr<Quad>> *consequent_quads_texid = nullptr;
  std::vector<float> *consequent_quads_texid_data = nullptr;
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
    // 2d坐标
    quad->model_data_offset[0] = _current_handle_index * sizeof(glm::vec2);
    // 2d尺寸
    quad->model_data_offset[1] = _current_handle_index * sizeof(glm::vec2);
    // 旋转角度
    quad->model_data_offset[2] = _current_handle_index * sizeof(float);
    // 颜色
    quad->model_data_offset[3] = _current_handle_index * sizeof(glm::vec4);
    // uv
    quad->model_data_offset[4] = _current_handle_index * 6 * sizeof(glm::vec2);
    // texid
    quad->model_data_offset[5] = _current_handle_index * sizeof(float);
    // 直接加入更新列表和缓存
    _quads.push_back(quad);

    // 各自更新
    // 更新相关连续区段
    // 更新位置信息
    update_consecutive(_update_consequent_quads_poss, consequent_quads_pos,
                       _update_consequent_quads_pos_datas,
                       consequent_quads_pos_data, POSITION, quad);

    // 更新尺寸信息
    update_consecutive(_update_consequent_quads_sizes, consequent_quads_size,
                       _update_consequent_quads_size_datas,
                       consequent_quads_size_data, SIZE, quad);

    // 更新旋转信息
    update_consecutive(_update_consequent_quads_rotations,
                       consequent_quads_rotation,
                       _update_consequent_quads_rotation_datas,
                       consequent_quads_rotation_data, ROTATION, quad);

    // 更新颜色信息
    update_consecutive(_update_consequent_quads_colors, consequent_quads_color,
                       _update_consequent_quads_color_datas,
                       consequent_quads_color_data, COLOR, quad);

    // 更新uv坐标信息
    update_consecutive(_update_consequent_quads_uvs, consequent_quads_uv,
                       _update_consequent_quads_uv_datas,
                       consequent_quads_uv_data, UV, quad);

    // 更新纹理id信息
    update_consecutive(_update_consequent_quads_texids, consequent_quads_texid,
                       _update_consequent_quads_texid_datas,
                       consequent_quads_texid_data, TEXID, quad);

    updatequadbatch(quad);
  } else {
    // 当前没有处理到结尾处
    // 获取当前位置矩形
    std::shared_ptr<Quad> _handle_quad = _quads[_current_handle_index];
    // 检查当前绘制到的矩形内容是否一致
    bool is_update{false};
    // 检查当前绘制到的矩形内容是否一致
    if (!_handle_quad->iscpequal(cp)) {
      // 坐标不同，需要更新
      _handle_quad->_cp = cp;
      update_consecutive(_update_consequent_quads_poss, consequent_quads_pos,
                         _update_consequent_quads_pos_datas,
                         consequent_quads_pos_data, POSITION, _handle_quad);
      is_update = true;
    }
    if (!_handle_quad->iswhequal(w, h)) {
      _handle_quad->_w = w;
      _handle_quad->_h = h;
      update_consecutive(_update_consequent_quads_sizes, consequent_quads_size,
                         _update_consequent_quads_size_datas,
                         consequent_quads_size_data, SIZE, _handle_quad);
      is_update = true;
    }
    if (!_handle_quad->isrotationequal(rotation)) {
      _handle_quad->_rotation = rotation;
      update_consecutive(
          _update_consequent_quads_rotations, consequent_quads_rotation,
          _update_consequent_quads_rotation_datas,
          consequent_quads_rotation_data, ROTATION, _handle_quad);
      is_update = true;
    }
    if (!_handle_quad->iscolorequal(color)) {
      _handle_quad->_color = color;
      update_consecutive(_update_consequent_quads_colors,
                         consequent_quads_color,
                         _update_consequent_quads_color_datas,
                         consequent_quads_color_data, COLOR, _handle_quad);

      is_update = true;
    }
    if (!_handle_quad->isuvequal(texture_type)) {
      _handle_quad->settextyppe(texture_type);
      update_consecutive(_update_consequent_quads_uvs, consequent_quads_uv,
                         _update_consequent_quads_uv_datas,
                         consequent_quads_uv_data, UV, _handle_quad);
      is_update = true;
    }
    if (!_handle_quad->istexequal(texture)) {
      _handle_quad->_tex = texture;
      update_consecutive(_update_consequent_quads_texids,
                         consequent_quads_texid,
                         _update_consequent_quads_texid_datas,
                         consequent_quads_texid_data, TEXID, _handle_quad);
      is_update = true;
    }
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
    std::vector<float> *current_consequent_data, DataType dataType,
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
  //  更新数据
  switch (dataType) {
  case POSITION: {
    current_consequent_data->push_back(handle_quad->_cp.x);
    current_consequent_data->push_back(handle_quad->_cp.y);
    break;
  }
  case SIZE: {
    current_consequent_data->push_back(handle_quad->_w);
    current_consequent_data->push_back(handle_quad->_h);
    break;
  }
  case ROTATION: {
    current_consequent_data->push_back(handle_quad->_rotation);
    break;
  }
  case COLOR: {
    current_consequent_data->push_back(handle_quad->_color.r);
    current_consequent_data->push_back(handle_quad->_color.g);
    current_consequent_data->push_back(handle_quad->_color.b);
    current_consequent_data->push_back(handle_quad->_color.a);
    break;
  }
  case UV: {
    current_consequent_data->push_back(handle_quad->_uv1.x);
    current_consequent_data->push_back(handle_quad->_uv1.y);
    current_consequent_data->push_back(handle_quad->_uv2.x);
    current_consequent_data->push_back(handle_quad->_uv2.y);
    current_consequent_data->push_back(handle_quad->_uv3.x);
    current_consequent_data->push_back(handle_quad->_uv3.y);
    current_consequent_data->push_back(handle_quad->_uv3.x);
    current_consequent_data->push_back(handle_quad->_uv3.y);
    current_consequent_data->push_back(handle_quad->_uv4.x);
    current_consequent_data->push_back(handle_quad->_uv4.y);
    current_consequent_data->push_back(handle_quad->_uv1.x);
    current_consequent_data->push_back(handle_quad->_uv1.y);
    break;
  }
  case TEXID: {
    current_consequent_data->push_back((float)(handle_quad->_tex->texid));
    break;
  }
  }
}

void Mesh::finish() {
  // std::cout << "cpu size buffer:" << std::endl;
  // for (auto &datas : _update_consequent_quads_size_datas) {
  //   for (auto data : datas) {
  //     std::cout << data << ",";
  //   }
  //   std::cout << std::endl;
  // }
  //  处理显存
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
      glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[0]);
      glBufferSubData(GL_ARRAY_BUFFER, consequent_pos_data_start_offset,
                      consequent_pos_data_size,
                      _update_consequent_quads_pos_datas[i].data());
    }
  }
  // 更新连续的尺寸数据
  if (!_update_consequent_quads_sizes.empty()) {
    for (int i = 0; i < _update_consequent_quads_sizes.size(); i++) {
      auto quads_size = _update_consequent_quads_sizes[i];
      uint32_t consequent_size_data_start_offset =
          quads_size.front()->model_data_offset[1];
      uint32_t consequent_size_data_size =
          quads_size.back()->model_data_offset[1] -
          consequent_size_data_start_offset + sizeof(glm::vec2);
      glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[1]);
      glBufferSubData(GL_ARRAY_BUFFER, consequent_size_data_start_offset,
                      consequent_size_data_size,
                      _update_consequent_quads_size_datas[i].data());
    }
  }
  // 更新连续的旋转数据
  if (!_update_consequent_quads_rotations.empty()) {
    for (int i = 0; i < _update_consequent_quads_rotations.size(); i++) {
      auto quads_rotation = _update_consequent_quads_rotations[i];
      uint32_t consequent_rotation_data_start_offset =
          quads_rotation.front()->model_data_offset[2];
      uint32_t consequent_rotation_data_size =
          quads_rotation.back()->model_data_offset[2] -
          consequent_rotation_data_start_offset + sizeof(float);
      glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[2]);
      glBufferSubData(GL_ARRAY_BUFFER, consequent_rotation_data_start_offset,
                      consequent_rotation_data_size,
                      _update_consequent_quads_rotation_datas[i].data());
    }
  }
  // 颜色
  if (!_update_consequent_quads_colors.empty()) {
    for (int i = 0; i < _update_consequent_quads_colors.size(); i++) {
      auto quads_color = _update_consequent_quads_colors[i];
      uint32_t consequent_color_data_start_offset =
          quads_color.front()->model_data_offset[3];
      uint32_t consequent_color_data_size =
          quads_color.back()->model_data_offset[3] -
          consequent_color_data_start_offset + sizeof(glm::vec4);
      glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[3]);
      glBufferSubData(GL_ARRAY_BUFFER, consequent_color_data_start_offset,
                      consequent_color_data_size,
                      _update_consequent_quads_color_datas[i].data());
    }
  }
  // uv
  if (!_update_consequent_quads_uvs.empty()) {
    for (int i = 0; i < _update_consequent_quads_uvs.size(); i++) {
      auto quads_uv = _update_consequent_quads_uvs[i];
      uint32_t consequent_uv_data_start_offset =
          quads_uv.front()->model_data_offset[4];
      uint32_t consequent_uv_data_size = quads_uv.back()->model_data_offset[4] -
                                         consequent_uv_data_start_offset +
                                         6 * sizeof(glm::vec2);
      glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[4]);
      glBufferSubData(GL_ARRAY_BUFFER, consequent_uv_data_start_offset,
                      consequent_uv_data_size,
                      _update_consequent_quads_uv_datas[i].data());
    }
  }
  // texid
  if (!_update_consequent_quads_texids.empty()) {
    for (int i = 0; i < _update_consequent_quads_texids.size(); i++) {
      auto quads_texid = _update_consequent_quads_texids[i];
      uint32_t consequent_texid_data_start_offset =
          quads_texid.front()->model_data_offset[5];
      uint32_t consequent_texid_data_size =
          quads_texid.back()->model_data_offset[5] -
          consequent_texid_data_start_offset + sizeof(float);
      glBindBuffer(GL_ARRAY_BUFFER, quad_instanceVBOs[5]);
      glBufferSubData(GL_ARRAY_BUFFER, consequent_texid_data_start_offset,
                      consequent_texid_data_size,
                      _update_consequent_quads_texid_datas[i].data());
    }
  }
  // 绑定默认1*1白色纹理到纹理槽0
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _deftexture->texture);
  if (_max_texid == 0) {
    // 都是默认纹理
    // 直接全部绘制
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, (int)(_quads.size()));
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
      glDrawArraysInstanced(GL_TRIANGLES, 0, 6, (int)(quadBatch->batch.size()));
    }
  }
  updateinstanceoffset();
  //  恢复处理索引
  _current_handle_index = 0;
  // 清空批次
  _all_batchs.clear();
  // 清除需要更新的内容
  _update_consequent_quads_poss.clear();
  _update_consequent_quads_pos_datas.clear();
  _update_consequent_quads_sizes.clear();
  _update_consequent_quads_size_datas.clear();
  _update_consequent_quads_rotations.clear();
  _update_consequent_quads_rotation_datas.clear();
  _update_consequent_quads_colors.clear();
  _update_consequent_quads_color_datas.clear();
  _update_consequent_quads_uvs.clear();
  _update_consequent_quads_uv_datas.clear();
  _update_consequent_quads_texids.clear();
  _update_consequent_quads_texid_datas.clear();
}

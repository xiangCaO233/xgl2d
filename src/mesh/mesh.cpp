#include "mesh.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

Mesh::Mesh(Shader *shader, std::string &texture_dir, int oval_segment,
           int initial_oval_count, int initial_quad_count)
    : _shader(shader), _oval_segment(oval_segment),
      max_oval_count(initial_oval_count), max_quad_count(initial_quad_count) {
  // 初始化缓冲对象
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &instanceVBO);
  // glGenBuffers(1, &FBO);
  // 初始化矩形顶点
  std::vector<float> vertices = {
      -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // v1
      1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // v2
      1.0f,  1.0f,  0.0f, 1.0f, 1.0f, // v3
      -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, // v4
  };
  // 初始化椭圆顶点
  for (int i = 0; i < oval_segment; i++) {
    float angle = 2.0f * M_PI * float(i) / float(oval_segment);
    float x = cos(angle);
    float y = sin(angle);
    float texcoordx = 0.5f + 0.5f * x;
    float texcoordy = 0.5f + 0.5f * y;
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(0.0f);
    vertices.push_back(texcoordx);
    vertices.push_back(texcoordy);
  }

  // 绑定顶点数组缓冲对象
  glBindVertexArray(VAO);
  // 绑定顶点数据缓冲对象
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // 上传原始矩形数据到缓冲区(VBO中)
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);

  // 描述location0 顶点缓冲0~2float为float类型数据(用vec3接收)
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);

  // 描述location1 顶点缓冲3~4float为float类型数据(用vec2接收为默认uv坐标)
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));

  // 实例数据
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  glBufferData(GL_ARRAY_BUFFER, max_quad_count * 11 * sizeof(float), nullptr,
               GL_DYNAMIC_DRAW);
  // 描述location3 为2*float(0~max_quad_count * 2float内存块)
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
  // 每个实例变化一次
  glVertexAttribDivisor(2, 1);

  // 绑定尺寸缓冲(x和y的缩放倍率)
  // 描述location3 为2*float(max_quad_count * 2 ~ max_quad_count * 4float内存块)
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                        (void *)(max_quad_count * 2 * sizeof(float)));
  // 每个实例变化一次
  glVertexAttribDivisor(3, 1);

  // 绑定旋转角缓冲
  // 描述location4 为float(max_quad_count * 4 ~ max_quad_count * 5float内存块)
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float),
                        (void *)(max_quad_count * 4 * sizeof(float)));
  // 每个实例变化一次
  glVertexAttribDivisor(4, 1);

  // 绑定颜色缓冲
  // 描述location5 为4*float(max_quad_count * 5 ~ max_quad_count * 9float内存块)
  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(max_quad_count * 5 * sizeof(float)));
  // 每个实例变化一次
  glVertexAttribDivisor(5, 1);

  // 绑定纹理元数据索引缓冲
  // 描述location6 为float(max_quad_count * 9 ~ max_quad_count * 10float内存块)
  glEnableVertexAttribArray(6);
  glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(float),
                        (void *)(max_quad_count * 9 * sizeof(float)));
  // 每个实例变化一次
  glVertexAttribDivisor(6, 1);

  // 绑定纹理填充选项缓冲
  // 描述 location7 为 float(max_quad_count * 10 ~ max_quad_count *
  // 11float内存块)
  glEnableVertexAttribArray(7);
  glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(float),
                        (void *)(max_quad_count * 10 * sizeof(float)));
  glVertexAttribDivisor(7, 1);

  // 初始化纹理池
  _texpool = new Texturepool(texture_dir, shader);
  _texpool->creatatlas();
  // 绑定纹理池
  _texpool->bind();

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

void Mesh::updateVAOpointer(uintptr_t shapecount) {

  // 描述location3 为2*float(0~max_quad_count * 2float内存块)
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                        (void *)(shapecount * 2 * sizeof(float)));
  // 绑定尺寸缓冲(x和y的缩放倍率)
  glVertexAttribPointer(
      3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
      (void *)((max_quad_count + shapecount) * 2 * sizeof(float)));
  // 绑定旋转角缓冲
  glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float),
                        (void *)(max_quad_count * 4 * sizeof(float) +
                                 shapecount * sizeof(float)));
  // 绑定颜色缓冲
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(max_quad_count * 5 * sizeof(float) +
                                 shapecount * 4 * sizeof(float)));
  // 绑定纹理元数据索引缓冲
  glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(float),
                        (void *)(max_quad_count * 9 * sizeof(float) +
                                 shapecount * sizeof(float)));
  // 绑定纹理填充选项缓冲
  glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(float),
                        (void *)(max_quad_count * 10 * sizeof(float) +
                                 shapecount * sizeof(float)));
};
// 绘制矩形
void Mesh::drawquad(glm::vec2 &cp, float w, float h, float rotation,
                    glm::vec4 &color, std::shared_ptr<TextureMeta> &texmeta,
                    TexType texture_type, glm::vec2 &screen_size, bool isoval) {
  rotation = -rotation;
  // debug
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
  std::vector<std::shared_ptr<Quad>> *consequent_quads_texmeta = nullptr;
  std::vector<float> *consequent_quads_texmeta_data = nullptr;
  std::vector<std::shared_ptr<Quad>> *consequent_quads_uvarg = nullptr;
  std::vector<float> *consequent_quads_uvarg_data = nullptr;
  // 判断当前缓存矩形列表是否到达末尾
  if (_consequent_shapes.empty() ||
      _current_handle_index ==
          _consequent_shapes.back().back()->_draw_order + 1) {
    // 缓存shapes
    std::vector<std::shared_ptr<Quad>> *tempshapes = nullptr;
    // 检查是否需要创建新序列的条件
    bool needNewSequence =
        _consequent_shapes.empty() ||
        !_consequent_shapes.back().back()->is_oval_equal(isoval);
    if (needNewSequence) {
      // 创建新序列
      _consequent_shapes.emplace_back();
    }
    // 引用最后的序列
    tempshapes = &_consequent_shapes.back();

    // 初始化此矩形
    std::shared_ptr<Quad> quad = std::make_shared<Quad>(
        cp, w, h, rotation, color, texmeta, texture_type);
    quad->_isoval = isoval;
    quad->_draw_order = _current_handle_index;
    // 设置当前的所处更新位置
    quad->updateindexlist = _consequent_shapes.size() - 1;
    quad->updateindexelement = tempshapes->size();
    // 设置模型数据偏移点
    // 内存偏移
    // posdata
    quad->model_data_offset[0] = _current_handle_index * 2 * sizeof(float);
    // sizedata
    quad->model_data_offset[1] = max_quad_count * 2 * sizeof(float) +
                                 _current_handle_index * 2 * sizeof(float);
    // rotationdata
    quad->model_data_offset[2] = max_quad_count * 4 * sizeof(float) +
                                 _current_handle_index * sizeof(float);
    // colordata
    quad->model_data_offset[3] = max_quad_count * 5 * sizeof(float) +
                                 _current_handle_index * 4 * sizeof(float);
    // texmetadata
    quad->model_data_offset[4] = max_quad_count * 9 * sizeof(float) +
                                 _current_handle_index * sizeof(float);
    // uvargdata
    quad->model_data_offset[5] = max_quad_count * 10 * sizeof(float) +
                                 _current_handle_index * sizeof(float);
    // 直接加入更新序列和缓存序列
    tempshapes->push_back(quad);
    // 加入缓存
    _quads.push_back(quad);

    // 更新数据
    update_consecutive(_update_consequent_quads_poss, consequent_quads_pos,
                       _update_consequent_quads_pos_datas,
                       consequent_quads_pos_data, quad, Datatype::POSITION);
    update_consecutive(_update_consequent_quads_sizes, consequent_quads_size,
                       _update_consequent_quads_size_datas,
                       consequent_quads_size_data, quad, Datatype::SIZE);
    update_consecutive(
        _update_consequent_quads_rotations, consequent_quads_rotation,
        _update_consequent_quads_rotation_datas, consequent_quads_rotation_data,
        quad, Datatype::ROTATION);
    update_consecutive(_update_consequent_quads_colors, consequent_quads_color,
                       _update_consequent_quads_color_datas,
                       consequent_quads_color_data, quad, Datatype::COLOR);
    update_consecutive(_update_consequent_quads_texmetas,
                       consequent_quads_texmeta,
                       _update_consequent_quads_texmeta_datas,
                       consequent_quads_texmeta_data, quad, Datatype::TEXMETA);
    update_consecutive(_update_consequent_quads_uvargs, consequent_quads_uvarg,
                       _update_consequent_quads_uvarg_datas,
                       consequent_quads_uvarg_data, quad, Datatype::UVARG);
  } else {
    // 当前没有处理到结尾处
    // 获取当前位置形状
    std::shared_ptr<Quad> &_handle_quad = _quads[_current_handle_index];
    // 检查当前绘制到的内容是否一致
    if (!_handle_quad->is_cp_equal(cp)) {
      _handle_quad->_cp = cp;
      update_consecutive(_update_consequent_quads_poss, consequent_quads_pos,
                         _update_consequent_quads_pos_datas,
                         consequent_quads_pos_data, _handle_quad,
                         Datatype::POSITION);
    };
    if (!_handle_quad->is_size_equal(w, h)) {
      _handle_quad->_w = w;
      _handle_quad->_h = h;
      update_consecutive(_update_consequent_quads_sizes, consequent_quads_size,
                         _update_consequent_quads_size_datas,
                         consequent_quads_size_data, _handle_quad,
                         Datatype::SIZE);
    };
    if (!_handle_quad->is_rotation_equal(rotation)) {
      _handle_quad->_rotation = rotation;
      update_consecutive(
          _update_consequent_quads_rotations, consequent_quads_rotation,
          _update_consequent_quads_rotation_datas,
          consequent_quads_rotation_data, _handle_quad, Datatype::ROTATION);
    };
    if (!_handle_quad->is_color_equal(color)) {
      _handle_quad->_color = color;
      update_consecutive(
          _update_consequent_quads_colors, consequent_quads_color,
          _update_consequent_quads_color_datas, consequent_quads_color_data,
          _handle_quad, Datatype::COLOR);
    };
    if (!_handle_quad->is_tex_equal(texmeta)) {
      _handle_quad->_texmeta = texmeta;
      update_consecutive(
          _update_consequent_quads_texmetas, consequent_quads_texmeta,
          _update_consequent_quads_texmeta_datas, consequent_quads_texmeta_data,
          _handle_quad, Datatype::TEXMETA);
    };
    if (!_handle_quad->is_textype_equal(texture_type)) {
      _handle_quad->_textype = texture_type;
      update_consecutive(
          _update_consequent_quads_uvargs, consequent_quads_uvarg,
          _update_consequent_quads_uvarg_datas, consequent_quads_uvarg_data,
          _handle_quad, Datatype::UVARG);
    };
    if (!_handle_quad->is_oval_equal(isoval)) {
      // 形状改变了
      if (_consequent_shapes.size() >= 2) {
        // 前段有其他形状绘制
        // 首选向前合并
        // 将自己从当前所处更新列表弹出
        _consequent_shapes[_handle_quad->updateindexlist].pop_back();
        // 加入前一个列表
        _consequent_shapes[_handle_quad->updateindexlist - 1].push_back(
            _handle_quad);
        // 更改对应的元素标识
        _handle_quad->updateindexlist--;
        _handle_quad->updateindexelement =
            _consequent_shapes[_handle_quad->updateindexlist].size();
      } else {
        // 新增更新列表
        _consequent_shapes.emplace_back();
        _consequent_shapes.back().push_back(_handle_quad);
        // 更改对应的元素标识
        _handle_quad->updateindexlist++;
        _handle_quad->updateindexelement = 0;
      }
    }
  }
  _current_handle_index++;
};
void Mesh::drawquad(glm::vec2 &&cp, float w, float h, float rotation,
                    glm::vec4 &color, std::shared_ptr<TextureMeta> &texture,
                    TexType texture_type, glm::vec2 &screen_size) {
  drawquad(cp, w, h, rotation, color, texture, texture_type, screen_size);
};
void Mesh::drawquad(glm::vec2 &cp, float w, float h, float rotation,
                    glm::vec4 &&color, std::shared_ptr<TextureMeta> &texture,
                    TexType texture_type, glm::vec2 &screen_size) {
  drawquad(cp, w, h, rotation, color, texture, texture_type, screen_size);
};
void Mesh::drawquad(glm::vec2 &&cp, float w, float h, float rotation,
                    glm::vec4 &&color, std::shared_ptr<TextureMeta> &texture,
                    TexType texture_type, glm::vec2 &screen_size) {
  drawquad(cp, w, h, rotation, color, texture, texture_type, screen_size);
};
void Mesh::drawquad(glm::vec2 &cp, float w, float h, float rotation,
                    glm::vec4 &color, glm::vec2 &screen_size) {
  drawquad(cp, w, h, rotation, color, _texpool->_defmeta, FILL, screen_size);
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
                    std::shared_ptr<TextureMeta> &texture, TexType texture_type,
                    glm::vec2 &screen_size) {
  drawquad(cp, w, h, rotation, {1.0f, 1.0f, 1.0f, 1.0f}, texture, texture_type,
           screen_size);
};
void Mesh::drawquad(glm::vec2 &&cp, float w, float h, float rotation,
                    std::shared_ptr<TextureMeta> &texture, TexType texture_type,
                    glm::vec2 &screen_size) {
  drawquad(cp, w, h, rotation, texture, texture_type, screen_size);
};

void Mesh::drawoval(glm::vec2 &cp, float rx, float ry, float rotation,
                    glm::vec4 &color, std::shared_ptr<TextureMeta> &texture,
                    TexType texture_type, glm::vec2 &screensize) {
  drawquad(cp, rx, ry, rotation, color, texture, texture_type, screensize,
           true);
};
void Mesh::drawoval(glm::vec2 &&cp, float rx, float ry, float rotation,
                    glm::vec4 &color, std::shared_ptr<TextureMeta> &texture,
                    TexType texture_type, glm::vec2 &screen_size) {
  drawoval(cp, rx, ry, rotation, color, texture, texture_type, screen_size);
};
void Mesh::drawoval(glm::vec2 &cp, float rx, float ry, float rotation,
                    glm::vec4 &&color, std::shared_ptr<TextureMeta> &texture,
                    TexType texture_type, glm::vec2 &screen_size) {

  drawoval(cp, rx, ry, rotation, color, texture, texture_type, screen_size);
};
void Mesh::drawoval(glm::vec2 &&cp, float rx, float ry, float rotation,
                    glm::vec4 &&color, std::shared_ptr<TextureMeta> &texture,
                    TexType texture_type, glm::vec2 &screen_size) {

  drawoval(cp, rx, ry, rotation, color, texture, texture_type, screen_size);
};
void Mesh::drawoval(glm::vec2 &cp, float rx, float ry, float rotation,
                    glm::vec4 &color, glm::vec2 &screen_size) {

  drawoval(cp, rx, ry, rotation, color, _texpool->_defmeta, FILL, screen_size);
};
void Mesh::drawoval(glm::vec2 &&cp, float rx, float ry, float rotation,
                    glm::vec4 &color, glm::vec2 &screen_size) {

  drawoval(cp, rx, ry, rotation, color, screen_size);
};
void Mesh::drawoval(glm::vec2 &cp, float rx, float ry, float rotation,
                    glm::vec4 &&color, glm::vec2 &screen_size) {

  drawoval(cp, rx, ry, rotation, color, screen_size);
};
void Mesh::drawoval(glm::vec2 &&cp, float rx, float ry, float rotation,
                    glm::vec4 &&color, glm::vec2 &screen_size) {

  drawoval(cp, rx, ry, rotation, color, screen_size);
};
void Mesh::drawvoal(glm::vec2 &cp, float rx, float ry, float rotation,
                    std::shared_ptr<TextureMeta> &texture, TexType texture_type,
                    glm::vec2 &screen_size) {
  drawoval(cp, rx, ry, rotation, {1.0f, 1.0f, 1.0f, 1.0f}, texture,
           texture_type, screen_size);
};
void Mesh::drawoval(glm::vec2 &&cp, float rx, float ry, float rotation,
                    std::shared_ptr<TextureMeta> &texture, TexType texture_type,
                    glm::vec2 &screen_size) {
  drawoval(cp, rx, ry, rotation, {1.0f, 1.0f, 1.0f, 1.0f}, texture,
           texture_type, screen_size);
};

// 通用的连续更新处理函数
void Mesh::update_consecutive(
    std::vector<std::vector<std::shared_ptr<Quad>>> &update_consequent_list,
    std::vector<std::shared_ptr<Quad>> *current_consequent_list,
    std::vector<std::vector<float>> &update_consequent_data_list,
    std::vector<float> *current_consequent_data,
    const std::shared_ptr<Quad> &handle_quad, Datatype type) const {
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
  switch (type) {
  case Datatype::POSITION: {
    current_consequent_data->push_back(handle_quad->_cp.x);
    current_consequent_data->push_back(handle_quad->_cp.y);
    break;
  }
  case Datatype::SIZE: {
    current_consequent_data->push_back(handle_quad->_w);
    current_consequent_data->push_back(handle_quad->_h);
    break;
  }
  case Datatype::ROTATION: {
    current_consequent_data->push_back(handle_quad->_rotation);
    break;
  }
  case Datatype::COLOR: {
    current_consequent_data->push_back(handle_quad->_color.r);
    current_consequent_data->push_back(handle_quad->_color.g);
    current_consequent_data->push_back(handle_quad->_color.b);
    current_consequent_data->push_back(handle_quad->_color.a);
    break;
  }
  case Datatype::TEXMETA: {
    current_consequent_data->push_back(handle_quad->_texmeta->metaid);
    break;
  }
  case Datatype::UVARG: {
    current_consequent_data->push_back(handle_quad->_textype);
    break;
  }
  }
}

void Mesh::finish() {
  //  处理显存
  if (_current_handle_index < _quads.size() - 1) {
    // 上一帧还有残余数据
    // 擦除后面部分的缓存
    auto &shape = _quads[_current_handle_index];
    // 删除此shape之后全部的更新列表
    _consequent_shapes.erase(_consequent_shapes.begin() +
                                 shape->updateindexlist + 1,
                             _consequent_shapes.end());
    // 擦出此shape所处更新列表之后的全部
    auto &currentlist = _consequent_shapes[shape->updateindexlist];
    currentlist.erase(currentlist.begin() + shape->updateindexelement,
                      currentlist.end());
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
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  if (!_update_consequent_quads_poss.empty()) {
    // 需要更新矩形位置数据
    for (int i = 0; i < _update_consequent_quads_poss.size(); i++) {
      auto &quads = _update_consequent_quads_poss[i];
      // 计算连续显存的起始偏移
      uint32_t consequent_data_start_offset =
          quads.front()->model_data_offset[0];
      // 计算连续的显存大小
      uint32_t consequent_data_size = quads.back()->model_data_offset[0] -
                                      consequent_data_start_offset +
                                      2 * sizeof(float);
      // 更新那一段显存
      glBufferSubData(GL_ARRAY_BUFFER, consequent_data_start_offset,
                      consequent_data_size,
                      _update_consequent_quads_pos_datas[i].data());
    }
  }
  if (!_update_consequent_quads_sizes.empty()) {
    for (int i = 0; i < _update_consequent_quads_sizes.size(); i++) {
      auto &quads = _update_consequent_quads_sizes[i];
      uint32_t consequent_data_start_offset =
          quads.front()->model_data_offset[1];
      uint32_t consequent_data_size = quads.back()->model_data_offset[1] -
                                      consequent_data_start_offset +
                                      2 * sizeof(float);
      glBufferSubData(GL_ARRAY_BUFFER, consequent_data_start_offset,
                      consequent_data_size,
                      _update_consequent_quads_size_datas[i].data());
    }
  }
  if (!_update_consequent_quads_rotations.empty()) {
    for (int i = 0; i < _update_consequent_quads_rotations.size(); i++) {
      auto &quads = _update_consequent_quads_rotations[i];
      uint32_t consequent_data_start_offset =
          quads.front()->model_data_offset[2];
      uint32_t consequent_data_rotation = quads.back()->model_data_offset[2] -
                                          consequent_data_start_offset +
                                          sizeof(float);
      glBufferSubData(GL_ARRAY_BUFFER, consequent_data_start_offset,
                      consequent_data_rotation,
                      _update_consequent_quads_rotation_datas[i].data());
    }
  }
  if (!_update_consequent_quads_colors.empty()) {
    for (int i = 0; i < _update_consequent_quads_colors.size(); i++) {
      auto &quads = _update_consequent_quads_colors[i];
      uint32_t consequent_data_start_offset =
          quads.front()->model_data_offset[3];
      uint32_t consequent_data_color = quads.back()->model_data_offset[3] -
                                       consequent_data_start_offset +
                                       4 * sizeof(float);
      glBufferSubData(GL_ARRAY_BUFFER, consequent_data_start_offset,
                      consequent_data_color,
                      _update_consequent_quads_color_datas[i].data());
    }
  }
  if (!_update_consequent_quads_texmetas.empty()) {
    for (int i = 0; i < _update_consequent_quads_texmetas.size(); i++) {
      auto &quads = _update_consequent_quads_texmetas[i];
      uint32_t consequent_data_start_offset =
          quads.front()->model_data_offset[4];
      uint32_t consequent_data_texmeta = quads.back()->model_data_offset[4] -
                                         consequent_data_start_offset +
                                         sizeof(float);
      glBufferSubData(GL_ARRAY_BUFFER, consequent_data_start_offset,
                      consequent_data_texmeta,
                      _update_consequent_quads_texmeta_datas[i].data());
    }
  }
  if (!_update_consequent_quads_uvargs.empty()) {
    for (int i = 0; i < _update_consequent_quads_uvargs.size(); i++) {
      auto &quads = _update_consequent_quads_uvargs[i];
      uint32_t consequent_data_start_offset =
          quads.front()->model_data_offset[5];
      uint32_t consequent_data_uvarg = quads.back()->model_data_offset[5] -
                                       consequent_data_start_offset +
                                       sizeof(float);
      glBufferSubData(GL_ARRAY_BUFFER, consequent_data_start_offset,
                      consequent_data_uvarg,
                      _update_consequent_quads_uvarg_datas[i].data());
    }
  }
  // 遍历更新列表
  for (auto &updatelist : _consequent_shapes) {
    updateVAOpointer(updatelist.front()->_draw_order);
    if (updatelist.front()->_isoval) {
      // 绘制椭圆
      glDrawArraysInstanced(GL_TRIANGLE_FAN, 4, _oval_segment,
                            (int)(updatelist.size()));
    } else {
      // 直接全部绘制矩形
      glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, (int)(updatelist.size()));
    }
  }
  //  恢复处理索引
  _current_handle_index = 0;
  // 清除需要更新的内容
  _update_consequent_quads_poss.clear();
  _update_consequent_quads_pos_datas.clear();
  _update_consequent_quads_sizes.clear();
  _update_consequent_quads_size_datas.clear();
  _update_consequent_quads_rotations.clear();
  _update_consequent_quads_rotation_datas.clear();
  _update_consequent_quads_colors.clear();
  _update_consequent_quads_color_datas.clear();
  _update_consequent_quads_texmetas.clear();
  _update_consequent_quads_texmeta_datas.clear();
  _update_consequent_quads_uvargs.clear();
  _update_consequent_quads_uvarg_datas.clear();
}

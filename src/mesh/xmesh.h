#ifndef XMESH_H
#define XMESH_H

#include "../shader/shader.h"
#include "../texture/texture.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

struct Vertex {
  // 顶点元数据
  glm::vec3 position{0.0f, 0.0f, 0.0f};
  glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec2 uv{0.0f, 0.0f};
  // 矩阵变换索引
  float matindex{0};
  // 顶点材质
  float texid{0.0f};
  std::vector<float> dump() {
    std::vector<float> data;
    data.push_back(position.x);
    data.push_back(position.y);
    data.push_back(position.z);
    data.push_back(color.r);
    data.push_back(color.g);
    data.push_back(color.b);
    data.push_back(color.a);
    data.push_back(uv.x);
    data.push_back(uv.y);
    data.push_back(matindex);
    data.push_back(texid);
    return data;
  }
  // dump出变换后的顶点数据
  std::vector<float> dump(glm::mat4 transform) {
    std::vector<float> data;
    glm::vec4 position4 = glm::vec4(position, 1.0f);
    // 变换顶点
    position4 = transform * position4;
    data.push_back(position4.x);
    data.push_back(position4.y);
    data.push_back(position4.z);
    // debug变换结果
    // std::cout << "[x=" + std::to_string(position4.x) +
    //                 ",y=" + std::to_string(position4.y) + "]"
    //          << std::endl;
    // color数据
    data.push_back(color.r);
    data.push_back(color.g);
    data.push_back(color.b);
    data.push_back(color.a);
    // 材质uv数据
    data.push_back(uv.x);
    data.push_back(uv.y);
    // 矩阵索引数据
    data.push_back(matindex);
    // 材质索引数据
    data.push_back(texid);
    return data;
  }
  bool operator==(const Vertex &v) {
    return position == v.position && color == v.color && uv == v.uv &&
           matindex == v.matindex && texid == v.texid;
  };
};

class Xmesh {
protected:
  // 缓冲对象
  // VertexBufferObject VertexArrayObject
  // ElementBufferObject
  GLuint VBO, VAO, EBO;
  Shader *program;
  // mesh顶点数量分配
  uint32_t _vcount_size;
  // mesh顶点缓冲对象实际分配大小(VBO)(byte)
  uint64_t _vbuffer_size;
  // mesh顶点索引数量分配
  uint32_t _velecount_size;
  // mesh顶点索引缓冲对象实际分配大小(EBO)(byte)
  uint64_t _velebuffer_size;

public:
  // 构造Xmesh
  Xmesh(Shader *shader, uint32_t vcount_size = 1024);
  // 析构Xmesh
  virtual ~Xmesh();
  // 绑定mesh缓冲
  virtual void bind();
  // 解绑mesh缓冲
  virtual void unbind();
};

#endif /* XMESH_H */

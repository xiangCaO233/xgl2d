#ifndef XMESH_H
#define XMESH_H

#include "../shader/shader.h"
#include "../texture/texture.h"
#include <cstdint>
#include <memory>

struct Vertex {
  // 顶点元数据
  glm::vec3 position{0.0f, 0.0f, 0.0f};
  glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec2 uv{0.0f, 0.0f};
  // 矩阵变换索引
  float matindex{0};
  // 顶点材质
  float texid{0.0f};
  float *dump() const {
    float *data = new float[11];
    data[0] = position.x;
    data[1] = position.y;
    data[2] = position.z;
    data[3] = color.r;
    data[4] = color.g;
    data[5] = color.b;
    data[6] = color.a;
    data[7] = uv.x;
    data[8] = uv.y;
    data[9] = matindex;
    data[10] = texid;
    return data;
  }
  // dump出变换后的顶点数据
  float *dump(glm::mat4 transform) const {
    float *data = new float[11];
    glm::vec4 position4 = glm::vec4(position, 1.0f);
    // 变换顶点
    position4 = transform * position4;
    data[0] = position4.x;
    data[1] = position4.y;
    data[2] = position4.z;
    data[3] = color.r;
    data[4] = color.g;
    data[5] = color.b;
    data[6] = color.a;
    data[7] = uv.x;
    data[8] = uv.y;
    data[9] = matindex;
    data[10] = texid;
    return data;
  }
  bool operator==(const Vertex &v) const {
    return position == v.position && color == v.color && uv == v.uv &&
           matindex == v.matindex && texid == v.texid;
  };
};

class Xmesh {
protected:
  // 缓冲对象
  // VertexBufferObject VertexArrayObject
  // ElementBufferObject TextureBuffer
  GLuint VBO, VAO, EBO, TBO;
  Shader *_shader;
  // mesh顶点数量分配
  uint32_t _vcount_size;
  // mesh顶点缓冲对象实际分配大小(VBO)(byte)
  uint64_t _vbuffer_size;
  // mesh顶点索引数量分配
  uint32_t _velecount_size;
  // mesh顶点索引缓冲对象实际分配大小(EBO)(byte)
  uint64_t _velebuffer_size;
  // 驱动最大支持纹理单元数
  int _max_texture_unit{16};

public:
  std::shared_ptr<Texture> deftexture;
  // 构造Xmesh
  Xmesh(Shader *shader, int max_texture_unit, uint32_t vcount_size = 1024);
  // 析构Xmesh
  virtual ~Xmesh();
  // 绑定mesh缓冲
  virtual void bind();
  // 解绑mesh缓冲
  virtual void unbind();
};

#endif /* XMESH_H */

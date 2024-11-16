#ifndef XMESH_H
#define XMESH_H

#include "../shader/shader.h"
#include "../texture/texture.h"
#include <cstdint>
#include <memory>

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
  // 完成绘制
  virtual void finish() = 0;
};

#endif /* XMESH_H */

#ifndef XCIRCLEMESH_H
#define XCIRCLEMESH_H

#include "../xmesh.h"

class Circle {
public:
  // 构造Circle
  Circle(float x, float y, float radius);
  // 析构Circle
  virtual ~Circle();
};

class Xcirclemesh : public Xmesh {

public:
  // 构造Xcirclemesh
  Xcirclemesh(Shader *shader);
  // 析构Xcirclemesh
  ~Xcirclemesh() override;

protected:
};
#endif /* XCIRCLEMESH_H */

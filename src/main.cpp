#include "mesh/xquadmesh.h"
#include <core/glcore.h>
#include <iostream>

int main(int argc, char *argv[]) {
  std::cout << "活了" << std::endl;
  if (!glfwInit()) {
    std::cout << "glfw初始化失败" << std::endl;
    return -1;
  }
  // 配置glfw参数
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_REFRESH_RATE, 240);
#ifdef __APPLE__
  // Apple平台前向适配
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  // 创建窗口
  GLFWwindow *w = glfwCreateWindow(960, 540, "example", nullptr, nullptr);
  if (w == NULL) {
    std::cout << "创建窗口失败" << std::endl;
    glfwTerminate();
    return -1;
  }
  // 创建opengl上下文
  glfwMakeContextCurrent(w);
  // 查询最大支持抗锯齿MSAA倍率
  GLint maxSamples;
  glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
  printf("最大抗锯齿倍率: %d\n", maxSamples);
  // 启用 最大 MSAA
  glfwWindowHint(GLFW_SAMPLES, maxSamples);

  // 初始化glew显卡opengl api(显卡驱动实现)
  if (glewInit()) {
    std::cout << "初始化GLFW失败" << std::endl;
    return -1;
  }

  int maxTextureUnits = 0;
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
  std::cout << "最大材质插槽数: " << maxTextureUnits << std::endl;

  // 创建mesh
  // XquadMesh mesh;
  // mesh.creatquad()
  return 0;
}

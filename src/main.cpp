#include "mesh/shape/xquadmesh.h"
#include "shader/shader.h"
#include <core/glcore.h>
#include <iostream>

glm::mat4 proj;
int windowWidth = 960, windowHeight = 540;
glm::vec2 screensize = {windowWidth, windowHeight};

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  //  std::cout << "大小改变" << std::endl;
  glViewport(0, 0, width, height);
  glfwGetWindowSize(window, &windowWidth, &windowHeight);
  screensize = {windowWidth, windowHeight};
  proj = glm::ortho(-(float)windowWidth / 2.0f, (float)windowWidth / 2.0f,
                    -(float)windowHeight / 2.0f, (float)windowHeight / 2.0f,
                    -1.0f, 1.0f);
  // proj = glm::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f, height
  // / 2.0f,
  //                 -1.0f, 1.0f);
  // float whrate = (float)width / (float)height;
  //   if (whrate >= 1.0f) {
  //     // proj = glm::ortho(-1.0f, 1.0f, -1.0f /
  //     whrate, 1.0f / whrate, -1.0f, 1.0f); proj =
  //     glm::ortho(-1.0f, 1.0f, -1.0f / whrate, 1.0f /
  //     whrate, -1.0f, 1.0f);
  //   } else {
  //     proj = glm::ortho(-whrate, whrate, -1.0f, 1.0f,
  //     -1.0f, 1.0f);
  //   }
}

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
  GLFWwindow *w =
      glfwCreateWindow(windowWidth, windowHeight, "example", nullptr, nullptr);
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

  // 绑定窗口大小回调函数
  glfwSetFramebufferSizeCallback(w, framebuffer_size_callback);
  // 初始化glew显卡opengl api(显卡驱动实现)
  if (glewInit()) {
    std::cout << "初始化GLFW失败" << std::endl;
    return -1;
  }

  int maxTextureUnits = 0;
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
  std::cout << "最大材质插槽数: " << maxTextureUnits << std::endl;

  // 初始化着色器
  Shader shader("../assets/shader/vertexshader.glsl",
                "../assets/shader/fragmentshader.glsl");
  shader.use();

  // 计算正交投影矩阵
  proj = glm::ortho(-(float)windowWidth / 2.0f, (float)windowWidth / 2.0f,
                    -(float)windowHeight / 2.0f, (float)windowHeight / 2.0f,
                    -1.0f, 1.0f);

  // 创建mesh
  XquadMesh mesh(&shader);
  mesh.bind();
  mesh.drawquad(0, 0, 100, 100, {0.92f, 0.46f, 0.12f, 1.0f}, screensize);
  mesh.drawquad(200, 200, 150, 100, {0.92f, 0.20f, 0.45f, 1.0f}, screensize);

  // 渲染循环
  while (!glfwWindowShouldClose(w)) {
    glClearColor(0.23f, 0.23f, 0.23f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 应用正交投影
    shader.set_unfmat4f("projmat", proj);
    // 绘制quadmesh中的所有矩形
    glDrawElements(GL_TRIANGLES, mesh.size() * 6, GL_UNSIGNED_INT, (void *)0);

    glfwSwapBuffers(w);
    glfwPollEvents();
    // 读取错误信息
    while (GLenum error = glGetError()) {
      std::cerr << "OpenGL Error: " << error << std::endl;
    }
  }
  mesh.unbind();
  shader.unuse();
  glfwTerminate();
  return 0;
}

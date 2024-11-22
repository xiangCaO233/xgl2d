#include "mesh/mesh.h"
#include "shader/shader.h"
#include "texture/texturepool.h"
#include <chrono>
#include <core/glcore.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

glm::mat4 proj;
int windowWidth, windowHeight;
glm::vec2 screensize = {windowWidth, windowHeight};
// 初始化着色器
Shader *shader;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
  glfwGetWindowSize(window, &windowWidth, &windowHeight);
  screensize = {windowWidth, windowHeight};
  proj = glm::ortho(-(float)windowWidth / 2.0f, (float)windowWidth / 2.0f,
                    -(float)windowHeight / 2.0f, (float)windowHeight / 2.0f,
                    -1.0f, 1.0f);
  // 应用正交投影
  shader->set_unfmat4f("projmat", proj);
}

int main(int argc, char *argv[]) {
  std::cout << "活了" << std::endl;
  srand(time(nullptr));
  if (!glfwInit()) {
    std::cout << "glfw初始化失败" << std::endl;
    return -1;
  }
  // 配置glfw参数
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_REFRESH_RATE, 240);
#ifdef __APPLE__
  // Apple平台前向适配
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  windowWidth = 1440, windowHeight = 820;
  // 创建窗口
  GLFWwindow *w =
      glfwCreateWindow(windowWidth, windowHeight, "example", nullptr, nullptr);
  if (w == nullptr) {
    std::cout << "创建窗口失败" << std::endl;
    glfwTerminate();
    return -1;
  }
  // 创建opengl上下文
  glfwMakeContextCurrent(w);
  const GLubyte *version = glGetString(GL_VERSION);
  std::cout << "OpenGL Version: " << version << std::endl;
  // 查询最大支持抗锯齿MSAA倍率
  GLint maxSamples;
  glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
  printf("最大抗锯齿倍率: %d\n", maxSamples);
  // 启用 最大 MSAA
  glfwWindowHint(GLFW_SAMPLES, maxSamples);
  // 禁用V-Sync
  // glfwSwapInterval(0);

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
  int maxUBOSize;
  glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUBOSize);
  std::cout << "最大UBO块容量: " << maxUBOSize << std::endl;
  shader = new Shader("../assets/shader/newvertexshader.glsl.vert",
                      "../assets/shader/newfragmentshader.glsl.frag");
  shader->use();

  // 计算正交投影矩阵
  proj = glm::ortho(-(float)windowWidth / 2.0f, (float)windowWidth / 2.0f,
                    -(float)windowHeight / 2.0f, (float)windowHeight / 2.0f,
                    -1.0f, 1.0f);

  // 加载纹理
  // std::cout << "加载纹理..." << std::endl;
  // std::vector<std::shared_ptr<Texture>> texs;
  // texs.push_back(std::make_shared<Texture>("../assets/texture/aijier.png"));
  // texs.push_back(std::make_shared<Texture>("../assets/texture/aimudeng.png"));
  // texs.push_back(
  //    std::make_shared<Texture>("../assets/texture/beierfasite.png"));
  // texs.push_back(std::make_shared<Texture>("../assets/texture/bisimai.png"));

  // texs.push_back(std::make_shared<Texture>("../assets/texture/bunao.png"));
  // texs.push_back(std::make_shared<Texture>("../assets/texture/dafeng.png"));
  // texs.push_back(std::make_shared<Texture>("../assets/texture/daiduo.png"));
  // texs.push_back(std::make_shared<Texture>("../assets/texture/dujiaoshou.png"));

  // texs.push_back(std::make_shared<Texture>("../assets/texture/guanghui.png"));
  // texs.push_back(std::make_shared<Texture>("../assets/texture/jiahe.png"));
  // texs.push_back(std::make_shared<Texture>("../assets/texture/kewei.png"));
  // texs.push_back(std::make_shared<Texture>("../assets/texture/nengdai.png"));

  // texs.push_back(
  //     std::make_shared<Texture>("../assets/texture/ougenqinwang.png"));
  // texs.push_back(std::make_shared<Texture>("../assets/texture/qiabayefu.png"));
  // texs.push_back(
  //     std::make_shared<Texture>("../assets/texture/qibolinbojue.png"));
  // texs.push_back(std::make_shared<Texture>("../assets/texture/shengli.png"));

  // texs.push_back(std::make_shared<Texture>("../assets/texture/sikula.png"));
  // texs.push_back(std::make_shared<Texture>("../assets/texture/tiancheng.png"));
  // texs.push_back(
  //     std::make_shared<Texture>("../assets/texture/tianlangxing.png"));
  // texs.push_back(std::make_shared<Texture>("../assets/texture/wuzang.png"));

  // texs.push_back(
  //     std::make_shared<Texture>("../assets/texture/xingdengbao.png"));
  // texs.push_back(std::make_shared<Texture>("../assets/texture/xinnong.png"));
  // texs.push_back(std::make_shared<Texture>("../assets/texture/xinzexi.png"));
  // texs.push_back(std::make_shared<Texture>("../assets/texture/yuanchou.png"));

  // std::cout << "加载纹理完成" << std::endl;
  //  应用正交投影
  shader->set_unfmat4f("projmat", proj);
  // glBindTexture(GL_TEXTURE_2D, mesh.deftexture->texture);

  // 初始化网格
  std::string texpath = "../assets/texture/";
  Mesh mesh(shader, texpath);
  mesh.bind();
  // 初始化纹理池
  int framecount = 1;
  // 渲染循环
  while (!glfwWindowShouldClose(w)) {
    glClearColor(0.23f, 0.23f, 0.23f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // 获取当前时间点
    // arm处理器获取时钟周期
    //__asm__ volatile("mrs %0, CNTVCT_EL0" : "=r"(tsc));
    // std::cout << std::to_string(tsc) << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    // for (int i = 0; i < 5; i++) {
    //   for (int j = 0; j < 5; j++) {
    //     float rotation = asin(sin(glfwGetTime())) / M_PI * 180.0f;
    //     auto meta = mesh.gettexmeta(5 * i + j + 1);
    //     mesh.drawquad(
    //         {-windowWidth / 2.0f + i * windowWidth / 5.0f + windowWidth
    //         / 10.0f,
    //          windowHeight / 2.0 - j * windowHeight / 5.0f -
    //              windowHeight / 10.0f},
    //         windowWidth / 5.0f, windowHeight / 5.0f, 0, {1.0, 1.0, 1.0, 1.0},
    //         meta, TexType::REAPEAT_BY_CENTER, screensize);
    //   }
    // }
    // mesh.drawquad({100, 100}, 200, 80, 45, {1.0, 1.0, 0.0, 1.0}, screensize);
    // mesh.drawquad({-200, 20}, 50, 300, -25, {0.2, 0.9, 0.5, 1.0},
    // screensize); mesh.drawquad({300, -200}, 100, 150, 70, {0.0, 1.0,
    // 0.0, 1.0}, screensize);
    float rotation = asin(sin(glfwGetTime())) / M_PI * 180.0f;
    // auto meta = mesh.gettexmeta(rand() % 25 + 1);
    auto meta = mesh.gettexmeta(10);
    mesh.drawquad({0, 0}, windowWidth, windowHeight, rotation,
                  {1.0, 0.1, 0.6, 1.0}, meta, FIT_HEIGHT_AND_REPEAT_BY_CENTER,
                  screensize);
    mesh.drawquad({400, 200}, 300, 80, rotation, {0.5, 0.5, 0.5, 1.0}, meta,
                  FIT_HEIGHT_AND_REPEAT_BY_CENTER, screensize);
    mesh.drawquad({0, -300}, 400, 200, rotation, {0.0, 1.0, 1.0, 1.0}, meta,
                  REAPEAT, screensize);
    mesh.drawquad({500, -200}, 328, 328, rotation, {1.0, 0.0, 1.0, 1.0}, meta,
                  FILL, screensize);
    mesh.drawquad({-400, 100}, 400, 146, rotation, {1.0, 1.0, 0.0, 1.0}, meta,
                  FIT_HEIGHT_AND_REPEAT, screensize);
    mesh.drawquad({-400, -150}, 520, 276, rotation, {0.0, 1.0, 0.0, 1.0}, meta,
                  REAPEAT_BY_CENTER, screensize);
    mesh.drawquad({100, 100}, 128, 345, rotation, {1.0f, 1.0f, 1.0f, 1.0f},
                  meta, FIT_WIDTH_AND_REPEAT, screensize);
    mesh.finish();
    glfwSwapBuffers(w);
    // 获取代码执行后的时间点
    auto end = std::chrono::high_resolution_clock::now();
    // 计算时间差
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "frame time:" << std::to_string(duration.count()) << "us"
              << std::endl;

    glfwPollEvents();
    // 读取错误信息
    while (GLenum error = glGetError()) {
      std::cerr << "OpenGL Error: " << error << std::endl;
    }
    // std::cout << "frame[" + std::to_string(framecount++) + "]" << std::endl;
  }
  shader->unuse();
  glfwTerminate();
  return 0;
}

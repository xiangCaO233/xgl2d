# Xgl2d (Xiang's OpenGL 2D Library)

练手项目,用学过的东西做一个能跑的 2D 图形库

## 源码文件夹 
- Shader
  - 读取”assets/shader/“目录下的 glsl 着色器源代码文件简单实现了 uniform 变量的编辑
- Texture
  - 单独实现了纹理文件的读取,类中保留可访问 gpu 的 GLuint 纹理对象(句柄？)
- Mesh

  - 基类 Xmesh 实现 mesh 的全部顶点缓冲对象管理和数组对象的描述
  - Xquadmesh 类 实现顶点元素索引缓冲对象的自动管理,
    外部调用 drawquad 在帧缓冲区绘制矩形 使用 drawlinestrip 绘制线段
  - Xovalmesh 类 椭圆的实现(实现中)
  - Xpolygonmesh 类 多边形的实现(实现中)
  - Xtextmesh 类 文本的实现(实现中)

- main.cpp 示例(
  ![涩涩不叫我.png](./sesebujiaowo.jpg)
- 当前渲染瓶颈:CPU侧
  ![2024瓶颈](20241116-10.40.jpg)
- 优化后:
  ![finish占用](finish.jpg)
- 新渲染逻辑优化:
  ![finish占用](premiere.jpg)
- 新的批绘制处理纹理由于驱动opengl版本限制
  不得不在每一帧频繁绑定缓冲区,导致帧生成时间严重不稳定
  ![frametime](frametimeissue.jpg)

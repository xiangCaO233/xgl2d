#version 410 core

// 顶点数据
layout (location = 0) in vec3 vpos;
layout (location = 1) in vec2 vuv;

// 缓存顶点数据
vec4 temppos;
vec4 tempcolor;
vec2 tempuv;
float temptexid;

// 矩形数据
// 更新标识
layout (location = 2) in float is_updated;
// 图形中心点坐标
layout (location = 3) in vec2 shape_cp;
// 图形旋转角度(角度)
layout (location = 4) in float shape_rotation;
// 图形填充颜色
layout (location = 5) in vec4 shape_color;
// 图形的纹理坐标
layout (location = 6) in vec2 shape_uv;
// 图形的纹理id(用于多纹理选择)
layout (location = 7) in vec2 shape_texid;

// 投影矩阵,变换坐标用的
uniform mat4 projection;

// 缓存的模型矩阵,通过标识判断是否需要更新
mat4 model;

void main(){

}

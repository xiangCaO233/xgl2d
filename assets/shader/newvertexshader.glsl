#version 410 core

// 顶点数据
layout (location = 0) in vec3 vpos;
layout (location = 1) in vec2 vuv;

// 矩形数据
// 图形中心点坐标
layout (location = 2) in vec2 shape_cp;
// 图形的尺寸
layout (location = 3) in vec2 shape_size;
// 图形旋转角度(角度)
layout (location = 4) in float shape_rotation;
// 图形填充颜色
layout (location = 5) in vec4 shape_color;
// 图形的纹理id(用于多纹理选择)
layout (location = 6) in float shape_texid;
// 图形的纹理变换
layout (location = 7) in vec3 shape_uvtransforml1;
layout (location = 8) in vec3 shape_uvtransforml2;
layout (location = 9) in vec3 shape_uvtransforml3;

// 视图矩阵
uniform mat4 viewmat;
// 投影矩阵
uniform mat4 projmat;

const float PI = atan(1.0) * 4;

out vec4 vertex_color;
out vec2 texcoord;
out float texid;

void main(){
    // 缩放矩形到指定大小
    vec2 scaled_pos = vpos.xy * shape_size * 0.5;
    // 旋转矩形
    float angle_rad = radians(shape_rotation);
    float cos_angle = cos(angle_rad);
    float sin_angle = sin(angle_rad);
    mat2 rotation_matrix = mat2(cos_angle, -sin_angle, sin_angle, cos_angle);
    vec2 rotated_pos = rotation_matrix * scaled_pos;

    // 平移到指定中心点
    vec2 final_pos = rotated_pos + shape_cp;
    // 应用视图和投影矩阵
    gl_Position = projmat * vec4(final_pos, 0.0, 1.0);
    // 传递颜色、UV和纹理ID到片段着色器
    vertex_color = shape_color;
		//mat3 shape_uvtransform  = mat3(
		//		shape_uvtransforml1,
		//		shape_uvtransforml2,
		//		shape_uvtransforml3);
		//texcoord = (shape_uvtransform * vec3(vuv, 1.0)).xy;
		texcoord = vuv;
		texid = shape_texid;
}

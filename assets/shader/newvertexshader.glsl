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
// 图形的纹理坐标
layout (location = 6) in vec2 shape_uv;
// 图形的纹理id(用于多纹理选择)
layout (location = 7) in float shape_texid;

// 视图矩阵
uniform mat4 viewmat;
// 投影矩阵
uniform mat4 projmat;

const float PI = atan(1.0) * 4;

out vec4 vertex_color;

void main(){
	// 计算模型矩阵
	mat4 temp_model_mat;
	// 尺寸缩放
	// 缩放矩阵
	mat4 scale_mat = mat4(
		shape_size.x,			0.0,				0.0, 0.0,
			0.0,				shape_size.y, 	0.0, 0.0,
			0.0,						0.0,				1.0, 0.0,
			0.0,						0.0,				0.0, 1.0
	);
	// 移动位置(与平移矩阵相乘)
	// 平移矩阵
	//mat4 translate_mat = mat4(
	//	1.0, 0.0, 0.0, shape_cp.x,
	//	0.0, 1.0, 0.0, shape_cp.y,
	//	0.0, 0.0, 1.0, 0.0,
	//	0.0, 0.0, 0.0, 1.0
	//);
	// 旋转矩阵（绕Z轴）
	// 计算弧度值
	float angle = shape_rotation / 180.0 * PI;
	// 分别计算正弦余弦值
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);
	// 定义旋转矩阵
    mat4 rotation_mat = mat4(
        cosTheta, -sinTheta, 0.0, 0.0,
        sinTheta,  cosTheta, 0.0, 0.0,
        0.0,       0.0,      1.0, 0.0,
        0.0,       0.0,      0.0, 1.0
    );
	// 按照顺序应用矩阵变换(不可逆)
	temp_model_mat = rotation_mat * scale_mat;
	// 输出顶点位置到之后的渲染管线
	vec4 vertex_pos = temp_model_mat * vec4(vpos, 1.0);
	vertex_pos = vertex_pos + vec4(shape_cp, 1.0, 1.0);
	// 应用投影
	gl_Position = projmat * vertex_pos;
	vertex_color = shape_color;

}

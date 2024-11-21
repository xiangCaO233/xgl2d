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
layout (location = 6) in float shape_texmetaid;
// 图形的纹理变换
layout (location = 7) in float shape_uvarg;

// 视图矩阵
uniform mat4 viewmat;
// 投影矩阵
uniform mat4 projmat;

const float PI = atan(1.0) * 4;

struct TextureMeta{
	// 纹理元数据
	float woffset;
	float hoffset;
	float width;
	float height;
};

// 定义 UTBO，大小固定为最大支持纹理数 65536bytes/sizeof(TextureMeta)[16bytes]
layout(std140) uniform TextureMetaBuffer {
	float atlas_width;
	float atlas_height;
  TextureMeta textureMetas[4095]; 
};

// 直接填充纹理[0]
const int FILL = 0;
// 直接重复纹理(默认的左下角起点)[1]
const int REAPEAT = 1;
// 直接重复纹理(以中心点扩散)[2]
const int REAPEAT_BY_CENTER = 2;
// 直接适应宽度并纵向重复(同样默认左下角)[3]
const int FIT_WIDTH_AND_REPEAT = 3;
// 直接适应高度并横向重复(同样默认左下角)[4]
const int FIT_HEIGHT_AND_REPEAT = 4;
// 直接适应宽度并纵向重复(以中心点扩散)[5]
const int FIT_WIDTH_AND_REPEAT_BY_CENTER = 5;
// 直接适应高度并横向重复(以中心点扩散)[6]
const int FIT_HEIGHT_AND_REPEAT_BY_CENTER = 6;

out vec4 vertex_color;
out vec2 texcoord;
out float debug_var;

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
    // UV
		// 元数据id,定义了当前图元绑定纹理在纹理集中的宽高偏移和大小
		int metaid = int(shape_texmetaid + 0.5);
		TextureMeta meta = textureMetas[metaid];
		
		float x = meta.woffset;
		float y = meta.hoffset;
		float w = meta.width;
		float h = meta.height;

    vertex_color = shape_color;

		// 计算归一化的纹理坐标
		// texcoord.x 范围应在 [0, w]
		float u = (x ) / atlas_width;  
		// texcoord.y 范围应在 [0, h]
		float v = (y ) / atlas_height; 

		vec2 uv = vec2(u,v);

		texcoord = vuv * uv;

		//texcoord = (shape_uvtransform * vec3(vuv, 1.0)).xy;
		debug_var = textureMetas[metaid].width;
}

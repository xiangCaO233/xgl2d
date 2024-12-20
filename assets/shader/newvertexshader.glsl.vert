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

// 纹理元数据
struct TextureMeta{ 
    float woffset;
    float hoffset;
    float width;
    float height; };

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
// 所使用的纹理在纹理集中的位置uv
out vec2 texcoord;
out vec2 shapesize;
out float texuvarg;
out float is_textured;
out float texnormalizedx;
out float texnormalizedy;
out float texnormalizedw;
out float texnormalizedh;
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
    shapesize = shape_size;
	// 应用视图和投影矩阵
    gl_Position = projmat * vec4(final_pos, 0.0, 1.0);
    vertex_color = shape_color;
    debug_var = shape_uvarg;
	// 元数据id,定义了当前图元绑定纹理在纹理集中的宽高偏移和大小
    int metaid = int(shape_texmetaid + 0.5);
    is_textured = metaid;
    if (metaid == 0){ return; } else { texuvarg = shape_uvarg;
        TextureMeta meta = textureMetas[metaid];

        // 纹理具体属性元数据
        float x = meta.woffset;
        float y = meta.hoffset;
        float w = meta.width;
        float h = meta.height;
		// 归一化元数据
        float normalizedx = x / atlas_width;
        float normalizedy = y / atlas_width;
        float normalizedw = w / atlas_width;
        float normalizedh = h / atlas_width;
		// 传递给片段着色器
        texnormalizedx = normalizedx;
        texnormalizedy = normalizedy;
        texnormalizedw = normalizedw;
        texnormalizedh = normalizedh;

        // 计算归一化的纹理坐标
        float u = vuv.x * normalizedw + normalizedx;
        float v = vuv.y * normalizedh + normalizedy;
		// 当前顶点对应纹理无视宽高在纹理集中一一对应位置(直接使用就是填充到整个矩形)
        vec2 uv = vec2(u, v);
		// 当前顶点对应纹理在纹理集中的相对尺寸
        vec2 tuv = vec2(normalizedw, normalizedh);

        // 填充方式参数
        int uvarg = int(shape_uvarg + 0.5);
		// 选择填充方式
        switch (uvarg){ 
			case FILL:{ 
			// 直接使用归一化uv
            texcoord = uv;
            break; 
			}
            case REAPEAT:{ 
				// 传递超出边界的uv
                float xratio = shape_size.x / meta.width;
                float yratio = shape_size.y / meta.height;
                float ru = (x + vuv.x * xratio * w) / atlas_width;
                float rv = (y + vuv.y * yratio * h) / atlas_height;
                texcoord  = vec2(ru, rv);
                break;
			}
            case REAPEAT_BY_CENTER:{ 
				// 计算当前顶点矩形的中心点偏移
                float centerX = x + w * 0.5;
                float centerY = y + h * 0.5;
				// 计算相对中心点的归一化坐标
                float relativeX = (vuv.x - 0.5) * (shape_size.x / meta.width);
                float relativeY = (vuv.y - 0.5) * (shape_size.y / meta.height);// 将相对坐标映射回实际纹理集坐标
                float cx = centerX + relativeX * w;
                float cy = centerY + relativeY * h;// 归一化回纹理集的 UV 坐标
                float u = cx / atlas_width;
                float v = cy / atlas_height;
                texcoord = vec2(u, v);
                break; 
			}
            case FIT_WIDTH_AND_REPEAT:{ // 适应宽并在纵向repeat
                float xratio = shape_size.x / meta.width;
                float transformedtexheight = meta.height * xratio;
                float yratio = shape_size.y / transformedtexheight;
                float ru = uv.x;
                float rv = (y + vuv.y * yratio * h) / atlas_height;
                texcoord = vec2(ru, rv);
                break; 
			}
            case FIT_WIDTH_AND_REPEAT_BY_CENTER:{ // 适应宽并相对中心向外重复采样// 计算适应宽度的缩放比例
                float xratio = shape_size.x / meta.width;
                float transformedTexHeight = meta.height * xratio;// 计算相对中心点的偏移量
                float relativeCenterX = (vuv.x - 0.5) * shape_size.x;
                float relativeCenterY = (vuv.y - 0.5) * shape_size.y / transformedTexHeight;// 计算纹理在纹理集中的实际坐标// 计算新的X坐标，考虑宽度适配
                float cx = x + 0.5 * w + relativeCenterX;// 计算新的Y坐标，考虑高度适配
                float cy = y + 0.5 * h + relativeCenterY * h;// 归一化回纹理集的 UV 坐标
                float u = uv.x;
                float v = cy / atlas_height;
                texcoord = vec2(u, v);
                break;
			}
            case FIT_HEIGHT_AND_REPEAT:{ // 适应高并在横向repeat
                float yratio = shape_size.y / meta.height;
                float transformedtexwidth = meta.width * yratio;
                float xratio = shape_size.x / transformedtexwidth;
                float ru = (x + vuv.x * xratio * w) / atlas_width;
                float rv = uv.y;
                texcoord = vec2(ru, rv);
                break;
			}
            case FIT_HEIGHT_AND_REPEAT_BY_CENTER:{ // 适应高并相对中心向外重复采样// 计算适应宽度的缩放比例
                float yratio = shape_size.y / meta.height;
                float transformedTexWidth = meta.width * yratio;// 计算相对中心点的偏移量
                float relativeCenterX = (vuv.x - 0.5) * shape_size.x / transformedTexWidth;
                float relativeCenterY = (vuv.y - 0.5) * shape_size.y;// 计算纹理在纹理集中的实际坐标// 计算新的X坐标，考虑高度适配
                float cx = x + 0.5 * w + relativeCenterX * w;// 计算新的Y坐标，考虑宽度适配
                float cy = y + 0.5 * h + relativeCenterY;// 归一化回纹理集的 UV 坐标
                float u = cx / atlas_width;
                float v = uv.y;
                texcoord = vec2(u, v);
                break; 
			}
        }
    }
}

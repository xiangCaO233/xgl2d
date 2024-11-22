#version 410 core

// 接收光栅化结果
in vec4 vertex_color;
in vec2 texcoord;
in float is_textured;
in float texuvarg;
in float texnormalizedx;
in float texnormalizedy;
in float texnormalizedw;
in float texnormalizedh;

//debug
in float debug_var;

// 纹理采样器
uniform sampler2D sampler;

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

// 渲染颜色结果(向后传输着色结果)
out vec4 FragColor;

void main() {
	// 是否使用纹理 
	int fistextured = int(is_textured + 0.5);
	if(is_textured == 0){
		FragColor = vertex_color;
	}else{
		// 定义采样纹理颜色
		vec4 textureColor;
		// 选择填充方式
		// 使用纹理采样器
		int uvarg = int(texuvarg + 0.5);
		switch(uvarg){
			case FILL:{
				textureColor = texture(sampler, texcoord);
				break;
			}
			case REAPEAT:{
				// 归一化 UV：得到相对于当前指定纹理的归一化坐标 repeatUV
  			vec2 repeatUV = fract((texcoord - vec2(texnormalizedx, texnormalizedy)) / vec2(texnormalizedw, texnormalizedh));
				// 转化为纹理集中的实际位置 finalUV
				vec2 finalUV;
				finalUV.x = texnormalizedx + repeatUV.x * texnormalizedw;
				finalUV.y = texnormalizedy + repeatUV.y * texnormalizedh;
				textureColor = texture(sampler, finalUV);
				break;
			}
		}

		// 混合采样器颜色与顶点颜色
		// FragColor = textureColor;
		// FragColor = vec4(texcoord.y , 0.0, 0.0, 1.0);
		FragColor = textureColor * vertex_color;
		// FragColor = vertex_color;
	}
}

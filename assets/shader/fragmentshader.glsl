#version 400 core

// 接收光栅化结果
in vec4 ocolor;
in vec2 ouv;
in float otexid;

// 材质采样器(数量根据显卡材质插槽填入最大值-1)
uniform sampler2D samplers[16];
// uniform samplerBuffer samplers;

// 渲染颜色结果(向后传输着色结果)
out vec4 FragColor;

void main(){

	//FragColor = texture(samplers[int(floor(texid + 0.5)),ouv]) * ocolor;
	// 获取纹理颜色
	int txindex = int(otexid);
	// 如果 txindex 为 0，使用 samplers[0]，否则根据范围映射到 samplers[1] 到 samplers[15]
  int samplerIndex = (txindex == 0) ? 0 : ((txindex - 1) % 15) + 1;
	//int tid = int(floor(texid + 0.5f));
  vec4 textureColor = texture(samplers[samplerIndex], ouv);
	// 混合采样器颜色与顶点颜色
	FragColor = textureColor * ocolor;
}

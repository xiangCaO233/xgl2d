#version 410 core

// 接收光栅化结果
in vec4 vertex_color;
in vec2 texcoord;
in float texid;

//debug
in float is_update;

// 材质采样器(数量根据显卡材质插槽填入最大值-1)
uniform sampler2D samplers[16];

// 渲染颜色结果(向后传输着色结果)
out vec4 FragColor;

void main() {
	// 获取纹理颜色
	int txindex = int(texid);
	// 如果 txindex 为 0，使用 samplers[0]，否则根据范围映射到 samplers[1] 到 samplers[15]
	// 使用纹理采样器
  int samplerIndex = (txindex == 0) ? 0 : ((txindex - 1) % 15) + 1;
  vec4 textureColor = texture(samplers[samplerIndex], texcoord);
	// 混合采样器颜色与顶点颜色
	// FragColor = textureColor;
	// FragColor = vec4(texid / 24.0);
	FragColor = textureColor * vertex_color;
	// FragColor = vec4(texcoord.y);
}

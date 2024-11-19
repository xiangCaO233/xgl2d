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
	FragColor = vertex_color;
	// FragColor = vec4(is_update/300.0,0.0,1.0,1.0);
}

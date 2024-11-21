#version 410 core

// 接收光栅化结果
in vec4 vertex_color;
in vec2 texcoord;

//debug
in float debug_var;

// 纹理采样器
uniform sampler2D sampler;

// 渲染颜色结果(向后传输着色结果)
out vec4 FragColor;

void main() {
	// 使用纹理采样器
  vec4 textureColor = texture(sampler, texcoord);

	// 混合采样器颜色与顶点颜色
	// FragColor = textureColor;
	// FragColor = vec4(debug_var /1500);
	FragColor = textureColor * vertex_color;
	// FragColor = vertex_color;
}

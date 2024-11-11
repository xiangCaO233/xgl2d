#version 330 core

// 接收光栅化结果
in vec4 ocolor;
in vec2 ouv;
in float texid;

// 材质采样器(数量根据显卡材质插槽填入最大值-1)
uniform sampler2D samplers[15];

// 渲染颜色结果(向后传输着色结果)
out vec4 FragColor;

void main(){
	FragColor = texture(samplers[int(floor(texid + 0.5)),ouv]) * ocolor;
}

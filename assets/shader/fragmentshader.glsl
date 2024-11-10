#version 330

// 接收光栅化结果
in vec4 ocolor;
in vec2 ouv;
in float texid;

// 材质采样器
uniform sampler2DArray samplers;

// 渲染颜色结果(向后传输着色结果)
out vec4 FragColor;

void main(){
	vec4 samplecolor = texture(samplers,vec3(ouv,int(texid)));
	FragColor = samplecolor * ocolor;
}

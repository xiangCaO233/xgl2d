#version 330 core

// 顶点位置
layout (location = 0) in vec3 mposition;
// 顶点颜色
layout (location = 1) in vec4 mcolor;
// 顶点材质uv
layout (location = 2) in vec2 muv;
// 顶点矩阵id(暂无作用)
layout (location = 3) in float matindex;
// 顶点材质id
layout (location = 4) in float texid;

// 视图矩阵
uniform mat4 viewmat;
// 投影矩阵
uniform mat4 projmat;

// 传输到下一渲染管线
out vec4 ocolor;
out vec2 ouv;
out float otexid;

void main(){
	// 应用所有矩阵
	// gl_Position = viewmat * projmat * mmodmat * vec4(mposition,1.0f);
	int transformindex = int(floor((matindex + 0.5)));
	gl_Position = projmat * vec4(mposition,1.0f);
	ocolor = mcolor;
	ouv = muv;
	otexid = texid;
}

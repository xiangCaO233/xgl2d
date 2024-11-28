#version 410 core

// 接收光栅化结果
in vec4 vertex_color;
in vec2 texcoord;
in vec2 shapesize;
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
const int FILL = 0;// 直接重复纹理(默认的左下角起点)[1]
const int REAPEAT = 1;// 直接重复纹理(以中心点扩散)[2]
const int REAPEAT_BY_CENTER = 2;// 直接适应宽度并纵向重复(同样默认左下角)[3]
const int FIT_WIDTH_AND_REPEAT = 3;// 直接适应高度并横向重复(同样默认左下角)[4]
const int FIT_HEIGHT_AND_REPEAT = 4;// 直接适应宽度并纵向重复(以中心点扩散)[5]
const int FIT_WIDTH_AND_REPEAT_BY_CENTER = 5;// 直接适应高度并横向重复(以中心点扩散)[6]
const int FIT_HEIGHT_AND_REPEAT_BY_CENTER = 6;

// 渲染颜色结果(向后传输着色结果)
out vec4 FragColor;

void main() { // 是否使用纹理
    int fistextured = int(is_textured + 0.5);
    if (is_textured == 0){ FragColor = vertex_color; } else { // 定义采样纹理颜色
        vec4 textureColor;// 选择填充方式// 使用纹理采样器
        int uvarg = int(texuvarg + 0.5);
        switch (uvarg){ case FILL:{ textureColor = texture(sampler, texcoord);
            break; }
            case REAPEAT:{ // 归一化 UV：得到相对于当前指定纹理的归一化坐标 repeatUV
                vec2 repeatUV = fract((texcoord - vec2(texnormalizedx, texnormalizedy)) / vec2(texnormalizedw, texnormalizedh));// 转化为纹理集中的实际位置 finalUV
                vec2 finalUV;
                finalUV.x = texnormalizedx + repeatUV.x * texnormalizedw;
                finalUV.y = texnormalizedy + repeatUV.y * texnormalizedh;
                textureColor = texture(sampler, finalUV);
                break; }
            case REAPEAT_BY_CENTER:{ // 当前纹理的中心在纹理集中的归一化坐标
                vec2 textureCenter = vec2(texnormalizedx + 0.5 * texnormalizedw, texnormalizedy + 0.5 * texnormalizedh);// 计算归一化 UV，相对于纹理中心进行对称调整
                vec2 centeredUV = (texcoord - textureCenter) / vec2(texnormalizedw, texnormalizedh);// 以中心为基准对 UV 进行 repeat// 保持对称，UV范围 [-0.5, 0.5]
                vec2 repeatUV = fract(centeredUV + 0.5) - 0.5;// 转回到纹理集中实际的UV
                vec2 finalUV;
                finalUV.x = textureCenter.x + repeatUV.x * texnormalizedw;
                finalUV.y = textureCenter.y + repeatUV.y * texnormalizedh;// 使用最终 UV 进行采样
                textureColor = texture(sampler, finalUV);
                break; }
            case FIT_WIDTH_AND_REPEAT:{ // 适应宽并在纵向repeat// 归一化 UV：得到相对于当前指定纹理的归一化坐标 repeatUV
                vec2 repeatUV = fract((texcoord - vec2(texnormalizedx, texnormalizedy)) / vec2(texnormalizedw, texnormalizedh));// 转化为纹理集中的实际位置 finalUV
                vec2 finalUV;
                finalUV.x = texcoord.x;
                finalUV.y = texnormalizedy + repeatUV.y * texnormalizedh;
                textureColor = texture(sampler, finalUV);
                break; }
            case FIT_WIDTH_AND_REPEAT_BY_CENTER:{ // 适应宽并相对中心向外重复采样// 当前纹理的中心在纹理集中的归一化坐标
                vec2 textureCenter = vec2(texnormalizedx + 0.5 * texnormalizedw, texnormalizedy + 0.5 * texnormalizedh);// 计算归一化 UV，相对于纹理中心进行对称调整
                vec2 centeredUV = (texcoord - textureCenter) / vec2(texnormalizedw, texnormalizedh);// 以中心为基准对 UV 进行 repeat// 保持对称，UV范围 [-0.5, 0.5]
                vec2 repeatUV = fract(centeredUV + 0.5) - 0.5;// 转回到纹理集中实际的UV
                vec2 finalUV;
                finalUV.x = textureCenter.x + repeatUV.x * texnormalizedw;
                finalUV.y = textureCenter.y + repeatUV.y * texnormalizedh;// 使用最终 UV 进行采样
                textureColor = texture(sampler, finalUV);
                break; }
            case FIT_HEIGHT_AND_REPEAT:{ // 适应宽并在纵向repeat// 归一化 UV：得到相对于当前指定纹理的归一化坐标 repeatUV
                vec2 repeatUV = fract((texcoord - vec2(texnormalizedx, texnormalizedy)) / vec2(texnormalizedw, texnormalizedh));// 转化为纹理集中的实际位置 finalUV
                vec2 finalUV;
                finalUV.x = texnormalizedx + repeatUV.x * texnormalizedw;
                finalUV.y = texcoord.y;
                textureColor = texture(sampler, finalUV);
                break; }
            case FIT_HEIGHT_AND_REPEAT_BY_CENTER:{ // 适应高并相对中心向外重复采样// 当前纹理的中心在纹理集中的归一化坐标
                vec2 textureCenter = vec2(texnormalizedx + 0.5 * texnormalizedw, texnormalizedy + 0.5 * texnormalizedh);// 计算归一化 UV，相对于纹理中心进行对称调整
                vec2 centeredUV = (texcoord - textureCenter) / vec2(texnormalizedw, texnormalizedh);// 以中心为基准对 UV 进行 repeat// 保持对称，UV范围 [-0.5, 0.5]
                vec2 repeatUV = fract(centeredUV + 0.5) - 0.5;// 转回到纹理集中实际的UV
                vec2 finalUV;
                finalUV.x = textureCenter.x + repeatUV.x * texnormalizedw;
                finalUV.y = textureCenter.y + repeatUV.y * texnormalizedh;// 使用最终 UV 进行采样
                textureColor = texture(sampler, finalUV); } }

        // 混合采样器颜色与顶点颜色
        FragColor = textureColor * vertex_color;// FragColor = textureColor;// FragColor = vec4(texcoord.y , 0.0, 0.0, 1.0);// FragColor = vec4(is_textured);
    }
}

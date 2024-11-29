#ifndef MESHBASE
#define MESHBASE

#include <cstdint>
#include <memory>
#include <vector>

#include "../../include/core/glcore.h"
#include "quad.h"
#include "shader/shader.h"
#include "texture/texturepool.h"

enum class Datatype {
    // 位置数据
    POSITION,  // 尺寸数据
    SIZE,      // 旋转数据
    ROTATION,  // 颜色数据
    COLOR,     // 纹理元数据
    TEXMETA,   // 贴图参数
    UVARG
};

class Mesh {
    // 缓冲对象
    // VBO:顶点数据缓冲对象
    // VAO:顶点数组缓冲对象
    // FBO:帧缓冲对象
    // (
    // instanceVBO[0]:x,y中心坐标,
    // instanceVBO[1]:尺寸
    // instanceVBO[2]:旋转角度,
    // instanceVBO[3]:r,g,b,a颜色,
    // instanceVBO[4]:texid贴图元数据id
    // instanceVBO[5]:u,v变换使用参数(自动计算矩阵)
    // )
    GLuint VBO{0}, VAO{0}, instanceVBO{0}, FBO{0};
    // 最大矩形数量
    int max_quad_count;
    // 椭圆采样点数
    int _oval_segment;
    // 绑定的着色器
    Shader *_shader;
    // 纹理池
    Texturepool *_texpool;
    // 包含的图形
    std::vector<std::vector<std::shared_ptr<Quad>>> _consequent_shapes;
    std::vector<std::shared_ptr<Quad>> _shapes;
    // 每一段要更新的连续的矩形和数据
    std::vector<std::vector<std::shared_ptr<Quad>>> _update_consequent_shapes_poss;
    std::vector<std::vector<float>> _update_consequent_shapes_pos_datas;
    std::vector<std::vector<std::shared_ptr<Quad>>> _update_consequent_shapes_sizes;
    std::vector<std::vector<float>> _update_consequent_shapes_size_datas;
    std::vector<std::vector<std::shared_ptr<Quad>>> _update_consequent_shapes_rotations;
    std::vector<std::vector<float>> _update_consequent_shapes_rotation_datas;
    std::vector<std::vector<std::shared_ptr<Quad>>> _update_consequent_shapes_colors;
    std::vector<std::vector<float>> _update_consequent_shapes_color_datas;
    std::vector<std::vector<std::shared_ptr<Quad>>> _update_consequent_shapes_texmetas;
    std::vector<std::vector<float>> _update_consequent_shapes_texmeta_datas;
    std::vector<std::vector<std::shared_ptr<Quad>>> _update_consequent_shapes_uvargs;
    std::vector<std::vector<float>> _update_consequent_shapes_uvarg_datas;
    // 当前正在处理的矩形下标
    uint32_t _current_handle_index{0};

    void updateVAOpointer(uintptr_t shapecount) const;

public:
    // 构造Mesh
    explicit Mesh(Shader *shader, std::string &texdir, int oval_segment = 32, int initial_shape_count = 4096);

    // 析构Mesh
    virtual ~Mesh();

    std::shared_ptr<TextureMeta> gettexmeta(const char *name) {
        return (*_texpool)[name];
    }

    std::shared_ptr<TextureMeta> gettexmeta(int index) {
        return (*_texpool)[index];
    }

    // 绑定mesh缓冲
    void bind() const;

    // 解绑mesh缓冲
    static void unbind();

    // 绘制矩形(材质默认混合颜色并填充到矩形)
    void
    drawquad(glm::vec2 &cp, float w, float h, float rotation, glm::vec4 &color, std::shared_ptr<TextureMeta> &texture,
             TexType texture_type, glm::vec2 &screen_size, ShapeType shapetype = QUAD);

    void
    drawquad(glm::vec2 &&cp, float w, float h, float rotation, glm::vec4 &color, std::shared_ptr<TextureMeta> &texture,
             TexType texture_type, glm::vec2 &screen_size);

    void
    drawquad(glm::vec2 &cp, float w, float h, float rotation, glm::vec4 &&color, std::shared_ptr<TextureMeta> &texture,
             TexType texture_type, glm::vec2 &screen_size);

    void
    drawquad(glm::vec2 &&cp, float w, float h, float rotation, glm::vec4 &&color, std::shared_ptr<TextureMeta> &texture,
             TexType texture_type, glm::vec2 &screen_size);

    // 绘制矩形(仅填充颜色)
    void drawquad(glm::vec2 &cp, float w, float h, float rotation, glm::vec4 &color, glm::vec2 &screen_size);

    void drawquad(glm::vec2 &&cp, float w, float h, float rotation, glm::vec4 &color, glm::vec2 &screen_size);

    void drawquad(glm::vec2 &cp, float w, float h, float rotation, glm::vec4 &&color, glm::vec2 &screen_size);

    void drawquad(glm::vec2 &&cp, float w, float h, float rotation, glm::vec4 &&color, glm::vec2 &screen_size);

    // 绘制矩形(仅填充材质)
    void drawquad(glm::vec2 &cp, float w, float h, float rotation, std::shared_ptr<TextureMeta> &texture,
                  TexType texture_type, glm::vec2 &screen_size);

    void drawquad(glm::vec2 &&cp, float w, float h, float rotation, std::shared_ptr<TextureMeta> &texture,
                  TexType texture_type, glm::vec2 &screen_size);

    // 更新连续区
    void update_consecutive(std::vector<std::vector<std::shared_ptr<Quad>>> &update_consequent_list,
                            std::vector<std::shared_ptr<Quad>> *current_consequent_list,
                            std::vector<std::vector<float>> &update_consequent_data_list,
                            std::vector<float> *current_consequent_data, const std::shared_ptr<Quad> &handle_quad,
                            Datatype type) const;

    // 绘制椭圆,混合材质与颜色
    void
    drawoval(glm::vec2 &cp, float rx, float ry, float rotation, glm::vec4 &color, std::shared_ptr<TextureMeta> &texture,
             TexType texture_type, glm::vec2 &screen_size);

    void drawoval(glm::vec2 &&cp, float rx, float ry, float rotation, glm::vec4 &color,
                  std::shared_ptr<TextureMeta> &texture, TexType texture_type, glm::vec2 &screen_size);

    void drawoval(glm::vec2 &cp, float rx, float ry, float rotation, glm::vec4 &&color,
                  std::shared_ptr<TextureMeta> &texture, TexType texture_type, glm::vec2 &screen_size);

    void drawoval(glm::vec2 &&cp, float rx, float ry, float rotation, glm::vec4 &&color,
                  std::shared_ptr<TextureMeta> &texture, TexType texture_type, glm::vec2 &screen_size);

    // 绘制椭圆(仅填充颜色)
    void drawoval(glm::vec2 &cp, float rx, float ry, float rotation, glm::vec4 &color, glm::vec2 &screen_size);

    void drawoval(glm::vec2 &&cp, float rx, float ry, float rotation, glm::vec4 &color, glm::vec2 &screen_size);

    void drawoval(glm::vec2 &cp, float rx, float ry, float rotation, glm::vec4 &&color, glm::vec2 &screen_size);

    void drawoval(glm::vec2 &&cp, float rx, float ry, float rotation, glm::vec4 &&color, glm::vec2 &screen_size);

    // 绘制椭圆(仅填充材质)
    void drawvoal(glm::vec2 &cp, float rx, float ry, float rotation, std::shared_ptr<TextureMeta> &texture,
                  TexType texture_type, glm::vec2 &screen_size);

    void drawoval(glm::vec2 &&cp, float rx, float ry, float rotation, std::shared_ptr<TextureMeta> &texture,
                  TexType texture_type, glm::vec2 &screen_size);

    void
    drawtext(glm::vec2 &pos, const char *characters, float fontsize, STYLE fontstyle, float rotation, glm::vec4 &color,
             glm::vec2 &screensize);

    void finish();
};

#endif /* MESHBASE */

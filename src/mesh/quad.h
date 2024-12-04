#ifndef QUAD_H
#define QUAD_H

#include <memory>

#include "texture/texturepool.h"

enum ShapeType { QUAD, OVAL, TEXT };
enum STYLE { NORMAL, BOLD, ITALIC };

class Quad {
    // 基本数据
    glm::vec2 _cp;
    float _w, _h;
    float _rotation{0.0f};
    glm::vec4 _color;
    std::shared_ptr<TextureMeta> _texmeta;
    ShapeType _shapetype;
    // 纹理类型
    TexType _textype;

    // 绘制顺序
    uint32_t _draw_order{0};
    int updateindexlist{0}, updateindexelement{0};

    // 在模型缓冲区中的偏移
    uint32_t model_data_offset[6]{0, 0, 0, 0, 0, 0};

    friend class Mesh;

   public:
    // 构造Quad
    Quad(glm::vec2 &cp, float w, float h, float rotation, glm::vec4 &color,
         std::shared_ptr<TextureMeta> &tex, TexType texture_type,
         ShapeType shapetype)
        : _cp(cp),
          _w(w),
          _h(h),
          _rotation(rotation),
          _color(color),
          _texmeta(tex),
          _textype(texture_type),
          _shapetype(shapetype){};

    // 析构Quad
    virtual ~Quad() = default;

    // 重写==运算符
    inline bool operator==(const Quad &q) const {
        return _cp == q._cp && _w == q._w && _h == q._h &&
               _rotation == q._rotation && _color == q._color &&
               _texmeta == q._texmeta && _textype == q._textype &&
               _shapetype == q._shapetype;
    };

    inline bool equals(const glm::vec2 &cp, const float w, const float h,
                       const float rotation, const glm::vec4 &color,
                       const std::shared_ptr<TextureMeta> &tex,
                       const TexType texture_type, ShapeType shapetype) {
        return _cp == cp && _w == w && _h == h && _rotation == rotation &&
               _color == color && _texmeta == tex && _textype == texture_type &&
               _shapetype == shapetype;
    }

    inline bool is_cp_equal(const glm::vec2 &cp) { return _cp == cp; }

    inline bool is_size_equal(const float w, const float h) {
        return _w == w && _h == h;
    }

    inline bool is_rotation_equal(const float rotation) {
        return _rotation == rotation;
    }

    inline bool is_color_equal(const glm::vec4 &color) {
        return _color == color;
    }

    inline bool is_tex_equal(const std::shared_ptr<TextureMeta> &meta) {
        return _texmeta == meta;
    }

    inline bool is_textype_equal(const TexType &texture_type) {
        return _textype == texture_type;
    }

    inline bool is_shape_equal(ShapeType shapetype) {
        return _shapetype == shapetype;
    }
};

#endif /* QUAD_H */

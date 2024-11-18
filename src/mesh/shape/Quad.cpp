#include "Quad.h"
#include "Shape.h"

XQuad::XQuad(glm::vec2 &cp, float width, float height)
    : Shape(4), width(width), height(height) {
  _vertices[0].position = {-width / 2, -height / 2, 0.0f};
  _vertices[1].position = {width / 2, -height / 2, 0.0f};
  _vertices[2].position = {width / 2, height / 2, 0.0f};
  _vertices[3].position = {-width / 2, height / 2, 0.0f};
  translate({cp.x, cp.y, 0.0f});
}
XQuad::XQuad(glm::vec2 &&cp, float width, float height)
    : XQuad(cp, width, height){};
XQuad::XQuad(glm::vec2 &cp, float width, float height, glm::vec4 &color)
    : XQuad(cp, width, height) {
  _vertices[0].color = color;
  _vertices[1].color = color;
  _vertices[2].color = color;
  _vertices[3].color = color;
};
XQuad::XQuad(glm::vec2 &cp, float width, float height,
             const std::shared_ptr<Texture> &texture, TexType texture_type)
    : XQuad(cp, width, height) {
  tex = texture;
  switch (texture_type) {
  case TexType::FILL: {
    // 直接填充整个矩形(尺寸不一则变形)
    _vertices[0].uv = {0.0f, 0.0f};
    _vertices[1].uv = {1.0f, 0.0f};
    _vertices[2].uv = {1.0f, 1.0f};
    _vertices[3].uv = {0.0f, 1.0f};
    break;
  }
  case TexType::REAPEAT: {
    // 重复绘制材质到矩形(根据quad尺寸自动重复绘制或直接裁剪(左下角为起点))
    _vertices[0].uv = {0.0f, 0.0f};
    _vertices[1].uv = {width / texture->width, 0.0f};
    _vertices[2].uv = {width / texture->width, height / texture->height};
    _vertices[3].uv = {0.0f, height / texture->height};
    break;
  }
  case TexType::REAPEAT_BY_CENTER: {
    // 重复绘制矩形(中心扩散)
    break;
  }
  case TexType::FIT_WIDTH_AND_REPEAT: {
    // 适应宽度并重复绘制
    break;
  }
  case TexType::FIT_HEIGHT_AND_REPEAT: {
    // 适应高度并重复绘制
    break;
  }
  case TexType::FIT_WIDTH_AND_REPEAT_BY_CENTER: {
    // 适应宽度中心扩散重复绘制
    break;
  }
  case TexType::FIT_HEIGHT_AND_REPEAT_BY_CENTER: {
    // 适应高度中心扩散重复绘制
    break;
  }
  }
  _vertices[0].texid = texture->texid;
  _vertices[1].texid = texture->texid;
  _vertices[2].texid = texture->texid;
  _vertices[3].texid = texture->texid;
};

XQuad::XQuad(glm::vec2 &cp, float width, float height, glm::vec4 &color,
             std::shared_ptr<Texture> texture, TexType texture_type)
    : XQuad(cp, width, height, texture, texture_type) {
  _vertices[0].color = color;
  _vertices[1].color = color;
  _vertices[2].color = color;
  _vertices[3].color = color;
};
XQuad::~XQuad() = default;

// 线段构造
Linestrip::Linestrip(glm::vec2 &p1, glm::vec2 &p2, float width)
    : linelength(sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2))),
      linewidth(width),
      XQuad({(p2.x + p1.x) / 2.0f, (p2.y - p1.y) / 2.0f},
            sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2)), width) {
  // 旋转线段
  rotate(atan((p2.y - p1.y) / (p2.x - p1.x)) * 180.0f / M_PI,
         {0.0f, 0.0f, 1.0f});
}

Linestrip::Linestrip(glm::vec2 &p1, glm::vec2 &p2, float width,
                     glm::vec4 &color)
    : Linestrip(p1, p2, width) {
  setfill(color);
};

Linestrip::Linestrip(glm::vec2 &sp, float length, float degrees, float width)
    : XQuad({sp.x + length * cos(degrees / 180.0f * M_PI),
            sp.y + length * sin(degrees / 180.0f * M_PI)},
            length, width),
      linewidth(width), linelength(length) {}

Linestrip::Linestrip(glm::vec2 &sp, float length, float degrees, float width,
                     glm::vec4 &color)
    : Linestrip(sp, length, degrees, width) {
  setfill(color);
};

Linestrip::~Linestrip() = default;

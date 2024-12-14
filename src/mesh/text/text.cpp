#include "text.h"

#include "logger/logger.h"
#include "texture/texturepool.h"

XText::XText(std::string &fontfile, Shader *s) : Texturepool(s) {
    loadtexture(fontfile);
}

XText::~XText() {}

void XText::loadtexture(std::string &fontPath) {
    // 初始化 FreeType 库
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        LOG_CRITICAL("初始化FreeType库失败");
        throw std::runtime_error("Could not init FreeType Library");
    }
    // 加载字体
    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        LOG_CRITICAL("加载字体失败");
        throw std::runtime_error("Failed to load font");
    }
};

void XText::creatatlas(){};

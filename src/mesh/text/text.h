#ifndef X_TEXT_H
#define X_TEXT_H

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include <string>

#include "../../texture/texturepool.h"

class XText : public Texturepool {
   public:
    // 构造XText
    XText(std::string &fontfile, Shader *s);
    // 析构XText
    virtual ~XText();

    virtual void loadtexture(std::string &fontPath) override;

    virtual void creatatlas() override;
};

#endif  // X_TEXT_H

#define STB_IMAGE_IMPLEMENTATION

#include "texturepool.h"

#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "MaxRectsBinPack.h"
#include "stb_image.h"

Texturepool::Texturepool(std::string &texturedir, Shader *shader)
    : _shader(shader) {
    glGenTextures(1, &texture_atlas);
    glBindTexture(GL_TEXTURE_2D, texture_atlas);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    // 重复绘制
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 缩小使用临近过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // 放大使用线性过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 反转y轴,合乎gl礼
    stbi_set_flip_vertically_on_load(true);

    // 生成默认白色纹理
    // RGBA(255, 255, 255, 255)
    static unsigned char whitePixel[4 * 9] = {
	255,
	255,
	255,
	255,
    };
    _defmeta = std::make_shared<TextureMeta>("defmeta", 1, 1, 0, 0);
    _texmetas[_defmeta->name] = _defmeta;
    _texdatas[_defmeta] = whitePixel;

    // 加载纹理文件夹
    loadtexture(texturedir);
}

Texturepool::~Texturepool() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &texture_atlas);
    glDeleteBuffers(1, &UTBO);
}

void Texturepool::bind() {
    glBindTexture(GL_TEXTURE_2D, texture_atlas);
    glActiveTexture(GL_TEXTURE0);
    _shader->set_sampler("sampler", 0);
}

void Texturepool::unbind() { glBindTexture(GL_TEXTURE_2D, 0); };

void Texturepool::loadtexture(std::string &texturedir) {
    if (!is_done) {
	if (!std::filesystem::exists(texturedir)) {
	    // 非法路径
	    LOG_CRITICAL("非法纹理文件(路径)");
	    throw std::runtime_error("illegal texture file");
	} else if (std::filesystem::is_regular_file(texturedir)) {
	    LOG_INFO("读取纹理:[" +
		     std::filesystem::absolute(texturedir).string() + "]");
	    int twidth, theight, nrChannels;
	    unsigned char *data =
		stbi_load(std::filesystem::absolute(texturedir).c_str(),
			  &twidth, &theight, &nrChannels, 0);
	    // 读取到表中
	    auto meta = std::make_shared<TextureMeta>();
	    auto filename =
		std::filesystem::path(texturedir).filename().string();
	    meta->name = filename;
	    meta->width = twidth;
	    meta->height = theight;
	    meta->woffset = 0;
	    meta->hoffset = 0;
	    meta->metaid = (float)(_texmetas.size());
	    _texmetas[filename] = meta;
	    _texdatas[meta] = data;
	    _metalist.push_back(meta);
	} else if (std::filesystem::is_directory(texturedir)) {
	    // 递归遍历目录中的所有文件和文件夹
	    for (const auto &entry :
		 std::filesystem::directory_iterator(texturedir)) {
		auto child_path = entry.path().string();
		loadtexture(child_path);
	    }
	} else {
	    LOG_WARN("文件(路径)[" +
		     std::filesystem::absolute(texturedir).string() +
		     "]不存在");
	}
    } else {
	LOG_INFO("纹理集生成已经完成,无法再次加载纹理");
    }
}

void Texturepool::creatatlas() {
    is_done = true;
    MaxRectsBinPack pack = MaxRectsBinPack();
    for (const auto &pair : _texmetas) {
	// 将图像合并
	pack.Insert(pair.second, MaxRectsBinPack::RectBottomLeftRule);
    }
    // 生成atlas图像
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pack.binWidth, pack.binHeight, 0,
		 GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    _vwidth = pack.binWidth;
    _vheight = pack.binHeight;
    LOG_INFO("当前纹理位置信息:");
    // 填充并释放纹理数据
    for (auto &pair : _texmetas) {
	std::string dbgstr = pair.first + "---res tex pos:[x:" +
			     std::to_string(pair.second->woffset) +
			     ",y:" + std::to_string(pair.second->hoffset) + "]";
	LOG_INFO(dbgstr);
	// 放置纹理到atlas
	glTexSubImage2D(GL_TEXTURE_2D, 0, pair.second->woffset,
			pair.second->hoffset, pair.second->width,
			pair.second->height, GL_RGBA, GL_UNSIGNED_BYTE,
			_texdatas[pair.second]);
    }
    // 绘制默认meta纹理
    glTexSubImage2D(GL_TEXTURE_2D, 0, _defmeta->woffset, _defmeta->hoffset,
		    _defmeta->width, _defmeta->height, GL_RGBA,
		    GL_UNSIGNED_BYTE, _texdatas[_defmeta]);
    LOG_INFO("最终纹理集大小:" + std::to_string(pack.binWidth) + "x" +
	     std::to_string(pack.binHeight) + "]");
    LOG_INFO("纹理集填充率:[" + std::to_string(pack.Occupancy()) + "]");
    LOG_DEBUG("开始初始化UTBO");

    // 生成UTBO纹理元数据
    glGenBuffers(1, &UTBO);
    glBindBuffer(GL_UNIFORM_BUFFER, UTBO);
    std::vector<float> utbodata;
    utbodata.push_back((float)_vwidth);
    utbodata.push_back((float)_vheight);
    utbodata.push_back(0);
    utbodata.push_back(0);
    utbodata.push_back((float)_defmeta->woffset);
    utbodata.push_back((float)_defmeta->hoffset);
    utbodata.push_back((float)_defmeta->width);
    utbodata.push_back((float)_defmeta->height);
    _texmetas_by_index[0] = _defmeta;
    for (auto &meta : _metalist) {
	LOG_INFO("导入meta:[" + meta->name + "],id[" +
		 std::to_string(meta->metaid) + "]");
	_texmetas_by_index[(int)meta->metaid] = meta;
	utbodata.push_back((float)meta->woffset);
	utbodata.push_back((float)meta->hoffset);
	utbodata.push_back((float)meta->width);
	utbodata.push_back((float)meta->height);
    }
    LOG_INFO("meta数:[" + std::to_string(_texmetas.size()) + "]");
    // 为 UTBO 分配数据空间
    glBufferData(GL_UNIFORM_BUFFER, (int)(utbodata.size() * sizeof(float)),
		 utbodata.data(), GL_STATIC_DRAW);

    // 将 UTBO 绑定到绑定点 0
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, UTBO);

    // 解除绑定
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    LOG_DEBUG("初始化UTBO完成");
    LOG_INFO("生成纹理集完成");
    is_done = true;
}

#include "texturepool.h"
#include "stb_image.h"
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <stdexcept>

Texturepool::Texturepool(uint32_t vwidth, uint32_t vheight)
    : _vwidth(vwidth), _vheight(vheight) {
  glGenBuffers(1, &TBO);
  glGenTextures(1, &texture_atlas);
  stbi_set_flip_vertically_on_load(true);
}
Texturepool::Texturepool(const char *texturedir) {
  glGenBuffers(1, &TBO);
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
  loadtexture(texturedir);
}
Texturepool::~Texturepool() {
  glDeleteTextures(1, &texture_atlas);
  glDeleteBuffers(1, &TBO);
  // 释放分配的meta
  for (auto &meta : _texmetas) {
    delete meta.second;
  };
}
void Texturepool::bind() { glBindTexture(GL_TEXTURE_2D, TBO); };
void Texturepool::unbind() { glBindTexture(GL_TEXTURE_2D, 0); };

void Texturepool::loadtexture(const char *texturedir) {
  if (!std::filesystem::exists(texturedir)) {
    // 非法路径
    throw std::runtime_error("illegal texture file");
  } else if (std::filesystem::is_regular_file(texturedir)) {
    std::cout << "读取纹理: " << std::filesystem::absolute(texturedir);
    int twidth, theight, nrChannels;
    unsigned char *data =
        stbi_load(std::filesystem::absolute(texturedir).c_str(), &twidth,
                  &theight, &nrChannels, 0);
    // 读取到表中
    auto meta = new TextureMeta();
    meta->name = texturedir;
    meta->width = twidth;
    meta->height = theight;
    meta->woffset = 0;
    meta->hoffset = 0;
    meta->texid = _texmetas.size();
    _texmetas[texturedir] = meta;
    _texdatas[meta] = data;
  } else if (std::filesystem::is_directory(texturedir)) {
    // 遍历目录中的所有文件和文件夹
    for (const auto &entry :
         std::filesystem::recursive_directory_iterator(texturedir)) {
      if (std::filesystem::is_regular_file(entry)) {
        // 获取文件的绝对路径
        std::cout << "读取纹理: " << std::filesystem::absolute(entry.path())
                  << std::endl;
        int twidth, theight, nrChannels;
        unsigned char *data =
            stbi_load(std::filesystem::absolute(entry.path()).c_str(), &twidth,
                      &theight, &nrChannels, 0);
        // 读取到表中
        auto meta = new TextureMeta();
        meta->name = entry.path().c_str();
        meta->width = twidth;
        meta->height = theight;
        meta->woffset = 0;
        meta->hoffset = 0;
        meta->texid = _texmetas.size();
        _texmetas[entry.path().c_str()] = meta;
        _texdatas[meta] = data;
      }
    }
  } else {
    std::cout << "dir empty" << std::endl;
  }
}
void Texturepool::creatatlas() {
  is_done = true;
  for (const auto &meta : _texmetas) {
    // 将图像合并
    // 释放纹理数据
    stbi_image_free(_texdatas[meta.second]);
  }
};

// maxrectangl合并纹理图集算法
// 插入新矩形并返回其位置
bool Texturepool::insert(int width, int height, TextureMeta *result) {
  // 如果图集空间不足，扩展图集
  if (!canInsert(width, height)) {
    expandAtlas();
  }

  // 寻找最适合的空闲区域（最大空闲矩形）
  int bestScore = -1;
  int bestIndex = -1;

  for (int i = 0; i < freeDimensions.size(); ++i) {
    const Rect &r = freeDimensions[i];
    if (r.width >= width && r.height >= height) {
      // 计算合适性得分,面积差距
      int score = (r.width - width) * (r.height - height);
      if (bestScore == -1 || score < bestScore) {
        bestScore = score;
        bestIndex = i;
      }
    }
  }
  // 没有找到合适的位置
  if (bestIndex == -1) {
    return false;
  }

  // 使用找到的空闲区域插入矩形
  const Rect &r = freeDimensions[bestIndex];
  result->woffset = r.x;
  result->hoffset = r.y;

  // 更新空闲区域：拆分原始空闲区域
  freeDimensions.push_back(Rect(r.x + width, r.y, r.width - width, height));
  freeDimensions.push_back(Rect(r.x, r.y + height, width, r.height - height));

  // 删除原空闲区域
  freeDimensions.erase(freeDimensions.begin() + bestIndex);

  // 合并相邻的空闲区域（可选优化）
  mergeFreeRectangles();
  return true;
}

// 检查是否能插入该矩形
bool Texturepool::canInsert(int width, int height) {
  for (const auto &r : freeDimensions) {
    if (r.width >= width && r.height >= height) {
      // 有足够空间插入
      return true;
    }
  }
  // 没有足够的空间
  return false;
}

// 扩展图集的尺寸
void Texturepool::expandAtlas() {
  _vwidth *= 2;
  _vheight *= 2;

  // 重新创建一个更大的图集区域，所有空闲区域都需要更新
  std::vector<Rect> newFreeRectangles;
  // 新的空闲区域是整个图集区域
  newFreeRectangles.push_back(Rect(0, 0, _vwidth, _vheight));
  freeDimensions = newFreeRectangles;

  std::cout << "Atlas size scaled to: " << _vwidth << "x" << _vheight
            << std::endl;
}
// 合并相邻的空闲区域（减少浪费）
void Texturepool::mergeFreeRectangles() {
  std::vector<Rect> merged;
  for (int i = 0; i < freeDimensions.size(); ++i) {
    bool mergedFlag = false;
    for (int j = 0; j < merged.size(); ++j) {
      if (canMerge(freeDimensions[i], merged[j])) {
        merged[j] = merge(freeDimensions[i], merged[j]);
        mergedFlag = true;
        break;
      }
    }
    if (!mergedFlag) {
      merged.push_back(freeDimensions[i]);
    }
  }
  freeDimensions = merged;
}
// 判断两个空闲区域是否能合并
bool Texturepool::canMerge(const Rect &r1, const Rect &r2) {
  return (r1.x + r1.width == r2.x && r1.y == r2.y) ||
         (r1.y + r1.height == r2.y && r1.x == r2.x);
}
// 合并两个相邻的空闲区域
Rect Texturepool::merge(const Rect &r1, const Rect &r2) {
  int x = std::min(r1.x, r2.x);
  int y = std::min(r1.y, r2.y);
  int width = std::max(r1.x + r1.width, r2.x + r2.width) - x;
  int height = std::max(r1.y + r1.height, r2.y + r2.height) - y;
  return Rect(x, y, width, height);
}

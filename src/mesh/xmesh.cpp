#include "xmesh.h"
#include <stdexcept>

Xmesh::Xmesh() {}

Xmesh::~Xmesh() {}

void Xmesh::load(std::vector<std::vector<float>> &vertices_data) {
  // 解码顶点数据并添加到data
  for (auto vertex_data : vertices_data) {
    if (vertex_data.size() == 3) {
      _positions.push_back({vertex_data[0], vertex_data[1], vertex_data[2]});
    } else if (vertex_data.size() == 7) {
      _positions.push_back({vertex_data[0], vertex_data[1], vertex_data[2]});
      _colors.push_back(
          {vertex_data[3], vertex_data[4], vertex_data[5], vertex_data[6]});
    } else if (vertex_data.size() == 5) {
      _positions.push_back({vertex_data[0], vertex_data[1], vertex_data[2]});
      _uvs.push_back({vertex_data[3], vertex_data[4]});
    } else if (vertex_data.size() == 9) {
      _positions.push_back({vertex_data[0], vertex_data[1], vertex_data[2]});
      _colors.push_back(
          {vertex_data[3], vertex_data[4], vertex_data[5], vertex_data[6]});
      _uvs.push_back({vertex_data[7], vertex_data[8]});
    } else {
      throw std::runtime_error("vertex data error");
    }
    // 检查数据
    if ((_positions.size() == _colors.size() == _uvs.size())) {
      throw std::runtime_error("vertex data error");
    }
  }
}

#include "yui/YuiTilemap.h"

using namespace BlackT;

namespace Md {


YuiTilemap::YuiTilemap() {
  
}

void YuiTilemap::read(BlackT::TStream& ifs, int w__, int h__) {
  tileIds_.resize(w__, h__);
  
  for (int j = 0; j < h__; j++) {
    for (int i = 0; i < w__; i++) {
      tileIds_.data(i, j) = ifs.readu8be();
    }
  }
}

void YuiTilemap::write(BlackT::TStream& ofs) const {
  for (int j = 0; j < h(); j++) {
    for (int i = 0; i < w(); i++) {
      ofs.writeu8be(tileIds_.data(i, j));
    }
  }
}

void YuiTilemap::toMdTilemap(MdTilemap& dst, int rawBaseTileId) const {
  dst.resize(w(), h());
  
  for (int j = 0; j < h(); j++) {
    for (int i = 0; i < w(); i++) {
      MdTileId id;
      id.fromRawValue(rawBaseTileId + tileIds_.data(i, j));
      dst.setTileId(i, j, id);
    }
  }
}

void YuiTilemap::fromMdTilemap(MdTilemap& src, int rawBaseTileId) {
  tileIds_.resize(src.w(), src.h());
  
  for (int j = 0; j < h(); j++) {
    for (int i = 0; i < w(); i++) {
      MdTileId id = src.getTileId(i, j);
      tileIds_.data(i, j) = ((id.toRawValue() - rawBaseTileId) & 0x00FF);
    }
  }
}

int YuiTilemap::w() const {
  return tileIds_.w();
}

int YuiTilemap::h() const {
  return tileIds_.h();
}


}

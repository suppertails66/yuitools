#include "md/MdTilemap.h"

using namespace BlackT;

namespace Md {


MdTilemap::MdTilemap() { }
  
void MdTilemap::resize(int w__, int h__) {
  tileIds.resize(w__, h__);
}

const MdTileId& MdTilemap::getTileId(int x, int y) const {
  return tileIds.data(x, y);
}

void MdTilemap::setTileId(int x, int y, const MdTileId& tileId) {
  tileIds.data(x, y) = tileId;
}
  
int MdTilemap::w() const {
  return tileIds.w();
}

int MdTilemap::h() const {
  return tileIds.h();
}

void MdTilemap::read(const char* src, int w__, int h__) {
  resize(w__, h__);
  
  for (int j = 0; j < h__; j++) {
    for (int i = 0; i < w__; i++) {
      tileIds.data(i, j).read(src);
      src += MdTileId::size;
    }
  }
}

int MdTilemap::write(char* dst) const {
  int pos = 0;
  for (int j = 0; j < tileIds.h(); j++) {
    for (int i = 0; i < tileIds.w(); i++) {
      tileIds.data(i, j).write(dst + pos);
      pos += MdTileId::size;
    }
  }
  
  return pos;
}
  
void MdTilemap::toColorGraphic(BlackT::TGraphic& dst,
                    const MdVram& vram,
                    const MdPalette& pal) {
  dst.resize(tileIds.w() * MdPattern::w,
             tileIds.h() * MdPattern::h);
  dst.clearTransparent();
  
  for (int j = 0; j < tileIds.h(); j++) {
    for (int i = 0; i < tileIds.w(); i++) {
      tileIds.data(i, j).toColorGraphic(
        dst, vram, pal, (i * MdPattern::w), (j * MdPattern::h));
    }
  }
}
  
void MdTilemap::toGrayscaleGraphic(BlackT::TGraphic& dst,
                    const MdVram& vram) {
  dst.resize(tileIds.w() * MdPattern::w,
             tileIds.h() * MdPattern::h);
  dst.clearTransparent();
  
  for (int j = 0; j < tileIds.h(); j++) {
    for (int i = 0; i < tileIds.w(); i++) {
      tileIds.data(i, j).toGrayscaleGraphic(
        dst, vram, (i * MdPattern::w), (j * MdPattern::h));
    }
  }
}


}

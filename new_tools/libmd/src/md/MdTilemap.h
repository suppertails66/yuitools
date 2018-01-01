#ifndef MDTILEMAP_H
#define MDTILEMAP_H


#include "util/TArray.h"
#include "util/TTwoDArray.h"
#include "util/TByte.h"
#include "util/TGraphic.h"
#include "util/TIfstream.h"
#include "util/TOfstream.h"
#include "md/MdTileId.h"

namespace Md {


class MdTilemap {
public:
  
  MdTilemap();
  
  void resize(int w__, int h__);
  const MdTileId& getTileId(int x, int y) const;
  void setTileId(int x, int y, const MdTileId& tileId);
  
  int w() const;
  int h() const;
  
  void read(const char* src, int w__, int h__);
  int write(char* dst) const;
  
  void toColorGraphic(BlackT::TGraphic& dst,
                      const MdVram& vram,
                      const MdPalette& pal);
  
  void toGrayscaleGraphic(BlackT::TGraphic& dst,
                      const MdVram& vram);
  
  BlackT::TTwoDArray<MdTileId> tileIds;
protected:
};


}


#endif

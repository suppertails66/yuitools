#ifndef YUITILEMAP_H
#define YUITILEMAP_H


#include "md/MdTilemap.h"
#include "util/TTwoDArray.h"
#include "util/TStream.h"

namespace Md {


class YuiTilemap {
public:
  YuiTilemap();
  
  void read(BlackT::TStream& ifs, int w__, int h__);
  void write(BlackT::TStream& ofs) const;
  
  void toMdTilemap(MdTilemap& dst, int rawBaseTileId) const;
  void fromMdTilemap(MdTilemap& src, int rawBaseTileId);
  
  int w() const;
  int h() const;
protected:
  typedef BlackT::TTwoDArray<BlackT::TByte> TileIdMatrix;
  
  TileIdMatrix tileIds_;
};


}


#endif

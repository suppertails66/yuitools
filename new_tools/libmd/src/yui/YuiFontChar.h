#ifndef YUIFONTCHAR_H
#define YUIFONTCHAR_H


#include "util/TTwoDArray.h"
#include "util/TByte.h"
#include "util/TStream.h"
#include "util/TGraphic.h"
#include <vector>

namespace Md {


class YuiFontChar {
public:
  const static int width = 8;
  const static int height = 8;
  const static int size = 8;
  const static int pixelsPerByte = 8;

  YuiFontChar();
  
  void read(BlackT::TStream& ifs);
  void write(BlackT::TStream& ofs) const;
  void saveToGraphic(BlackT::TGraphic& dst,
                     int offsetX = 0, int offsetY = 0) const;
  void loadFromGraphic(const BlackT::TGraphic& src,
                     int offsetX = 0, int offsetY = 0);
protected:
  const static BlackT::TColor fgColor;
  const static BlackT::TColor bgColor;

  typedef BlackT::TTwoDArray<bool> CharData;
  
  CharData data_;
  
};


}


#endif

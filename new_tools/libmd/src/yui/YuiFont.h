#ifndef YUIFONT_H
#define YUIFONT_H


#include "util/TTwoDArray.h"
#include "util/TByte.h"
#include "util/TStream.h"
#include "util/TGraphic.h"
#include "yui/YuiFontChar.h"
#include <vector>

namespace Md {


class YuiFont {
public:
  YuiFont();
  
  void read(BlackT::TStream& ifs, int numChars);
  void write(BlackT::TStream& ofs);
  void saveToGraphic(BlackT::TGraphic& dst) const;
  void loadFromGraphic(const BlackT::TGraphic& src,
                     int numChars);
protected:
  const static int outputCharsPerRow = 16;

  typedef std::vector<YuiFontChar> CharDataCollection;
  
  CharDataCollection chars_;
  
};


}


#endif

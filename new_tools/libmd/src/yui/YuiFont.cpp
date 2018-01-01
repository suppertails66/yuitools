#include "yui/YuiFont.h"

namespace Md {


YuiFont::YuiFont() {
  
}

void YuiFont::read(BlackT::TStream& ifs, int numChars) {
  chars_.resize(numChars);
  for (CharDataCollection::size_type i = 0; i < chars_.size(); i++) {
    chars_[i].read(ifs);
  }
}

void YuiFont::write(BlackT::TStream& ofs) {
  for (CharDataCollection::size_type i = 0; i < chars_.size(); i++) {
    chars_[i].write(ofs);
  }
}

void YuiFont::saveToGraphic(BlackT::TGraphic& dst) const {
  int numRows = chars_.size() / outputCharsPerRow;
  // above calculation underruns by 1 if number of chars is not exactly
  // divisble by number of rows
  if ((chars_.size() % outputCharsPerRow) != 0) ++numRows;
  
  dst.resize(outputCharsPerRow * YuiFontChar::width,
             numRows * YuiFontChar::height);
  dst.clearTransparent();
  
  for (CharDataCollection::size_type i = 0; i < chars_.size(); i++) {
    int x = (i % outputCharsPerRow) * YuiFontChar::width;
    int y = (i / outputCharsPerRow) * YuiFontChar::height;
    
    chars_[i].saveToGraphic(dst, x, y);
  }
}

void YuiFont::loadFromGraphic(const BlackT::TGraphic& src,
                              int numChars) {
  chars_.resize(numChars);
  
  for (CharDataCollection::size_type i = 0; i < chars_.size(); i++) {
    int x = (i % outputCharsPerRow) * YuiFontChar::width;
    int y = (i / outputCharsPerRow) * YuiFontChar::height;
    
    chars_[i].loadFromGraphic(src, x, y);
  }
}


} 

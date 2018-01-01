#include "yui/YuiFontChar.h"

using namespace BlackT;

namespace Md {


const BlackT::TColor YuiFontChar::fgColor(
  0, 0, 0, BlackT::TColor::fullAlphaOpacity);
const BlackT::TColor YuiFontChar::bgColor(
  255, 255, 255, BlackT::TColor::fullAlphaTransparency);
  
YuiFontChar::YuiFontChar() {
  data_.resize(width, height);
}

void YuiFontChar::read(BlackT::TStream& ifs) {
  char buffer[size];
  ifs.read(buffer, sizeof(buffer));
  
  for (unsigned int j = 0; j < sizeof(buffer); j++) {
    char next = buffer[j];
    int mask = 0x80;
    for (int i = 0; i < pixelsPerByte; i++) {
      // set bit = pixel exists
      if ((next & mask) != 0) {
        data_.data(i, j) = true;
      }
      else {
        data_.data(i, j) = false;
      }
      
      mask >>= 1;
    }
  }
}

void YuiFontChar::write(BlackT::TStream& ofs) const {
  char buffer[size];
  for (unsigned int j = 0; j < data_.h(); j++) {
    char next = 0;
    int mask = 0x80;
    
    for (int i = 0; i < pixelsPerByte; i++) {
      // set bit = pixel exists
      if (data_.data(i, j)) next |= mask;
      
      mask >>= 1;
    }
    
    buffer[j] = next;
  }
  
  ofs.write(buffer, sizeof(buffer));
}

void YuiFontChar::saveToGraphic(BlackT::TGraphic& dst,
                                int offsetX, int offsetY) const {
  for (int j = 0; j < data_.h(); j++) {
    for (int i = 0; i < data_.w(); i++) {
      if (data_.data(i, j)) dst.setPixel(offsetX + i, offsetY + j, fgColor);
      else dst.setPixel(offsetX + i, offsetY + j, bgColor);
    }
  }
}

void YuiFontChar::loadFromGraphic(const BlackT::TGraphic& src,
                                int offsetX, int offsetY) {
  for (int j = 0; j < data_.h(); j++) {
    for (int i = 0; i < data_.w(); i++) {
      if (src.getPixel(offsetX + i, offsetY + j).a()
            == BlackT::TColor::fullAlphaOpacity)
        data_.data(i, j) = true;
      else
        data_.data(i, j) = false;
    }
  }
}


}

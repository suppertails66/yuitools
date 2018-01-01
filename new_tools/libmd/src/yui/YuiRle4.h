#ifndef YUIRLE4_H
#define YUIRLE4_H


#include "util/TByte.h"
#include "util/TStream.h"

namespace Md {


class YuiRle4 {
public:
  
  static void decompress(BlackT::TStream& src, BlackT::TStream& dst);
  
  static void compress(BlackT::TStream& src, int srcLen,
                       BlackT::TStream& dst);
  
protected:
  const static char terminator = 0;
  const static int maxAbsoluteRun = 8;
  const static int minRepeatRun = 4;
  const static int maxRepeatRun = 10;

  static inline char fetchNextNybble(char* srcChar, bool* srcNybLow,
                              BlackT::TStream& src);

  static inline char peekNextNybble(char* srcChar, bool* srcNybLow,
                              BlackT::TStream& src);

  static inline void setNextNybble(char nyb,
                              char* dstChar, bool* dstNybLow,
                              BlackT::TStream& dst);

  static inline int countNybbleRepeats(char nyb,
                              char* srcChar, bool* srcNybLow,
                              BlackT::TStream& src,
                              int srcEndPos);
  
  static void appendToAbsoluteBuffer(char nyb,
                              char pendingAbsoluteBuffer[],
                              int* pendingAbsolutePos,
                              char* dstChar, bool* dstNybLow,
                              BlackT::TStream& dst);
  
  static void flushAbsoluteBuffer(char pendingAbsoluteBuffer[],
                              int* pendingAbsolutePos,
                              char* dstChar, bool* dstNybLow,
                              BlackT::TStream& dst);
  
};


}


#endif

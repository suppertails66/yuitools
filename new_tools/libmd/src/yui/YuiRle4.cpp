#include "yui/YuiRle4.h"
#include <iostream>

using namespace BlackT;

namespace Md {


void YuiRle4::decompress(BlackT::TStream& src, BlackT::TStream& dst) {
  bool srcNybLow = true;
  bool dstNybLow = true;
  char srcChar = 0;
  char dstChar = 0;
  
  while (true) {
    // Fetch next command nybble
    char nextNyb = fetchNextNybble(&srcChar, &srcNybLow, src);
    
    // If terminator, we're done
    if (nextNyb == terminator) {
      // Make sure the last byte gets written if pending
      if (!dstNybLow) dst.put(dstChar);
      return;
    }
    // If >= 8, this is an absolute run
    else if (nextNyb >= 8) {
      int length = nextNyb - 8 + 1;
      
      for (int i = 0; i < length; i++) {
        char nyb = fetchNextNybble(&srcChar, &srcNybLow, src);
        setNextNybble(nyb, &dstChar, &dstNybLow, dst);
      }
    }
    // If < 8, this is a repeat run
    else {
      // add 3 to get actual length
      int length = nextNyb + 3;
      
      char nyb = fetchNextNybble(&srcChar, &srcNybLow, src);
      for (int i = 0; i < length; i++) {
        setNextNybble(nyb, &dstChar, &dstNybLow, dst);
      }
    }
  }
}
  
void YuiRle4::compress(BlackT::TStream& src, int srcLen,
                     BlackT::TStream& dst) {
  int srcEndPos = src.tell() + srcLen;
                     
  char pendingAbsoluteBuffer[maxAbsoluteRun];
  int pendingAbsolutePos = 0;
  
  bool srcNybLow = true;
  bool dstNybLow = true;
  char srcChar = 0;
  char dstChar = 0;
  
  while (true) {
    char nextNyb = fetchNextNybble(&srcChar, &srcNybLow, src);
//    std::cout << std::hex << (int)nextNyb << std::endl;
//    char c; std::cin >> c;
    // Add 1 to repeat count -- the returned value does not include the
    // intial nybble we've already read
    int repCount = countNybbleRepeats(nextNyb, &srcChar, &srcNybLow, src,
                                      srcEndPos)
                      + 1;
    
    // If there are enough repeats to do a repeat command, flush the pending
    // absolute command, then add the repeat command
    if ((repCount >= minRepeatRun)) {
      flushAbsoluteBuffer(pendingAbsoluteBuffer, &pendingAbsolutePos,
                          &dstChar, &dstNybLow, dst);
      
      // Add repeat command
      setNextNybble(repCount - 3, &dstChar, &dstNybLow, dst);
      setNextNybble(nextNyb, &dstChar, &dstNybLow, dst);
      
    }
    // Otherwise, add the repeat nybbles we read to the pending absolute
    // buffer (which will automatically be flushed if full)
    else {
      for (int i = 0; i < repCount; i++) {
        appendToAbsoluteBuffer(nextNyb, pendingAbsoluteBuffer,
                               &pendingAbsolutePos,
                               &dstChar, &dstNybLow, dst);
      }
    }
    
    // Stop when we're done
    if ((src.tell() >= srcEndPos)
        && (srcNybLow)) break;
  }
  
  // Flush any pending absolute command
  flushAbsoluteBuffer(pendingAbsoluteBuffer, &pendingAbsolutePos,
                      &dstChar, &dstNybLow, dst);
  
  // Write terminator
  setNextNybble(0, &dstChar, &dstNybLow, dst);
  
  
  // Make sure the last byte gets written if pending
  if (!dstNybLow) dst.put(dstChar);
}

inline char YuiRle4::fetchNextNybble(char* srcChar, bool* srcNybLow,
                            BlackT::TStream& src) {
  char nextNyb;
  
  // if next nybble is low, get it from the current byte
  if ((*srcNybLow = !(*srcNybLow))) nextNyb = *srcChar & 0x0F;
  // if next nybble is high, fetch a new source byte
  else {
    *srcChar = src.get();
    nextNyb = (*srcChar & 0xF0) >> 4;
  }
  
  return nextNyb;
}

inline char YuiRle4::peekNextNybble(char* srcChar, bool* srcNybLow,
                            BlackT::TStream& src) {
  char nextNyb;
  
  // if next nybble is low, get it from the current byte
  if (!(*srcNybLow)) nextNyb = *srcChar & 0x0F;
  // if next nybble is high, peek at next source byte
  else {
//    src.unget();
    nextNyb = (src.peek() & 0xF0) >> 4;
  }
  
  return nextNyb;
}

inline void YuiRle4::setNextNybble(char nyb,
                            char* dstChar, bool* dstNybLow,
                            BlackT::TStream& dst) {
  // If next nybble is low, write to low position and place
  // byte in stream
  if ((*dstNybLow = !(*dstNybLow))) {
    *dstChar |= nyb;
    dst.put(*dstChar);
    // Reset dst byte
    *dstChar = 0;
  }
  // If next nybble is high, write to high position
  else *dstChar |= (nyb << 4);
}

inline int YuiRle4::countNybbleRepeats(char nyb,
                            char* srcChar, bool* srcNybLow,
                            BlackT::TStream& src,
                            int srcEndPos) {
  // Only count up to the maximum repeat length (minus one; the implicit
  // initial occurence of the nybble counts towards the total)
  for (int i = 0; i < maxRepeatRun - 1; i++) {
    // Don't read past end of input
    if ((src.tell() >= srcEndPos)
        && (*srcNybLow)) return i;
    
    char nextNyb = peekNextNybble(srcChar, srcNybLow, src);
    
    if (nextNyb != nyb) return i;
    // Only fetch the nybble if it's not the last one; we need to leave
    // the terminating nybble in the stream for the next iteration of the
    // compression loop to find
    else fetchNextNybble(srcChar, srcNybLow, src);
  }
  
  return maxRepeatRun - 1;
}
  
void YuiRle4::appendToAbsoluteBuffer(char nyb,
                            char pendingAbsoluteBuffer[],
                            int* pendingAbsolutePos,
                            char* dstChar, bool* dstNybLow,
                            BlackT::TStream& dst) {
  // If the buffer is full, flush it
  if (*pendingAbsolutePos == maxAbsoluteRun)
    flushAbsoluteBuffer(pendingAbsoluteBuffer,
                        pendingAbsolutePos,
                        dstChar, dstNybLow, dst);
  
  // Add the nybble
  pendingAbsoluteBuffer[(*pendingAbsolutePos)++] = nyb;
}
  
void YuiRle4::flushAbsoluteBuffer(char pendingAbsoluteBuffer[],
                            int* pendingAbsolutePos,
                            char* dstChar, bool* dstNybLow,
                            BlackT::TStream& dst) {
  // Do nothing if buffer is empty
  if (*pendingAbsolutePos == 0) return;
  
  // Write absolute command nybble
  char commandNyb = *pendingAbsolutePos + 8 - 1;
  setNextNybble(commandNyb, dstChar, dstNybLow, dst);
  
  // Write each absolute nybble
  for (int i = 0; i < *pendingAbsolutePos; i++) {
    char nyb = pendingAbsoluteBuffer[i];
    setNextNybble(nyb, dstChar, dstNybLow, dst);
  }
  
  // Reset buffer position
  *pendingAbsolutePos = 0;
}


}

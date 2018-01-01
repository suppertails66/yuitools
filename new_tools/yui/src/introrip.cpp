#include "util/TThingyTable.h"
#include "util/TStringConversion.h"
#include "util/TIfstream.h"
#include "util/TOfstream.h"
#include "util/TBufStream.h"
#include "util/TOpt.h"
#include <iostream>

using namespace std;
using namespace BlackT;

#define RIPMAPS 1
#define RIPASM 0

int main(int argc, char* argv[]) {
  if (argc < 5) {
    cout << "Battle Golfer Yui intro tilemap ripper" << endl;
    cout << "Usage: " << argv[0] << " <infile> <offset> <thingy> <numentries>"
      << endl;
    cout << "Options: " << endl;
    cout << "  -p   Pad each line to the given width with spaces" << endl;
    
    return 0;
  }
  
  char* infileName = argv[1];
  int offset = TStringConversion::stringToInt(string(argv[2]));
  char* thingyName = argv[3];
  int numentries = TStringConversion::stringToInt(string(argv[4]));

  int padwidth = -1;
  TOpt::readNumericOpt(argc, argv, "-p", &padwidth);
  
  TThingyTable thingy;
  thingy.readSjis(string(thingyName));
  
  TIfstream ifs(infileName, ios_base::binary);
  ifs.seek(offset);
  
  #if RIPMAPS
    cout << "//================================================================"
      << endl;
    cout << "// Intro. Originals are variable-width; switched to uniform 36\n"
            "// tiles for simplicity's sake."
      << endl;
    cout << "//================================================================"
      << endl << endl;
    cout << "#ALIGN(4)" << endl << endl;
  #endif
  
  int currentIndexOffset = offset;
  for (int i = 0; i < numentries; i++) {
    int baseIndexOffset = ifs.tell();
  
    // 4b pointer to half-width tilemap data
    int mapPointer = ifs.readu32be();
    
    // 2b target vram address (we don't care)
    ifs.readu16be();
    
    // 2b width
    int width = ifs.readu16be();
    // 2b height
    int height = ifs.readu16be();
    
    currentIndexOffset = ifs.tell();
    
    ifs.seek(mapPointer);
    
    string outtxt;
    
    // add blank lines if less than 2
    if (height < 2) {
      for (int j = 0; j < 2 - height; j++) {
        for (int i = 0; i < padwidth; i++) {
          outtxt += thingy.getEntry(0);
        }
        outtxt += "\n";
      }
    }
    
    for (int j = 0; j < height; j++) {
      for (int i = 0; i < width; i++) {
        unsigned char value = ifs.get();
        outtxt += thingy.getEntry(value);
      }
      
      if (padwidth != -1) {
        for (int i = 0; i < (padwidth - width); i++) {
          outtxt += thingy.getEntry(0);
        }
      }
      
      if (j != height - 1) outtxt += "\n";
    }
    
    #if RIPMAPS
      cout << "// [" << TStringConversion::intToString(mapPointer,
                          TStringConversion::baseHex) << "]" << endl;
  //    cout << "#W32($" << hex << baseIndexOffset << ")" << endl;
      // atlas will not accept lower case letters in hex values
      cout << "#W32($" << hex << TStringConversion::intToString(baseIndexOffset,
                          TStringConversion::baseHex).substr(2, string::npos)
                          << ")" << endl;
      cout << outtxt << endl << endl;
    #endif
    
    #if RIPASM
      // Generate 68k assembler directives to correct widths
      cout << "  org $" << hex << baseIndexOffset + 4 << endl;
      // intro
//      cout << "  dc.b $BB,$04,$00,$24,$00,$02" << endl << endl;
      // cutscene 1
//      cout << "  dc.b $E3,$04,$00,$24,$00,$02" << endl << endl;
      // cutscene 2
      cout << "  dc.b $EB,$04,$00,$24,$00,$02" << endl << endl;
    #endif
    
    ifs.seek(currentIndexOffset);
  }
  
  return 0;
} 

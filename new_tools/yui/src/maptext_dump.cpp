#include "util/TThingyTable.h"
#include "util/TStringConversion.h"
#include "util/TIfstream.h"
#include "util/TOfstream.h"
#include "util/TBufStream.h"
#include "util/TOpt.h"
#include <cstring>
#include <vector>
#include <iostream>

using namespace std;
using namespace BlackT;

int main(int argc, char* argv[]) {
  if (argc < 6) {
    cout << "Battle Golfer Yui 16-bit tilemap text dumper" << endl;
    cout << "Usage: " << argv[0] << " <infile> <offset> <thingy>"
      << " <width> <height>" << endl;
    cout << "Options: " << endl;
    cout << "  -p   Pad each line to the given width with spaces" << endl;
    cout << "  -w   Specify target offset for a #W32 write" << endl;
    
    return 0;
  }
  
  char* infileName = argv[1];
  int offset = TStringConversion::stringToInt(string(argv[2]));
  char* thingyName = argv[3];
  int width = TStringConversion::stringToInt(string(argv[4]));
  int height = TStringConversion::stringToInt(string(argv[5]));
//  char* outfileName = argv[7];

  int padwidth = -1;
  TOpt::readNumericOpt(argc, argv, "-p", &padwidth);
  
//  int w32Offset = -1;
//  TOpt::readNumericOpt(argc, argv, "-w", &w32Offset);
  vector<int> w32Offsets;
  for (int i = 1; i < argc - 1; i++) {
    if (strcmp(argv[i], "-w") == 0) {
      w32Offsets.push_back(
        TStringConversion::stringToInt(string(argv[i + 1])));
    }
  }
  
  TThingyTable thingy;
  thingy.readSjis(string(thingyName));
  
  TIfstream ifs(infileName, ios_base::binary);
  ifs.seek(offset);
  
  string outtxt;
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      // first character gets copied as-is
      unsigned char copy = ifs.get();
      outtxt += "<$";
      string copyStr = TStringConversion::intToString((int)copy,
                      TStringConversion::baseHex).substr(2, string::npos);
      // pad to 2 bytes to keep things neat
      if (copyStr.size() < 2) copyStr = "0" + copyStr;
      outtxt += copyStr;
      outtxt += ">";
      
      // second character is mapped as text if possible
      unsigned char value = ifs.get();
      if (thingy.hasEntry(value)) {
        outtxt += thingy.getEntry(value);
      }
      else {
        outtxt += "<$";
        string copyStr = TStringConversion::intToString((int)value,
                        TStringConversion::baseHex).substr(2, string::npos);
        // pad to 2 bytes to keep things neat
        if (copyStr.size() < 2) copyStr = "0" + copyStr;
        outtxt += copyStr;
        outtxt += ">";
      }
    }
    
    if (padwidth != -1) {
      for (int i = 0; i < (padwidth - width); i++) {
        outtxt += "<$00>";
        outtxt += thingy.getEntry(0);
      }
    }
    
    if (j != height - 1) outtxt += "\n";
  }
  
  cout << "// [" << TStringConversion::intToString(offset,
                      TStringConversion::baseHex) << "]" << endl;
  for (vector<int>::size_type i = 0; i < w32Offsets.size(); i++) {
    cout << "#W32($" << TStringConversion::intToString(w32Offsets[i],
                      TStringConversion::baseHex).substr(2, string::npos)
        << ")" << endl;
  }
  cout << outtxt << endl << endl;
  
  return 0;
} 

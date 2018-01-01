#include "util/TIfstream.h"
#include "util/TOfstream.h"
#include "util/TStringConversion.h"
#include <iostream>

using namespace std;
using namespace BlackT;

int main(int argc, char* argv[]) {
  if (argc < 4) {
    cout << "Battle Golfer Yui script table dumper" << endl;
    cout << "Usage: " << argv[0] << " <infile> <offset> <numentries>"
      << endl;
    
    return 0;
  }

  char* infileName = argv[1];
  int offset = TStringConversion::stringToInt(string(argv[2]));
  int numentries = TStringConversion::stringToInt(string(argv[3]));
  
  TIfstream ifs(infileName, ios_base::binary);
  ifs.seek(offset);
  
  for (int i = 0; i < numentries; i++) {
    int ptrLoc = ifs.tell();
    int ptrVal = ifs.readu16be() + offset;
    
    cout << "RAW "
      << TStringConversion::intToString(ptrLoc, TStringConversion::baseHex)
          .substr(2, string::npos)
      << " "
      << TStringConversion::intToString(ptrVal, TStringConversion::baseHex)
          .substr(2, string::npos)
      << endl;
  }
  
  return 0;
}

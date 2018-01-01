#include "yui/YuiRle4.h"
#include "util/TIfstream.h"
#include "util/TOfstream.h"
#include "util/TBufStream.h"
#include "util/TStringConversion.h"
#include <iostream>

using namespace std;
using namespace BlackT;
using namespace Md;

int main(int argc, char* argv[]) {
  if (argc < 4) {
    cout << "Battle Golfer Yui RLE4 decompressor" << endl;
    cout << "Usage: " << argv[0] << " <infile> <offset> <outfile>" << endl;
    
    return 0;
  }
  
  char* infileName = argv[1];
  int offset = TStringConversion::stringToInt(string(argv[2]));
  char* outfileName = argv[3];
  
  TIfstream ifs(infileName, ios_base::binary);
  ifs.seek(offset);
  
  TBufStream ofs(0x100000);
  YuiRle4::decompress(ifs, ofs);
  
  cout << "Decompressed " << ifs.tell() - offset << " bytes" << endl;
  
  ofs.save(outfileName);
  
  return 0;
}

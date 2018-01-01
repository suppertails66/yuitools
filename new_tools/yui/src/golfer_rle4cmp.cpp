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
  if (argc < 3) {
    cout << "Battle Golfer Yui RLE4 compressor" << endl;
    cout << "Usage: " << argv[0] << " <infile> <outfile>" << endl;
    
    return 0;
  }
  
  char* infileName = argv[1];
//  int offset = TStringConversion::stringToInt(string(argv[2]));
  char* outfileName = argv[2];
  
  TIfstream ifs(infileName, ios_base::binary);
//  ifs.seek(offset);
  
  TBufStream ofs(0x100000);
  YuiRle4::compress(ifs, ifs.size(), ofs);
  
  cout << "Compressed size: " << ofs.tell() << " bytes" << endl;
  
  ofs.save(outfileName);
  
  return 0;
}

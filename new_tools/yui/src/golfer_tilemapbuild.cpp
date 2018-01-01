#include "yui/YuiTilemap.h"
#include "util/TStringConversion.h"
#include "util/TFileManip.h"
#include <iostream>

using namespace std;
using namespace BlackT;
using namespace Md;

int main(int argc, char* argv[]) {
  if (argc < 6) {
    cout << "Battle Golfer Yui tilemap builder" << endl;
    cout << "Usage: " << argv[0] << " <infile> <w> <h>"
      << " <basevdp> <outfile>" << endl;
    
    return 0;
  }
  
  char* infileName = argv[1];
  int w = TStringConversion::stringToInt(string(argv[2]));
  int h = TStringConversion::stringToInt(string(argv[3]));
  int baseVdp = TStringConversion::stringToInt(string(argv[4]));
  char* outfileName = argv[5];
  
  TArray<TByte> infile;
  TFileManip::readEntireFile(infile, infileName);
//  TIfstream ifs(infileName, ios_base::binary);
//  ifs.seek(offset);

  MdTilemap mdMap;
  mdMap.read((char*)(infile.data()), w, h);
  
  YuiTilemap yuiMap;
  yuiMap.fromMdTilemap(mdMap, baseVdp);
  
  TOfstream ofs(outfileName, ios_base::binary);
  yuiMap.write(ofs);

  return 0;
}

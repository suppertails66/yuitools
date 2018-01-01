#include "yui/YuiTilemap.h"
#include "util/TStringConversion.h"
#include "util/TFileManip.h"
#include <iostream>

using namespace std;
using namespace BlackT;
using namespace Md;

int main(int argc, char* argv[]) {
  if (argc < 7) {
    cout << "Battle Golfer Yui tilemap ripper" << endl;
    cout << "Usage: " << argv[0] << " <infile> <offset> <w> <h>"
      << " <basevdp> <outfile>" << endl;
    
    return 0;
  }
  
  char* infileName = argv[1];
  int offset = TStringConversion::stringToInt(string(argv[2]));
  int w = TStringConversion::stringToInt(string(argv[3]));
  int h = TStringConversion::stringToInt(string(argv[4]));
  int baseVdp = TStringConversion::stringToInt(string(argv[5]));
  char* outfileName = argv[6];
  
//  TArray<TByte> infile;
//  TFileManip::readEntireFile(infile, infileName);
  TIfstream ifs(infileName, ios_base::binary);
  ifs.seek(offset);
  
  YuiTilemap yuiMap;
  yuiMap.read(ifs, w, h);
  
  MdTilemap mdMap;
  yuiMap.toMdTilemap(mdMap, baseVdp);
  
  char outbuffer[0x10000];
  int outbufferSize = mdMap.write(outbuffer);
  
  TOfstream ofs(outfileName, ios_base::binary);
  ofs.write(outbuffer, outbufferSize);

  return 0;
}

#include "util/TIfstream.h"
#include "util/TStringConversion.h"
#include "util/TGraphic.h"
#include "util/TPngConversion.h"
#include "md/MdPattern.h"
#include "yui/YuiFont.h"
#include <iostream>

using namespace std;
using namespace BlackT;
using namespace Md;

int main(int argc, char* argv[]) {
  if (argc < 5) {
    cout << "Battle Golfer Yui font ripper" << endl;
    cout << "Usage: " << argv[0] << " <src> <srcoffset> <numchars> <outfile>"
      << endl;
    
    return 0;
  }
  
  char* romName = argv[1];
  int offset = TStringConversion::stringToInt(string(argv[2]));
  int numChars = TStringConversion::stringToInt(string(argv[3]));
  char* outfileName = argv[4];
  
  TIfstream ifs(romName, ios_base::binary);
  ifs.seek(offset);
  
  YuiFont font;
  font.read(ifs, numChars);
  
  TGraphic dst;
  font.saveToGraphic(dst);
  
  TPngConversion::graphicToRGBAPng(string(outfileName), dst);
  
  return 0;
}

#include "util/TIfstream.h"
#include "util/TOfstream.h"
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
  if (argc < 4) {
    cout << "Battle Golfer Yui font builder" << endl;
    cout << "Usage: " << argv[0] << " <infile> <numchars> <outfile>"
      << endl;
    
    return 0;
  }
  
  char* infileName = argv[1];
  int numChars = TStringConversion::stringToInt(string(argv[2]));
  char* outfileName = argv[3];
  
  TGraphic src;
  TPngConversion::RGBAPngToGraphic(string(infileName), src);
  
  YuiFont font;
  font.loadFromGraphic(src, numChars);
  
  TOfstream ofs(outfileName, ios_base::binary);
  
  font.write(ofs);
  
  return 0;
}

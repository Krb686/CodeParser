#include <iostream>

#include "ObjDumpParser.hpp"

using namespace std;

int main(){
  ObjDumpParser newParser(true);
  newParser.parse("/home/kevin/gitrepos/common/test/resources/tiny.disasm");

  return 0;
}

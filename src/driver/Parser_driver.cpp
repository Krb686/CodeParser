#include <iostream>

#include "ObjDumpParser.hpp"

using namespace std;

int main(){
  ObjDumpParser newParser(true);
  newParser.parse("/home/kevin/gitrepos/CodeParser/bin/test/test_dump.disasm");

  return 0;
}

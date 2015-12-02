#include <iostream>
#include "../include/ParserObj.hpp"

using namespace std;

int main(){
  ParserObj newParser(true);
  newParser.parse("/home/kevin/gitrepos/common/test/resources/tiny.disasm");

  return 0;
}

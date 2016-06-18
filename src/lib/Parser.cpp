#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "../../include/Parser.hpp"

using namespace std;

Parser::Parser(){

}

Parser::~Parser(){
    cout << "Exiting!\n";
}

string Parser::openFileToString(string filename){
    ifstream fileStream(filename.c_str(), ios::binary);
    stringstream buffer;
    buffer << fileStream.rdbuf();
    return buffer.str();
}

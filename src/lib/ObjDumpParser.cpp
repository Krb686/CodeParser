#include "../../include/ObjDumpParser.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <regex>
#include <sstream>
#include <string>

using namespace std;

ObjDumpParser::ObjDumpParser(){
	ObjDumpParser::debugLog = false;
}


ObjDumpParser::ObjDumpParser(bool debug){
	ObjDumpParser::debugLog = debug;
}

ObjDumpParser::~ObjDumpParser(){

}

//ObjDumpParser::ObjDumpParser(string pFlavor){
//    flavor = pFlavor;//
//}

string ObjDumpParser::openFileToString(string filename){
    ifstream fileStream(filename.c_str(), ios::binary);
    stringstream buffer;
    buffer << fileStream.rdbuf();
    return buffer.str();
}

void ObjDumpParser::parse(string filename){
    fileString = openFileToString(filename);
    fileString = preProcess(fileString);
    parseByLine(fileString);
    process();
    dumpAll();
}


void ObjDumpParser::parseByLine(string fileString){
    cout << "parseByLine:\n";

    istringstream fileInputStringStream(fileString);
    string line;
    int lineNum = 0;
    // Important strings


    int flagBasicInfo           = 0;
    int flagProgramHeader       = 0;
    int flagDynamicSection      = 0;
    int flagVersionReferences   = 0;
    int flagSectionsOverview    = 0;
    int flagSymbolTable         = 0;
    int flagSectionContent      = 0;
    int flagSectionDisassembly  = 0;

    int flagBasicInfoDetected           = 0;
    int flagProgramHeaderDetected       = 0;
    int flagDynamicSectionDetected      = 0;
    int flagVersionReferencesDetected   = 0;
    int flagSectionsOverviewDetected    = 0;
    int flagSymbolTableDetected         = 0;
    //int flagSectionContentDetected      = 0;
    //int flagSectionDisassemblyDetected  = 0;


    //regex* regexIdArray = new regex[10];
    regex regexProgramHeaderIdentifier("^Program Header:$");
    regex regexDynamicSectionIdentifier("^Dynamic Section:$");
    regex regexVersionReferencesIdentifier("Version References:$");
    regex regexSectionsOverviewIdentifer("^Sections:$");
    regex regexSymbolTableIdentifier("^SYMBOL TABLE:$");
    regex regexSectionContentIdentifier("^Contents of section.*:$");
    regex regexSectionDisassemblyIdentifier("^Disassembly of section.*:$");

    regex regexDoubleNewlineSeparator("^$");


    while(getline(fileInputStringStream, line)){
        lineNum++;
        //cout << fileStr + "\n";
        //cout << regex_search(fileStr, something);


        //TODO: Make sure this doesn't break if the text "Program Header: exists elsewhere, like a piece of data
        if(!flagBasicInfoDetected && !flagBasicInfo){
            flagBasicInfo++;
        } else if(regex_search(line, regexProgramHeaderIdentifier)){
            cout << "Entering program header!\n";
            if(!flagProgramHeader){
                flagProgramHeader++;
            }
        } else if(regex_search(line, regexDynamicSectionIdentifier)){
            cout << "Entering dynamic section!\n";
            if(!flagDynamicSection){
                flagDynamicSection++;
            }
        } else if(regex_search(line, regexVersionReferencesIdentifier)){
            cout << "Entering version references!\n";
            if(!flagVersionReferences){
                flagVersionReferences++;
            }
        } else if(regex_search(line, regexSectionsOverviewIdentifer)){
            cout << "Entering the sections overview!\n";
            if(!flagSectionsOverview){
                flagSectionsOverview++;
            }
        } else if(regex_search(line, regexSymbolTableIdentifier)){
            cout << "Entering the symbol table!\n";
            if(!flagSymbolTable){
                flagSymbolTable++;
            }
        } else if(regex_search(line, regexSectionContentIdentifier)){
            cout << "Entering section content!\n";
            if(!flagSectionContent){
                flagSectionContent++;
            } else {
                vectorSectionContent.push_back(strSectionContent);
                strSectionContent.clear();
            }
        } else if(regex_search(line, regexSectionDisassemblyIdentifier)){
            cout << "Entering section disassembly!\n";
            if(!flagSectionDisassembly){
                flagSectionDisassembly++;
            } else {
                vectorSectionDisassembly.push_back(strSectionDisassembly);
                strSectionDisassembly.clear();
            }
        }


        // Check the flags to see what section the parser is currently in
        if(flagBasicInfo){
            strBasicInfo.append(line + "\n");
            if(regex_search(line, regexDoubleNewlineSeparator) && lineNum != 1){
                flagBasicInfo--;
                flagBasicInfoDetected++;
                cout << "1) Basic info detected!\n";
            }
        } else if(flagProgramHeader){
            strProgramHeader.append(line + "\n");
            if(regex_search(line, regexDoubleNewlineSeparator)){
                flagProgramHeader--;
                flagProgramHeaderDetected++;
                cout << "2) Program header detected!\n";
            }
        } else if(flagDynamicSection){
            strDynamicSection.append(line + "\n");
            if(regex_search(line, regexDoubleNewlineSeparator)){
                flagDynamicSection--;
                flagDynamicSectionDetected++;
                cout << "3) Dynamic section detected!\n";
            }
        } else if(flagVersionReferences){
            strVersionReferences.append(line + "\n");
            if(regex_search(line, regexDoubleNewlineSeparator)){
                flagVersionReferences--;
                flagVersionReferencesDetected++;
                cout << "4) Version references detected!\n";
            }
        } else if(flagSectionsOverview){
            strSectionsOverview.append(line + "\n");
            if(regex_search(line, regexDoubleNewlineSeparator)){
                flagSectionsOverview--;
                flagSectionsOverviewDetected++;
                cout << "5) Sections Overview detected!\n";
            }

        } else if(flagSymbolTable){
            strSymbolTable.append(line + "\n");
            if(regex_search(line, regexDoubleNewlineSeparator)){
                flagSymbolTable--;
                flagSymbolTableDetected++;
                cout << "6) Symbol Table detected!\n";
            }

         } else if(flagSectionContent){
            strSectionContent.append(line + "\n");

            if(regex_search(line, regexDoubleNewlineSeparator)){
                flagSectionContent--;
            }

         } else if(flagSectionDisassembly){
            strSectionDisassembly.append(line + "\n");

         }


    }
    flagSectionDisassembly--;
    vectorSectionDisassembly.push_back(strSectionDisassembly);
}

void ObjDumpParser::parseByChar(string filename){


    cout << "Beginning to parse:\n";
    cout << filename + "\n";

    cout << flavor + "\n";
    if(flavor == "asm"){
        cout << "Flavor: assembly\n";
    } else if(flavor == "js"){
        cout << "Flavor: javascript\n";
    } else {
        cout << "Unsupported flavor!\n";
    }

    ifstream fileIn(filename.c_str(), ios::binary);
    vector<char> fileContents((istreambuf_iterator<char>(fileIn)), istreambuf_iterator<char>());

    //Vector length
    //cout << fileContents.size();

    for(vector<char>::iterator it = begin(fileContents); it != end(fileContents); ++it){
        //cout << *it;

    }

    //length
    //buffer.seekp(0, ios::end);
    //stringstream::pos_type offset = buffer.tellp();
    //cout << offset;

    //cout << buffer.str();
}

string ObjDumpParser::preProcess(string fileString){


    regex regexSymbolTableIdentifier("^SYMBOL TABLE:$");

    istringstream fileInputStringStream(fileString);
    string line;
    string preProcessedFileString;

    while(getline(fileInputStringStream, line)){
        if(regex_search(line, regexSymbolTableIdentifier)){
            cout << "hey\n";
            preProcessedFileString+="\n";
            preProcessedFileString+=line + "\n";
        } else {
            preProcessedFileString+=line + "\n";
        }
    }

    return preProcessedFileString;
}

void ObjDumpParser::dumpAll(){
    dumpBasicInfo();
    dumpProgramHeader();
    dumpDynamicSection();
    dumpVersionReferences();
    dumpSectionsOverview();
    dumpSymbolTable();

    int sectionContentLength = vectorSectionContent.size();
    int sectionDisassemblyLength = vectorSectionDisassembly.size();

    for(int i=0;i<sectionContentLength;i++){
        dumpSectionContent(i);
    }

    for(int i=0;i<sectionDisassemblyLength;i++){
        dumpSectionDisassembly(i);
    }
}

void ObjDumpParser::dumpBasicInfo(){
    cout << "Dumping basic info\n";
    cout << strBasicInfo + "\n";
}

void ObjDumpParser::dumpProgramHeader(){
    cout << "Dumping program header\n";
    cout << strProgramHeader + "\n";
}

void ObjDumpParser::dumpDynamicSection(){
    cout << "Dumping dynamic section\n";
    cout << strDynamicSection + "\n";
}

void ObjDumpParser::dumpVersionReferences(){
    cout << "Dumping version reference\n";
    cout << strVersionReferences + "\n";
}

void ObjDumpParser::dumpSectionsOverview(){
    cout << "Dumping section overview\n";
    cout << strSectionsOverview + "\n";
}

void ObjDumpParser::dumpSymbolTable(){
    cout << "Dumping symbol table\n";
    cout << strSymbolTable + "\n";
}

void ObjDumpParser::dumpSectionContent(unsigned int sectionNum){
    cout << "Dumping section content for section: " + to_string(sectionNum) + "\n";
    if(vectorSectionContent.size() > sectionNum){
        cout << vectorSectionContent[sectionNum] + "\n";
    } else {
        cout << "Section number " + to_string(sectionNum) + " does not exist!\n";
    }

}

void ObjDumpParser::dumpSectionContent(string section){
    cout << "Dumping section content for: " + section + "\n";
    //
}

void ObjDumpParser::dumpSectionDisassembly(unsigned int sectionNum){
    cout << "Dumping section disassembly for section: " + to_string(sectionNum) + "\n";
    if(vectorSectionDisassembly.size() > sectionNum){
        cout << vectorSectionDisassembly[sectionNum] + "\n";
    } else {
        cout << "Section number " + to_string(sectionNum) + " does not exist!\n";
    }

}

void ObjDumpParser::dumpSectionDisassembly(string section){
    cout << "Dumping section content for: " + section + "\n";
    //
}

void ObjDumpParser::process(){
    cout << "Processing!\n";

}

#include <iostream>
#include <sstream>
#include <regex>

#include "../include/ParserObj.hpp"

using namespace std;

ParserObj::ParserObj(){

}

ParserObj::~ParserObj(){

    cout << "Exiting!\n";
}



void ParserObj::parse(string filename){
    fileString = openFileToString(filename);
    fileString = preProcess(fileString);
    parseByLine(fileString);
    process();
    dumpBasicInfo();
}


void ParserObj::parseByLine(string fileString){
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
            ParserObj::strBasicInfo.append(line + "\n");
            if(regex_search(line, regexDoubleNewlineSeparator) && lineNum != 1){
                flagBasicInfo--;
                flagBasicInfoDetected++;
                cout << "1) Basic info detected!\n";
            }
        } else if(flagProgramHeader){
            ParserObj::strProgramHeader.append(line + "\n");
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

string ParserObj::preProcess(string fileString){


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

void ParserObj::dumpAll(){
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

void ParserObj::dumpBasicInfo(){
    cout << "Dumping basic info\n";
    cout << strBasicInfo + "\n";
}

void ParserObj::dumpProgramHeader(){
    cout << "Dumping program header\n";
    cout << strProgramHeader + "\n";
}

void ParserObj::dumpDynamicSection(){
    cout << "Dumping dynamic section\n";
    cout << strDynamicSection + "\n";
}

void ParserObj::dumpVersionReferences(){
    cout << "Dumping version reference\n";
    cout << strVersionReferences + "\n";
}

void ParserObj::dumpSectionsOverview(){
    cout << "Dumping section overview\n";
    cout << strSectionsOverview + "\n";
}

void ParserObj::dumpSymbolTable(){
    cout << "Dumping symbol table\n";
    cout << strSymbolTable + "\n";
}

void ParserObj::dumpSectionContent(int sectionNum){
    cout << "Dumping section content for section: " + to_string(sectionNum) + "\n";
    if(vectorSectionContent.size() > sectionNum){
        cout << vectorSectionContent[sectionNum] + "\n";
    } else {
        cout << "Section number " + to_string(sectionNum) + " does not exist!\n";
    }

}

void ParserObj::dumpSectionContent(string section){
    cout << "Dumping section content for: " + section + "\n";
    //
}

void ParserObj::dumpSectionDisassembly(int sectionNum){
    cout << "Dumping section disassembly for section: " + to_string(sectionNum) + "\n";
    if(vectorSectionDisassembly.size() > sectionNum){
        cout << vectorSectionDisassembly[sectionNum] + "\n";
    } else {
        cout << "Section number " + to_string(sectionNum) + " does not exist!\n";
    }

}

void ParserObj::dumpSectionDisassembly(string section){
    cout << "Dumping section content for: " + section + "\n";
    //
}

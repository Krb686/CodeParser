#ifndef OBJPARSER_HPP_INCLUDED
#define OBJPARSER_HPP_INCLUDED

#include "../include/Parser.hpp"
#include <string>
#include <vector>

using namespace std;

class ParserObj : public Parser {
    public:
        ParserObj();
        ParserObj(bool);
        ~ParserObj();
        void parse(string filename);

    private:
        void dumpBasicInfo();
        void dumpProgramHeader();
        void dumpDynamicSection();
        void dumpVersionReferences();
        void dumpSectionsOverview();
        void dumpSymbolTable();
        void dumpSectionContent(int sectionNum);
        void dumpSectionContent(string section);
        void dumpSectionDisassembly(int sectionNum);
        void dumpSectionDisassembly(string section);

        void dumpAll();
        void parseByChar(string filename);
        void parseByLine(string filename);

        string openFileToString(string filename);

        string preProcess(string fileString);
        void process();
        void processBasicInfo();

        string fileString;
        string flavor;

        bool debugLog;

        string strBasicInfo;
        string strProgramHeader;
        string strDynamicSection;
        string strVersionReferences;
        string strSectionsOverview;
        string strSymbolTable;
        string strSectionContent;
        string strSectionDisassembly;

        vector<string> vectorSectionContent;
        vector<string> vectorSectionDisassembly;
};

#endif // OBJPARSER_HPP_INCLUDED

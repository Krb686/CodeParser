#ifndef PARSER_HPP_INCLUDED
#define PARSER_HPP_INCLUDED

#include <string>

using namespace std;

class Parser {
    public:
        //Methods
        Parser();
        ~Parser();
    private:
    protected:
        string openFileToString(string filename);
        string fileString;
};
#endif // PARSER_HPP_INCLUDED

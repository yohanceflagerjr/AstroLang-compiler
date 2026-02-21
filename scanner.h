//scanner.h
#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <fstream>
#include <cctype>  //for isspace()
#include <string>
using namespace std;

//token structure
struct Token {
    string lexeme;
    string name;
};

struct ScannerState {
    char currentChar = '\0';
    char lookaheadChar = '\0';
    bool hasChar = false;
    bool hasLookahead = false;
};

class Scanner {
    private:
        ifstream& file;
        ScannerState& state;
        
        string getTokenName( const string& lexeme );
        char getCharacter();
        char lookaheadChar();
        void ungetCharacter();
        
        
    public:
        Scanner( ifstream& f, ScannerState& s ) : file( f ), state( s ) {}
        Token getNextToken();
        static bool findWordInFile( const string& filename, const string& word );
        Token peekNextToken();
        void goBackWord();
};

#endif
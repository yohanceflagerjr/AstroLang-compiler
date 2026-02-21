//scanner.cpp
#include "scanner.h"
#include <iostream>
#include <fstream>
#include <cctype>  //for isspace()
#include <string>
using namespace std;

//function to classify token based on lexeme
string Scanner::getTokenName( const string& lexeme ) {
    if( lexeme == "@" ) return "addToken";
    if( lexeme == "@_")  return "addAssignToken";
    if( lexeme == "@@" ) return "incToken";
    if( lexeme == "@+" ) return "posUnaryToken";
    if( lexeme == "^" ) return "subToken";
    if( lexeme == "^_" ) return "subAssignToken";
    if( lexeme == "^^" ) return "decToken";
    if( lexeme == "^-" ) return "negUnaryToken";
    if( lexeme == "#" ) return "multToken";
    if( lexeme == "#_" ) return "multAssignToken";
    if( lexeme == "%" ) return "divToken";
    if( lexeme == "%_" ) return "divAssignToken";
    if( lexeme == "*" ) return "modToken";
    if( lexeme == "*_" ) return "modAssignToken";
    if( lexeme == "__" ) return "equalToToken";
    if( lexeme == "_" ) return "assignmentToken";
    if( lexeme == "++" ) return "andToken";
    if( lexeme == "~+?" ) return "xorToken";
    if( lexeme == "~" ) return "notToken";
    if( lexeme == "?" ) return "orToken";
    if( lexeme == "[" ) return "leftBlockToken";
    if( lexeme == "]" ) return "rightBlockToken";
    if( lexeme == "$" ) return "endOfLineToken";
    if( lexeme == "<" ) return "leftSquareBracketToken";
    if( lexeme == ">" ) return "rightSquareBracketToken";
    if( lexeme == "{" ) return "leftParenthesisToken";
    if( lexeme == "}" ) return "rightParenthesisToken";
    if( lexeme == "|" ) return "greaterThanToken";
    if( lexeme == "-" ) return "lessThanToken";
    if( lexeme == "|_" ) return "greaterThanEqualToken";
    if( lexeme == "-_" ) return "lessThanEqualToken";
    if( lexeme == "..." ) return "semicolonToken";
    if( lexeme == "/" ) return "cinArrowToken";
    if( lexeme == "\\" ) return "coutArrowToken";
    if( isupper( lexeme[ 0 ] ) ) return "idToken";
    return "errorToken"; //default case
}

Token Scanner::peekNextToken() {
    ScannerState savedState = state; //save current state
    Token token = getNextToken();
    state = savedState; //restore state
    return token;
}

void Scanner::goBackWord() {
    char ch;
    while( file.tellg() > 0 ) {
        file.seekg( -1, std::ios::cur );
        file.get( ch );
        if( ch == ' ' || ch == '\n' ) {
            break;
        }
        if( file.tellg() <= 1 ) {
            file.seekg( 0, std::ios::beg );
            break;
        }
        file.seekg( -1, std::ios::cur );
    }
}


bool Scanner::findWordInFile( const string& filename, const string& word ) {
    ifstream file( "keywords.txt" );
    if( !file ) {
        cerr << "Error: Could not open file " << "keywords.txt" << endl;
        return false;
    }

    string currentWord;
    while( file >> currentWord ) { //read word by word
        if( currentWord == word ) {
            return true;
        }
    }

    return false;
}

char Scanner::getCharacter() {
    if( state.hasLookahead ) {
        state.currentChar = state.lookaheadChar;
        state.hasLookahead = false;
    } else if( file.get( state.currentChar ) ) {
        state.hasChar = true;
    } else {
        state.hasChar = false;
        return '\0';
    }
    return state.currentChar;
}

char Scanner::lookaheadChar() {
    if( !state.hasLookahead ) {
        if( file.get( state.lookaheadChar ) ) {
            state.hasLookahead = true;
        } else {
            state.lookaheadChar = '\0';
        }
    }
    return state.lookaheadChar;
}

void Scanner::ungetCharacter() {
    if( state.hasLookahead ) {
        file.putback( state.lookaheadChar );
        state.hasLookahead = false;
    } else if( state.hasChar ) {
        file.putback( state.currentChar );
        state.hasChar = false;
    }
}
    

//function to process file and identify tokens
Token Scanner::getNextToken() {
    Token token;
    string lexeme;
    char ch = getCharacter();
    int type;
    string keyword = "keywords.txt";
    //ignore spaces or newlines
    if( isspace( ch ) ) {
        do {
            ch = getCharacter();
        } while( isspace( ch ) );
    }
    
    if( ch == '\0' ) {
        token.name = "eofToken";
        token.lexeme = "";
        return token;
    }
    
    switch( ch ) {
        
        case '@': case '^': case '#': case '%': case '*': case '_': case '-' : case '|':
            lexeme += ch;
            ch = getCharacter();
            if( ch == '\n' ) {
                ungetCharacter(); //ignore the newline and break the case
                token.name = getTokenName( lexeme );
                token.lexeme = lexeme;
                return token;
            }
            if( ch == '@' || ch == '_' || ch == '+' || ch == '^' || ch == '-' ) {
                lexeme += ch;
            }
            else {
                ungetCharacter();
            }
            token.name = getTokenName( lexeme );
            token.lexeme = lexeme;
            return token;

        case '+': {
            lexeme += ch;
            ch = getCharacter();
            if( ch == '+' ) lexeme += ch;
            else ungetCharacter();
            token.name = getTokenName( lexeme );
            token.lexeme = lexeme;
            return token;
        }
        case '~': {
            lexeme += ch;
            ch = getCharacter();
            if( ch == '+' ) {
                lexeme += ch;
                ch = getCharacter();
                if( ch == '?' ) lexeme += ch;
                else ungetCharacter();
            } 
            else {
                ungetCharacter();
            }
            token.name = getTokenName( lexeme );
            token.lexeme = lexeme;
            return token;
        }
        case '?': case '[': case ']': case '$': case '<': case '>': case '{': case '}': {
            lexeme += ch;
            token.name = getTokenName( lexeme );
            token.lexeme = lexeme;
            return token;
        }
        case '!': {
            do {
                ch = getCharacter(); 
            } while( ch != '\n' && ch != '\0' );
            return getNextToken();
        }
        case ';': {
            lexeme += ch;
            ch = getCharacter();
            lexeme += ch;
            ch = getCharacter();
            if( ch == ';' ) {
                lexeme += ch;
                token.name = "characterLitToken";
                token.lexeme = lexeme;
            }
            else {
                lexeme += ch;
                token.name = "errorToken";
                token.lexeme = lexeme;
            }
            return token;
        }
        case ':': {
            bool reading = false; //tracks if we're inside `:: ... ::`
            lexeme += ch;
            ch = getCharacter();
        
            if( ch == ':' ) { //first `::` detected
                lexeme += ch;
                reading = true;
                do {
                    ch = getCharacter();
                    if( ch == ':' && reading ) {
                        lexeme += ch;
                        ch = getCharacter();
                        if( ch == ':' ) { //closing `::` detected
                            lexeme += ch; 
                            reading = false;
                            break; //stop reading
                        }
                        else {
                            //not a valid closing `::`, put back the character
                            ungetCharacter();
                        }
                    }
                    else {
                        lexeme += ch; //store characters between `:: ... ::`
                    }
        
                } while( ch != '\0' ); //stop if EOF is reached
                
                if( !reading ) {
                    token.name = "stringLitToken";
                    token.lexeme = lexeme;
                }
                else {
                    token.name = "errorToken";
                    token.lexeme = lexeme;
                }
            }
            else {
                token.name = "errorToken";
                token.lexeme = lexeme;
            }
            return token;
        }
        case '.': {
            lexeme += ch;
            ch = getCharacter();
            if( ch == '.' ) {
                lexeme += ch;
                ch = getCharacter();
                if( ch == '.' ) lexeme += ch;
                else ungetCharacter();
            } 
            else {
                ungetCharacter();
            }
            token.name = getTokenName( lexeme );
            token.lexeme = lexeme;
            return token;
        }

        case '/': {
            bool contin = true;
            char look;
            look = lookaheadChar();
            if( look == '/' ) {
                ch = getCharacter();
                if( ch == '/' ) { //ensure it's a comment starting with //
                    do {
                        ch = getCharacter();
                        //handle escape sequences (\\\)
                        if( ch == '\\' ) {
                            look = lookaheadChar();
                            if( look == '\\' )
                            {
                                ch = getCharacter();
                                if( ch == '\\' ) { //triple backslash is treated differently
                                    getCharacter();
                                    contin = false; //end the loop on triple backslash
                                }
                            }
                        }
                        
                        //don't break for newline or EOF, continue reading to the next line
                        if( ch == '\0' ) {
                            //if EOF is encountered, break out of the loop
                            contin = false;
                        }
        
                    } while( contin == true ); //keep reading until the stop condition is met (triple \ or EOF)
        
                    return getNextToken(); //skip the rest of the code, move to the next token
                }
            } 
            else {
                //not a comment, it's just a regular '/'
                ungetCharacter(); //put back the second slash
                lexeme += ch;
                token.name = getTokenName( lexeme );
                token.lexeme = lexeme;
                return token;
            }
        }
        case '\\': {
            lexeme += ch;
            token.name = getTokenName( lexeme );
            token.lexeme = lexeme;
            return token;
        }

        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z': {
            do {
                lexeme += ch;
                ch = getCharacter();
            } while( islower( ch ) );
            //give the next character back to be read in properly
            ungetCharacter();
            if( findWordInFile( keyword, lexeme ) ) {
                //keywords processing
                if( lexeme == "radius" ) {
                    token.name = "intDecToken";
                    token.lexeme = lexeme;
                    return token;
                } 
                else if( lexeme == "mass" ) {
    				token.name = "floatDecToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "signal" ) {
    				token.name = "boolDecToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "ping" ) {
    				token.name = "boolTLitToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "scramble" ) {
    				token.name = "boolFLitToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "message" ) {
    				token.name = "stringDecToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "frag" ) {
    				token.name = "charDecToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "static" ) {
    				token.name = "lagrangeDecToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "cluster" ) {
    				token.name = "arrDecToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "eternal" ) {
    				token.name = "eternalDecToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "orbit" ) {
    				token.name = "forLToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "galaxy" ) {
    				token.name = "whileToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "activate" ) {
    				token.name = "activateToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "ignite" ) {
    				token.name = "igniteToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "probe" ) {
    				token.name = "probeToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "investigate" ) {
    				token.name = "investToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "fallback" ) {
    				token.name = "fallBToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "missionControl" ) {
    				token.name = "missionControlToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "mission" ) {
    				token.name = "missionToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "catchall" ) {
    				token.name = "catchallToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "halt" ) {
    				token.name = "haltToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "linker" ) {
    				token.name = "linkLitToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "ground" ) {
    				token.name = "groundLitToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "coordinate" ) {
    				token.name = "cordDecToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "launchfunction" ) {
    				token.name = "launchFunDecToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "blackhole" ) {
    				token.name = "voidLitToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "trigger" ) {
    				token.name = "triggerDecToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "messageend" ) {
    				token.name = "messageendToken";
                    token.lexeme = lexeme;
    			}
    			else if( lexeme == "reenter" ) {
    				token.name = "reenterToken";
                    token.lexeme = lexeme;
    			}
    			else {
					token.name = "transmitToken";
                    token.lexeme = lexeme;
				}
            }
            else {
                token.name = "errorToken";
                token.lexeme = lexeme;
            }
            return token;
            
            //idToken recognition
    	case 'A':
    	case 'B':
    	case 'C':
    	case 'D':
    	case 'E':
    	case 'F':
    	case 'G':
    	case 'H':
    	case 'I':
    	case 'J':
    	case 'K':
    	case 'L':
    	case 'M':
    	case 'N':
    	case 'O':
    	case 'P':
    	case 'Q':
    	case 'R':
    	case 'S':
    	case 'T':
    	case 'U':
    	case 'V':
    	case 'W':
    	case 'X':
    	case 'Y':
    	case 'Z': {
		    do {
                lexeme += ch;
                ch = getCharacter();
            } while( ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || isdigit( ch ) );
            //give the next character back to be read in properly
            ungetCharacter();
            token.name = getTokenName( lexeme );
            token.lexeme = lexeme;
            return token;
		}
		//number literal regconition
    	case '0':
    	case '1':
    	case '2':
    	case '3':
    	case '4':
    	case '5':
    	case '6':
    	case '7':
    	case '8':
    	case '9': {
            do {
                lexeme += ch;
                ch = getCharacter();
            } while( isdigit( ch ) );
        
            //check if it's a hexadecimal number (e.g., 0x...)
            if( lexeme == "0" && ch == 'x' ) {
                lexeme += ch;
                ch = getCharacter();
                
                while( ( ch >= 'A' && ch <= 'F' ) || isdigit( ch ) ) {
                    lexeme += ch;
                    ch = getCharacter();
                }
                
                ungetCharacter(); //put back the non-hex character
                token.name = "hexLitToken";
                token.lexeme = lexeme;
                return token;
            }
        
            //check if it's a floating-point number (e.g., 123.456)
            if( ch == '.' ) {
                //need to use a lookahead function to be able to unget both dots
                char look = lookaheadChar();
                if( look == '.' ) {
                    ungetCharacter();
                    ungetCharacter();
                    token.name = "intLitToken";
                    token.lexeme = lexeme;
                    return token;
                }
                lexeme += ch;
                ungetCharacter();
                ch = getCharacter();
                if( !isdigit( ch ) ) {  //if no digits follow, it's an error
                    ungetCharacter();  //put back the incorrect character
                    token.name = "errorToken";
                    token.lexeme = lexeme;
                    return token;
                }
                //read decimal part
                do {
                    lexeme += ch;
                    ch = getCharacter();
                } while( isdigit( ch ) );
        
                ungetCharacter(); //put back non-numeric character
                token.name = "realLitToken";
                token.lexeme = lexeme;
                return token;
            }
        
            //regular integer case
            ungetCharacter();  //put back the non-digit character
            token.name = "intLitToken";
            token.lexeme = lexeme;
            return token;
        } 
        default:
            token.name = "errorToken";
            token.lexeme = string( 1, ch );
            return token;
        }
    }
    return token;
}
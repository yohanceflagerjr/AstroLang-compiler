//parser.h
#include "scanner.h"
#include "symbol_table.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <string>
#include <ctype.h>
//#include "studentInterpreter.h"
using namespace std;

//external declarations
extern unsigned char M[];
extern int curr_addr;
extern bool DEBUG;

//opcode constants
extern const unsigned char ADD, SUB, MUL, DIV, MOD;
extern const unsigned char LDM4, LDD4, STD4, JMPD, BYES, BNO, HALT;

//emit helpers
void emit1( int opcode );
void emit2( int value );
void emit4( int value );
void emitIntVal( const char* z );
void emitRealVal( const char* z );
void backPatch2( int value, int addr );


unsigned int unreal( float x ) {
    float f = x;
    int *i;
    i = (int *) &f;
    return *i;
}


void emit1( int opcode ) {
    M[ curr_addr++ ] = opcode & 0xFF;
    cout.setf( ios::hex );
    if( DEBUG ) {
        printf( "\n emit1 at %d:%x", curr_addr,M[ curr_addr - 1 ] );
    }
}
void emit2( int value ) {
    M[ curr_addr++ ] = value & 0xFF;
    M[ curr_addr++ ] = (value & 0xFF00) >> 8;
    if( DEBUG ) {
        printf( "\n emit2 at %d:%x", curr_addr - 1, M[ curr_addr - 1 ] );
    }
}
void emit4( int value ) {
    M[ curr_addr++ ] = value & 0xFF;
    M[ curr_addr++ ] = (value & 0xFF00) >> 8;
    M[ curr_addr++ ] = (value & 0xFF0000) >> 16;
    M[ curr_addr++ ] = (value & 0xFF000000) >> 24;
    if( DEBUG ) {
        printf( "\n emit4 at %x", M[ curr_addr - 1 ] );
    }
}
void emitIntVal( char * z ) {
    emit1( LDM4) ;
    emit4( atoi( z ) );
}
void emitRealVal( char * z ) {
    float zf;
    zf = atof( z );
    emit1( LDM4 );
    emit4( unreal( zf ) );
}
void backPatch1( int opcode, int addr ) {
    M[ addr ] = opcode & 0xFF;
}
void backPatch2( int value, int addr ) {
    M[ addr ] = value & 0xFF;
    M[ addr + 1 ] = (value & 0xFF00) >> 8;
}
void backPatch4( int value, int addr ) {
    M[ addr ] = value & 0xFF;
    M[ addr + 1 ] = (value & 0xFF00) >> 8;
    M[ addr + 2 ] = (value & 0xFF0000) >> 16;
    M[ addr + 3 ] = (value & 0xFF000000) >> 24;
}

class Parser {
    private:
        Scanner& scanner;
        SymbolTable& symtab;
        Token currentToken;

        void advance() {
            currentToken = scanner.getNextToken();
        }

        void error( const string& message ) {
            cout << "Parser error: " << message << " at lexeme: " << currentToken.lexeme << endl;
            exit( 1 );
        }

        bool match( const string& tokenName ) {
            if( currentToken.name == tokenName ) {
                advance();
                return true;
            } else {
                error( "Expected " + tokenName + ", got " + currentToken.name );
                return false;
            }
        }

        bool isTypeSpecifier() {
            return( currentToken.name == "intDecToken" || currentToken.name == "floatDecToken" ||
                    currentToken.name == "boolDecToken" || currentToken.name == "stringDecToken" ||
                    currentToken.name == "charDecToken" || currentToken.name == "eternalDecToken" ||
                    currentToken.name == "arrDecToken" || currentToken.name == "voidLitToken" );
        }

        bool isStatementStarter() {
            return( currentToken.name == "forLToken" || currentToken.name == "whileToken" ||
                    currentToken.name == "activateToken" || currentToken.name == "probeToken" ||
                    currentToken.name == "investToken" || currentToken.name == "fallBToken" ||
                    currentToken.name == "missionControlToken" || currentToken.name == "catchallToken" ||
                    currentToken.name == "reenterToken" || currentToken.name == "haltToken" || isTypeSpecifier() ||isExpressionStarter() );
        }

        bool isExpressionStarter() {
            return( currentToken.name == "idToken" || currentToken.name == "boolTLitToken" ||
                    currentToken.name == "boolFLitToken" || currentToken.name == "groundLitToken" ||
                    currentToken.name == "linkLitToken" || currentToken.name == "posUnaryToken" ||
                    currentToken.name == "negUnaryToken" || currentToken.name == "notToken" ||
                    currentToken.name == "leftParenthesisToken" );
        }
        string mapTypeName( const string& scannerType ) {
            if( scannerType == "radius" ) return "INT";
            if( scannerType == "mass" ) return "REAL";
            if( scannerType == "signal" ) return "YN";
            if( scannerType == "message" ) return "STR";
            if( scannerType == "frag" ) return "CHAR";
            if( scannerType == "static" ) return "PTR";
            if( scannerType == "eternal" ) return "NONE";
            if( scannerType == "cluster" ) return "PTR"; //assuming array is pointer type
            if( scannerType == "blackhole" ) return "VOID";
            return "INT"; // default
        }

    public:
        Parser( Scanner& sc, SymbolTable& st ) : scanner( sc ), symtab( st ) {
            advance(); //load first token
        }

        void compile() {
            parseProgram();
        }

    private:
        void parseProgram() 
        {
            //showing parsing
            //cout << "Program(" << endl;
            
            while( isTypeSpecifier() || currentToken.name == "launchFunDecToken" )
            {
                //check if next token is Controlroom(peek ahead)
                Token nextToken = scanner.peekNextToken();
                //scanner.goBackWord();
                
                if( nextToken.lexeme == "Controlroom" ) {
                    parseTypeSpecifier();
                    parseControlroomFunction();
                } 
                else if( currentToken.name == "launchFunDecToken" ){
                    parseFunctionDeclaration();
                }
                else {
                    parseDeclarations();
                }
            }
            
            //showing parse return
            //cout << ")" << endl;
        }

        void parseDeclarations() {
            //showing parsing
            //cout << "Declarations(" << endl;
            
            if( currentToken.name == "leftBlockToken" ) {
                parseCodeBlock();
            } else if( isTypeSpecifier() ) {
                parseVariableDeclaration();
            } else {
                parseFunctionDeclaration();
            }
            
            //showing parse return
            //cout << ")" << endl;
        }

        void parseVariableDeclaration() {
            //showing parsing
            //cout << "VariableDeclaration(" << endl;
            
            string scannerType = currentToken.lexeme;
            string type = mapTypeName( scannerType );
            parseTypeSpecifier();
            parsePointer();
            
            string id = currentToken.lexeme;
            match( "idToken" );
           
            //check if symbol exists in current scope
            if( symtab.find_symbol( id ) == -1 || 
                ( symtab.find_symbol( id ) != -1 && symtab.find_symbol( id ) < symtab.getCurrentScopeStart() ) ) {
                Symbol sym = SymbolTable::create_symbol( id, type, "VAR" );
                symtab.add_symbol( sym );
            } else {
                error( "Duplicate variable declaration: " + id );
            }
        
            if( currentToken.name == "assignmentToken" ) {
                match( "assignmentToken" );
                parseExpression();
                int addr = symtab.find_symbol( id );
                if( addr != -1 ) {
                    emit1( STD4 );
                    //may not be putting the correct address in
                    //may need to add the last address location to it
                    emit2( addr );
                }
            }
            match( "semicolonToken" );
            
            //showing parse return
            //cout << ")" << endl;
        }

        void parseFunctionDeclaration() {
            //showing parsing
            //cout << "FunctionDeclaration(" << endl;
            
            scanner.goBackWord();
            Token nextToken = scanner.peekNextToken();
            string scannerType = nextToken.lexeme;
            //cout << currentToken.lexeme;
            string type = mapTypeName( scannerType );
            // this worked cout << type << endl;
            // this worked cout << scannerType << endl;
            //Token nextToken = scanner.peekNextToken();
            //scanner.goBackWord();
            //parseTypeSpecifier();
            //parsePointer();
            string id = currentToken.lexeme;
            
            //check if valid name in symtab
            
            if( id == "Controlroom" ) {
                parseTypeSpecifier();
                parseControlroomFunction();
            } else {
                parseRegularFunction( type, id );
            }
            
            //showing parse return
            //cout << ")" << endl;
        }
        
        void parseControlroomFunction() {
            //showing parsing
            //cout << "Controlroom(" << endl;
            
            //match( "idToken" ); //match "Controlroom"
            
            //set this where main beings
            backPatch2( curr_addr, 0x09 );
            
            // Add to symbol table as special main function
            if( symtab.find_symbol( "Controlroom" ) == -1 ) {
                Symbol sym = SymbolTable::create_symbol( "Controlroom", "INT", "FUN") ;
                sym.scope = 0; //global scope
                symtab.add_symbol( sym );
                
                //need to emit the functions address to memory
            }
    
            match( "leftParenthesisToken" );
            match( "rightParenthesisToken" );
            
            //special handling for Controlroom if needed
            if( currentToken.name == "eternalToken" ) {
                match( "eternalToken" ); //if Controlroom can have eternal modifier
            }
    
            parseCodeBlock();
            
            //clean up function scope
            //symtab.del_current_scope();
            
            //generate special code for Controlroom if needed
            emit1( HALT ); // Ensure program ends after Controlroom
            backPatch2( curr_addr, 0x01 );
            backPatch2( curr_addr, 0x05 );
            
            //showing parse return
            //cout << ")" << endl;
        }
        
        void parseRegularFunction( const string& type, const string& id ) {
            //showing parsing
            //cout << "Regular Function(" << endl;
            
            // this worked cout << currentToken.name << endl;
            //Token nextToken = scanner.peekNextToken();
            //cout << nextToken.name << endl;
            match( "launchFunDecToken" );
            //scanner.goBackWord();
            //this worked cout << currentToken.name << endl;
            //cout << currentToken.lexeme;
            //cout << type;
            //scanner.goBackWord();
            //cout << currentToken.name;
            //cout << currentToken.lexeme;

            //parseTypeSpecifier();
            string newID = currentToken.lexeme;
            match( "idToken" );
            match( "leftParenthesisToken" );
            
            // add function to symbol table
            if( symtab.find_symbol( id ) == -1 ) {
                Symbol sym = SymbolTable::create_symbol( newID, type, "FUN" );
                sym.scope++;
                symtab.add_symbol( sym );
            }
    
            parseParameterList();
            match( "rightParenthesisToken" );
            
            if( currentToken.name == "eternalToken" ) {
                match( "eternalToken" );
            }
    
            parseFunctionBody();
            
            //clean up function scope
            //symtab.del_current_scope();
            
            //showing parsing return
            //cout << ")" << endl;
        }

        void parseFunctionHeader() {
            //showing parsing
            //cout << "FunctionHeader(" << endl;
            
            parseTypeSpecifier();
            parsePointer();
            match( "idToken" );
            match( "leftParenthesisToken" );
            parseParameterList();
            match( "rightParenthesisToken" );
            if( currentToken.name == "eternalToken" ) {
                match( "eternalToken" );
            }
            
            //showing parse return
            //cout << ")" << endl;
        }

        void parseFunctionBody() {
            //showing parsing
            //cout << "FunctionBody(" << endl;
            
            parseCodeBlock();
            
            //showing parse return
            //cout << ")" << endl;
        }

        void parseCodeBlock() {
            //showing parsing
            //cout << "CodeBlock(" << endl;
    
            if( currentToken.name == "leftBlockToken" ) {
                match( "leftBlockToken" );
                
                while( currentToken.name != "rightBlockToken" ) {
                    if( isTypeSpecifier() ) {
                        parseVariableDeclaration();
                    } 
                    else if( isStatementStarter() ) {
                        parseStatement();
                    }
                    else if( currentToken.name == "idToken" ) {
                        //handle potential assignment to existing variable
                        Token nextToken = scanner.peekNextToken();
                        if( nextToken.name == "assignmentToken" ) {
                            //this is an assignment to existing variable
                            string id = currentToken.lexeme;
                            int addr = symtab.find_symbol( id );
                            if( addr == -1 ) {
                                error( "Undeclared variable: " + id );
                            }
                            match( "idToken" );
                            match( "assignmentToken" );
                            parseExpression();
                            emit1( STD4 );
                            emit2( addr );
                            match( "semicolonToken" );
                        } else {
                            //not an assignment, proceed with normal expression
                            parseExpressionStatement();
                        }
                    }
                    //add explicit handling for reenterToken
                    else if( currentToken.name == "reenterToken" ) {
                        parseJumpStatement();
                    }
                    //else if( currentToken.name == "transmitToken" ) {
                        //parseOutputStatement();
                    //}
                    //else if( currentToken.name == "scanToken" ) {
                        //parseInputStatement();
                    //}
                    else {
                        error( "Unexpected token in code block: " + currentToken.lexeme );
                    }
                    //cout << currentToken.name;
                }
                
                match( "rightBlockToken" );
            }
            
            //showing parse return
            //cout << ")" << endl;
        }

        void parseStatement() {
            //showing parsing
            //cout << "Statement(" << endl;
            
            if( isStatementStarter() ) {
                parseStatementList();
                //parseStatement();
            }
            
            //showing parse return
            //cout << ")" << endl;
        }

        void parseStatementList() {
            //showing parsing
            //cout << "StatementList(" << endl;
            
            if( isExpressionStarter() ) {
                parseExpressionStatement();
            } else if( currentToken.name == "forLToken" || currentToken.name == "whileToken" || currentToken.name == "activateToken" ) {
                parseLoopStatement();
            } else if( currentToken.name == "probeToken" || currentToken.name == "investToken" || currentToken.name == "fallBToken" ) {
                parseDecisionStatement();
            } else if( currentToken.name == "missionControlToken" || currentToken.name == "catchallToken" ) {
                parseCaseStatement();
            } else if( currentToken.name == "reenterToken" || currentToken.name == "haltToken" ) {
                parseJumpStatement();
            }
            
            //showing parse return
            //cout << ")" << endl;
        }

        void parseExpressionStatement() {
            //showing parsing
            //cout << "ExpressionStatement(" << endl;
    
            if( currentToken.name == "transmitToken" ) {  // cout
                match("transmitToken");
                //parse message parts
                while( currentToken.name != "messageendToken" ) {
                    if( currentToken.name == "coutArrowToken" ) {  //handle "\"
                        match("coutArrowToken");
                        //special output formatting
                    }
                    else {
                        parseExpression();
                    }
                }
                match( "messageendToken" );  // endl
                match( "semicolonToken" );
            }
            else if( currentToken.name == "reenterToken" ) {  // return
                match( "reenterToken" );
                parseExpression();
                match( "semicolonToken" );
            }
            else {
                parseExpression();
                //cout << currentToken.name;
                match( "semicolonToken" );  //your "..." replaces semicolons
            }
            
            //showing parse return
            //cout << ")" << endl;
        }

        void parseLoopStatement() {
            //showing parsing
            //cout << "LoopStatement(" << endl;
            
            if( currentToken.name == "forLToken" ) {
                match( "forLToken" );
                match( "leftParenthesisToken" );
                
                //initialization
                if( currentToken.name == "intDecToken" ) {
                    parseVariableDeclaration();  //handle "radius Index _ 0..."
                }
                
                //condition
                parseExpression();
                match( "semicolonToken" );
                
                //increment
                if( currentToken.name == "idToken" ) {
                    string id = currentToken.lexeme;
                    match( "idToken" );
                    if( currentToken.name == "incToken" ) {  //handle "@@"
                        match( "incToken" );
                    }
                    //match( "semicolonToken" );
                }
                match( "rightParenthesisToken" );
                parseCodeBlock();
            }
            else if( currentToken.name == "whileToken" ) {
                match( "whileToken" );
                match( "leftParenthesisToken" );
                parseExpression();
                match( "rightParenthesisToken" );
                parseCodeBlock();
            }
            else if( currentToken.name == "activateToken" ) {
                match( "activateToken" );
                //match( "leftBlockToken" );
                //cout << currentToken.name;
                parseCodeBlock();
                //match( "rightBlockToken" );
                match( "igniteToken" );
                match( "leftParenthesisToken" );
                parseExpression();
                match( "rightParenthesisToken" );
                match( "semicolonToken" );
            }
            //showing parse return
            //cout << ")" << endl;
        }

        void parseDecisionStatement() {
            //showing parsing
            //cout << "DecisionStatement(" << endl;
            
            if( currentToken.name == "probeToken" ) {
                match( "probeToken" );
                match( "leftParenthesisToken" );
                parseExpression();
                match( "rightParenthesisToken" );
                parseCodeBlock();
                
                //optional else-if/else
                while( currentToken.name == "investToken" || currentToken.name == "fallBToken" ) {
                    if( currentToken.name == "investToken" ) {  //else-if
                        match( "investToken" );
                        match( "leftParenthesisToken" );
                        parseExpression();
                        match( "rightParenthesisToken" );
                        parseCodeBlock();
                    }
                    else {  // else
                        match( "fallBToken" );
                        parseCodeBlock();
                    }
                }
            }
            
            //showing parse return
            //cout << ")" << endl;
        }

        void parseCaseStatement() {
            //showing parsing
            //cout << "CaseStatement(" << endl;
            
            if( currentToken.name == "missionControlToken" ) {
                match( "missionControlToken" );
                parseExpression();
                match( "colonToken" );
                match( "leftBlockToken" );
                parseStatement();
                match( "rightBlockToken" );
            }
            else if( currentToken.name == "catchallToken" ) {
                match( "catchallToken" );
                match( "colonToken" );
                match( "leftBlockToken" );
                parseCodeBlock();
                match( "rightBlockToken" );
            }
            //showing parse return
            //cout << ")" << endl;
        }

        void parseJumpStatement() {
            //showing parsing
            //cout << "JumpStatement(" << endl;
            
            if( currentToken.name == "reenterToken" ) {
                match( "reenterToken" );
                //match("leftSquareBracketToken");
                //parseCodeBlock();
                //match("rightSquareBracketToken");
                //match("semicolonToken");
                //instead, parse the return expression if it exists
                if( currentToken.name != "semicolonToken" ) {
                    parseExpression();
                }
                match( "semicolonToken" );
                
                // Generate return opcode
                emit1( 0x21 ); //example return opcode
            }
            else if( currentToken.name == "haltToken" ) {
                match( "haltToken" );
                match( "semicolonToken" );
            }
            
            //showing parse return
            //cout << ")" << endl;
        }

        void parseExpression() {
            //showing parsing
            //cout << "Expression(" << endl;
            
            parseAssignmentExpr();
            
            //showing parse return
            //cout << ")" << endl;
        }

        void parseAssignmentExpr() {
            //showing parsing
            //cout << "AssignmentExpression(" << endl;
            
            parseLogicalORExpr();
            if( currentToken.name == "assignmentToken" ) {
                match( "assignmentToken" );
                parseLogicalORExpr();
            }
            
            //showing parse return
            //cout << ")" << endl;
        }

        void parseLogicalORExpr() {
            //showing parsing
            //cout << "LogicalORExpression(" << endl;
            
            parseLogicalANDExpr();
            while( currentToken.name == "orToken" ) {
                match( "orToken" );
                parseLogicalANDExpr();
            }
            
            //showing parse return
            //cout << ")" << endl;
        }

        void parseLogicalANDExpr() {
            //showing parsing
            //cout << "LogicalANDExpression(" << endl;
            
            parseEqualExpr();
            while( currentToken.name == "andToken" ) {
                match( "andToken" );
                parseEqualExpr();
            }
            
            //showing parse return
            //cout << ")" << endl;
        }

        void parseEqualExpr() {
            //showing parsing
            //cout << "EqualExpression(" << endl;

            parseRelationalExpr();
            while( currentToken.name == "equalToToken" ) {
                match( "equalToToken" );
                parseRelationalExpr();
            }

            //showing parse return
            //cout << ")" << endl;
        }

        void parseRelationalExpr() {
            //showing parsing
            //cout << "RelationalExpression(" << endl;

            parseAddExpr();
            while(  currentToken.name == "greaterThanToken" || currentToken.name == "lessThanToken" || currentToken.name == "greaterThanEqualToken" || currentToken.name == "lessThanEqualToken" ) {
                string op = currentToken.name;
                advance();
                parseAddExpr();
            }

            //showing parse return
            //cout << ")" << endl;
        }

        void parseAddExpr() {
            //showing parsing
            //cout << "AddExpression(" << endl;

            parseMultExpr();
            while( currentToken.name == "addToken" || currentToken.name == "subToken" ) {
                string op = currentToken.name;
                advance();
                parseMultExpr();
                if( op == "addToken" ) emit1( ADD );
                else emit1( SUB );
            }

            //showing parse return
            //cout << ")" << endl;
        }

        void parseMultExpr() {
            //showing parsing
            //cout << "MultExpression(" << endl;

            parseUnaryExpr();
            while( currentToken.name == "multToken" || currentToken.name == "divToken" || currentToken.name == "modToken" ) {
                string op = currentToken.name;
                advance();
                parseUnaryExpr();
                if( op == "multToken" ) emit1(  MUL);
                else if( op == "divToken" ) emit1( DIV );
                else emit1( MOD );
            }

            //showing parse return
            //cout << ")" << endl;
        }

        void parseUnaryExpr() {
            //showing parsing
            //cout << "UnaryExpression(" << endl;

            if( currentToken.name == "posUnaryToken" || currentToken.name == "negUnaryToken" || currentToken.name == "notToken" ) {
                advance();
                parseUnaryExpr();
            } else {
                parsePostfixExpr();
            }

            //showing parse return
            //cout << ")" << endl;
        }

        void parsePostfixExpr() {
            //showing parsing
            //cout << "PostfixExpression(" << endl;
    
            //first parse the primary expression (operand for postfix operators)
            parsePrimaryExpr();
            
            //check for postfix operators (++, --, function calls, array access, etc.)
            if( currentToken.name == "incToken" || currentToken.name == "decToken" ) {
                //cout << currentToken.name; //print the operator (e.g., "++" or "--")
                advance(); //move past the operator
            }
            //add other postfix cases (e.g., function calls, array subscript) here
            
            //showing parse return
            //cout << ")" << endl;
        }

        void parsePrimaryExpr() {
            //showing parsing
            //cout << "PrimaryExpression(" << endl;
        
            if( currentToken.name == "boolTLitToken" || currentToken.name == "boolFLitToken" || 
                currentToken.name == "idToken" || currentToken.name == "intLitToken" ||
                currentToken.name == "floatLitToken" || currentToken.name == "stringLitToken" ||
                currentToken.name == "charLitToken" ) {
                //handle identifiers
                if( currentToken.name == "idToken" ) {
                    string id = currentToken.lexeme;
                    int addr = symtab.find_symbol( id );
                    if( addr == -1 ) {
                        error( "Undeclared identifier: " + id );
                    }
                    emit1( LDD4 );  //load variable value
                    emit2( addr );  //address of variable
                }
                //handle numeric literals
                else if( currentToken.name == "intLitToken" ) {
                    emitIntVal( (char*)currentToken.lexeme.c_str() );
                } else if( currentToken.name == "floatLitToken" ) {
                    emitRealVal( (char*)currentToken.lexeme.c_str() );
                }
                //handle character literals
                else if( currentToken.name == "charLitToken" ) {
                    //assuming the lexeme includes the single quotes, e.g., ";a;"
                    char charValue = currentToken.lexeme[ 1 ];  // Get the character between quotes
                    //NEED TO FIX THIS emit1(LDD4);  //load character constant
                    emit1( static_cast<int>(charValue) );  //ASCII value of the character
                }
                advance();
            } else if( currentToken.name == "leftParenthesisToken" ) {
                match( "leftParenthesisToken" );
                parseAssignmentExpr();
                match( "rightParenthesisToken" );
            } else {
                error( "Invalid primary expression" );
            }
        }

        void parseTypeSpecifier() {
            //showing parsing
            //cout << "TypeSpecifier(" << endl;

            if( currentToken.name == "lagrangeDecToken" || currentToken.name == "eternalDecToken" ) {
                advance();
            }
            if( isTypeSpecifier() ) {
                advance();
            } else {
                error( "Expected type specifier" );
            }

            //showing parse return
            //cout << ")" << endl;
        }

        void parsePointer() {
            //showing parsing
            //cout << "Pointer(" << endl;

            if( currentToken.name == "cordDecToken" ) {
                advance();
            }

            //showing parse return
            //cout << ")" << endl;
        }

        void parseParameterList() {
            //showing parsing
            //cout << "ParameterList(" << endl;

            if( isTypeSpecifier() ) {
                parseParameter();
                while( currentToken.name == "commaToken" ) {
                    match( "commaToken" );
                    parseParameter();
                }
            }

            //showing parse return
            //cout << ")" << endl;
        }

        void parseParameter() {
            //showing parsing
            //cout << "Parameter(" << endl;
            
            string scannerType = currentToken.lexeme;
            string type = mapTypeName( scannerType );
            parseTypeSpecifier();
            parsePointer();
            string id = currentToken.lexeme;
            match( "idToken" );
        
            //add parameter to symbol table
            if( symtab.find_symbol( id ) == -1 ) {
                Symbol sym = SymbolTable::create_symbol( id, type, "PARAM" );
                symtab.add_symbol( sym );
            } else {
                error( "Duplicate parameter name: " + id );
            }
        
            //showing parse return
            //cout << ")" << endl;
        }
        void parseOutputStatement() {
            //cout << "OutputStatement(" << endl;
            //match("transmitToken");
            
            //handle output items separated by arrows (<< equivalents)
            do {
                if( currentToken.name == "coutArrowToken" ) {
                    match( "coutArrowToken" );
                    //handle special formatting if needed
                }
                parseExpression();
                
                //generate output code
                emit1( 0x21 ); //example: output opcode
            } while( currentToken.name == "coutArrowToken" );
            
            match( "messageendToken" );
            match( "semicolonToken" );
            
            //showing parsing return
            //cout << ")" << endl;
    }
    
    void parseInputStatement() {
        //showing parsing
        //cout << "InputStatement(" << endl;
        
        match( "scanToken" );
        
        //handle input items separated by arrows (>> equivalents)
        do {
            if( currentToken.name == "cinArrowToken" ) {
                match( "cinArrowToken" );
            }
            
            //should be an lvalue (variable)
            if( currentToken.name != "idToken" ) {
                error( "Expected variable for input" );
            }
            
            //lookup variable in symbol table
            string varName = currentToken.lexeme;
            int addr = symtab.find_symbol( varName );
            if( addr == -1 ) {
                error( "Undeclared variable: " + varName );
            }
            
            match( "idToken" );
            
            //generate input code
            emit1(0x22);        //example: input opcode
            emit2(addr);        //variable address
            
        } while( currentToken.name == "cinArrowToken" );
        
        match( "semicolonToken" );
        
        //showing parse return
        //cout << ")" << endl;
    }
};
//main.cpp
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <iomanip>
#include <cctype>
#include "scanner.h"
#include "parser.h"
#include "symbol_table.h"
//#include "studentInterpreter.h"
using namespace std;

//op-codes
const unsigned char INC  = 0x01;
const unsigned char DEC  = 0x02;
const unsigned char ADD  = 0x03;
const unsigned char SUB  = 0x04;
const unsigned char MUL  = 0x05;
const unsigned char DIV  = 0x06;
const unsigned char MOD  = 0x07;
const unsigned char NEG  = 0x08;

const unsigned char ADDF = 0x11;
const unsigned char SUBF = 0x12;
const unsigned char MULF = 0x13;
const unsigned char DIVF = 0x14;
const unsigned char MODF = 0x15;
const unsigned char NEGF = 0x16;
const unsigned char NOT  = 0x17;
const unsigned char AND  = 0x18;
const unsigned char OR   = 0x19;

const unsigned char XOR  = 0x20;
const unsigned char SHR  = 0x21;
const unsigned char SHL  = 0x22;
const unsigned char SHRA = 0x23;
const unsigned char ROR  = 0x25;
const unsigned char ROL  = 0x26;
const unsigned char HALT = 0x27;
const unsigned char DER  = 0x28;

const unsigned char TPS  = 0x31;
const unsigned char TNG  = 0x32;
const unsigned char TZE  = 0x33;
const unsigned char TNP  = 0x34;
const unsigned char TNN  = 0x35;
const unsigned char TNZ  = 0x36;

const unsigned char TPSF = 0x41;
const unsigned char TNGF = 0x42;
const unsigned char TZEF = 0x43;
const unsigned char TNPF = 0x44;
const unsigned char TNNF = 0x45;
const unsigned char TNZF = 0x46;

const unsigned char PUSL = 0x50;
const unsigned char PUSG = 0x51;
const unsigned char PUSP = 0x52;
const unsigned char PUSS = 0x53;
const unsigned char POPL = 0x54;
const unsigned char POPG = 0x55;
const unsigned char POPP = 0x56;
const unsigned char POPS = 0x57;
const unsigned char INS  = 0x58;
const unsigned char OTS  = 0x59;

const unsigned char BYES  = 0x60;
const unsigned char BNO   = 0x61;
const unsigned char JMP   = 0x62;
const unsigned char JMPD  = 0x63;
const unsigned char JMPI  = 0x64;
const unsigned char JMPR  = 0x65;

const unsigned char LDD1  = 0x70;
const unsigned char LDDN1 = 0x71;
const unsigned char LDI1  = 0x72;
const unsigned char LDIN1 = 0x73;
const unsigned char LDR1  = 0x74;
const unsigned char LDRN1 = 0x75;
const unsigned char LDM1  = 0x76;
const unsigned char LDLR1 = 0x79;

const unsigned char LDLN1 = 0x80;
const unsigned char LDGR1 = 0x81;
const unsigned char LDGN1 = 0x82;
const unsigned char STD1  = 0x83;
const unsigned char STDN1 = 0x84;
const unsigned char STI1  = 0x85;
const unsigned char STIN1 = 0x86;
const unsigned char STR1  = 0x87;
const unsigned char STRN1 = 0x88;

const unsigned char STLR1 = 0x92;
const unsigned char STLN1 = 0x93;
const unsigned char STGR1 = 0x94;
const unsigned char STGN1 = 0x95;

const unsigned char LDD2  = 0xA0;
const unsigned char LDDN2 = 0xA1;
const unsigned char LDI2  = 0xA2;
const unsigned char LDIN2 = 0xA3;
const unsigned char LDR2  = 0xA4;
const unsigned char LDRN2 = 0xA5;
const unsigned char LDM2  = 0xA6;
const unsigned char LDLR2 = 0xA7;

const unsigned char LDLN2 = 0xB0;
const unsigned char LDGR2 = 0xB1;
const unsigned char LDGN2 = 0xB2;
const unsigned char STD2  = 0xB3;
const unsigned char STDN2 = 0xB4;
const unsigned char STI2  = 0xB5;
const unsigned char STIN2 = 0xB6;
const unsigned char STR2  = 0xB7;
const unsigned char STRN2 = 0xB8;

const unsigned char STLR2 = 0xC2;
const unsigned char STLN2 = 0xC3;
const unsigned char STGR2 = 0xC4;
const unsigned char STGN2 = 0xC5;

const unsigned char LDD4  = 0xD0;
const unsigned char LDDN4 = 0xD1;
const unsigned char LDI4  = 0xD2;
const unsigned char LDIN4 = 0xD3;
const unsigned char LDR4  = 0xD4;
const unsigned char LDRN4 = 0xD5;
const unsigned char LDM4  = 0xD6;
const unsigned char LDLR4 = 0xD9;

const unsigned char LDLN4 = 0xE0;
const unsigned char LDGR4 = 0xE1;
const unsigned char LDGN4 = 0xE2;
const unsigned char STD4  = 0xE3;
const unsigned char STDN4 = 0xE4;
const unsigned char STI4  = 0xE5;
const unsigned char STIN4 = 0xE6;
const unsigned char STR4  = 0xE7;
const unsigned char STRN4 = 0xE8;

const unsigned char STLR4 = 0xF2;
const unsigned char STLN4 = 0xF3;
const unsigned char STGR4 = 0xF4;
const unsigned char STGN4 = 0xF5;
const unsigned char NOOP  = 0xFF;

const unsigned char DSTK  = 0xF6;

const unsigned char DROP  = 0x47;
const unsigned char FLT	  = 0x48;
const unsigned char SWAP  = 0x49;
const unsigned char OUTC  = 0x4A;
const unsigned char OUTI  = 0x4B;
const unsigned char OUTR  = 0x4C;
const unsigned char COPY  = 0x1F;
const unsigned char DSTCK = 0x1B;

const unsigned char INPUTC = 0x5A;
const unsigned char INPUTI = 0x5B;
const unsigned char INPUTR = 0x5C;

//GLOBALS for interpreter
unsigned char M[65536] = {};  //64KB memory
int curr_addr = 0;
bool DEBUG = true;  //set true to print emits


//read to txt
void writeMachineCodeToFile( const char* filename ) {
    ofstream out( filename, ios::binary );
    if( !out ) {
        cerr << "Could not open output file!" << endl;
        return;
    }
    out.write( reinterpret_cast<char*>( M ), curr_addr );
    out.close();
}

//MAIN DRIVER
int main() {
    emit1( LDM2 );
    emit2( 0x00 );
    emit1( POPG );
    emit1( LDM2 );
    emit2( 0x00 );
    emit1( POPL );
    emit1( LDM2 );
    emit2( 0x00 );
    emit1( POPP );
    
    cout << endl;
    
    cout << "=== COMPILER DRIVER START ===" << endl;

    ifstream inputFile( "example.AST" ); //filename
    if( !inputFile ) {
        cerr << "Error: Could not open source file!" << endl;
        return 1;
    }

    ScannerState state;
    Scanner Scanner( inputFile, state );
    SymbolTable symtab;
    Parser parser( Scanner, symtab );

    try {
        parser.compile(); //triggers Scanner + Parser + CodeGen
        cout << "\n=== Parsing and CodeGen completed successfully ===" << endl;
    } catch( const exception& e ) {
        cerr << "\nCompiler Error: " << e.what() << endl;
        return 1;
    }

    inputFile.close();

    cout << "\n=== Final Machine Code Memory Dump ===" << endl;
    for( int i = 0; i < curr_addr; ++i ) {
        printf( "%02X ", M[ i ] );
        if( ( i + 1 ) % 16 == 0 ) cout << endl;
    }
    cout << endl;
    
    writeMachineCodeToFile( "nums.x" );
    
    //look into SymbolTable
    cout << "\n=== Final Symbol Table Dump ===" << endl;
    symtab.printTable();

    cout << "\n=== COMPILER DRIVER END ===" << endl;
    return 0;
}
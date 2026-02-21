//interpreterWorking.cpp
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <ctype.h>
#include <string>
#include <cstring>
using namespace std;


/********** op-codes *****************************/
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
/******************************************************/

/**************** machine *************************/
bool done;
bool quit;
/*****************************************************/

/****************** config ***************************/
const unsigned int MemSize = 256 * 256;	//64K needs to be a power of 2
const unsigned int StackSize = 1024;
/******************************************************/

/***************** convert ****************************/
float real( unsigned int x ) {
	int i = x;
	float *f;
	f = ( float * ) &i;
	return *f;
}

unsigned int unreal( float x ) {
	float f = x;
	int *i;
	i = ( int * ) &f;
	return *i;
}
/*********************************************/

/******************** registers ***************************/
unsigned int  PC;	//program Counter
unsigned char IR;	//instruction Register
unsigned int  T;	//top of Expression Stack
//special purpose registers
unsigned int S;
unsigned int L;
unsigned int G;
/**************************************************************/

/************** expression Stack ****************************/
unsigned int X[ StackSize ] = { 0 }; //expression stack

unsigned int pop( void );
void push( unsigned int value );
void push( unsigned short value );
void push( unsigned char value );
void DumpStack();

unsigned int pop( void ){
	int value;
	if( T ) {
		value = X[ --T ];   
	} else {
		cout << "Expressions Stack Underflow. on line " << PC << endl;
		done = true;  
	}
	return value;
}

void push( unsigned char value ) {
	unsigned int x;
	if( 0x80 & value ) x = value | 0xffffff00;
		else x = value;
	push( x );
}

void push( unsigned short value ) {
	unsigned int x;
	if( 0x8000 & value ) x = value | 0xffff0000;
		else x = value;
	push( x );
}

void push( unsigned int value ) {
	if( T < StackSize ) {
		X[ T++ ] = value;
	} else {
		cout << "Expression Stack Overflow." << endl;
		//done = true;
	}
}

void DumpStack() {
	cout << endl << "Dumping Stack" ;
	int i;
	char tach;
	for( i = ( T - 1 ); i >= 0; i-- ) 
		cout << endl << X[ i ];
	cout << " End stack " << endl;
	cin >> tach;
}
/***************************************************/

/**************** memory ***************************/
const unsigned int AddrMask = MemSize - 1;
unsigned char M[ MemSize ] = { 0 };           //RAM
unsigned int get4( unsigned int & addr );
unsigned int get2( unsigned int & addr );
unsigned int get1( unsigned int & addr );
void put1( unsigned int & addr, unsigned int value );
void put2( unsigned int & addr, unsigned int value );
void put4( unsigned int & addr, unsigned int value );

unsigned int get4( unsigned int & addr ) {
	int a = get2( addr );
	int b = get2( addr ) << 16;
	return a | b;
}

unsigned int get2( unsigned int & addr ) {
	int a = get1( addr) ;
	int b = get1( addr ) << 8;
	return a | b;
}

unsigned int get1( unsigned int & addr ) {
	return M[ addr++ & AddrMask ];
}

void put4( unsigned int & addr, unsigned int value ) {
	put2( addr, value );
	put2( addr, value >> 16 );
}

void put2( unsigned int & addr, unsigned int value ) {
	put1( addr, value );
	put1( addr, value >> 8 );
}

void put1( unsigned int & addr, unsigned int value ) {
	M[ addr++ & AddrMask ] = value & 0xFF;
}
/**************************************************/

/****************** loader *************************/
const int MaxCommandLineLen = 80;

//made global so that decode can write same filename
char commandLine[ MaxCommandLineLen ]; 
void load( char prompt = '>' );

void load( char prompt ) {
	bool loaded = false;
	char fname[ 80 ];
	cout << endl;
	while( !loaded ) {
		cout << prompt;
		cin.getline( fname, MaxCommandLineLen );
		strcpy( commandLine, fname );
		strcat( fname, ".x" );
		ifstream fin( fname, ios::in );
		ofstream fout( "debug.txt", ios::out );
		if( !fin ) {
			cout << "*** ERROR: Invalid Filename" << endl;
		} else {
			fin.read( reinterpret_cast< char *>( M ), MemSize );
		}
	}
}

/************************************************/

/**************** fetch ****************************/
//routines for fetching the next instruction and stored operands
unsigned int next4( void );
unsigned int next2( void );
unsigned int next1( void );

unsigned int next4( void ) {
	int a = next2();
	int b = next2() << 16;
	return  a | b ;
}

unsigned int next2( void ) {
	int a = next1();
	int b = next1() << 8;
	return a | b;
}

unsigned int next1( void ) {
	return get1( PC );
}
/**************************************************/

int main() {
	int IR = 0;
	const unsigned int MaxInputLen = 80;
	unsigned int effAddr;
	unsigned int a, b, c, d; 	
	int n;
	int i;
	char ch;
	float r;			
	char k[ MaxInputLen ];	
	ofstream pcdata( "pcdata.dat", ios::out );

	quit = false;

	FILE *ifp;
	ifp = fopen( "testdata2.txt", "w" );
	ofstream testdata( "testdata.txt", ios::out );	

	while( !quit ) {
		load( 0xAF );
		T = 0;
		done = false;
		PC = 0;
		S = 1;
		L = 1;
		G = 1;
		while( !done ) {
	
			fprintf( ifp,"List of Registers:\n" );
			fprintf( ifp, "PC: %d", PC );
			fprintf( ifp, "\tIR: %d", IR );
			fprintf( ifp, "\tT: %d", T );
			fprintf( ifp, "\tS: %d", S );
			fprintf( ifp, "\tL: %d", L );
			fprintf( ifp, "\tG: %d", G );
			fprintf( ifp, "List of Stack: \n");
			for( n = 0; n < T; n++ ) {
				fprintf( ifp,"  %d \t ",X[ n ] );
				if( ( n + 1 ) % 10 == 0 ) fprintf( ifp, " \n" );
			}
			fprintf( ifp, "\n------------------------------------------------------\n" );
			testdata << endl << "------------------------------------------------------" << endl;
			cout.setf( ios::dec );
			for( n = 0; n < 48; n++ ) {
				fprintf( ifp, " %d \t ", M[ G + n ] );
				if( ( n + 1 ) % 10 == 0 ) fprintf( ifp, " \n" );
			}
			fprintf( ifp, "\n=======================================================\n" );
			testdata << endl << "=======================================================" << endl;
			
			IR = next1();  //fetch

			cout << IR << endl;

			pcdata << "PC: " << hex << PC-1 << " M[ 17 ]: " << M[ 23 ] << dec << endl; 
			cout << "PC: " << hex << PC << " M[ 17 ]: " << M[ 23 ] << dec << endl; 

			
			switch( IR ) {  

				//integer arithmetic operations
				case INC:
					push( pop() + 1 );
					cout << "INC" << endl;
					break;

				case DEC: 
					push( pop() - 1 );
					break;

				case ADD: 
					b = pop();
					push( pop() + b );
					break;

				case SUB: 
					b = pop();
					push( pop() - b );
					break;

				case MUL: 
					b = pop();
					push( pop() * b );		
					break;

				case DIV: 
					b = pop();
					push( pop() / b );
					break;

				case MOD: 
					b = pop();
					push( pop() % b );
					break;

				case NEG: 
					push( pop() * -1 );		 
					break;

				//floating-point arithmetic operations
				case ADDF: //floating-point add
					b = pop();
					push( unreal( real( pop() ) + real( b ) ) );
					break;

				case SUBF: //floating-point subtraction
					b = pop();
					push( unreal( real( pop() ) - real( b ) ) );
					break;

				case MULF: //floating-point multiplcation
					b = pop();
					push( unreal( real( pop() ) * real( b ) ) );
					break;

				case DIVF: //floating-point division
					b = pop();
					push( unreal( real( pop() ) / real( b ) ) );
					break;

				case NEGF: //floating-point negation
					push( unreal( real( pop() ) * -1 ) );
					break;
				

				//bitwise logical operations
				case NOT: 
					push( ~pop() );
					break;

				case AND: 
					b = pop();
					push( pop() & b );
					break;

				case OR: 
					b = pop();
					push( pop() | b ); 
					break;		  

				case XOR: 
					b = pop();
					push( pop() ^ b ); 
					break;	

				//shift operations
				case SHR: 
					push( ( pop() >> 1 ) & 0x7FFFFFFF );
					break;

				case SHL: 
					push( ( pop() << 1 ) & 0xFFFFFFFE );
					break;

				case SHRA: //arithmetic shift right
					push( pop() >> 1 );
					break;

				case ROR: //rotate right
					a = pop();
					b = ( a & 0x00000001 ) << 31;
					push( ( ( a >> 1 ) & 0x7FFFFFFF ) | b );
					break;

				case ROL: //rotate left
					a = pop(); 
					b = ( a & 0x80000000 ) >> 31;		
					push( ( ( a << 1 ) & 0xFFFFFFFE ) | b );		    
					break;

				//test integer operations
				case TPS: 
					n = pop();
					if( n > 0 ) push( 0xFFFFFFFF ); else push( (unsigned int)0 );
					break;

				case TNG: 	    
					n = pop();
					if( n < 0 ) push( 0xFFFFFFFF ); else push( (unsigned int)0 );
					break;

				case TZE: 	    
					n = pop();
					if( n == 0 ) push( 0xFFFFFFFF ); else push( (unsigned int)0 );
					break;

				case TNP: //test not positive
					n = pop();
					if( n < 0 ) push( 0xFFFFFFFF ); else push( (unsigned int)0 );
					break;

				case TNN: 
					n = pop();
					if( n >= 0 ) push( 0xFFFFFFFF ); else push( (unsigned int)0 );
					break;
					
				case TNZ:    
					n = pop();
					if( n != 0 ) push( 0xFFFFFFFF ); else push( (unsigned int)0 );
					break;

				//rest floating-point operations
				case TPSF: //test positive (floating-point)
					if( real( pop() ) > 0.0 ) push( 0xFFFFFFFF ); else push( (unsigned int)0 );
					break;

				case TNGF: //test negative  (floating-point)
					if( real( pop() ) < 0.0 ) push( 0xFFFFFFFF ); else push( (unsigned int)0 );
					break;

				case TZEF: //test zero (floating-point)
					if( real( pop() ) == 0.0 ) push( 0xFFFFFFFF ); else push( (unsigned int)0 );
					break;

				case TNPF: //test not positive  (floating-point)
					if( real( pop() ) <= 0.0 ) push( 0xFFFFFFFF ); else push( (unsigned int)0 );
					break;

				case TNNF: //test not negative  (floating-point)
					if( real( pop() ) > 0.0 ) push( 0xFFFFFFFF ); else push( (unsigned int)0 );
					break;	

				case TNZF: //test not zero  (floating-point)
					if( real( pop() ) != 0.0 ) push( 0xFFFFFFFF ); else push( (unsigned int)0 );
					break;

				//register transfer operations
				case PUSL: //push L onto stack
					push (L);
					break;

				case PUSG: //push G onto stack
					push (G);
					break;

				case PUSP: //push PC onto stack
					push (PC);
					break;

				case PUSS: //push S onto stack
					push (S);
					break;
							
				case POPL: //pop L from the stack
					L = pop();
					break;

				case POPG: //pop G from the stack
					G = pop();
					break;

				case POPP: //pop PC from the stack
					PC = pop();
					break;

				case POPS: //pop S from the stack
					S = pop();
					break;

				//IO operations
				case INS: /* Input String from keyboard*/
					effAddr = pop();
					cin.getline( k, MaxInputLen );
						i = 0;
						do {
							if( k[ i ] == '\0' ) break;
							else i++;
						} while( i < MaxInputLen );
						b = effAddr;	//b starts as base address of str
						put1( b, i );

						//d = get1( b ); 	//d is declared len as str
						//c = strlen( k ); 	//c is current 1 en of str
						//if ( c > d ) c = d; 	//the strings are safe
						//M[ b++ & AddrMask ] = c;
				
					for( a = 0; a < i; a++ ) put1( b, k[ a ] );
					break;

				case OTS: //output string to screen
					effAddr = pop();    
					b = effAddr;    
					c = get1( b );	//c is cur len of str
					for( a = 0; a < c; a++ ) cout << (char)( get1( b ) & 0xFF );
					break;

				//branch instructions
				case BYES: //branch on not zero (true)
					a = pop();
					b = next1();
					if( a ) PC = PC + b;
					break;

				case BNO: //branch on zero (false)
					a = pop();
					b = next1();
					if( !a ) PC = PC + b;
					break;
						
				//jump instructions
				case JMP: //jump (unconditional) implied
					PC = pop();
					break;

				case JMPD: //jump (unconditional) direct
					a = next2();
					PC = a;		
					break;

				case JMPI: //jump (unconditional) indirect
					a = next2();
					PC = get2( a );
					break;

				case JMPR: //jump (unconditional) relative
					a = next2();
					PC += a;
					break;

				//load instructions
				case LDD1: //load direct
					a = next2();
					push( get1( a ) );
					break;

				case LDDN1: //load direct indexed
					a = next2() + pop();
					push( get1( a ) );
					break;

				case LDI1: //load indirect
					b = next2();
					a = get2( b );
					push( get1( a ) );
					break;

				case LDIN1: //load indirect indexed
					b = next2();
					a = get2( b ) + pop();
					push( get1( a ) );
					break;

				case LDR1: //load relative
					c = next1();
					a = PC + c;
					push( get1( a ) );
					break;

				case LDRN1: //load relative indexed
					c = next1();
					a = PC + c + pop();
					push( get1( a ) );
					break;

				case LDM1: //load immediate 1 byte
					d = next1();
					push( d );
					break;

				case LDLR1: //load local relative
					a = next2() + L;
					push( get1( a ) );
					break;

				case LDLN1: //load local indexed
					a = next2() + pop() + L;
					push( get1( a ) );
					break;

				case LDGR1: //load local global relative
					a = next2() + G;
					push( get1( a ) );
					break;

				case LDGN1: //load local global indexed
					a = next2() + pop() + G;
					push( get1( a ) );
					break;

				case LDD2: //load direct
					a = next2();
					push( get2( a ) );
					break;			

				case LDDN2: //load direct indexed
					a = next2() + pop();
					push( get2( a ) );
					break;			

				case LDI2: //load indirect
					b = next2();
					a = get2( b );
					push( get2( a ) );
					break;
						
				case LDIN2: //load indirect indexed
					b = next2();
					a = get2( b ) + pop();
					push( get2( a ) );
					break;			

				case LDR2: //load relative
					c = next1();
					a = PC + c;
					push( get2( a ) );
					break;	
						
				case LDRN2: //load relative indexed
					c = next1();
					a = PC + c + pop();
					push( get2( a ) );
					break;			

				case LDM2: //load immediate 2 byte
					d = next2();
					push( d );
					break;			

				case LDLR2: //load local relative
					a = next2() + L;
					push( get2( a ) );
					break;	

				case LDLN2: //load local indexed
					a = next2() + pop() + L;
					push( get2( a ) );
					break;			

				case LDGR2: //local global relative
					a = next2() + G;
					push( get2( a ) );
					break;			

				case LDGN2: //local global indexed
					a = next2() + pop() + G;
					push( get2( a ) );
					break;

				case LDD4: //load direct
					a = next2();
					push( get4( a ) );
					break;

				case LDDN4: //load direct indexed
					a = next2() + pop();
					push( get4( a ) );
					break;

				case LDI4: //load indirect
					b = next2();
					a = get2( b );
					push( get4( a ) );
					break;

				case LDIN4: //load indirect indexed
					b = next2();
					a = get2( b ) + pop();
					push( get4( a ) );
					break;

				case LDR4: //load relative
					c = next1();
					a = PC + c;
					push( get4( a ) );
					break;

				case LDRN4: //load relative indexed
					c = next1();
					a = PC + c + pop();
					push( get4( a ) );
					break;

				case LDM4: //load immediate 4 byte
					d = next4();
					push( d );
					break;

				case LDLR4: //load local relative
					a = next2() + L;
					push( get4( a ) );
					break;

				case LDLN4: //load local indexed
					a = next2() + pop() + L;
					push( get4( a ) );
					break;

				case LDGR4: //local global relative
					a = next2() + G;
					push( get4( a ) );
					break;

				case LDGN4: //local global indexed
					a = next2() + pop() + G;
					push( get4( a ) );
					break;

				case STD1: //store direct
					a = next2();
					put1( a, pop() );
					break;		

				case STDN1: //store direct indexed
					a = next2() + pop();
					put1( a, pop() );
					break;		

				case STI1: //store indirect
					b = next2();
					a = get2( b );
					put1( a, pop() );
					break;

				case STIN1: //store indirect indexed
					b = next2();
					a = get2( b ) + pop();
					put1( a, pop() );
					break;

				case STR1: //store relative
					c = next1();
					a = PC + c;
					put1( a, pop() );
					break;

				case STRN1: //store relative indexed
					c = next1();
					a = PC + c + pop();
					put1( a, pop() );
					break;

				case STLR1: //store local relative
					a = next2() + L;
					put1( a, pop() );
					break;

				case STLN1: //store local indexed
					a = next2() + pop() + L;
					put1( a, pop() );
					break;

				case STGR1: //store global relative
					a = next2() + G;
					put1( a, pop() );
					break;

				case STGN1: //store global indexed
					a = next2() + pop() + G;
					put1( a, pop() );
					break;

				case STD2: //store direct
					a = next2();
					put2( a, pop() );
					break;

				case STDN2: //store direct indexed
					a = next2() + pop();
					put2( a, pop() );
					break;		

				case STI2: //store indirect
					b = next2();
					a = get2( b );
					put2( a, pop() );
					break;

				case STIN2: //store indirect indexed
					b = next2();
					a = get2( b ) + pop();
					put2( a, pop() );
					break;			

				case STR2: //store relative
					c = next1();
					a = PC + c;
					put2( a, pop() );
					break;			

				case STRN2: //store relative indexed
					c = next1();
					a = PC + c + pop();
					put2( a, pop() );
					break;			

				case STLR2: //store local relative
					a = next2() + L;
					put2( a, pop() );
					break;

				case STLN2: //store local indexed
					a = next2() + pop() + L;
					put2( a, pop() );
					break;

				case STGR2: //store global relative
					a = next2() + G;
					put2( a, pop() );
					break;

				case STGN2: //store global indexed
					a = next2() + pop() + G;
					put2( a, pop() );
					break;

				case STD4: //store direct
					a = next2();
					put4( a, pop() );
					break;

				case STDN4: //store direct indexed
					a = next2() + pop();
					put4( a, pop() );
					break;		

				case STI4: //store indirect
					b = next2();
					a = get2( b );
					put4( a, pop() );
					break;

				case STIN4: //store indirect indexed
					b = next2();
					a = get2( b ) + pop();
					put4( a, pop() );
					break;

				case STR4: //store relative
					c = next1();
					a = PC + c;
					put4( a, pop() );
					break;

				case STRN4: //store relative indexed
					c = next1();
					a = PC + c + pop();
					put4( a, pop() );
					break;

				case STLR4: //store local relative
					a = next2() + L;
					put4( a, pop() );
					break;

				case STLN4: //store local indexed
					a = next2() + pop() + L;
					put4( a, pop() );
					break;

				case STGR4: //store global relative
					a = next2() + G;
					put4( a, pop() );
					break;

				case STGN4: //store global indexed
					a = next2() + pop() + G;
					put4( a, pop() );
					break;

				case DER: //dereference
					a = pop();
					push( get2( a ) );
					break;

				case HALT: //halt the processor
					cout << "Halt" << endl;
					done = true;
					break;

				case FLT: //float an Int
					d = pop();
					r = d;
					push( unreal( r ) );
					break;
				
				case COPY: 	
					b = pop();
					push( b );
					push( b );
					break;
						
				case DROP: //drop top of stack
					pop();
					break;
						
				case SWAP: //swap top two
					a = pop();
					b = pop();
					push( a );
					push( b );
					break;
								
				case OUTC: //output a char
					ch = pop() & 0xff;
					cout << ch;
					break;
						
				case OUTI: //output a int
					n = pop();
					cout << n;
					break; 
						
				case OUTR: //output a real
					d = pop();
					r = real( d);
					cout << r;
					break; 	
						
				case INPUTC: //input a char
					cin >> ch;
					push( static_cast<unsigned char>( ch ) );
					break;
				
				case INPUTI: //input an int
					cin >> n;
					push( static_cast<unsigned int>( n ) );
					break;
				
				case INPUTR: //input a real
					cin >> r;
					push( unreal( r ) );
					break;
																	
				case DSTK: {	//grab top stack val and shove it to the screen
					cout << "Stack: ";
					cout << hex;
					for( int i = 0; i < T; i++ ) cout << X[ i ] << " ";
					cout << dec << endl;
					break;
				}
									
				case NOOP: break;  //no operation

				default    : //no instruction match up with op code
						cout << "Invalid Instruction. PC:" << hex << PC << dec << endl;
						done = true;
						break;
			}	//end switch
		}	//end while( !done )
 	}	//end while( !quit )
 testdata.close();
//close( ifp );
}	//end main
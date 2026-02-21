#include <iostream>
using namespace std;

#include <iomanip>

#include <cstdlib>

#include <cstdio>
#include <fstream>
#include <ctype.h>
#include <string>


using namespace std;


/********** Op-codes *****************************/
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
const unsigned int MemSize = 256 * 256;	/* 64K needs to be a power of 2*/
const unsigned int StackSize = 1024;
/******************************************************/

/***************** convert ****************************/
float real(unsigned int x) {
  int i = x;
  float *f;
  f = (float *) &i;
  return *f;
}

unsigned int unreal(float x) {
  float f = x;
  int *i;
  i = (int *) &f;
  return *i;
}
/*********************************************/

/******************** Registers ***************************/
unsigned int  PC;	/* Program Counter*/
unsigned char IR;	/* Instruction Register*/
unsigned int  T;	/* Top of Expression Stack*/
/* Special Purpose Registers*/
unsigned int S;
unsigned int L;
unsigned int G;
/**************************************************************/

/************** EXPRESSION Stack ****************************/
unsigned int X[StackSize] = {0}; /*** Expression Stack*/

unsigned int pop(void);
void push(unsigned int value);
void push(unsigned short value);
void push(unsigned char value);
void DumpStack();

unsigned int pop(void){
  int value;
  if(T){
    value = X[--T];   
  } else {
      cout << "Expressions Stack Underflow. on line " << PC << endl;
      done = true;  
  }
  return value;
}

void push( unsigned char value)
{unsigned int x;
 if (0x80 & value) x = value | 0xffffff00;
 	else x = value;
 push(x);
 }

void push(unsigned short value)
{unsigned int x;
 if ( 0x8000 & value) x = value | 0xffff0000;
 	else x = value;
 push(x);
}
void push(unsigned int value) {
  if (T < StackSize) {
    X[T++] = value;
  } else {
    cout << "Expression Stack Overflow." << endl;
    /*done = true; */
  }
}

void DumpStack()
{cout << endl << "Dumping Stack" ;
 int i;
 char tach;
 for ( i = (T-1); i >=0; i--) cout << endl << X[T];
 cout << " End stack " << endl;
 cin >> tach;
}
/***************************************************/

/**************** Memory ***************************/
const unsigned int AddrMask = MemSize - 1;
unsigned char M[MemSize] = {0};           /* RAM*/
unsigned int get4(unsigned int & addr);
unsigned int get2(unsigned int & addr);
unsigned int get1(unsigned int & addr);
void put1(unsigned int & addr, unsigned int value);
void put2(unsigned int & addr, unsigned int value);
void put4(unsigned int & addr, unsigned int value);
unsigned int get4(unsigned int & addr) {
  int a = get2(addr);
  int b = get2(addr) << 16;
  return a | b;
}
unsigned int get2(unsigned int & addr) {
  int a = get1(addr);
  int b = get1(addr) << 8;
  return a | b;
}

unsigned int get1(unsigned int & addr) {
  return M[addr++ & AddrMask];
}

void put4(unsigned int & addr, unsigned int value) {
  put2(addr,value);
  put2(addr,value >> 16);
}

void put2(unsigned int & addr, unsigned int value) {
  put1(addr,value);
  put1(addr,value >> 8);
}

void put1(unsigned int & addr, unsigned int value) {
  M[addr++ & AddrMask] = value & 0xFF;
}
/**************************************************/
/****************** loader *************************/
const int MaxCommandLineLen = 80;

/* Made Global so that xdecode can write same filename.*/
char commandLine[ MaxCommandLineLen]; 
void load( char prompt = '>');
void load( char prompt) 
{
  bool loaded = false;
  char fname[ 80];
  cout << endl;
  while( !loaded) 
  {
    cout << prompt;
    cin.getline( fname, MaxCommandLineLen);
/*    strcpy( fname, commandLine);
    strcat( fname, ".x");
  */  ifstream fin( fname, ios::in);
    ofstream fout("nums.x", ios::out);
    if( !fin) {
      cout << "*** ERROR: Invalid Filename" << endl;
    } else {
      fin.read( reinterpret_cast< char *>( M), MemSize);
  /*   if( !fin) {
     cout << "*** ERROR: File Error" << endl;
      } else {
  */      loaded = true;
/*      }      */
    }
  }
}

/************************************************/
/**************** Fetch ****************************/
/* Routines for fetching the next instruction and stored operands*/
unsigned int next4(void);
unsigned int next2(void);
unsigned int next1(void);

unsigned int next4(void){
  int a = next2();
  int b = next2() << 16;
  return  a | b ;
}

unsigned int next2(void) {
  int a = next1();
  int b = next1() << 8;
  return a | b;
}

unsigned int next1(void) {
  return get1(PC);
}
/**************************************************/

int main() {
	int IR = 0;
	const unsigned int MaxInputLen = 80;
	unsigned int effAddr;
	unsigned int a,b,c,d; 	
	int n;
	int i;
	char ch;
	float r;			
	char k[ MaxInputLen];	
/*  ofstream pcdata ("pcdata.dat",ios::out);
*/
  quit = false;
/* ===============>Test codes
	//for(n=0;n<512;n++)
	//	X[n]=rand();
*/
    FILE *ifp;
	ifp=fopen("testdata2.txt","w");
	ofstream testdata("testdata.txt",ios::out);	
/*<===============*/
  while (!quit) {
    load(0xAF);    /* 0xAF is '�', giving user a prompt to enter input filename.    */
    T = 0;
    done = false;
    PC = 0;
	S = 1;
	L = 1;
	G = 1;
    while (!done) {

/*===============>Test codes*/
	testdata<<"List of Registers:"<<endl;
	testdata<<"PC:"<<PC;
/*	testdata<<"\tIR:"<<setbase(10)<<IR;
*/	testdata<<"\tT:"<<T;
	testdata<<"\tS:"<<S;
	testdata<<"\tL:"<<L;
	testdata<<"\tG:"<<G<<endl;
	testdata<<"List of Stack:"<<endl;
	for (n=0;n<T;n++){
		testdata<< X[n]<<"\t";
		if((n+1)%10==0) testdata<<endl;
	}
	
	fprintf(ifp,"List of Registers:\n");
	fprintf(ifp,"PC: %d",PC);
	fprintf(ifp,"\tIR: %d",IR);
	fprintf(ifp,"\tT: %d",T);
	fprintf(ifp,"\tS: %d",S);
	fprintf(ifp,"\tL: %d",L);
	fprintf(ifp,"\tG: %d",G);
	fprintf(ifp,"List of Stack: \n");
	for (n=0;n<T;n++){
		fprintf(ifp,"  %d \t ",X[n]);
		if((n+1)%10==0) fprintf(ifp," \n");
	}
	fprintf(ifp,"\n------------------------------------------------------\n");
	testdata<<endl<<"------------------------------------------------------"<<endl;
	cout.setf(ios::dec);
	for( n=0;n<48;n++){
		fprintf(ifp," %d \t ",M[G+n]);
		if((n+1)%10==0) fprintf(ifp," \n");
	}
	fprintf(ifp,"\n=======================================================\n");
	testdata<<endl<<"======================================================="<<endl;
	
/*<===============*/

      IR = next1();  /* fetch*/

/*	 cout << IR << endl;*/
/*
//	pcdata << "PC: " << hex << PC-1 << " M[17]: " << M[23] << dec << endl; 
	cout << "PC: " << hex << PC << " M[17]: " << M[23] << dec << endl; 
*/
      
 switch( IR) {  

	/* Integer Arithmetic Operations*/

	case INC :
		  push (pop() + 1);
		  break;

	case DEC : 
		  push (pop() - 1);
		  break;
//?????????????????????????????
	case ADD : 
	      b = pop();
		  push(pop() + b);
		  break;
//?????????????????????????????		

	case SUB : 
		  b = pop();
		  push(pop() - b);
		  break;

	case MUL : 
		  b = pop();
		  push(pop() * b);		
		  break;

	case DIV : 
	 	  b = pop();
		  push(pop() / b);
		  break;

	case MOD : 
		  b = pop();
		  push(pop() % b);
		  break;

	case NEG : 
		  push(pop() * -1);		 
		  break;
/* Floating-point Arithmetic Operations*/

	case ADDF : /*Floating-point Add*/
		    b = pop();
		    push( unreal( real( pop()) + real( b)));
		    break;
//?????????????????????????????
	case SUBF : /*Floating-point Subtraction*/
		    b = pop();
		    push( unreal( real( pop()) - real( b)));
		    break;
//?????????????????????????????		    

	case MULF : /*Floating-point Multiplcation*/
			b = pop();
		    push( unreal( real( pop())*real( b)));
		    break;
	case DIVF : /*Floating-point Division*/
			b = pop();
		    push( unreal( real( pop()) / real( b)));
		    break;



	case NEGF : /*Floating-point Negation*/
			push( unreal( real( pop()) * -1));
		    break;
	

	/* Bitwise Logical Operations*/
	case NOT : 
		   push( ~pop());
		   break;

	case AND : 
		   b = pop();
		   push( pop() & b);
		   break;

	case OR  : 
		   b = pop();
		   push(pop() | b); 
		   break;		  

	case XOR : 
		   b = pop();
		   push(pop() ^ b); 
		   break;	

	/* Shift Operations*/
	case SHR  : 
		    push(( pop() >> 1) & 0x7FFFFFFF);
		    break;

	case SHL  : 
		    push((pop() << 1) & 0xFFFFFFFE);
		    break;

	case SHRA : /* Arithmetic Shift Right*/
		    push(pop() >> 1);
  	    break;

	case ROR  : /* Rotate Right*/
			a = pop();
        	b = ( a & 0x00000001) << 31;
		    push((( a >> 1) & 0x7FFFFFFF) | b);
		    break;

	case ROL  : /* Rotate Left*/	   
  			a = pop(); 
			b = ( a & 0x80000000) >> 31;		
	    	push((( a << 1) & 0xFFFFFFFE) | b);		    
	    	break;

	/* Test Integer Operations*/

	case TPS  : 
			n = pop();
      if(n > 0) push(0xFFFFFFFF); else push((unsigned int)0);
	    break;

	case TNG  : 	    
		n = pop();
      	if(n < 0) push(0xFFFFFFFF); else push((unsigned int)0);
	    break;
	case TZE  : 	    
		n = pop();
      	if(n == 0) push(0xFFFFFFFF); else push((unsigned int)0);
	    break;
//?????????????????????????????
	case TNP  : //Test not positive
		n = pop();
        if(n < 0) push(0xFFFFFFFF); else push((unsigned int)0);
	    break;
//?????????????????????????????

	case TNN  : 
		n = pop();
      	if(n >= 0) push(0xFFFFFFFF); else push((unsigned int)0);
	    break;
	    
	case TNZ  :    
		n = pop();
      	if(n != 0) push(0xFFFFFFFF); else push((unsigned int)0);
	    break;
    		    

	/* Test Floating-point Operations*/
case TPSF  : /* Test Positive (floating-point)*/
          if (real(pop()) > 0.0) push(0xFFFFFFFF); else push((unsigned int)0);
			    break;

	case TNGF : /* Test Negative  (floating-point)*/
          if (real(pop()) < 0.0) push(0xFFFFFFFF); else push((unsigned int)0);
			break;

	case TZEF : /* Test Zero (floating-point)*/
          if (real(pop()) == 0.0) push(0xFFFFFFFF); else push((unsigned int)0);
			    break;

	case TNPF : /* Test Not Positive  (floating-point)*/
          if (real(pop()) <= 0.0) push(0xFFFFFFFF); else push((unsigned int)0);
			    break;
//?????????????????????????????
	case TNNF : /* Test Not Negative  (floating-point)*/
          if (real(pop()) > 0.0) push(0xFFFFFFFF); else push((unsigned int)0);
			    break;	
//?????????????????????????????

	case TNZF : /* Test Not Zero  (floating-point)*/
          if (real(pop()) != 0.0) push(0xFFFFFFFF); else push((unsigned int)0);
			    break;

	/* Register Transfer Operations*/

	
	case PUSL : /* Push L Onto Stack*/
			    push (L);
			    break;

	case PUSG : /* Push G Onto Stack*/
			    push (G);
			    break;

	case PUSP : /* Push PC Onto Stack*/
			    push (PC);
			    break;

	case PUSS : /* Push S Onto Stack*/
			    push (S);
			    break;
			    
	case POPL : /* Pop L from the Stack*/
			    L = pop();
			    break;

	case POPG : /* Pop G from the Stack*/
			    G = pop();
			    break;
//?????????????????????????????
	case POPP : /* Pop PC from the Stack*/
	            PC = pop();
	            break;
//?????????????????????????????

	case POPS : /* Pop S from the Stack*/
			    S = pop();
			    break;


	/* IO Operations.*/

	case INS  : /* Input String from keyboard*/
	    effAddr = pop();
	    cin.getline( k, MaxInputLen);
			i = 0;
			do {
				if (k[i] == '\0') break;
				else i++;
			} while (i < MaxInputLen);
			b = effAddr;	/* b starts as base address of str*/
			put1(b, i);

/*		//    d = get1( b); 	// d is declared len as str
		//    c = strlen( k); 	// c is current 1 en of str
		//    if ( c > d) c = d; 	//the strings are safe
		//    M[ b++ & AddrMask] = c;
*/
	    for( a = 0; a < i; a++) put1( b, k[ a]);
		  break;

	case OTS  : /* Output String to Screen*/
		    effAddr = pop();    
		    b = effAddr;    
		    c = get1( b);	/* c is cur len of str*/
   	    for( a = 0; a < c; a++) cout << ( char) ( get1( b) & 0xFF);
		    break;

    /*	 Branch Instructions*/

	case BYES : /* Branch on Not Zero (True)*/
		    a = pop();
			b = next1();
		    if( a) PC = PC + b;
		    break;

	case BNO  : /* Branch on Zero (False)*/
		    a = pop();
        	b = next1();
		    if( !a) PC = PC + b;
		    break;
		    
	/* Jump Instructions*/

	case JMP  : /* Jump (Unconditional) Implied*/
		    PC = pop();
		    break;

	case JMPD : /* Jump (Unconditional) Direct*/
		    a = next2();
		    PC = a;		
	      break;

	case JMPI : /* Jump (Unconditional) Indirect*/
		    a = next2();
		    PC = get2( a);
		    break;

	case JMPR : /* Jump (Unconditional) Relative*/
		    a = next2();
		    PC += a;
		    break;

	/* Load Instructions*/

	case LDD1  : /* Load Direct*/
			a = next2();
			push(get1(a));
			break;

	case LDDN1 : /* Load Direct Indexed*/
			a = next2() + pop();
			push(get1(a));
			break;

	case LDI1  : /* Load Indirect*/
			b = next2();
			a = get2(b);
			push(get1(a));
			break;

	case LDIN1 : /* Load Indirect Indexed*/
			b = next2();
			a = get2(b) + pop();
			push(get1(a));
			break;

	case LDR1  : /* Load Relative*/
			c = next1();
			a = PC + c;
			push(get1(a));
			break;

	case LDRN1 : /* Load Relative Indexed*/
			c = next1();
			a = PC + c + pop();
			push(get1(a));
			break;

	case LDM1  : /* Load Immediate 1 byte*/
			d = next1();
			push(d);
			break;

	case LDLR1 : /* Load Local Relative*/
			a = next2() + L;
			push(get1(a));
			break;

	case LDLN1 : /* Load Local Indexed*/
			a = next2() + pop() + L;
			push(get1(a));
			break;

	case LDGR1 : /* Local Global Relative*/
			a = next2() + G;
			push(get1(a));
			break;

	case LDGN1 : /* Local Global Indexed*/
			a = next2() + pop() + G;
			push(get1(a));
			break;
//?????????????????????????????
// Fix all of the Load 2s, use the Load 1s and Load 4s for help
	case LDD2  : /* Load Direct*/
			a = next2();
			push(get2(a));
			break;			

	case LDDN2 : /* Load Direct Indexed*/
			a = next2() + pop();
			push(get2(a));
			break;			

	case LDI2  : /* Load Indirect*/
            b = next2();
			a = get2(b);
			push(get2(a));
			break;
			
	case LDIN2 : /* Load Indirect Indexed*/
			b = next2();
			a = get2(b) + pop();
			push(get2(a));
			break;			

	case LDR2  : /* Load Relative*/
			c = next1();
			a = PC + c;
			push(get2(a));
			break;	
			
	case LDRN2 : /* Load Relative Indexed*/
			c = next1();
			a = PC + c + pop();
			push(get2(a));
			break;			
	case LDM2  : /* Load Immediate 2 byte*/
			d = next2();
			push(d);
			break;			
	case LDLR2 : /* Load Local Relative*/
			a = next2() + L;
			push(get2(a));
			break;			
	case LDLN2 : /* Load Local Indexed*/
			a = next2() + pop() + L;
			push(get2(a));
			break;			
	case LDGR2 : /*/ Local Global Relative*/
			a = next2() + G;
			push(get2(a));
			break;			
	case LDGN2 : /* Local Global Indexed*/
			a = next2() + pop() + G;
			push(get2(a));
			break;
//?????????????????????????????

	case LDD4  : /* Load Direct*/
			a = next2();
			push(get4(a));
			break;

	case LDDN4 : /* Load Direct Indexed*/
			a = next2() + pop();
			push(get4(a));
			break;

	case LDI4  : /* Load Indirect*/
			b = next2();
			a = get2(b);
			push(get4(a));
			break;

	case LDIN4 : /* Load Indirect Indexed*/
			b = next2();
			a = get2(b) + pop();
			push(get4(a));
			break;

	case LDR4  : /* Load Relative*/
			c = next1();
			a = PC + c;
			push(get4(a));
			break;

	case LDRN4 : /* Load Relative Indexed*/
			c = next1();
			a = PC + c + pop();
			push(get4(a));
			break;

	case LDM4  : /* Load Immediate 4 byte*/
			d = next4();
			push(d);
			break;

	case LDLR4 : /* Load Local Relative*/
			a = next2() + L;
			push(get4(a));
			break;

	case LDLN4 : /* Load Local Indexed*/
			a = next2() + pop() + L;
			push(get4(a));
			break;

	case LDGR4 : /* Local Global Relative*/
			a = next2() + G;
			push(get4(a));
			break;

	case LDGN4 : /* Local Global Indexed*/
			a = next2() + pop() + G;
			push(get4(a));
			break;

	case STD1  : /* Store Direct*/
			a = next2();
			put1(a,pop());
			break;		

	case STDN1 : /* Store Direct Indexed*/
			a = next2() + pop();
			put1(a,pop());
			break;		

	case STI1  : /* Store Indirect*/
			b = next2();
			a = get2(b);
			put1(a,pop());
			break;

	case STIN1 : /* Store Indirect Indexed*/
			b = next2();
			a = get2(b) + pop();
			put1(a,pop());
			break;

	case STR1  : /* Store Relative*/
			c = next1();
			a = PC + c;
			put1(a,pop());
			break;

	case STRN1 : /* Store Relative Indexed*/
			c = next1();
			a = PC + c + pop();
			put1(a,pop());
			break;

	case STLR1 : /* Store Local Relative*/
			a = next2() + L;
			put1(a,pop());
			break;

	case STLN1 : /* Store Local Indexed*/
			a = next2() + pop() + L;
			put1(a,pop());
			break;

	case STGR1 : /* Store Global Relative*/
			a = next2() + G;
			put1(a,pop());
			break;

	case STGN1 : /* Store Global Indeed*/
			a = next2() + pop() + G;
			put1(a,pop());
			break;

	case STD2  : /* Store Direct*/
			a = next2();
			put2(a,pop());
			break;

	case STDN2 : /* Store Direct Indexed*/
			a = next2() + pop();
			put2(a,pop());
			break;		

	case STI2  : /* Store Indirect*/
			b = next2();
			a = get2(b);
			put2(a,pop());
			break;
//?????????????????????????????
	case STIN2 : /* Store Indirect Indexed*/
			b = next2();
			a = get2(b) + pop();
			put2(a,pop());
			break;			

	case STR2  : /* Store Relative*/
			c = next1();
			a = PC + c;
			put2(a,pop());
			break;			
	case STRN2 : /* Store Relative Indexed*/
			c = next1();
			a = PC + c + pop();
			put2(a,pop());
			break;			
	case STLR2 : /* Store Local Relative*/
			a = next2() + L;
			put2(a,pop());
			break;
//?????????????????????????????

	case STLN2 : /* Store Local Indexed*/
			a = next2() + pop() + L;
			put2(a,pop());
			break;

	case STGR2 : /* Store Global Relative*/
			a = next2() + G;
			put2(a,pop());
			break;

	case STGN2 : /* Store Global Indeed*/
			a = next2() + pop() + G;
			put2(a,pop());
			break;

	case STD4  : /* Store Direct*/
			a = next2();
			put4(a,pop());
			break;

	case STDN4 : /* Store Direct Indexed*/
			a = next2() + pop();
			put4(a,pop());
			break;		

	case STI4  : /* Store Indirect*/
			b = next2();
			a = get2(b);
			put4(a,pop());
			break;

	case STIN4 : /* Store Indirect Indexed*/
			b = next2();
			a = get2(b) + pop();
			put4(a,pop());
			break;

	case STR4  : /* Store Relative*/
			c = next1();
			a = PC + c;
			put4(a,pop());
			break;

	case STRN4 : /* Store Relative Indexed*/
			c = next1();
			a = PC + c + pop();
			put4(a,pop());
			break;

	case STLR4 : /* Store Local Relative*/
			a = next2() + L;
			put4(a,pop());
			break;

	case STLN4 : /* Store Local Indexed*/
			a = next2() + pop() + L;
			put4(a,pop());
			break;

	case STGR4 : /* Store Global Relative*/
			a = next2() + G;
			put4(a,pop());
			break;

	case STGN4 : /* Store Global Indexed*/
			a = next2() + pop() + G;
			put4(a,pop());
			break;

	case DER   : /* Dereference*/
			a = pop();
			push(get2(a));
			break;

	case HALT  : /* Halt the processor*/
			done = true;
			cout << "HALT";
			break;

	case FLT : /* Float an Int*/
           d = pop();
           r = d;
           push( unreal( r));
           break;
	
	case COPY : 	
			b = pop();
			push(b);
			push(b);
			break;
			
    case DROP  : /* Drop Top of Stack*/
            pop();
            break;
            
    case SWAP  : /* Swap Top Two*/
             a = pop();
             b = pop();
             push( a);
             push( b);
             break;
                     
    case OUTC  : /* Output a Char*/
             ch = pop() & 0xff;
             cout << ch;
             break;
             
    case OUTI  : /* Output a Int*/
             n = pop();
             cout << n;
             break; 
             
    case OUTR  : /* Output a Int*/
             d = pop();
             r = real( d);
             cout << r;
             break; 	
             
    case INPUTC : /*Input a Char*/
    		cin >> ch;
    		push( static_cast<unsigned char>(ch));
    		break;
    
    case INPUTI : /*Input an Int*/
    		cin >> n;
    		push( static_cast<unsigned int>(n));
    		break;
    
    case INPUTR : /*Input a real*/
    		cin >> r;
    		push( unreal(r));
    		break;
    		             
   
    		             
	/* woohoo, added this one fer m'own purpose*/
    case DSTK: /* Grab top stack val and shove it to the screen*/
		{
		 cout << "Stack: ";
		 cout << hex;
		 for (int i = 0; i < T; i++) cout << X[i] << " ";
		 cout << dec << endl;
		 break;
		}
    		             
	case NOOP  : break;  /* No operation*/

	default    : /* No instruction match up with op code*/
			cout << "Invalid Instruction. PC:" << hex << PC << dec << endl;
			done = true;
			break;
	}/* End switch*/
   } /*End while(!done)*/
 } /* End while(!quit) */
 testdata.close();
/* close(ifp);*/
} /** End Main */
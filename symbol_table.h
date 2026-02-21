//symbol_table.h
#pragma once

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

//Symbol struct
struct Symbol {
    string name;
    string type;
    string kind;
    int scope = 0;
    int address = 0;
};

//SymbolTable class
class SymbolTable {
private:
    static const int MAX_SYMBOLS = 100;
    static const int MAX_SCOPES = 50;
    
    Symbol table[ MAX_SYMBOLS ];
    int current_size = 0;
    int current_scope = 0;
    int current_address[ MAX_SCOPES]  = { 0 };

    void uppercase( string& str ) const {
        for( char &c : str ) {
            c = toupper( c );
        }
    }

public:
    SymbolTable();

    //core functionality
    void add_symbol( Symbol sym );
    void del_current_scope();
    
    //symbol creation
    static Symbol create_symbol( string name, string type, string kind );
    
    //lookup and info
    int find_symbol( string name ) const;
    int getCurrentScopeStart() const;
    int getCurrentScope() const { return current_scope; }
    
    //printing
    void print_symbol( int pos ) const;
    void printTable() const;
};

/*
#pragma once        //prevent multiple inclusions of this header file

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

//Symbol struct
struct Symbol
{
	string name;
	string type;
	string kind;
	int scope = 0 ;
	int address = 0;
};

//SymbolTable class
class SymbolTable
{
	private:
		Symbol table[ 100 ];
		int current_size = 0;			//current_size is how many element existed int the table, so counter = current_size - 1
		int current_scope = 0;
		int current_address[ 50 ];		//current_address of scope 0 is current_address[ 0 ]
	public:
		SymbolTable();
		int getCurrentScopeStart() const;

		void add_symbol( Symbol );
		void del_current_scope();
		
		static Symbol create_symbol( string, string, string );	//need the namespace SymbolTable::create_symbol
		int find_symbol( string ) const;
		void print_symbol( int ) const;
		void printTable() const;
};
*/
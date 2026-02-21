//symbol_table.cpp
#include "symbol_table.h"
#include <iostream>
#include <iomanip>
using namespace std;

//constructor
SymbolTable::SymbolTable() {
    current_size = 0;
    current_scope = 0;
    current_address[ 0 ] = 0; //initialize global scope address
}

//add a symbol to the table
void SymbolTable::add_symbol( Symbol sym ) {
    if( current_size >= MAX_SYMBOLS ) {
        cerr << "Symbol table overflow!" << endl;
        return;
    }

    //convert to uppercase for consistency
    uppercase( sym.name );
    uppercase( sym.type );
    uppercase( sym.kind );

    //set scope and address
    sym.scope = current_scope;
    sym.address = current_address[current_scope];

    //calculate size based on type
    if( sym.type == "CHAR" ) {
        current_address[ current_scope ] += 1;
    } 
    else if( sym.type == "INT" || sym.type == "HEX" || sym.type == "REAL" ) {
        current_address[ current_scope ] += 4;
    } 
    else if( sym.type == "PTR" ) {
        current_address[ current_scope ] += 2;
    } 
    else if( sym.type == "YN" ) {
        current_address[ current_scope ] += 1;
    } 
    else if( sym.type == "STR" ) {
        current_address[ current_scope ] += 80; //max string length
    } 
    else { //NONE or unknown
        current_address[ current_scope ] += 0;
    }

    //add to table
    table[ current_size++ ] = sym;

    //if this is a function, increment scope
    if( sym.kind == "FUN" ) {
        current_scope++;
        if( current_scope >= MAX_SCOPES ) {
            cerr << "Scope depth exceeded!" << endl;
            current_scope--;
        } else {
            current_address[ current_scope ] = 0; //reset address for new scope
        }
    }
}

//delete all symbols in current scope
void SymbolTable::del_current_scope() {
    if( current_size == 0 ) return;

    int scope_to_delete = current_scope;
    while( current_size > 0 && table[ current_size - 1 ].scope == scope_to_delete ) {
        current_size--;
    }

    //if we deleted function scope, go back to parent scope
    if( scope_to_delete > 0 ) {
        current_scope--;
    }
}

//create a new symbol
Symbol SymbolTable::create_symbol( string name, string type, string kind ) {
    Symbol sym;
    sym.name = name;
    sym.type = type;
    sym.kind = kind;
    return sym;
}

//find a symbol by name( returns index or -1 if not found )
int SymbolTable::find_symbol( string name ) const {
    uppercase( name );
    
    //search from most recent to oldest( to respect scoping )
    for( int i = current_size - 1; i >= 0; i-- ) {
        if( table[ i ].name == name ) {
            return i;
        }
    }
    return -1;
}

//get starting index of current scope
int SymbolTable::getCurrentScopeStart() const {
    for( int i = current_size-1; i >= 0; i-- ) {
        if( table[ i ].kind == "FUN" ) {
            return i + 1; //first symbol after function declaration
        }
    }
    return 0; //global scope
}

//print a single symbol
void SymbolTable::print_symbol( int pos ) const {
    if( pos < 0 || pos >= current_size ) {
        cerr << "Invalid symbol position" << endl;
        return;
    }

    cout << "Symbol: " << table[ pos ].name 
         << " | Type: " << table[ pos ].type 
         << " | Kind: " << table[ pos ].kind
         << " | Scope: " << table[ pos ].scope
         << " | Address: " << table[ pos ].address << endl;
}

//print entire symbol table
void SymbolTable::printTable() const {
    const int col_width = 15;
    
    cout << left 
         << setw( col_width ) << "Name" 
         << setw( col_width ) << "Type" 
         << setw( col_width ) << "Kind"
         << setw( col_width ) << "Scope"
         << setw( col_width ) << "Address"
         << endl;
    
    cout << string( col_width * 5, '-' ) << endl;
    
    for (int i = 0; i < current_size; i++) {
        cout << left 
             << setw( col_width ) << table[ i ].name 
             << setw( col_width ) << table[ i ].type 
             << setw( col_width ) << table[ i ].kind
             << setw( col_width ) << table[ i ].scope
             << setw( col_width ) << table[ i ].address
             << endl;
    }
}
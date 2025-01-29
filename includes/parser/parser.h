#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring> 
#include <vector>
#include <cstring>
#include <queue>
#include <fstream>
#include <cctype>

using namespace std;
// #include "consts:b"
#include "../bplustree/map.h"
#include "../bplustree/multimap.h"
#include "../table/typedefs.h"
#include "../stack/MyStack.h"
// #include "parsed_constants.h"
#include "../queue/MyQueue.h"
// #include "../token/child_tokens.h"
#include "../binary_files/utilities.h"
#include "../binary_files/file_record.h"
#include "../shunting_yard/shunting_yard.h"
#include "../tokenizer/stokenize.h"


class Parser {
    public:
    Parser ();
    Parser (char s[]);
    mmap_ss parse_tree ();
    bool get_parse_tree (Queue <string> q);
    void set_string (char* str);

    void maketab ();
    void build_keys ();
    int get_col (string val);

    bool invalid (const string s);
    //TODO build keyword list

    private:
    

     enum keys {BEGIN, SELECT, FROM, WHERE, //2
                INSERT, INTO, CREATE, MAKE, //5
                TABLE, ASTERIK, QUOTE, FIELDS,//8
                COMMA, RELATIONAL_OPERATOR, LOGICAL_OPERATOR, //11?
                SYMBOL, TK_TABLE, LParen, Rparen, VALUES}; //14


    char * _command ;
    int adjecency_table [MAX_ROWS][MAX_COLUMNS];
    bool failyuh;
    mmap_ss _ptree;
    map_sl _keywords;
    Queue <string> input;
    
};


#endif // PARSER_H_
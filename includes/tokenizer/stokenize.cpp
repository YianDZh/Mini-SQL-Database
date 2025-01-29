#include "state_machine_functions.h"
#include "tokenmake.h"
#include "stokenize.h"
#include <iostream>
using namespace std;
int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];



    STokenizer::STokenizer(){
        _pos = 0;
        _buffer [0]= '\0';
        make_table (_table);
        actuallydone=false;
    }

    STokenizer::STokenizer(char str[]){
        //? Length or null?
        set_string (str);
        _pos = 0;
        make_table (_table);
        actuallydone=false;

    }

    bool STokenizer::done(){
        // bool ans = _pos > strlen(_buffer);
        // if(ans)
        //     _pos = 0;
        // return ans;
        return actuallydone;
    }            //true: there are no more tokens
    bool STokenizer::more(){
            return !done();
    }            //true: there are more tokens
    //

    //---------------
    //extract one token (very similar to the way cin >> works)
    STokenizer& operator >> (STokenizer& s, Tokenog& t){
        int start = 0;
        string as = "";
        // int prev_pos= s._pos;
        bool something = s.get_token (start,as);
        
        // if (something == false) s._pos++;


        if (s.actuallydone == false) {t= Tokenog (as,start);}
        // cout<<t.type_string()<<"\t"<<t<<endl;

        return s;
        // s.set_string(t.token_str().c_str());
    }

    //set a new string as the input string
    void STokenizer::set_string(char str[]){
        int i;
        for (i = 0; str[i] != '\0'; i++) {
            _buffer[i] = str[i];
        }
        _buffer[i] = '\0';
        /*
        cout<<"Actual string: ";
        for (i = 0; _buffer[i] != '\0'; i++) {
            cout<<_buffer[i];
        }
        */
        if (_buffer [i]=='\0')
            // cout<<"NULL\n";
        // cout<<endl;
        actuallydone=false;
        _pos = 0;
    }


    void STokenizer::make_table(int _table[][MAX_COLUMNS]){
    init_table(_table);
//? Number Related
    mark_cells (START_DOUBLE, _table, DIGITS, START_DOUBLE+1);// 0 -> 1
    mark_cell (START_DOUBLE+1,_table,'.',START_DOUBLE+2);// 1 -> 2
    mark_cells (START_DOUBLE+1, _table, DIGITS, START_DOUBLE+1); // 1 -> 1
    mark_cells (START_DOUBLE+2, _table, DIGITS, START_DOUBLE+3); // 2->3
    mark_cells (START_DOUBLE+3, _table, DIGITS, START_DOUBLE+3); //3->3

//? Alpha
    mark_cells (START_ALPHA, _table, ALFA, START_ALPHA+1);
    mark_cells (START_ALPHA+1, _table, ALFA, START_ALPHA+1);
//? Operators
    mark_cells (START_OPERATOR, _table, OPERATORS, START_OPERATOR+1);
    mark_cells (START_OPERATOR+1, _table, OPERATORS, START_OPERATOR+1);
//? Punctuations
    mark_cells (START_PUNC, _table, PUNC, START_PUNC+1);
    mark_cells (START_PUNC+1, _table, PUNC, START_PUNC+1);
//? Spaces
    mark_cells (START_SPACES, _table, SPACES, START_SPACES+1);
    mark_cells (START_SPACES+1, _table, SPACES, START_SPACES+1);

    mark_cells (80, _table, QUOTE, 80);
    mark_success(_table, 80); 
    mark_cells (85, _table, LPAREN, 86);
    mark_cells (86, _table, LPAREN, 2);
    mark_success(_table, 86); 
    mark_cells (90, _table, RPAREN, 91);
    mark_cells (91, _table, RPAREN, 2);
    mark_success(_table, 91); 
    
    mark_cells (100, _table, STAR, 100);
    mark_success(_table, 100); 

    mark_success(_table, START_ALPHA+1); 
    mark_success(_table, START_DOUBLE+1); 
    mark_success(_table, START_DOUBLE+3); 
    mark_success(_table, START_SPACES+1); 
    mark_success(_table, START_PUNC+1); 

    mark_success(_table, START_OPERATOR+1); 


    mark_fail(_table, 2); 

    
    // mark_cells(0, _table, ALFA, START_ALPHA); 
    // mark_cells(0, _table, DIGITS, START_DOUBLE); 
    // mark_cells(0, _table, OPERATORS, START_OPERATOR); 
    // mark_cells(0, _table, PUNC, START_SPACES); 
    // mark_cells(0, _table, SPACES, START_PUNC); 
    // mark_cells(0, _table, ALFA, TOKEN_ALPHA); 
    // mark_cells(0, _table, DIGITS, TOKEN_NUMBER); 
    // mark_cells(0, _table, OPERATORS, TOKEN_OPERATOR); 
    // mark_cells(0, _table, PUNC, TOKEN_PUNC); 
    // mark_cells(0, _table, SPACES, TOKEN_SPACE); 
    // mark_cells(TOKEN_ALPHA, _table, ALFA, TOKEN_ALPHA); 
    // mark_cells(TOKEN_NUMBER, _table, DIGITS, TOKEN_NUMBER); 
    // mark_cells(TOKEN_OPERATOR, _table, OPERATORS, TOKEN_OPERATOR); 
    // mark_cells(TOKEN_SPACE, _table, SPACES, TOKEN_SPACE); 
    // mark_cells(TOKEN_PUNC, _table, PUNC, TOKEN_PUNC); 

    // mark_cells(TOKEN_PUNC, _table, DIGITS, TOKEN_NUMBER); 
    // mark_cells(TOKEN_NUMBER, _table, PUNC, TOKEN_PUNC); 

    }

    //extract the longest string that match
    //     one of the acceptable token types
bool STokenizer::get_token(int& start_state, string& token) {
        return get_token_og (_table, _pos, start_state , _buffer,token, actuallydone);
    }
    
#ifndef CHILD_TOKENS_H_
#define CHILD_TOKENS_H_

#include <iostream>
#include "assert.h"
#include "../../includes/table/typedefs.h"
#include "token.h"
#include "string.h"
#include "../../includes/queue/MyQueue.h"
#include "../../includes/bplustree/multimap.h"
#include "../../includes/bplustree/btree_array_funcs.h"
#include <algorithm>

using namespace std;

//! Token String
class TokenStr:public Token {
    public: 
    TokenStr ();
    TokenStr (string something);
    void print ();
    // int get_type ();    
    string get_Value () const;
    // friend ostream& operator << (ostream& outs, Token* printMe);
    
    private:
    string _content;
    // int _type;
};

//!result_set
class Result_set: public Token {
    public:
    Result_set ();
    Result_set (vector<long> result);
    void print ();
    // int get_type ();    
    vector<long> get_Value () const;
    // friend ostream& operator << (ostream& outs, Token* printMe);

    private:
    vector<long> _result;
};

//! Relational
class Relational:public Token {
    public: 
    Relational ();
    Relational (string something);
    void print ();
    // int get_type ();    
    string get_Value () const;
    // friend ostream& operator << (ostream& outs, Token* printMe);
    Result_set* eval (const TokenStr field, const TokenStr value, vector<mmap_sl> _ind, map_sl look_up);
    private:
    string _content;
    // int _type;

};

//! Logical
class Logical:public Token {
    public: 
    Logical ();
    Logical (string something);
    void print ();
    string get_Value () const;
    
    // double get_Value () const;
    // friend ostream& operator << (ostream& outs, Token* printMe);
    vector <long> eval (const Result_set l, const Result_set r);
    
    private:
    string _content;
    // int _type;
};

//! Left Parent
class LeftParen: public Token{
    public:
    LeftParen ();
    void print ();
    LeftParen (string bug);
    
    private:
    string something;
    char _par;
};

//! Right Parent

class RightParen:public Token{
    public:
    RightParen ();
    RightParen (string newset);
    void print ();
    private:
    string _string;
    char _par;
};
vector<long> unionVectors(vector<long> vec1, vector<long> vec2);
vector<long> intersection(vector<long> v1, vector<long> v2);

#endif // CHILD_TOKENS_H_
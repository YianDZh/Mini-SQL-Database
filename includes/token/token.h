#ifndef TOKEN_H_
#define TOKEN_H_

#include <iostream>
#include "assert.h"
#include "string.h"
using namespace std;

class Token {
    public:
    Token ();
    Token (int type);
    Token (string types);
    Token  (int type, string types);

    //!Big 3
    // Token (const Token& copyMe);
    // ~Token ();
    // Token& operator =(Token &RHS);
    //!
    // virtual string to_string();
    // virtual int get_type ();    
    virtual string get_element () const;
    virtual int get_type () const;  
    virtual void print ();  

    friend ostream& operator << (ostream& outs, Token* printMe);
    friend ostream& operator << (ostream& outs, Token& printMe);
    
    private:
    string _types;
    int _type;
    // int _type;

};


#endif // TOKEN_H_
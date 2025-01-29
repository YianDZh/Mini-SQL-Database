#include "token.h"
#include "constants.h"

    Tokenog::Tokenog(){
        _token = "";
        _type = -1;
    }
    Tokenog::Tokenog(string str, int type){
        _token = str;
        _type =type;
    }
    ostream& operator<<(ostream& outs, const Tokenog& t){
        cout<<"|"<<t.token_str()<<"|";
        return outs;
    }
    int Tokenog::type() const{
        return this->_type;
    }
    string Tokenog::type_string() const{

        switch (this->type())
        {
        case TOKEN_ALPHA:
            return "ALPHA";
            break;        
        case TOKEN_NUMBER:
            return "NUMBER";
            break;        
        case TOKEN_SPACE:
            return "SPACE";
            break;       
        case TOKEN_OPERATOR:
            return "OPERATORS";
            break;        
        case TOKEN_PUNC:
            return "PUNC";
            break;
        case TOKEN_QUOTE:
            return "QUOTE";
            break;
        case TOKEN_LEFT:
            return "LPAREN";
            break;
        case TOKEN_RIGHT:
            return "RPAREN";
            break;
        case TOKEN_STAR:
            return "STAR";
            break;
        case TOKEN_UNKNOWN:
            return "unknown";
            break;
        }
    }
    string Tokenog::token_str() const{
        return this->_token;
    }
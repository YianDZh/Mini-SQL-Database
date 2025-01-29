#ifndef TOKENOG_H_
#define TOKENOG_H_
#include <iostream>
#include <ostream>
#include <fstream>
#include <cstring>
#include "string.h"

using namespace std;

class Tokenog
{
public:
    Tokenog();
    Tokenog(string str, int type);
    friend ostream& operator <<(ostream& outs, const Tokenog& t);
    int type() const;
    string type_string() const;
    string token_str() const;
private:
    string _token;
    int _type;
};


#endif // TOKEN_H_
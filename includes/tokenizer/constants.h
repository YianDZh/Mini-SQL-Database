#ifndef CONSTANTS_H_
#define CONSTANTS_H_

const int MAX_COLUMNS = 256;
const int MAX_ROWS = 100;
const int MAX_BUFFER = 200;

const char ALFA[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char DIGITS[] = "0123456789";
const char OPERATORS[] = "><=!+-%&|";
const char SPACES[] = {' ', '\t', '\n'};
const char PUNC[] = "?.,:;'`~!";
const char LPAREN []= "(";
const char RPAREN []= ")";
const char QUOTE []= "\"";
const char STAR []= "*";


const int START_DOUBLE = 0;
const int START_SPACES = 4;
const int START_ALPHA = 6;
const int START_OPERATOR = 20;
const int START_PUNC = 10;

//token types:
const int TOKEN_NUMBER = 1;
const int TOKEN_ALPHA = 2;
const int TOKEN_SPACE = 3;
const int TOKEN_OPERATOR = 4;
const int TOKEN_PUNC = 5;

const int TOKEN_LEFT = 7;
const int TOKEN_RIGHT = 9;
const int TOKEN_QUOTE= 11;
const int TOKEN_STAR= 8;


const int TOKEN_UNKNOWN = -1;
const int TOKEN_END = -2;

#endif // CONSTANTS_H_


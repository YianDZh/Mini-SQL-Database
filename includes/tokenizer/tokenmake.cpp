#include "tokenmake.h"
#include "constants.h"

bool get_token_og (int table [][MAX_COLUMNS], int& pos, int& state, char input [], string& token, bool &done){
    if (input [pos] == '\0') {
        done=true;
        return true;
    }
    //!!!!!
    if (isAlpha (input[pos])){
        state = START_ALPHA;
    }
    else if (isNumb (input[pos])){
        state = START_DOUBLE;
    }
    else if (isPunc (input [pos])){
        state = START_PUNC;
    }
    else if (isSpace(input[pos])){
        state=START_SPACES;
    }
    else if (isOperator(input[pos])){
        state=START_OPERATOR;
    }
    else if (input[pos]=='\"'){
        state = 80;
    }
    else if (input[pos]=='('){
        state = 85;
    }
    else if (input[pos]==')'){
        state = 90;
    }
    else if (input[pos]=='*'){
        state = 100;
    }
    else{
        state = TOKEN_UNKNOWN;
    }
    //!!!!!
    string temp = "";
    int i = pos;
    int last = -1; 
    int prev_sucess;
    int current=state;

    

while (input[i]!='\0'){

//checks for the available position

    if (table[current][input[i]] == TOKEN_UNKNOWN) {
            break;
        }
        current = table[current][input[i]];
        // cout<<"Current state: "<<current<<endl;
        // cout<<"Current char "<< input [i]<<endl;
        if (table [current][0]==1){
            // cout<<"Sucess "<<endl;
            prev_sucess = current;
            last = i;
        }
        // last = i;
        i++;
    }
    // pos=last+1;
        for (int j = pos; j <= last; j++) {
            temp += input[j];
        }
    //!update
    if (last!=-1){
        state = prev_sucess;
        fixer (state);
        pos = last+1 ;

        // cout<<"true\n";

        token = temp;

        return true;
    }
    else{
        pos++;
        return false;
    }    

    //!!!!!

}











/*#include "tokenmake.h"
#include "constants.h"

bool get_token_og(int table[][MAX_COLUMNS], int& pos, int& state, char input[], string& token) {
    string temp = "";
    int i = pos;
    int last = -1; 
    int next=0;
    if (pos > strlen(input)) {
        return false;
    }

    while (input[i] != '\0') {


        if (input [i]<0&&input[i]>=MAX_COLUMNS){
            break;
        }

        next = table[state][input[i]];
        

        if (next == TOKEN_UNKNOWN) {
            break;
        }



  if (state == TOKEN_NUMBER) {
            if (i + 1 < strlen(input) && (table[next][input[i + 1]] == TOKEN_PUNC || table[next][input[i + 1]] == 0)) {
                last = i;
                break;
            }
        }

    if (state == TOKEN_NUMBER) {
            if (i + 2 < strlen(input) && table[next][input[i + 1]] == TOKEN_PUNC || table[next][input[i + 1]] == 0) {
                if (table[next][input[i + 2]] != TOKEN_NUMBER) {
                    // last = i;
                    break;
                }
            }
        }

        state = next;
        last = i;
        i++;
    }

    if (last == -1) {
        return false;
    }


//  if (state == TOKEN_NUMBER && i < strlen(input) && table[next][input[i]] == TOKEN_PUNC) {
//         for (int j = pos; j < last; j++) {
//             temp += input[j];
//         }
//         token = temp;
//         pos = i + 1; 
//     } 
//     else {
        for (int j = pos; j <= last; j++) {
            temp += input[j];
        }
        token = temp;
        pos = last + 1;
    // }

    return true;
}*/
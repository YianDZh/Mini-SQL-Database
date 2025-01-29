#include "state_machine_functions.h"

//Fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS]){
    for (int i=0; i<MAX_ROWS; i++){
        for (int j = 0; j < MAX_COLUMNS; j++)
        {
            _table [i][j]=-1;
        }
        
    }
}

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state){
    _table [state] [0]= 1;
}

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS], int state){
    _table [state] [0]= 0;
}

//true if state is a success state
bool is_success(int _table[][MAX_COLUMNS], int state){
    return (_table [state][0]==1);
}

//Mark a range of cells in the array. 
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state){
    for (int i = from; i<=to;i++){
        //!check
        _table [row][i] = state;
    }
}

//Mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state){
    for(int i = 0; columns[i] != '\0'; i++){
        _table[row][columns[i]] = state;
    }
}

//Mark this row and column
void mark_cell(int row, int table[][MAX_COLUMNS], int column, int state){
    //!CHECK
    table[row][column] = state;
}

//This can realistically be used on a small table
void print_table(int _table[][MAX_COLUMNS]){
    for (int i=0; i<MAX_ROWS; i++){
        for (int j = 0 ; j <MAX_COLUMNS;j++){
            cout<< _table [i][j];
        }
        cout<<"\n";
    }
}

//show string s and mark this position on the string:
//hello world   pos: 7
//       ^
void show_string(char s[], int _pos){
 for(int i = 0; s[i] != '\0'; i++){
        cout << s[i];
    }
    cout << "   pos: " << _pos << endl;
    for (int i = 0; i != _pos || i != '\0'; i++){
        cout << " ";
    }
    cout << "^" << endl;
}


bool isOperator(char c) {
    for (int i = 0; i < sizeof(OPERATORS); i++) {
        if (c == OPERATORS[i]) {
            return true; 
        }
    }
    return false; 
}

bool isSpace(char c) {
    for (int i = 0; i < sizeof(SPACES); i++) {
        if (c == SPACES[i]) {
            return true; 
        }
    }
    return false; 
}

bool isNumb(char c) {
    for (int i = 0; i < sizeof(DIGITS); i++) {
        if (c == DIGITS[i]) {
            return true; 
        }
    }
    return false; 
}
bool isAlpha(char c) {
    for (int i = 0; i < sizeof(ALFA); i++) {
        if (c == ALFA[i]) {
            return true; 
        }
    }
    return false; 
}
bool isPunc(char c) {
    for (int i = 0; i < sizeof(PUNC); i++) {
        if (c == PUNC[i]) {
            return true; 
        }
    }
    return false; 
}
void fixer (int &state){
    if (state==1||state==3) state = TOKEN_NUMBER;
    else if (state == 5)      state = TOKEN_SPACE;
    else if (state == 7)    state = TOKEN_ALPHA;
    else if (state == 21)   state= TOKEN_OPERATOR;
    else if (state==11)     state = TOKEN_PUNC;
    else if (state == 80) state = TOKEN_QUOTE;
    else if (state == 85) state = TOKEN_LEFT;
    else if (state == 90) state = TOKEN_RIGHT;
    else if (state == 100) state = TOKEN_STAR;

    else state = TOKEN_UNKNOWN;
    
}

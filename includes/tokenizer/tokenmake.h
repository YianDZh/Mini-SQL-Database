#ifndef TOKENMAKE_H_
#define TOKENMAKE_H_
#include <iostream>
#include <iomanip>
#include "string.h"
#include "constants.h"
#include "state_machine_functions.h"
#include <cstring>
using namespace std;

bool get_token_og (int table [][MAX_COLUMNS], int& pos, int &state, char input [], string& token, bool &done );


#endif // TOKENMAKE_H_
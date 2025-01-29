// #include "gtest/gtest.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>

//------------------------------------------------------------------------------------------
//Files we are testing:
#include "includes/sql/sql.h"
using namespace std;

int main(int argv, char** argc) {
    cout << "\n\n"
         << endl;

    cout << "\n\n\n=====================" << endl;
    SQL sql;
    sql.run();

    return 0;
}
// bool test_stub(bool debug = false)
// {
//     SQL sql;
//      Table t;
//      cout << ">" << command_list[0] << endl;
//      sql.command(command_list[0]);
//      cout << "basic_test: table created." << endl<<endl;

//      for (int i = 0; i < command_list.size(); i++)
//      {
//           cout << ">" << command_list[i] << endl;
//           cout<< "here is yo table \n" << sql.command(command_list[i]);
//           cout << "basic_test: records selected: "<<sql.select_recnos() << endl;

//      }
//      return true;
// }
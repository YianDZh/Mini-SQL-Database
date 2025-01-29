#ifndef SQL_H_
#define SQL_H_
#include <iostream>
#include "../../includes/binary_files/file_record.h"
#include "../../includes/binary_files/utilities.h"
#include "../../includes/parser/parser.h"
#include "../../includes/table/table.h"
#include <iomanip>
#include "string.h"
#include <fstream>
#include <sstream>
#include "../../includes/binary_files/utilities.h"
#include <vector>
#include <cstring>

using namespace std;

class SQL{
    public:
    SQL ();
    SQL (string filename);
    Table command (string input);
    vector <long> select_recnos ();
    vectorstr get_table_names (fstream& f);
    void run();
    // void demo 
    private:
    string _sql_buffer;
    Table _table;
    Map<string, Table> _table_list;
    bool _flag;
};


#endif // SQL_H_
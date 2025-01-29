#ifndef TABLE_H_
#define TABLE_H_
#include <iostream>
#include <iomanip>
#include <string> 
#include <vector>
#include <cstring>
#include <queue>
#include <fstream>

using namespace std;
// #include "consts:b"
#include "../bplustree/map.h"
#include "../bplustree/multimap.h"
#include "typedefs.h"
#include "../stack/MyStack.h"
#include "../queue/MyQueue.h"
#include "../token/child_tokens.h"
#include "../binary_files/utilities.h"
#include "../binary_files/file_record.h"
#include "../shunting_yard/shunting_yard.h"




class Table//:public token
{
public:
    Table();
    Table(const string& name);
    Table(const string& name, const vector<string> &field_names); //vector<emap 35> indices;
    // void delete_table();
    void set_fields(vector<string> fld_names);
    vectorstr get_fields() const;
    bool is_empty() const;
    int insert_into (vectorstr& fields); //return row number
    int field_col_no(string field_name);
    int field_col_no(string field_name) const;
    Table select_all();
    Table select_all(vectorstr fields);
    // Table select(const vectorstr& fields, const Queue<Token *>& postfix); 

    // Table select(const vectorstr& fields, const string& field, 
    //             const string& op, const string& value,
    //             const mmap_ss& p_tree = mmap_ss());

    Table select(const vectorstr& fields, const string& field, const string& op, const string& value);
    Table select(const vectorstr& fields, const Queue<Token *>& postfix);
    Table select(const vectorstr& fields, const vectorstr& condition);

    // void build_keyword_list(map_sl &list);
    int get_token_type(const string& s); //NOT USED!
    void reindex(); //call this from ctor!
    string Name() const;
    friend ostream& operator << (ostream& outs, const Table& t); 
    vectorlong cond (const Queue<Token*> &post);
    Table vector_to_table(const vectorstr &ftelds, const vectorlong &vector_of_recnos);
    int total_record ();
    vectorstr ordered_fields (vectorstr v, const Table T) const;
    string get_name ()const{
        return _name;
    }



vector<long> select_recnos() const;
private:
    vector<long> _select_recnos;
    static int serial;
    enum OPERATORS {EQUAL,
                    LESS_THAN,
                    GREATER_THAN,
                    LESS_EQUAL,
                    GREATER_EQUAL,
                    NOT_EQUAL,
                    AND,
                    OR,
                    NOT
                    };
    enum keys {SELECT, FROM, WHERE, //2
                INSERT, INTO, CREATE, //5
                TABLE, ASTERIK, QUOTE, //8
                COMMA, RELATIONAL_OPERATOR, LOGICAL_OPERATOR, //11?
                SYMBOL, STRING, NUMBER, TK_TABLE}; //14
    string _name;
    string _file_name;

    string original_fields;

    map_sl _keywords;
    vector <mmap_sl> _indices;
    bool _empty;
    vectorstr _field_names;
    map_sl original_field_names;
    map_sl _field_map;

    //vector<Record> rows;
    map_sl OPERATOR_CODES;
    // map_st OPERATOR_LOGICAL;
    // map_st OPERATOR_RELATIONAL;
    //! Might be a function
    long _last_record;
    int _count;
    static const bool DEBUG = false;

    
    void set_fields_initial(vector<string> fld_names);

};


#endif // TABLE_H_
#include "parser.h"
#include <stack>


    Parser::Parser (){
        maketab();
        build_keys ();
    }

    Parser::Parser (char s[]){
        // _command = s;
        maketab();
        build_keys ();
        set_string (s);
    }

    mmap_ss Parser::parse_tree (){
        return _ptree;
    }
 
    bool Parser::get_parse_tree (Queue <string> q){
        int state=0;
        while (!(q.empty())){
            string current = q.pop();
            // state = adjecency_table [state][get_col(q.front())];
            state = adjecency_table [state][get_col(current)];

            // cout<<state<<endl;
            // cout<<current<<endl;
            switch(state){
                case (0):
                    throw (string ("Fail state\n"));
                    break;
                case (-1):
                    throw (string ("invalid input, unrecognized\n"));
                    break;
                case (1):
                _ptree["command"]+=current;
                break;
                // case (INTO):
                // case (FROM):
                case (2):
                case (3):
                case (8):
                case (5):
                case(10):
                // q.pop();
                break;
                // case (3):
                // _ptree["fields"]+=current ;
                // break;
                case(4):
                case(6):
                case(70):
                _ptree ["table_name"]+=current;
                break;

                case (7):
                //make values
                _ptree ["col"]+=current;
                break;
                case (32):
                //insert values
                _ptree  ["values"]+=current;
                break;
                case (20):
                case (40):
                _ptree ["fields"]+=current;
                break;
                case (71):
                current = "yes";
                _ptree ["where"] += current;
                break;
                case (50):
                _ptree ["condition"]+=current;
                break;
            }
        }
        return (state!=0||state !=-1);
    
    }


    int Parser::get_col (string val){
        for (char &c : val) {
        c = tolower(c);
        }
        if (_keywords.contains (val)){
            return _keywords[val];
        }
        else {
            return SYMBOL;
        }
    }
    
    void Parser::set_string (char*  str){
        _command = str;
        // for (int i=0;i < )
        // cout<<_command<<endl;
        STokenizer I (_command);
        Queue <string> tokenq;
        Tokenog read;
        bool flag = false;
        string hold;
        I >> read;
        while (I.more()){

            // cout<<read<<endl;
            if (read.type_string()=="QUOTE"){
                flag = !flag;
            }
            if (flag){
                read = Tokenog();
                I>>read;
                if (read.type_string()=="QUOTE"){
                    flag = false;
                    // cout<<"Found next\n";                   
                    // cout<<hold<<endl;
                    tokenq.push (hold);
                    hold.clear();
                    read = Tokenog();
                    I>>read;
                    continue;
                }
                hold +=read.token_str();
                // cout<<hold<<endl;
                continue;
            }

                 
            if(read.type_string()!= "SPACE"){
                tokenq.push (read.token_str());
            }
            read = Tokenog();
            I>>read;
        }
        // cout<<tokenq<<endl;
        _ptree.clear();
        get_parse_tree (tokenq);
        /*   char s[] = 
            "So, it was the night of october 17th. pi was still 3.14. sigh! 2.";
    char s[] = "it";
    cout << "\n\ntokenizing: [" << s << "]\n" << endl;
    STokenizer stk(s);
    Token t;
    stk>>t;
    while(stk.more()){
        //process token here...
        cout<<setw(10)<<t.type_string()<<setw(10)<<t<<endl;


        t = Token();
        stk>>t;
    }
*/
    }

    void Parser::build_keys (){
        _keywords ["select"] = SELECT;
        _keywords ["from"] = FROM;
        _keywords ["where"] = WHERE;
        _keywords ["insert"] = INSERT;
        _keywords ["into"] = INTO;
        _keywords ["create"] = CREATE;
        _keywords ["make"] = MAKE;
        
        // _keywords ["make"] = MAKE;

        _keywords ["table"] = TABLE;
        _keywords ["*"] = ASTERIK;
        _keywords ["fields"] = FIELDS;
        _keywords ["values"] = VALUES;

        _keywords [","] = COMMA;
        // _keywords ["("] = LParen;
        // _keywords [")"] = Rparen;
        // _keywords ["and"] = LOGICAL_OPERATOR;
        // _keywords ["or"] = LOGICAL_OPERATOR;

        // _keywords ["<"] = RELATIONAL_OPERATOR; 
        // _keywords [">"] = RELATIONAL_OPERATOR; 
        // _keywords ["="] = RELATIONAL_OPERATOR; 
        

    }



                
    void Parser::maketab (){
        init_table (adjecency_table);
        //?create
        mark_cell (0, adjecency_table, CREATE, 1);
        mark_cell (0, adjecency_table, MAKE, 1);
        //!  Table name for make 51, field names 52, 53 commas
        mark_cell (1, adjecency_table, TABLE, 2);
        mark_cell (2, adjecency_table, SYMBOL, 4);
        mark_cell (4, adjecency_table, FIELDS, 3);
        mark_cell (3, adjecency_table, SYMBOL, 7);
        mark_success (adjecency_table, 7);
        mark_cell (7, adjecency_table, COMMA, 8);
        mark_fail (adjecency_table, 8);
        mark_cell (8, adjecency_table, SYMBOL, 7);


        //? insert
        //! 31 table name, 32 names, 33 is commas
        mark_cell (0, adjecency_table, INSERT, 1);
        mark_cell (1, adjecency_table, INTO, 10);
        mark_cell (10, adjecency_table, SYMBOL, 6);
        mark_cell (6,adjecency_table,VALUES,5);
        mark_cell (5,adjecency_table,SYMBOL,32);
        mark_cell (32, adjecency_table,COMMA,33);
        mark_success (adjecency_table, 32);
        mark_fail (adjecency_table, 33);
        mark_cell (33, adjecency_table, SYMBOL, 32);

        //? Select
        mark_cell (0, adjecency_table, SELECT, 1);
        mark_cell (1,adjecency_table, ASTERIK, 20);
        mark_cell (20, adjecency_table, FROM, 21);
        mark_cell (21,adjecency_table, SYMBOL, 70);
        mark_success (adjecency_table, 70);
        mark_cell (70, adjecency_table, WHERE, 71);
        mark_cell (1,adjecency_table, SYMBOL, 40);
        mark_cell (40, adjecency_table, COMMA, 41);
        mark_cell (40, adjecency_table, FROM, 21);
        mark_cell (41, adjecency_table, SYMBOL, 40);
        mark_cell (71, adjecency_table, SYMBOL, 50);
        mark_cell (50, adjecency_table, SYMBOL, 50);
        mark_success (adjecency_table, 50);


    /*

        //!select star
        mark_cell (2, adjecency_table, FROM, 3);
        mark_cell (3, adjecency_table, SYMBOL, 60);
        mark_cell (60, adjecency_table, WHERE, 61);
        mark_cell (61, adjecency_table, SYMBOL, 62);
        mark_cell (62, adjecency_table, RELATIONAL_OPERATOR, 63);
        mark_cell (62, adjecency_table, LOGICAL_OPERATOR, 64);
        
        mark_cell (64, adjecency_table, LOGICAL_OPERATOR, 64);
        mark_cell (64, adjecency_table, SYMBOL, 62);
        mark_cell (63, adjecency_table, SYMBOL, 62);
        mark_cell (63, adjecency_table, RELATIONAL_OPERATOR, 62);
*/

/*

/ SELECT -> SYMBOL(fields) -> FROM -> SYMBOL(table_name)
   SELECT -> SYMBOL(fields) -> FROM
   SELECT -> STAR -> FROM
   FROM -> SYMBOL(table_name)
   SYMBOL(table_name) -> WHERE -> SYMBOL(condition) 
   SYMBOL -> OP -> SYMBOL
   LOG -> LOOP -> LOG
   SOS LOOP:
  ACCEPT PARENTHESIS BEFORE/AFTER SOS:

        mark_cell (9, adjecency_table, SYM, 9);

        mark_fail (adjecency_table, 0);
        mark_fail (adjecency_table, SELECT);
        mark_fail (adjecency_table, FROM);
        mark_fail (adjecency_table, WHERE);

        mark_fail (adjecency_table, INSERT);
        mark_fail (adjecency_table, INTO);
        mark_fail (adjecency_table, FROM);
        mark_fail (adjecency_table, ASTERIK);
        mark_fail (adjecency_table, QUOTE);
        
    ? fail states 0,1,2,3
    mark_fail(_table, 0);   
    mark_fail(_table, 1);            
    mark_fail(_table, 2);             
    mark_fail(_table, 3);             

    ?success states 4,5,6,7
    mark_success(_table, 4);
    mark_success(_table, 5);
    mark_success(_table, 6);
    mark_success(_table, 7);

    mark_cells(0, _table, SELECT, 1);   //? 0 -> select -> 1 
    mark_cells(0, _table, MAKE, 1);     // 0 -> make -> 1 
    mark_cells(0, _table, INSERT, 1);   // 0 -> insert -> 1 


    mark_cells(1, _table, STAR, 2);     // 1 -> * -> 2
    mark_cells(1, _table, SYMBOL, 2);   // 1 -> symbol -> 2

    mark_cells(1, _table, INTO, 3);     // 1 -> into -> 3
    mark_cells(1, _table, TABLE, 3);    // 1 -> table -> 3

    mark_cells(2, _table, FROM, 3);     //2 -> from -> 3
    mark_cells(2, _table, SYMBOL, 2);   //2 -> symbol -> 2

    mark_cells(3, _table, SYMBOL, 4);   //3 -> symbol -> 4
    mark_cells(4, _table, SYMBOL, 4);   //4 -> symbol -> 4

    mark_cells(4, _table, WHERE, 5);    //4 -> where -> 5
    mark_cells(4, _table, VALUES, 6);   //4 -> values -> 6
    mark_cells(4, _table, FIELDS, 7);   //4 -> fields -> 7

    mark_cells(5, _table, SYMBOL, 5);   //5 -> symbol -> 5

    mark_cells(6, _table, SYMBOL, 6);   //6 -> symbol -> 6

    mark_cells(7, _table, SYMBOL, 7);   // 7 -> symbol -> 7

*/

    }
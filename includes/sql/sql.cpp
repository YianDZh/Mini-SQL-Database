#include "sql.h"


SQL::SQL (){

}
SQL::SQL (string filename){
    
    fstream f;
    f.open (filename);
    string temp;
    if (f.fail()){
            cout<<"batch file failed"<<filename<<endl;
            throw("file RW failed  ");
    }
    vectorstr names = get_table_names(f);
    cout<<"----- SQL MANAGER---------\n";
   
    for (int i = 0; i<names.size();i++){
        cout<<names[i]<<endl;
    }
    cout <<"-----------------------------------\n";
    f.close();
    f.open (filename);
    cout<<"> "<<filename<<endl;
    int count = 0;
    while (getline (f, temp)) {
  // Output the text from the file
        if (temp.rfind("//", 0) == 0) { 
            cout<<temp<<endl;
        }
        else if (temp.rfind("make", 0) == 0||temp.rfind("create", 0) == 0||temp.rfind("insert", 0) == 0) {
            cout << "[ "<<count<<" ]" << temp << endl;
            this->command(temp);
            count++;
        }
        else if (temp.rfind("select", 0) == 0){
            cout << "[ "<<count<<" ]" << temp << endl;
            cout<<this->command(temp)<<endl;
            count++;

        }
    }
    cout<<"SQL: DONE. \n"; 
    cout<<"\n\n\n\n------- END OF BATCH PROCESS ------->\n";
}

Table SQL::command (string input){
    
    char hold [input.length()+1];
    // vectorstr options = {command, };
    Parser parse;
    try {
        // Attempt to parse the input
        parse.set_string(strcpy (hold, input.c_str()));
    } 
    catch (const string& e) {
        // Handle exceptions from the Parser
        cout << "Parser error: " << e << endl;
        cout << "Returning empty table.\n";
        throw (1);
    }
    mmap_ss ptree = parse.parse_tree();
    string order = ptree ["command"] [0];
    // ptree.print_lookup();
    cout<<endl;

    try {
        if (order != "make" && order != "insert" && order != "select") 
            throw string("Invalid input!");  
        if (ptree["table_name"].size()>1){
            throw string("Invalid table_name fields!");  
        }  
        if (ptree["table_name"].size()==0){
            throw string("No table_name found!");  
        }
        if (ptree["condition"].size()==1 || ptree ["condition"].size()==2){
            throw string("Small condition and non functional!");  
        }          
    }
    catch (string extra) {
        cout << extra<<endl;
        throw (2);
    }


    if (order == "make"||order == "create"){
        _table = Table (ptree["table_name"][0], ptree ["col"]);
    }
    
    else if (order == "insert"){
        if (ptree["table_name"][0]!=_table.get_name()){
            //TODO maybe source of problems
            _table = Table (ptree ["table_name"][0]);
        }
        _table.insert_into (ptree ["values"]);
    }

    else if (order == "select"){
        if (ptree["table_name"][0]!=_table.get_name()){
            //TODO maybe source of problems
            _table = Table (ptree ["table_name"][0]);
        }
        if (ptree["fields"][0] == "*"){
            if (ptree ["condition"].size()==0){
                cout<<"There is no condition\n";
                // cout<< _table.select_all ()<<endl;
                return _table.select_all();
            }
            else {

                return _table.select (_table.get_fields(),ptree ["condition"]);
                // return _table.select_all (ptree ["condition"]);
            }
        }
        else if (ptree["fields"].size()>=1){
            if (ptree ["condition"].size()==0){
                return _table.select_all(ptree ["fields"]);
            }
            else {
                
                return _table.select (ptree ["fields"],ptree ["condition"]);
            }
        }
    }
    else {
        //TODO error message
        cout<<"Someone fucked up\n";
        exit (99999);
    }
    return _table;
}


vector <long> SQL:: select_recnos (){
    return _table.select_recnos();
}

void SQL::run(){
    string input="";
    char t ='n';
    cout<<"================================================\n We  got SQL at home, SQL at home:\n================================================\n";
    Table tab;
    do {
        input.clear();
        cout<<"Please input your command:\n";
        getline(cin, input);
        try
        {
            tab = command (input);        
        }
        catch(int result)
        {
            if (result == 1)
                cout<<"Invalid Input"<<endl;
            else if (result ==2 )
                cout<<"Incomplete condition"<<endl;
            input.clear();
            continue;
        }
        
        cout<<"Here is your table: \n";
        cout<<"> "<<input<<endl;
        cout<<tab<<endl;
        cout<<"Would you like to continue?\n";
        cin>>t;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear the buffer
        cout<<endl;
    }
    while (t=='y'||t=='Y');

    cout<<"================================================\nTerminated SQL on users command\n";
    cout<<"Have a nice day :)\n================================================\n";
}


vectorstr SQL::get_table_names (fstream &f){
    string temp;
    int nam = 0;
    vectorstr names;
    while (getline (f, temp)) {
        istringstream reader (temp);
        string single;

        if (temp.rfind("make", 0) == 0||temp.rfind("create", 0) == 0){
            // cout<<temp<<endl;
            while (reader >> single) {
                // cout<<single<<endl;
                nam++;
                if (nam == 3) {
                    if (find(names.begin(), names.end(), single) == names.end()) {
                        // cout<<"pushing "<<single<<endl;
                        names.push_back(single);
                    }
                    nam = 0;
                    break;
                }
            }
        }
    }
    return names;
}
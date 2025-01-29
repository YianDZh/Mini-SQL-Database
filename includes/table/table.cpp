#include "table.h"
    int Table::serial = 0;

    Table::Table(){
        serial++;
    }

    Table::Table(const string& name){
        serial++;
        //! Beware
        _name = name;
        _file_name = _name+"_fields";
        original_fields = _file_name;

        total_record();
        reindex ();
        // exit(0);
        original_field_names = _field_map;

        // cout<<"reindxing\n";
        //TODO Write info into the files after reindexing
        // cout<<serial<<endl;
        // fstream f;
        // open_fileW (f, name.c_str());

    }

    Table::Table(const string& name, const vector<string> &field_names){
  
        serial ++;
        _name = name;   
        _file_name = name + "_fields"; 
        original_fields = _file_name;
        set_fields_initial (field_names);
        
        //? Bin File creation and written
        fstream f;
        open_fileW (f, name.c_str());
        f.close();
        //? txt file
        fstream g;
        open_fileW_txt (g, _file_name.c_str());
        for (const auto& line : field_names) {
            g << line << endl; 
        }
        g.close();
        _count = 0;
        _last_record = _count-1;
        _field_names = vector_from_file(_file_name);
        original_field_names = _field_map;

        //!maybe just building

    } 
    //vector<emap 35> indices;

    // void Table::delete_table(){

    // }

    void Table::set_fields_initial(vector<string> fld_names){
        _field_names = fld_names;
        for (long i = 0; i < fld_names.size();i++){
            _indices.push_back(mmap_sl());
            // cout << "fld_namrd: " << fld_names[i] << endl; 
            _field_map [fld_names[i]]=i;
        }
    }
    
    void Table::set_fields(vector<string> fld_names){
    //clear
        _field_names = fld_names;
    //modify field map and file        
        for (long i = 0; i < fld_names.size();i++){
            _indices.push_back(mmap_sl());
            _field_map [fld_names[i]]=i;
        }
    }

    vectorstr Table::get_fields() const{
        return _field_names;
    }
    bool Table::is_empty() const{
        return _count == 0;
    }
    int Table::insert_into (vectorstr& fields){
        fstream f;
        open_fileRW (f, _name.c_str());
        FileRecord fr (fields);
        int cond = fr.write (f);
        for (int i = 0 ; i<fields.size();i++){
            _indices [i][fields[i]].push_back(cond);
        }
        _count++;
        _last_record++;
        return cond;
    } //return row number
    int Table::field_col_no (string field_name){
        return _field_map[field_name];
    }

    int Table::field_col_no (const string field_name) const{
        return _field_map.at(field_name);
    }

    Table Table::select_all(){
        _select_recnos.clear();
        for (int i = 0; i<_count;i++){
            _select_recnos.push_back(i);
        }
        return vector_to_table(_field_names,_select_recnos);
        /*
        vector<long> select;
        for (int i = 0; i<_count;i++){
            select.push_back(i);
        }
        return vector_to_table(fields,select);
        */
    }
    Table Table::select_all(vectorstr fields){ 
        _select_recnos.clear();
        for (int i = 0; i<_count;i++){
            _select_recnos.push_back(i);
        }
        return vector_to_table(fields,_select_recnos);
    }
    Table Table::select(const vectorstr& fields, const Queue<Token *>& postfix){
        _select_recnos.clear();
        _select_recnos = cond (postfix);
        // cout<<"Selected :"<<_select_recnos<<endl;
        
         return vector_to_table (fields,_select_recnos);
    } 
    Table Table::select(const vectorstr& fields, const vectorstr& condition){
        // cout<<condition<<endl;
        ShuntingYard sy (condition);
        Queue <Token*> hold = sy.postfix ();
        // cout<<"returned from shunting yard\n";
        return select (fields, hold);
    }

    Table Table::select(const vectorstr& fields, const string& field, const string& op, const string& value){
        int index = field_col_no (field);
        _select_recnos.clear();
        if (op=="="){
            _select_recnos = _indices [index][value];
        }
        else if (op == "<"){
            for (mmap_sl::Iterator it = _indices [index].begin(); it != _indices [index].lower_bound (value);++it ){
                for (int i=0; i<(*it).value_list.size();i++){
                    _select_recnos.push_back ((*it).value_list[i]);
                }

            }
        }
        else if (op == ">"){
            for (mmap_sl::Iterator it = _indices [index].upper_bound(value); it != _indices [index].end ();++it ){
                for (int i=0; i<(*it).value_list.size();i++){
                    _select_recnos.push_back ((*it).value_list[i]);
                }

            }
        
        }
        else if (op ==">="){
            for (mmap_sl::Iterator it = _indices [index].lower_bound(value); it != _indices [index].end();++it ){
                for (int i=0; i<(*it).value_list.size();i++){
                    _select_recnos.push_back ((*it).value_list[i]);
                }

            }

        }
        else if (op=="<="){
            for (mmap_sl::Iterator it = _indices [index].begin(); it != _indices [index].upper_bound (value);++it ){
                for (int i=0; i<(*it).value_list.size();i++){
                    _select_recnos.push_back ((*it).value_list[i]);
                }

            }
        }
        else{
            //!try and catch maybe
            exit (0);
        }
//switch
        return vector_to_table (fields,_select_recnos);

    }

    // void Table::build_keyword_list(map_sl &list){

    // }

    // int Table::get_token_type(const string& s){

    // }  
    //!NOT USED!

    void Table::reindex(){
        fstream f;
        open_fileRW(f, _name.c_str());
        //!check for count
       _field_names = vector_from_file (_file_name.c_str());

        for (long i = 0; i < _field_names.size();i++){
            _indices.push_back(mmap_sl());
            _field_map [_field_names[i]]=i;
        }

        for (int i = 0; i < _count; i++)
        {
            FileRecord r;
            int temp = r.read(f, i);
            // cout<<"Opened bin file fr reading\n";

            // push each record into the attribute mmaps
            vectorstr tempo = r.record_info();
            for (int j = 0; j < tempo.size(); j++){
                // cout<<tempo[j]<<"\t";
                _indices [j][tempo[j]].push_back (i);
                // cout<<"Pushed into vector\n";
            }
            // push_into_attribute_mmaps(r._record, i);
        }

        // cout<<"Done w the loop\n";
        f.close();
    } //call this from ctor!
    
    ostream& operator << (ostream& outs, const Table& t){
        cout<<"\n";
        if (t.is_empty()){
            return outs;
        }
   
        //?Might not need it

        cout<<"record\t";
        for (int i = 0; i<t._field_names.size();i++){
            cout<< t._field_names [i]<<"\t";
        }
        cout<<"\n";

        fstream g;
        open_fileRW (g, t._name.c_str());
        fstream text;
        open_fileRW_txt (text, t._file_name.c_str()); 
        for (int i=0; i<t._count;i++){
            FileRecord fr;
            int temp = fr.read(g, i);
            // push each record into 
            cout<<i<<"\t";
            vectorstr v = fr.record_info();
            cout<<v<<endl;
            // cout<< t.ordered_fields (v,t)<<endl;
            // for (int j = 0; j < v.size(); j++){
            //     string key = vector_from_file (t._file_name.c_str())[j];
            //     int extra = t.original_field_names.at(key);
            //     // int mega = t._field_map.at(vector_from_file (t._file_name.c_str())[j]);

            //     cout<<v[extra]<<"\t";
            //     // cout<<fr.record_info()[mega]<<"\t";

            // }
            // cout<<"\noriginal file name: "<<t.original_fields<<endl;
            // cout<<"file name: "<<t._file_name<<endl;

                // cout<<endl<<fr.record_info()<<endl;
                //  cout<<vector_from_file(t.original_fields.c_str())<<endl;
                // cout<<vector_from_file(t._file_name.c_str())<<endl;
               

            cout<<"\n";
        }

        return outs;
        //file record
        //
    } 
    
    vectorlong Table::cond (const Queue<Token*> &post){
        Stack <Token*> rpn;
        for (Queue<Token*>::Iterator it = post.begin(); it!= post.end();it++){
            Token* current = *it;
            // cout<<"Current token: ";
            // current->print();
            // cout<<endl<<"Type is: "<<current->get_type()<<endl;
            // cout<<rpn<<endl;
            if (current->get_type()==8){
                Relational* temp = static_cast <Relational*> (current);
                TokenStr* key = static_cast <TokenStr*> (rpn.pop());
                TokenStr* field = static_cast <TokenStr*> (rpn.pop());
                current = temp->eval (*field, *key, _indices, _field_map);
            }
            else if (current->get_type()==7){
                Logical* temp = static_cast <Logical*> (current);
                Result_set* top = static_cast <Result_set*> (rpn.pop());
                Result_set* bottom = static_cast <Result_set*> (rpn.pop());
                current = new Result_set (temp->eval (*top, *bottom));

            }
            rpn.push (current);
        }
        Result_set* ans = static_cast <Result_set*> (rpn.pop());
        return ans->get_Value();
    }

    Table Table::vector_to_table(const vectorstr &ftelds, const vectorlong &vector_of_recnos){
        // serial++;
        // Table ans (_name+"_"+to_string(serial), ftelds);
        // cout<<"Received vector str: "<< ftelds<<endl;
        // cout<<"Received vector long: "<< vector_of_recnos<<endl;

        Table ans("sample" , ftelds);
        fstream f;
        // cout<<original_fields<<endl;
        ans.original_fields = _file_name;
        // cout<<original_fields<<endl;
        ans.original_field_names = _field_map;
        
        FileRecord fr;
        open_fileRW (f, _name.c_str());

        //writing into the new file
        for (int i = 0;i< vector_of_recnos.size();i++){
            
            fr.read (f, vector_of_recnos[i]);
        
            vectorstr v = fr.record_info();
            vectorstr hold = ordered_fields (v,ans);
            // cout<<"Refactored vector"<<hold<<endl;
            // cout<< hold<<endl;
            
            ans.insert_into (hold); 
        }

        f.close ();
        // cout<<"Finished making\n";
        return ans;
    }

    int Table::total_record (){
        fstream f;
        open_fileRW(f, _name.c_str());
        FileRecord fr;
        for (_count = 0; fr.read(f, _count); _count++)
        {
        }
        return _count;
    }

    vectorstr Table::ordered_fields (vectorstr v, const Table T)const {
        vectorstr temp;
        for (int j = 0; j < T._field_names.size(); j++){
                string key = vector_from_file (T._file_name.c_str())[j];
                int index = T.original_field_names.at(key);
                // cout<<key<<endl;
                // cout<<v[extra]<<endl;
                temp.push_back (v[index]);
        }
        return temp;
    }
string Table::Name() const{
    return _name; 
}
vector<long> Table::select_recnos() const
{ 
    return _select_recnos; 
    }
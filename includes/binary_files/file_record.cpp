#include "file_record.h"

long FileRecord::write(fstream &outs){
    //r.write(f); //take the Record r and write it into file f
    //  and return the record number of this Record

    //write to the end of the file.
    outs.seekg(0, outs.end);


    long pos = outs.tellp();    //retrieves the current position of the
                                //      file pointer

    //outs.write(&record[0], sizeof(record));
    outs.write(&_record [0][0], sizeof(_record));

    return pos/sizeof(_record);  //record number
}

long FileRecord::read(fstream &ins, long recno){
    //returns the number of bytes read.
    //    r.read(f, 6);
    //    cout<<r<<endl;
    long pos= recno * sizeof(_record);
    ins.seekg(pos, ios_base::beg);

    //ins.read(&_record[0], sizeof(_record))
    ins.read(&_record[0][0], sizeof(_record));
    //don't you want to mark the end of  the cstring with null?
    //_record[] => [zero'\0'trash trash trash trash]
    //don't need the null character, but for those of us with OCD and PTSD:
    // _record[recno][ins.gcount()] = '\0';
    return ins.gcount();

}

int FileRecord::many_valid () const{
    int result = 0;
    for (int i=0; i<101;i++){
        if (* (_record [i]) != '\0'){
        result ++;
        }
    }
    return result;
}
ostream& operator<<(ostream& outs,const FileRecord& r){  
    // cout<<r.many_valid()<<endl;  
    for (int i = 0; i<r.many_valid();i++){
            outs<<r._record[i]<<"\t";
    }
        return outs;

}

vector <string> FileRecord::record_info (){
    // string temp;
    vector <string> a;
    for (int i = 0 ; i < many_valid ();i++){
            // temp = _record[i];
            a.push_back ( _record[i]);
    }
    return a;
}

void FileRecord::refac (vector<long> bs){
    // vector <string> temp;
    // for (int i = 0;i<bs.size();i++){
    //     temp.push_back(_record[i]);
    // }

    // for(int i = 0; i <many_valid();i++){
    //     strcpy(_record[i],"\0");
    // }   

    // for(int i = 0; i <temp.size();i++){
    //     strcpy(_record[i], temp[i].c_str());
    // }
}




#ifndef FILE_RECORD_H_
#define FILE_RECORD_H_
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <string>
#include <vector>
using namespace std;

struct FileRecord{
    //when you construct a Record, it's either empty or it
    //  contains a word
    FileRecord(){
        for(int i = 0; i<MAX+1;i++){
            for(int j = 0; j<MAX+1;j++){
                _record [i][j]='\0';
            }
        }
        // _record[0] [0]= '\0';
        recno = -1;
    }

    FileRecord(char str[]){
        for(int i = 0; i<MAX+1;i++){
            for(int j = 0; j<MAX+1;j++){
                _record [i][j]='\0';
            }
        }
        //copies string into record
        strncpy(_record[0], str, MAX);
        // num_field = 1;

    }
    FileRecord(string s){
       for(int i = 0; i<MAX+1;i++){
            for(int j = 0; j<MAX+1;j++){
                _record [i][j]='\0';
            }
        }
        strncpy(_record[0], s.c_str(), MAX);
        // num_field = 1;

    }

    FileRecord (vector <string> v){
        for(int i = 0; i<MAX+1;i++){
            for(int j = 0; j<MAX+1;j++){
                _record [i][j]='\0';
            }
        }
        for (int i=0 ;i <v.size();i++){
            strncpy (_record [i], v[i].c_str(),MAX);
        }
        // num_field = v.size();
    }

    int many_valid () const;

    long write(fstream& outs);              //returns the record number
    long read(fstream& ins, long recno);    //returns the number of bytes
                                            //      read = MAX, or zero if
                                            //      read passed the end of file
    friend ostream& operator<<(ostream& outs, const FileRecord& r);
    vector <string> record_info ();
    void refac (vector<long> bs);
// private:
    static const int MAX = 100;
    int recno;
    char _record[MAX+1][MAX+1];


};

#endif // FILE_RECORD_H_
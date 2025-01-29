#ifndef UTILITIES_H_
#define UTILITIES_H_
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <vector>
#include <string>
using namespace std;

//* FILE FUNCTIONS SIGNATURES
bool file_exists(const char filename[]);
void open_fileRW(fstream& f, const char filename[]) ;
void open_fileRW_txt(fstream& f, const char filename[]);
void open_fileW(fstream& f, const char filename[]);
void open_fileW_txt(fstream& f, const char filename[]);
vector<string> vector_from_file (const string& _filename);




#endif // UTILITIES_H_
#include "ftokenize.h"

    FTokenizer::FTokenizer(char* fname){
        _f.open (fname);
        if (!_f){
            cerr << "Can't open input file!\n";
        }
        else {
            cout<<"Opened\n";
        }
        _more = true;
        _pos = 0;
        _blockPos = 0;
        get_new_block ();
    }



    Tokenog FTokenizer::next_token(){
        Tokenog t;
        if (_stk.done()){
            _more = get_new_block ();
        }
        _stk>>t;
        return t;
    }
    bool FTokenizer::more(){
        if (!_more){
            if (_f.is_open()) {
            _f.close();
        }
            // _f.close();
        }
        return _more;
    }        //returns the current value of _more

    int FTokenizer::pos(){
        return _pos;
    }          //returns the value of _pos

    int FTokenizer::block_pos(){
        return _blockPos;
    }     //returns the value of _blockPos

    FTokenizer& operator >> (FTokenizer& f, Tokenog& t){
        t = f.next_token ();
        return f;
    }

    bool FTokenizer::get_new_block(){
        // cout<<"Something\n";
        char block [MAX_BUFFER];
        _f.read(block,MAX_BLOCK-1);
        if (_f.gcount() ==0)
            return false;
        block[_f.gcount()]='\0';
        _stk.set_string (block);   
        return true;
    } //gets the new block from the file

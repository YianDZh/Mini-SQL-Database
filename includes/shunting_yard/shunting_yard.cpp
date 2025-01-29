#include "shunting_yard.h"


ShuntingYard::ShuntingYard (){

}
ShuntingYard::ShuntingYard (Queue <Token*> infix){
    _inner = infix;
}
ShuntingYard::ShuntingYard (vectorstr list){
    to_fix = list;
    tokenize();
}
ShuntingYard::ShuntingYard (Queue <Token*> infix,vectorstr list){
    to_fix = list;
    _inner = infix;
}
//? token_str 1
//? relation 7
//? logical 8
//? right 6
//? left 5

Queue <Token*> ShuntingYard::postfix(){
    // _inner = tokenize ();
    // cout<<*this<<endl;
    Queue <Token*> post;
    Stack <Token*> to_do;
    for (Queue <Token*>::Iterator _it = _inner.begin(); _it!=_inner.end();_it++){
        Token* current = *_it;
        // cout<<"Processing "<<current<<endl;

        if(current->get_type()==1){
            post.push (current);
        }
        else if (current->get_type ()==8){
            //!Relational
            // if (to_doto_do.top()->get_type()==5 )
            if(!(to_do.empty()) && to_do.top()-> get_type() == 8) {
                post.push(to_do.pop());
            }
            to_do.push (current);
        }
        else if (current->get_type ()==7){
            //!logical
            while (!(to_do.empty())&&to_do.top()->get_type()!=5&&to_do.top()->get_type()>=7){
                if (to_do.top()->get_element()=="or"&&current->get_element()=="and"){
                    break;
                }
                post.push(to_do.pop());
            }
            to_do.push(current);
        }

        else if (current->get_type ()== 5){
            //!Left
            to_do.push(current);
        }
        else if (current -> get_type ()==6){
            //!Right
            while (to_do.top()->get_type()!=5){
                post.push (to_do.pop());
            }
            to_do.pop();
        }
        // cout<<"Stack: \n"<<to_do<<endl;
        // cout<<"Queue\n"<<post<<endl;
    }
    while (!(to_do.empty())){
        // cout<<"Stack: \n"<<to_do<<endl;
        // cout<<"Queue\n"<<post<<endl;
        // cout<<to_do.top()<<endl;
        Token* hold = to_do.pop();
            post.push (hold);
        
    }
        // cout<<"Final Queue\n"<<post<<endl;

    return post;
}
Queue <Token*> ShuntingYard::postfix(Queue <Token*> Sample){
    _inner = Sample;
    return postfix();
}
void ShuntingYard::infix(Queue <Token*> Sample){

}

void ShuntingYard::tokenize (){
    // Queue <Token *> _inner;
    for (int i = 0; i<to_fix.size();i++){
        // Token* pushed;
        if (is_logic (to_fix [i])){
            // cout<<"Logical\n";
            _inner.push(new Logical (to_fix[i]));
        }
        else if (is_relation(to_fix[i])){
            // cout<<"Relational\n";

            _inner.push(new Relational (to_fix[i]));
        }
        else if (is_democrat(to_fix[i])){
            // cout<<"Left\n";

            _inner.push(new LeftParen (to_fix[i]));
        }
        else if (is_republican(to_fix[i])){
            // cout<<"right\n";

            _inner.push(new RightParen (to_fix[i]));
        }
        else {
            // cout<<"regular\n";
            _inner.push(new TokenStr (to_fix[i]));
        }
        // _inner.push (pushed);
    }
    // cout<<_inner<<endl;
    // return _inner;
}
bool ShuntingYard::is_logic (string option){
    return (option == "AND"||option == "OR"||option == "or"||option=="and");
}
bool ShuntingYard::is_relation (string option){
    return  (option == "<"||option == "="||option == "<="||option == ">"||option == ">=");
}
bool ShuntingYard::is_republican (string option){
    return  (option == ")");
}
bool ShuntingYard::is_democrat (string option){
    return  (option == "(");
}


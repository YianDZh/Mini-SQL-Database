#ifndef LINKEDLISTFUNCTIONSTEMPLATED_H_
#define LINKEDLISTFUNCTIONSTEMPLATED_H_



#include <iostream>
#include "../../includes/node/node.h"
#include "string.h"
#include "ostream"

using namespace std;
//Linked List General Functions:
template <typename T>
void _print_list(node<T>* head);

//recursive fun! :)
template <typename T>
void _print_list_backwards(node<T> *head);

//return ptr to key or NULL
template <typename T>
node<T>* _search_list(node<T>* head,
                            T key);


template <typename T>
node<T>* _insert_head(node<T> *&head,
                            T insert_this);

//insert after ptr: insert head if marker null
template <typename T>
node<T>* _insert_after(node<T>*& head,
                                node<T> *after_this,
                                T insert_this);

//insert before ptr: insert head if marker null
template <typename T>
node<T>* _insert_before(node<T>*& head,
                                node<T>* before_this,
                                T insert_this);

//ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* prev_to_this);

//delete, return item
template <typename T>
T _delete_node(node<T>*& head, node<T>* delete_this);

//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head);

//duplicate the list, return pointer to last node in dest... 
//     use this function in your queue big three 
template <typename T>
node<T> *_copy_list(node<T> *&dest, node<T> *src);



//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head);

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos);

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

template <typename T>
void _print_list(node<T>* head){
    cout<<"Head->";
    if (head!= nullptr){
        for (node <T> *w=head; w!=nullptr;w=w->_next){
            cout<<*w<<" ";
        }
        cout<<"||||||||\n";
    }
}

template <typename T>
node<T>* _search_list(node<T>* head, T key){
    for (node <T> *w=head; w!=nullptr;w=w->_next){
        if (w->_item==key){
            return w;
        }
    }
    return nullptr;
}


template <typename T>
node<T>* _insert_head(node<T> *&head, T insert_this){
    node<T>* temp= new node<T> (insert_this);
    temp->_next = head;
    if (head!=nullptr){
        head->_prev= temp;
    }
    head = temp;
    // cout << *head << endl;
    return head;




    
    // if(head->_next!=nullptr){
    //     node<T>* newp= new node<T>;
    //     newp->_item = insert_this;
    //     head->_next->_prev=newp;
    //     newp->_next= head->_next;
    //     head->_next= newp;
    // }
    // else{
    //     node<T>* newp= new node<T>;
    //     newp->_item = insert_this;
    //     newp->_next= head->_next;
    //     head->_next= newp;

    // }
    
}

template <typename T>
node<T>* _insert_after(node<T>*& head, node<T> *after_this, T insert_this){
    if (head==nullptr||after_this==nullptr){
        return _insert_head (head, insert_this);
    }
    
    node<T>* newp= new node<T> (insert_this);
    newp->_next=after_this->_next;
    if (after_this->_next!=nullptr){
        after_this->_next->_prev=newp;
    }
    after_this->_next = newp;
    newp->_prev= after_this;
    return newp;
}

template <typename T>
node<T>* _insert_before(node<T>*& head, node<T>* before_this, T insert_this){
    if (head==nullptr||before_this==head||before_this->_prev==nullptr){
        return _insert_head (head, insert_this);
    }
    node<T>* newp= new node<T> (insert_this);
    newp->_next=before_this;
    before_this->_prev->_next=newp;
    newp->_prev=before_this->_prev;
    before_this->_prev=newp;
    return newp;
}

//ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* prev_to_this){
    return prev_to_this->_prev;
}

//delete, return item
template <typename T>
T _delete_node(node<T>*& head, node<T>* delete_this){
    assert (!(head==nullptr));
    T items = delete_this->_item;
    if (head == delete_this){
        head=delete_this->_next;
        // head->_prev = nullptr;
        delete delete_this;
        return items;
    }
    delete_this->_prev->_next = delete_this->_next;
    if (delete_this->_next!=nullptr){
        delete_this->_next->_prev = delete_this->_prev;
    }
    delete_this->_next=nullptr;
    delete_this->_prev=nullptr;
    delete delete_this;
    return items;
}

//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head){
    if (head==nullptr){
        return nullptr;
    }
    node <T> * tempo = nullptr;
    node <T> * walker2=tempo;
    node <T> *w=head;
    while (w!=nullptr){
        walker2= _insert_after(tempo, walker2, w->_item);
        w=w->_next;
    }
    return tempo;
}

//duplicate the list, return pointer to last node in dest... 
//     use this function in your queue big three 
template <typename T>
node<T> *_copy_list(node<T> *&dest, node<T> *src){
    dest = _copy_list (src);
    for (node <T> *w=dest; w!=nullptr;w=w->_next){
        if (w->_next==nullptr){
            return w;
        }
    }
    return nullptr;
}



//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head){
    // for (node <T> *w=head; w!=nullptr;w=new_var){
    //     _print_list (head);
    //     new_var = w->_next;
    //     T a = _delete_node (head,w);
        // if (w->_next==nullptr)
        //     T a = _delete_node (head,w);
        // cout<< a <<"\n";

    // }
    // cout<<*head<<"\n";
    node <T> * w=head ; 
    // cout<<__LINE__<<"\n";

    while (w!=nullptr){
        // _print_list (head);
        node <T> * next_w = w->_next;
    // cout<<__LINE__<<"\n";
        T a = _delete_node (head, w);
    // cout<<__LINE__<<"\n";

        w = next_w;
    }
}

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos){
    assert (!(pos<0));
    int i = 0;
    for (node <T> *w=head; w!=nullptr;w=w->_next, i++){
        if (i==pos){
            return w->_item;
        }
    }
}
#endif // LINKEDLISTFUNCTIONSTEMPLATED_H_

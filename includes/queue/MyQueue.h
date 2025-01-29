#ifndef MYQUEUE_H_
#define MYQUEUE_H_
#include "../../includes/node/node.h"
#include <iostream>
#include "../../includes/linked_list_functions/linkedlistfunctionsTemplated.h"
using namespace std;

template <typename T>
class Queue
{
public:
    class Iterator{
        public:
        friend class Queue;          //give access to list to access _ptr
        
        Iterator(){_ptr = NULL;}                    //default ctor
        Iterator(node<T>* p);
                                                    //Point Iterator to where p
                                                    //  is pointing to
        T &operator*();                             //dereference operator

        T *operator->();                            //member access operator
        bool is_null();                             //true if _ptr is NULL
        friend bool operator!=(const Iterator &left, const Iterator &right){
            return !(left==right);
        } //true if left != right

        friend bool operator==(const Iterator &left, const Iterator &right){
            const node <T>* temp = left._ptr;
            const node <T>* temp2 = right._ptr;
            return temp==temp2;

        } //true if left == right

        Iterator &operator++(){
            this->_ptr=this->_ptr->_next;
            return this;
        }                        //member operator:
                                                        //  ++it; or
                                                        //  ++it = new_value
        friend Iterator operator++(Iterator &it,int unused){
            Iterator hold = it;
            it=it._ptr->_next;
            return hold;
        }          //friend operator: it++
    private:
        node<T>* _ptr;                          //pointer being encapsulated
    };

    Queue();

    Queue(const Queue<T>& copyMe);
    ~Queue();
    Queue& operator=(const Queue<T>& RHS);

    bool empty();
    T front();
    T back();

    void push(T item);
    T pop();

    Iterator begin() const;            //Iterator to the head node
    Iterator end() const;              //Iterator to NULL
    void print_pointers();
    int size() const { return _size; }
    template<typename TT>
    friend ostream& operator << (ostream& outs, const Queue<TT>& printMe);
private:
    node<T>* _front;
    node<T>* _rear;
    int _size;
};

//!================================================================================================
//**ITERATOR CLASS FUNCTIONS
template <class T>
Queue<T>::Iterator::Iterator(node<T>* p){
    _ptr = p;
}
template <class T>
T& Queue<T>::Iterator::operator*(){
    return _ptr->_item;
}                             //dereference operator
 
template <class T>
T* Queue<T>::Iterator::operator->(){
    return &(_ptr->_item);
}

template <class T>
bool Queue<T>::Iterator::is_null(){
    return _ptr==nullptr;
}                            //true if _ptr is NULL
//!================================================================================================

template <typename T>
Queue<T>::Queue(){
    _front = nullptr;
    _rear = nullptr;
    _size = 0;
}

//? BIG THREE
//?===================================================================================================
template <typename T>
Queue<T>::Queue(const Queue<T>& copyMe){
    _rear = _copy_list(_front,copyMe._front);
    _size = copyMe._size;
}
template <typename T>
Queue<T>::~Queue(){
    _clear_list (_front);
    _size = 0;
    _rear = nullptr;
}
template <typename T>
Queue<T>& Queue<T>::operator=(const Queue<T>& RHS){
    if (this==&RHS){
        return *this;
    }
    _rear = nullptr;
    _size = RHS._size;

    if (_front!=nullptr){
    _clear_list (_front);
    
    }
    _rear = _copy_list(_front,RHS._front);
    return *this;  
}
//?===================================================================================================

template <typename T>
bool Queue<T>::empty(){
    return _size==0;
}
template <typename T>
T Queue<T>::front(){
    return _front->_item;
}
template <typename T>
T Queue<T>::back(){
    // cout<<"Here\n";
    // cout<<_rear->_item<<"\n";
    return _rear->_item;
}
template <typename T>
void Queue<T>::push(T item){
    _size++;
    if (this->empty()){
        _front = _insert_head (_front,item);
        _rear = _front;
    }
    else{
    _rear = _insert_after (_front, _rear, item);
    }
}

template <typename T>
T Queue<T>::pop(){
    _size--;
    return _delete_node (_front, _front);
    
}
template <typename T>
typename Queue<T>::Iterator Queue<T>::begin() const{
    Iterator top (_front);
    return top;
}            //Iterator to the head node
template <typename T>
typename Queue<T>::Iterator Queue<T>::end() const{
    Iterator empt;
    return empt;
}              //Iterator to NULL
template <typename T>
void Queue<T>::print_pointers(){
    cout<<"Front: "<< _front<<"\n";
    cout<<"Rear: "<<_rear<<"\n";
}

template<typename TT>
ostream& operator << (ostream& outs, const Queue<TT>& printMe){
    _print_list (printMe._front);
    // printMe.print_pointers();
    return outs;
}

#endif // MYQUEUE_H_

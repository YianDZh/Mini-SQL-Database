#ifndef MYSTACK_H_
#define MYSTACK_H_
#include "../../includes/node/node.h"
#include <iostream>
#include "../../includes/linked_list_functions/linkedlistfunctionstemplated.h"

template <typename T>
class Stack{
public:
    class Iterator{
    public:
        friend class Stack;                     //give access to list to access _ptr
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

    Stack();
    Stack(const Stack<T>& copyMe);
    ~Stack();
    Stack<T>& operator=(const Stack<T>& RHS);
    T top();
    bool empty();
    void push(T item);
    T pop();
    template<typename TT>
    friend ostream& operator<<(ostream& outs, const Stack<TT>& printMe);

    Iterator begin() const;              //Iterator to the head node
    Iterator end() const;                //Iterator to NULL
    int size() const { return _size; }

private:
    node<T>* _top;
    int _size;
};

//!================================================================================================
//**ITERATOR CLASS FUNCTIONS
template <class T>
Stack<T>::Iterator::Iterator(node<T>* p){
    _ptr = p;
}
template <class T>
T& Stack<T>::Iterator::operator*(){
    return _ptr->_item;
}                             //dereference operator
 
template <class T>
T* Stack<T>::Iterator::operator->(){
    return &(_ptr->_item);
}

template <class T>
bool Stack<T>::Iterator::is_null(){
    return _ptr==nullptr;
}                            //true if _ptr is NULL
//!================================================================================================

template <class T>
Stack<T>::Stack(){
    _top = nullptr;
    _size = 0;
}
//? BIG THREE
//?================================================================================================
template <class T>
Stack<T>::Stack(const Stack<T>& copyMe){
    _size = copyMe._size;
    _top = _copy_list (copyMe._top);
}

template <class T>
Stack<T>::~Stack(){
    _size = 0;
    _clear_list (_top);
}

template <class T>
Stack<T>& Stack<T>::operator=(const Stack<T>& RHS){
    if (this==&RHS){
        return *this;
    }
    _size = RHS._size;
    if (_top!=nullptr)
    _clear_list (_top);
    _top = _copy_list (RHS._top);
    return *this;  
}
//?================================================================================================

template <class T>
T Stack<T>::top(){
    return _top->_item;
}

template <class T>
bool Stack<T>::empty(){
    return _top==nullptr;
}

template <class T>
void Stack<T>::push(T item){
    _size++;
    _top = _insert_head (_top, item);
}

template <class T>
T Stack<T>::pop(){
    _size--;
    return _delete_node (_top,_top);
}

template<typename TT>
ostream& operator<<(ostream& outs, const Stack<TT>& printMe){
    _print_list (printMe._top);
    return outs;
}

template <class T>
typename Stack<T>::Iterator Stack<T>::begin() const{
    Iterator start (_top);
    return start;
}              //Iterator to the head node

template <class T>
typename Stack<T>::Iterator Stack<T>::end() const{
    Iterator neo;
    return neo;
}                //Iterator to NULL

#endif // MYSTACK_H_
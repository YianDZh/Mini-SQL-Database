#ifndef BPLUSTREE_H_
#define BPLUSTREE_H_
#include <iomanip>
#include <iostream>
#include "string.h"
#include "assert.h"
#include "../../includes/bplustree/btree_array_funcs.h"

using namespace std;

template <class T>
class BPlusTree
{
public:
    class Iterator{
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it=NULL, int _key_ptr = 0): it(_it), key_ptr(_key_ptr){}

        T operator *();

        Iterator operator++(int un_used){
            Iterator copy(this->it, this->key_ptr);
            key_ptr = (key_ptr + 1) % it->data_count;
            if (key_ptr == 0) it = it->next;
            return copy;
        }
        Iterator operator++(){
            key_ptr = (key_ptr + 1) % it->data_count;
            if (key_ptr == 0) it = it->next;
            return *this;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            const BPlusTree <T>* temp = lhs.it;
            const BPlusTree <T>* temp2 = rhs.it;
            if (lhs.key_ptr != rhs.key_ptr){
                return false;
            }
            return temp==temp2;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return !(lhs==rhs);

        }
        void print_Iterator(){
            if (it){
                cout<<"iterator: ";
                print_array(it->data, it->data_count, key_ptr);
            }
            else{
                cout<<"iterator: NULL, key_ptr: "<<key_ptr<<endl;
            }
        }
        bool is_null(){return !it;}
        void info(){
            cout<<endl<<"Iterator info:"<<endl;
            cout<<"key_ptr: "<<key_ptr<<endl;
            cout<<"it: "<<*it<<endl;
        }

    private:
        BPlusTree<T>* it;
        int key_ptr;
    };

    BPlusTree(bool dups = false);
    BPlusTree(T *a, int size, bool dups = false);

    //big three:
    BPlusTree(const BPlusTree<T>& other);
    ~BPlusTree();
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS);

    void copy_tree(const BPlusTree<T>& other);  //copy other into this object
    void copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node);

    void insert(const T& entry);   //insert entry into the tree
    void remove(const T& entry);   //remove entry from the tree
    void clear_tree();             //clear this object (delete all nodes etc.)

    bool contains(const T& entry) const; //true if entry can be found
    T& get(const T& entry);              //return a reference to entry
    const T& get(const T& entry)const;   //return a reference to entry
    T& get_existing(const T& entry);     //return a reference to entry
    const T& get_existing(const T& entry) const;
   
    Iterator find(const T& key);         //return an iterator to this key.
                                         //     NULL if not there.
    Iterator lower_bound(const T& key);  //return first that goes NOT BEFORE
                                         // key entry or next if does not
                                         // exist: >= entry
    Iterator upper_bound(const T& key);  //return first that goes AFTER key
                                         //exist or not, the next entry  >entry

    int size() const;                    //count the number of elements

    bool empty() const;                  //true if the tree is empty

    void print_tree(int level = 0,
                    ostream &outs=cout) const;
    friend ostream& operator<<(ostream& outs,
                               const BPlusTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }

    bool is_valid();
    string in_order();
    string pre_order();
    string post_order();


    Iterator begin();
    Iterator end();
    ostream& list_keys(Iterator from = NULL, Iterator to = NULL){
        if (from == NULL) from = begin();
        if (to == NULL) to = end();
        for (Iterator it = from; it != to; it++)
            cout<<*it<<" ";
        return cout;

    }

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                        //true if duplicate keys are allowed
    int data_count;                      //number of data elements
    T data[MAXIMUM + 1];                 //holds the keys
    int child_count;                     //number of children
    BPlusTree* subset[MAXIMUM + 2];      //subtrees
    BPlusTree* next;
    
    //!=====================Functions====================================================
    
    bool is_leaf() const
    {return child_count==0;}             //true if this is a leaf node

    T* find_ptr(const T& entry);         //return a pointer to this key.
                                         // NULL if not there.

    //insert element functions
    void loose_insert(const T& entry);   //allows MAXIMUM+1 data elements in
                                         //   the root
    void fix_excess(int i);              //fix excess in child i

    //remove element functions:
    void loose_remove(const T& entry);  //allows MINIMUM-1 data elements
                                        //  in the root

    BPlusTree<T>* fix_shortage(int i);  //fix shortage in child i
    // and return the smallest key in this subtree
    BPlusTree<T>* get_smallest_node();
    void get_smallest(T& entry);      //entry := leftmost leaf
    void get_biggest(T& entry);       //entry := rightmost leaf
    void remove_biggest(T& entry);    //remove the biggest child of tree->entry
    void transfer_left(int i);        //transfer one element LEFT from child i
    void transfer_right(int i);       //transfer one element RIGHT from child i
    BPlusTree<T> *merge_with_next_subset(int i); //merge subset i with  i+1

};

//?????????????????????????????ITERATOR BS??????????????????????????????????????????????????????
template <class T>
T BPlusTree<T>::Iterator::operator *(){
    return it->data[key_ptr];
}

// template <class T>
// typename BPlusTree<T>::Iterator BPlusTree<T>::Iterator::operator++(int un_used){
//     Iterator copy (this,key_ptr);
//     key_ptr ++;

//     if (key_ptr>=it->data_count){
//        key_ptr = 0;
//         it=it._ptr->_next;
//     // return hold;
//     }
//     return copy;
            
// }
// template <class T>
// typename BPlusTree<T>::Iterator  BPlusTree<T>::Iterator::operator++(){
//     key_ptr ++;
//     if (key_ptr>=it->data_count){
//        key_ptr = 0;
//         it=it._ptr->_next;
//     // return hold;
//     }
//     return copy;

// }
// template <class T>
// bool operator ==(const Iterator& lhs, const Iterator& rhs){
// }
// template <class T>
// bool operator !=(const Iterator& lhs, const Iterator& rhs){

// }
//!================???======================================================

template <class T>
BPlusTree<T>::BPlusTree(bool dups){
    dups_ok = dups;                        //true if duplicate keys are allowed
    data_count = 0;                      //number of data elements
    child_count = 0;                     //number of children
    next = nullptr;
}
template <class T>
BPlusTree<T>::BPlusTree(T *a, int size, bool dups){
    data_count = 0;
    child_count = 0;
    for (int i = 0 ; i <size ; i++){
        insert (a[i]);
    }
    
}

//??????????????????????????????? BIG THREE ??????????????????????????????????????????????????:
template <class T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& other){
    copy_tree (other);
}
template <class T>
BPlusTree<T>::~BPlusTree(){
    clear_tree ();
}
template <class T>
BPlusTree<T>& BPlusTree<T>::operator =(const BPlusTree<T>& RHS){
    if (this == &RHS){
        return *this;
    }
    if (!this->empty ()){
        this->clear_tree ();
    }
    copy_tree (RHS);
    return *this;
}
//??????????????????????????????? BIG THREE ??????????????????????????????????????????????????:

//***************************** Copy and Clear ***********************************************
template <class T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other){
    BPlusTree <T>* smth = nullptr;
    copy_tree (other, smth);
}  //copy other into this object

template <class T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node){
    data_count = other.data_count;
    child_count = other.child_count;
    copy_array (data, other.data, data_count,other.data_count);
    if (is_leaf ()){
        if (last_node != nullptr){
            last_node->next = this;
        }
        last_node = this;
    }
    else {
        for (int i=0; i<child_count;i++){
            subset [i] = new BPlusTree <T>;
            subset [i]->copy_tree (*(other.subset[i]), last_node);
        }
    }

          //make the connection for the next ptr 

}



template <class T>
void BPlusTree<T>::clear_tree(){
    //TODO: Check if clear has to remain the same
        for (int i=0; i<child_count;i++){
        if (subset [i]){
            subset [i]->clear_tree();
            delete subset [i];
        }
    }
    data_count =0;
    child_count = 0;
    next = nullptr;
}             //clear this object (delete all nodes etc.)

//***************************** Copy and Clear ***********************************************

//***************************** GENERAL FUNCTIONS ***********************************************

template <class T>
typename BPlusTree<T>::Iterator BPlusTree<T>::lower_bound(const T& key){
    Iterator i;
    for (i = this->begin(); i != this->end() && *i < key; i++) {}
    return i;
}  //return first that goes NOT BEFORE
                                         // key entry or next if does not
                                         // exist: >= entry
template <class T>
typename BPlusTree<T>::Iterator BPlusTree<T>::upper_bound(const T& key){
    Iterator i;
    for (i = this->begin(); i != this->end() && *i <= key; i++) {}
    return i;
}  //return first that goes AFTER key
                                         //exist or not, the next entry  >entry

template <class T>
int BPlusTree<T>::size() const{
    int count = 0;
    for (int i=0;i<data_count;i++){
        count++;
    }
    for (int i=0; i<child_count;i++){
        count += subset [i]->size();
    }
    return count;
}                    //count the number of elements

template <class T>
bool BPlusTree<T>::empty() const{
    return size()==0;
}                  //true if the tree is empty

template <class T>
void BPlusTree<T>::print_tree(int level, ostream &outs) const{
    if (!is_leaf()){
        subset[data_count]->print_tree(level+1,outs);
    outs << setw(level * 4) << " " << "=====" << endl;
    for (int i = data_count; i>0 ;i--){
        // cout<<i<<endl;
        cout<<setw(4*level)<<""<<data[i-1]<<endl;
        if (i==1)
            outs << setw(level * 4) << " " << "=====" << endl;
            subset[i-1]->print_tree(level+1,outs);
        }
    }

    else{
    outs << setw(level * 4) << " " << "=====" << endl;
        for (int i = data_count; i>0 ;i--){
            // cout<<i<<endl;
            cout<<setw(4*level)<<""<<data[i-1]<<endl;
        // subset[i]->print_tree(level+1,outs);
        }
    outs << setw(level * 4) << " " << "=====" << endl;
    }
       
}
template <class T>
bool BPlusTree<T>::is_valid(){
    for (int i = 0; i < data_count; i++){
        if (i+1>=data_count){
            break;
        }
        if (!(data [i]<data[i+1])){
            return false;
        }
    }

    if (is_leaf()){
        return true;
    }
/* 
    



 
*/
//data[data_count-1] must be less than equal to every subset[child_count-1]->data[ ]
    for (int i = 0; i< subset[child_count-1]->data_count; i++){
        if (i>data_count){
            break;
        }
        if ((data[data_count - 1] > subset [child_count-1]->data[i])){
            return false;
        }
    }
//    every data[i] is greater than every subset[i]->data[ ]

    for (int i = 0; i< data_count; i++){
        for (int j = 0; j<subset [i]->data_count;j++){
            if ((data[i] <= subset [i]->data[j])){
                return false;
            }
        }
    }
// B+Tree: Every data[i] is equal to subset[i+1]->smallest

    for (int i = 0; i< data_count; i++){
        T temp;
        subset[i+1]->get_smallest (temp);
        if (!(data[i] == temp)){
            return false;
        }
    }
    
//Recursively validate every subset[i]

    for (int i = 0; i< child_count; i++){
        return subset [i]->is_valid();
    }   
    return true;
    /*
     should check that every data[i] < data[i+1]
    if (is_leaf())
        return true;
    

    data[data_count-1] must be less than equal to 
      every subset[child_count-1]->data[ ]
    
    every data[i] is greater than every subset[i]->data[ ]



    B+Tree: Every data[i] is equal to subset[i+1]->smallest
 
    Recursively validate every subset[i]

    return true;
    */
    //? TODO make sure you have to return true;
    return true;
}

template <class T>
string BPlusTree<T>::in_order(){
    string smth;
    for (int i = 0; i < data_count; i++) {
        //will access the node as it goes
        if (i < child_count) {
            smth += subset[i]->in_order();
        }
        smth += to_string(data[i]) + "|";
    }
    
    //data_count is always less than 1, getting data count, gets the last subtree
    if (data_count < child_count) {
        smth += subset[data_count]->in_order();
    }

    return smth;

    
}

//TODO make sure this guys are right
template <class T>
string BPlusTree<T>::pre_order(){
    string smth;
    for(int i=0; i<data_count; i++){

        smth += to_string (data[i]);
        smth += "|";
        if (!is_leaf())
            smth += subset [i]->pre_order();
    }
    if (!is_leaf())
        smth += subset [data_count]->pre_order();
    return smth;
}
template <class T>
string BPlusTree<T>::post_order(){
    string smth; 
    if (!is_leaf()){
        smth+=subset[0]->post_order();
    }
    for(int i=0; i<data_count; i++){
        if (!is_leaf()){
            smth += subset [i+1]->post_order();
        }
        smth += to_string (data[i]);
        smth += "|";
    }
    return smth;
}
template <class T>
typename BPlusTree<T>::Iterator  BPlusTree<T>::begin(){
    return BPlusTree<T>::Iterator(get_smallest_node());
}

template <class T>
typename BPlusTree<T>::Iterator BPlusTree<T>:: end(){
    return BPlusTree<T>::Iterator();
}
//***************************** GENERAL FUNCTIONS ***********************************************


//***************************** SEARCH  ***********************************************
template <class T>
bool BPlusTree<T>::contains(const T& entry) const{
//    assert(contains(entry));
    int i = first_ge (data, data_count , entry);
    bool found = (i<data_count && data[i]==entry);
    if (is_leaf()){
        if (found){
            return true;
        }
        else{
            // if (debug) cout<<"get_existing was called with nonexistent entry"<<endl;
            return false;
        }
    }
    if (found) //inner node
        return subset[i+1]->contains(entry);
        // or just return true?
    else //not found yet...
        return subset[i]->contains(entry);
} //true if entry can be found

template <class T>
T& BPlusTree<T>::get(const T& entry){
    /*  //If entry is not in the tree, add it to the tree
    assert(contains(entry));

    const bool debug = false;
    if (!contains(entry))
        insert(entry);

    return get_existing(entry);*/
    if (!contains(entry)){
        // cout<<"Going into  insert\n";
        insert(entry);
        // cout<<"Returned from insert\n";
    }
    return get_existing(entry); 
}              //return a reference to entry
template <class T>
const T& BPlusTree<T>::get(const T& entry)const{
     int i = first_ge (data, data_count , entry);
    bool found = (i<data_count && data[i]==entry);
    // cout << "i: " << i << data[i]<<endl;
    if (is_leaf()){
        if (found){
            return data[i];
        }
        else{
            // if (debug) cout<<"get_existing was called with nonexistent entry"<<endl;
            assert(found);
        }
    }
    if (found) //inner node
        return subset[i+1]->get(entry);
        // or just return true?
    else //not found yet...
        return subset[i]->get(entry);


   /*
 * ---------------------------------------------------------------
 * This routing explains plainly how the BPlusTree is organized.
 * i = first_ge
 *       |      found         |    !found         |
 * ------|--------------------|-------------------|
 * leaf  |  you're done       | it's not there    |
 *       |                    |                   |
 * ------|--------------------|-------------------|
 * !leaf | subset[i+1]->get() |  subset[i]->get() |
 *       | (inner node)       |                   |
 * ------|--------------------|-------------------|
 *       |                    |                   |
 *
 * ---------------------------------------------------------------
 *
 
    assert that entry is not in the tree.
    assert(contains(entry));

    const bool debug = false;
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);
    if (is_leaf())
        if (found){
            return data[i];
        }
        else{
            if (debug) cout<<"get_existing was called with nonexistent entry"<<endl;
            assert(found);
        }
    if (found) //inner node
        return subset[i+1]->get(entry);
        or just return true?
    else //not found yet...
        return subset[i]->get(entry);

    */

}   //return a reference to entry

template <class T>
const T& BPlusTree<T>::get_existing(const T& entry) const{
    // cout<<"Get existing\n";
    // assert(contains(entry));
    int i = first_ge (data, data_count , entry);
    bool found = (i<data_count && data[i]==entry);
    // cout << "i: " << i << data[i]<<endl;
    if (is_leaf()){
        if (found){
            return data[i];
        }
        else{
            // if (debug) cout<<"get_existing was called with nonexistent entry"<<endl;
            assert(found);
        }
    }
    if (found) //inner node
        return subset[i+1]->get(entry);
        // or just return true?
    else //not found yet...
        return subset[i]->get(entry);

}
template <class T>
T& BPlusTree<T>::get_existing(const T& entry){
    // cout<<"Get existing\n";
    // assert(contains(entry));
    int i = first_ge (data, data_count , entry);
    bool found = (i<data_count && data[i]==entry);
    // cout << "i: " << i << data[i]<<endl;
    if (is_leaf()){
        if (found){
            return data[i];
        }
        else{
            // if (debug) cout<<"get_existing was called with nonexistent entry"<<endl;
            assert(found);
        }
    }
    if (found) //inner node
        return subset[i+1]->get(entry);
        // or just return true?
    else //not found yet...
        return subset[i]->get(entry);


   /*
 * ---------------------------------------------------------------
 * This routing explains plainly how the BPlusTree is organized.
 * i = first_ge
 *       |      found         |    !found         |
 * ------|--------------------|-------------------|
 * leaf  |  you're done       | it's not there    |
 *       |                    |                   |
 * ------|--------------------|-------------------|
 * !leaf | subset[i+1]->get() |  subset[i]->get() |
 *       | (inner node)       |                   |
 * ------|--------------------|-------------------|
 *       |                    |                   |
 *
 * ---------------------------------------------------------------
 *
 
    assert that entry is not in the tree.
    assert(contains(entry));

    const bool debug = false;
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);
    if (is_leaf())
        if (found){
            return data[i];
        }
        else{
            if (debug) cout<<"get_existing was called with nonexistent entry"<<endl;
            assert(found);
        }
    if (found) //inner node
        return subset[i+1]->get(entry);
        or just return true?
    else //not found yet...
        return subset[i]->get(entry);

    */
}     //return a reference to entry

template <class T>
T* BPlusTree<T>::find_ptr(const T& entry){
    for (int i=0; i<data_count;i++){
        if (data [i]==entry){
            return &data[i];
        }
    }
    // if (is_leaf()){
        return nullptr;
    // }
    //might need an else;
    // return subset[i]->find_ptr (entry);
}         //return a pointer to this key. // Fnull if not there.

template <class T>
typename BPlusTree<T>::Iterator BPlusTree<T>::find(const T& key){
//    assert(contains(entry));

    int i = first_ge (data, data_count , key);
    bool found = (i<data_count && data[i]==key);
    if (is_leaf()){
        if (found){
            return BPlusTree<T>::Iterator (this,i);
        }
        else{
            // if (debug) cout<<"get_existing was called with nonexistent entry"<<endl;
            return BPlusTree<T>::Iterator (NULL,0);
        }
    }
    if (found) //inner node
        return BPlusTree<T>::Iterator (subset[i+1]->find(key));
        // or just return true?
    else //not found yet...
        return BPlusTree<T>::Iterator (subset[i]->find(key));
}         //return an iterator to this key. NULL if not there.
//***************************** SEARCH  ***********************************************

//***************************** INSERT ***********************************************
template <class T>
void BPlusTree<T>::insert(const T& entry){
    // cout<<"Going into loose insert\n";
    loose_insert (entry);
    // cout<<" Done with loose insert\n";
    
    if (data_count > MAXIMUM){
        BPlusTree <T> *rep = new BPlusTree <T>;
        //         cout<<"Here\n";
        // cout<<*this<<endl;
        // cout<<data_count<<endl;
        // cout<<child_count<<endl;
        copy_array (rep->data, data, rep->data_count, data_count);
        copy_array (rep->subset, subset, rep->child_count, child_count);
        data_count = 0;
        child_count = 1;
        subset [0] = rep;
        // print_array (subset[0]->data, subset[0]->data_count);
        // cout<<"Growing tree\n";
        fix_excess (0);

    }
      //in order for this class to be able to keep track of the number of the keys,
    //      this function (and the functions it calls ) must return a success code.
    //If we are to keep track of the number the keys (as opposed to key/values)
    //  then the success code must distinguish between inserting a new key, or
    //  adding a new key to the existing key. (for "dupes_ok")
    //
    //loose_insert this entry into this root.
    //loose_insert(entry) will insert entry into this tree. Once it returns,
    //  all the subtrees are valid
    //  btree subtrees EXCEPT this root may have one extra data item:
    //    in this case (we have excess in the root)
    //      create a new node, copy all the contents of this root into it,
    //      clear this root node,
    //      make the new node this root's only child (subset[0])
    //
    //Then, call fix_excess on this only subset (subset[0])

}   //insert entry into the tree

template <class T>
void BPlusTree<T>::loose_insert(const T& entry){
    // cout<<"starting Loose insert\n";
    int i = first_ge (data, data_count, entry);

    bool smth = (i<data_count && data[i] == entry);
    // cout<<"Here\n";

    if (smth&&dups_ok&&is_leaf()){
        // cout<<"Dupes add\n";
        data[i]=data[i]+entry;
        return;
    } 

    if (smth&&is_leaf() && !dups_ok){
        // cout<<"Dupes not replace\n";

        data[i]=entry;
        return;
    }
    else if (!is_leaf() && smth){
        // cout<<"Not found, not leaf\n";
        subset [i+1]->loose_insert (entry);
        // cout<<"Fix excess i+1\n";
        // cout<<"i is: "<<i<<endl;
        fix_excess(i+1);
        return;
    }

    if (is_leaf () && !smth){
        // cout<<"Inserting at leaf\n";
        insert_item (data,i,data_count,entry);
        return;
    }
    else {
        // cout<<"Recursive step\n";
        subset[i]->loose_insert (entry);
        // cout<<"index: "<<i<<endl;
        // cout<<"Fix excess i\n";
        // cout<<"i is: "<<i<<endl;
        fix_excess (i);
        return;
        
    }


   
/*
       int i = first_ge(data, data_count, entry);
       bool found = (i<data_count && data[i] == entry);

       three cases:
         found
         a. found/leaf: deal with duplicates: call +
         b. found/no leaf: subset[i+1]->loose_insert(entry)
                           fix_excess(i+1) if there is a need

         ! found:
         c. !found / leaf : insert entry in data at position i
         c. !found / !leaf: subset[i]->loose_insert(entry)
                            fix_excess(i) if there is a need

            |   found          |   !found         |
      ------|------------------|------------------|-------
      leaf  |a. dups? +        | c: insert entry  |
            |                  |    at data[i]    |
      ------|------------------|------------------|-------
            | b.               | d.               |
            |subset[i+1]->     | subset[i]->      |
      !leaf | loose_insert()   |  loose_insert()  |
            |fix_excess(i+1)   | fix_excess(i)    |
            |                  |                  |
      ------|------------------|------------------|-------
    */

}   //allows MAXIMUM+1 data elements in the root

template <class T>
void BPlusTree<T>::fix_excess(int i){
    // cout<<"Fix excess\n";
 //this node's child i has one too many items: 
    if (subset[i]->data_count <= MAXIMUM){
        return;
    }
    //3 steps:
    //1. add a new subset at location i+1 of this node
    insert_item (subset, i+1 ,child_count, new BPlusTree<T> (dups_ok));
    //2. split subset[i] (both the subset array and the data array) and move half into
    //      subset[i+1] (this is the subset we created in step 1.)
    split (subset[i]->data,subset [i]->data_count, subset [i+1]-> data, subset [i+1]->data_count);
    split (subset [i]-> subset, subset [i]->child_count, subset [i+1]-> subset, subset [i+1]->child_count);
    // cout<<"SPLITING\n";
    T temp;
    //3. detach the last data item of subset[i] and bring it and insert
    //      it into this node's data[]
    //
    
    //Note that this last step may cause this node to have too many itesm.
    //  This is OK. This will be
    //  dealt with at the higher recursive level. (my parent will fix it!)
    detach_item (subset[i]->data, subset[i]-> data_count, temp);
    // cout<<"Going to ordered \n";
    ordered_insert (data, data_count, temp);
    //NOTE: ODD/EVENNESS
    // when there is an excess, data_count will always be odd
    //  and child_count will always be even.
    //  so, splitting is always the same.

    //  000 B_PLUS_TREE
     if (subset[i]->is_leaf()){
        //TODO make connections of i+1 with i
        ordered_insert (subset [i+1]->data, subset[i+1]->data_count, temp);
        subset [i+1]->next = subset [i]->next;
        subset [i]->next = subset [i+1];
     }
    //  transfer the middle entry to the right and...
    //  Deal with next pointers. just like linked list insert
}              //fix excess in child i remove element functions: 
//***************************** INSERT ***********************************************

//***************************** REMOVE ***********************************************
template <class T>
void BPlusTree<T>::remove(const T& entry){
     loose_remove (entry);
    
    if (data_count == 0 && child_count == 1){
        // cout<<"Shrinking\n";
        BPlusTree <T>* victim = subset[0];
        copy_array (data,victim -> data,data_count, victim -> data_count);
        copy_array (subset,victim -> subset, child_count , victim -> child_count);
        victim -> data_count = 0;
        victim -> child_count = 0;
        delete victim;
    }
}   //remove entry from the tree

template <class T>
void BPlusTree<T>::loose_remove(const T& entry){
    // cout<<"===============How does the branch look before delete=====================\n";
    // cout<<*this<<endl;
    // cout<<"===============How does the branch look before delete=====================\n";
    int i = first_ge (data, data_count, entry);
    bool smth = (i<data_count && data[i] == entry);
    T temp;
    // bool is_present = contains (entry);
    if (is_leaf ()){
        if (smth){
            // cout<<"deleting at leaf\n";
            // cout<<data_count<<endl;
            delete_item (data, i, data_count, temp);
            // cout<<data_count<<endl;

            return;
        }
        else {
            
            return;
        }
    } 
   
    else {
   
    if (smth){
        subset[i+1]->loose_remove (entry);
        fix_shortage (i+1);
        T* finder = find_ptr (entry);
    // cout<<"===============How does the branch look before magic=====================\n";
    // cout<<*this<<endl;
    // cout<<"===============How does the branch look before magic=====================\n";
        if (finder){
            int ge = first_ge (data, data_count,entry);
            // print_array (data, data_count);
            //! BEWARE the dereference of finder 
            // cout<<"GE: "<<ge<<endl;
            // cout<<*finder<<endl;
            subset [ge+1]->get_smallest (*finder);
            // cout<<*finder<<endl;
        }

        else {
            // cout<<"Going into subsets\n";
            int helper = first_ge (data,data_count,entry);
            
            finder = subset [i]->find_ptr (entry);

            T* holy_grail = subset[helper]->find_ptr (entry);

            if (finder){
                int extra = first_ge (subset [i]->data, subset [i]->data_count,entry);
                //! Beware another var
                subset [i] -> subset [extra+1]-> get_smallest (*finder);
            }
            else if (holy_grail){
                int extra = first_ge (subset [helper]->data, subset [helper]->data_count,entry);
                subset [helper] -> subset [extra+1]-> get_smallest (*holy_grail);
            }
            else{
                // cout<<"No pointer magic for you\n";
                return;
            }
    // cout<<"===============How does the branch look after magic=====================\n";
    // cout<<*this<<endl;
    // cout<<"===============How does the branch look after magic=====================\n";
       
        }

    }
    else {
        // cout<<"Case recursing\n";
        subset [i]->loose_remove (entry);
        fix_shortage (i);
    }
    // cout<<"===============How does the branch look after delete=====================\n";
    // cout<<*this<<endl;
    // cout<<"===============How does the branch look after delete=====================\n";

    }
   /* four cases:
           leaves:
                a. not found: there is nothing to do
                b. found    : just remove the target
           non leaf:
                c. not found: subset[i]->loose_remove, fix_shortage(i)
                d. found    : subset[i+1]->loose_remove, fix_shortage(i+1) [...]
                    (No More remove_biggest)

             |   !found               |   found                 |
       ------|------------------------|-------------------------|-------
       leaf  |  a: nothing            | b: delete               |
             |     to do              |    target               |
       ------|------------------------|-------------------------|-------
       !leaf | c:                     | d: B_PLUS_TREE          |
             |  [i]->  loose_remove   |   [i+1]-> loose_remove  |
             |  fix_shortage(i)       | fix_shortage(i+1) [...] |
       ------|------------------------|-------------------------|-------


     */

    /*
    const bool debug = false;
    T found_entry;
    if (is_leaf()){
        if(!found){
            //[a.] nothing to do
            //entry not in this tree
        }
        else{
            //[b.] just delete the item
            //   my parent will take care of my shortage
        }

    }
    else{
        //not leaf:
        // ---- 000 B_PLUS_TREE: no more remove_biggest
        if (!found){
            //[c.] Not here: subset[i]->loose_remove
            //not found yet. search in the next level:
        }
        else{
            //[d.] found key in an inner node:subset[i+1]->loose_remove
            // assert(i < child_count-1);

            /*
             * Here's The Story:
             * data[i] is the same as the item that we have deleted.
             * so, now, it needs to be replaced by the current smallest key
             *      in subset[i+1]
             * Problem: After fix_shortage, data[i] could have moved to a different index(?)
             *      or it could have sunk down to a lower level as a result of merge
             *      we need to find this item and replace it.
             *
             *      Before fix_shortage: we cannot replace data[i] with subset[i+1]->smallest
             *      before fix_excess because this smallest key could be the very entry
             *      we are removing. So, we'd be replacing data[i] with entry. and this will
             *      leave the deleted key in the inner nodes.
             *
             *      After fix_shortage: We have no way of knowing where data[i] key (which is
             *      same as entry) will end up. It could move down one level by the time
             *      fix_shortage returns.
             *
             *
             
             *      Therefore it seems like we have no choice but to search for it in
             *      data[ ] AND subset[i]->data[ ]
             * Current Solution: Kind of messy:
             *      After fix_shortage(i+1):
             *      Search for entry in data[ ] and if found, replace it with smallest.
             *      otherwise, search for it in subset[i]->data[ ]
             *          and replace it. that's where the key (entry) will end up after
             *          fix order returns (credit: Jiaruy Li)
             *
           
            subset[i+1]->loose_remove(entry);

            remember. it's possible that data[i] will be gone by the time
            fix_shortage returns.
            key was removed from subset[i+1]:
             1. shortage: find entry in data or subset[i+1]->data 
                         and replace it with subset[i+1]->smallest
             2. no shortage: replace data[i] with subset[i+1]->smallest
            T* finder = find_ptr (entry);
            if (finder){
                get_smallest (*finder);
            }
            else {
                finder = subset [i+1]->find_ptr (entry);
                if (finder){
                    get_smallest (*finder);
                }
                else{
                    return;
                } 
            }
             
        }
        // --------------------------------------------------------
    }
    */
}  //allows MINIMUM-1 data elements in the root

template <class T>
BPlusTree<T>* BPlusTree<T>::fix_shortage(int i){
    if (is_leaf() ||subset[i]->data_count>=MINIMUM){
        return nullptr;
    }
    if (i+1<child_count&& subset[i+1]->data_count > MINIMUM){
        // cout<<"Transfer left\n";
        transfer_left (i+1);
    }
    else if (i>0 && subset [i-1]->data_count > MINIMUM){
        // cout<<"Transfer right\n";
        transfer_right (i-1);
        

    }
    else if (i+1<child_count){
        // cout<<"Merge with next\n";
        merge_with_next_subset (i);
    }
    else{
        // cout<<"Merge with prev\n";
        merge_with_next_subset (i-1);
    }
    return this;

  /*
     * fix shortage in subtree i:
     * if child i+1 has more than MINIMUM,
     *          (borrow from right) transfer / rotate left(i+1)
     * elif child i-1 has more than MINIMUM,
     *          (borrow from left) transfer /rotate right(i-1)
     * elif there is a left sibling,
     *          merge with prev child: merge(i-1)
     * else
     *          merge with next (right) child: merge(i)
     *
     *
     * returns a pointer to the fixed_subset
     */
    const bool debug = false;
    if ((i<child_count-1) && (subset[i+1]->data_count > MINIMUM)){
        //borrow from right subtree if you can

        transfer_left(i+1);
        return subset[i];
    }
}  //fix shortage in child i nd return the smallest key in this subtree
template <class T>
BPlusTree<T>* BPlusTree<T>::get_smallest_node(){

    if (is_leaf()){
        return this;
    }
    return subset[0]->get_smallest_node ();
    
}
template <class T>
void BPlusTree<T>::get_smallest(T& entry){
    if (is_leaf()){
        entry = data [0];
        return;

    }
    subset[0]->get_smallest(entry);

}      //entry := leftmost leaf
template <class T>
void BPlusTree<T>::get_biggest(T& entry){
    if (is_leaf()){
        entry = data [data_count-1];
        return;
    }
    subset [child_count-1] -> get_biggest (entry);
}       //entry := rightmost leaf
template <class T>
void BPlusTree<T>::remove_biggest(T& entry){

}    //remove the biggest child of tree->entry
template <class T>
void BPlusTree<T>::transfer_left(int i){
    T temp;
    // if (subset [i]->data_count > 0)
    delete_item (subset [i]->data, 0, subset[i]-> data_count,temp);
    if (!(subset[i]->is_leaf())){
        // cout<<subset[i]->data_count<<endl;
        // cout<<temp<<endl;
        T temp2 = data [i-1];
        // cout<<temp2<<endl;
        data [i-1] = temp;
        attach_item (subset [i-1]->data, subset [i-1]->data_count, temp2);
        if (!(subset [i]-> is_leaf())){
            BPlusTree <T> * smth;
            delete_item (subset [i]->subset, 0, subset[i]-> child_count,smth);
            attach_item (subset [i-1]->subset, subset [i-1]->child_count, smth);
        }
    }
    else {
        attach_item (subset [i-1]->data, subset [i-1]->data_count, temp);
        data[i-1] = subset [i]->data[0];

        /* * -------------
        *  fix_shortage(0) calls trnasfer_left(1) so, i = 1
        *          [7 | 10]
        *        /    |     \
        * [( )]<=[(7) | 8]  [10]
        *
        * 1. transfer subset[i(1)]->data[0] to end of subset[i-1]
        *        [7 | 10]
        *      /    |     \
        *   [7]    [8]   [10]
        *
        * 2. update parent of the transfered subset: data[i-1 (0)] = subset[i(1)]->data[0]
        *
        *        [(8) | 10]
        *      /      |     \
        *   [7]     [(8)]   [10]
        *
        *
        *
        */
    }
    /*
     * --------------------------------------------------------------
     * transfer_left: from subset[i] to subset[i-1]
     *      non-leaf: same as BTree
     *      leaves  : rotate and leave and update data[i-1] with subset[i]->data[0]
     *                  leftmost key after you borrowed goes to replace data[i-1]
     * --------------------------------------------------------------
     *
     * non-leaf keys: BTree's rotate_left
     * ---------------
     *
     * (0 < i < child_count) and (subset[i]->data_count > MINIMUM)
     * subset[i-1] has only MINIMUM - 1 entries.
     *
     * item transfers from child[i] to child [i-1]
     *
     * FIRST item of subset[i]->data moves up to data to replace data[i-1],
     * data[i-1] moves down to the RIGHT of subset[i-1]->data
     *
     *  i = 1:
     *              [50  100]
     *  [  ]         [65 75]       ....
     *   [a]      [b]  [c]  [d]
     *
     *  65 move up to replace 50 (data[i])
     *  65's child (its child 0) moves over to be the future child of 50
     *  50 moves down to the right of subset[i]->data
     *
     *               [65 100]
     *   [50]         [ 75 ]       ....
     * [a]  [b]      [c]  [d]
     *
     * ****This does not belong here:
     * last item of subset[i-1]->data moves up to data to replace data[i],
     * data[i] moves down to the RIGHT of subset[i]->data
     *
     * leaf keys:
     * -------------
     *  fix_shortage(0) calls trnasfer_left(1) so, i = 1
     *          [7 | 10]
     *        /    |     \
     * [( )]<=[(7) | 8]  [10]
     *
     * 1. transfer subset[i(1)]->data[0] to end of subset[i-1]
     *        [7 | 10]
     *      /    |     \
     *   [7]    [8]   [10]
     *
     * 2. update parent of the transfered subset: data[i-1 (0)] = subset[i(1)]->data[0]
     *
     *        [(8) | 10]
     *      /      |     \
     *   [7]     [(8)]   [10]
     *
     *
     *
     */
}        //transfer one element LEFT from child i

template <class T>
void BPlusTree<T>::transfer_right(int i){
    // cout<<i<<endl;
    // cout<<*this<<endl;
        T temp;
        detach_item (subset [i]->data, subset [i]->data_count, temp);

        if (!(subset[i]->is_leaf())){
            T temp2 = data[i];
            data [i] = temp;
            ordered_insert (subset [i+1]->data, subset [i+1]->data_count, temp2);
            if (!(subset [i]-> is_leaf ())){
            BPlusTree <T> * tempo;
            detach_item (subset [i]->subset, subset [i]->child_count, tempo);
            insert_item (subset [i+1]->subset, 0,subset[i+1]->child_count,tempo);
        }
    }
    else{
        data [i] = temp;
        ordered_insert (subset [i+1]->data, subset [i+1]->data_count, temp);
        
        /*
    * 1. transfer subset[i(1)]->data[last item] to left of subset[i+1]->data
     *        [7 | (10)]
     *      /    |     \
     *   [5|6]->[7]-> [8]
     *    * 2. update parent of the transfered subset: data[i (1)] = subset[i+1(2)]->data[0]
     *
     *        [7 | 8]
     *      /    |    \
     *   [5|6]->[7]-> [8]
     *
     *
     */
    }
  /*
     * --------------------------------------------------------------
     * transfer right from subset[i] to subset[i+1]
     *      non-leaf: same as BTree
     *      leaves  : rotate and leave a 'trace':
     *              data[i] is replaced with subset[i+1]->data[0]
     *              after you borrowed the key,
     * --------------------------------------------------------------
     *
     * non-leaf keys: BTree's rotate_right
     * ---------------
     * (i < child_count - 1) and (subset[i-1]->data_count > MINIMUM)
     * subset[i+ 1] has only MINIMUM - 1 entries.
     *
     * item transfers from child[i] to child [i+1]
     *
     * LAST item of subset[i]->data moves up to data to replace data[i],
     * data[i] moves down to the LEFT of subset[i+1]->data
     *
     * i = 1
     *                     [50 100]
     *      [20 30]        [65 75]          [ ]
     *  [..] [..] [..]   [a] [b] [c]        [..]
     *
     *  75 moves up to replace 100 (data[i])
     *  75's child (its last child) moves over to be the (child 0) child of 100
     *  100 moves down to subset[i]->data
     *
     *                     [50 75]
     *      [20 30]          [65]          [100]
     *  [..] [..] [..]     [a] [b]        [c] [..]
     *
     * last item of subset[i-1]->data moves up to data,
     * data[i] moves down to the RIGHT of subset[i]->data
     *
     * leaf keys:
     * -------------
     *  fix_shortage(2) calls trnasfer_right(1) so, i = 1 subset[i] is ([7|8])
     *  subset[i+1] is the one being fixed.
     *        [7 | 10]
     *      /    |     \
     * [5|6]->[7 | (8)]=>[()]
     *
     * 1. transfer subset[i(1)]->data[last item] to left of subset[i+1]->data
     *        [7 | (10)]
     *      /    |     \
     *   [5|6]->[7]-> [8]
     *
     * 2. update parent of the transfered subset: data[i (1)] = subset[i+1(2)]->data[0]
     *
     *        [7 | 8]
     *      /    |    \
     *   [5|6]->[7]-> [8]
     *
     *
     */

    // const bool debug = false;
}       //transfer one element RIGHT from child i
template <class T>
BPlusTree<T> *BPlusTree<T>::merge_with_next_subset(int i){
    //TODO : see if you actually change if leaf
    T temp;
    delete_item (data, i, data_count, temp);
    
    if (!(subset [i]->is_leaf())){
        attach_item (subset [i]-> data, subset [i]-> data_count, temp);
    }
    merge (subset [i]-> data, subset [i]-> data_count, subset[i+1]-> data , subset [i+1]->data_count);
    if (!(subset [i+1]->is_leaf()))
    merge (subset [i]-> subset, subset [i]-> child_count,subset[i+1]-> subset, subset [i+1]->child_count);

    BPlusTree <T> *tempo;
    delete_item (subset, i+1, child_count, tempo);
    // cout<<tempo->data_count<<endl;
/*    
    * Exactly the same, but do not attach the deleted data[i] to subset[i]->data[ ]
     *
     *  i = 0 : merge 5 and [()]
     *        [7 | 10]
     *      /    |     \
     *  [5]->   [()]->  [10]
     *
     *  Delete data[i] (7), merge subset[i] and subset[i+1]
     *      WITHOUT bringing down data[i]
     *
     *        [10]
     *      /      \
     *  [5]->      [10]
     *
     *
     * i = 1 merge 7 and [()]
     *        [7 | 10]
     *      /    |     \
     *  [5]->   [7]->  [()]
     *
     *  Delete data[i] (10), merge subset[i] and subset[i+1]
     *      WITHOUT bringing down data[i]
     *
     *        [7]
     *      /     \
     *  [5]->      [7]
     *
     *
     *
     **/
    if (subset [i]->is_leaf()){
        subset [i]->next = tempo->next;
    }
    if (tempo!=nullptr)
        delete tempo;
    return this;
 /*
     * ----------------------------------------------------------------------
     *  Merge subset[i] with subset [i+1] REMOVE data[i];
     *  non leaf: same as BTree
     *  leaves  : delete but do not bring down data[i]
     * ----------------------------------------------------------------------
     *
     *   1. remove data[i] from this object
     *   2. if not a leaf, append it to child[i]->data:
     *   3. Move all data items from subset[i+1]->data to right of subset[i]->data
     *   4. Move all subset pointers from subset[i+1]->subset to
     *          right of subset[i]->subset
     *   5. delete subset[i+1] (store in a temp ptr)
     *   6. if a leaf, point subset[i]->next to temp_ptr->next
     *   6. delete temp ptr
     *
     *
     * non-leaf nodes: (same as BTree)
     * ------------------
     *  i = 0:
     *             [50  100]
     *          /      |     \
     *  [  ]         [75]       ....
     *    |         /    \
     *   [a]      [b]    [c]
     *
     *  bring down data[i], merge it with subset[i] and subset[i+1]:
     *      then, get rid of subset[i+1]
     *             [100]
     *               |
     *            [50|75]       ....
     *            /  |  \
     *         [a]  [b]  [c]
     *
     *
     *
     * leaf node:
     * ----------------
     * Exactly the same, but do not attach the deleted data[i] to subset[i]->data[ ]
     *
     *  i = 0 : merge 5 and [()]
     *        [7 | 10]
     *      /    |     \
     *  [5]->   [()]->  [10]
     *
     *  Delete data[i] (7), merge subset[i] and subset[i+1]
     *      WITHOUT bringing down data[i]
     *
     *        [10]
     *      /      \
     *  [5]->      [10]
     *
     *
     * i = 1 merge 7 and [()]
     *        [7 | 10]
     *      /    |     \
     *  [5]->   [7]->  [()]
     *
     *  Delete data[i] (10), merge subset[i] and subset[i+1]
     *      WITHOUT bringing down data[i]
     *
     *        [7]
     *      /     \
     *  [5]->      [7]
     *
     *
     *
     *
     */
    // const bool debug = false;
    //...

    // return subset[i];
} //merge subset i with  i+1
#endif // BPLUSTREE_H_

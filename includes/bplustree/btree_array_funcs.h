#ifndef BTREE_ARRAY_FUNCS_H_
#define BTREE_ARRAY_FUNCS_H_
#include <iostream>
#include "string.h"
#include "assert.h"
#include <cassert>
#include <vector>
using namespace std;

template <class T>
T maximal(const T& a, const T& b);                      //return the larger of the two items

template <class T>
void swap(T& a, T& b);  //swap the two items

template <class T>
int index_of_maximal(T data[ ], int n);                 //return index of the largest item in data

template <class T>
void ordered_insert(T data[ ], int& n, T entry);        //insert entry into the sorted array
                                                        //data with length n

template <class T>
int first_ge(const T data[ ], int n, const T& entry);   //return the first element in data that is
                                                        //not less than entry

template <class T>
void attach_item(T data[ ], int& n, const T& entry);    //append entry to the right of data

template <class T>
void insert_item(T data[ ], int i, int& n, T entry);    //insert entry at index i in data

template <class T>
void detach_item(T data[ ], int& n, T& entry);          //remove the last element in data and place
                                                        //it in entry

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry);   //delete item at index i and place it in entry

template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2);   //append data2 to the right of data1

template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2);   //move n/2 elements from the right of data1
                                                        //and move to data2

template <class T>
void copy_array(T dest[], const T src[],
                int& dest_size, int src_size);              //copy src[] into dest[]

template <class T>
void print_array(const T data[], int n, int pos = -1);  //print array data

template <class T>
bool is_gt(const T data[], int n, const T& item);       //item > all data[i]

template <class T>
bool is_le(const T data[], int n, const T& item);       //item <= all data[i]

//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list); //print vector list

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme); //list.push_back addme

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


template <class T>
T maximal(const T& a, const T& b){
    if (a > b){
        return a;
    }
    else if (b > a){
        return b;
    }
    return NULL;
}                      //return the larger of the two items

template <class T>
void swap(T& a, T& b){
    T temp = a;
    a = b;
    b = temp;
}  //swap the two items

template <class T>
int index_of_maximal(T data[ ], int n){
    //return data[n];
    //? Up to change
    // T* w = data;
    //* What will you do on dupes??
    int i=0;
    int index=0;
    for (int i=0; i<n;i++){
        if (data[i]>data [index]){
            index = i;
        }
    }
    return index;
    // while (w!=nullptr){
    //     if (*w < n ){
    //         *w = n;
    //         index = n;
    //     }
    //     w++;
    //     i++;
    // }
    
}                 //return index of the largest item in data

template <class T>
void ordered_insert(T data[ ], int& n, T entry){
    // cout<<entry<<endl;
    // cout<<n<<endl;
    int i = first_ge (data, n, entry);
    // cout<<i<<endl;
    // cout<<data[i]<<endl;
    //======================
    //!BEWARE
    // if (data[i] == entry){
    //     cout<<"Replacing\n";
    //     data[i] = entry;
    //     return;
    // }
    //!BEWARE 
    //========================
    // cout<<n<<endl;
    insert_item (data, i, n, entry);
    // cout<<n<<endl;
    // for (int i = 0; i<n ; i++){
    //     // cout<<i<<endl;
    //     if (data [i] > entry){
    //         insert_item (data, i, n, entry);
    //         return;
    //     }
    // }
    // attach_item (data, n, entry);
}        //insert entry into the sorted array
                                                        //data with length n

template <class T>
int first_ge(const T data[ ], int n, const T& entry){
    for (int i = 0; i<n;i++){
        if (data [i]>entry||data[i]==entry){
            return i;
        }
    }
    return n;
}   //return the first element in data that is
                                                        //not less than entry

template <class T>
void attach_item(T data[ ], int& n, const T& entry){
    data [n] = entry;
    n++;
}    //append entry to the right of data

template <class T>
void insert_item(T data[ ], int i, int& n, T entry){
    // cout<<n<<"\t"<<i<<endl;
    if (n==i){
        // cout<<"Attachin\n";
        // cout<<"Special\n";
        attach_item (data, n, entry);
        return;
    }
    // cout<<i<<endl;
    // cout<<n<<endl;
    for (int j = n ; j > i;j--){
        data[j] = data [j-1];
    }
    n++;
    data[i] = entry;

}    //insert entry at index i in data

template <class T>
void detach_item(T data[ ], int& n, T& entry){
    entry = data [n-1];
    // cout<<data[n-1]<<endl;
    // cout<<n<<endl;
    n--;
    // cout<<n<<endl;
}          //remove the last element in data and place
                                                        //it in entry

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry){
    if (i == n-1){
        detach_item (data, n, entry);
        return;
    }
    entry = data [i];
    for (int j = i; j < n; j++)
      {
        data [j] = data [j+1];
        // T* tempo = (walkman+1);
        // *walkman=*tempo;    
      }

    n--;
}   //delete item at index i and place it in entry

template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2){
    for (int i = 0; i<n2; i++){
        data1[n1] = data2 [i]; 
        n1+=1;
        //?Maybe need a +1
    }
    n2 = 0;
}   //append data2 to the right of data1

template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2){
     n2 = n1/2;
     n1 -= n2;
    //  cout<<n1<<endl;
    //  cout<<n2<<endl;
    //  print_array (data1,n1);
     for (int i = 0 ; i < n2; i++){

        // cout<< "index: "<< n1+i <<endl;
        // cout<<"element at index: "<<data1[n1+i]<<endl;
        // cout<<"ELEMENT THAT SHOULDNT BE CHANGED: "<< data1 [i]<<endl;
        // print_array (data1,n1);
        data2[i]= data1[n1+i]; 
        // cout<<data1 [i]<<endl;
        // cout<< n1+i <<endl;

    //  print_array (data1,n1);

     }
    //  cout<<"Done splitting\n";
}   //move n/2 elements from the right of data1
                                                        //and move to data2

template <class T>
void copy_array(T dest[], const T src[],int& dest_size, int src_size){
    //! Check for not equal
    // cout<<dest_size<<endl;
    // cout<<src_size<<endl;
    // assert (!(dest_size == src_size));
    for (int i = 0; i < src_size; i++){
        dest [i] = src[i];
        
        //insert ()
    }
    dest_size = src_size;

}              //copy src[] into dest[]

template <class T>
void print_array(const T data[], int n, int pos){
    for (int i = 0; i<n; i++){
        cout<< "[ "<< data [i] << " ]\t";
    }
    cout<<endl;
}  //print array data

template <class T>
bool is_gt(const T data[], int n, const T& item){
    for (int i = 0; i < n; i++){
        if (data [i]>item)
            return false;
    }
    return true;
}       //item > all data[i]

template <class T>
bool is_le(const T data[], int n, const T& item){
    for (int i = 0; i < n; i++){
        if (data [i]<= item){
            return false;
        }
    }
    return true;
}       //item <= all data[i]

//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list){
 for (int i = 0; i<list.size(); i++){
        cout<< "[ "<< list.at(i) << " ]\t";
    }
    return outs;
} //print vector list

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme){
    list.push_back (addme);
    return list;
} //list.push_back addme


#endif // BTREE_ARRAY_FUNCTIONS_H_
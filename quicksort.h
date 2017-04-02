/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   quicksort.h
 * Author: kxw
 *
 * Created on August 17, 2016, 5:39 AM
 */

#ifndef QUICKSORT_H
#define QUICKSORT_H
#include <cstdlib>
#include <string>
#include <iostream>
#include <algorithm>
#include "mergesort.h"
using namespace std;
void myswap(kv &a,kv& b){
    string c_k = a.k;
    string c_v = a.v;
    a.k = b.k;
    a.v = b.v;
    b.k = c_k;
    b.v = c_v;
}
template <typename T>
string  Median3(T A[], int p, int q )
{
    int c = ( p + q ) / 2;
    if( get_id(A[p])> get_id(A[c]))
        myswap(A[p], A[c]);
    if( get_id(A[p])>get_id(A[q]))
        myswap(A[p], A[q]);
    if( get_id(A[c])>get_id(A[q]))
        myswap(A[c], A[q]);
    myswap(A[c],A[q-1]);
    return get_id(A[q-1]);
}
template<typename T>
int Partition( T A[], int p, int q )
{
    string  key = Median3( A, p, q );
    int i = p;
    int j = q-1;
    while(1)
    {
        while(get_id(A[++i])<key ){}
        while( get_id(A[--j])>key){}
        if( i < j )
            myswap( A[i], A[j] );
        else
            break;
    }
    myswap( A[i], A[q-1] );
    return i;
}
template<typename T>
void InsertionSort(T A[], int N)
{
    T tmp;
    int j;
    int p;

    for( p = 1; p < N; p++ )
    {
        tmp.k= A[p].k;
        tmp.v = A[p].v;
        for( j = p; j > 0 && get_id(A[j -1])>get_id(tmp); j -- )
        {
            A[j].k = A[j-1].k;
            A[j].v = A[j-1].v;
        }
        A[j].k = tmp.k;
        A[j].v = tmp.v;
    }
}
#define cutoff 5
template<typename T>
void QuickSort(T A[],int p, int q)
{
    if( p + cutoff <= q )
    {
        int r = Partition(A, p, q);
        QuickSort( A, p, r - 1 );
        QuickSort( A, r + 1, q );
    }
    else
        InsertionSort(A + p, q - p + 1 );
}


#endif /* QUICKSORT_H */


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LinkedList.hpp
 * Author: Hans
 *
 * Created on May 1, 2017, 1:35 PM
 */

#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

template<typename T>
struct ListNode{
    T data;
    int* next;
    ListNode(){
        next = nullptr;
    }
};

template<typename T>
int* ListAppend(T newLVal, int* listptr)
{
    ListNode<T>* l = new ListNode<T>();
    l->data = newLVal;
    l->next = listptr;
    return (int*)l;
}

int* ListInit()
{
    return nullptr;
}

template<typename T>
void ListRead(int* listPtr, void(*f)(T))
{
    ListNode<T>* l = (ListNode<T>*)listPtr;
    while(l != nullptr)
    {
        (*f)(l->data);
        auto temp = l;
        l = (ListNode<T>*)(l->next);
        delete temp;
    }
}

#endif /* LINKEDLIST_HPP */


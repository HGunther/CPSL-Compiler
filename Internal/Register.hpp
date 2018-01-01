/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Register.h
 * Author: Hans
 *
 * Created on April 29, 2017, 5:52 PM
 */

#ifndef REGISTER_HPP
#define REGISTER_HPP

#include<string>

//using Register = std::string;
struct Register{
public:
    std::string id;
    Register(){
        id = "";
    }
    Register(std::string s)
    {
        id = s;
    }
    Register(const Register& r){
        id = r.id;
    }
    //Register(Register&&) = default;
    ~Register() = default;
    Register& operator=(Register rhs){
        id = rhs.id;
        return *this;
    }
};

std::ostream &operator<<(std::ostream &os, Register const &r) { 
    return os << r.id;
}

#endif /* REGISTER_HPP */


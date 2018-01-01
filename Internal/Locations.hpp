/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Locations.hpp
 * Author: Hans
 *
 * Created on April 28, 2017, 4:27 PM
 */

#ifndef LOCATIONS_HPP
#define LOCATIONS_HPP

#include <string>
#include "Internal/SymbolTable.hpp"

class MemoryLocation{
public:
    MemoryLocation(){
        _isReg = false;
    }
    MemoryLocation(std::string base){
      _base = base;
      _isReg = true;
    };
    MemoryLocation(std::string base, int offset){
        _base = base;
        _offset = offset;
        _isReg = false;
    };
    MemoryLocation(const MemoryLocation& l){
        _base = l._base;
        _offset = l._offset;
        _isReg = l._isReg;
    }
    ~MemoryLocation() = default;
    bool isRegister() {
        return _isReg;
    }
    MemoryLocation& operator=(const MemoryLocation& ) = default;
    std::string toString() {
        if (isRegister()){
            return _base;
        }
        else {
            std::string s = std::to_string(_offset);
            s.append("(");
            s.append(_base);
            s.append(")");
            return s;
        }
    }

private:
    std::string _base;
    int _offset;
    bool _isReg;
};

std::ostream &operator<<(std::ostream &os, MemoryLocation &l) { 
    os << l.toString();
    return os ;
}


#endif /* LOCATIONS_HPP */


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef USERDEFINEDDATATYPES_CPP
#define USERDEFINEDDATATYPES_CPP

#include <stack>
#include <string>
#include <stdexcept>
#include "Internal/SymbolTable.hpp"
#include "Internal/RegisterPool.hpp"
#include "Internal/ExpressionType.hpp"
#include "Internal/MIPS.hpp"
#include "Internal/HighMIPS.hpp"

extern SYMBOLTABLE_HPP::SymbolTable symbolTable;
extern SYMBOLTABLE_HPP::SymbolTable typeTable;
std::stack<Symbol> g_symbols;

char* declareArray(intExpression a, intExpression b, char* _type){
    Expression lower = fromInt(a);
    Expression upper = fromInt(b);
    std::string type = std::string{_type};
    
    Symbol symb = Symbol();
    symb.id = "TempSymbol";
    if (!lower.isConstant || !upper.isConstant){
        throw std::runtime_error("Tried to set the bounds of an array using non-constants");
    }
    symb.size = upper.value - lower.value + 1;
    symb.size = symb.size * typeTable.getSize(type);
    if (symb.size <= 0){
        throw std::runtime_error("Tried to create an array with zero or fewer elements");
    }
    symb.value = lower.value;
    symb.type = "array";
    symb.subType = type;
    g_symbols.push(symb);
    //typeTable.store(symb);
    char* r = nullptr;
    return r;
}

char* declareRec(){
    return new char['r', 'e', 'c', 'o', 'r', 'd', '\0'];
}

void typeDeclaration(char* _id, char* _type){
    std::string id = std::string{_id};
    //if(type == "array" || type == "record"){
    if(_type == nullptr){
        Symbol symb = g_symbols.top();//typeTable.lookup("TempSymbol");
        g_symbols.pop();
        symb.id = id;
        typeTable.store(symb);
    }
    else{
        std::string type = std::string{_type};
        Symbol symb = typeTable.lookup(type);
        symb.id = id;
        typeTable.store(symb);
    }
    //std::cout << "Recieved id=" << id << ", and type'" << type << "'." << std::endl;
    
}

intExpression loadArrayElement(intExpression a, intExpression b){
    Expression lvalue = fromInt(a);
    Expression shift = fromInt(b);
    
    Symbol typeInfo = typeTable.lookup(lvalue.type);
    Symbol subTypeInfo = typeTable.lookup(typeInfo.subType);
    Register offset = registers.getRegister();
    
    mips::subi(offset, shift.reg, typeInfo.value);
    mips::li(shift.reg, subTypeInfo.size);
    mips::mult(offset, shift.reg);
    mips::mflo(offset);
    mips::add(lvalue.reg, lvalue.reg, offset);
    registers.returnRegister(offset);
    
    lvalue.type = typeInfo.subType;
    lvalue.isConstant = false;
    return toInt(lvalue);
}

intExpression loadRecElement(intExpression a, char* c){
    
}

#endif /* USERDEFINEDDATATYPES_CPP */

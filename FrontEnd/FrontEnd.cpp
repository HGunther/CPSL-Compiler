#ifndef FRONTEND_H
#define FRONTEND_H

#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <stdexcept>
#include "Internal/SymbolTable.hpp"
#include "Internal/RegisterPool.hpp"
#include "Internal/ExpressionType.hpp"
#include "Internal/LinkedList.hpp"
#include "Internal/MIPS.hpp"
#include "Internal/HighMIPS.hpp"

extern std::ofstream ofile;
extern int linenumber;
//extern SYMBOLTABLE_HPP::SymbolTable<std::tuple<std::string, std::string>> symbtable;
SYMBOLTABLE_HPP::SymbolTable symbolTable;
SYMBOLTABLE_HPP::SymbolTable typeTable;
REGISTERPOOL_HPP::RegisterPool registers;

//SYMBOLTABLE_HPP::SymbolTable<int> int_table;
//SYMBOLTABLE_HPP::SymbolTable<char> char_table;
//SYMBOLTABLE_HPP::SymbolTable<bool> bool_table;
//SYMBOLTABLE_HPP::SymbolTable<std::tuple<std::string, std::string>> symbtable;
//SYMBOLTABLE_HPP::SymbolTable symbtable;
std::map<int, Expression> expressionDictionary;

int gpOffset = 0;
int labelCounter = 0;

void Precompile(){
    //ofile << ".data" << std::endl;
    ofile << ".text" << std::endl;
    ofile << "\taddi $fp, $gp, 0" << std::endl;
    
    //linenumber = 0;
//     symbtable = SYMBOLTABLE_HPP::SymbolTable<std::tuple<std::string, std::string>> ();
    registers = REGISTERPOOL_HPP::RegisterPool();
    expressionDictionary = std::map<int, Expression>{};
    
    //create predefined types
    typeTable = SymbolTable{};
    typeTable.store(Symbol("integer", "integer", 4));
    typeTable.store(Symbol("char", "char", 4));
    typeTable.store(Symbol("boolean", "boolean", 4));
    
    //create predefined constantants
    symbolTable = SymbolTable{};
    Symbol s = Symbol();
    s.id = "true";
    Symbol typeInfo = typeTable.lookup("boolean");
    s.size = typeInfo.size;
    s.location = symbolTable.allocateLocalVariable(s.size);
    s.isConst = true;
    s.value = 1;
    symbolTable.store(s);
    
    s.id = "false";
    s.size = typeInfo.size;
    s.location = symbolTable.allocateLocalVariable(s.size);
    s.isConst = true;
    s.value = 0;
    symbolTable.store(s);
}

void programStart()
{
    //initializeConstants();
    ofile << ".text" << std::endl;
    ofile << "programStart:" << std::endl;
    return;
}

//void initializeConstants()
//{
//    ofile << "#Defining constants" << std::endl;
//    ofile << "true" << ": .space " << 4 << std::endl;
//    ofile << "false" << ": .space " << 4 << std::endl;    
//}

void programEnd()
{
    ofile << "programEnd:" << std::endl;
    mips::syscall(10);
    return;
}

//std::string makeLocation(int size)
//{
//    std::string location;
//    location.append(std::to_string(gpOffset));
//    location.append("($gp)");
//    gpOffset += size;
//    return location;
//}

std::string makeLabel()
{
    return std::to_string(labelCounter++);
}

void variableDeclaration(std::string id, std::string type)
{
    Symbol symb = Symbol();
    symb.id = id;
    Symbol typeInfo = typeTable.lookup(type);
    symb.type = typeInfo.id;
    symb.size = typeInfo.size;
    symb.isConst = false;
    symb.value = typeInfo.value;
    
    std::cout << "Creating '" << symb.id << "' as a '" << symb.type << "'" << std::endl;

    symb.location = symbolTable.allocateLocalVariable(symb.size);
    symbolTable.store(symb);
}

void ConstDeclaration(char* c, intExpression a){
    std::string id = std::string{c};
    Expression exp = fromInt(a);
    if (!exp.isConstant){
        throw std::runtime_error("Exception: Cannot initialize a constant with a non-constant value");
    }
    Symbol symb = Symbol();
    symb.id = id;
    symb.type = exp.type;
    symb.size = typeTable.getSize(symb.type);
    symb.location = symbolTable.allocateLocalVariable(symb.size);
    symb.isConst = true;
    symb.isRef = false;
    symb.value = exp.value;
    std::clog << "Creating constant '" << symb.id << "' with a value of '" << symb.value << "'" << std::endl;
    symbolTable.store(symb);
    //Initialize with value
    //ofile << "\t#Setting constant '" << symb.id << "' with initial value" << std::endl;
    //Register newRegister = registers.getRegister();
    //mips::la(newRegister, symb.location);
    //mips::sw(exp.reg, newRegister, 0);
    //registers.returnRegister(newRegister);
}

void assign(const Expression& lvalue, const Expression& rvalue){
    if(lvalue.isConstant){throw std::runtime_error("Exception: Tried to assign into a constant");}
    if(lvalue.type != rvalue.type){std::runtime_error("Exception: Tried to assign a "
        + lvalue.type + " variable with a " + rvalue.type);
    }
    mips::sw(rvalue.reg, lvalue.reg, 0);
}
void assign(std::string idToBeAssigned, const Expression& rvalue){
    ofile << "\t#Setting '" << idToBeAssigned << "' equal to value in " << rvalue.reg << std::endl;
    Symbol symb = symbolTable.lookup(idToBeAssigned);
    Expression lvalue = Expression();
    if(symb.isConst){throw std::runtime_error("Exception: Tried to assign into a constant");}
    lvalue.type = symb.type;
    mips::la(lvalue.reg, symb.location);
    assign(lvalue, rvalue);
}
void assign(std::string idToBeAssigned, intExpression _rvalue){
    Expression rvalue = fromInt(_rvalue);
    assign(idToBeAssigned, rvalue);
}
void assign(intExpression a, intExpression b)
{
    Expression lvalue = fromInt(a);
    Expression rvalue = fromInt(b);
    assign(lvalue, rvalue);
    return;
}

intExpression intval(const int& val)
{
    Expression expr;
    expr.type = "integer";
    expr.isConstant = true;
    expr.value = int{val};
    ofile << "\t#Loading value " << expr.value << " into register " << expr.reg << std::endl;
    mips::li(expr.reg, expr.value);
    return toInt(expr);
}

intExpression loadValue(intExpression _in)
{
    Expression in = fromInt(_in);
    if(in.isConstant){
        mips::li(in.reg, in.value);
    }
    else{
        mips::lw(in.reg, in.reg, 0);
    }
    return toInt(in);
}

intExpression loadAddress(char* _id){
    std::string id = std::string{_id};
    
    Symbol symb = symbolTable.lookup(id);
    Expression exp = Expression();
    exp.type = symb.type;
    exp.isConstant = symb.isConst;
    exp.value = symb.value;
    mips::la(exp.reg, symb.location);
    
    return toInt(exp);
}

void write(intExpression e){
    Expression expr = fromInt(e);
    ofile << "\t#Writing the value in " << expr.reg << std::endl;
    if (expr.type == "string"){
        mips::moveReg(expr.reg, "$a0");
        mips::syscall(4);
    }
    else if (expr.type == "char"){
        mips::moveReg(expr.reg, "$a0");
        mips::syscall(11);
    }
    else{
        mips::moveReg(expr.reg, "$a0");
        mips::syscall(1);
    }
}

int* expressionListInit(){
    return ListInit();
}

int* expressionListAppend(intExpression newExpr, int* listptr){
    return ListAppend(newExpr, listptr);
}

void writelist(int* listPtr){
    ListRead(listPtr, write);
}

intExpression stringval(char* _s)
{
    std::string s{_s};
    ofile << ".data" << std::endl;
    std::string stringAddress{"string"};
    stringAddress += makeLabel();
    ofile << stringAddress << ": .asciiz " << s << std::endl;
    ofile << ".text" << std::endl;
    Expression expr;
    expr.type = "string";
    mips::la(expr.reg, stringAddress);
    return toInt(expr);
}

intExpression charval(char* _c){
    char c = _c[1];
    Expression expr = Expression();
    expr.type = "char";
    expr.isConstant = true;
    expr.value = (int) c;
    Register zero("$zero");
    mips::addi(expr.reg, zero, (int)c);
    return toInt(expr);
}

void read_exp(const Expression& exp)
{
    if(exp.isConstant){throw std::runtime_error("Exception: cannot read a value into a constant");}
    mips::comment("Reading in " + exp.type);
    if (exp.type == "integer"){
        mips::syscall(5);
        mips::sw("$v0", exp.reg, 0);
    }
    else if (exp.type == "char"){
        mips::syscall(12);
        mips::sw("$v0", exp.reg, 0);
    }
    else if(exp.type == "boolean"){
        mips::syscall(5);
        mips::sw("$v0", exp.reg, 0);
    }
    else{
        throw std::runtime_error("Exception: Tried to read in illegal type " + exp.type);
    }
}

int* lvalueListAppend(intExpression newLVal, int* listptr){
    return ListAppend(newLVal, listptr);
}

int* lvalueListInit(){
    return ListInit();
}

void read(intExpression _exp){
    Expression exp = fromInt(_exp);
    read_exp(exp);
}

void readlist(int* listPtr){
    ListRead(listPtr, read);
}

int* identListInit(char* c){
    int* p = ListInit();
    std::string s = std::string{c};
    return ListAppend(s, p);
}

int* identListAppend(char* c, int* p){
    std::string s = std::string{c};
    return ListAppend(s, p);
}

void variableDeclarationList(int* listPtr, char* type){
    ListNode<std::string>* l = (ListNode<std::string>*)listPtr;
    std::string s_type = std::string{type};
    while(l != nullptr)
    {
        variableDeclaration(l->data, s_type);
        auto temp = l;
        l = (ListNode<std::string>*)(l->next);
        delete temp;
    }
}

void stop(){
    mips::j("programEnd");
}

#endif	// FRONTEND_H


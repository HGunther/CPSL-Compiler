/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SymbolTable.hpp
 * Author: hans
 *
 * Created on February 21, 2017, 6:44 PM
 */

#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include <vector>
#include <string>
#include <map>
#include <tuple>
#include "Internal/Locations.hpp"
#include "Internal/MIPS.hpp"
#include "Internal/HighMIPS.hpp"

struct Symbol{
    Symbol() = default;
    Symbol(std::string i, std::string t, int s)
    {
        id = i;
        location = MemoryLocation{};
        type = t;
        size = s;
        isConst = false;
        int value = 0;
        bool isRef = false;
    }
    Symbol(std::string i, MemoryLocation l, std::string t, int s){
        id = i;
        location = MemoryLocation{l};
        type = t;
        size = s;
        isConst = false;
        int value = 0;
        bool isRef = false;
    }
    Symbol& operator=(const Symbol& s){
        id = std::string{s.id};
        location = s.location;
        type = std::string{s.type};
        subType = std::string{s.subType};
        size = s.size;    
        isConst = s.isConst;
        value = s.value;
        isRef = s.isRef;
    }
    std::string id;
    MemoryLocation location;
    std::string type;
    std::string subType;
    int size;    
    bool isConst;
    int value;
    bool isRef;
};

class SymbolTable {
public:
    SymbolTable()
    {
        symbols.emplace_back();
        fpOffsets.push_back(0);
        spOffsets.push_back(0);
    }
    
    void store(Symbol s)
    {
        auto id = s.id;
        auto f = symbols.back().find(id);
        if (f != symbols.back().end())
        {
            if (s.id == "TempSymbol"){
                symbols.back()[id] = s;
            }
            else{throw std::runtime_error(id + " has already been defined");}
        }
        symbols.back()[id] = s;
    }
    
    Symbol lookup(std::string id)
    {
        for (int i = symbols.size() - 1; i >= 0; i--)
        {
            auto f = symbols[i].find(id);
            if (f != symbols[i].end() ){
                return f->second;
            }
        }
        throw std::runtime_error("Exception: " + id + " could not be found in symbol table.");
    }
    
    bool contains(std::string id)
    {
        for (int i = symbols.size() - 1; i >= 0; i--)
        {
            auto f = symbols[i].find(id);
            if (f != symbols[i].end() ){
                return true;
            }
        }
        return false;
    }
    
    MemoryLocation getLocation(std::string id)
    {
        return lookup(id).location;
    };
    std::string getType(std::string id)
    {
        return lookup(id).type;
    };
    int getSize(std::string id){
        return lookup(id).size;
    }
    
    void enterScope()
    {
        symbols.emplace_back();
        fpOffsets.push_back(0);
        spOffsets.push_back(0);
    }
    
    void exitScope()
    {
        symbols.pop_back();
        fpOffsets.pop_back();
        spOffsets.pop_back();
    }
    
    bool inLocalScope()
    {
        return symbols.size() == 2u;
    }
        
    MemoryLocation allocateLocalVariable(int size){
        int offset = fpOffsets.back();
        fpOffsets.back() += size;
        mips::comment("Variable allocated. $sp pushed to end of $fp allocations.");
        mips::la("$sp", "$fp", offset + size);
        MemoryLocation l = MemoryLocation("$fp", offset);
        return l;
    }

    MemoryLocation allocateStackMemory(int size){
        int offset = spOffsets.back();
        spOffsets.back() += size;
        MemoryLocation l = MemoryLocation("$sp", offset);
        return l;
    }
    
    //SymbolTable(const SymbolTable& orig);
    //virtual ~SymbolTable();
private:
    std::vector<std::map<std::string, Symbol>> symbols;
    std::vector<int> fpOffsets;
    std::vector<int> spOffsets;
};

#endif /* SYMBOLTABLE_HPP */


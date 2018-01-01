/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RegisterPool.hpp
 * Author: hans
 *
 * Created on February 24, 2017, 6:25 PM
 */

#ifndef REGISTERPOOL_HPP
#define REGISTERPOOL_HPP

#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include "Internal/Register.hpp"

class RegisterPool
{
public:
    RegisterPool()
    {
        pool = std::vector<Register> {};
        pool.emplace_back("$v0");
        pool.emplace_back("$v1");
        pool.emplace_back("$a0");
        pool.emplace_back("$a1");
        pool.emplace_back("$a2");
        pool.emplace_back("$a3");
        pool.emplace_back("$s7");
        pool.emplace_back("$s6");
        pool.emplace_back("$s5");
        pool.emplace_back("$s4");
        pool.emplace_back("$s3");
        pool.emplace_back("$s2");
        pool.emplace_back("$s1");
        pool.emplace_back("$s0");
        pool.emplace_back("$t9");
        pool.emplace_back("$t8");
        pool.emplace_back("$t7");
        pool.emplace_back("$t6");
        pool.emplace_back("$t5");
        pool.emplace_back("$t4");
        pool.emplace_back("$t3");
        pool.emplace_back("$t2");
        pool.emplace_back("$t1");
        pool.emplace_back("$t0");
        
        //pool = std::vector<int> ();
        //for (int i = 0; i < 24; i++)
        //{
        //    pool.emplace_back(i);
        //    dictionary[i] = registers[i];
        //}
    }
    ~RegisterPool() = default;
    Register getRegister()
    {
        if (pool.empty())
        {
            throw std::runtime_error("Exception: Requested a register when the register pool was empty.");
        }
        Register reg = pool.back();
        pool.pop_back();
        return reg;
    }
    void returnRegister(Register reg)
    {
        pool.push_back(reg);
        return;
    }
    //std::string lookupRegister(int reg)
    //{
    //    return dictionary[reg];
    //}
private:
    std::vector<Register> pool;
    //std::map<int, std::string> dictionary;
};

#endif /* REGISTERPOOL_HPP */
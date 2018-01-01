/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef HIGHMIPS_HPP
#define HIGHMIPS_HPP

#include <string>
#include "Internal/Register.hpp"

namespace mips
{
    void moveReg(Register sourceReg, Register destReg)
    {
        mips::add(destReg, sourceReg, Register("$zero"));
    }
    
    void moveReg(Register sourceReg, std::string destReg)
    {
        mips::add(destReg, sourceReg, Register("$zero"));
    }
    
    void comment(std::string s){
        ofile << "\t#" << s << std::endl;
    }
    
    void multiply(Register destReg, Register sourceReg1, Register sourceReg2){
        mult(sourceReg1, sourceReg2);
        mflo(destReg);
    }
    
    void writeLabel(std::string s){
        ofile << s << ":" << std::endl;
    }
    
}

#endif /* HIGHMIPS_HPP */
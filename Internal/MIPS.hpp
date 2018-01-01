/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MIPS.hpp
 * Author: hans
 *
 * Created on February 24, 2017, 9:47 PM
 */

#ifndef MIPS_HPP
#define MIPS_HPP

#include <iostream>
#include <fstream>
#include <string>
#include "Register.hpp"

extern std::ofstream ofile;
//extern REGISTERPOOL_HPP::RegisterPool registers;
using address = std::string;

namespace mips
{
    template<typename T1, typename T2>
    void la(T1 loadTo, T2 loadFrom, int offset = -1)
    {
        if(offset != -1)
        {
            ofile << "\tla " << loadTo << ", " << offset << "("
                    << loadFrom << ")" << std::endl;
        }
        else
        {
            ofile << "\tla " << loadTo << ", "
                    << loadFrom << std::endl;
        }
        return;
    }
    
    template<typename T1, typename T2>
    void lw(T1 loadTo, T2 loadFrom, int offset = -1)
    {
        if(offset != -1)
        {
            ofile << "\tlw " << loadTo << ", " << offset << "("
                    << loadFrom << ")" << std::endl;
        }
        else
        {
            ofile << "\tlw " << loadTo << ", "
                    << loadFrom << std::endl;
        }
        return;
    }
    
    template<typename T1, typename T2>
    void sw(T1 storeFrom, T2 storeTo, int offset = -1)
    {
        if(offset != -1){
            ofile << "\tsw " << storeFrom
                << ", " << offset << "(" << storeTo << ")" << std::endl;
        }
        else{
            ofile << "\tsw " << storeFrom
                << ", " << storeTo << std::endl;
        }
        
        return;
    }

    void li(Register storeTo, int value)
    {
        ofile << "\tli " << storeTo
                << ", " << value << std::endl;
        return;
    }
    
    void li(std::string storeTo, int value)
    {
        ofile << "\tli " << storeTo
                << ", " << value << std::endl;
        return;
    }
    
    void syscall(int x)
    {
        mips::li("$v0", x);
        ofile << "\tsyscall" << std::endl;
    }
    
    void add(Register storeToReg, Register add1Reg, Register add2Reg)
    {
        ofile << "\tadd " << storeToReg
                << ", " << add1Reg
                << ", " << add2Reg << std::endl;
        return;
    }
    
    void addi(Register storeToReg, Register sourceReg, int value)
    {
        ofile << "\taddi " << storeToReg
                << ", " << sourceReg
                << ", " << value << std::endl;
        return;
    }
    
    void sub(Register storeToReg, Register add1Reg, Register add2Reg)
    {
        ofile << "\tsub " << storeToReg
                << ", " << add1Reg
                << ", " << add2Reg << std::endl;
        return;
    }
    
    void subi(Register storeToReg, Register add1Reg, int value)
    {
        ofile << "\tsubi " << storeToReg
                << ", " << add1Reg
                << ", " << value << std::endl;
        return;
    }
    
    void mult(Register sourceReg1, Register sourceReg2){
        ofile << "\tmult " << sourceReg1 << ", " << sourceReg2 << std::endl;
    }
    
    void div(Register sourceReg1, Register sourceReg2){
        ofile << "\tdiv " << sourceReg1 << ", " << sourceReg2 << std::endl;
    }
    
    void mfhi(Register destReg){
        ofile << "\tmfhi " << destReg << std::endl;
    }
    
    void mflo(Register destReg){
        ofile << "\tmflo " << destReg << std::endl;
    }
    
    void slt(Register destReg, Register sourceReg1, Register sourceReg2){
        ofile << "\tslt " << destReg
                << ", " << sourceReg1
                << ", " << sourceReg2 << std::endl;
        return;
    }
    
    void slti(Register destReg, Register sourceReg1, int value){
        ofile << "\tslti " << destReg
                << ", " << sourceReg1
                << ", " << value << std::endl;
        return;
    }
    
    void b_and(Register destReg, Register sourceReg1, Register sourceReg2){
        ofile << "\tand " << destReg
                << ", " << sourceReg1
                << ", " << sourceReg2 << std::endl;
        return;
    }
    
    void l_and(Register destReg, Register sourceReg1, Register sourceReg2){
        ofile << "\tand " << destReg
                << ", " << sourceReg1
                << ", " << sourceReg2 << std::endl;
        return;
    }
    
    void l_or(Register destReg, Register sourceReg1, Register sourceReg2){
        ofile << "\tor " << destReg
                << ", " << sourceReg1
                << ", " << sourceReg2 << std::endl;
        return;
    }
    
    void nor(Register destReg, Register sourceReg1, Register sourceReg2){
        ofile << "\tnor " << destReg
                << ", " << sourceReg1
                << ", " << sourceReg2 << std::endl;
        return;
    }
    
    template<typename T>
    void jal(T label){
        ofile << "\tjal " << label << std::endl;
    }
    
    template<typename T>
    void beq(Register reg1, Register reg2, T label){
        ofile << "\tbeq " << reg1 << ", " << reg2 << ", " << label << std::endl;
    }
    
    template<typename T>
    void bne(Register reg1, Register reg2, T label){
        ofile << "\tbne " << reg1 << ", " << reg2 << ", " << label << std::endl;
    }
    
    template<typename T>
    void j(T label){
        ofile << "\tj " << label << std::endl;
    }
    
    
    
}
#endif /* MIPS_HPP */


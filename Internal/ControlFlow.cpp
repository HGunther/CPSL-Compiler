/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stack>
#include <string>
#include "FrontEnd/FrontEnd.cpp"
#include "MIPS.hpp"
#include "HighMIPS.hpp"
#include "ExpressionType.hpp"
#include "Expressions.cpp"
#include "SymbolTable.hpp"
#include "RegisterPool.hpp"

extern REGISTERPOOL_HPP::RegisterPool registers;

std::stack<int> sectionStack = std::stack<int>{};
std::stack<int> labelStack = std::stack<int>{};

int ifOpening(intExpression a){
    Expression exp = fromInt(a);
    std::string label = makeLabel();
    sectionStack.push(1);
    labelStack.push(std::stoi(label));
    Register zero("$zero");
    mips::beq(exp.reg, zero, "Else" + label + "Part1");    
    return std::stoi(label);
}

void elseiflist_start(){
    int part = sectionStack.top();
    int label = labelStack.top();
    mips::j("ifEnd" + std::to_string(label));
    mips::writeLabel("Else" + std::to_string(label) + "Part" + std::to_string(part));
}

void elseiflist_later(intExpression a){
    Expression exp = fromInt(a);
    int part = sectionStack.top();
    sectionStack.pop();
    sectionStack.push(part + 1);    
    int label = labelStack.top();
    Register zero("$zero");
    mips::beq(exp.reg, zero, "Else" + std::to_string(label) + "Part" + std::to_string(part + 1));    
}

void elselist(){
    int part = sectionStack.top();
    sectionStack.pop();
    sectionStack.push(part + 1);    
    int label = labelStack.top();
    mips::j("ifEnd" + std::to_string(label));
    
    mips::writeLabel("Else" + std::to_string(label) + "Part" + std::to_string(part));
}

void ifClosing(int label){
    int part = sectionStack.top();
    int Label = labelStack.top();
    sectionStack.pop();
    labelStack.pop();
    
    mips::writeLabel("Else" + std::to_string(label) + "Part" + std::to_string(part));
    mips::writeLabel("ifEnd" + std::to_string(label));
}


void whileInit(){
    std::string label = makeLabel();
    labelStack.push(std::stoi(label));
    mips::writeLabel("WhileEvaluate" + label);
}

void whileBody(intExpression a){
    Expression exp = fromInt(a);
    Register zero("$zero");
    int label = labelStack.top();
    mips::beq(exp.reg, zero, "WhileEnd" + std::to_string(label)); 
    mips::writeLabel("WhileBody" + std::to_string(label));
}

void whileEnd(){
    int label = labelStack.top();
    labelStack.pop();
    mips::j("WhileEvaluate" + std::to_string(label));
    mips::writeLabel("WhileEnd" + std::to_string(label));
}

void repeatInit(){
    std::string label = makeLabel();
    labelStack.push(std::stoi(label));
    mips::writeLabel("RepeatStart" + label);
}

void repeatEnd(intExpression a){
    Expression exp = fromInt(a);
    Register zero("$zero");
    int label = labelStack.top();
    labelStack.pop();
    mips::beq(exp.reg, zero, "RepeatStart" + std::to_string(label)); 
}

void forInit(char* c, intExpression iValue, int upOrDown, intExpression fValue){
    std::string label = makeLabel();
    labelStack.push(std::stoi(label));
    std::string id = std::string(c);
    Expression iExp = fromInt(iValue);
    
    //create variable, if it does not already exist
    if(!symbolTable.contains(id)){
        variableDeclaration(id, iExp.type);
    }
    Symbol symb = symbolTable.lookup(id);
    //set the variable to its initial value
    assign(id, iExp);
    //save the value of the loop condition
    MemoryLocation conditionLocation;
    {
        Expression fExp = fromInt(fValue);    
        conditionLocation = symbolTable.allocateLocalVariable(typeTable.getSize(fExp.type));
        mips::sw(fExp.reg, conditionLocation);
    }
    //jump to evaluate
    mips::j("ForEvaluate" + label);
    //write the increment/decrement procedure
    mips::writeLabel("ForIncrement" + label);
    {
        Expression temp = Expression();
        temp.type = iExp.type;
        mips::lw(temp.reg, symb.location);
        if (upOrDown > 0){ //Increment
            assign(id, e_succ( toInt(temp) ) );
        }
        else{  //Decrement
            assign(id, e_pred( toInt(temp) ) );
        }
    }
    //write Evaluate. jump to End if condition is false
    mips::writeLabel("ForEvaluate" + label);
    Expression vExp = Expression();
    vExp.type = iExp.type;
    mips::lw(vExp.reg, symb.location);
    Register zero = Register("$zero");
    Expression condition = Expression();
    mips::lw(condition.reg, conditionLocation);
    if (upOrDown > 0){
        Expression switchExp = lessthanequal(vExp, condition);
        mips::beq(switchExp.reg, zero, "ForEnd" + label);
    }
    else{
        Expression switchExp = lessthanequal(condition, vExp);
        mips::beq(switchExp.reg, zero, "ForEnd" + label);
    }
    mips::comment("ForBody" + label);
}

void forEnd(){
    int label = labelStack.top();
    labelStack.pop();
    //jump to incrment/decrement section
    mips::j("ForIncrement" + std::to_string(label));
    //write label End
    mips::writeLabel("ForEnd" + std::to_string(label));
}
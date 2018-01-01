/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Expression.hpp
 * Author: Hans
 *
 * Created on March 18, 2017, 7:06 PM
 */

#ifndef EXPRESSIONTYPE_HPP
#define EXPRESSIONTYPE_HPP

#include <map>
#include <string>
#include "Internal/Register.hpp"
#include "Internal/RegisterPool.hpp"

extern REGISTERPOOL_HPP::RegisterPool registers;
class Expression;
extern std::map<int, Expression> expressionDictionary;
using intExpression = int;


static int expressionCounter;

class Expression
{
public:
    Expression(){
        reg = registers.getRegister();
        id = expressionCounter;
        expressionCounter += 1;
        type = "";
        isConstant = false;
        value = -1;
    }
    Expression(Expression&& expr){
        reg.id = expr.reg.id;
        expr.reg.id = "";
        id = expr.id;
        type = expr.type;
        isConstant = expr.isConstant;
        value = expr.value;
    }
    ~Expression()
    {
        if (!reg.id.empty())
            registers.returnRegister(reg);
    }
        std::string type; 
	bool isConstant;
	int value;
	REGISTERPOOL_HPP::Register reg;
        int id;
};

intExpression toInt(Expression& e)
{
    int id = e.id;
    expressionDictionary.emplace( id, std::move(e) );
    return id;
}

Expression fromInt(intExpression id)
{
    Expression e = std::move(expressionDictionary[id]);
    if (e.reg.id == ""){
        throw std::runtime_error("Tried to find an expression that did not exist.");
    }
    expressionDictionary.erase(e.id);
    return e;
}

//Expression makeExpr(){
//   Expression expr = Expression();
//    expressionDictionary[expr.id] = expr;
//}

#endif /* EXPRESSION_HPP */


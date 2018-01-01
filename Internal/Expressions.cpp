/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#ifndef EXPRESSIONS_CPP
#define EXPRESSIONS_CPP

#include "ExpressionType.hpp"
#include "Internal/MIPS.hpp"
#include "Internal/HighMIPS.hpp"
#include "SymbolTable.hpp"

extern SYMBOLTABLE_HPP::SymbolTable symbolTable;

intExpression e_plus(intExpression a, intExpression b){
    Expression expA = fromInt(a);
    Expression expB = fromInt(b);
    if(expA.type != expB.type){
        throw std::runtime_error("Exception: tried to add a " + expA.type + " and a " + expB.type);
    }
    Expression answer{};
    answer.type = expA.type;
    if(expA.isConstant && expB.isConstant){
        answer.isConstant = true;
        answer.value = expA.value + expB.value;
    }
    mips::add(answer.reg, expA.reg, expB.reg);
    return toInt(answer);
}

intExpression e_minus(intExpression a, intExpression b){
    Expression expA = fromInt(a);
    Expression expB = fromInt(b);
    if(expA.type != expB.type){
        throw std::runtime_error("Exception: tried to subtract a " + expA.type + " and a " + expB.type);
    }
    Expression answer{};
    answer.type = expA.type;
    if(expA.isConstant && expB.isConstant){
        answer.isConstant = true;
        answer.value = expA.value - expB.value;
    }
    mips::sub(answer.reg, expA.reg, expB.reg);
    return toInt(answer);
}

intExpression e_mult(intExpression a, intExpression b){
    Expression expA = fromInt(a);
    Expression expB = fromInt(b);
    if(expA.type != expB.type){
        throw std::runtime_error("Exception: tried to multiply a " + expA.type + " and a " + expB.type);
    }
    Expression answer{};
    answer.type = expA.type;
    if(expA.isConstant && expB.isConstant){
        answer.isConstant = true;
        answer.value = expA.value * expB.value;
    }
    mips::multiply(answer.reg, expA.reg, expB.reg);
    return toInt(answer);
}

intExpression e_div(intExpression a, intExpression b){
    Expression expA = fromInt(a);
    Expression expB = fromInt(b);
    if(expA.type != expB.type){
        throw std::runtime_error("Exception: tried to divide a " + expA.type + " by a " + expB.type);
    }
    Expression answer{};
    answer.type = expA.type;
    if(expA.isConstant && expB.isConstant){
        answer.isConstant = true;
        answer.value = expA.value / expB.value;
    }
    mips::div(expA.reg, expB.reg);
    mips::mflo(answer.reg);
    return toInt(answer);
}

intExpression e_mod(intExpression a, intExpression b){
    Expression expA = fromInt(a);
    Expression expB = fromInt(b);
    if(expA.type != expB.type){
        throw std::runtime_error("Exception: tried to modulo a " + expA.type + " with a " + expB.type);
    }
    Expression answer{};
    answer.type = expA.type;
    if(expA.isConstant && expB.isConstant){
        answer.isConstant = true;
        answer.value = expA.value % expB.value;
    }
    mips::div(expA.reg, expB.reg);
    mips::mfhi(answer.reg);
    return toInt(answer);
}

Expression e_tilda(const Expression& a){
    Expression answer{};
    answer.type = a.type;
    answer.isConstant = false;
    mips::nor(answer.reg, a.reg, a.reg);
    return answer;
}
intExpression e_tilda(intExpression a){
    Expression expA = fromInt(a);
    Expression answer = e_tilda(expA);
    return toInt( answer );
}

Expression e_not(const Expression& a){
    Expression answer{};
    answer.type = "boolean";
    if(a.isConstant){
        answer.value = !a.value;
        answer.isConstant = true;
    }
    mips::slti(answer.reg, a.reg, 1);
    return answer;
}

intExpression e_not(intExpression a){
    Expression expA = fromInt(a);
    Expression answer = e_not(expA);
    return toInt( answer );
}

Expression e_negative(const Expression& a){
    Expression answer{};
    answer.type = "integer";
    if(a.isConstant){
        answer.value = - a.value;
        answer.isConstant = true;
    }
    Register zero("$zero");
    mips::sub(answer.reg, zero, a.reg);
    return answer;
}

intExpression e_negative(intExpression a){
    Expression expA = fromInt(a);
    Expression answer = e_negative(expA);
    return toInt( answer );
}



Expression e_lessthan(const Expression& a, const Expression& b){
    Expression answer{};
    answer.type = "boolean";
    if(a.isConstant && b.isConstant){
        answer.isConstant = true;
        answer.value = a.value < b.value;
    }
    mips::slt(answer.reg, a.reg, b.reg);
    return answer;
}

intExpression e_lessthan(intExpression a, intExpression b){
    Expression expA = fromInt(a);
    Expression expB = fromInt(b);
    Expression answer = e_lessthan(expA, expB);
    return toInt(answer);
}

Expression e_pipe(const Expression& a, const Expression& b){
    Expression answer{};
    answer.type = "boolean";
    if(a.isConstant && b.isConstant){
        answer.isConstant = true;
        answer.value = a.value || b.value;
    }
    Expression x{};
    Expression y{};
    Register zero{"$zero"};
    mips::slt(x.reg, zero, a.reg);
    mips::slt(y.reg, zero, b.reg);
    mips::l_or(answer.reg, x.reg, y.reg);
    return answer;
}

intExpression e_pipe(intExpression a, intExpression b){
    Expression expA = fromInt(a);
    Expression expB = fromInt(b);
    Expression answer = e_pipe(expA, expB);
    return toInt(answer);
}

Expression e_and(const Expression& a, const Expression& b){
    Expression answer{};
    answer.type = "boolean";
    if(a.isConstant && b.isConstant){
        answer.isConstant = true;
        answer.value = a.value && b.value;
    }
    Expression x{};
    Expression y{};
    Register zero{"$zero"};
    mips::slt(x.reg, zero, a.reg);
    mips::slt(y.reg, zero, b.reg);
    mips::b_and(answer.reg, x.reg, y.reg);
    return answer;
}

intExpression e_and(intExpression a, intExpression b){
    Expression expA = fromInt(a);
    Expression expB = fromInt(b);
    Expression answer = e_and(expA, expB);
    return toInt(answer);
}

Expression e_equal(const Expression& expA, const Expression& expB){
    Expression answer = e_and( e_not(e_lessthan(expA, expB)), e_not(e_lessthan(expB, expA)) );
    if(expA.isConstant && expB.isConstant){
        answer.isConstant = true;
        answer.value = (expA.value == expB.value);
    }
    return answer;
}

intExpression e_equal(intExpression a, intExpression b){
    Expression expA = fromInt(a);
    Expression expB = fromInt(b);
    Expression answer = e_equal(expA, expB);
    return toInt(answer);
}

intExpression e_greaterlessthan(intExpression a, intExpression b){
    Expression expA = fromInt(a);
    Expression expB = fromInt(b);
    Expression answer = e_not(e_equal(expA, expB));
    if(expA.isConstant && expB.isConstant){
        answer.isConstant = true;
        answer.value = (expA.value != expB.value);
    }
    return toInt(answer);
}

Expression lessthanequal(const Expression& a, const Expression& b){
    Expression answer = e_pipe( e_lessthan(a, b), e_equal(a, b));
    if(a.isConstant && b.isConstant){
        answer.isConstant = true;
        answer.value = (a.value <= b.value);
    }
    return answer;
}

intExpression lessthanequal(intExpression a, intExpression b){
    Expression expA = fromInt(a);
    Expression expB = fromInt(b);
    Expression answer = lessthanequal(expA, expB);
    return toInt(answer);
}

intExpression e_greaterthan(intExpression a, intExpression b){
    Expression expA = fromInt(a);
    Expression expB = fromInt(b);
    Expression answer = e_lessthan(expB, expA);
    return toInt(answer);
}

intExpression greaterthanequal(intExpression a, intExpression b){
    Expression expA = fromInt(a);
    Expression expB = fromInt(b);
    Expression answer = e_pipe( e_lessthan(expB, expA), e_equal(expA, expB));
    return toInt(answer);
}

intExpression e_chr(intExpression a){
    Expression expA = fromInt(a);
    //if (expA.type != "integer"){
    //    throw std::runtime_error("Called CHR on a non-integer.");
    //}
    expA.type = "character";
    return toInt(expA);
}

intExpression e_ord(intExpression a){
    Expression expA = fromInt(a);
    //if (expA.type != "character"){
    //    throw std::runtime_error("Called ORD on a non-character.");
    //}
    expA.type = "integer";
    return toInt(expA);
}

intExpression e_pred(intExpression a){
    Expression expA = fromInt(a);
    if(expA.isConstant){
        expA.value = expA.value - 1;
    }
    if (expA.type == "integer" || expA.type == "character"){
        mips::addi(expA.reg, expA.reg, -1);
        return toInt(expA);
    }
    else if(expA.type == "boolean"){
        Expression answer = e_not(expA);
        return toInt(answer);
    }
    else{
        throw std::runtime_error("Called PRED on an illegal type.");
    }
}

//intExpression e_pred(std::string id){
//    Symbol symb = symbolTable.lookup(id);
//    Expression exp = Expression();
//    exp.type = symb.type;
//    mips::lw(exp.reg, symb.location);
//    Expression answer = fromInt(e_pred(toInt(exp)));
//    mips::sw(answer.reg, symb.location);
//    return toInt(answer);
//}

intExpression e_succ(intExpression a){
    Expression expA = fromInt(a);
    if(expA.isConstant){
        expA.value = expA.value + 1;
    }
    if (expA.type == "integer" || expA.type == "character"){
        mips::addi(expA.reg, expA.reg, 1);
        return toInt(expA);
    }
    else if(expA.type == "boolean"){
        Expression answer = e_not(expA);
        return toInt(answer);
    }
    else{
        throw std::runtime_error("Called SUCC on an illegal type.");
    }
}

//intExpression e_succ(std::string id){
//    Symbol symb = symbolTable.lookup(id);
//    Expression exp = Expression();
//    exp.type = symb.type;
//    mips::lw(exp.reg, symb.location);
//    Expression answer = fromInt(e_succ(toInt(exp)));
//    mips::sw(answer.reg, symb.location);
//    return toInt(answer);
//}

#endif /* EXPRESSIONS_CPP */
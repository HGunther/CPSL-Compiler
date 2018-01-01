/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: hans
 *
 * Created on January 21, 2017, 8:36 PM
 */

#include <iostream>
#include <string>
#include <fstream>
#include "parser.hpp"
#include "parser.cpp"
#include "scanner.cpp"
//#include "SymbolTable.hpp"
#include <tuple>
#include "RegisterPool.hpp"
#include "Internal/ExpressionType.hpp"

std::ofstream ofile;

/*
 *
 */
int main(int argc, char* argv[])
{
    if ( argc < 2 )
    {
       std::cout << "Please enter the name of a CPSL file to read." << std::endl;
       return 0;
    }

    ofile.open("output.asm");
    yyin = fopen( argv[1], "r" );
     
    try{
        FRONTEND_H::Precompile();
        yyparse();
    }
    catch (std::exception& e){
        std::cout << "exception caught: " << e.what() << '\n';
    }
    catch(...){
        std::cout << "Unhandled exception has caused the program to crash." << std::endl;
    }
     
    ofile.close();
    return 0;
}

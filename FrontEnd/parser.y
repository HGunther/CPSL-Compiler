%{
#include <iostream>
    #include <string>
#include "FrontEnd/FrontEnd.cpp"
#include "Internal/Expressions.cpp"
#include "Internal/ControlFlow.cpp"
#include "Internal/Functions.cpp"
#include "Internal/UserDefinedDataTypes.cpp"
#include "SymbolTable.hpp"

  
extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern int linenumber;

void yyerror(const char *s);
%}

%union {
	int ival;
	float fval;
	char* sval;
        int* ptr;
}

%token <ival> INTVAL
%token <fval> FLOATVAL
%token <sval> STRINGVAL
%token <sval> CHARVAL

%token K_ARRAY
%token K_BEGIN
%token K_CHR
%token K_CONST
%token K_DO
%token K_DOWNTO
%token K_ELSE
%token K_ELSEIF
%token K_END
%token K_FOR
%token K_FORWARD
%token K_FUNCTION
%token K_IF
%token K_OF
%token K_ORD
%token K_PRED
%token K_PROCEDURE
%token K_READ
%token K_RECORD
%token K_REF
%token K_REPEAT
%token K_RETURN
%token K_STOP
%token K_SUCC
%token K_THEN
%token K_TO
%token K_TYPE
%token K_UNTIL
%token K_VAR
%token K_WHILE
%token K_WRITE

%token GREATERLESSTHAN
%token LESSTHANEQUAL
%token GREATERTHANEQUAL
%token COLONEQUAL
%token PLUS
%token MINUS
%token MULT
%token DIV
%token AND
%token PIPE
%token TILDA
%token EQUAL
%token LESSTHAN
%token GREATERTHAN
%token DOT
%token COMMA
%token COLON
%token SEMICOLON
%token OPENPAREN
%token CLOSEPAREN
%token OPENSQUARE
%token CLOSESQUARE
%token MOD

%token IDENT
%type <sval> IDENT

%type <ptr> IdentList
%type <sval> Type
%type <sval> SimpleType
%type <sval> RecordType
%type <sval> ArrayType
%type <ival> Expression
%type <ival> Term
%type <ival> Factor
%type <ival> RValue
%type <ival> LValue
%type <ptr> ExpressionList
%type <ptr> LValueList
%type <ival> IF_Opening
%type <ival> ToDownto

%%
Program :
	ConstantDecl TypeDecl VarDecl ProcFuncDecl Block Tail
	{
            FRONTEND_H::programEnd();
            std::clog << "Finished parsing the program" << std::endl;
	}
;

Tail : 
        DOT         {}
        | SEMICOLON {}
        |           {}
;

ConstantDecl :
	K_CONST ConstDeclList {/*std::clog << "Finished Constant Declarations" << std::endl;*/}
        | {std::clog << "No Constant Declarations" << std::endl;}
;
ConstDeclList :
        IDENT EQUAL Expression SEMICOLON ConstDeclList  { FRONTEND_H::ConstDeclaration($1, $3); }
	|                                           {std::clog << "End of Constant Declarations" << std::endl;}
;

ProcFuncDecl :
	ProcedureDecl ProcFuncDecl  {}
	| FunctionDecl ProcFuncDecl {}
	|                           {std::clog << "Finished Procedure/Function Declarations" << std::endl; FRONTEND_H::programStart();}
;

ProcedureDecl :
	K_PROCEDURE IDENT OPENPAREN ParameterList CLOSEPAREN SEMICOLON K_FORWARD SEMICOLON {}
	| K_PROCEDURE IDENT OPENPAREN ParameterList CLOSEPAREN SEMICOLON Body SEMICOLON {}
;

FunctionDecl :
	K_FUNCTION IDENT OPENPAREN ParameterList CLOSEPAREN COLON Type SEMICOLON K_FORWARD SEMICOLON {}
	| K_FUNCTION IDENT OPENPAREN ParameterList CLOSEPAREN COLON Type SEMICOLON Body SEMICOLON {}
;

ParameterList:
	IdentList COLON Type SEMICOLON ParameterList {}
	| K_VAR IdentList COLON Type SEMICOLON ParameterList {}
	| K_REF IdentList COLON Type SEMICOLON ParameterList {}
	| IdentList COLON Type
	| K_VAR IdentList COLON Type {}
	| K_REF IdentList COLON Type
	| {}
;

Body:
	ConstantDecl TypeDecl VarDecl Block {}
	| ConstantDecl TypeDecl Block {}
	| ConstantDecl VarDecl Block {}
	| ConstantDecl Block {}
	| TypeDecl VarDecl Block {}
	| TypeDecl Block {}
	| VarDecl Block {}
	| Block {}
;

Block :
	K_BEGIN StatementSequence K_END {std::clog << "Finished program body" << std::endl;}
;

TypeDecl :
	K_TYPE TypeList {std::clog << "Finished Type Declarations" << std::endl;}
	| {std::clog << "No Type Declarations" << std::endl;}
;
TypeList :
	IDENT EQUAL Type SEMICOLON TypeList {typeDeclaration($1, $3);}
	| {}
;

Type :
	SimpleType {$$ = $1;}
	| RecordType {$$ = $1;}
	| ArrayType {$$ = $1;}
;

SimpleType : IDENT {std::clog << "Matched Type: " << $1 << std::endl; $$ = $1;};
RecordType : K_RECORD RecordList K_END {$$ = declareRec();};
RecordList : IdentList COLON Type SEMICOLON {};
ArrayType : K_ARRAY OPENSQUARE Expression COLON Expression CLOSESQUARE K_OF Type {$$ = declareArray($3,$5,$8); };

IdentList :
	IDENT COMMA IdentList   {$$ = identListAppend($1, $3);}
	| IDENT                 {$$ = identListInit($1); }
;

        
VarDecl :
        K_VAR VarDeclList   {std::clog << "Finished Variable Declarations" << std::endl;}
        |                   {std::clog << "No Variable Declarations" << std::endl;}
;

VarDeclList :
        IdentList COLON Type SEMICOLON VarDeclList  { FRONTEND_H::variableDeclarationList($1, $3); }
	|                                           {std::clog << "End of Variable Declarations" << std::endl;}
;



StatementSequence :
	Statement SEMICOLON StatementSequence {}
	| Statement {}
;

Statement :
	Assignment {}
	| IfStatement {}
	| WhileStatement {}
	| RepeatStatement {}
	| ForStatement {}
	| StopStatement {}
	| ReturnStatement {}
	| ReadStatement {}
	| WriteStatement {}
	| ProcedureCall {}
	| NullStatement {}
;

Assignment : LValue COLONEQUAL Expression { FRONTEND_H::assign($1, $3); };


IfStatement :
	IF_Opening StatementSequence ElseIfList ElseList K_END {ifClosing($1);}
;
IF_Opening : K_IF Expression K_THEN   {$$ = ifOpening($2);}
;
ElseIfList :
        elseiflist_later StatementSequence ElseIfList {}
	| {}
;
elseiflist_opening: K_ELSEIF    {elseiflist_start();}
;
elseiflist_later: elseiflist_opening Expression K_THEN {elseiflist_later($2);}
;
ElseList :
	else_statement StatementSequence {}
	| {}
;
else_statement: K_ELSE {elselist();}


WhileStatement : whileBody StatementSequence K_END {whileEnd();};
whileInit : K_WHILE     {whileInit();};
whileBody : whileInit Expression K_DO       {whileBody($2);};


RepeatStatement : repeatInit StatementSequence K_UNTIL Expression {repeatEnd($4);};
repeatInit : K_REPEAT {repeatInit();}


ForStatement : forInit StatementSequence K_END {forEnd();};
forInit : K_FOR IDENT COLONEQUAL Expression ToDownto Expression K_DO {forInit($2, $4, $5, $6);};
ToDownto :
	K_TO {$$ = 1;}
	| K_DOWNTO {$$ = 0;}
;


StopStatement : K_STOP {stop();};


ReturnStatement :
	K_RETURN Expression {}
	| K_RETURN {}
;

ReadStatement :
        K_READ OPENPAREN LValue CLOSEPAREN {FRONTEND_H::read($3); }
        | K_READ OPENPAREN LValueList CLOSEPAREN {FRONTEND_H::readlist($3);}
;
LValueList :
	LValue COMMA LValueList {$$ = FRONTEND_H::lvalueListAppend($1, $3);}
        | LValue LValueList {$$ = FRONTEND_H::lvalueListAppend($1, $2);}
	|                               {$$ = FRONTEND_H::lvalueListInit();}
;

WriteStatement :
        K_WRITE OPENPAREN Expression CLOSEPAREN {FRONTEND_H::write($3); }
        | K_WRITE OPENPAREN ExpressionList CLOSEPAREN {FRONTEND_H::writelist($3);}
;
        
ExpressionList :
	Expression COMMA ExpressionList {$$ = FRONTEND_H::expressionListAppend($1, $3);}
        | Expression ExpressionList {$$ = FRONTEND_H::expressionListAppend($1, $2);}
	|                               {$$ = FRONTEND_H::expressionListInit();}
;

ProcedureCall :
	IDENT OPENPAREN ExpressionList CLOSEPAREN {}
	| IDENT OPENPAREN CLOSEPAREN {}
;

NullStatement : {};

Expression :
        Expression PLUS Term {$$ = e_plus($1, $3);}
	| Expression MINUS Term {$$ = e_minus($1, $3);}
	| Expression PIPE Expression {$$ = e_pipe($1, $3);}
	| Expression AND Expression {$$ = e_and($1, $3);}
	| Expression EQUAL Expression {$$ = e_equal($1, $3);}
	| Expression GREATERLESSTHAN Expression {$$ = e_greaterlessthan($1, $3);}
	| Expression LESSTHANEQUAL Expression {$$ = lessthanequal($1, $3);}
	| Expression GREATERTHANEQUAL Expression {$$ = greaterthanequal($1, $3);}
	| Expression LESSTHAN Expression {$$ = e_lessthan($1, $3);}
	| Expression GREATERTHAN Expression {$$ = e_greaterthan($1, $3);}
	| IDENT OPENPAREN ExpressionList CLOSEPAREN {}
	| K_CHR OPENPAREN Expression CLOSEPAREN {$$ = e_chr($3);}
	| K_ORD OPENPAREN Expression CLOSEPAREN {$$ = e_ord($3);}
	| K_PRED OPENPAREN Expression CLOSEPAREN {$$ = e_pred($3);}
	| K_SUCC OPENPAREN Expression CLOSEPAREN {$$ = e_succ($3);}
        | Term {$$ = $1;}	
;
Term :
        Term MULT Factor {$$ = e_mult($1, $3);}
	| Term DIV Factor {$$ = e_div($1, $3);}
	| Term MOD Factor {$$ = e_mod($1, $3);}
        | TILDA Term {$$ = e_tilda($2);}
	| MINUS Term {$$ = e_negative($2);}
        | Factor {$$ = $1;}
;
Factor :
        OPENPAREN Expression CLOSEPAREN {$$ = $2;}
        | RValue {$$ = $1;}
	| FLOATVAL {}
	| INTVAL {$$ = intval($1);}
	| STRINGVAL {$$ = stringval($1);}
        | CHARVAL {$$ = charval($1);}
;
RValue : LValue {$$ = FRONTEND_H::loadValue($1);};
        
LValue : 
        LValue DOT IDENT {$$ = loadRecElement($1, $3);}
        | LValue OPENSQUARE Expression CLOSESQUARE {$$ = loadArrayElement($1, $3);}
        | IDENT {$$ = loadAddress($1);}
;

%%

void yyerror(const char* msg)
{
	std::clog << "Encountered an error on line number " << linenumber << ": " << msg << std::endl;
        //std::cout << "Encountered an error on line number " << linenumber << ": " << msg << std::endl;
}

/*
Hans Gunther
A01077132
*/

%{

#include <math.h>
#include <string.h>
#include "parser.hpp"

int linenumber = 1;

%}

CHAR	 [[:alpha:]]
DIGIT    [[:digit:]]
ID       [[:alpha:]][[:alnum:]_]*

%%

$.*$		    { printf( "Found comment: %s\n", yytext ); }

array|ARRAY	    { printf( "Found keyword: ARRAY\n" ); return K_ARRAY;}
begin|BEGIN	    { printf( "Found keyword: BEGIN\n" ); return K_BEGIN;}
chr|CHR		    { printf( "Found keyword: CHR\n" ); return K_CHR;}
const|CONST	    { printf( "Found keyword: CONST\n" ); return K_CONST;}
do|DO		    { printf( "Found keyword: DO\n" ); return K_DO;}
downto|DOWNTO	    { printf( "Found keyword: DOWNTO\n" ); return K_DOWNTO;}
else|ELSE	    { printf( "Found keyword: ELSE\n" ); return K_ELSE;}
elseif|ELSEIF	    { printf( "Found keyword: ELSEIF\n" ); return K_ELSEIF;}
end|END		    { printf( "Found keyword: END\n" ); return K_END;}
for|FOR		    { printf( "Found keyword: FOR\n" ); return K_FOR;}
forward|FORWARD	    { printf( "Found keyword: FORWARD\n" ); return K_FORWARD;}
function|FUNCTION   { printf( "Found keyword: FUNCTION\n" ); return K_FUNCTION;}
if|IF		    { printf( "Found keyword: IF\n" ); return K_IF;}
of|OF		    { printf( "Found keyword: OF\n" ); return K_OF;}
ord|ORD		    { printf( "Found keyword: ORD\n" ); return K_ORD;}
pred|PRED	    { printf( "Found keyword: PRED\n" ); return K_PRED;}
procedure|PROCEDURE { printf( "Found keyword: PROCEDURE\n" ); return K_PROCEDURE;}
read|READ	    { printf( "Found keyword: READ\n" ); return K_READ;}
record|RECORD	    { printf( "Found keyword: RECORD\n" ); return K_RECORD;}
ref|REF		    { printf( "Found keyword: REF\n" ); return K_REF;}
repeat|REPEAT	    { printf( "Found keyword: REPEAT\n" ); return K_REPEAT;}
return|RETURN	    { printf( "Found keyword: RETURN\n" ); return K_RETURN;}
stop|STOP	    { printf( "Found keyword: STOP\n" ); return K_STOP;}
succ|SUCC	    { printf( "Found keyword: SUCC\n" ); return K_SUCC;}
then|THEN	    { printf( "Found keyword: THEN\n" ); return K_THEN;}
to|TO		    { printf( "Found keyword: TO\n" ); return K_TO;}
type|TYPE	    { printf( "Found keyword: TYPE\n" ); return K_TYPE;}
until|UNTIL	    { printf( "Found keyword: UNTIL\n" ); return K_UNTIL;}
var|VAR		    { printf( "Found keyword: VAR\n" ); return K_VAR;}
while|WHILE	    { printf( "Found keyword: WHILE\n" ); return K_WHILE;}
write|WRITE	    { printf( "Found keyword: WRITE\n" ); return K_WRITE;}


0x{DIGIT}+	    {
  printf( "Found hex integer: %s (Decimal %ld)\n", yytext, strtol( yytext, NULL, 16 ) );
  yylval.ival = strtol( yytext, NULL, 16 );
  return INTVAL;
}
0{DIGIT}+	    {
  printf( "Found octal integer: %s (Decimal %ld)\n", yytext, strtol( yytext, NULL, 8 ) );
  yylval.ival = strtol( yytext, NULL, 8 );
  return INTVAL;
}
{DIGIT}+    	    {
  printf( "Found decimal integer: %s (Decimal %d)\n", yytext, atoi( yytext ) );
  yylval.ival = atoi( yytext );
  return INTVAL;
}

0x{DIGIT}+"."{DIGIT}*  {
  printf( "Found hex float: %s (Decimal %g)\n", yytext, strtod( yytext, NULL) );
  yylval.fval = strtod( yytext, NULL);
  return FLOATVAL;
}
0{DIGIT}+"."{DIGIT}*   {
  printf( "Found octal float: %s (Decimal %g)\n", yytext, strtod( yytext, NULL ) );
  yylval.fval = strtod( yytext, NULL);
  return FLOATVAL;
}
{DIGIT}+"."{DIGIT}*   {
  printf( "Found decimal float: %s (Decimal %g)\n", yytext, atof( yytext ) );
  yylval.fval = atof( yytext );
  return FLOATVAL;
}


''	      { printf( "ERROR: character literal may not be empty\n" ); }
'.'	      {
  printf( "Found character: %s\n", yytext);
  yylval.sval = strdup(yytext);
  return CHARVAL;
}

"\\n"       {
  printf( "Found character: \\n\n");
  char c[5] = {'"', '\\', 'n', '"', '\0'};
  yylval.sval = c;
  return STRINGVAL;
}
'\\n'       {
  printf( "Found character: \\n\n");
  char c[5] = {'"', '\\', 'n', '"', '\0'};
  yylval.sval = c;
  return STRINGVAL;
}

'[^']{2,}'    { printf( "ERROR: character literal may not contain more than one character\n" ); }
\"[^\"\n]*\"		      {
  printf( "Found string: %s\n", yytext);
  yylval.sval = strdup(yytext);
  return STRINGVAL;
}
\"			      { printf( "ERROR: Found an odd number of quotation marks on a line.\n" ); }


{ID}        	      {
  printf( "Found identifier: %s\n", yytext );
  yylval.sval = strdup(yytext);
  return IDENT;
}


"<>" { printf( "Found symbol: <>\n"); return GREATERLESSTHAN;}
"<=" { printf( "Found symbol: <=\n"); return LESSTHANEQUAL;}
">=" { printf( "Found symbol: >=\n"); return GREATERTHANEQUAL;}
":=" { printf( "Found symbol: :=\n"); return COLONEQUAL;}
"+" { printf( "Found symbol: +\n"); return PLUS;}
"-" { printf( "Found symbol: -\n"); return MINUS;}
"*" { printf( "Found symbol: *\n"); return MULT;}
"/" { printf( "Found symbol: /\n"); return DIV;}
"&" { printf( "Found symbol: &\n"); return AND;}
"|" { printf( "Found symbol: |\n"); return PIPE;}
"~" { printf( "Found symbol: ~\n"); return TILDA;}
"=" { printf( "Found symbol: =\n"); return EQUAL;}
"<" { printf( "Found symbol: <\n"); return LESSTHAN;}
">" { printf( "Found symbol: >\n"); return GREATERTHAN;}
"." { printf( "Found symbol: .\n"); return DOT;}
"," { printf( "Found symbol: ,\n"); return COMMA;}
":" { printf( "Found symbol: :\n"); return COLON;}
";" { printf( "Found symbol: ;\n"); return SEMICOLON;}
"(" { printf( "Found symbol: (\n"); return OPENPAREN;}
")" { printf( "Found symbol: )\n"); return CLOSEPAREN;}
"[" { printf( "Found symbol: [\n"); return OPENSQUARE;}
"]" { printf( "Found symbol: ]\n"); return CLOSESQUARE;}
"%" { printf( "Found symbol: MODULO\n"); return MOD;}

\n  {linenumber += 1;}
\r  {}
[ \t]+      {}
.           	  { printf( "ERROR: Unrecognized character: %s\n", yytext ); }

%%

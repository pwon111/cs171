%{
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "data.h"
#include "graphicsparser.tab.h"
using namespace std;
int reading_indices = 0;

/* %x declares "states" 
http://aquamentus.com/tut_lexyacc.html
Advanced
start states
*/

/* //failed regex
-?([0-9]*\.[0-9]+|[0-9]+\.?)(e[+-][0-9]+)?  {
[-+]?[0-9]*\.?[0-9]+
*/

%}
%option noyywrap
%x COMMENT
%x MULTICOMMENT
%x SCOMMENT
%%
<INITIAL>#   					    {BEGIN(COMMENT);}
<COMMENT>.
<COMMENT>\n 			            {BEGIN(INITIAL);}
<INITIAL>"/*"                       {BEGIN(MULTICOMMENT);}
<MULTICOMMENT>.
<MULTICOMMENT>"*/"                  {BEGIN(INITIAL);}
<INITIAL>"//"                       {BEGIN(SCOMMENT);}
<SCOMMENT>.
<SCOMMENT>\n                  {BEGIN(INITIAL);}


v                   return TOKV;
f                   reading_indices = 1; return TOKF; 

[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?                        if(reading_indices==0){
                                              yylval.y_float = atof(yytext);
                                              return TOKFLOAT;
                                            }else{
                                    yylval.y_int = atoi(yytext); return TOKINT;
                                            };

([0-9]*)                yylval.y_int = atoi(yytext); return TOKINT;
 


\n                      /* ignore end of line */;
[ \t]+                  /* ignore whitespace */;


%%








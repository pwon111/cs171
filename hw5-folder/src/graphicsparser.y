%{ /*http://ds9a.nl/lex-yacc/cvs/lex-yacc-howto.html*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <list>
#include <vector>
#include "data.h"
#include <stdint.h>
#include <FlexLexer.h>
#include <iostream>
    
using namespace std;
    
extern int yylex();
    
void yyerror(const char *str)
{
    fprintf(stderr,"error: %s\n",str);
}

extern "C"
{
    int yyparse(void);
    //int yylex(void);
}

static Mesh * results;
static vector<face> * Faces;
static vector<point> * Points;

%}

%union
{
    int y_int;
    float y_float;
    vector<face*> * y_vector_face_P;
    vector<point*> * y_vector_point_P;
    face * y_face_P;
    point * y_point_P;
}
%defines

%type <y_vector_face_P> faces;
%type <y_face_P> p_face;
%type<y_vector_point_P> points;
%type<y_point_P> p_point;

%token TOKV TOKF
%token <y_int> TOKINT
%token <y_float> TOKFLOAT

%%
file: points faces
{
    results->points = $1;
    results->faces = $2;
};

points:
{
    $$ = new vector<point*>;
}
| points p_point
{
    $1->push_back($2);
    $$ = $1;
};

faces:
{
    $$ = new vector<face*>;
}
| faces p_face
{
    $1->push_back($2);
    $$ = $1;
};

p_point: TOKV TOKFLOAT TOKFLOAT TOKFLOAT
{
    point * P = new point;
    P->x=$2;
    P->y=$3;
    P->z=$4;
    $$=P;
};

p_face: TOKF TOKINT TOKINT TOKINT
{
    face * F = new face;
    F->ind1=$2;
    F->ind2=$3;
    F->ind3=$4;
    $$=F;
};

%%

//static FlexLexer *lexer;
/*int yylex()
{
    return lexer->yylex();
}*/

void parse(Mesh * A, istream &datafile)
{
    results=A;
    
    // Construct the lexer.
    //lexer = new yyFlexLexer(&datafile);
    // Run the parser.
    if(yyparse())
    {
        cerr << "Parse failed!" << endl;
        exit(1);
    }
    //delete lexer;
}

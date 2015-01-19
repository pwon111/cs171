/* CS/CNS 171 Fall 2014
 *
 * This is the lexer part of the parser.
 *
 * We recommend that you avoid editing this file unless you really know your
 * way around the flex/bison syntax and feel a compelling need to change our
 * code. Keep in mind that we will be giving you updated versions of this
 * lexer in the later assignments, so any changes you make here will have to
 * be made again to the later versions that we give you.
 *
 * This lexer is mostly self-explanatory. Most of our responses to lines in
 * our input Open Inventor file just involve returning appropriate token
 * representations of the lines.
 */

%{
#include <iostream>
#include <stdlib.h>
#include "openivparser.tab.hpp"

using namespace std;

/* The following variable will be used as a boolean to indicate whether the
 * integers that the parser reads are supposed to be treated as indices, in
 * which case we want to return an INT token, or as a number for an object
 * parameter (e.g. coordinate of a point, component of vector, etc), in
 * which case we want to return a FLOAT token. A value of 0 means that we
 * are not reading indices. */
int reading_indices = 0;
%}

/* The following line tells our parser to only parse one file (our input
 * Open Inventor file) once. */
%option noyywrap
/* Since flex will always generate a file called "lex.yy.c" by default,
 * we set a prefix to differentiate the lex file this will generate from
 * the other, potential lex files in the same directory. The prefix replaces
 * the "yy" for every associated file, variable, and function.
 * e.g. lex.yy.c will be lex.openiv.c and yylval will be openivlval. */
%option prefix="openiv"

%%

0|-?[1-9]+[0-9]*    { /* Reads any integer number in the file. */
                      if(reading_indices) /* We want to return an INT token. */
                      {
                            /* Convert the read number to an integer. */
                            openivlval.ival = atoi(openivtext);
                            /* The negative ones in the file are not actual
                             * indices and instead mark the end of a faceset.
                             * We need to return a different token for the
                             * them so that our parser will respond appropriately
                             * and not mistake a -1 for an index. */
                            return (openivlval.ival != -1) ? INT : TERMINATOR;
                      }
                      else /* We want to return a FLOAT token. */
                      {
                            /* Convert the read number to a float. */
                            openivlval.fval = atof(openivtext);
                            return FLOAT;
                      }
                    };

-?([0-9]*\.[0-9]+|[0-9]+\.?)(e[+-][0-9]+)?  { /* This line reads any decimal or number
                                               * written in scientific notation. */
                                              openivlval.fval = atof(openivtext);
                                              return FLOAT;
                                            };

\{                  return OPEN_BRACE;
\}                  { /* The times when we are reading in indices are when we
                       * are in an IndexedFaceSet block. When we reach such a
                       * block, we set our 'reading_indices' indicator variable
                       * to 1 for true. After the block ends (marked by the
                       * closing brace), we want to switch back to reading in
                       * floats. To do so, we switch our indicator variable
                       * back to 0 for false. */
                      if(reading_indices) { reading_indices = 0; }
                      return CLOSE_BRACE;
                    };
\[                  return OPEN_BRACKET;
\]                  return CLOSE_BRACKET;

,                   return COMMA;

PerspectiveCamera   return CAMERA;
position            return CAM_POSITION;
orientation         return CAM_ORIENTATION;
nearDistance        return CAM_NEAR;
farDistance         return CAM_FAR;
left                return CAM_LEFT;
right               return CAM_RIGHT;
top                 return CAM_TOP;
bottom              return CAM_BOTTOM;

PointLight          return LIGHT;
location            return LIGHT_POSITION;
color               return LIGHT_COLOR;

Separator           return SEPARATOR;

Transform           return TRANSFORM;
translation         return TRANSLATE;
rotation            return ROTATE;
scaleFactor         return SCALE;

Material            return MATERIAL;
ambientColor        return AMBIENT;
diffuseColor        return DIFFUSE;
specularColor       return SPECULAR;
shininess           return SHININESS;

Coordinate3         return COORDINATE;
point               return POINT;

Normal              return NORMAL;
vector              return VECTOR;

IndexedFaceSet      { /* All numbers that we read in from an IndexedFaceSet block
                       * should be indices, hence we set our indicator variable
                       * to 1 here. It will be set back to 0 when we exit the
                       * block. */
                      reading_indices = 1;
                      return FACESET;
                    };
coordIndex          return COORDINATE_INDEX;
normalIndex         return NORMAL_INDEX;

#.*\n               /* This line reads comments from the file. We ignore these. */
[ \t\r\n]+          /* This line reads whitespace from the file. We ignore these. */
.                   { /* The period can match any character. This line serves to
                       * read any character sequences that doesn't match the above
                       * lines, in which case, we throw an unexpected character
                       * error. */
                      cerr << "Unexpected character: " << openivtext << endl;
                      return 0;
                    }

%%

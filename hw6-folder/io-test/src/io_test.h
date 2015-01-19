// include guard
#ifndef OGL_RENDERER_H
#define OGL_RENDERER_H

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <array>
#include <map>
#include <math.h>
#define _USE_MATH_DEFINES

// include OpenGL
#include <GL/glew.h>
#include <GL/glut.h>

// include Eigen
#include <Eigen/Eigen>

#include "util.h"

using namespace std;
using namespace Eigen;

/* This function initializes all of the basic required OpenGLsettings along with
 * setting up the projection matrix
 * fill_vertex_normal_buffer is also called in this function
 */
void init(void);

/* This function defines how the display responds when the window is resized
 *
 * This function takes two arguments:
 *      - width, height - new width and height to adjust to (int)
 */
void reshape(int width, int height);

/* This function defines how each frame is rendered */
void display(void);

/* This function draws each object in objects */
void draw_objects();

/* This function defines how OpenGL responds on mouse click
 *
 * This function takes four arguments:
 *      - button - the button on the mouse that got clicked or released,
 *               represented by an enum
 *      - state - either 'GLUT_DOWN' or 'GLUT_UP' for specifying whether the
 *              button was pressed down or released up respectively
 *      - x, y - screen coordinates (int)
 */
void mouse_pressed(int button, int state, int x, int y);

/* This function defines how OpenGL responds on mouse movement
 * 
 * This function takes two arguments:
 *      - x, y - screen coordinates (int)
 */
void mouse_moved(int x, int y);

/* This function defines how OpenGL responds on key press
 *
 * This function takes three arguments:
 *      - key - the character of the key itself or the ASCII value of the key
 *      - x, y - screen coordinates (int)
 */
void key_pressed(unsigned char key, int x, int y);

/* This function returns the vector required for arcball rotation calculation
 *
 * This function takes two arguments:
 *      - x, y - screen coordinates (int)
 */
Vector3f get_arcball_vector(int x, int y);

/* This function takes a 3D world coordinate and checks if it is inside a
 * globally stored superquadric */
float isq(float x, float y, float z);

/* This function turns a superquadric into a parametric function in terms of u
 * and v and returns the point at a specific u and v values */
Vector4f psq(float u, float v);

#endif
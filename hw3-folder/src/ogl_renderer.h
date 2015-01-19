// include guard
#ifndef OGL_RENDERER_H
#define OGL_RENDERER_H

#include <iostream>
#include <vector>

#include <math.h>
#define _USE_MATH_DEFINES

// include OpenGL
#include <GL/glew.h>
#include <GL/glut.h>

// include Eigen
#include <Eigen/Eigen>

#include "ivdata.h"
#include "util.h"

using namespace std;
using namespace Eigen;

// function for parsing an iv datafile
IVData parse_iv(istream &datafile);

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

/* This function sets up all of the Light instances in lights using the built in
 * OpenGL lights. However, the positions of the lights are NOT set in this
 * function
 */
void init_lights();

/* This function positions all of the lights */
void set_lights();

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

/* This function fills in vertex and normal buffers in objects
 *
 * This function takes one argument:
 *      - objects - reference to the vector of Objects
 */
void fill_vertex_normal_buffer(vector<Object> &objects);

/* This function returns the vector required for arcball rotation calculation
 *
 * This function takes two arguments:
 *      - x, y - screen coordinates (int)
 */
Vector3f get_arcball_vector(int x, int y);

/* This function converts given angle in degrees to radians. */
float deg2rad(float angle);

/* This function converts given angle in radians to degrees. */
float rad2deg(float angle);

#endif
/* CS/CNS 171 Fall 2014
 *
 * This header file contains some useful utility functions.
 */

// include guard
#ifndef UTIL_H
#define UTIL_H

#include <Eigen/Eigen>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cfloat>
#include <vector>
#include <math.h>
#define _USE_MATH_DEFINES

using namespace Eigen;
using namespace std;

// this struct holds information on a vertual camera
struct Camera
{
	// position
	float position[3];
	// orientation
	float orientation_axis[3];
	float orientation_angle;
	// render frutsum
	float near;
	float far;
	float left;
	float right;
	float top;
	float bottom;
	// xres and yres
	int xres;
	int yres;
};

// this struct holds information on quadratic equations in the form of
// ax^2 + bx + c = 0
struct Quadratic
{
    float a;
    float b;
    float c;
};

/* Gets the translation matrix for a given translation vector (x, y, z). */
Matrix4f get_translate_mat(float x, float y, float z);
/* Gets the rotation matrix for a given rotation axis (x, y, z) and angle. */
Matrix4f get_rotate_mat(float x, float y, float z, float angle);
/* Gets the scaling matrix for a given scaling vector (x, y, z). */
Matrix4f get_scale_mat(float x, float y, float z);
/* Gets the perspective projection matrix given the frustum parameters. */
Matrix4f get_perspective_mat(float near, float far,
                             float left, float right,
                             float top, float bottom);

/* This function converts given angle in degrees to radians. */
float deg2rad(float angle);

/* This function converts given angle in radians to degrees. */
float rad2deg(float angle);

// helper functions for parameterizing superquadrics
float exp_cos(float theta, float exp);
float exp_sin(float theta, float exp);

// returns the sign of a number
// -1 for negative, 1 for positive, 0 for 0
float sign(float num);

// conversion between Vertext3f and Vertex4f
Vector3f Vector4fTo3f(Vector4f v);
Vector4f Vector3fTo4f(Vector3f v);

// this function returns the smaller root of a quadratic equation
float small_ans(Quadratic q);

/* this function takes three different color pixel grids and outputs them into
 * a .ppm file
 *
 * this function takes four arguments:
 *      - red, blue, green - pixel grids
 *      - filename - name of the output file
 */
void ppm_render(MatrixXd red, MatrixXd blue, MatrixXd green, string filename);

/* this function takes three different color pixel grids and outputs them into
 * standard output
 *
 * this function takes one argument:
 *      - red, blue, green - pixel grids
 */
void std_out_render(MatrixXd red, MatrixXd blue, MatrixXd green, int MAX_BRT);

#endif
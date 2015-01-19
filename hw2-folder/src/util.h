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
#include <vector>
#include <math.h>

using namespace Eigen;
using namespace std;

/* Gets the translation matrix for a given translation vector (x, y, z). */
MatrixXd get_translate_mat(float x, float y, float z);
/* Gets the rotation matrix for a given rotation axis (x, y, z) and angle. */
MatrixXd get_rotate_mat(float x, float y, float z, float angle);
/* Gets the scaling matrix for a given scaling vector (x, y, z). */
MatrixXd get_scale_mat(float x, float y, float z);
/* Gets the perspective projection matrix given the frustum parameters. */
MatrixXd get_perspective_mat(float near, float far,
                             float left, float right,
                             float top, float bottom);

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
void std_out_render(MatrixXd red, MatrixXd blue, MatrixXd green);

/* this function swaps the values of two int values */
void swap(int &a, int &b);

/* this function returns the sign of a float
 * i.o.w. it returns 0 if it's 0, 1 if positive, -1 if negative
 */
int sign(float a);

/* this function truncates an integer to a set max value */
int truncate(int a, int b);

/* this function returns the maximum value among three given float values */
float max(float a, float b, float c);

/* this function returns the minimum value among three given float values */
float min(float a, float b, float c);

#endif
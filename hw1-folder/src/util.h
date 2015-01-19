/* CS/CNS 171 Fall 2014
 *
 * This header file contains some useful utility functions.
 */

#pragma once

#include <Eigen/Eigen>
#include <iostream>
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

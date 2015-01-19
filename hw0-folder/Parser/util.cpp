/* CS/CNS 171 Fall 2014
 *
 * This file contains the actual implementations of the functions in util.h.
 * Right now, each function simply returns the identity matrix. You will
 * fill these functions in with the proper implementations in Assignment 1.
 */

#include "util.h"

/* Gets the translation matrix for a given translation vector (x, y, z). */
MatrixXd get_translate_mat(float x, float y, float z)
{
	MatrixXd translation_matrix = MatrixXd::Identity(4,4);
	translation_matrix(0, 3) = x;
	translation_matrix(1, 3) = y;
	translation_matrix(2, 3) = z;
	return translation_matrix;
}

/* Gets the rotation matrix for a given rotation axis (x, y, z) and angle. */
MatrixXd get_rotate_mat(float x, float y, float z, float angle)
{
	MatrixXd rotation_matrix(4, 4);
					// first row
	rotation_matrix << x * x + (1 - x * x) * cos(angle),
					x * y * (1 - cos(angle)) - z * sin(angle),
					x * z * (1 - cos(angle)) + y * sin(angle),
					0,
					// second row
					y * x * (1 - cos(angle)) + z * sin(angle),
					y * y + (1 - y * y) * cos(angle),
					y * z * (1 - cos(angle)) - x * sin(angle),
					0,
					// third row
					z * x * (1 - cos(angle)) - y * sin(angle),
					z * y * (1 - cos(angle)) + x * sin(angle),
					z * z + (1 - z * z) * cos(angle), 0,
					// fourth row
					0, 0, 0, 1; 
	return rotation_matrix;
}

/* Gets the scaling matrix for a given scaling vector (x, y, z). */
MatrixXd get_scale_mat(float x, float y, float z)
{
	MatrixXd scaling_matrix = MatrixXd::Identity(4,4);
	scaling_matrix(0, 0) = x;
	scaling_matrix(1, 1) = y;
	scaling_matrix(2, 2) = z;
	return scaling_matrix;
}

/* Gets the perspective projection matrix given the frustum parameters. */
MatrixXd get_perspective_mat(float near, float far,
							float left, float right,
							float top, float bottom)
{
	MatrixXd perspective_matrix(4, 4);
						// first row
	perspective_matrix << 2 * near / (right - left),
						0,
						(right + left) / (right - left),
						0,
						// second row
						0,
						2 * near / (top - bottom),
						(top + bottom) / (top - bottom),
						0,
						// third row
						0,
						0,
						(far + near) / (near - far),
						2 * far * near / (near - far),
						// fourth row
						0, 0, -1, 0;
	return perspective_matrix;
}

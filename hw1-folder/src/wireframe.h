// include guard
#ifndef WIREFRAME_H
#define WIREFRAME_H

/* CS 171 Assignment by JongSeok (Parker) Won
 * 
 * this program outputs an image of a wireframe of an object in .ppm format
 * the user can determine what resoultion the image is rendered in and choose
 * which datafile to read the information on the wireframe from
 * additionally, the user can choose between two modes: mode 0 means use
 * Bresenham's line algorithm (no anti-aliasing) and mode 1 means use Xiaolin
 * Wu's line algorithm (yes anti-aliasing)
 *
 * this program takes three arguments and one file input
 * xres - x resolution (number of pixels) of the output image
 * yres - y resolution (number of pixels) of the output image
 * mode - either 0 or 1 for determining which line algorithm to use
 * one file input using standard input - this file contains all required
 *		on the wireframe
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <math.h>

// include Eigen
#include <Eigen/Eigen>

// include code for parsing .iv datafiles
#include "ivdata.h"
// include util.h
#include "util.h"

using namespace std;
using namespace Eigen;

/* this function takes an instance of Camera and fills in its world2cam
 * 
 * this function takes one argument:
 *		camera - reference to instance of camera
 */
void calculate_world2cam(Camera &camera);

/* this function takes an instance of Camera and fills in its perspective_mat
 * 
 * this function takes one argument:
 *		camera - reference to instance of camera
 */
void calculate_perspective_mat(Camera &camera);

/* this function takes a vector of Objects and fills in the overall_transform
 * of each Object in the vector
 *
 * this function takes one argument:
 *		objects - reference to the vector of Objects
 */
void calculate_overall_transforms(vector<Object> &objects);

/* this function takes a vector of Objects and fills in the ndc vector in each
 * of the Objects in the said vector
 * transforming fromm world coordinates to ndc requires multiplying the world
 * coordinates with overall_transform of the object and then multiplying
 * camera.world2cam followed by the perspective matrix
 *
 * this function takes two arguments:
 *		objects - reference to the Object vector
 *		camera - instance of Camera (stores world2cam and perpespective_mat)
 */
void calculate_ndc(vector<Object> &objects, Camera camera);

/* this function draws every point included in a given vector of Objects
 *
 * this function takes four arguments:
 *		objects - reference to the vector of Objects
 *		red, blue, green - pixel grids
 */
void draw_points(
	vector<Object> &objects,
	MatrixXd &red,
	MatrixXd &blue,
	MatrixXd &green);

/* this function rasterizes each of the edges of a given object's facesets
 * this is achieved by calling the correct line drawing algorithm for each of
 * the edges included in the faceset and for extra edges that break up every
 * non-triangle facesets into triangles
 *
 * this function takes four arguments:
 *		object_p - pointer to the object that is to be rasterized
 *		red, blue, green - references to the three pixel grids
 *		mode - an int that determines which rasterization algorithm to use
 *			- mode = 0: use Bresenham's line algorithm
 *			- mode = 1: use Xiaolin Wu's line algorithm
 *
 * _fs suffix means faceset
 */
void rasterize(
	vector<Object> &objects,
	MatrixXd &red,
	MatrixXd &blue,
	MatrixXd &green,
	int mode);

/* this function draws a line in the provided pixel grids
 * the drawn line connects two points specified by four NDC values
 *
 * this function takes eight arguments:
 *		x0, y0, x1, y1 - NDC of two points
 *		red, blue, green - references to the three pixel grids
 *		mode - an int that determines which rasterization algorithm to use
 *			- mode = 0: use Bresenham's line algorithm
 *			- mode = 1: use Xiaolin Wu's line algorithm
 */
void draw_line(
	float x0,
	float y0,
	float x1,
	float y1,
	MatrixXd &red,
	MatrixXd &blue,
	MatrixXd &green,
	int mode);

/* this function takes three different color pixel grids and outputs them into
 * a .ppm file
 *
 * this function takes four arguments:
 *		- red, blue, green - pixel grids
 *		- filename - name of the output file
 */
void render(MatrixXd red, MatrixXd blue, MatrixXd green, string filename);

/* this function takes three different color pixel grids and outputs them into
 * standard output
 *
 * this function takes one argument:
 *		- red, blue, green - pixel grids
 */
void std_out_render(MatrixXd red, MatrixXd blue, MatrixXd green);

/* this function swaps the values of two int values
 *
 * this function takes two arguments:
 *		a, b - references to the two integers
 */
void swap(int &a, int &b);

/* this function returns the sign of a float
 * i.o.w. it returns 0 if it's 0, 1 if positive, -1 if negative
 *
 * this function takes one argument:
 *		a - the float
 */
int sign(float a);

#endif
// include guard
#ifndef WIREFRAME_H
#define WIREFRAME_H

/* CS 171 Assignment by JongSeok (Parker) Won
 * 
 * this program renders and image using a given .iv datafile
 *
 * this program takes three arguments and one file input
 * xres - x resolution (number of pixels) of the output image
 * yres - y resolution (number of pixels) of the output image
 * k - degree of attenuation
 * one file input using standard input - this file contains all required info
 *
 * this program outputs the result into a .ppm file and also to std output
 */

#include <iostream>
#include <stdlib.h>
#include <math.h>

// include Eigen
#include <Eigen/Eigen>

// include util.h
#include "util.h"
// include ivdata_util.h
#include "ivdata_util.h"

// define maximum brightness
#define MAX_BRT 255

using namespace std;
using namespace Eigen;

/////////////////////////////////// POINT ////////////////////////////////////

/* this function draws every point included in a given vector of Objects
 *
 * this function takes four arguments:
 *      objects - reference to the vector of Objects
 *      red, blue, green - pixel grids
 */
void draw_points(
    vector<Object> &objects,
    MatrixXd &red,
    MatrixXd &blue,
    MatrixXd &green);

/////////////////////////////////// LINE /////////////////////////////////////

/* this function rasterizes each of the edges of a given list of objects
 * this is achieved by calling the correct line drawing algorithm for each of
 * the edges included in the faceset and for extra edges that break up every
 * non-triangle facesets into triangles
 *
 * this function takes five arguments:
 *      object_p - reference to the object that is to be rasterized
 *      red, blue, green - references to the three pixel grids
 *      mode - an int that determines which rasterization algorithm to use
 *          - mode = 0: use Bresenham's line algorithm
 *          - mode = 1: use Xiaolin Wu's line algorithm
 */
void raster_line(
    vector<Object> &objects,
    MatrixXd &red,
    MatrixXd &blue,
    MatrixXd &green,
    int mode);

/* this function draws a line in the provided pixel grids
 * the drawn line connects two points specified by four NDC values
 *
 * this function takes eight arguments:
 *      x0, y0, x1, y1 - NDC of two points
 *      red, blue, green - references to the three pixel grids
 *      mode - an int that determines which rasterization algorithm to use
 *          - mode = 0: use Bresenham's line algorithm
 *          - mode = 1: use Xiaolin Wu's line algorithm
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

/////////////////////////////////// TRIANGLE /////////////////////////////////

/* this function calculates the values of alpha, beta and gamma for a Point
 * based on three other points (these x and y values represent x and y
 * coordinates on the screen/pixel grid NOT NDC)
 *
 * this function takes eight arguments:
 *      x, y - x, y screen coordinates of the point in question
 *      x0, x1, x2, y0, y1, y2 - x, y screen coordinates of the basis points
 *
 * this function returns an instance of Vector3d (alpha, beta, gamma)
 */
Vector3d alpha_beta_gamma(
    int x, int y,
    int x0, int y0,
    int x1, int y1,
    int x2, int y2);

/* this function is a helper function for the alpha_beta_gamma function
 * this function exists for the sake of nicer looking code
 *
 * this function takes six arguments:
 *      x, y, x0, x1, y0, y1
 *
 * this function returns a float value
 */
float f(int x, int y, int x0, int y0, int x1, int y1);

/* this function rasterizes each of the faces of a given list of objects
 * if a face is not a triangle, it is broken up into triangles
 * shading information is stored in lights
 *
 * this function takes eight arguments:
 *      objects - reference to the vector of objects that is to be rasterized
 *      lights - reference to the vector of Point_Light instances
 *              used for shading
 *      camera - reference to the Camera instance
 *      red, blue, green - references to the three pixel grids
 *      depth_buffer - reference to the depth_buffer
 *      k - degree of attenuation
 */
void raster_triangle(
    vector<Object> &objects,
    vector<Point_Light> &lights,
    Camera &camera,
    MatrixXd &red,
    MatrixXd &blue,
    MatrixXd &green,
    MatrixXd &depth_buffer,
    float k);

/* this function fills a triangle with color
 * each vertex has a color and the color is interpolated over the triangle
 *
 * this function takes 16 arguments:
 *      x0, x1, x2, y0, y1, y2, z0, z1, z2 - NDC coordinates of the triangle
 *      rgb0, rgb1, rgb2 - color for each of the points
 *      red, blue, green - references to the three pixel grids
 *      depth_buffer - reference to the depth_buffer
 */
void fill_colored_triangle(
    float x0, float y0, float z0,
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    Vector3d rgb0, Vector3d rgb1, Vector3d rgb2,
    MatrixXd &red, MatrixXd &blue, MatrixXd &green,
    MatrixXd &depth_buffer);

/////////////////////////////////// SHADING //////////////////////////////////

/* this function calculates rgb of a point using the Phong reflection model
 *
 * this function takes four arguments:
 *      point_i - index of the point
 *      normal_i - index of the normal
 *      object, lights, camera - information required for shading calculation
 *      k - degree of attenuation
 *
 * this function returns an instance of Vector3d that represents the resulting
 * rgb of the point
 */
Vector3d lighting(
    int point_i,
    int normal_i,
    Object &object,
    vector<Point_Light> &lights,
    Camera &camera,
    float k);

/* this function adds in the eyelight effect into the scene */
void addEyelight(vector<Point_Light> &lights, Camera camera);

#endif
/* CS 171 Assignment by JongSeok (Parker) Won
 *
 * This header file contains some useful utility functions related to structs
 * contained in ivdata.h
 */

// include guard
#ifndef IVDATA_UTIL_H
#define IVDATA_UTIL_H

#include <Eigen/Eigen>
#include <iostream>
#include <vector>
#include <math.h>

// include code for parsing .iv datafiles
#include "ivdata.h"
// include util.h
#include "util.h"

using namespace Eigen;
using namespace std;

/* this function takes an instance of Camera and fills in its world2cam
 * 
 * this function takes one argument:
 *      camera - reference to instance of camera
 */
void calculate_world2cam(Camera &camera);

/* this function takes an instance of Camera and fills in its perspective_mat
 * 
 * this function takes one argument:
 *      camera - reference to instance of camera
 */
void calculate_perspective_mat(Camera &camera);

/* this function takes a vector of Objects and fills in the overall_transform
 * of each Object in the vector
 *
 * this function takes one argument:
 *      objects - reference to the vector of Objects
 */
void calculate_overall_transforms(vector<Object> &objects);

/* this function takes a vector of Objects and fills in the normal_transform
 * of each Object in the vector
 *
 * this function takes one argument:
 *      objects - reference to the vector of Objects
 */
void calculate_normal_transforms(vector<Object> &objects);

/* this function takes a vector of Objects and fills in the ndc vector in each
 * of the Objects in the said vector
 * transforming fromm world coordinates to ndc requires multiplying the world
 * coordinates with overall_transform of the object and then multiplying
 * camera.world2cam followed by the perspective matrix
 *
 * this function takes two arguments:
 *      objects - reference to the Object vector
 *      camera - instance of Camera (stores world2cam and perpespective_mat)
 */
void calculate_ndc(vector<Object> &objects, Camera camera);

#endif
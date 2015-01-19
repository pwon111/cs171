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

// include Eigen
#include <Eigen/Eigen>

#include "util.h"

#define MAX_BRT 255

using namespace std;
using namespace Eigen;

struct Ray
{
    Vector3f A;
    Vector3f B;
};

/* This function takes a 3D world coordinate and checks if it is inside a
 * globally stored superquadric */
float isq(float x, float y, float z);

/* This function takes a 3D world coordinate and finds the gradient of the isq
 * function at that point */
Vector3f isq_gradient(float x, float y, float z);

// this function simply returns isq_graident normalized
// helper function for find normals on the surfaces of superquadrics
Vector3f isq_normal(float x, float y, float z);

/* This function turns a superquadric into a parametric function in terms of u
 * and v and returns the point at a specific u and v values */
Vector3f psq(float u, float v);

/* This function finds the smallest value of t at which a ray (globally stored)
 * intersects a globally stored superquadric
 * If there is no intersection FLT_MAX is returned */
float calculateIntersection(Ray ray);

/* This function finds the value of t at which a ray (globally stored) first
 * hits the bounding sphere of a superquadric (globally stored)
 * If there is no intersection FLT_MAX is returned */
float rayXBoundingSphere(Ray ray);

// These are helper functions for calculating the actual intersection using
// Newton's Method
float g(Ray ray, float t);
float g_slope(Ray ray, float t);

/* This function returns the lighting color for one ray using Phong Lighting
 * Model (ray, superquadric and lights in the scene are globally stored)
 * 
 * This function takes two arguments:
 *      - point - the point at which we want the lighting for (point is going to
 *              be the intersection between ray and superquadric)
 *      - k - attenuation factor
 */
Vector3f lighting(Vector3f point, float k);

#endif
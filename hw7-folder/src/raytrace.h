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

// struct that holds information for each superquadric being rendered
struct Superquadric
{
    // id
    // useful for keeping track of which superquadric to use when calculating
    // lighting
    int id;

    // transformations for this superquadric
    Vector3f T; // translation
    Vector3f S; // scaling
    Vector4f R; // rotation
    
    // e and n values that define the shape of the superquadric
    float e;
    float n;

    // color of the material of the superquadric
    Vector3f color_d;
    Vector3f color_s;
    // shininess of the material
    float shininess;
};

// struct that holds the information for each light source in the scene being
// rendered
struct Light
{
    // position
    Vector3f origin;
    // light color
    Vector3f rgb;
};

// struct that holds the information for a ray
// it's probably going to be useful in this lab. it's not like it's called ray
// tracing or anything
struct Ray
{
    // A * t + B represents a ray
    Vector3f A;
    Vector3f B;
};

/* This function takes a 3D world coordinate and checks if it is inside a
 * specifc superquadric
 *
 * This function takes four arguments
 *      - x, y, z - the 3D world coordinate
 *      - s - a pointer to the superquadric in question
 */
float isq(
    float x,
    float y,
    float z,
    Superquadric* s);

/* This function takes a 3D world coordinate and finds the gradient of the isq
 * function at that point
 *
 * This function takes four arguments
 *      - x, y, z - the 3D world coordinate
 *      - s - a pointer to the superquadric in question
 */
Vector3f isq_gradient(
    float x,
    float y,
    float z,
    Superquadric* s);

// this function simply returns isq_graident normalized
// helper function for find normals on the surfaces of superquadrics
Vector3f isq_normal(
    float x,
    float y,
    float z,
    Superquadric* s);

/* This function turns a superquadric into a parametric function in terms of u
 * and v and returns the point at a specific u and v values
 *
 * This function takes three arguments:
 *      - u, v - the u, v values in question
 *      - s - a pointer to the superquadric in question
 */
Vector3f psq(float u, float v, Superquadric* s);

/* This function finds the smallest value of t at which a ray intersects a
 * superquadric
 * If there is no intersection FLT_MAX is returned
 *
 * This function takes two arguments:
 *      - ray - the ray in question
 *      - s - a pointer to the superquadric in question
 */
float calculateIntersection(Ray ray, Superquadric* s);

/* This function finds the value of t at which a ray first hits the bounding
 * sphere of a superquadric
 * If there is no intersection FLT_MAX is returned
 *
 * This function takes two arguments:
 *      - ray - the ray in question
 *      - s - a pointer to the superquadric in question
 */
float rayXBoundingSphere(Ray ray, Superquadric* s);

// These are helper functions for calculating the actual intersection using
// Newton's Method
float g(Ray ray, Superquadric* s, float t);
float g_slope(Ray ray, Superquadric* s, float t);

/* This function returns the lighting color for one ray using Phong Lighting
 * Model
 * 
 * This function takes six arguments:
 *      - ray - the ray in question
 *      - superquadrics - a pointer to a vector of pointers of superquadrics in
 *                      the scene
 *      - lights - a pointer to a vector of pointers of lights in the scene
 *      - k - attenuation factor
 *      - recursion_lvl - this function can be called recursively for reflection
 *                      and this variable keeps track of the recursion depth
 *      - last_s_id - this function keeps track of the last superquadric this
 *                  function was called from. Only relevant for reflections
 *                  since one source of bug is when the ray hits the
 *                  superquadric it started from. Initial call of lighting
 *                  should be -1 since id of superquadrics is never -1
 */
Vector3f lighting(
    Ray ray,
    vector<Superquadric*>* superquadrics,
    vector<Light*>* lights,
    float k,
    int recursion_lvl,
    int last_s_id);

#endif
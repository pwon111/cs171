/* CS/CNS 171 Fall 2014
 *
 * This is a remake of the 'ivdata.h' header file from Assignment 2 that
 * accommodates for the different needs of OpenGL.
 *
 * The comments are sparse, since the changes should all be fairly straight-
 * forward to you after you have looked over our code in the 'opengl_demo.cpp'
 * file.
 */

// include guard
#ifndef IVDATA_H
#define IVDATA_H

#include <vector>

using namespace std;

/* The following struct is used for representing points and normals in world
 * coordinates.
 */
struct Triple
{
    float x;
    float y;
    float z;
};

/* The following struct is used for storing a set of transformations. If at
 * any point one of the three transformations is missing from a "Transform
 * block" in the Open Inventor file, then that transformation is set to
 * its "identity" version. i.e.:
 *
 * "identity translation" = translate by [0, 0, 0]
 * "identity rotation" = rotate by 0 degrees around [0, 0, 0]
 * "identity scaling" = scale by [1, 1, 1]
 *
 * The above transformations produce no change when applied to a vector
 * or matrix.
 */
struct Transforms
{
    /* For each array below,
     * Index 0 has the x-component
     * Index 1 has the y-component
     * Index 2 has the z-component
     */
    float translation[3];
    float rotation[3];
    float scaling[3];
    
    /* The parser sets the following angle in RADIANS. Make sure to convert to
     * degrees when necessary.
     */
    float rotation_angle;
};

/* The following struct is used for representing an object defined in a
 * "Separator block" in an Open Inventor file.
 */
struct Object
{
    /* These four following vectors are the exact same as they were in
     * Assignment 2 (except we now use the 'Triple' struct for both points
     * and normals).
     */
    vector<Triple> points;
    vector<Triple> normals;
    vector< vector<int> > facesets;
    vector< vector<int> > normal_sets;
    
    /* You are going to need to form the vertex and normal arrays below
     * yourself using the above four vectors.
     */
    vector<Triple> vertex_buffer;
    vector<Triple> normal_buffer;
    
    vector<Transforms> transform_sets;
    
    /* Index 0 has the r-component
     * Index 1 has the g-component
     * Index 2 has the b-component
     */
    float ambient_reflect[3];
    float diffuse_reflect[3];
    float specular_reflect[3];
    
    float shininess;
};

/* The following struct is used for representing a point light.
 */
struct Point_Light
{
    /* Index 0 has the x-coordinate
     * Index 1 has the y-coordinate
     * Index 2 has the z-coordinate
     * Index 3 has the w-coordinate
     */
    float position[4];
    
    /* Index 0 has the r-component
     * Index 1 has the g-component
     * Index 2 has the b-component
     */
    float color[3];

    /* We commented out the attenuation constant, since attenuation was not in
     * the main part of Assignment 2 (and thus is also not required for this
     * assignment). Those of you who want to play around with attenuation are
     * free to uncomment the line though.
     */
    float attenuation_k;
};

/* This following struct is used for representing a camera.
 */
struct Camera
{
    float position[3];
    float orientation_axis[3];
    
    /* The parser sets the following angle in RADIANS. Make sure to convert to
     * degrees when necessary.
     */
    float orientation_angle;
    
    float near;
    float far;
    float left;
    float right;
    float top;
    float bottom;
};

/* A self-explanatory struct representing the overall Open Inventor file.
 */
struct IVData
{
    Camera camera;
    vector<Object> objects;
    vector<Point_Light> lights;
};

#endif
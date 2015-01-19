/* CS/CNS 171 Fall 2014
 *
 * This header file contains a C++ struct representation of the
 * Open Inventor file format.
 *
 * You may change the implementation in this header file as you
 * find convenient, but be wary that some changes may require
 * corresponding changes in the parser code (the syntax of which
 * we won't cover until a later assignment).
 *
 * Also keep in mind that we will give you updated versions of
 * this header file in the later assignments as we continue to
 * work with the Open Inventor file format; hence, any changes
 * that you make here will have to be made again to the later
 * versions that we give you.
 *
 * Feel free to get rid of our comments if you find them ugly :p
 */

#pragma once

#include <Eigen/Eigen>
#include <vector>

using namespace Eigen;
using namespace std;

/* A self-explanatory struct representing a point/vertex in
 * homogeneous coordinates.
 */
struct Point
{
    float x;
    float y;
    float z;
    float w;
};

/* This struct is meant to represent a "Separator" block from
 * the Open Inventor file format. Each Separator block contains
 * the details that characterize an object in our scene.
 */
struct Object
{
    /* A vector to store the matrix representations of every
     * transformation that needs to be applied to the object.
     * 
     * This vector is already filled in by the parser with all
     * the specified transformation matrices for the object in
     * the .iv file.
     * 
     * The transformations are specified in STACK order. This
     * means that the LAST transformation in the vector is the
     * FIRST transformation that gets applied.
     */
    vector<MatrixXd> transforms;
    
    /* A vector to store all the vertices that define the object.
     * 
     * This vector is already filled in by the parser with all
     * the specified points for the object in the .iv file. The
     * points are all in homogeneous world coordinates.
     * 
     * It is recommended to not modify this vector. If you want
     * to transform the coordinates of these points and store
     * the resultant coordinates somewhere, then we recommend
     * you store them in the vector called 'ndc' further below.
     *
     * There will be times where you might want to keep a copy
     * of the original coordinates of the points.
     */
    vector<Point> points;
    
    /* A vector to store the "facesets" from the Open Inventor
     * file format. Each inner vector contains a set of points
     * that make a face in our object. The set of points are
     * specified as a set of indices, each index referring to
     * the index of a point in the 'points' vector. For
     * instance, the first vector of 'facesets' might be
     * [0, 1, 3], which would specify a triangle made up of the
     * first, second, and fourth vertices in 'points'. It also
     * does not have to be a triangle. For instance, the first
     * vector might be [0, 1, 3, 5], which would specify a square
     * made up of the first, second, fourth, and sixth vertices
     * in 'points'.
     *
     * This vector is already filled in by the parser with all
     * the specified facesets for the object in the .iv file.
     */
    vector< vector<int> > facesets;
    
    /* You may find it convenient to use the following matrix
     * to store the overall transformation matrix resulting
     * from the product of all matrices in the 'transforms'
     * vector.
     * 
     * This matrix is NOT initialized by the parser.
     */
    MatrixXd overall_transform;
    
    /* You may find it convenient to use the following vector
     * to store all the points that you get after transforming
     * all the points in the 'points' vector from world cooridnates
     * to NDC.
     * 
     * This vector is NOT initialized by the parser.
     */
    vector<Point> ndc;
};

/* This struct is meant to represent the "PerspectiveCamera"
 * block from the Open Inventor file format. The block contains
 * all the details regarding our set up of the camera, through
 * which we view our scene.
 */
struct Camera
{
    /* Stores the position of the camera in non-homogeneous world
     * coordinates. Index 0 stores the x-coordinate; index 1 stores
     * the y-coordinate; and index 2 stores the z-coordinate.
     *
     * This Vector3d is already initialized by the parser with the
     * specified camera position in the .iv file. The position is
     * in world coordinates.
     */
    Vector3d position;
    
    /* Stores the vector pointing in the direction of our camera's
     * orientation axis. Index 0 stores the x-component; index 1
     * stores the y-component; and index 2 stores the z-component.
     *
     * This Vector3d is already initialized by the parser with the
     * specified orientation axis in the .iv file.
     */
    Vector3d orientation_axis;
    
    /* Stores the angle that we should rotate the camera about the
     * orientation axis by.
     *
     * This float is already initialized by the parser with the
     * specified orientation angle in RADIANS in the .iv file.
     */
    float orientation_angle;
    
    /* Our parameters for our perspective projection.
     * 
     * These are already initialized by the parser with the
     * specified values in the .iv file.
     */
    float near;
    float far;
    float left;
    float right;
    float top;
    float bottom;
    
    /* You may find it convenient to use the following matrix to
     * store the transformation matrix for switching between
     * world and camera coordinates.
     * 
     * This matrix is NOT initialized by the parser.
     */
    MatrixXd world2cam;
    
    /* You may find it convenient to use the following matrix to
     * store the transformation matrix for switching between
     * camera coordinates and NDC.
     * 
     * This matrix is NOT initialized by the parser.
     */
    MatrixXd perspective_mat;
};

/* A self-explanatory struct representing the overall Open Inventor file.
 */
struct IVData
{
    Camera camera;
    vector<Object> objects;
};

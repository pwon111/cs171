/* CS/CNS 171 Fall 2014
 *
 * This file contains the actual implementations of the functions in util.h.
 * Right now, each function simply returns the identity matrix. You will
 * fill these functions in with the proper implementations in Assignment 1.
 */

#include "util.h"

/* Gets the translation matrix for a given translation vector (x, y, z). */
Matrix4f get_translate_mat(float x, float y, float z)
{
    Matrix4f translation_matrix = Matrix4f::Identity();
    translation_matrix(0, 3) = x;
    translation_matrix(1, 3) = y;
    translation_matrix(2, 3) = z;
    return translation_matrix;
}

/* Gets the rotation matrix for a given rotation axis (x, y, z) and angle. */
Matrix4f get_rotate_mat(float x, float y, float z, float angle)
{
    Matrix4f rotation_matrix;
    float magnitude = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    x *= 1 / magnitude;
    y *= 1 / magnitude;
    z *= 1 / magnitude;
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
                    z * z + (1 - z * z) * cos(angle),
                    0,
                    // fourth row
                    0, 0, 0, 1;
    return rotation_matrix;
}

/* Gets the scaling matrix for a given scaling vector (x, y, z). */
Matrix4f get_scale_mat(float x, float y, float z)
{
    Matrix4f scaling_matrix = Matrix4f::Identity();
    scaling_matrix(0, 0) = x;
    scaling_matrix(1, 1) = y;
    scaling_matrix(2, 2) = z;
    return scaling_matrix;
}

/* Gets the perspective projection matrix given the frustum parameters. */
Matrix4f get_perspective_mat(float near, float far,
                            float left, float right,
                            float top, float bottom)
{
    Matrix4f perspective_matrix;
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

/* This function converts given angle in degrees to radians. */
float deg2rad(float angle)
{
    return angle * M_PI / 180.0;
}

/* This function converts given angle in radians to degrees. */
float rad2deg(float angle)
{
    return angle * 180.0 / M_PI;
}

// helper functions for parameterizing superquadrics
float exp_cos(float theta, float e)
{
    if(cos(theta) == 0)
        return 0;
    else
        return sign(cos(theta)) * pow(abs(cos(theta)), e);
}
float exp_sin(float theta, float e)
{
    if(sin(theta) == 0)
        return 0;
    else
        return sign(sin(theta)) * pow(abs(sin(theta)), e);
}

// returns the sign of a number
// -1 for negative, 1 for positive, 0 for 0
float sign(float num)
{
    return num / abs(num);
}

// conversion between Vertext3f and Vertex4f
Vector3f Vector4fTo3f(Vector4f v)
{
    Vector3f v_3f;
    v_3f << v[0] / v[3],
            v[1] / v[3],
            v[2] / v[3];
    return v_3f;
}
Vector4f Vector3fTo4f(Vector3f v)
{
    Vector4f v_4f;
    v_4f << v[0], v[1], v[2], v[3];
    return v_4f;
}

// this function returns the smaller root of a quadratic equation
float small_ans(Quadratic q)
{
    float discriminant = pow(q.b, 2) - 4 * q.a * q.c;
    if(discriminant < 0)
        return FLT_MAX;
    float t_neg = (-q.b - sqrt(discriminant)) / (2 * q.a);
    float t_pos = (2 * q.c) / (-q.b - sqrt(discriminant));
    return min(t_neg, t_pos);
}

/* this function takes three different color pixel grids and outputs them into
 * a .ppm file
 *
 * this function takes four arguments:
 *      - red, green, blue - pixel grids
 *      - filename - name of the output file
 */
void ppm_render(MatrixXd red, MatrixXd green, MatrixXd blue, string filename)
{
    // assuming red, blue and green are all of the same dimension
    int xres = red.rows();
    int yres = red.cols();
    // ready output file
    ofstream output_file;
    output_file.open(filename.c_str());
    // write header of the ppm file
    output_file << "P3" << endl;
    output_file << xres << " " << yres << endl;
    output_file << 255 << endl;
    // write all pixels into output_file
    for(int y = yres - 1; y >= 0; y--)
        for(int x = 0; x < xres; x++)
            output_file << red(x, y) << " "
                        << green(x, y) << " "
                        << blue(x, y) << endl;
    // close file
    output_file.close();
}

/* this function takes three different color pixel grids and outputs them into
 * standard output
 *
 * this function takes one argument:
 *      - red, green, blue - pixel grids
 */
void std_out_render(MatrixXd red, MatrixXd green, MatrixXd blue, int MAX_BRT)
{
    // assuming red, blue and green are all of the same dimension
    int xres = red.rows();
    int yres = red.cols();
    cout << "P3" << endl << xres << " " << yres << endl << MAX_BRT << endl;
    for(int y = yres - 1; y >= 0; y--)
        for(int x = 0; x < xres; x++)
            cout << red(x, y) << " "
                << green(x, y) << " "
                << blue(x, y) << endl;
}
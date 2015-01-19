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

/* this function takes three different color pixel grids and outputs them into
 * a .ppm file
 *
 * this function takes four arguments:
 *      - red, blue, green - pixel grids
 *      - filename - name of the output file
 */
void ppm_render(MatrixXd red, MatrixXd blue, MatrixXd green, string filename)
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
 *      - red, blue, green - pixel grids
 */
void std_out_render(MatrixXd red, MatrixXd blue, MatrixXd green)
{
    // assuming red, blue and green are all of the same dimension
    int xres = red.rows();
    int yres = red.cols();
    cout << "P3" << endl << xres << " " << yres << endl << 255 << endl;
    for(int y = yres - 1; y >= 0; y--)
        for(int x = 0; x < xres; x++)
            cout << red(x, y) << " "
                << green(x, y) << " "
                << blue(x, y) << endl;
}

/* this function swaps the values of two int values */
void swap(int &a, int &b)
{
    int temp = b;
    b = a;
    a = temp;
}

/* this function returns the sign of a float
 * i.o.w. it returns 0 if it's 0, 1 if positive, -1 if negative
 */
int sign(float a)
{
    if(a == 0)
        return 0;
    else if(a > 0)
        return 1;
    else
        return -1;
}

/* this function truncates an integer to a set max value */
int truncate(int a, int b)
{
    if(a > b)
        return b;
    return a;
}

/* this function returns the maximum value among three given float values */
float max(float a, float b, float c)
{
    if(a > b)
        if(a > c)
            return a;
        else
            return c;
    else
        if(b > c)
            return b;
        else
            return c;
}

/* this function returns the minimum value among three given float values */
float min(float a, float b, float c)
{
    if(a < b)
        if(a < c)
            return a;
        else
            return c;
    else
        if(b < c)
            return b;
        else
            return c;
}
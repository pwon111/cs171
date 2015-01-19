// include guard
#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>

#include <math.h>
#define _USE_MATH_DEFINES

// include OpenGL
#include <GL/glew.h>
#include <GL/glut.h>

// include Eigen
#include <Eigen/Eigen>

#include "ivdata.h"
#include "util.h"

#define BUFFER_SIZE 256

using namespace std;
using namespace Eigen;

// self-explanatory struct for storing Frames
struct Frame
{
    int id;
    Vector3f translation;
    Vector3f scaling;
    Vector4f rotation;
};
// self-explanatory struct for storing scripts
struct Script
{
    int frame_count;
    vector<Frame> key_frames;
    vector<Frame> frames;

    // s is calcualted using t, the tension parameter
    int s;
    // B is calculated using s and is used for linearly interpolating
    // translation and scaling transformations
    Matrix4f B;
};

// This function initializes all of the basic required OpenGLsettings
void init(void);

/* This function defines how the display responds when the window is resized
 *
 * This function takes two arguments:
 *      - width, height - new width and height to adjust to (int)
 */
void reshape(int width, int height);

// This function defines how each frame is rendered
void display(void);

// This function defines how the IBar is drawn
void drawIBar();

/* This function defines how OpenGL responds on key press
 *
 * This function takes three arguments:
 *      - key - the character of the key itself or the ASCII value of the key
 *      - x, y - screen coordinates (int)
 */
void key_pressed(unsigned char key, int x, int y);

/* This function takes a .script datafile and parses out the information on each
 * keyframe into an instance of the Script struct
 *
 * This function takes two arguments:
 *      - datafile - a reference to the istream instance of the input .script
 *                  datafile
 *      - t - the tension parameter
 *
 * NOTE: This function makes various assumptions about the .script file. This
 *      means that the .script file MUST follow the EXACT formatting of the
 *      .script files given as examples
 */
Script parse_script(istream &datafile, int t);

/* This function parses out all of the numbers in a line in the .script datafile
 *
 * This function takes one argument:
 *      - line - char array that contain the line in question
 *
 * NOTE: This function assumes that the line provided follows a very strict
 *      format. Only feed it lines from the .script datafiles (except the first
 *      lines which contains the total number of frames)
 */
vector<float> parse_script_line_nums(char* line);

/* This function calculates the frame for cur_frame_id
 *
 * FOR TRANSLATION AND SCALING: we find the cardinal spline for the given
 *                              keyframes
 * FOR ROTATION: we use SLERP (use sin(omega) version if mode is 1 and
 *                          sin(omega) is not 0)
 */
void calculateFrame();

// helper function for SLERP
// returns the correct A rotation vector for a given Q quaternion
Vector4f QToA(Vector4f Q);

// helper function for SLERP
// returns the correct Q quaternion for a given A rotation vector
Vector4f AToQ(Vector4f A);

/* This function converts given angle in degrees to radians. */
float deg2rad(float angle);

/* This function converts given angle in radians to degrees. */
float rad2deg(float angle);

#endif
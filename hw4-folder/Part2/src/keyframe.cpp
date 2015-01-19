/* CS 171 Assignment by JongSeok (Parker) Won
 *
 * This OpenGL program parses a .script file that contains sets of
 * transformations (translation, scaling and rotation) that represent keyframes
 * and displays an animation produced by linearly interpolating between the
 * keyframes
 *
 * this program takes four arguments and one file input
 * xres - x resolution (number of pixels) of the output image
 * yres - y resolution (number of pixels) of the output image
 * t - tension parameter
 * mode - 0 for linear interpolation, 1 for interpolation with sin(omega) in
 *      SLERP
 * one file input using standard input - the script datafile
 */

#include "keyframe.h"

// self-explanatory global variables
int mode;
bool running = false;
int xres, yres;
GLUquadricObj *quadratic;
Script script;
int cur_frame_id = 0;
int cur_key_frame_id = 0;

// setup camera
Camera camera = {
    {0, 0, 40}, // position
    {1, 0, 0}, // orientation_axis
    0, // orientation_angle
    1.0, // near
    50.0, // far
    -1.0, // left
    1.0, // right
    -1.0, // bottom
    1.0, // top
};

// main function
int main(int argc, char* argv[])
{
    if(argc != 5)
        cout << "usage: " << argv[0] << " <xres> <yres> <mode> <t> < <filename>"
            << endl;
    else
    {
        // setup necessary global variables
        xres = atoi(argv[1]);
        yres = atoi(argv[2]);
        mode = atoi(argv[3]);
        script = parse_script(cin, atoi(argv[4]));

        // init OpenGL
        glutInit(&argc, argv);
        // set OpenGL display mode
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        // setup test window
        glutInitWindowSize(xres, yres);
        glutInitWindowPosition(0, 0);
        glutCreateWindow("Test");
        
        // call init
        init();
        // Specify to OpenGL our display function.
        glutDisplayFunc(display);
        // Specify to OpenGL our reshape function.
        glutReshapeFunc(reshape);
        // Specify to OpenGL our function for handling key presses.
        glutKeyboardFunc(key_pressed);

        // start main loop
        glutMainLoop();
    }
}

// This function initializes all of the basic required OpenGLsettings
void init(void)
{
    // use Gouraud shading model
    glShadeModel(GL_SMOOTH);
    // enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // enable depth buffering
    glEnable(GL_DEPTH_TEST);
    // enable using vertex_buffer and normal_buffer
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    // setup projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // setup OpenGL frustum
    glFrustum(camera.left, camera.right,
              camera.bottom, camera.top,
              camera.near, camera.far);
    // switch glMatrixMode back to GL_MODELVIEW
    glMatrixMode(GL_MODELVIEW);
    // setup quadratic for drawing cylinders
    quadratic = gluNewQuadric();
}

/* This function defines how the display responds when the window is resized
 *
 * This function takes two arguments:
 *      - width, height - new width and height to adjust to (int)
 */
void reshape(int width, int height)
{
    // width and height can not be less than 1
    height = (height == 0) ? 1 : height;
    width = (width == 0) ? 1 : width;
    // change the viewport accordingly
    glViewport(0, 0, width, height);
    // redisplay
    glutPostRedisplay();
}

// This function defines how each frame is rendered
void display(void)
{
    // clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // reset glMatrix
    glLoadIdentity();
    // camera tranformation matrices
    glRotatef(-rad2deg(camera.orientation_angle),
              camera.orientation_axis[0],
              camera.orientation_axis[1],
              camera.orientation_axis[2]);
    glTranslatef(-camera.position[0],
                -camera.position[1],
                -camera.position[2]);
    // draw the IBar using the script
    drawIBar();
    // swap buffer
    glutSwapBuffers();
}

// This function defines how the IBar is drawn
void drawIBar()
{
    // if the frame with cur_frame_id has not yet been interpolated, calculate
    // the frame
    if(cur_frame_id >= script.frames.size())
        calculateFrame();
    // push matrix for each IBar
    glPushMatrix();
    // apply appropriate transformations
    glTranslatef(script.frames.at(cur_frame_id).translation[0],
                script.frames.at(cur_frame_id).translation[1],
                script.frames.at(cur_frame_id).translation[2]);
    glScalef(script.frames.at(cur_frame_id).scaling[0],
            script.frames.at(cur_frame_id).scaling[1],
            script.frames.at(cur_frame_id).scaling[2]);
    glRotatef(rad2deg(script.frames.at(cur_frame_id).rotation[3]),
            script.frames.at(cur_frame_id).rotation[0],
            script.frames.at(cur_frame_id).rotation[1],
            script.frames.at(cur_frame_id).rotation[2]);

    /* Parameters for drawing the cylinders */
    float cyRad = 0.2, cyHeight = 1.0;
    int quadStacks = 4, quadSlices = 4;
    
    glPushMatrix();
    glColor3f(0, 0, 1);
    glTranslatef(0, cyHeight, 0);
    glRotatef(90, 1, 0, 0);
    gluCylinder(quadratic, cyRad, cyRad, 2.0 * cyHeight, quadSlices, quadStacks);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(0, 1, 1);
    glTranslatef(0, cyHeight, 0);
    glRotatef(90, 0, 1, 0);
    gluCylinder(quadratic, cyRad, cyRad, cyHeight, quadSlices, quadStacks);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1, 0, 1);
    glTranslatef(0, cyHeight, 0);
    glRotatef(-90, 0, 1, 0);
    gluCylinder(quadratic, cyRad, cyRad, cyHeight, quadSlices, quadStacks);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1, 1, 0);
    glTranslatef(0, -cyHeight, 0);
    glRotatef(-90, 0, 1, 0);
    gluCylinder(quadratic, cyRad, cyRad, cyHeight, quadSlices, quadStacks);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(0, 1, 0);
    glTranslatef(0, -cyHeight, 0);
    glRotatef(90, 0, 1, 0);
    gluCylinder(quadratic, cyRad, cyRad, cyHeight, quadSlices, quadStacks);
    glPopMatrix();

    glPopMatrix();
    
    // only go to next frame if running is set to true
    if(running)
    {
        cur_frame_id = (cur_frame_id + 1) % script.frame_count;
        glutPostRedisplay();
    }
}

/* This function defines how OpenGL responds on key press
 *
 * This function takes three arguments:
 *      - key - the character of the key itself or the ASCII value of the key
 *      - x, y - screen coordinates (int)
 */
void key_pressed(unsigned char key, int x, int y)
{
    // q for quit
    if(key == 'q')
    {
        exit(0);
    }
    // p for pause
    if(key == 'p')
    {
        running = !running;
        glutPostRedisplay();
    }
    // s for step
    if(key == 's')
    {
        running = false;
        cur_frame_id = (cur_frame_id + 1) % script.frame_count;
        glutPostRedisplay();
    }
}

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
Script parse_script(istream &datafile, int t)
{
    // buffer char[] for reading each line
    char line[BUFFER_SIZE];
    // read first line with the total frame count
    datafile.getline(line, BUFFER_SIZE);
    int frame_count = atoi(line);
    Script script;
    script.frame_count = frame_count;
    // Frame for temporarily storing data from the .script file
    Frame temp = Frame();
    // it is assumed that the keyframes follow the exact formatting of:
    //      Frame #
    //      translation # # #
    //      scale       # # #
    //      rotation    # # # #
    // the variable type uses this fact to deduce the type of information that
    // the current line holds
    //      - type = 0 means line is for Frame
    //      - type = 1 means line is for translation
    //      - type = 2 means line is for scaling
    //      - type = 3 means line is for rotation
    int type = 0;
    while(datafile.getline(line, BUFFER_SIZE))
    {
        // parse the numbers in line
        vector<float> line_nums = parse_script_line_nums(line);
        switch(type)
        {
            case 0:
                temp.id = int(line_nums.front());
                break;
            case 1:
                temp.translation[0] = line_nums[0];
                temp.translation[1] = line_nums[1];
                temp.translation[2] = line_nums[2];
                break;
            case 2:
                temp.scaling[0] = line_nums[0];
                temp.scaling[1] = line_nums[1];
                temp.scaling[2] = line_nums[2];
                break;
            case 3:
                temp.rotation[0] = line_nums[0];
                temp.rotation[1] = line_nums[1];
                temp.rotation[2] = line_nums[2];
                temp.rotation.normalize();
                temp.rotation[3] = deg2rad(line_nums[3]);
                // the fact that we just added in the rotation means that we
                // parsed one complete keyframe
                script.key_frames.push_back(temp);
                // reset temp
                temp = Frame();
                break;
        }
        type = (type + 1) % 4;
    }
    // add in the first keyframe to script.frames
    script.frames.push_back(script.key_frames.front());
    // calculate script.B which is going to be used for linearly interpolating
    // the translation and scaling transformations
    float s = (1 - t) / 2;
    script.s = s;
    script.B << 0,      1,      0,          0,
                -s,     0,      s,          0,
                2 * s,  s - 3,  3 - 2 * s,  -s,
                -s,     2 - s,  s - 2,      s;
    return script;
}

/* This function parses out all of the numbers in a line in the .script datafile
 *
 * This function takes one argument:
 *      - line - char array that contain the line in question
 *
 * NOTE: This function assumes that the line provided follows a very strict
 *      format. Only feed it lines from the .script datafiles (except the first
 *      lines which contains the total number of frames)
 */
vector<float> parse_script_line_nums(char* line)
{
    // position where numbers start
    // hardcoded***
    int last_space = 11;
    // position where numbers start is different for Frame
    if(line[0] == 'F')
        last_space = 5;
    // buffer
    char buffer[BUFFER_SIZE];
    // temp will be returned
    vector<float> temp;
    // read until you run into a space and once you do turn what you've read
    // into float and append the float to temp
    int index = last_space;
    while(index < strlen(line))
    {
        index++;
        while(line[index] != ' ' && index < strlen(line))
        {
            buffer[index - last_space - 1] = line[index];
            index++;
        }
        temp.push_back(atof(buffer));
        last_space = index;
        memset(buffer, '\0', BUFFER_SIZE);
    }
    return temp;
}

/* This function calculates the frame for cur_frame_id
 *
 * FOR TRANSLATION AND SCALING: we find the cardinal spline for the given
 *                              keyframes
 * FOR ROTATION: we use SLERP (use sin(omega) version if mode is 1 and
 *                          sin(omega) is not 0)
 */
void calculateFrame()
{
    // get all four frames we need for interpolation
    int prev_key_frame_id = (cur_key_frame_id - 1 + script.key_frames.size())
                            % script.key_frames.size();
    int next_key_frame_id = (cur_key_frame_id + 1) % script.key_frames.size();
    int next2_key_frame_id = (cur_key_frame_id + 2) % script.key_frames.size();
    Frame &prev_key_frame = script.key_frames.at(prev_key_frame_id);
    Frame &cur_key_frame = script.key_frames.at(cur_key_frame_id);
    Frame &next_key_frame = script.key_frames.at(next_key_frame_id);
    Frame &next2_key_frame = script.key_frames.at(next2_key_frame_id);
    // if we are at a keyframe, simply add in the keyframe
    if(cur_frame_id == next_key_frame.id)
    {
        script.frames.push_back(next_key_frame);
        cur_key_frame_id = next_key_frame_id;
    }
    // else interpolate
    else
    {
        // calculate u
        float u;
        if(next_key_frame.id == 0)
            u = float(cur_frame_id - cur_key_frame.id) /
                (script.frame_count - cur_key_frame.id);
        else
            u = float(cur_frame_id - cur_key_frame.id) /
                (next_key_frame.id - cur_key_frame.id);
        MatrixXf U(1, 4);
        U << 1, u, pow(u, 2), pow(u, 3);
        // translation transformations of the 4 keyframes
        Vector4f T0(
            prev_key_frame.translation(0),
            cur_key_frame.translation(0),
            next_key_frame.translation(0),
            next2_key_frame.translation(0));
        Vector4f T1(
            prev_key_frame.translation(1),
            cur_key_frame.translation(1),
            next_key_frame.translation(1),
            next2_key_frame.translation(1));
        Vector4f T2(
            prev_key_frame.translation(2),
            cur_key_frame.translation(2),
            next_key_frame.translation(2),
            next2_key_frame.translation(2));
        // scaling transformations of the 4 keyframes
        Vector4f S0(
            prev_key_frame.scaling(0),
            cur_key_frame.scaling(0),
            next_key_frame.scaling(0),
            next2_key_frame.scaling(0));
        Vector4f S1(
            prev_key_frame.scaling(1),
            cur_key_frame.scaling(1),
            next_key_frame.scaling(1),
            next2_key_frame.scaling(1));
        Vector4f S2(
            prev_key_frame.scaling(2),
            cur_key_frame.scaling(2),
            next_key_frame.scaling(2),
            next2_key_frame.scaling(2));
        // create new frame
        Frame new_frame;
        new_frame.id = cur_frame_id;
        // set up U as U * B
        U *= script.B;
        // interpolate translation using U * B
        new_frame.translation[0] = (U * T0)[0];
        new_frame.translation[1] = (U * T1)[0];
        new_frame.translation[2] = (U * T2)[0];
        // interpolate scaling using U * B
        new_frame.scaling[0] = (U * S0)[0];
        new_frame.scaling[1] = (U * S1)[0];
        new_frame.scaling[2] = (U * S2)[0];
        // interpolate rotation using SLERP
        Vector4f Q0 = AToQ(cur_key_frame.rotation);
        Vector4f Q1 = AToQ(next_key_frame.rotation);
        float omega = asin(Q0.dot(Q1));
        Vector4f Q_u;
        if(mode == 1 && sin(omega) != 0)
            Q_u = sin((1 - u) * omega) / sin(omega) * Q0 +
                sin(u * omega) / sin(omega) * Q1;
        else
            Q_u = (1 - u) * Q0 + u * Q1;
        Q_u.normalize();
        Vector4f A_u = QToA(Q_u);
        new_frame.rotation[0] = A_u[0];
        new_frame.rotation[1] = A_u[1];
        new_frame.rotation[2] = A_u[2];
        new_frame.rotation[3] = A_u[3];
        script.frames.push_back(new_frame);
    }
}

// helper function for SLERP
// returns the correct A rotation vector for a given Q quaternion
Vector4f QToA(Vector4f Q)
{
    Vector4f A;
    if(abs(Q[3]) == 1)
    {
        A << 1, 0, 0, 0;
    }
    else
    {
        A << Q[0] / sqrt(1 - pow(Q[3], 2)),
            Q[1] / sqrt(1 - pow(Q[3], 2)),
            Q[2] / sqrt(1 - pow(Q[3], 2)),
            2 * acos(Q[3]);
    }
    return A;
}

// helper function for SLERP
// returns the correct Q quaternion for a given A rotation vector
Vector4f AToQ(Vector4f A)
{
    Vector4f Q;
    Q << A[0] * sin(A[3] / 2),
        A[1] * sin(A[3] / 2),
        A[2] * sin(A[3] / 2),
        cos(A[3] / 2);
    return Q;
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
/* CS 171 Assignment by JongSeok (Parker) Won
 *
 * This OpenGL program parses an iv datafile and displays the contents on a test
 * window
 *
 * this program takes three arguments and one file input
 * xres - x resolution (number of pixels) of the output image
 * yres - y resolution (number of pixels) of the output image
 * k - degree of attenuation
 * one file input using standard input - the iv datafile
 */

#include "io_test.h"

// global variables defined with their default values
int xres = 1000, yres = 1000;
float e = 1.0, n = 1.0;  // shape exponents
Vector3f S(1.0, 1.0, 1.0);  // scaling vector
Vector3f T(0.0, 0.0, 0.0);  // translation vector
Vector4f R(0.0, 0.0, 0.0, 1.0);  // rotation vector
//          ^theta in this vector is stored in degrees
float xmin = -1.2, ymin = -1.2, zmin = -1.2;
float xmax = 1.2, ymax = 1.2, zmax = 1.2;
int N = 20;
float jitter = 0.5; // jitter must be between 0 and 1
int Nu = 80, Nv = 40;

// global map for storing the random jitterings of the NxNxN points generated
// we don't want the jitterings to change every frame
// stores <x, y, z> int coordinates vs <dx, dy, dz> random jitters
map< array<int, 3>, Vector3f*> random_jitter;

// parameters for the camera position camera
// defaulted to an apporpriate value depending on the min and the max values of
// x, y and z
float camera_near;
float camera_far;
float camera_left;
float camera_right;
float camera_top;
float camera_bottom;
float camera_orientation_angle;
Vector3f camera_orientation_axis;
Vector3f camera_position;

// variables for interactive mouse ui
int mouse_x, mouse_y;
float mouse_scale_x, mouse_scale_y;
const float step_size = 0.2;
const float x_view_step = 90.0, y_view_step = 90.0;
float x_view_angle = 0, y_view_angle = 0;

// state variables
bool is_pressed = false;
bool wireframe = false;

// arcball matrix
Matrix4f arcball_rotation = Matrix4f::Identity();

// main function
int main(int argc, char* argv[])
{
    if(argc != 3 && argc != 23)
        cout << "usage: " << argv[0] <<
        " <e> <n>" <<
        " [<Sx> <Sy> <Sz> <Tx> <Ty> <Tz> <Rtheta> <Rx> <Ry> <Rz>" <<
        " <minX> <maxX> <minY> <maxY> <minZ> <maxZ>" <<
        " <N> <jitter> <Nu> <Nv>]" << endl;
    else
    {
        xres = 1000;
        yres = 1000;
        e = atof(argv[1]);
        n = atof(argv[2]);
        if(argc == 23)
        {
            S << atof(argv[3]),
                atof(argv[4]),
                atof(argv[5]);
            T << atof(argv[6]),
                atof(argv[7]),
                atof(argv[8]);
            R << atof(argv[9]),
                atof(argv[10]),
                atof(argv[11]),
                atof(argv[12]);
            xmin = atof(argv[13]);
            ymin = atof(argv[14]);
            zmin = atof(argv[15]);
            xmax = atof(argv[16]);
            ymax = atof(argv[17]);
            zmax = atof(argv[18]);
            N = atoi(argv[19]);
            jitter = atof(argv[20]);
            Nu = atof(argv[21]);
            Nv = atof(argv[22]);
        }
        // set camera parameters based on min and max values of x, y and z
        camera_near = 1;
        camera_far = 20;
        camera_left = -0.5;
        camera_right = 0.5;
        camera_top = 0.5;
        camera_bottom = -0.5;
        camera_orientation_angle = 0.0;
        camera_orientation_axis << 0.0, 0.0, 1.0;
        camera_position << 0.0, 0.0, 5 * zmax;
        
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
        // Specify to OpenGL our function for handling mouse presses.
        glutMouseFunc(mouse_pressed);
        // Specify to OpenGL our function for handling mouse movement.
        glutMotionFunc(mouse_moved);
        // Specify to OpenGL our function for handling key presses.
        glutKeyboardFunc(key_pressed);

        // start main loop
        glutMainLoop();
    }
}

/* This function initializes all of the basic required OpenGLsettings along with
 * setting up the projection matrix
 * fill_vertex_normal_buffer is also called in this function
 */
void init(void)
{
    // use Gouraud shading model
    glShadeModel(GL_SMOOTH);
    // enable depth buffering
    glEnable(GL_DEPTH_TEST);
    // setup projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(camera_left, camera_right,
              camera_bottom, camera_top,
              camera_near, camera_far);
    // switch glMatrixMode back to GL_MODELVIEW
    glMatrixMode(GL_MODELVIEW);
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
    // change mouse scale
    mouse_scale_x = (float) (camera_right - camera_left) / (float) width;
    mouse_scale_y = (float) (camera_top - camera_bottom) / (float) height;
    // redisplay
    glutPostRedisplay();
}

/* This function defines how each frame is rendered */
void display(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    // clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // reset glMatrix
    glLoadIdentity();
    // rotatation matrix for interactive rotation
    glRotatef(y_view_angle, 1, 0, 0);
    glRotatef(x_view_angle, 0, 1, 0);
    // camera tranformation matrices
    glRotatef(-rad2deg(camera_orientation_angle),
              camera_orientation_axis[0],
              camera_orientation_axis[1],
              camera_orientation_axis[2]);
    glTranslatef(-camera_position[0],
                -camera_position[1],
                -camera_position[2]);
    // turn arcball rotation matrix into type that can be used with glMultMatrix
    GLfloat *arcball_rotation_data = arcball_rotation.data();
    // FUCK AROUND WITH ORDER TO FIX LIGHT STATIONARY VS ROTATING
    glMultMatrixf(arcball_rotation_data);
    draw_objects();
    // swap buffer
    glutSwapBuffers();
}

/* This function draws each object in objects */
void draw_objects()
{
    glPushMatrix();
    {
        // enable coloring of the material
        glEnable(GL_COLOR_MATERIAL);
        // enabling blend allows transparent objects to be displayed
        glEnable(GL_BLEND);  
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // if wireframe is toggled, draw the wireframe of the superquadric
        // ellipse
        if(wireframe)
        {
            glPushMatrix();
            // translate
            glTranslatef(T[0], T[1], T[2]);
            // rotate
            glRotatef(R[0], R[1], R[2], R[3]);
            // scale
            glScalef(S[0], S[1], S[2]);

            float v_angle = -M_PI / 2;
            float u_angle = -M_PI;
            float v_angle_step = M_PI / (Nv - 1);
            float u_angle_step = 2 * M_PI / Nu;
            // draw wireframe
            for(int v = 0; v < Nv; v++)
            {
                for(int u = 0; u < Nu; u++)
                {
                    u_angle += u_angle_step;
                    Vector4f V00 = psq(u_angle, v_angle);
                    Vector4f V01 = psq(u_angle, v_angle + v_angle_step);
                    Vector4f V10 = psq(u_angle + u_angle_step, v_angle);
                    Vector4f V11 = psq(u_angle + u_angle_step,
                                    v_angle + v_angle_step);
                    if(v != Nv - 1)
                    {
                        glBegin(GL_TRIANGLES);
                            glVertex3f(
                                V00[0] / V00[3],
                                V00[1] / V00[3],
                                V00[2] / V00[3]
                            );
                            glVertex3f(
                                V01[0] / V01[3],
                                V01[1] / V01[3],
                                V01[2] / V01[3]
                            );
                            glVertex3f(
                                V11[0] / V11[3],
                                V11[1] / V11[3],
                                V11[2] / V11[3]
                            );
                        glEnd();
                    }
                    if(v != 0)
                    {
                        glBegin(GL_TRIANGLES);
                            glVertex3f(
                                V00[0] / V00[3],
                                V00[1] / V00[3],
                                V00[2] / V00[3]
                            );
                            glVertex3f(
                                V11[0] / V11[3],
                                V11[1] / V11[3],
                                V11[2] / V11[3]
                            );
                            glVertex3f(
                                V10[0] / V10[3],
                                V10[1] / V10[3],
                                V10[2] / V10[3]
                            );
                        glEnd();
                    }
                }
                v_angle += v_angle_step;
            }

            glPopMatrix();
        }

        // draw small transparent red spheres
        for(int x = 0; x < N; x++)
            for(int y = 0; y < N; y++)
                for(int z = 0; z < N; z++)
                {
                    glPushMatrix();
                    float x_n = (xmax - xmin) * (x + 0.5) / N + xmin;
                    float y_n = (ymax - ymin) * (y + 0.5) / N + ymin;
                    float z_n = (zmax - zmin) * (z + 0.5) / N + zmin;
                    array<int, 3> cur_coord = {x, y, z};
                    if(random_jitter.find(cur_coord) == random_jitter.end())
                    {
                        // randomly "jitter" the points
                        float jitter_x = jitter * (xmax - xmin);
                        float jitter_y = jitter * (ymax - ymin);
                        float jitter_z = jitter * (zmax - zmin);
                        Vector3f* new_random_jitter = new Vector3f;
                        *new_random_jitter <<
                            jitter_x * (0.05 - float(rand() % 100) / 1000),
                            jitter_y * (0.05 - float(rand() % 100) / 1000),
                            jitter_z * (0.05 - float(rand() % 100) / 1000);
                        random_jitter[cur_coord] = new_random_jitter;
                    }
                    // translate to correct coordinates
                    glTranslatef(
                        x_n + (*random_jitter[cur_coord])[0],
                        y_n + (*random_jitter[cur_coord])[1],
                        z_n + (*random_jitter[cur_coord])[2]);
                    // check if the current coordinate in in the
                    // superquadric ellipse
                    if(isq(x_n + (*random_jitter[cur_coord])[0],
                        y_n + (*random_jitter[cur_coord])[1],
                        z_n + (*random_jitter[cur_coord])[2]) > 0)
                    {
                        // if not, make the sphere small
                        glColor4f(1.0, 0.0, 0.0, 0.1);
                        // it is assumed that xmax - xmin is reasonably close to
                        // ymax - ymin and zmax - zmin
                        glutSolidSphere(0.001 * (xmax - xmin), 3, 3);
                    }
                    else
                    {
                        // if yes, make the sphere large
                        glColor4f(1.0, 0.0, 0.0, 0.7);
                        glutSolidSphere(0.008 * (xmax - xmin), 10, 10);
                    }
                    glPopMatrix();
                }

        // draw the external wireframe box boundary
        glColor3f(0.0, 0.0, 0.0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBegin(GL_QUADS);
            // FACE 1
            glVertex3f(1.1 * xmin, 1.1 * ymin, 1.1 * zmin);
            glVertex3f(1.1 * xmax, 1.1 * ymin, 1.1 * zmin);
            glVertex3f(1.1 * xmax, 1.1 * ymax, 1.1 * zmin);
            glVertex3f(1.1 * xmin, 1.1 * ymax, 1.1 * zmin);
        glEnd();
        glBegin(GL_QUADS);
            // FACE 2
            glVertex3f(1.1 * xmin, 1.1 * ymin, 1.1 * zmin);
            glVertex3f(1.1 * xmin, 1.1 * ymax, 1.1 * zmin);
            glVertex3f(1.1 * xmin, 1.1 * ymax, 1.1 * zmax);
            glVertex3f(1.1 * xmin, 1.1 * ymin, 1.1 * zmax);
        glEnd();
        glBegin(GL_QUADS);
            // FACE 3
            glVertex3f(1.1 * xmin, 1.1 * ymin, 1.1 * zmin);
            glVertex3f(1.1 * xmax, 1.1 * ymin, 1.1 * zmin);
            glVertex3f(1.1 * xmax, 1.1 * ymin, 1.1 * zmax);
            glVertex3f(1.1 * xmin, 1.1 * ymin, 1.1 * zmax);
        glEnd();
        glBegin(GL_QUADS);
            // FACE 4
            glVertex3f(1.1 * xmax, 1.1 * ymin, 1.1 * zmax);
            glVertex3f(1.1 * xmax, 1.1 * ymax, 1.1 * zmax);
            glVertex3f(1.1 * xmax, 1.1 * ymax, 1.1 * zmin);
            glVertex3f(1.1 * xmax, 1.1 * ymin, 1.1 * zmin);
        glEnd();
        glBegin(GL_QUADS);
            // FACE 5
            glVertex3f(1.1 * xmax, 1.1 * ymax, 1.1 * zmin);
            glVertex3f(1.1 * xmin, 1.1 * ymax, 1.1 * zmin);
            glVertex3f(1.1 * xmin, 1.1 * ymax, 1.1 * zmax);
            glVertex3f(1.1 * xmax, 1.1 * ymax, 1.1 * zmax);
        glEnd();
    }
    glPopMatrix();
}

float isq(float x, float y, float z)
{
    Vector4f V(x, y, z, 1);
    Matrix4f translation = get_translate_mat(T[0], T[1], T[2]);
    Matrix4f scaling = get_scale_mat(S[0], S[1], S[2]);
    Matrix4f rotation = get_rotate_mat(R[1], R[2], R[3], deg2rad(R[0]));
    V = translation.inverse() * V;
    V = rotation.inverse() * V;
    V = scaling.inverse() * V;
    return pow(pow(pow(V[0] / V[3], 2), 1 / e)
                + pow(pow(V[1] / V[3], 2), 1 / e), e / n) \
        + pow(pow(V[2] / V[3], 2), 1 / n) - 1;
}

Vector4f psq(float u, float v)
{
    Vector4f psq;
    psq << exp_cos(v, n) * exp_cos(u, e),
        exp_cos(v, n) * exp_sin(u, e),
        exp_sin(v, n),
        1;
    return psq;
}

/* This function defines how OpenGL responds on mouse click
 *
 * This function takes four arguments:
 *      - button - the button on the mouse that got clicked or released,
 *               represented by an enum
 *      - state - either 'GLUT_DOWN' or 'GLUT_UP' for specifying whether the
 *              button was pressed down or released up respectively
 *      - x, y - screen coordinates (int)
 */
void mouse_pressed(int button, int state, int x, int y)
{
    // if the left-mouse button was clicked down
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // store the mouse position in our global variables.
        mouse_x = x;
        mouse_y = y;
        // set is_pressed to be true
        is_pressed = true;
    }
    // if the left-mouse button was released up
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        // set is_pressed to be false
        is_pressed = false;
    }
}

/* This function defines how OpenGL responds on mouse movement
 * 
 * This function takes two arguments:
 *      - x, y - screen coordinates (int)
 */
void mouse_moved(int x, int y)
{
    // if pressed
    if(is_pressed)
    {
        if(mouse_x != x && mouse_y != y)
        {
            // get arcball vectors of current mouse location and previous
            // mouse location
            Vector3f v0 = get_arcball_vector(mouse_x, mouse_y);
            Vector3f v1 = get_arcball_vector(x, y);
            // calculate the rotation angle and axis
            float angle = acos(fmin(1.0, v0.dot(v1)));
            Vector3f axis = v0.cross(v1);
            // take into account the orientation of the camera
            Matrix4f camera_rotation = get_rotate_mat(
                camera_orientation_axis[0],
                camera_orientation_axis[1],
                camera_orientation_axis[2],
                deg2rad(camera_orientation_angle));
            axis = camera_rotation.topLeftCorner(3, 3) * axis;
            // calculate new arcball rotation matrix to arcball_rotation
            Matrix4f new_arcball_rot = get_rotate_mat(
                axis[0], axis[1], axis[2], angle);
            // multiple new_arcball_rot to arcball_rotation
            arcball_rotation = new_arcball_rot * arcball_rotation;
            // update mouse_x and mouse_y
            mouse_x = x;
            mouse_y = y;
            // redisplay
            glutPostRedisplay();
        }
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
    if(key == 'q' || key == 'Q')
    {
        exit(0);
    }
    // t for toggle wireframe
    else if(key == 't' || key == 'T')
    {
        wireframe = !wireframe;
        glutPostRedisplay();
    }
}

/* This function returns the vector required for arcball rotation calculation
 *
 * This function takes two arguments:
 *      - x, y - screen coordinates (int)
 */
Vector3f get_arcball_vector(int x, int y)
{
    Vector3f P(float(x) / xres * 2 - 1, float(y) / yres * 2 - 1, 0);
    // account for difference in y coordinates of OpenGL vs screen y coordinates
    P[1] *= -1;
    // find z or normalize in case x and y are too large
    if(P.norm() <= 1)
        P[2] = sqrt(1 - P.squaredNorm());
    else
        P.normalize();
    return P;
}
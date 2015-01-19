/* CS 171 Assignment by JongSeok (Parker) Won */

#include "intersection_test.h"

// global variables defined with their default values
int xres = 1000, yres = 1000;
float e = 1.0, n = 1.0;  // shape exponents
Vector3f S(1.0, 1.0, 1.0);  // scaling vector
Vector3f T(0.0, 0.0, 0.0);  // translation vector
Vector4f R(0.0, 0.0, 0.0, 1.0);  // rotation vector
//          ^theta in this vector is stored in degrees
float xmin = -1.2, ymin = -1.2, zmin = -1.2;
float xmax = 1.2, ymax = 1.2, zmax = 1.2;
float Nv = 20;
float Nu = 10;

// set up Ray with its default values
Ray ray = {
    {5, 0.1, 0.1},   // A
    {-1, 0, 0}       // B
};
// set up ray length
float RAY_LENGTH = 7.0;

// t value at the first intersection of ray and the superquadratic ellipse
float t_ans = FLT_MAX;

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

// arcball matrix
Matrix4f arcball_rotation = Matrix4f::Identity();

// main function
int main(int argc, char* argv[])
{
    if(argc != 3 && argc != 21)
        cout << "usage: " << argv[0] <<
        " <e> <n>" <<
        " [<Sx> <Sy> <Sz> <Tx> <Ty> <Tz> <Rtheta> <Rx> <Ry> <Rz>" <<
        " <minX> <maxX> <minY> <maxY> <minZ> <maxZ>" <<
        " <Nu> <Nv>] and 6 floats via stdin" << endl;
    else
    {
        xres = 1000;
        yres = 1000;
        e = atof(argv[1]);
        n = atof(argv[2]);
        if(argc == 21)
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
            Nu = atof(argv[19]);
            Nv = atof(argv[20]);
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

        // fill ray from standard input
        cin >> ray.A[0];
        cin >> ray.A[1];
        cin >> ray.A[2];
        cin >> ray.B[0];
        cin >> ray.B[1];
        cin >> ray.B[2];
        
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
    calculateIntersection();
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

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glColor3f(0.0, 0.0, 0.0);

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
        // draw wireframe of the globally stored superquadric
        for(int v = 0; v < Nv; v++)
        {
            for(int u = 0; u < Nu; u++)
            {
                u_angle += u_angle_step;
                Vector3f V00 = psq(u_angle, v_angle);
                Vector3f V01 = psq(u_angle, v_angle + v_angle_step);
                Vector3f V10 = psq(u_angle + u_angle_step, v_angle);
                Vector3f V11 = psq(u_angle + u_angle_step,
                                v_angle + v_angle_step);
                if(v != Nv - 1)
                {
                    glBegin(GL_TRIANGLES);
                        glVertex3f(V00[0], V00[1], V00[2]);
                        glVertex3f(V01[0], V01[1], V01[2]);
                        glVertex3f(V11[0], V11[1], V11[2]);
                    glEnd();
                }
                if(v != 0)
                {
                    glBegin(GL_TRIANGLES);
                        glVertex3f(V00[0], V00[1], V00[2]);
                        glVertex3f(V11[0], V11[1], V11[2]);
                        glVertex3f(V10[0], V10[1], V10[2]);
                    glEnd();
                }
            }
            v_angle += v_angle_step;
        }
        glPopMatrix();

        // draw the Ray R
        glPushMatrix();
        // first draw the starting point
        glTranslatef(ray.B[0], ray.B[1], ray.B[2]);
        // with a red dot
        glColor3f(1.0, 0.0, 0.0);
        // it is assumed that xmax - xmin is reasonably close to
        // ymax - ymin and zmax - zmin
        glutSolidSphere(0.008 * (xmax - xmin), 10, 10);
        // then draw a line that represents the ray direction from the dot
        glBegin(GL_LINES);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(
                RAY_LENGTH * ray.A[0],
                RAY_LENGTH * ray.A[1],
                RAY_LENGTH * ray.A[2]);
        glEnd();
        glPopMatrix();

        if(t_ans != FLT_MAX)
        {
            // draw the intersection
            glPushMatrix();
            // with a blue dot
            glColor3f(0.0, 0.0, 1.0);
            Vector3f x_ans = ray.A * t_ans + ray.B;
            glTranslatef(
                x_ans[0],
                x_ans[1],
                x_ans[2]);
            glutSolidSphere(0.008 * (xmax - xmin), 10, 10);
            // then draw the normal vector
            Vector3f intersection_normal = isq_normal(
                                            x_ans[0],
                                            x_ans[1],
                                            x_ans[2]
                                        );
            glBegin(GL_LINES);
                glVertex3f(0.0, 0.0, 0.0);
                glVertex3f(
                    RAY_LENGTH * intersection_normal[0],
                    RAY_LENGTH * intersection_normal[1],
                    RAY_LENGTH * intersection_normal[2]);
            glEnd();
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

/* This function takes a 3D world coordinate and checks if it is inside a
 * globally stored superquadric */
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

/* This function takes a 3D world coordinate and finds the gradient of the isq
 * function at that point */
Vector3f isq_gradient(float x, float y, float z)
{
    Vector4f V4(x, y, z, 1);
    Matrix4f translation = get_translate_mat(T[0], T[1], T[2]);
    Matrix4f scaling = get_scale_mat(S[0], S[1], S[2]);
    Matrix4f rotation = get_rotate_mat(R[1], R[2], R[3], deg2rad(R[0]));
    V4 = translation.inverse() * V4;
    V4 = rotation.inverse() * V4;
    V4 = scaling.inverse() * V4;
    Vector3f V = Vector4fTo3f(V4);
    float temp = pow(pow(abs(V[0]), 2 / e) + pow(abs(V[1]), 2 / e), e / n - 1);
    Vector4f gradient4;
    gradient4 << 2 * V[0] * pow(abs(V[0]), 2 / e - 2) * temp / n,
                2 * V[1] * pow(abs(V[1]), 2 / e - 2) * temp / n,
                2 * V[2] * pow(abs(V[2]), 2 / n - 2) / n,
                1;
    gradient4 = rotation * gradient4;
    Vector3f gradient = Vector4fTo3f(gradient4);
    return gradient;
}

// this function simply returns isq_graident normalized
// helper function for find normals on the surfaces of superquadrics
Vector3f isq_normal(float x, float y, float z)
{
    Vector3f normal = isq_gradient(x, y, z);
    normal.normalize();
    return normal;
}

/* This function turns a superquadric into a parametric function in terms of u
 * and v and returns the point at a specific u and v values */
Vector3f psq(float u, float v)
{
    Vector3f psq;
    psq << exp_cos(v, n) * exp_cos(u, e),
        exp_cos(v, n) * exp_sin(u, e),
        exp_sin(v, n);
    return psq;
}

/* This function finds the smallest value of t at which a ray (globally stored)
 * intersects a globally stored superquadric
 * If there is no intersection FLT_MAX is returned */
int calculateIntersection()
{
    t_ans = rayXBoundingSphere();
    if(t_ans == FLT_MAX)
        return -1;

    float prev_slope = g_slope(t_ans);
    float prev_g = g(t_ans);
    while(g(t_ans) > 0.0001)
    {
        t_ans = t_ans - prev_g / prev_slope;
        if(sign(prev_slope) != sign(g_slope(t_ans)))
            return -1;
        prev_slope = g_slope(t_ans);
        prev_g = g(t_ans);
        if(prev_slope == 0 && prev_g > 0)
            return -1;
    }
    return 1;
}

/* This function finds the value of t at which a ray (globally stored) first
 * hits the bounding sphere of a superquadric (globally stored)
 * If there is no intersection FLT_MAX is returned */
float rayXBoundingSphere()
{
    Matrix4f translation = get_translate_mat(T[0], T[1], T[2]);
    Matrix4f scaling = get_scale_mat(S[0], S[1], S[2]);
    Matrix4f rotation = get_rotate_mat(R[1], R[2], R[3], deg2rad(R[0]));
    // ray.A = translation.topLeftCorner(3, 3).inverse() * ray.A;
    Vector4f temp_ray_A4(ray.A[0], ray.A[1], ray.A[2], 1);
    Vector4f temp_ray_B4(ray.B[0], ray.B[1], ray.B[2], 1);
    temp_ray_A4 = rotation.inverse() * temp_ray_A4;
    temp_ray_A4 = scaling.inverse() * temp_ray_A4;
    temp_ray_B4 = translation.inverse() * temp_ray_B4;
    temp_ray_B4 = rotation.inverse() * temp_ray_B4;
    temp_ray_B4 = scaling.inverse() * temp_ray_B4;
    Vector3f temp_ray_A = Vector4fTo3f(temp_ray_A4);
    Vector3f temp_ray_B = Vector4fTo3f(temp_ray_B4);
    Quadratic Q = {
        temp_ray_A.dot(temp_ray_A),
        2 * temp_ray_A.dot(temp_ray_B),
        temp_ray_B.dot(temp_ray_B) - 3
    };
    return small_ans(Q);
}

// These are helper functions for calculating the actual intersection using
// Newton's Method
float g(float t)
{
    Vector3f x = ray.A * t + ray.B;
    return isq(x[0], x[1], x[2]);
}
float g_slope(float t)
{
    Vector3f x = ray.A * t + ray.B;
    return ray.A.dot(isq_gradient(x[0], x[1], x[2]));
}
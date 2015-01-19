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

#include "ogl_renderer.h"

// global variables
int xres, yres;
IVData data = parse_iv(cin);
Camera &camera = data.camera;
vector<Object> &objects = data.objects;
vector<Point_Light> &lights = data.lights;
int attenuation_k;
float fog_density = 0.2;

// variables for interactive mouse ui
int mouse_x, mouse_y;
float mouse_scale_x, mouse_scale_y;
const float step_size = 0.2;
const float x_view_step = 90.0, y_view_step = 90.0;
float x_view_angle = 0, y_view_angle = 0;

// state variables
bool is_pressed = false;
bool wireframe_mode = false;
bool arcball = true;
bool fog = true;
// arcball_rotate_object = true means only rotate object
bool arcball_rotate_object = true;

// arcball matrix
Matrix4f arcball_rotation = Matrix4f::Identity();

// main function
int main(int argc, char* argv[])
{
    if(argc != 4)
        cout << "usage: " << argv[0] <<
        " <xres> <yres> <k> < <filename>" << endl;
    else
    {
        xres = atoi(argv[1]);
        yres = atoi(argv[2]);
        attenuation_k = atof(argv[3]);
    
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
    glFrustum(camera.left, camera.right,
              camera.bottom, camera.top,
              camera.near, camera.far);
    // switch glMatrixMode back to GL_MODELVIEW
    glMatrixMode(GL_MODELVIEW);
    // fill vertex and normal buffers in objects
    fill_vertex_normal_buffer(objects);
    // initialize lights
    init_lights();
    // setup the fog effect
    GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};
    // you can set the mode to EXP, EXP2 or LINEAR
    glFogi(GL_FOG_MODE, GL_EXP2);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, fog_density);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
    glFogf(GL_FOG_START, 1.0);
    glFogf(GL_FOG_END, 5.0);
    glEnable(GL_FOG);
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
    mouse_scale_x = (float) (camera.right - camera.left) / (float) width;
    mouse_scale_y = (float) (camera.top - camera.bottom) / (float) height;
    // redisplay
    glutPostRedisplay();
}

/* This function defines how each frame is rendered */
void display(void)
{
    // set clear color to fog color if fog is enabled
    if(fog)
        glClearColor(0.3 + 0.2 * fog_density,
                    0.3 + 0.2 * fog_density,
                    0.3 + 0.2 * fog_density,
                    1.0);
    // else set to black
    else
        glClearColor(0, 0, 0, 1.0);
    // clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // reset glMatrix
    glLoadIdentity();
    // rotatation matrix for interactive rotation
    glRotatef(y_view_angle, 1, 0, 0);
    glRotatef(x_view_angle, 0, 1, 0);
    // camera tranformation matrices
    glRotatef(-rad2deg(camera.orientation_angle),
              camera.orientation_axis[0],
              camera.orientation_axis[1],
              camera.orientation_axis[2]);
    glTranslatef(-camera.position[0],
                -camera.position[1],
                -camera.position[2]);
    // turn arcball rotation matrix into type that can be used with glMultMatrix
    GLfloat *arcball_rotation_data = arcball_rotation.data();
    // if you want arcball to rotate the entire scene
    if(!arcball_rotate_object)
    {
        // rotate with arcball before setting lights
        glMultMatrixf(arcball_rotation_data);
        set_lights();
    }
    else
    {
        // rotate with arcball after setting lights
        set_lights();
        glMultMatrixf(arcball_rotation_data);
    }
    draw_objects();
    // swap buffer
    glutSwapBuffers();
}

/* This function sets up all of the Light instances in lights using the built in
 * OpenGL lights. However, the positions of the lights are NOT set in this
 * function
 */
void init_lights()
{
    // enable lighting
    glEnable(GL_LIGHTING);
    // for each light...
    int num_lights = lights.size();
    for(int i = 0; i < num_lights; ++i)
    {
        // set light_id to one of the built in lights
        int light_id = GL_LIGHT0 + i;
        // enable the light
        glEnable(light_id);
        // set color and attenutation
        glLightfv(light_id, GL_AMBIENT, lights[i].color);
        glLightfv(light_id, GL_DIFFUSE, lights[i].color);
        glLightfv(light_id, GL_SPECULAR, lights[i].color);
        glLightf(light_id, GL_QUADRATIC_ATTENUATION, attenuation_k);
    }
}

/* This function positions all of the lights */
void set_lights()
{
    // for each light...
    int num_lights = lights.size();
    for(int i = 0; i < num_lights; ++i)
    {
        int light_id = GL_LIGHT0 + i;
        // set position
        glLightfv(light_id, GL_POSITION, lights[i].position);
    }
}

/* This function draws each object in objects */
void draw_objects()
{
    // for each object...
    int num_objects = objects.size();
    for(int i = 0; i < num_objects; ++i)
    {
        glPushMatrix();
        // draw object
        {
            // for each transform stored in objects[i]
            int num_transform_sets = objects[i].transform_sets.size();
            for(int j = 0; j < num_transform_sets; ++j)
            {
                // translation
                glTranslatef(objects[i].transform_sets[j].translation[0],
                             objects[i].transform_sets[j].translation[1],
                             objects[i].transform_sets[j].translation[2]);
                // rotation
                glRotatef(rad2deg(objects[i].transform_sets[j].rotation_angle),
                          objects[i].transform_sets[j].rotation[0],
                          objects[i].transform_sets[j].rotation[1],
                          objects[i].transform_sets[j].rotation[2]);
                // scaling
                glScalef(objects[i].transform_sets[j].scaling[0],
                         objects[i].transform_sets[j].scaling[1],
                         objects[i].transform_sets[j].scaling[2]);
            }
            // set surface properties
            glMaterialfv(GL_FRONT, GL_AMBIENT, objects[i].ambient_reflect);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, objects[i].diffuse_reflect);
            glMaterialfv(GL_FRONT, GL_SPECULAR, objects[i].specular_reflect);
            glMaterialf(GL_FRONT, GL_SHININESS, objects[i].shininess);
            // provide OpenGL with a pointer to the vertex_buffer of object[i]
            glVertexPointer(3, GL_FLOAT, 0, &objects[i].vertex_buffer[0]);
            // provide OpenGL with a pointer to the normal_buffer of object[i]
            glNormalPointer(GL_FLOAT, 0, &objects[i].normal_buffer[0]);
            // tell OpenGL to draw the object
            int buffer_size = objects[i].vertex_buffer.size();
            if(!wireframe_mode)
                glDrawArrays(GL_TRIANGLES, 0, buffer_size);
            else
                // must draw triangles at a time for wireframe
                for(int j = 0; j < buffer_size; j += 3)
                    glDrawArrays(GL_LINE_LOOP, j, 3);
        }
        glPopMatrix();
    }
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
        // if arcball is enabled
        if(arcball)
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
                    camera.orientation_axis[0],
                    camera.orientation_axis[1],
                    camera.orientation_axis[2],
                    camera.orientation_angle);
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
        // if arcball is disabled
        else
        {
            // update x_view_angle
            x_view_angle += ((float) x - (float) mouse_x) *
                            mouse_scale_x * x_view_step;
            // update y_view_angle
            float temp_y_view_angle = y_view_angle +
                                    ((float) y - (float) mouse_y) *
                                    mouse_scale_y * y_view_step;
            // make sure not to let abs(y_view_angle) above 90
            y_view_angle = abs(temp_y_view_angle) > 90 ?
                           y_view_angle : temp_y_view_angle;
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
    if(key == 'q')
    {
        exit(0);
    }
    // t for toggle wireframe
    else if(key == 't')
    {
        wireframe_mode = !wireframe_mode;
        glutPostRedisplay();
    }
    // b for toggle arcball
    else if(key == 'b')
    {
        arcball = !arcball;
        glutPostRedisplay();
    }
    // o for toggle arcball object rotation
    else if(key == 'o')
    {
        arcball_rotate_object = !arcball_rotate_object;
        glutPostRedisplay();
    }
    // f for toggle fog
    else if(key == 'f')
    {
        fog = !fog;
        if(fog)
            glEnable(GL_FOG);
        else
            glDisable(GL_FOG);
        glutPostRedisplay();
    }
    // g to decrease fog density
    else if(key == 'g')
    {
        fog_density -= 0.05;
        fog_density = fog_density > 0 ? fog_density : 0;
        glFogf(GL_FOG_DENSITY, fog_density);
        glutPostRedisplay();
    }
    // h to increase fog density
    else if(key == 'h')
    {
        fog_density += 0.05;
        fog_density = fog_density < 1 ? fog_density : 1;
        glFogf(GL_FOG_DENSITY, fog_density);
        glutPostRedisplay();
    }
    else
    {
        // we need x_view_angle for determining in which direction to move the
        // camera
        float x_view_rad = deg2rad(x_view_angle);
        // wasd for movement
        if(key == 'w')
        {
            camera.position[0] += step_size * sin(x_view_rad);
            camera.position[2] -= step_size * cos(x_view_rad);
            glutPostRedisplay();
        }
        else if(key == 'a')
        {
            camera.position[0] -= step_size * cos(x_view_rad);
            camera.position[2] -= step_size * sin(x_view_rad);
            glutPostRedisplay();
        }
        else if(key == 's')
        {
            camera.position[0] -= step_size * sin(x_view_rad);
            camera.position[2] += step_size * cos(x_view_rad);
            glutPostRedisplay();
        }
        else if(key == 'd')
        {
            camera.position[0] += step_size * cos(x_view_rad);
            camera.position[2] += step_size * sin(x_view_rad);
            glutPostRedisplay();
        }
    }
}

/* This function fills in vertex and normal buffers in objects
 *
 * This function takes one argument:
 *      - objects - reference to the vector of Objects
 */
void fill_vertex_normal_buffer(vector<Object> &objects)
{
    for(vector<Object>::iterator it_object = objects.begin();
        it_object != objects.end();
        it_object++)
    {
        for(vector< vector<int> >::iterator it = it_object->facesets.begin();
            it != it_object->facesets.end();
            it++)
        {
            for(int i = 0; i < it->size(); i++)
            {
                // fill vertex_buffer
                if(i > 2)
                {
                    // insert first and i-1^th points in faceset in order to cut
                    // faces into triangles
                    Triple point_0 = it_object->points.at(it->at(0));
                    Triple point_neg1 = it_object->points.at(it->at(i - 1));
                    it_object->vertex_buffer.push_back(point_0);
                    it_object->vertex_buffer.push_back(point_neg1);
                }
                Triple point_i = it_object->points.at(it->at(i));
                it_object->vertex_buffer.push_back(point_i);
            }
        }
    }
    for(vector<Object>::iterator it_object = objects.begin();
        it_object != objects.end();
        it_object++)
    {
        for(vector< vector<int> >::iterator it = it_object->normal_sets.begin();
            it != it_object->normal_sets.end();
            it++)
        {
            for(int i = 0; i < it->size(); i++)
            {
                // fill normal_buffer
                if(i > 2)
                {
                    // insert first and i-1^th normals in faceset in order to
                    // cut faces into triangles
                    Triple normal_0 = it_object->normals.at(it->at(0));
                    Triple normal_neg1 = it_object->normals.at(it->at(i - 1));
                    it_object->normal_buffer.push_back(normal_0);
                    it_object->normal_buffer.push_back(normal_neg1);
                }
                Triple normal_i = it_object->normals.at(it->at(i));
                it_object->normal_buffer.push_back(normal_i);
            }
        }
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
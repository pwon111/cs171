#include <GL/glew.h>
#include <GL/glut.h>

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

struct Point
{
    float x;
    float y;
};

struct Color
{
    float r;
    float g;
    float b;
};

struct Spring_Pendulum
{
    float m; // mass
    
    float x;
    float y;
    
    float px; // x-momentum
    float py; // y-momentum
    
    float k; // spring constant
    float rl; // rest length of spring
};

const float cam_position[] = {0, 0, 2};

const float near_param = 1, far_param = 4,
            left_param = -10, right_param = 10,
            top_param = 10, bottom_param = -10;

const float dt = 0.01; // our time step
const float g = -0.0; // acceleration of gravity
const int max_num_points = 4000; // max number of points to keep in our traced paths
bool draw_system = true; // to indicate whether to draw the masses and "springs"
float lagrangian_0; // initial value of Lagrangian for color value calculations

Spring_Pendulum m1;
Spring_Pendulum m2;

vector<Point> path1;
vector<Point> path2;
vector<Color> path_colors1;
vector<Color> path_colors2;

void trace_path();
void draw_spring_pendulums();

void init(void)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // We use glOrtho instead of glFrustum, since we're only doing 2D displays.
    glOrtho(left_param, right_param,
            bottom_param, top_param,
            near_param, far_param);
    
    glMatrixMode(GL_MODELVIEW);
}

void reshape(int width, int height)
{
    height = (height == 0) ? 1 : height;
    width = (width == 0) ? 1 : width;
    
    glViewport(0, 0, width, height);
    
    glutPostRedisplay();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(-cam_position[0], -cam_position[1], -cam_position[2]);
    
    trace_path();
    
    if(draw_system)
        draw_spring_pendulums();
    
    glutSwapBuffers();
}

float compute_lagrangian()
{
    float ke = 1.0 / 2.0 * m1.m * ((m1.px / m1.m) * (m1.px / m1.m) + (m1.py / m1.m) * (m1.py / m1.m)) +
               1.0 / 2.0 * m2.m * ((m2.px / m2.m) * (m2.px / m2.m) + (m2.py / m2.m) * (m2.py / m2.m));
    float pe = 1.0 / 2.0 * m1.k * (sqrt(m1.x * m1.x + m1.y * m1.y) - m1.rl) *
                                  (sqrt(m1.x * m1.x + m1.y * m1.y) - m1.rl) +
               1.0 / 2.0 * m2.k * (sqrt((m1.x - m2.x) * (m1.x - m2.x) + (m1.y - m2.y) * (m1.y - m2.y)) - m2.rl) *
                                  (sqrt((m1.x - m2.x) * (m1.x - m2.x) + (m1.y - m2.y) * (m1.y - m2.y)) - m2.rl) -
               m1.m * g * m1.y -
               m2.m * g * m2.y;
    
    cout << "KE: " << ke << ", PE: " << pe << ", Total Energy: " << ke + pe << endl;
    
    return ke - pe;
}

void update_path()
{    
    // Delete the oldest point in each path when the paths get too big to prevent slowdown.
    if(path1.size() == max_num_points)
    {
        path1.erase(path1.begin());
        path2.erase(path2.begin());
        
        path_colors1.erase(path_colors1.begin());
        path_colors2.erase(path_colors2.begin());
    }
    
    Point point1;
    point1.x = m1.x;
    point1.y = m1.y;
    
    Point point2;
    point2.x = m2.x;
    point2.y = m2.y;
    
    // Compute a "normalized" Lagrangian value by dividing the current Lagrangian by the
    // initial value of the Lagrangian. This isn't exactly a normalization, but it suffices
    // for the color computations.
    float lagrangian_norm = abs(compute_lagrangian() / lagrangian_0);
    lagrangian_norm = (lagrangian_norm > 1.0) ? 1.0 : lagrangian_norm;
    
    Color color1;
    color1.r = lagrangian_norm;
    color1.g = lagrangian_norm;
    color1.b = 1.0 - lagrangian_norm;
    
    Color color2;
    color2.r = 1.0 - lagrangian_norm;
    color2.g = lagrangian_norm;
    color2.b = lagrangian_norm;
    
    path1.push_back(point1);
    path_colors1.push_back(color1);
    
    path2.push_back(point2);
    path_colors2.push_back(color2);
}

void update_pendulums()
{
    // ### TODO ###
    // This currently uses an explicit Euler integrator. Please change this to a
    // variational time integrator. When you have successfully changed this to a
    // variational time integrator, you may want to bump the time step up from
    // 0.004 to 0.01 to allow the animation to go faster.

    float temp1 = sqrt(m1.x * m1.x + m1.y * m1.y);
    float temp2 = sqrt((m1.x - m2.x) * (m1.x - m2.x) + (m1.y - m2.y) * (m1.y - m2.y));
    
    m1.px += dt * (m1.k * m1.x * (m1.rl / temp1 - 1) + m2.k * (m1.x - m2.x) * (m2.rl / temp2 - 1));
    m1.py += dt * (m1.k * m1.y * (m1.rl / temp1 - 1) + m2.k * (m1.y - m2.y) * (m2.rl / temp2 - 1) + m1.m * g);
    m2.px += dt * (-m2.k * (m1.x - m2.x) * (m2.rl / temp2 - 1));
    m2.py += dt * (-m2.k * (m1.y - m2.y) * (m2.rl / temp2 - 1) + m2.m * g);
    
    m1.x += m1.px / m1.m * dt;
    m1.y += m1.py / m1.m * dt;
    
    m2.x += m2.px / m2.m * dt;
    m2.y += m2.py / m2.m * dt;
}

void update()
{
    update_path();
    update_pendulums();
    
    glutPostRedisplay();
}

void trace_path()
{
    glVertexPointer(2, GL_FLOAT, 0, &path1[0]);
    glColorPointer(3, GL_FLOAT, 0, &path_colors1[0]);
    glDrawArrays(GL_POINTS, 0, path1.size());
    
    glVertexPointer(2, GL_FLOAT, 0, &path2[0]);
    glColorPointer(3, GL_FLOAT, 0, &path_colors2[0]);
    glDrawArrays(GL_POINTS, 0, path2.size());
}

void draw_spring_pendulums()
{
    glColor3f(1, 1, 1);
    
    glutSolidCube(0.2);
    
    glPushMatrix();
    {
        glTranslatef(m1.x, m1.y, 0);
        glutSolidSphere(0.1, 20, 20);
    }
    glPopMatrix();
    
    glPushMatrix();
    {
        glTranslatef(m2.x, m2.y, 0);
        glutSolidSphere(0.1, 20, 20);
    }
    glPopMatrix();
    
    glBegin(GL_LINES);
    glVertex2f(0, 0);
    glVertex2f(m1.x, m1.y);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex2f(m1.x, m1.y);
    glVertex2f(m2.x, m2.y);
    glEnd();
}

void key_pressed(unsigned char key, int x, int y)
{
    if(key == 'q')
    {
        exit(0);
    }
    else if(key == 't')
    {
        draw_system = !draw_system;
    }
}

int main(int argc, char* argv[])
{
    if(argc != 7)
    {
        cerr << "\nERROR: Incorrect number of arguments." << endl;
        exit(1);
    }
    
    int xres = atoi(argv[1]);
    int yres = atoi(argv[2]);
    
    m1.m = 1;
    m1.x = atof(argv[3]);
    m1.y = atof(argv[4]);
    m1.px = 0;
    m1.py = 0;
    m1.k = 20;
    m2.rl = 1;
    
    m2.m = 1;
    m2.x = atof(argv[5]);
    m2.y = atof(argv[6]);
    m2.px = 0;
    m2.py = 0;
    m2.k = 20;
    m2.rl = 1;
    
    lagrangian_0 = compute_lagrangian();
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(xres, yres);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Double Spring Pendulum");
    
    init();
    glutDisplayFunc(display);
    glutIdleFunc(update);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key_pressed);
    glutMainLoop();
}

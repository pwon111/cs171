// This includes more headers than you will probably need.
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <utility> // pair
#include <list>
#include <vector>
#include <array>
#include "data.h"
#include <math.h>
#include <limits>
#include <fstream>
#include <algorithm> //swap
#include <cassert>

#include <Eigen/Eigen>

#include "util.h"
#include "superquadrics.h"

using namespace std;
using namespace Eigen;


// prototype functions
void parse(Mesh * A, istream &datafile);
GLuint loadBMP_custom(const char * imagepath);

// half edge structure
typedef struct HE {
    struct HE * brother; // the HE in the opposite direction.
    struct HEF * adj_face; // the face that this half edge originates from
    struct HE * next; // the HE that is 'out' of 'vertex' and has the same 'adj_face'
    struct HEV * vertex; // the vertex this HE points to
} HE;

//half edge face
typedef struct HEF {
    int a,b,c; // useful for helping the initial loading process.
    struct HE * adj; // one of the three HEs that surround a face, picked randomly
    int oriented; // also useful for the loading. marks if this face was oriented or not.
} HEF;

//half edge vertex
typedef struct HEV {
    float x;
    float y;
    float z;
    struct HE * out; // one of the HEs coming out of this vertex.
} HEV;

// vector. Feel free to use or ignore it.
typedef struct Tvec {
    float x,y,z;

} Tvec;

struct Point_Light
{
    float position[4];
    float color[3];
    float attenuation_k;
};

/* This function takes a half edge face and loads all three half edges of the
 * half edge face into the half_edges map. It orients each face arbitrarily.
 *
 * This function takes one argument:
 *      - hef - the face that we are trying to load the half edges of
 */
void loadHE(HEF* hef);

/* This function takes a half edge face and an anchor half edge face and orients
 * the face so that the anchor points in the opposite direction of its brother
 *
 * This function takes two arguments:
 *      - hef - the face we are trying to orient
 *      - anchor - the anchor half edge
 */
bool orient(HEF* hef, HE* anchor);

/* This function checks if two half edge vertices have the same x, y and z. This
 * function is just a useful utility function */
bool equivalent(HEV* hev1, HEV* hev2);

/* This function takes a half edge and calculates the normal at the vertex that
 * the half edge points to.
 *
 * This function takes one argument:
 *      - anchor - the edge that points to the vertex in question
 */
Vector3f* calculateNormal(HE* anchor);

// gl prototype functions
void init(void);
void reshape(int width, int height);
void display(void);

void init_lights();
void set_lights();
void draw_coordinate_axis();
void draw_objects();
void initTextures();

void draw_rect_prism(float x, float y, float z);
void draw_superquadrics();

void mouse_pressed(int button, int state, int x, int y);
void mouse_moved(int x, int y);
void key_pressed(unsigned char key, int x, int y);

void create_lights();
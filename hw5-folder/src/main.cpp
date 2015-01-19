#include "main.hpp"

//gl globals
GLUquadricObj *quadratic;
GLuint image;

int mouse_x, mouse_y;
float mouse_scale_x, mouse_scale_y;

const float x_view_step = 90.0, y_view_step = 90.0;
float x_view_angle = 0, y_view_angle = 0;

bool is_pressed = false;

float cam_position[] = {0, 0, 2};

float cam_orientation_axis[] = {1, 1, 1};

float cam_orientation_angle = 0; // degrees

float near_param = 1, far_param = 10,
      left_param = -1, right_param = 1,
      top_param = 1, bottom_param = -1;
vector<Point_Light> lights;

// global variables for the parsed data
vector<point*> * points;
vector<face*> * faces;
map< array<int, 2>, HE*> half_edges;
vector<HEF*> half_edge_faces;
map< array<float, 3>, Vector3f*> normals;


//draws shaded objects. takes in iv file, writes in ppm format to Canvas.txt. 
int main(int argc, char ** argv)
{
    // parse data
    Mesh * containerP = new Mesh;
    printf("Parsing...\t\t\t");
    parse(containerP,cin);
    cout << "FINISHED" << endl;

    // load up the global vectors that store the points and the faces
    points = containerP->points;
    faces = containerP->faces;
    
    // load the data into a half edge datastructure
    cout << "loading half edges..." << "\t\t";
    for(vector<face*>::iterator it = faces->begin(); it != faces->end(); it++)
    {
        // create and load a new half edge face
        HEF* temp = new HEF;
        temp->a = (*it)->ind1;  // a
        temp->b = (*it)->ind2;  // b
        temp->c = (*it)->ind3;  // c
        temp->adj = 0;
        temp->oriented = 0;
        half_edge_faces.push_back(temp);
        // load all the half edges of the half edge face
        loadHE(temp);
    }
    cout << "FINISHED" << endl;

    // orient the half edge faces
    cout << "orienting..." << "\t\t\t";
    // "orient" the first face
    half_edge_faces.back()->oriented = 1;
    // find first edge with a brother
    HE* anchor = half_edge_faces.back()->adj;
    while(anchor->brother == 0)
        anchor = anchor->next;
    // orient all faces with the first face as the reference
    bool orientable = orient(anchor->brother->adj_face, anchor->brother);
    if(!orientable)
    {
        cout << "The given surface is not orientable" << endl;
        exit(0);
    }
    else
        cout << "FINISHED" << endl;
    
    //use opengl stuff --------------------------------------
    int xres = 1000;
    int yres = 1000;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(xres, yres);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Object");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse_pressed);
    glutMotionFunc(mouse_moved);
    glutKeyboardFunc(key_pressed);
    glutMainLoop();

    // free all memory
    delete containerP;
    half_edges.clear();
    half_edge_faces.clear();
    normals.clear();

    return 0;
}


/* This function takes a half edge face and loads all three half edges of the
 * half edge face into the half_edges map. It orients each face arbitrarily.
 *
 * This function takes one argument:
 *      - hef - the face that we are trying to load the half edges of
 */
void loadHE(HEF* hef)
{
    // initialize each new half edge
    HE* new_HE1 = new HE;
    HE* new_HE2 = new HE;
    HE* new_HE3 = new HE;
    // define each new half edge's adj_faces
    new_HE1->adj_face = hef;
    new_HE2->adj_face = hef;
    new_HE3->adj_face = hef;
    // link the half edges with the next parameter
    new_HE1->next = new_HE2;
    new_HE2->next = new_HE3;
    new_HE3->next = new_HE1;
    // set up half edge vertices (a, b, c are 1-indexed)
    HEV* hev_a = new HEV;
    HEV* hev_b = new HEV;
    HEV* hev_c = new HEV;
    // hev_a
    hev_a->x = points->at(hef->a - 1)->x;
    hev_a->y = points->at(hef->a - 1)->y;
    hev_a->z = points->at(hef->a - 1)->z;
    hev_a->out = new_HE2;
    // hev_b
    hev_b->x = points->at(hef->b - 1)->x;
    hev_b->y = points->at(hef->b - 1)->y;
    hev_b->z = points->at(hef->b - 1)->z;
    hev_b->out = new_HE3;
    // hev_c
    hev_c->x = points->at(hef->c - 1)->x;
    hev_c->y = points->at(hef->c - 1)->y;
    hev_c->z = points->at(hef->c - 1)->z;
    hev_c->out = new_HE1;
    // set the vertex parameter for each half edge
    new_HE1->vertex = hev_b;
    new_HE2->vertex = hev_c;
    new_HE3->vertex = hev_a;
    // load each half edge to half_edges if it is not already in the map
    // if already in half_edge, set up brother relation between the appropriate
    // half edges
    array<int, 2> ordered_HE_key1 = {min(hef->a, hef->b), max(hef->a, hef->b)};
    array<int, 2> ordered_HE_key2 = {min(hef->b, hef->c), max(hef->b, hef->c)};
    array<int, 2> ordered_HE_key3 = {min(hef->c, hef->a), max(hef->c, hef->a)};
    // set every new half edges' brother to default of 0
    new_HE1->brother = 0;
    new_HE2->brother = 0;
    new_HE3->brother = 0;
    // load new_HE1
    if(half_edges.find(ordered_HE_key1) == half_edges.end())
        half_edges[ordered_HE_key1] = new_HE1;
    else
    {
        new_HE1->brother = half_edges[ordered_HE_key1];
        half_edges[ordered_HE_key1]->brother = new_HE1;
        array<int, 2> reversed_HE_key1 = {ordered_HE_key1[1],
                                        ordered_HE_key1[0]};
        half_edges[reversed_HE_key1] = new_HE1;
    }
    // load new_HE2
    if(half_edges.find(ordered_HE_key2) == half_edges.end())
        half_edges[ordered_HE_key2] = new_HE2;
    else
    {
        new_HE2->brother = half_edges[ordered_HE_key2];
        half_edges[ordered_HE_key2]->brother = new_HE2;
        array<int, 2> reversed_HE_key2 = {ordered_HE_key2[1],
                                        ordered_HE_key2[0]};
        half_edges[reversed_HE_key2] = new_HE1;
    }
    // load new_HE3
    if(half_edges.find(ordered_HE_key3) == half_edges.end())
        half_edges[ordered_HE_key3] = new_HE3;
    else
    {
        new_HE3->brother = half_edges[ordered_HE_key3];
        half_edges[ordered_HE_key3]->brother = new_HE3;
        array<int, 2> reversed_HE_key3 = {ordered_HE_key3[1],
                                        ordered_HE_key3[0]};
        half_edges[reversed_HE_key3] = new_HE1;
    }
    hef->adj = new_HE1;
}
;

/* This function takes a half edge face and an anchor half edge face and orients
 * the face so that the anchor points in the opposite direction of its brother
 *
 * This function takes two arguments:
 *      - hef - the face we are trying to orient
 *      - anchor - the anchor half edge
 */
bool orient(HEF* hef, HE* anchor)
{
    // cout << anchor->brother->vertex << endl;
    if(equivalent(anchor->vertex, anchor->brother->vertex))
    {
        // flip orientation
        HE temp = *(anchor); // copy of anchor useful for the flip
        // first set all of the nexts
        anchor->next = anchor->next->next;
        temp.next->next->next = temp.next;
        temp.next->next = anchor;
        // then set all of the vertices
        anchor->vertex = anchor->next->vertex;
        anchor->next->vertex = anchor->next->next->vertex;
        anchor->next->next->vertex = temp.vertex;
        // then set all of the outs for each vertex
        anchor->vertex->out = anchor->next;
        anchor->next->vertex->out = anchor->next->next;
        anchor->next->next->vertex->out = anchor;
    }
    // now hef is oriented correctly
    hef->oriented = 1;
    // recursively call orient on the faces of the brothers of two non-anchor
    // half edges
    if(anchor->next->brother != 0)
        if(anchor->next->brother->adj_face->oriented == 0)
            return orient(anchor->next->brother->adj_face,
                        anchor->next->brother);
        else    // if it is oriented, check to make sure the orientations match
            if(equivalent(anchor->next->vertex, anchor->next->brother->vertex))
                return false;
    if(anchor->next->next->brother != 0)
        if(anchor->next->next->brother->adj_face->oriented == 0)
            return orient(anchor->next->next->brother->adj_face,
                anchor->next->next->brother);
        else    // if it is oriented, check to make sure the orientations match
            if(equivalent(anchor->next->next->vertex,
                anchor->next->next->brother->vertex))
                return false;
    return true; // if all orients are didn't return false, the surface was
                // orientable
};

/* This function checks if two half edge vertices have the same x, y and z. This
 * function is just a useful utility function */
bool equivalent(HEV* hev1, HEV* hev2)
{
    // cout << hev1->x << ", " << hev1->y << ", " << hev1->z << endl;
    if(hev1->x == hev2->x &&
        hev1->y == hev2->y &&
        hev1->z == hev2->z)
        return true;
    return false;
}

GLuint loadBMP_custom(const char * imagepath)
{
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;   // Position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;   // = width*height*3
    // Actual RGB data
    unsigned char * data;

    // Open the file
    printf("texture filename: %s\n",imagepath);
    FILE * file = fopen(imagepath,"rb");
    if (!file)
    {
        printf("Image could not be opened\n");
        return 0;
    }

    if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
        printf("Not a correct BMP file\n");
        return false;
    }

    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file\n");
        return 0;
    }

    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    // Create a buffer
    data = new unsigned char [imageSize];
     
    // Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);
     
    //Everything is in memory now, the file can be closed
    fclose(file);

    //------ finished loading bmp file. important variables: width, height, data

    // set up textures
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        width, height,
        0,
        GL_BGR,
        GL_UNSIGNED_BYTE,
        data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    return textureID;
}

void initTextures()
{
    // image = loadBMP_custom("../data/strawberry.bmp");
    image = loadBMP_custom("../data/star.bmp");
}

void init(void)
{
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(left_param, right_param,
              bottom_param, top_param,
              near_param, far_param);
    glMatrixMode(GL_MODELVIEW); 

    quadratic = gluNewQuadric();
    gluQuadricNormals(quadratic, GLU_SMOOTH);
    // gluQuadricTexture(quadratic, GL_TRUE);

    // initTextures();

    create_lights();
    init_lights();
}

void reshape(int width, int height)
{
    height = (height == 0) ? 1 : height;
    width = (width == 0) ? 1 : width;
    glViewport(0, 0, width, height);
    mouse_scale_x = (float) (right_param - left_param) / (float) width;
    mouse_scale_y = (float) (top_param - bottom_param) / (float) height;
    glutPostRedisplay();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(-cam_orientation_angle,
              cam_orientation_axis[0], cam_orientation_axis[1], cam_orientation_axis[2]);
    glTranslatef(-cam_position[0], -cam_position[1], -cam_position[2]);
    set_lights();
    glEnable(GL_TEXTURE_2D);
    draw_coordinate_axis();
    draw_objects();
    glutSwapBuffers();
}

void init_lights()
{
    glEnable(GL_LIGHTING);
    int num_lights = lights.size();
    for(int i = 0; i < num_lights; ++i)
    {
        int light_id = GL_LIGHT0 + i;
        glEnable(light_id);
        glLightfv(light_id, GL_AMBIENT, lights[i].color);
            glLightfv(light_id, GL_DIFFUSE, lights[i].color);
            glLightfv(light_id, GL_SPECULAR, lights[i].color);
        glLightf(light_id, GL_QUADRATIC_ATTENUATION, lights[i].attenuation_k);
    }
}

void draw_coordinate_axis()
{
    // set diffuse and specular effect to 0 for the axis
    float diffuse_clr[3], specular_clr[3];
    // diffuse
    diffuse_clr[0] = 0.0;
    diffuse_clr[1] = 0.0;
    diffuse_clr[2] = 0.0;
    // specular
    specular_clr[0] = 0.0;
    specular_clr[1] = 0.0;
    specular_clr[2] = 0.0;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_clr);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_clr);
    // set color for the x axis
    float x_ambient_clr[3];
    // ambient color of x
    x_ambient_clr[0] = 1.0;
    x_ambient_clr[1] = 0.0;
    x_ambient_clr[2] = 0.0;
    // load the colors
    glMaterialfv(GL_FRONT, GL_AMBIENT, x_ambient_clr);
    // draw x axis
    glPushMatrix();
    {
        glRotatef(x_view_angle, 0, 1, 0);
        glRotatef(y_view_angle, 1, 0, 0);
        // draw a thin rectangular prism for axis
        draw_rect_prism(2.0, 0.01, 0.01);
        // draw the positive tip with a sphere
        glTranslatef(1.0, 0.0, 0.0);
        glutSolidSphere(0.01, 10, 10);
    }
    glPopMatrix();

    // set color for the y axis
    float y_ambient_clr[3];
    // ambient color of x
    y_ambient_clr[0] = 0.0;
    y_ambient_clr[1] = 1.0;
    y_ambient_clr[2] = 0.0;
    // load the colors
    glMaterialfv(GL_FRONT, GL_AMBIENT, y_ambient_clr);
    // draw x axis
    glPushMatrix();
    {
        glRotatef(x_view_angle, 0, 1, 0);
        glRotatef(y_view_angle, 1, 0, 0);
        // draw a thin rectangular prism for axis
        draw_rect_prism(0.01, 2.0, 0.01);
        // draw the positive tip with a sphere
        glTranslatef(0.0, 1.0, 0.0);
        glutSolidSphere(0.01, 10, 10);
    }
    glPopMatrix();

    // set color for the y axis
    float z_ambient_clr[3];
    // ambient color of x
    z_ambient_clr[0] = 0.0;
    z_ambient_clr[1] = 0.0;
    z_ambient_clr[2] = 1.0;
    // load the colors
    glMaterialfv(GL_FRONT, GL_AMBIENT, z_ambient_clr);
    // draw x axis
    glPushMatrix();
    {
        glRotatef(x_view_angle, 0, 1, 0);
        glRotatef(y_view_angle, 1, 0, 0);
        // draw a thin rectangular prism for axis
        draw_rect_prism(0.01, 0.01, 2.0);
        // draw the positive tip with a sphere
        glTranslatef(0.0, 0.0, 1.0);
        glutSolidSphere(0.01, 10, 10);
    }
    glPopMatrix();
}

void draw_rect_prism(float x, float y, float z)
{
    glBegin(GL_QUADS);
        // negative tip
        glVertex3f(-x / 2, y / 2, z / 2);
        glVertex3f(-x / 2, -y / 2, z / 2);
        glVertex3f(-x / 2, -y / 2, -z / 2);
        glVertex3f(-x / 2, y / 2, -z / 2);
        // side 1
        glVertex3f(-x / 2, y / 2, z / 2);
        glVertex3f(x / 2, y / 2, z / 2);
        glVertex3f(x / 2, -y / 2, z / 2);
        glVertex3f(-x / 2, -y / 2, z / 2);
        // side 2
        glVertex3f(-x / 2, y / 2, z / 2);
        glVertex3f(x / 2, y / 2, z / 2);
        glVertex3f(x / 2, y / 2, -z / 2);
        glVertex3f(-x / 2, y / 2, -z / 2);
        // side 3
        glVertex3f(-x / 2, -y / 2, -z / 2);
        glVertex3f(x / 2, -y / 2, -z / 2);
        glVertex3f(x / 2, -y / 2, z / 2);
        glVertex3f(-x / 2, -y / 2, z / 2);
        // side 4
        glVertex3f(-x / 2, -y / 2, -z / 2);
        glVertex3f(x / 2, -y / 2, -z / 2);
        glVertex3f(x / 2, y / 2, -z / 2);
        glVertex3f(-x / 2, y / 2, -z / 2);
        // positive tip
        glVertex3f(x / 2, y / 2, z / 2);
        glVertex3f(x / 2, -y / 2, z / 2);
        glVertex3f(x / 2, -y / 2, -z / 2);
        glVertex3f(x / 2, y / 2, -z / 2);
    glEnd();
}

void draw_superquadrics()
{
    Superquadric x_axis;
    {
        x_axis.T << 0.0, 0.0, 0.0;
        x_axis.S << 1.0, 0.1, 0.1;
        x_axis.R << 0.0, 1.0, 0.0, 0.0;
        x_axis.e = 2;
        x_axis.n = 0.01;
    }
    // glTranslatef(x_axis.T[0], x_axis.T[1], x_axis.T[2]);
    // glScale
    int Nv = 20;
    int Nu = 20;
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
            Vector3f V00 = psq(u_angle, v_angle, &x_axis);
            Vector3f V01 = psq(u_angle, v_angle + v_angle_step, &x_axis);
            Vector3f V10 = psq(u_angle + u_angle_step, v_angle, &x_axis);
            Vector3f V11 = psq(u_angle + u_angle_step,
                            v_angle + v_angle_step,
                            &x_axis);
            if(v != Nv - 1)
            {
                glBegin(GL_TRIANGLES);
                    glVertex3f(
                        V00[0] * x_axis.S[0],
                        V00[1] * x_axis.S[1],
                        V00[2] * x_axis.S[2]
                    );
                    glVertex3f(
                        V01[0] * x_axis.S[0],
                        V01[1] * x_axis.S[1],
                        V01[2] * x_axis.S[2]
                    );
                    glVertex3f(
                        V11[0] * x_axis.S[0],
                        V11[1] * x_axis.S[1],
                        V11[2] * x_axis.S[2]
                    );
                glEnd();
            }
            if(v != 0)
            {
                glBegin(GL_TRIANGLES);
                    glVertex3f(
                        V00[0] * x_axis.S[0],
                        V00[1] * x_axis.S[1],
                        V00[2] * x_axis.S[2]
                    );
                    glVertex3f(
                        V11[0] * x_axis.S[0],
                        V11[1] * x_axis.S[1],
                        V11[2] * x_axis.S[2]
                    );
                    glVertex3f(
                        V10[0] * x_axis.S[0],
                        V10[1] * x_axis.S[1],
                        V10[2] * x_axis.S[2]
                    );
                glEnd();
            }
        }
        v_angle += v_angle_step;
    }
}

void draw_objects()
{
    float colrs1[3]; float colrs2[3]; float colrs3[3];
    colrs1[0]=.5; colrs1[1]=.5; colrs1[2]=.5;
    glMaterialfv(GL_FRONT, GL_AMBIENT, colrs1);

    colrs2[0]=.9; colrs2[1]=.9; colrs2[2]=.9;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, colrs2);

    colrs3[0]=.3; colrs3[1]=.3; colrs3[2]=.3;
    glMaterialfv(GL_FRONT, GL_SPECULAR, colrs3);

    glMaterialf(GL_FRONT, GL_SHININESS, 8);

    glPushMatrix();

    glRotatef(x_view_angle, 0, 1, 0);
    glRotatef(y_view_angle, 1, 0, 0);

    // draw object
    for(vector<HEF*>::iterator it = half_edge_faces.begin();
        it != half_edge_faces.end();
        it++)
    {
        // if((*it)->adj->next->next->brother->vertex == (*it)->adj->next->next->vertex)
        //     cout << "broken" << endl;
        // cout << (*it)->adj << endl;
        glBegin(GL_TRIANGLES);
        array<float, 3> v1 = {  // vertex 1
            (*it)->adj->vertex->x,
            (*it)->adj->vertex->y,
            (*it)->adj->vertex->z
        };
        array<float, 3> v2 = {  // vertex 1
            (*it)->adj->next->vertex->x,
            (*it)->adj->next->vertex->y,
            (*it)->adj->next->vertex->z
        };
        array<float, 3> v3 = {  // vertex 1
            (*it)->adj->next->next->vertex->x,
            (*it)->adj->next->next->vertex->y,
            (*it)->adj->next->next->vertex->z
        };
        Vector3f* n1;
        Vector3f* n2;
        Vector3f* n3;
        // make sure the normal at every vertex is the same
        if(normals.find(v1) == normals.end())
        {
            n1 = calculateNormal((*it)->adj);
            normals[v1] = n1;
        }
        else
            n1 = normals[v1];
        if(normals.find(v2) == normals.end())
        {
            n2 = calculateNormal((*it)->adj->next);
            normals[v2] = n2;
        }
        else
            n2 = normals[v2];
        if(normals.find(v3) == normals.end())
        {
            n3 = calculateNormal((*it)->adj->next->next);
            normals[v3] = n3;
        }
        else
            n3 = normals[v3];
        // feed all vertices and normals to OpenGL
        glTexCoord2d(0, 0.73);
        glNormal3f((*n1)[0], (*n1)[1], (*n1)[2]);
        glVertex3f(v1[0], v1[1], v1[2]);
        glTexCoord2d(0.73, 0);
        glNormal3f((*n2)[0], (*n2)[1], (*n2)[2]);
        glVertex3f(v2[0], v2[1], v2[2]);
        glTexCoord2d(0, 0);
        glNormal3f((*n3)[0], (*n3)[1], (*n3)[2]);
        glVertex3f(v3[0], v3[1], v3[2]);
        glEnd();
    }
    
    glPopMatrix();
}

/* This function takes a half edge and calculates the normal at the vertex that
 * the half edge points to.
 *
 * This function takes one argument:
 *      - anchor - the edge that points to the vertex in question
 */
Vector3f* calculateNormal(HE* anchor)
{
    Vector3f* ans = new Vector3f(0, 0, 0);
    HE* temp1;
    HE* temp2;
    temp1 = anchor;
    temp2 = anchor->next;
    while(true)
    {
        // if either of the brothers of temp1 and temp2 are 0, then the vertex
        // in question is on an edge
        if(temp1->brother == 0 || temp2->brother == 0)
            return new Vector3f(0, 0, 0);
        Vector3f vec1(
            temp2->next->vertex->x - temp1->vertex->x,
            temp2->next->vertex->y - temp1->vertex->y,
            temp2->next->vertex->z - temp1->vertex->z);
        Vector3f vec2(
            temp2->vertex->x - temp1->vertex->x,
            temp2->vertex->y - temp1->vertex->y,
            temp2->vertex->z - temp1->vertex->z);
        // the cross product is proportional to the area of the face while
        // pointing in the same direction as the normal
        (*ans) += vec2.cross(vec1);
        // if the loop has been completed around the vertex in question, break
        if(temp2->brother == anchor)
            break;  
        // go to the next face around the vertex in question
        temp1 = temp2->brother;
        temp2 = temp1->next;
    }
    // cause random perturbations
    // (*ans)[0] *= (rand() % 10000) * 0.0000000000000000001;
    // (*ans)[1] *= (rand() % 10000) * 0.0000000000000000001;
    // (*ans)[2] *= (rand() % 10000) * 0.0000000000000000001;
    // normalize
    ans->normalize();
    return ans;
}

void set_lights()
{
    int num_lights = lights.size();
    
    for(int i = 0; i < num_lights; ++i)
    {
        int light_id = GL_LIGHT0 + i;
        
        glLightfv(light_id, GL_POSITION, lights[i].position);
    }
}
void mouse_pressed(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        mouse_x = x;
        mouse_y = y;
        is_pressed = true;
    }
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        is_pressed = false;
    }
}
void mouse_moved(int x, int y)
{
    if(is_pressed)
    {
        x_view_angle += ((float) x - (float) mouse_x) * mouse_scale_x * x_view_step;
        y_view_angle += ((float) y - (float) mouse_y) * mouse_scale_y * y_view_step;

        mouse_x = x;
        mouse_y = y;
        glutPostRedisplay();
    }
}

void key_pressed(unsigned char key, int x, int y)
{
    if(key == 'q')
    {
        exit(0);
    }
}
void create_lights()
{
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Light 1 Below
    ///////////////////////////////////////////////////////////////////////////////////////////////
    
    Point_Light light1;
    
    light1.position[0] = -1;
    light1.position[1] = 0;
    light1.position[2] = 1;
    light1.position[3] = 1;
    
    light1.color[0] = 1;
    light1.color[1] = 1;
    light1.color[2] = 1;
    light1.attenuation_k = 0;
    
    lights.push_back(light1);
    
    
}


/* CS 171 Assignment by JongSeok (Parker) Won
 * 
 * this program renders and image using a given .iv datafile
 *
 * this program takes three arguments and one file input
 * xres - x resolution (number of pixels) of the output image
 * yres - y resolution (number of pixels) of the output image
 * k - degree of attenuation
 * one file input using standard input - this file contains all required info
 *
 * this program outputs the result into a .ppm file and also to std output
 */

#include "shaded.h"

IVData parse_iv(istream &datafile);

int main(int argc, char* argv[])
{
    if(!(argc == 4 || argc == 5))
        cout << "usage: " << argv[0] <<
        " <xres> <yres> <k> [-eyelight] < <filename>" << endl;
    else
    {
        int xres = atoi(argv[1]);
        int yres = atoi(argv[2]);
        float k = atof(argv[3]);

        // pixel grids
        MatrixXd red = MatrixXd::Zero(xres, yres);
        MatrixXd blue = MatrixXd::Zero(xres, yres);
        MatrixXd green = MatrixXd::Zero(xres, yres);

        // depth buffer
        MatrixXd depth_buffer(xres, yres);
        depth_buffer.fill(INFINITY);

        // parse ivdata
        IVData data = parse_iv(cin);
        Camera &camera = data.camera;
        vector<Object> &objects = data.objects;
        vector<Point_Light> &lights = data.lights;

        // add in eyelight effect if flagged with -eyelight
        bool yes_eyelight = false;
        if(argc == 5)
            if(strcmp(argv[4], "-eyelight") == 0)
                addEyelight(lights, camera);
            else
            {
                cout << "usage: " << argv[0] <<
                " <xres> <yres> <k> [-eyelight] < <filename>" << endl;
                return 0;
            }
        // keep track of this flag for naming purposes
        yes_eyelight = true;

        // calculate transformation matrices for converting between world
        // coordinates and camera coordinates
        calculate_world2cam(camera);

        // calculate perspective projection matrix
        calculate_perspective_mat(camera);

        // calculate and store overall transforms of each object stored in data
        calculate_overall_transforms(objects);

        // calculate and store normal transformation matrix of each object
        // stored in data
        calculate_normal_transforms(objects);

        // calculate and store points in NDC for each object stored in data
        calculate_ndc(objects, camera);

        // render points to a set of three MatrixXd instances that represent
        // red, blue and green pixel grids
        // draw_points(objects, red, blue, green);

        // rasterize each of the edges of facesets of each object
        // raster_line(objects, red, blue, green, mode);

        // rasterize each of the faces of each object
        raster_triangle(
            objects,
            lights,
            camera,
            red, blue, green,
            depth_buffer,
            k);

        stringstream filename;
        // name every file to be:
        // wireframe_<xres>x<yres>_mode<mode>.ppm
        // store file in output directory
        if(yes_eyelight)
            filename << "../output/"
                        << "wireframe_" << xres << "x" << yres
                        << "_k=" << k << "_eyelight" << ".ppm";
        else
            filename << "../output/"
                        << "wireframe_" << xres << "x" << yres
                        << "_k=" << k << ".ppm";
        string filename_str = filename.str();
        // render pixels to a ppm image output file named filename_str
        ppm_render(red, blue, green, filename_str);

        // write all pixels into standard output
        std_out_render(red, blue, green);
    }
}

/////////////////////////////////// POINT ////////////////////////////////////

/* this function draws every point included in a given vector of Objects
 *
 * this function takes four arguments:
 *      objects - reference to the vector of Objects
 *      red, blue, green - references to pixel grids
 */
void draw_points(
    vector<Object> &objects,
    MatrixXd &red,
    MatrixXd &blue,
    MatrixXd &green)
{
    // assuming that red, blue and green are all of same dimensions
    int xres = red.rows();
    int yres = red.cols();
    for(vector<Object>::iterator it_object = objects.begin();
        it_object != objects.end();
        it_object++)
    {
        for(vector<Point>::iterator it = it_object->ndc.begin();
        it != it_object->ndc.end();
        it++)
        {
            float x_ndc = it->x / it->w;
            float y_ndc = it->y / it->w;
            float z_ndc = it->z / it->w;
            int x_screen = int(xres * (1 + x_ndc) / 2);
            int y_screen = int(yres * (1 + y_ndc) / 2);
            // skip if point is outside of the screen
            if(x_screen >= xres || x_screen < 0 ||
                y_screen >= yres || y_screen < 0)
                continue;
            // fill pixels
            red(x_screen, y_screen) = MAX_BRT;
            blue(x_screen, y_screen) = MAX_BRT;
            green(x_screen, y_screen) = MAX_BRT;
        }
    }
}

/////////////////////////////////// LINE /////////////////////////////////////

/* this function rasterizes each of the edges of a given list of objects
 * this is achieved by calling the correct line drawing algorithm for each of
 * the edges included in the faceset and for extra edges that break up every
 * non-triangle facesets into triangles
 *
 * this function takes five arguments:
 *      object_p - reference to the object that is to be rasterized
 *      red, blue, green - references to the three pixel grids
 *      mode - an int that determines which rasterization algorithm to use
 *          - mode = 0: use Bresenham's line algorithm
 *          - mode = 1: use Xiaolin Wu's line algorithm
 *
 * _fs suffix means faceset
 */
void raster_line(
    vector<Object> &objects,
    MatrixXd &red,
    MatrixXd &blue,
    MatrixXd &green,
    int mode)
{
    // for every object
    for(vector<Object>::iterator it_object = objects.begin();
        it_object != objects.end();
        it_object++)
        // for every faceset
        for(vector< vector<int> >::iterator it_fs = it_object->facesets.begin();
            it_fs != it_object->facesets.end();
            it_fs++)
        {
            // store NDC x and y values of every point in faceset
            vector<float> x;
            vector<float> y;
            for(vector<int>::iterator it = it_fs->begin();
                it != it_fs->end();
                it++)
            {
                x.push_back(it_object->ndc.at(*it).x /
                            it_object->ndc.at(*it).w);
                y.push_back(it_object->ndc.at(*it).y /
                            it_object->ndc.at(*it).w);
            }
            // keep track of number of edges added
            int num_edges = 0;
            // draw each edge except the last one
            for(int i = 0; i < x.size() - 1; i++)
            {
                draw_line(x.at(i), y.at(i), x.at(i + 1), y.at(i + 1),
                        red, blue, green, mode);
                num_edges++;
                // if num_edges is > 2, it means that the faceset is not a
                // triangle therefore, an extra edge must be drawn to break up
                // the faceset
                if(num_edges > 2)
                    draw_line(x.at(i), y.at(i), x.front(), y.front(),
                            red, blue, green, mode);
            }
            // complete the face by drawing the last edge
            draw_line(x.front(), y.front(), x.back(), y.back(),
                    red, blue, green, mode);
        }
}

/* this function draws a line in the provided pixel grids
 * the drawn line connects two points specified by four NDC values
 *
 * this function takes eight arguments:
 *      x0, y0, x1, y1 - NDC of two points
 *      red, blue, green - references to the three pixel grids
 *      mode - an int that determines which rasterization algorithm to use
 *          - mode = 0: use Bresenham's line algorithm
 *          - mode = 1: use Xiaolin Wu's line algorithm
 */
void draw_line(
    float x0,
    float y0,
    float x1,
    float y1,
    MatrixXd &red,
    MatrixXd &blue,
    MatrixXd &green,
    int mode)
{
    if(mode == 0)
    {
        // assuming that red, blue and green are all of same dimensions
        int xres = red.rows();
        int yres = red.cols();
        // x and y coordinates on the pixel grid
        int x_grid_0 = (x0 + 1) / 2 * xres;
        int y_grid_0 = (y0 + 1) / 2 * yres;
        int x_grid_1 = (x1 + 1) / 2 * xres;
        int y_grid_1 = (y1 + 1) / 2 * yres;
        float slope;
        // there are two cases depending on the slope
        // if abs(slope) < 1, then Bresenham's line algorithm can be used to
        // draw the line normally (with slight generalization accounting for
        // negative slope)
        if(abs(y_grid_1 - y_grid_0) < abs(x_grid_1 - x_grid_0))
        {
            // x_grid_0 must be less than x_grid_1
            if(x_grid_0 > x_grid_1)
            {
                swap(x_grid_0, x_grid_1);
                swap(y_grid_0, y_grid_1);
            }
            slope = float(y_grid_1 - y_grid_0) / float(x_grid_1 - x_grid_0);
            // implement generalized Bresenham's line algorithm
            float error = 0;
            int y = y_grid_0;
            for(int x = x_grid_0; x < x_grid_1; x++)
            {
                // skip if x, y is outside of the screen
                if(x < 0 || x >= xres || y <= 0 || y > yres)
                    continue;
                // fill in pixel grid
                red(x, y) = MAX_BRT;
                blue(x, y) = MAX_BRT;
                green(x, y) = MAX_BRT;
                // if error is small enough don't change y
                // absolute value function allows negative slopes to work as
                // well
                if(abs(error + slope) < 0.5)
                    error += slope;
                // if error is large enough increase/decrease y
                else
                {
                    error += slope - sign(slope);
                    y += sign(slope);
                }
            }
        }
        else
        // if abs(slope) > 1, then Bresenham's line algorithm can be used to
        // draw the line normally only if x and y are swapped (with slight 
        // generalization accounting for negative slope)
        {
            // y_grid_0 must be less than y_grid_1
            if(y_grid_0 > y_grid_1)
            {
                swap(x_grid_0, x_grid_1);
                swap(y_grid_0, y_grid_1);
            }
            // since we are swapping x and y, slope must be dx over dy
            slope = float(x_grid_1 - x_grid_0) / float(y_grid_1 - y_grid_0);
            // implement generalized Bresenham's line algorithm with x and y
            // swapped
            float error = 0;
            int x = x_grid_0;
            for(int y = y_grid_0; y < y_grid_1; y++)
            {
                // skip if x, y is outside of the screen
                if(x < 0 || x >= xres || y <= 0 || y > yres)
                    continue;
                // fill in pixel grid
                red(x, y) = MAX_BRT;
                blue(x, y) = MAX_BRT;
                green(x, y) = MAX_BRT;
                // if error is small enough don't change y
                // absolute value function allows negative slopes to work as
                // well
                if(abs(error + slope) < 0.5)
                    error += slope;
                // if error is large enough increase/decrease y
                else
                {
                    error += slope - sign(slope);
                    x += sign(slope);
                }
            }
        }
    }
    else if(mode == 1)
    {
        // assuming that red, blue and green are all of same dimensions
        int xres = red.rows();
        int yres = red.cols();
        // x and y coordinates on the pixel grid
        int x_grid_0 = (x0 + 1) / 2 * xres;
        int y_grid_0 = (y0 + 1) / 2 * yres;
        int x_grid_1 = (x1 + 1) / 2 * xres;
        int y_grid_1 = (y1 + 1) / 2 * yres;
        float slope;
        // there are two cases depending on the slope
        // if abs(slope) < 1, then Xiaolin Wu's line algorithm can be used to
        // draw the line normally (with slight generalization accounting for
        // negative slope)
        if(abs(y_grid_1 - y_grid_0) < abs(x_grid_1 - x_grid_0))
        {
            // x_grid_0 must be less than x_grid_1
            if(x_grid_0 > x_grid_1)
            {
                swap(x_grid_0, x_grid_1);
                swap(y_grid_0, y_grid_1);
            }
            slope = float(y_grid_1 - y_grid_0) / float(x_grid_1 - x_grid_0);
            // fill in the endpoints of the line first
            if(x_grid_0 >= 0 && x_grid_0 < xres &&
                y_grid_0 > 0 && y_grid_0 <= yres)
            {
                red(x_grid_0, y_grid_0) = MAX_BRT;
                blue(x_grid_0, y_grid_0) = MAX_BRT;
                green(x_grid_0, y_grid_0) = MAX_BRT;
            }
            if(x_grid_1 >= 0 && x_grid_1 < xres &&
                y_grid_1 > 0 && y_grid_1 <= yres)
            {
                red(x_grid_1, y_grid_1) = MAX_BRT;
                blue(x_grid_1, y_grid_1) = MAX_BRT;
                green(x_grid_1, y_grid_1) = MAX_BRT;
            }
            // implement rest of Xiaolin Wu's line algorithm
            float error = slope;
            int y = int(y_grid_0 + slope);
            // fill in pixel grid with two pixels per x
            // fill in each pixel with different brightness depending on
            // error
            for(int x = x_grid_0 + 1; x < x_grid_1 - 1; x++)
            {
                // skip if x, y is outside of the screen
                if(x < 0 || x >= xres || y <= 0 || y > yres)
                    continue;
                // fill pixels
                red(x, y - sign(slope)) = int(MAX_BRT * abs(error));
                blue(x, y - sign(slope)) = int(MAX_BRT * abs(error));
                green(x, y - sign(slope)) = int(MAX_BRT * abs(error));
                red(x, y) = int(MAX_BRT * (1 - abs(error)));
                blue(x, y) = int(MAX_BRT * (1 - abs(error)));
                green(x, y) = int(MAX_BRT * (1 - abs(error)));
                // if error is small enough don't change y
                // absolute value function allows negative slopes to work as
                // well
                if(abs(error + slope) < 1)
                    error += slope;
                // if error is large enough increase/decrease y
                else
                {
                    error += slope - sign(slope);
                    y += sign(slope);
                }
            }
        }
        else
        // if abs(slope) > 1, then Xiaolin Wu's line algorithm can be used to
        // draw the line normally only if x and y are swapped (with slight 
        // generalization accounting for negative slope)
        {
            // y_grid_0 must be less than y_grid_1
            if(y_grid_0 > y_grid_1)
            {
                swap(x_grid_0, x_grid_1);
                swap(y_grid_0, y_grid_1);
            }
            slope = float(x_grid_1 - x_grid_0) / float(y_grid_1 - y_grid_0);
            // fill in the endpoints of the line first
            if(x_grid_0 >= 0 && x_grid_0 < xres &&
                y_grid_0 > 0 && y_grid_0 <= yres)
            {
                red(x_grid_0, y_grid_0) = MAX_BRT;
                blue(x_grid_0, y_grid_0) = MAX_BRT;
                green(x_grid_0, y_grid_0) = MAX_BRT;
            }
            if(x_grid_1 >= 0 && x_grid_1 < xres &&
                y_grid_1 > 0 && y_grid_1 <= yres)
            {
                red(x_grid_1, y_grid_1) = MAX_BRT;
                blue(x_grid_1, y_grid_1) = MAX_BRT;
                green(x_grid_1, y_grid_1) = MAX_BRT;
            }
            // implement rest of Xiaolin Wu's line algorithm
            float error = slope;
            int x = int(x_grid_0 + slope);
            // fill in pixel grid with two pixels per y
            // fill in each pixel with different brightness depending on
            // error
            for(int y = y_grid_0 + 1; y < y_grid_1 - 1; y++)
            {
                // skip if x, y is outside of the screen
                if(x < 0 || x >= xres || y <= 0 || y > yres)
                    continue;
                // fill pixels
                red(x + sign(slope), y) = int(MAX_BRT * abs(error));
                blue(x + sign(slope), y) = int(MAX_BRT * abs(error));
                green(x + sign(slope), y) = int(MAX_BRT * abs(error));
                red(x, y) = int(MAX_BRT * (1 - abs(error)));
                blue(x, y) = int(MAX_BRT * (1 - abs(error)));
                green(x, y) = int(MAX_BRT * (1 - abs(error)));
                // if error is small enough don't change y
                // absolute value function allows negative slopes to work as
                // well
                if(abs(error + slope) < 1)
                    error += slope;
                // if error is large enough increase/decrease y
                else
                {
                    error += slope - sign(slope);
                    x += sign(slope);
                }
            }
        }
    }
}

/////////////////////////////////// TRIANGLE /////////////////////////////////

/* this function calculates the values of alpha, beta and gamma for a Point
 * based on three other points (these x and y values represent x and y
 * coordinates on the screen/pixel grid NOT NDC)
 *
 * this function takes eight arguments:
 *      x, y - x, y screen coordinates of the point in question
 *      x0, x1, x2, y0, y1, y2 - x, y screen coordinates of the basis points
 *
 * this function returns an instance of Vector3d (alpha, beta, gamma)
 */
Vector3d alpha_beta_gamma(
    int x, int y,
    int x0, int y0,
    int x1, int y1,
    int x2, int y2)
{
    float alpha = f(x, y, x1, y1, x2, y2) / f(x0, y0, x1, y1, x2, y2);
    float beta = f(x, y, x0, y0, x2, y2) / f(x1, y1, x0, y0, x2, y2);
    float gamma = f(x, y, x0, y0, x1, y1) / f(x2, y2, x0, y0, x1, y1);
    Vector3d ABC(alpha, beta, gamma);
    return ABC;
}

/* this function is a helper function for the alpha_beta_gamma function
 *
 * this function takes six arguments:
 *      x, y - x, y screen coordinates of the point in question
 *      x0, x1, y0, y1 - two of the three reference points from
 *                      alpha_beta_gamma function
 *
 * this function returns a float value
 */
float f(int x, int y, int x0, int y0, int x1, int y1)
{
    return float((y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0);
}

/* this function rasterizes each of the faces of a given list of objects
 * if a face is not a triangle, it is broken up into triangles
 * shading information is stored in lights
 *
 * this function takes eight arguments:
 *      objects - reference to the vector of objects that is to be rasterized
 *      lights - reference to the vector of Point_Light instances
 *              used for shading
 *      camera - reference to the Camera instance
 *      red, blue, green - references to the three pixel grids
 *      depth_buffer - reference to the depth_buffer
 *      k - degree of attenuation
 */
void raster_triangle(
    vector<Object> &objects,
    vector<Point_Light> &lights,
    Camera &camera,
    MatrixXd &red,
    MatrixXd &blue,
    MatrixXd &green,
    MatrixXd &depth_buffer,
    float k)
{
    // for every object
    for(vector<Object>::iterator it_object = objects.begin();
        it_object != objects.end();
        it_object++)
    {
        // for every faceset
        for(vector< vector<int> >::iterator it_fs = it_object->facesets.begin();
            it_fs != it_object->facesets.end();
            it_fs++)
        {
            // store NDC x, y and z values of every point in faceset
            vector<float> x;
            vector<float> y;
            vector<float> z;
            // indices of normal of each point
            vector<int> normal_index;
            // indices of each point
            vector<int> point_index;
            // keep track of index in for loop used below
            for(vector<int>::iterator it = it_fs->begin();
                it != it_fs->end();
                it++)
            {
                Point &p = it_object->ndc.at(*it);
                x.push_back(p.x / p.w);
                y.push_back(p.y / p.w);
                z.push_back(p.z / p.w);
                int set_index = it_fs - it_object->facesets.begin();
                vector<int> &normal_set = it_object->normal_sets.at(set_index);
                normal_index.push_back(normal_set.at(it - it_fs->begin()));
                point_index.push_back(*it);
            }
            // draw each triangle
            // assuming x, y, z and i are all of the same size
            for(int i = 2; i < x.size(); i++)
            {
                // backface culling
                Point &p0 = it_object->ndc.at(point_index.at(0));
                Point &p1 = it_object->ndc.at(point_index.at(i - 1));
                Point &p2 = it_object->ndc.at(point_index.at(i));
                Vector3d first_edge(
                    p0.x / p0.w - p1.x / p1.w,
                    p0.y / p0.w - p1.y / p1.w,
                    p0.z / p0.w - p1.z / p1.w);
                Vector3d second_edge(
                    p2.x / p2.w - p1.x / p1.w,
                    p2.y / p2.w - p1.y / p1.w,
                    p2.z / p2.w - p1.z / p1.w);
                if(second_edge.cross(first_edge)[2] < 0)
                    continue;
                // calculate rgb0, rgb1 and rgb2
                Vector3d rgb0 = lighting(point_index.at(0),
                                        normal_index.at(0),
                                        *it_object, lights, camera, k);
                Vector3d rgb1 = lighting(point_index.at(i - 1),
                                        normal_index.at(i - 1),
                                        *it_object, lights, camera, k);
                Vector3d rgb2 = lighting(point_index.at(i),
                                        normal_index.at(i),
                                        *it_object, lights, camera, k);
                // fill triangle
                fill_colored_triangle(
                    x.at(0), y.at(0), z.at(0),
                    x.at(i - 1), y.at(i - 1), z.at(i - 1),
                    x.at(i), y.at(i), z.at(i),
                    rgb0, rgb1, rgb2,
                    red, blue, green,
                    depth_buffer);
            }
        }
    }
}

/* this function fills a triangle with color
 * each vertex has a color and the color is interpolated over the triangle
 *
 * this function takes 16 arguments:
 *      x0, x1, x2, y0, y1, y2, z0, z1, z2 - NDC coordinates of the triangle
 *      rgb0, rgb1, rgb2 - color for each of the points
 *      red, blue, green - references to the three pixel grids
 *      depth_buffer - reference to the depth_buffer
 */
void fill_colored_triangle(
    float x0, float y0, float z0,
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    Vector3d rgb0, Vector3d rgb1, Vector3d rgb2,
    MatrixXd &red, MatrixXd &blue, MatrixXd &green,
    MatrixXd &depth_buffer)
{
    // assuming that red, blue and green are all of same dimensions
    int xres = red.rows();
    int yres = red.cols();
    // x and y coordinates on the pixel grid
    int x_grid_0 = (x0 + 1) / 2 * xres;
    int y_grid_0 = (y0 + 1) / 2 * yres;
    int x_grid_1 = (x1 + 1) / 2 * xres;
    int y_grid_1 = (y1 + 1) / 2 * yres;
    int x_grid_2 = (x2 + 1) / 2 * xres;
    int y_grid_2 = (y2 + 1) / 2 * yres;
    // find x_min, x_max, y_min, y_max
    int x_min = int(min(float(x_grid_0), float(x_grid_1), float(x_grid_2)));
    int y_min = int(min(float(y_grid_0), float(y_grid_1), float(y_grid_2)));
    int x_max = int(max(float(x_grid_0), float(x_grid_1), float(x_grid_2)));
    int y_max = int(max(float(y_grid_0), float(y_grid_1), float(y_grid_2)));
    for(int x = x_min; x < x_max; x++)
    {
        for(int y = y_min; y < y_max; y++)
        {
            // skip if x, y is outside of the screen
            if(x < 0 || x >= xres || y < 0 || y >= yres)
                continue;
            Vector3d abc = alpha_beta_gamma(x, y,
                x_grid_0, y_grid_0, x_grid_1, y_grid_1, x_grid_2, y_grid_2);
            if(abc[0] >= 0 && abc[0] <= 1 &&
                abc[1] >= 0 && abc[1] <= 1 &&
                abc[2] >= 0 && abc[2] <= 1)
            {
                // skip if z is not in range
                // z is in NDC
                float z = z0 * abc[0] + z1 * abc[1] + z2 * abc[2];
                if(z <= -1)
                    continue;
                // skip if z is larger than corresponding depth buffer
                if(z > depth_buffer(x, y))
                    continue;
                else
                    depth_buffer(x, y) = z;
                // fill in pixel grid
                red(x, y) = truncate(abc[0] * rgb0[0] + \
                                    abc[1] * rgb1[0] + \
                                    abc[2] * rgb2[0], MAX_BRT);
                green(x, y) = truncate(abc[0] * rgb0[1] + \
                                    abc[1] * rgb1[1] + \
                                    abc[2] * rgb2[1], MAX_BRT);
                blue(x, y) = truncate(abc[0] * rgb0[2] + \
                                    abc[1] * rgb1[2] + \
                                    abc[2] * rgb2[2], MAX_BRT);
            }
        }
    }
}

/////////////////////////////////// SHADING //////////////////////////////////

/* this function calculates rgb of a point using the Phong reflection model
 *
 * this function takes four arguments:
 *      point_i - index of the point
 *      normal_i - index of the normal
 *      object, lights, camera - information required for shading calculation
 *      k - degree of attenuation
 *
 * this function returns an instance of Vector3d that represents the resulting
 * rgb of the point
 */
Vector3d lighting(
    int point_i,
    int normal_i,
    Object &object,
    vector<Point_Light> &lights,
    Camera &camera,
    float k)
{
    Vector3d rgb_a = object.ambient_reflect;
    Vector3d c_d = object.diffuse_reflect;
    Vector3d c_s = object.specular_reflect;
    // shininess
    float shininess = object.shininess;
    // initialize rgb_d and rgb_s
    Vector3d rgb_d(0, 0, 0);
    Vector3d rgb_s(0, 0, 0);
    // position vector of point in question in world coordinates
    Point p = object.points.at(point_i);
    VectorXd p_4d(4, 1);
    p_4d << p.x, p.y, p.z, p.w;
    p_4d = object.overall_transform * p_4d;
    Vector3d point(p_4d[0] / p_4d[3], p_4d[1] / p_4d[3], p_4d[2] / p_4d[3]);
    // normal vector in world coordinates
    Vector3d n = object.normals.at(normal_i);
    Matrix3d normal_transform_m3 = object.normal_transform.block(0, 0, 3, 3);
    n = normal_transform_m3 * n;
    n /= n.norm();
    // position vector of camera
    Vector3d camera_p = camera.position;
    // unit vector pointing from point to camera
    Vector3d e = camera_p - point;
    e /= e.norm();
    for(vector<Point_Light>::iterator it = lights.begin();
        it != lights.end();
        it++)
    {
        Vector3d light_p = it->position;
        Vector3d rgb_l = it->color;
        // unit vector pointing from point to light and distance to light from
        // point
        Vector3d l = light_p - point;
        float distance = l.norm();
        l /= l.norm();
        // apply attenuation factor
        rgb_l /= 1 + k * distance * distance;
        // calculate rgb_d and rgb_s
        rgb_d += rgb_l * fmax(0, n.dot(l));
        rgb_s += rgb_l * pow(fmax(0, n.dot((e + l) / (e + l).norm())),
                            shininess);
    }
    // multiply c_d and c_s to rgb_d and rgb_s
    rgb_d = rgb_d.cwiseProduct(c_d);
    rgb_s = rgb_s.cwiseProduct(c_s);
    // add all rgb and truncate to MAX_BRT
    Vector3d rgb = rgb_a + rgb_d + rgb_s;
    rgb[0] = truncate(rgb[0] * MAX_BRT, MAX_BRT);
    rgb[1] = truncate(rgb[1] * MAX_BRT, MAX_BRT);
    rgb[2] = truncate(rgb[2] * MAX_BRT, MAX_BRT);
    return rgb;
}

/* this function adds in the eyelight effect into the scene */
void addEyelight(vector<Point_Light> &lights, Camera camera)
{
    Point_Light eyelight;
    eyelight.position = camera.position;
    eyelight.color = Vector3d::Zero();
    for(vector<Point_Light>::iterator it = lights.begin();
        it != lights.end();
        it++)
        eyelight.color += it->color;
    eyelight.color /= 5;
    eyelight.color /= lights.size();
    lights.push_back(eyelight);
}
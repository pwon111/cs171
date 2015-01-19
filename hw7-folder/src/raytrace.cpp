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

#include "raytrace.h"

// global variables defined with their default values
int xres = 100, yres = 100;

Vector3f eye(-20, 0, 30);
Vector3f eye_light_color(1.0, 1.0, 1.0);
Vector3f look_at(0, 0, 0);
Vector3f UP(0, 1, 0);
float Fx = 0.035;
float Fy = Fx * yres / xres;
float Fd = 0.05;

// max recursion depth
int MAX_REC;

// main function
int main(int argc, char* argv[])
{
    if(argc != 2)
        cout << "usage: " << argv[0] <<
        "<reflection MAX_REC>" << endl;
    else
    {
        MAX_REC = atoi(argv[1]);
        Vector3f e1, e2, e3;
        e3 = look_at - eye;
        e3.normalize();
        e2 = UP - e3 * e3.dot(UP);
        e2.normalize();
        e1 = e2.cross(e3);

        // initialize all superquadrics and light sources
        vector<Superquadric*> superquadrics;
        Superquadric* s0 = new Superquadric;
        {
            s0->id = 0;

            s0->T << 0.0, 0.0, 0.0;
            s0->S << 3.0, 3.0, 3.0;
            s0->R << 0.0, 0.0, 0.0, 1.0;

            s0->e = 1;
            s0->n = 1;

            s0->color_d << 1.0, 1.0, 1.0;
            s0->color_s << 1.0, 1.0, 1.0;

            s0->shininess = 0.5;
        }
        superquadrics.push_back(s0);
        Superquadric* s1 = new Superquadric;
        {
            s1->id = 1;

            s1->T << 50.0, 0.0, 0.0;
            s1->S << 45.0, 45.0, 45.0;
            s1->R << 0.0, 0.0, 0.0, 1.0;

            s1->e = 1;
            s1->n = 1;

            s1->color_d << 1.0, 1.0, 1.0;
            s1->color_s << 1.0, 1.0, 1.0;

            s1->shininess = 0.5;
        }
        superquadrics.push_back(s1);
        vector<Light*> lights;
        Light* l0 = new Light;
        {
            l0->origin << -10, 0, 0;
            l0->rgb << 1.0, 0.0, 0.0;
        }
        lights.push_back(l0);

        // pixel grids
        MatrixXd red = MatrixXd::Zero(xres, yres);
        MatrixXd green = MatrixXd::Zero(xres, yres);
        MatrixXd blue = MatrixXd::Zero(xres, yres);

        for(int x = 0; x < xres; x++)
            for(int y = 0; y < yres; y++)
            {
                // ray trace for each pixel
                Ray r = {
                    Fd * e3
                        + e1 * (x - xres / 2) * (Fx / xres)
                        + e2 * (y - yres / 2) * (Fy / yres),
                    eye
                };
                Vector3f rgb = lighting(r, &superquadrics, &lights, 0.05, 0, -1);
                red(x, y) = (int)(rgb[0]);
                green(x, y) = (int)(rgb[1]);
                blue(x, y) = (int)(rgb[2]);
            }

        // ppm_render(red, blue, green, string filename);
        std_out_render(red, green, blue, MAX_BRT);

        for(vector<Superquadric*>::iterator it = superquadrics.begin();
            it != superquadrics.end();
            it++)
            delete *it;
        for(vector<Light*>::iterator it = lights.begin();
            it != lights.end();
            it++)
            delete *it;
        superquadrics.clear();
        lights.clear();
    }
}

/* This function takes a 3D world coordinate and checks if it is inside a
 * specifc superquadric
 *
 * This function takes four arguments
 *      - x, y, z - the 3D world coordinate
 *      - s - a pointer to the superquadric in question
 */
float isq(
    float x,
    float y,
    float z,
    Superquadric* s)
{
    Vector4f V(x, y, z, 1);
    Matrix4f translation = get_translate_mat(s->T[0], s->T[1], s->T[2]);
    Matrix4f scaling = get_scale_mat(s->S[0], s->S[1], s->S[2]);
    Matrix4f rotation = get_rotate_mat(s->R[1],
                                    s->R[2],
                                    s->R[3],
                                    deg2rad(s->R[0]));
    V = translation.inverse() * V;
    V = rotation.inverse() * V;
    V = scaling.inverse() * V;
    return pow(pow(pow(V[0] / V[3], 2), 1 / s->e)
                + pow(pow(V[1] / V[3], 2), 1 / s->e), s->e / s->n) \
        + pow(pow(V[2] / V[3], 2), 1 / s->n) - 1;
}

/* This function takes a 3D world coordinate and finds the gradient of the isq
 * function at that point
 *
 * This function takes four arguments
 *      - x, y, z - the 3D world coordinate
 *      - s - a pointer to the superquadric in question
 */
Vector3f isq_gradient(
    float x,
    float y,
    float z,
    Superquadric* s)
{
    Vector4f V4(x, y, z, 1);
    Matrix4f translation = get_translate_mat(s->T[0], s->T[1], s->T[2]);
    Matrix4f scaling = get_scale_mat(s->S[0], s->S[1], s->S[2]);
    Matrix4f rotation = get_rotate_mat(s->R[1],
                                    s->R[2],
                                    s->R[3],
                                    deg2rad(s->R[0]));
    V4 = translation.inverse() * V4;
    V4 = rotation.inverse() * V4;
    V4 = scaling.inverse() * V4;
    Vector3f V = Vector4fTo3f(V4);
    float temp = pow(pow(abs(V[0]), 2 / s->e)
                + pow(abs(V[1]), 2 / s->e), s->e / s->n - 1);
    Vector4f gradient4;
    gradient4 << 2 * V[0] * pow(abs(V[0]), 2 / s->e - 2) * temp / s->n,
                2 * V[1] * pow(abs(V[1]), 2 / s->e - 2) * temp / s->n,
                2 * V[2] * pow(abs(V[2]), 2 / s->n - 2) / s->n,
                1;
    gradient4 = rotation * gradient4;
    Vector3f gradient = Vector4fTo3f(gradient4);
    return gradient;
}

// this function simply returns isq_graident normalized
// helper function for find normals on the surfaces of superquadrics
Vector3f isq_normal(
    float x,
    float y,
    float z,
    Superquadric* s)
{
    Vector3f normal = isq_gradient(x, y, z, s);
    normal.normalize();
    return normal;
}

/* This function turns a superquadric into a parametric function in terms of u
 * and v and returns the point at a specific u and v values
 *
 * This function takes three arguments:
 *      - u, v - the u, v values in question
 *      - s - a pointer to the superquadric in question
 */
Vector3f psq(float u, float v, Superquadric* s)
{
    Vector3f psq;
    psq << exp_cos(v, s->n) * exp_cos(u, s->e),
        exp_cos(v, s->n) * exp_sin(u, s->e),
        exp_sin(v, s->n);
    return psq;
}

/* This function finds the smallest value of t at which a ray intersects a
 * superquadric
 * If there is no intersection FLT_MAX is returned
 *
 * This function takes two arguments:
 *      - ray - the ray in question
 *      - s - a pointer to the superquadric in question
 */
float calculateIntersection(Ray ray, Superquadric* s)
{
    float t_ans = rayXBoundingSphere(ray, s);
    if(t_ans == FLT_MAX)
        return t_ans;

    float prev_slope = g_slope(ray, s, t_ans);
    float prev_g = g(ray, s, t_ans);
    while(g(ray, s, t_ans) > Fx / (20 * xres))
    {
        t_ans = t_ans - prev_g / prev_slope;
        if(sign(prev_slope) != sign(g_slope(ray, s, t_ans)))
            return FLT_MAX;
        prev_slope = g_slope(ray, s, t_ans);
        prev_g = g(ray, s, t_ans);
        if(prev_slope == 0 && prev_g > 0)
            return FLT_MAX;
    }
    return t_ans;
}

/* This function finds the value of t at which a ray first hits the bounding
 * sphere of a superquadric
 * If there is no intersection FLT_MAX is returned
 *
 * This function takes two arguments:
 *      - ray - the ray in question
 *      - s - a pointer to the superquadric in question
 */
float rayXBoundingSphere(Ray ray, Superquadric* s)
{
    Matrix4f translation = get_translate_mat(s->T[0], s->T[1], s->T[2]);
    Matrix4f scaling = get_scale_mat(s->S[0], s->S[1], s->S[2]);
    Matrix4f rotation = get_rotate_mat(s->R[1],
                                    s->R[2],
                                    s->R[3],
                                    deg2rad(s->R[0]));
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
float g(Ray ray, Superquadric* s, float t)
{
    Vector3f x = ray.A * t + ray.B;
    return isq(x[0], x[1], x[2], s);
}
float g_slope(Ray ray, Superquadric* s, float t)
{
    Vector3f x = ray.A * t + ray.B;
    return ray.A.dot(isq_gradient(x[0], x[1], x[2], s));
}

/* This function returns the lighting color for one ray using Phong Lighting
 * Model
 * 
 * This function takes six arguments:
 *      - ray - the ray in question
 *      - superquadrics - a pointer to a vector of pointers of superquadrics in
 *                      the scene
 *      - lights - a pointer to a vector of pointers of lights in the scene
 *      - k - attenuation factor
 *      - recursion_lvl - this function can be called recursively for reflection
 *                      and this variable keeps track of the recursion depth
 *      - last_s_id - this function keeps track of the last superquadric this
 *                  function was called from. Only relevant for reflections
 *                  since one source of bug is when the ray hits the
 *                  superquadric it started from. Initial call of lighting
 *                  should be -1 since id of superquadrics is never -1
 */
Vector3f lighting(
    Ray ray,
    vector<Superquadric*>* superquadrics,
    vector<Light*>* lights,
    float k,
    int recursion_lvl,
    int last_s_id)
{
    float t = FLT_MAX;
    float temp;
    Superquadric* temp_s;
    // first find the first intersection point for the ray
    for(vector<Superquadric*>::iterator it = superquadrics->begin();
        it != superquadrics->end();
        it++)
    {
        if((*it)->id == last_s_id)
            continue;
        temp = t;
        t = min(t, calculateIntersection(ray, *it));
        // save which superquadric has the first intersection
        if(t != temp)
            temp_s = *it;
    }
    if(t == FLT_MAX)
        return Vector3f(0, 0, 0);
    Vector3f point = ray.A * t + ray.B;
    Vector3f normal = isq_normal(point[0], point[1], point[2], temp_s);
    // initialize rgb_d and rgb_s
    Vector3f rgb_d(0, 0, 0);
    Vector3f rgb_s(0, 0, 0);
    // unit vector pointing from point to camera
    Vector3f e = eye - point;
    e.normalize();
    // calculate lighting
    Vector3f l;
    Vector3f light_color;
    // first account for eye light
    {
        // unit vector pointing from point to light and distance to light from
        // point
        l = eye - point;
        float distance = l.norm();
        l.normalize();
        light_color = eye_light_color;
        // apply attenuation factor
        light_color /= 1 + k * distance * distance;
        // calculate rgb_d and rgb_s
        rgb_d += light_color * fmax(0, normal.dot(l));
        rgb_s += light_color * pow(fmax(0, normal.dot((e + l) / (e + l).norm())),
                            temp_s->shininess);
    }
    // then account for all the other lights
    for(vector<Light*>::iterator it = lights->begin(); it != lights->end(); it++)
    {
        // unit vector pointing from point to light and distance to light from
        // point
        Vector3f l = (*it)->origin - point;
        float distance = l.norm();
        l.normalize();
        bool shadow = false;
        Ray shadow_ray = {l, point};
        // first check if current point is in a shadow of this light
        for(vector<Superquadric*>::iterator it_s = superquadrics->begin();
            it_s != superquadrics->end();
            it_s++)
        {
            // cout << (*it_s)->id << ", " << temp_s->id << endl;
            if((*it_s)->id == temp_s->id)
                continue;
            // cout << calculateIntersection(shadow_ray, *it_s) << endl;
            float t_temp = calculateIntersection(shadow_ray, *it_s);
            if(t_temp <= 0)
                continue;
            if(t_temp != FLT_MAX)
            {
                shadow = true;
                // cout << "SHADOW" << endl;
                break;
            }
        }
        // if it is in a shadow, skip this light
        if(shadow)
        {
            // cout << "CONTINUING for: " << temp_s->id << endl;
            continue;
        }
        // light color
        light_color = (*it)->rgb;
        // apply attenuation factor
        light_color /= 1 + k * distance * distance;
        // DIVIDE BY DISTANCE SQUARED
        // calculate rgb_d and rgb_s
        rgb_d += light_color * fmax(0, normal.dot(l));
        rgb_s += light_color * pow(fmax(0, normal.dot((e + l) / (e + l).norm())),
                            temp_s->shininess);
    }
    // multiply c_d and c_s to rgb_d and rgb_s
    rgb_d = rgb_d.cwiseProduct(temp_s->color_d);
    rgb_s = rgb_s.cwiseProduct(temp_s->color_s);
    // add all rgb and truncate to MAX_BRT
    Vector3f rgb = rgb_d + rgb_s;
    // add reflection
    if(recursion_lvl < MAX_REC)
    {
        Ray reflection_ray = {
            -l + 2 * l.dot(normal) * normal,
            point
        };
        rgb += lighting(
            reflection_ray,
            superquadrics,
            lights,
            k,
            recursion_lvl + 1,
            temp_s->id);
    }
    rgb[0] = min(rgb[0] * MAX_BRT, float(MAX_BRT));
    rgb[1] = min(rgb[1] * MAX_BRT, float(MAX_BRT));
    rgb[2] = min(rgb[2] * MAX_BRT, float(MAX_BRT));
    return rgb;
}
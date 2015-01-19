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

#include "rt_test.h"

// global variables defined with their default values
int xres = 300, yres = 300;
float e = 1.0, n = 1.0;  // shape exponents
Vector3f S(1.0, 1.0, 1.0);  // scaling vector
Vector3f T(0.0, 0.0, 0.0);  // translation vector
Vector4f R(0.0, 1.0, 0.0, 1.0);  // rotation vector
//          ^theta in this vector is stored in degrees
float xmin = -1.2, ymin = -1.2, zmin = -1.2;
float xmax = 1.2, ymax = 1.2, zmax = 1.2;
int N = 20;
float jitter = 0.5; // jitter must be between 0 and 1
int Nu = 20, Nv = 10;
Vector3f color_d(0.0, 0.0, 1.0);
Vector3f color_s(1.0, 1.0, 1.0);
float shininess = 10;

Vector3f eye(5, 5, 5);
Vector3f eye_light_color(1.0, 1.0, 1.0);
Vector3f look_at(0, 0, 0);
Vector3f UP(-1, -1, 1);
Vector3f e1, e2, e3;
float Fx = 0.035;
float Fy = Fx * yres / xres;
float Fd = 0.05;
Vector3f light0(-10, 10, 10);
Vector3f light0_color(1.0, 0.0, 0.0);

// main function
int main(int argc, char* argv[])
{
    if(argc != 3 && argc != 13)
        cout << "usage: " << argv[0] <<
        " <e> <n>" <<
        " [<Sx> <Sy> <Sz> <Tx> <Ty> <Tz> <Rtheta> <Rx> <Ry> <Rz>" << endl;
    else
    {
        e = atof(argv[1]);
        n = atof(argv[2]);
        if(argc == 13)
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
        }
        e3 = look_at - eye;
        e3.normalize();
        e2 = UP - e3 * e3.dot(UP);
        e2.normalize();
        e1 = e2.cross(e3);

        // pixel grids
        MatrixXd red = MatrixXd::Zero(xres, yres);
        MatrixXd green = MatrixXd::Zero(xres, yres);
        MatrixXd blue = MatrixXd::Zero(xres, yres);

        for(int x = 0; x < xres; x++)
            for(int y = 0; y < yres; y++)
            {
                Ray r = {
                    Fd * e3
                        + e1 * (x - xres / 2) * (Fx / xres)
                        + e2 * (y - yres / 2) * (Fy / yres),
                    eye
                };
                float t_intersection = calculateIntersection(r);
                if(t_intersection != FLT_MAX)
                {
                    Vector3f point = r.A * t_intersection + r.B;
                    Vector3f rgb = lighting(point, 0.01);
                    red(x, y) = int(rgb[0]);
                    green(x, y) = int(rgb[1]);
                    blue(x, y) = int(rgb[2]);
                }
            }

        // ppm_render(red, blue, green, string filename);
        std_out_render(red, green, blue, MAX_BRT);
    }
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
float calculateIntersection(Ray ray)
{
    float t_ans = rayXBoundingSphere(ray);
    if(t_ans == FLT_MAX)
        return t_ans;

    float prev_slope = g_slope(ray, t_ans);
    float prev_g = g(ray, t_ans);
    while(g(ray, t_ans) > 0.0001)
    {
        t_ans = t_ans - prev_g / prev_slope;
        if(sign(prev_slope) != sign(g_slope(ray, t_ans)))
            return FLT_MAX;
        prev_slope = g_slope(ray, t_ans);
        prev_g = g(ray, t_ans);
        if(prev_slope == 0 && prev_g > 0)
            return FLT_MAX;
    }
    return t_ans;
}

/* This function finds the value of t at which a ray (globally stored) first
 * hits the bounding sphere of a superquadric (globally stored)
 * If there is no intersection FLT_MAX is returned */
float rayXBoundingSphere(Ray ray)
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
float g(Ray ray, float t)
{
    Vector3f x = ray.A * t + ray.B;
    return isq(x[0], x[1], x[2]);
}
float g_slope(Ray ray, float t)
{
    Vector3f x = ray.A * t + ray.B;
    return ray.A.dot(isq_gradient(x[0], x[1], x[2]));
}

/* This function returns the lighting color for one ray using Phong Lighting
 * Model (ray, superquadric and lights in the scene are globally stored)
 * 
 * This function takes two arguments:
 *      - point - the point at which we want the lighting for (point is going to
 *              be the intersection between ray and superquadric)
 *      - k - attenuation factor
 */
Vector3f lighting(
    Vector3f point,
    float k)
{
    Vector3f normal = isq_normal(point[0], point[1], point[2]);
    // initialize rgb_d and rgb_s
    Vector3f rgb_d(0, 0, 0);
    Vector3f rgb_s(0, 0, 0);
    // unit vector pointing from point to camera
    Vector3f e = eye - point;
    e.normalize();
    // calculate lighting
    Vector3f l;
    Vector3f light_color;
    { // eye light
        light_color = eye_light_color;
        // unit vector pointing from point to light and distance to light from
        // point
        l = eye - point;
        float distance = l.norm();
        l.normalize();
        // apply attenuation factor
        light_color /= 1 + k * distance * distance;
        // DIVIDE BY DISTANCE SQUARED
        // calculate rgb_d and rgb_s
        rgb_d += light_color * fmax(0, normal.dot(l));
        rgb_s += light_color * pow(fmax(0, normal.dot((e + l) / (e + l).norm())),
                            shininess);
    }
    { // light0
        light_color = light0_color;
        // unit vector pointing from point to light and distance to light from
        // point
        Vector3f l = light0 - point;
        float distance = l.norm();
        l.normalize();
        // apply attenuation factor
        light_color /= 1 + k * distance * distance;
        // DIVIDE BY DISTANCE SQUARED
        // calculate rgb_d and rgb_s
        rgb_d += light_color * fmax(0, normal.dot(l));
        rgb_s += light_color * pow(fmax(0, normal.dot((e + l) / (e + l).norm())),
                            shininess);
    }
    // multiply c_d and c_s to rgb_d and rgb_s
    rgb_d = rgb_d.cwiseProduct(color_d);
    rgb_s = rgb_s.cwiseProduct(color_s);
    // add all rgb and truncate to MAX_BRT
    Vector3f rgb = rgb_d + rgb_s;
    rgb[0] = min(rgb[0] * MAX_BRT, float(MAX_BRT));
    rgb[1] = min(rgb[1] * MAX_BRT, float(MAX_BRT));
    rgb[2] = min(rgb[2] * MAX_BRT, float(MAX_BRT));
    return rgb;
}
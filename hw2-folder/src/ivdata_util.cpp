/* CS 171 Assignment by JongSeok (Parker) Won
 *
 * This cpp file contains implementations of the functions contained in
 * ivdata_util.h
 */

#include "ivdata_util.h"

/* this function takes an instance of Camera and fills in its world2cam
 * 
 * this function takes one argument:
 *      camera - reference to instance of camera
 */
void calculate_world2cam(Camera &camera)
{
    MatrixXd camera_to_world_T = get_translate_mat(camera.position[0],
                                                camera.position[1],
                                                camera.position[2]) * \
                                get_rotate_mat(camera.orientation_axis[0],
                                            camera.orientation_axis[1],
                                            camera.orientation_axis[2],
                                            camera.orientation_angle);
    camera.world2cam = camera_to_world_T.inverse();
}

/* this function takes an instance of Camera and fills in its perspective_mat
 * 
 * this function takes one argument:
 *      camera - reference to instance of camera
 */
void calculate_perspective_mat(Camera &camera)
{
    camera.perspective_mat = get_perspective_mat(camera.near, camera.far,
                                                camera.left, camera.right,
                                                camera.top, camera.bottom);
}

/* this function takes a vector of Objects and fills in the overall_transform
 * of each Object in the vector
 *
 * this function takes one argument:
 *      objects - reference to the vector of Objects
 */
void calculate_overall_transforms(vector<Object> &objects)
{
    for(vector<Object>::iterator it_object = objects.begin();
        it_object != objects.end();
        it_object++)
    {
        it_object->overall_transform.setIdentity(4, 4);
        for(vector<MatrixXd>::iterator it = it_object->transforms.begin();
        it != it_object->transforms.end();
        it++)
        {
            it_object->overall_transform *= *it;
        }
    }
}

/* this function takes a vector of Objects and fills in the normal_transform
 * of each Object in the vector
 *
 * this function takes one argument:
 *      objects - reference to the vector of Objects
 */
void calculate_normal_transforms(vector<Object> &objects)
{
    for(vector<Object>::iterator it_object = objects.begin();
        it_object != objects.end();
        it_object++)
    {
        it_object->normal_transform.setIdentity(4, 4);
        for(vector<MatrixXd>::iterator it = it_object->transforms.begin();
        it != it_object->transforms.end();
        it++)
        {
            int index = it - it_object->transforms.begin();
            // ignore translation matrices
            if(it_object->transform_types.at(index) != 't')
                it_object->normal_transform *= *it;
        }
        it_object->normal_transform = it_object->normal_transform.inverse();
        it_object->normal_transform.transposeInPlace();
    }
}

/* this function takes a vector of Objects and fills in the ndc vector in each
 * of the Objects in the said vector
 * transforming fromm world coordinates to ndc requires multiplying the world
 * coordinates with overall_transform of the object and then multiplying
 * camera.world2cam followed by the perspective matrix
 *
 * this function takes two arguments:
 *      objects - reference to the Object vector
 *      camera - instance of Camera (stores world2cam and perpespective_mat)
 */
void calculate_ndc(vector<Object> &objects, Camera camera)
{
    for(vector<Object>::iterator it_object = objects.begin();
        it_object != objects.end();
        it_object++)
    {
        for(vector<Point>::iterator it = it_object->points.begin();
        it != it_object->points.end();
        it++)
        {
            VectorXd ndc(4, 1);
            ndc << it->x, it->y, it->z, it->w;
            ndc = it_object->overall_transform * ndc;
            ndc = camera.world2cam * ndc;
            ndc = camera.perspective_mat * ndc;
            Point ndc_point;
            ndc_point.x = ndc[0];
            ndc_point.y = ndc[1];
            ndc_point.z = ndc[2];
            ndc_point.w = ndc[3];
            it_object->ndc.push_back(ndc_point);
        }
    }
}
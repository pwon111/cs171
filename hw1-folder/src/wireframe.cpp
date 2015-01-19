/* CS 171 Assignment by JongSeok (Parker) Won
 * 
 * this program outputs an image of a wireframe of an object in .ppm format
 * the user can determine what resoultion the image is rendered in and choose
 * which datafile to read the information on the wireframe from
 * additionally, the user can choose between two modes: mode 0 means use
 * Bresenham's line algorithm (no anti-aliasing) and mode 1 means use Xiaolin
 * Wu's line algorithm (yes anti-aliasing)
 *
 * this program takes three arguments and one file input
 * xres - x resolution (number of pixels) of the output image
 * yres - y resolution (number of pixels) of the output image
 * mode - either 0 or 1 for determining which line algorithm to use
 * one file input using standard input - this file contains all required
 *		on the wireframe
 *
 * variables with _T suffix represent transformation matrices
 *
 * VIEW WITH TABSIZE 4 FOR BEST READABILITY
 */

#include "wireframe.h"

IVData parse_iv(istream &datafile);

int main(int argc, char* argv[])
{
	if(argc != 4)
		cout << "usage: " << argv[0] << " <xres> <yres> <mode> < <filename>" << endl;
	else
	{
		int xres = atoi(argv[1]);
		int yres = atoi(argv[2]);
		int mode = atoi(argv[3]);

		// pixel grids
		MatrixXd red = MatrixXd::Zero(xres, yres);
		MatrixXd blue = MatrixXd::Zero(xres, yres);
		MatrixXd green = MatrixXd::Zero(xres, yres);

		// parse ivdata
		IVData data = parse_iv(cin);
		Camera camera = data.camera;
		vector<Object> objects = data.objects;

		// calculate transformation matrices for converting between world
		// coordinates and camera coordinates
		calculate_world2cam(camera);

		// calculate perspective projection matrix
		calculate_perspective_mat(camera);

		// calculate and store overall transforms of each object stored in data
		calculate_overall_transforms(objects);

		// calculate and store points in NDC for each object stored in data
		calculate_ndc(objects, camera);

		// render points to a set of three MatrixXd instances that represent
		// red, blue and green pixel grids
		// draw_points(objects, red, blue, green);

		// rasterize each of the edges of facesets of each object
		rasterize(objects, red, blue, green, mode);

		stringstream filename;
		// name every file to be:
		// wireframe_<xres>x<yres>_mode<mode>.ppm
		// store file in output directory
		filename << "../output/"
					<< "wireframe_" << xres << "x" << yres
					<< "_mode" << mode << ".ppm";
		string filename_str = filename.str();
		// render pixels to a ppm image output file named filename_str
		render(red, blue, green, filename_str);

		// write all pixels into standard output
		std_out_render(red, blue, green);
	}
}

/* this function takes an instance of Camera and fills in its world2cam
 * 
 * this function takes one argument:
 *		camera - reference to instance of camera
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
 *		camera - reference to instance of camera
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
 *		objects - reference to the vector of Objects
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

/* this function takes a vector of Objects and fills in the ndc vector in each
 * of the Objects in the said vector
 * transforming fromm world coordinates to ndc requires multiplying the world
 * coordinates with overall_transform of the object and then multiplying
 * camera.world2cam followed by the perspective matrix
 *
 * this function takes two arguments:
 *		objects - reference to the Object vector
 *		camera - instance of Camera (stores world2cam and perpespective_mat)
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

/* this function draws every point included in a given vector of Objects
 *
 * this function takes four arguments:
 *		objects - reference to the vector of Objects
 *		red, blue, green - references to pixel grids
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
			// "yres - " accounts for difference in NDC coordinates vs the
			// coordinates of the screen
			red(x_screen, yres - y_screen) = 255;
			blue(x_screen, yres - y_screen) = 255;
			green(x_screen, yres - y_screen) = 255;
		}
	}
}

/* this function rasterizes all lines included in a given vector of Objects
 * this is achieved by calling the correct line drawing algorithm for each of
 * the edges included in the faceset and for extra edges that break up every
 * non-triangle facesets into triangles
 *
 * this function takes four arguments:
 *		objects - reference to the vector of Objects
 *		red, blue, green - references to the three pixel grids
 *		mode - an int that determines which rasterization algorithm to use
 *			- mode = 0: use Bresenham's line algorithm
 *			- mode = 1: use Xiaolin Wu's line algorithm
 *
 * _fs suffix means faceset
 */
void rasterize(
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
				x.push_back(it_object->ndc.at(*it).x / it_object->ndc.at(*it).w);
				y.push_back(it_object->ndc.at(*it).y / it_object->ndc.at(*it).w);
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
 *		x0, y0, x1, y1 - NDC of two points
 *		red, blue, green - references to the three pixel grids
 *		mode - an int that determines which rasterization algorithm to use
 *			- mode = 0: use Bresenham's line algorithm
 *			- mode = 1: use Xiaolin Wu's line algorithm
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
				// "yres - " accounts for difference in NDC coordinates vs the
				// coordinates of the screen
				red(x, yres - y) = 255;
				blue(x, yres - y) = 255;
				green(x, yres - y) = 255;
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
				// "yres - " accounts for difference in NDC coordinates vs the
				// coordinates of the screen
				red(x, yres - y) = 255;
				blue(x, yres - y) = 255;
				green(x, yres - y) = 255;
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
				red(x_grid_0, yres - y_grid_0) = 255;
				blue(x_grid_0, yres - y_grid_0) = 255;
				green(x_grid_0, yres - y_grid_0) = 255;
			}
			if(x_grid_1 >= 0 && x_grid_1 < xres &&
				y_grid_1 > 0 && y_grid_1 <= yres)
			{
				red(x_grid_1, yres - y_grid_1) = 255;
				blue(x_grid_1, yres - y_grid_1) = 255;
				green(x_grid_1, yres - y_grid_1) = 255;
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
				// "yres - " accounts for difference in NDC coordinates vs the
				// coordinates of the screen
				red(x, yres - y - sign(slope)) = int(255 * abs(error));
				blue(x, yres - y - sign(slope)) = int(255 * abs(error));
				green(x, yres - y - sign(slope)) = int(255 * abs(error));
				red(x, yres - y) = int(255 * (1 - abs(error)));
				blue(x, yres - y) = int(255 * (1 - abs(error)));
				green(x, yres - y) = int(255 * (1 - abs(error)));
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
				red(x_grid_0, yres - y_grid_0) = 255;
				blue(x_grid_0, yres - y_grid_0) = 255;
				green(x_grid_0, yres - y_grid_0) = 255;
			}
			if(x_grid_1 >= 0 && x_grid_1 < xres &&
				y_grid_1 > 0 && y_grid_1 <= yres)
			{
				red(x_grid_1, yres - y_grid_1) = 255;
				blue(x_grid_1, yres - y_grid_1) = 255;
				green(x_grid_1, yres - y_grid_1) = 255;
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
				// "yres - " accounts for difference in NDC coordinates vs the
				// coordinates of the screen
				red(x + sign(slope), yres - y) = int(255 * abs(error));
				blue(x + sign(slope), yres - y) = int(255 * abs(error));
				green(x + sign(slope), yres - y) = int(255 * abs(error));
				red(x, yres - y) = int(255 * (1 - abs(error)));
				blue(x, yres - y) = int(255 * (1 - abs(error)));
				green(x, yres - y) = int(255 * (1 - abs(error)));
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

/* this function takes three different color pixel grids and outputs them into
 * a .ppm file
 *
 * this function takes four arguments:
 *		- red, blue, green - pixel grids
 *		- filename - name of the output file
 */
void render(MatrixXd red, MatrixXd blue, MatrixXd green, string filename)
{
	// assuming red, blue and green are all of the same dimension
	int xres = red.rows();
	int yres = red.cols();
	// ready output file
	ofstream output_file;
	output_file.open(filename.c_str());
	// write header of the ppm file
	output_file << "P3" << endl;
	output_file << xres << " " << yres << endl;
	output_file << 255 << endl;
	// write all pixels into output_file
	for(int y = 0; y < yres; y++)
		for(int x = 0; x < xres; x++)
			output_file << red(x, y) << " "
						<< green(x, y) << " "
						<< blue(x, y) << endl;
	// close file
	output_file.close();
}

/* this function takes three different color pixel grids and outputs them into
 * standard output
 *
 * this function takes one argument:
 *		- red, blue, green - pixel grids
 */
void std_out_render(MatrixXd red, MatrixXd blue, MatrixXd green)
{
	// assuming red, blue and green are all of the same dimension
	int xres = red.rows();
	int yres = red.cols();
	cout << "P3" << endl << xres << " " << yres << endl << 255 << endl;
			for(int y = 0; y < yres; y++)
				for(int x = 0; x < xres; x++)
					cout << red(x, y) << " "
						<< green(x, y) << " "
						<< blue(x, y) << endl;
}

/* this function swaps the values of two int values
 *
 * this function takes two arguments:
 *		a, b - references to the two integers
 */
void swap(int &a, int &b)
{
	int temp = b;
	b = a;
	a = temp;
}

/* this function returns the sign of a float
 * i.o.w. it returns 0 if it's 0, 1 if positive, -1 if negative
 *
 * this function takes one argument:
 *		a - the float
 */
int sign(float a)
{
	if(a == 0)
		return 0;
	else if(a > 0)
		return 1;
	else
		return -1;
}
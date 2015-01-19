/* CS 171 Assignment 0 PART 3 by JongSeok (Parker) Won 
 *
 * this program outputs an image in .ppm format
 * the user can determine the resolution of the image and choose which image
 * to output (two choices given)
 *
 * when executing this program takes three arguments:
 * xres - x resolution (num of pixels) of the output image
 * yres - y resolution (num of pixels) of the output image
 * mode - either 0 or 1
 *			0 means output a checkerboard image
 *			1 means output an image of a circle
 */

#include "ppm_test.h"

int main(int argc, char* argv[])
{
	// check and make sure that only 3 arguments (argc = 4) were given
	// and that xres, yres and mode are all ints
	if(argc != 4)
		cout << "usage: " << argv[0] << " <xres> <yres> <mode>" << endl;
	else
	{
		int xres = atoi(argv[1]);
		int yres = atoi(argv[2]);
		int mode = atoi(argv[3]);

		// ready the output file stream
		ofstream output_file;
		stringstream filename;
		// name every file to be:
		// ppm_test_<xres>x<yres>_mode<mode>.ppm
		filename << "ppm_test_" << xres << "x" << yres
								<< "_mode" << mode << ".ppm";
		string filename_str = filename.str();
		output_file.open(filename_str.c_str());

		// write header of the ppm file
		output_file << "P3" << endl;
		output_file << xres << " " << yres << endl;
		output_file << 255 << endl;

		// three instances of MatrixXd for pixel values
		MatrixXd red(xres, yres);
		MatrixXd green(xres, yres);
		MatrixXd blue(xres, yres);

		// depending on the mode, fill in MatrixXd red, green and blue
		// appropriately
		// if mode is 0, output a checkerboard image
		if(mode == 0)
		{
			// for every pixel, determine if the pixel is red or black by
			// dividing x and y by (xres / 8) and (yres / 8), respectively,
			// and checking the parity of the results
			int square_width = xres / 8;
			int square_height = yres / 8;
			for(int x = 0; x < xres; x++)
				for(int y = 0; y < yres; y++)
				{
					if((x / square_width) % 2 == (y / square_height) % 2)
					{
						red(x, y) = 255;
						green(x, y) = 0;
						blue(x, y) = 0;
					}
					else
					{
						red(x, y) = 0;
						green(x, y) = 0;
						blue(x, y) = 0;
					}
				}
		}
		// if mode is 1, output an image of a circle
		else if(mode == 1)
		{
			// for every pixel, determine if the pixel is in the circle
			// by using the inequality (x - xres / 2)^2 + (y - yres / 2)^2
			// <= r^2 where r is xres if xres < yres and yres if yres < xres
			int r;
			if(xres < yres)
				r = xres / 4;
			else
				r = yres / 4;
			for(int x = 0; x < xres; x++)
				for(int y = 0; y < yres; y++)
					if(pow(x - xres / 2, 2) + pow(y - yres / 2, 2) <= pow(r, 2))
					{
						red(x, y) = 255;
						green(x, y) = 0;
						blue(x, y) = 0;
					}
					else
					{
						red(x, y) = 0;
						green(x, y) = 0;
						blue(x, y) = 0;
					}
		}

		// write all pixels into output_file
		for(int y = 0; y < yres; y++)
			for(int x = 0; x < xres; x++)
				output_file << red(x, y) << " "
							<< green(x, y) << " "
							<< blue(x, y) << endl;

		// close file
		output_file.close();

		// print out filename
		cout << filename_str << endl;
	}
}
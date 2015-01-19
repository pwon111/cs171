*** README ***

SETUP:
	- The programs included in this assignment should all compile and run
	with no problems as long as the path to the Eigen library is changed in
	all Makefiles
		- This change is required for Parser and PPM
	- The Eigen library is located in the lib directory

OTHER INFO:
	- For the checkerboard part of the assignment, the code will run even
	if xres and yres are not multiples of 8. The code is written so that
	xres or yres that is not divisible by 8 will produce a very thin (less
	than 8 pixels wide/tall) extra col or row (respectively).
	- When you run ppm_test, it will output the image with the following
	name convention:
		ppm_test_<xres>x<yres>_mode<mode>.ppm

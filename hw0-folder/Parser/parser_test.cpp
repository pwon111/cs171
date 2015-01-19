/* You should edit this rather empty-looking file so that the program
 * does more than print out "Parsed successfully!"
 */

#include <iostream>
#include <string>
#include "ivdata.h"

IVData parse_iv(istream &datafile);

int main(int argc, char* argv[])
{
	// PART 1
	// parse ivdata
	IVData data = parse_iv(cin);
	cout << endl << "Parsed successfully!" << endl;

	// PART 2
	cout << endl << "CONTENTS OF PARSED DATA" << endl;
	// print out the contents of camera in data
	cout << "\tcamera orientation axis: " << data.camera.orientation_axis[0] << " "
										<< data.camera.orientation_axis[1] << " "
										<< data.camera.orientation_axis[2] << endl;
	cout << "\tcamera position: " << data.camera.position[0] << " "
								<< data.camera.position[1] << " "
								<< data.camera.position[2] << endl;
	cout << "\tcamera orientation angle: " << data.camera.orientation_angle << endl;
	cout << "\tcamera near: " << data.camera.near << endl;
	cout << "\tcamera far: " << data.camera.far << endl;
	cout << "\tcamera left: " << data.camera.left << endl;
	cout << "\tcamera right: " << data.camera.right << endl;
	cout << "\tcamera top: " << data.camera.top << endl;
	cout << "\tcamera bottom: " << data.camera.bottom << endl;

	// retrieve first object in data (we know there is only one in this data)
	Object object = data.objects.front();
	// print first faceset of object (we know there are only three nums per faceset)
	cout << "first faceset of first object: " << object.facesets[0][0] << " "
											<< object.facesets[0][1] << " "
											<< object.facesets[0][2] << endl;
	// print points of object with indices listed in the first faceset of object
	cout << "points referenced in this faceset:" << endl;
	for(vector<int>::iterator it = object.facesets[0].begin();
		it != object.facesets[0].end();
		++it)
	{
		cout << "\tpoint #" << *it << ": " << object.points[*it].x << " "
										<< object.points[*it].y << " "
										<< object.points[*it].z << endl;
	}

	// PART 3
	cout << endl << "MATRIX ALGEBRA EXAMPLE" << endl;
	// define S
	cout << "\tsum of all vectors in transforms of object is:" << endl;
	// currently assuming that transform matrices are 4 x 4
	MatrixXd S = MatrixXd::Zero(4, 4);
	for(vector<MatrixXd>::iterator it = object.transforms.begin();
		it != object.transforms.end();
		++it)
	{
		cout << *it << endl << endl;
		S += *it;
	}
	cout << S << endl;
	// define C
	cout << "\tdefined matrix C to be:" << endl;
	MatrixXd C(4,4);
	C << 4, 11, 7, 2,
		0, 5, 6, 7,
		1, 15, 12, 7,
		13, 0, 12, 10;
	cout << C << endl;
	// define P to be S * C
	cout << "\tdefine matrix P as S*C which is:" << endl;
	MatrixXd P;
	P = S * C;
	cout << P << endl;
	// inverse and transpose P
	cout << "\tinversing and transposing P results in:" << endl;
	P = P.inverse();
	P.transposeInPlace();
	cout << P << endl;
	// define v
	cout << "\tdefined a vector v to be:" << endl;
	VectorXd v(4, 1);
	v << 1, 4, 9, 16;
	cout << v << endl;
	// define A to be P * v
	cout << "\tdefine vector A as P*v which is:" << endl;
	VectorXd A(4, 1);
	A = P * v;
	cout << A << endl;
	// normalize A and dot product it with itself (i.e. dot product A.norm() and
	// A.norm())
	A /= A.norm();
	double ANS = A.dot(A);
	cout << "\tnormalized A dot producted with itself is: " << ANS << endl;
}
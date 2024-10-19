#include "matrix4x4.hpp"
#include <iostream>

template <typename TType>
std::ostream&	operator<<(std::ostream &ostream, const Matrix4x4<TType> &obj)
{
	for (size_t i = 0; i < 4; i++) {
		ostream << "| ";
		for (size_t j = 0; j < 4; j++) {
			ostream << obj.getElem(i, j) << " ";
		}
		ostream << "|\n";
	}
	ostream << std::flush;
	return (ostream);
}

int main()
{
	try {
		Matrix4x4<double>	myMat0(static_cast<MatrixType>(7));
	} catch (const std::exception& e) {
		std::cout << "Caught error: " << e.what() << '\n' << std::endl;
	}

	// This line should prevent compilation
//	Matrix4x4<std::string>	myMatStr;

	Matrix4x4<int>	myMat1({
		2,	7,	9,	3,
		5,	12,	1,	8,
		7,	11,	0,	10,
		4,	6,	13,	42
	});
	std::cout << "Mat1\n" << myMat1 << std::endl;

	Matrix4x4<int>	id(MatrixType::MAT_ID);
	std::cout << "id (default)\n" << id << std::endl;

	if (id == myMat1)
		std::cout << "id == Mat1\n" << std::endl;
	else if (id != myMat1)
		std::cout << "id != Mat1\n" << std::endl;

	Matrix4x4<int>	id2(id);
	std::cout << "id (default)\n" << id << std::endl;

	if (id == id2)
		std::cout << "id == id2\n" << std::endl;
	else if (id != id2)
		std::cout << "id != id2\n" << std::endl;

	Matrix4x4<int>	mat1plusid = myMat1 + id;
	std::cout << "Mat1 + id\n" << mat1plusid << std::endl;

	Matrix4x4<int>	mat1minusid = myMat1 - id;
	std::cout << "Mat1 - id\n" << mat1minusid << std::endl;

	Matrix4x4<int>	mat1x2 = 2 * myMat1;
	std::cout << "2 * Mat1\n" << mat1x2 << std::endl;

	Matrix4x4<int>	mat1xid = myMat1 * id;
	std::cout << "Mat1 * id\n" << mat1xid << std::endl;

	Matrix4x4<int>	idxmat1 = id * myMat1;
	std::cout << "id * Mat1\n" << idxmat1 << std::endl;

	Matrix4x4<float>	mat1div2 = myMat1 / 2;
	std::cout << "Mat1 / 2\n" << mat1div2 << std::endl;

	Matrix4x4<int>	mat1mod3 = myMat1 % 3;
	std::cout << "Mat1 % 3\n" << mat1mod3 << std::endl;

	Matrix4x4<float>	myMat2({
		2.0,	7.0,	9.5,	3.1,
		5.0,	12.0,	1.5,	8.2,
		7.5,	11.0,	0.5,	10.0,
		4.2,	6.1,	13.13,	42.42
	});
	std::cout << "Mat2\n" << myMat2 << std::endl;

	Matrix4x4<float>	mat2x3 = myMat2 * 3.0;
	std::cout << "Mat2 * 3.0\n" << mat2x3 << std::endl;

	Matrix4x4<float>	mat2div2 = myMat2 / 2.0;
	std::cout << "Mat2 / 2.0\n" << mat2div2 << std::endl;

	// This line should prevent compilation
//	Matrix4x4<float>	mat2mod2 = myMat2 % 2.0;
//	std::cout << "Mat2 / 2.0\n" << mat2mod2 << std::endl;

	Matrix4x4<int>	myMat3({
		1,	8,	2,	10,
		4,	8,	40,	1,
		5,	1,	12,	6,
		6,	13,	37,	1
	});
	std::cout << "Mat3\n" << myMat3 << std::endl;

	try {
		myMat3.setElem(4, 4, 7);
	} catch (const std::exception& e) {
		std::cout << "Caught error: " << e.what() << '\n' << std::endl;
	}

	try {
		const int i = myMat3.getElem(4, 4);
		(void)i;
	} catch (const std::exception& e) {
		std::cout << "Caught error: " << e.what() << '\n' << std::endl;
	}

	Matrix4x4<int>	mat3trans = myMat3.transpose();
	std::cout << "Mat3 (transposed)\n" << mat3trans << std::endl;

	Matrix4x4<int>	mat1xmat3 = myMat1 * myMat3;
	std::cout << "Mat1 * Mat3\n" << mat1xmat3 << std::endl;

	Matrix4x4<int>	mat3xmat1 = myMat3 * myMat1;
	std::cout << "Mat3 * Mat1\n" << mat3xmat1 << std::endl;

	Matrix4x4<float>	mat1_3average = Matrix4x4<int>::average(myMat1, myMat3);
	std::cout << "Average of Mat1 and Mat3\n" << mat1_3average << std::endl;

	Matrix4x4<int>	mat1_3compmult = Matrix4x4<int>::compMult(myMat1, myMat3);
	std::cout << "Component multiplication of Mat1 and Mat3\n" << mat1_3compmult << std::endl;

	try {
		Matrix4x4<int>	myMat4({
			1,	8,	2,
			4,	8,	40
		});
	} catch (const std::exception& e) {
		std::cout << "Caught error: " << e.what() << std::endl;
	}

	return 0;
}

#include <fstream>
#include <../zjucad/matrix/matrix.h>
#include <../jtflib/mesh/include/io.h>
#include "../vtk.h"

using namespace std;
using namespace jtf::mesh;
using namespace zjucad::matrix;

int tet_mesh_read_from_txt(const char* path, matrixd* node, matrixst* tet)
{
	ifstream ifs(path);
	if ( ifs.fail() )
	{
		cerr << "[INFO] " << "can not open the file" << path << endl;
		return 0;
	}
	int row1, column1, row2, column2;
	
	ifs >> row1 >> column1;
	(*node).resize(row1, column1);
	for (int j = 0; j < column1; j++)
	{
		for (int i = 0; i < row1; i++)
		{
			double dat;
			ifs >> dat;
			(*node)(i, j) = dat;
		}
	}
	
	ifs >> row2 >> column2;
	(*tet).resize(row2, column2);
	for (int j = 0; j < column2; j++)
	{
		for (int i = 0; i < row2; i++)
		{
			int dat;
			ifs >> dat;
			(*tet)(i, j) = dat;
		}
	}
	return 1;

}

int main()
{
	matrixd node;
	matrixst tet;
	matrixst tri;

	tet_mesh_read_from_txt("../fandisk_4k_polycube_after_split.tet.txt", &node, &tet);
//	tet_mesh_read_from_zjumat("../fandisk_4k_polycube_after_split.tet.txt", &node, &tet, &tri);

	ofstream ofs("../tet_mesh.vtk");
	
	tet2vtk(ofs, &node[0], node.size(2), &tet[0], tet.size(2));

	return 0;

}
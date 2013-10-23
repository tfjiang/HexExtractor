#ifndef HEX_EXTRATOR_H
#define HEX_EXTRATOR_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <hjlib/math/blas_lapack.h>
#include <zjucad/matrix/matrix.h>
#include <zjucad/matrix/lapack.h>

using namespace std;
using namespace zjucad::matrix;

typedef struct {
	double xmin, xmax, ymin, ymax, zmin, zmax;
}BOUNDING_BOX;

typedef struct {
	int x, y, z;
}INT_POINT;

typedef vector<vector<INT_POINT> > Imatrix;
typedef matrix<double> matrixd;
typedef matrix<size_t> matrixst;

int tet_mesh_read_from_txt(const char *path, matrixd *node, matrixst *tet, Imatrix &int_point)
{
	ifstream ifs(path);
	if ( ifs.fail() ) {
		cerr << "[INFO] " << "can not open file" << endl;
		return 0;
	}
	int row1, row2, column1, column2;
	ifs >> row1 >> column1;
	(*node).resize(row1, column1);
	for (int j = 0; j < column1; j++) {
		for (int i = 0; i < row1; i++) {
			double dat;
			ifs >> dat;
			(*node)(i, j) = dat;
		}
	}

	ifs >> row2 >> column2;
	(*tet).resize(row2, column2);
	int_point.resize(column2);
	for (int j = 0; j < column2; j++) {
		for (int i = 0; i < row2; i++) {
			int dat;
			ifs >> dat;
			(*node)(i, j) = dat;
		}
	}

	return 1;
}

void bounding_box(int id, matrixd *node, matrixst *tet, BOUNDING_BOX &bound)
{
	vector<double> xaxis;
	vector<double> yaxis;
	vector<double> zaxis;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++)
		switch (j) {
			case 0 : xaxis.push_back((*node)(j, (*tet)(i, id))); break;
			case 1 : yaxis.push_back((*node)(j, (*tet)(i, id))); break;
			case 2 : zaxis.push_back((*node)(j, (*tet)(i, id))); break;
		}
	}

	bound.xmin = *min_element(xaxis.begin(), xaxis.end()); 
	bound.ymin = *min_element(yaxis.begin(), yaxis.end());
	bound.zmin = *min_element(zaxis.begin(), zaxis.end());
	bound.xmax = *max_element(xaxis.begin(), xaxis.end());
	bound.ymax = *max_element(yaxis.begin(), yaxis.end());
	bound.zmax = *max_element(zaxis.begin(), zaxis.end());

}

bool contains(int x, int y, int z, int id, matrixd *node, matrixst *tet)
{
	/* goal:solve linear system: Mx = p, M^T * M is invertiable 
	   only when the tet is non-degenerate, attention here! */
	matrixd M(3, 4);
	matrixd p(3, 1);
	matrixd a(4, 1);
	p(0, 0) = x; p(1, 0) = y; p(2, 0) = z;
	for (int i = 0; i < 4; i++)
		M(colon(), i) = (*node)(colon(), (*tet)(i, id));
	matrixd MTM_inv = trans(M) * M;
	if(inv(MTM_inv)){
			cerr << "# [error] inv fail." << endl;
	}
	a = MTM_inv * trans(M) * p;
	for (int i = 0; i < 4; i++)
		if ( a(i, 0) < 0 )
			return false;
	return true;
}

void integer_grid_in_tet(int id, matrixd *node, matrixst *tet, Imatrix &int_point)
{
	BOUNDING_BOX bound;
	bounding_box(id, node, tet, bound);
	int cnt = 0;
	for (int x = (int)ceil(bound.xmin); x <= (int)floor(bound.xmax); x++) {
		for (int y = (int)ceil(bound.ymin); y <= (int)floor(bound.ymax); y++) {
			for(int z = (int)ceil(bound.zmin); z <= (int)floor(bound.zmax); z++) {
				if ( contains(x, y, z, id, node, tet) ) {
					INT_POINT tmp;
					tmp.x = x; tmp.y = y; tmp.z = z;
					int_point[id].push_back(tmp);
				}
			}
		}
	}
}

void generate_all_integer_grid(matrixd *node, matrixst *tet, Imatrix &int_point)
{
		for (int id = 0; id < (*tet).size(2); id++)
		integer_grid_in_tet(id, node, tet, int_point);
}

void print(const char *outfile, Imatrix &int_point)
{
	ofstream out(outfile);
	for (int i = 0; i < int_point.size(); i++) {
			for (int j = 0; j < int_point[i].size(); j++)
			out << "(" << 
				int_point[i][j].x << "," << 
				int_point[i][j].y << "," <<
				int_point[i][j].z << ")" << endl;
	}
}


#endif

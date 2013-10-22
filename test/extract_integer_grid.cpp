#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <zjucad/ptree/include/ptree.h>
#include <zjucad/matrix/matrix.h>

#include "../src/hex_extractor.h"

using namespace std;
using namespace zjucad::matrix;
using boost::property_tree::ptree;

int main(int argc, char *argv[])
{
  
  ptree pt;

  try {

  	string input_file = pt.get<string>("input_file.value");

  	matrixd node;
  	matrixst tet;
  	Imatrix int_point;

    tet_mesh_read_from_txt(input_file.c_str(), &node, &tet, int_point);

  	generate_all_integer_grid(&node, &tet, int_point);

  	print("../text.txt", int_point);


  }catch(std::exception &e){
    cerr << endl;
    cerr << "[error] "<<e.what() << endl;
    cerr << "Usage:" << endl;
    zjucad::show_usage_info(std::cerr, pt);
  }

  return 0;
}

#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <map>
#include <iostream>
#include <zjucad/matrix/matrix.h>

namespace extractor
{
  class extractor
  {
  public:
    virtual ~extractor(){}
    ///
    /// @brief extract      to extract quad/hex from a tri/tet mesh
    /// @param input_mesh   input mesh, either will be tri/tet
    /// @param param_node   mesh node in parameterizaton space
    /// @param orig_node    mesh node in original space
    /// @param inner_type   inner transition type defined on each two cells
    /// @param output_mesh  output mesh quad/hex
    /// @param output_node  node of output mesh
    /// @return             0 if works fine, or non-zeros
    ///
    virtual int extract(const zjucad::matrix::matrix<size_t> & input_mesh,
                        const zjucad::matrix::matrix<double> &param_node,
                        const zjucad::matrix::matrix<double> & orig_node,
                        const std::map<std::pair<size_t,size_t>,size_t> & inner_type,
                        zjucad::matrix::matrix<size_t> & output_mesh,
                        zjucad::matrix::matrix<double> & output_node) = 0;
  protected:
    ///
    /// @brief extract_integer_points extract integer points, each should be
    ///                               associated with its coordinates and
    ///                               its tet/tri idx, and its barycentric coordinate
    /// @param param_mesh             input param mesh
    /// @param param_nodes            input param nodes
    /// @return                       0 if works fine, or non-zeros
    ///
    virtual int extract_integer_points(const zjucad::matrix::matrix<size_t> &param_mesh,
                                       const zjucad::matrix::matrix<double> & param_nodes) = 0;

    ///
    /// @brief extract_integer_lines extract integer lines according to integer
    ///                              points and transition on each faces (tet)
    /// @param param_mesh            input mesh
    /// @param param_nodes           input param node
    /// @param inner_type            transition type defined on each two cells (cells should be ordered)
    /// @return                      0 if works fine, or non-zeros
    ///
    virtual int extract_integer_lines(const zjucad::matrix::matrix<size_t> & param_mesh,
                                      const zjucad::matrix::matrix<double> &param_nodes,
                                      const std::map<std::pair<size_t,size_t>, size_t> & inner_type) = 0;

    ///
    /// @brief extract_mesh         extract mesh from integer lines
    /// @param param_mesh           input param mesh
    /// @param param_nodes          input nodes
    /// @return                     0 if works fine, or non-zeros
    ///
    virtual int extract_mesh(const zjucad::matrix::matrix<size_t> & param_mesh,
                             const zjucad::matrix::matrix<double> &param_nodes) = 0;

    ///
    /// @brief mapping_to_orig_mesh  map extracted mesh to original space
    /// @param orig_mesh             input origin mesh
    /// @param orig_node             input origin node
    /// @return
    ///
    virtual int mapping_to_orig_space(const zjucad::matrix::matrix<size_t> & orig_mesh,
                                      const zjucad::matrix::matrix<double> & orig_node) = 0;
  };


  class hex_extractor : public extractor
  {
  public:
    ///
    /// @brief extract      to extract quad/hex from a tri/tet mesh
    /// @param input_mesh   input mesh, either will be tri/tet
    /// @param param_node   mesh node in parameterizaton space
    /// @param orig_node    mesh node in original space
    /// @param output_mesh  output mesh quad/hex
    /// @param output_node  node of output mesh
    /// @return             0 if works fine, or non-zeros
    ///
    virtual int extract(const zjucad::matrix::matrix<size_t> & input_mesh,
                        const zjucad::matrix::matrix<double> &param_node,
                        const zjucad::matrix::matrix<double> & orig_node,
                        const std::map<std::pair<size_t,size_t>,size_t> & inner_type,
                        zjucad::matrix::matrix<size_t> & output_mesh,
                        zjucad::matrix::matrix<double> & output_node)
    {
      if(extract_integer_points(input_mesh, param_node)) {
          std::cerr << "# [error] extract inerger_points fail." << std::endl;
          return __LINE__;
        }
      if(extract_integer_lines(input_mesh, param_node,inner_type)) {
          std::cerr << "# [error] extract inerger_lines fail." << std::endl;
          return __LINE__;
        }
      if(extract_mesh(input_mesh, param_node)) {
          std::cerr << "# [error] extract mesh fail." << std::endl;
          return __LINE__;
        }
      if(mapping_to_orig_space(input_mesh, param_node)) {
          std::cerr << "# [error] mappint to orig space fail." << std::endl;
          return __LINE__;
        }

      return 0;
    }
  protected:
    ///
    /// @brief extract_integer_points extract integer points, each should be
    ///                               associated with its coordinates and
    ///                               its tet/tri idx, and its barycentric coordinate
    /// @param param_mesh             input param mesh
    /// @param param_nodes            input param nodes
    /// @return                       0 if works fine, or non-zeros
    ///
    virtual int extract_integer_points(const zjucad::matrix::matrix<size_t> &param_mesh,
                                       const zjucad::matrix::matrix<double> & param_nodes);

    ///
    /// @brief extract_integer_lines extract integer lines according to integer
    ///                              points and transition on each faces (tet)
    /// @param param_mesh            input mesh
    /// @param param_nodes           input param node
    /// @param inner_type            transition type defined on each two cells (cells should be ordered)
    /// @return                      0 if works fine, or non-zeros
    ///
    virtual int extract_integer_lines(const zjucad::matrix::matrix<size_t> & param_mesh,
                                      const zjucad::matrix::matrix<double> &param_nodes,
                                      const std::map<std::pair<size_t,size_t>, size_t> & inner_type);

    ///
    /// @brief extract_mesh         extract mesh from integer lines
    /// @param param_mesh           input param mesh
    /// @param param_nodes          input nodes
    /// @return                     0 if works fine, or non-zeros
    ///
    virtual int extract_mesh(const zjucad::matrix::matrix<size_t> & param_mesh,
                             const zjucad::matrix::matrix<double> &param_nodes);

    ///
    /// @brief mapping_to_orig_mesh  map extracted mesh to original space
    /// @param orig_mesh             input origin mesh
    /// @param orig_node             input origin node
    /// @return
    ///
    virtual int mapping_to_orig_space(const zjucad::matrix::matrix<size_t> & orig_mesh,
                                      const zjucad::matrix::matrix<double> & orig_node);
  };
}

#endif

#include "element/LinQuad.hpp"
#include <cmath>

YAFEL_NAMESPACE_OPEN

LinQuad::LinQuad(const DoFManager &dofm) : Element(dofm, 2, 4, 4*dofm.getDofPerNode(), 9, 4)
{

  Vector v(n_spaceDim,0.0);
  
  //set up parent element xi_0 vectors
  xi_0.clear();
  v(0) = -1; v(1) = -1; xi_0.push_back(v);
  v(0) = 1; v(1) = -1; xi_0.push_back(v);
  v(0) = 1; v(1) = 1; xi_0.push_back(v);
  v(0) = -1; v(1) = 1; xi_0.push_back(v);

  //assign gauss points and weights
  quad_points.clear();
  gauss_weights.clear();
  double a = 1.0/sqrt(3.0);
  v(0) = -a; v(1) = -a; quad_points.push_back(v); gauss_weights.push_back(1.0);
  v(0) = a; v(1) = -a; quad_points.push_back(v); gauss_weights.push_back(1.0);
  v(0) = a; v(1) = a; quad_points.push_back(v); gauss_weights.push_back(1.0);
  v(0) = -a; v(1) = a; quad_points.push_back(v); gauss_weights.push_back(1.0);
}

double LinQuad::shape_value_xi(unsigned node, const Vector &xi) const {
  
  double val = 1.0;
  for(unsigned i=0; i<n_spaceDim; ++i) {
    val *= (1.0/2.0) * (xi_0[node](i)*xi(i) + 1);
  }
  
  return val;
}


YAFEL_NAMESPACE_CLOSE

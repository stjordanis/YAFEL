#ifndef _YAFEL_LINQUAD_HPP
#define _YAFEL_LINQUAD_HPP

#include "yafel_globals.hpp"
#include "element/Element.hpp"
#include "lin_alg/Vector.hpp"
#include "utils/DoFManager.hpp"
#include "utils/ElementType.hpp"
#include <vector>
#include <type_traits>
#include <cmath>

YAFEL_NAMESPACE_OPEN

template<unsigned NSD>
class LinQuad : public Element<NSD> {

public:
    using size_type = typename Element<NSD>::size_type;
    using coordinate_type = typename Element<NSD>::coordinate_type;

    template<typename = typename std::enable_if<NSD>=2>::type>
    LinQuad(const DoFManager &dofm) : Element<NSD>(dofm, ElementType::LINEAR_QUAD, 
                                                   2, 4, 4*dofm.dof_per_node(), 9, 4) {
        double a = 1.0/sqrt(3.0);
        this->quad_points.clear();
        this->quad_weights.resize(4,1.0);
        this->xi_0.clear();
    
        //set up parent element xi_0 vectors
        this->xi_0.emplace_back(coordinate_type{-1, -1});
        this->xi_0.emplace_back(coordinate_type{ 1, -1});
        this->xi_0.emplace_back(coordinate_type{ 1,  1});
        this->xi_0.emplace_back(coordinate_type{-1,  1});
    
        //assign gauss points and weights
        this->quad_points.emplace_back(coordinate_type{-a, -a});
        this->quad_points.emplace_back(coordinate_type{ a, -a});
        this->quad_points.emplace_back(coordinate_type{ a,  a});
        this->quad_points.emplace_back(coordinate_type{-a,  a});
    }
    
    double shape_value_xi(size_type node, const coordinate_type &xi) const 
    {
        //double val = 1.0;
        // topological dimension := 2, so hard-code to let compiler unroll loop
        // old loop limit: this->n_topoDim
        //for(unsigned i=0; i<2; ++i) { 
        //val *= (1.0/2.0) * (this->xi_0[node](i)*xi(i) + 1);
        //}
        
        return (this->xi_0[node](0)*xi(0) + 1)*(this->xi_0[node](1)*xi(1) + 1)/4.0;
        
        //return val;
    }
    
    double shape_grad_xi(size_type node, size_type component, const coordinate_type &xi) const {
        //double val = 1.0;
        // topological dimension := 2, so hard-code to let compiler unroll loop
        //for(size_type i=0; i<2; ++i) { // old loop limit: this->n_topoDim
        //if(component == i) {
        //        continue;
        //      }
        //      val *= (1.0/2.0) * ( this->xi_0[node](i)*xi(i) + 1 );
        //    }
        //return val * (this->xi_0[node](component))/(2.0);
        
        return (this->xi_0[node](1-component)*xi(1-component) + 1)
            *(this->xi_0[node](component)) / 4.0;
    }
    
};


    YAFEL_NAMESPACE_CLOSE

#endif
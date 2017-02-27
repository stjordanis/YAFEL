//
// Created by tyler on 2/21/17.
//

#include "lin_alg/new_tensor/Tensor.hpp"
#include "lin_alg/new_tensor/tensor_expression_types/TensorAddition.hpp"
#include "lin_alg/new_tensor/tensor_expression_types/TensorSubtraction.hpp"
#include "lin_alg/new_tensor/tensor_expression_types/TensorScaled.hpp"
#include "lin_alg/new_tensor/tensor_expression_types/TensorSlice.hpp"
#include "lin_alg/new_tensor/tensor_expression_types/TensorPermutation.hpp"
#include "lin_alg/new_tensor/tensor_expression_types/TensorContraction.hpp"
#include "lin_alg/new_tensor/tensor_functions/tensor_dot.hpp"
#include "lin_alg/new_tensor/mp_utils/sequence_functions.hpp"
#include "lin_alg/new_tensor/mp_utils/slice_mp_utils.hpp"

#include <iostream>

using namespace yafel;
using namespace std;

double do_contract2(double a)
{
    Tensor<3, 1> lhs;
    Tensor<3, 3> rhs;

    for (auto &li : lhs) {
        li = a;
        a *= 1.1;
    }
    for (auto &ri : rhs) {
        ri = a;
        a /= 1.2;
    }

    Tensor<3, 2, double> res;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            res(i,j) = dot(lhs(colon()), rhs(colon(), i, j));
        }
    }

    return dot(res, res);
}

double do_contract(double a)
{

    Tensor<3, 1> lhs;
    Tensor<3, 3> rhs;

    for (auto &li : lhs) {
        li = a;
        a *= 1.1;
    }
    for (auto &ri : rhs) {
        ri = a;
        a /= 1.2;
    }

    Tensor<3, 2, double> res = contract(lhs, rhs, sequence<1>());

    return contract(res,res,sequence<2>());
}


int main()//(int argc, char **argv)
{

    double a = 2.5;

    cout << do_contract(a) << "  " << do_contract2(a) << endl;
    return 0;
}
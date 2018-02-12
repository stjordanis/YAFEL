//
// Created by tyler on 5/9/17.
//

#ifndef YAFEL_LINEARELASTICITY_HPP
#define YAFEL_LINEARELASTICITY_HPP

#include "yafel_globals.hpp"

YAFEL_NAMESPACE_OPEN

template<int NSD>
struct LinearElasticity
{
    static constexpr int nsd() { return NSD; }

    static constexpr double Youngs{200.0e0};
    static constexpr double nu{0.3};

    static constexpr double lambda = (Youngs * nu) / ((1 + nu) * (1 - 2 * nu));
    static constexpr double mu = Youngs / (2 * (1 + nu));


    static void
    LocalResidual(const Element &E, int qpi, coordinate<> &, double,
                  Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, 1>> &u_el,
                  Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, 1>> &R_el)
    {
        for (auto A : IRange(0, static_cast<int>(R_el.rows()))) {
            R_el(A) += 0*10 * E.shapeValues[qpi](A / NSD) * E.jxw;
        }
    }

    static void LocalTangent(const Element &E, int, coordinate<> &, double,
                             Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, 1>> &u_el,
                             Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> &K_el)
    {

        //note permuted indices in tensor functor. this is so that the "j" and "l"
        // indices are adjacent in memory, which is necessary for the later operations
        Tensor<NSD, 4> Cikjl = make_TensorFunctor<NSD, 4, double>(
                [](int i, int k, int j, int l) {
                    auto lambda = LinearElasticity<NSD>::lambda;
                    auto mu = LinearElasticity<NSD>::mu;
                    return lambda * (i == j) * (k == l) + mu * ((i == k) * (j == l) + (i == l) * (j == k));
                }
        );


        const int nNodes = K_el.rows() / NSD;
        //loops attempting strength reduction by inserting more loops!
        int A{0};
        for (int Anode = 0; Anode < nNodes; ++Anode) {
            auto grad_Aj = make_TensorMap<NSD, 1>(&E.shapeGrad(Anode, 0));

            for (int i = 0; i < NSD; ++i) {
                int B{Anode * NSD};

                //Diagonal block
                auto tmp = otimes(grad_Aj, grad_Aj).eval();
                for (int k = 0; k < NSD; ++k) {
                    K_el(A, B) -= dot(Cikjl(i, k, colon(), colon()).eval(), tmp) * E.jxw;
                    ++B;
                }

                //Off-diagonal blocks
                for (int Bnode = Anode + 1; Bnode < nNodes; ++Bnode) {
                    auto grad_Bl = make_TensorMap<NSD, 1>(&E.shapeGrad(Bnode, 0));
                    auto tmp = otimes(grad_Aj, grad_Bl).eval();
                    //for (auto k : IRange(0, NSD)) {
                    for (int k = 0; k < NSD; ++k) {
                        K_el(A, B) -= dot(Cikjl(i, k, colon(), colon()).eval(), tmp) * E.jxw;
                        K_el(B, A) = K_el(A, B);

                        ++B;
                    }
                }

                ++A;
            }
        }

    }
};


YAFEL_NAMESPACE_CLOSE


#endif //YAFEL_LINEARELASTICITY_HPP

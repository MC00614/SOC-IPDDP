#include <eigen3/Eigen/Dense>
#include <iostream>

#include <cmath>

#include "soc_ipddp.h"
#include "model.h" 
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main() {
    // Intitial Setting
    auto model = InvPend();
    
    // Solver
    SOC_IPDDP soc_ipddp(model);

    soc_ipddp.init(100, 1e-2);

    soc_ipddp.solve();

    // Result
    Eigen::MatrixXd X_result = soc_ipddp.getX();
    Eigen::MatrixXd U_result = soc_ipddp.getU();
    std::vector<double> all_cost = soc_ipddp.getAllCost();




    // // // // // // // // // // // // // // // // // // // // // // // // // 
    //  VISUALIZATION  // VISUALIZATION  // VISUALIZATION  // VISUALIZATION // 
    // // // // // // // // // // // // // // // // // // // // // // // // // 
    int dim_x = X_result.rows();
    int dim_u = U_result.rows();
    std::vector<std::vector<double>> X_INIT(X_result.rows(), std::vector<double>(X_result.cols()));
    std::vector<std::vector<double>> X_RES(X_result.rows(), std::vector<double>(X_result.cols()));
    for (int i = 0; i < X_result.rows(); ++i) {
        for (int j = 0; j < X_result.cols(); ++j) {
            X_INIT[i][j] = model.X(i,j);
            X_RES[i][j] = X_result(i, j);
        }
    }

    std::vector<std::vector<double>> U_INIT(U_result.rows(), std::vector<double>(U_result.cols()));
    std::vector<std::vector<double>> U_RES(U_result.rows(), std::vector<double>(U_result.cols()));
    for (int i = 0; i < U_result.rows(); ++i) {
        for (int j = 0; j < U_result.cols(); ++j) {
            U_INIT[i][j] = model.U(i,j);
            U_RES[i][j] = U_result(i, j);
        }
    }

    for (size_t i = 0; i < X_RES.size(); ++i) {
        plt::subplot(dim_x + dim_u + 1, 1, i + 1);
        plt::plot(X_INIT[i], {{"label", "Init"}});
        plt::plot(X_RES[i], {{"label", "Result"}});
        plt::title("X_result Dimension " + std::to_string(i));
        plt::legend();
    }

    for (size_t i = 0; i < U_RES.size(); ++i) {
        plt::subplot(dim_x + dim_u + 1, 1, dim_x + 1 + i);
        plt::plot(U_INIT[i], {{"label", "Init"}});
        plt::plot(U_RES[i], {{"label", "Result"}});
        plt::title("U_result Dimension " + std::to_string(i));
        plt::legend();
    }

    plt::subplot(dim_x + dim_u + 1, 1, dim_x + dim_u + 1);
    plt::plot(all_cost, {{"label", "Cost"}});
    plt::title("Cost");
    plt::legend();

    plt::show();
    // // // // // // // // // // // // // // // // // // // // // // // // // 
    //  VISUALIZATION  // VISUALIZATION  // VISUALIZATION  // VISUALIZATION // 
    // // // // // // // // // // // // // // // // // // // // // // // // // 

    return 0;
}
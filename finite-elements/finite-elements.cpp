#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include "Matrix.h"

using namespace std;

using Task = map<int, vector<int>>;

Matrix local_matrix_heat(double lambdaX, double lambdaY, vector<double> b, vector<double> c, double S) {
    Matrix result(3, 3);

    for (size_t i = 0; i < 3; i++)
    {
        result.At(0, i) = lambdaX * S * b[0] * b[i] + lambdaY * S * c[0] * c[i];
        result.At(1, i) = lambdaX * S * b[1] * b[i] + lambdaY * S * c[1] * c[i];
        result.At(2, i) = lambdaX * S * b[2] * b[i] + lambdaY * S * c[2] * c[i];
    }
    return result;
}

std::tuple<Matrix, Matrix> write_system_equations(const Matrix& g1, const Matrix& g2, 
    const Matrix& g3, const Matrix& g4, const Matrix& f1, const Matrix& f2, 
    const Matrix& f3, const Matrix& f4) {
    Matrix G(6, 6);

    G.At(0, 0) = g1.At(0, 0);
    G.At(0, 1) = g1.At(0, 1);
    G.At(0, 3) = g1.At(0, 2);

    G.At(1, 0) = g1.At(1, 0);
    G.At(1, 1) = g1.At(1, 1) + g2.At(0, 0) + g3.At(0, 0);
    G.At(1, 2) = g3.At(0, 1);
    G.At(1, 3) = g1.At(1, 2) + g2.At(0, 2);
    G.At(1, 4) = g2.At(0, 1) + g3.At(0, 2);

    G.At(2, 1) = g3.At(1, 0);
    G.At(2, 2) = g3.At(1, 1);
    G.At(2, 4) = g3.At(1, 2);

    G.At(3, 0) = g3.At(2, 0);
    G.At(3, 1) = g1.At(2, 1) + g2.At(2, 0);
    G.At(3, 3) = g1.At(2, 2) + g2.At(2, 2) + g4.At(0, 0);
    G.At(3, 4) = g2.At(2, 1) + g4.At(0, 1);
    G.At(3, 5) = g4.At(0, 2);

    G.At(4, 1) = g2.At(1, 0) + g3.At(2, 0);
    G.At(4, 2) = g3.At(2, 1);
    G.At(4, 3) = g2.At(1, 2) + g4.At(1, 0);
    G.At(4, 4) = g2.At(1, 1) + g3.At(2, 2) + g4.At(1, 1);
    G.At(4, 5) = g4.At(1, 2);

    G.At(5, 3) = g4.At(2, 0);
    G.At(5, 4) = g4.At(2, 1);
    G.At(5, 5) = g4.At(2, 2);

    Matrix F(6, 1);

    F.At(0, 0) = f1.At(0, 0);
    F.At(1, 0) = f1.At(0, 0) + f2.At(0, 0) + f3.At(0, 0);
    F.At(2, 0) = f3.At(1, 0);

    F.At(3, 0) = f1.At(2, 0) + f2.At(2, 0) + f4.At(0, 0);
    F.At(4, 0) = f2.At(1, 0) + f3.At(2, 0) + f4.At(1, 0);
    F.At(5, 0) = f4.At(2, 0);
    
    return make_tuple(G, F);
}

Matrix solve_system_equations(const Matrix& G, const Matrix& F) {
    Matrix U(6, 1);

    Matrix invG(6, 6);

    // ...

    return U;
}

int main() {
    double lambdaX = 20, lambdaY = 20, alfa1 = 250, q1 = 10000, T = 473, Q_v = 100000;
    const int num_nodes = 6;
    vector<Matrix> local_matrices_heat;

    // Task odject
    // key: int - number of element
    // value: vector<int> - numeration of nodes of each element
    
    Task task = {
        {1, {1, 2, 4}},
        {2, {2, 5, 4}},
        {3, {2, 3, 5}},
        {4, {4, 5, 6}},
    };

    // Coordinates of nodes
    vector<double> X = {
        0, 0.01, 0.02, 0.01, 0.02, 0.02
    };
    vector<double> Y = {
        0, 0, 0, 0.01, 0.01, 0.02
    };

    // Lengths of edges
    vector<vector<double>> L;
    vector<double> S;
    vector<vector<double>> b;
    vector<vector<double>> c;


    for (auto element : task) {
        int i = element.second[0] - 1;
        int j = element.second[1] - 1;
        int k = element.second[2] - 1;

        L.push_back({
            sqrt(pow(X[i] - X[j], 2) + pow(Y[i] - Y[j], 2)),
            sqrt(pow(X[j] - X[k], 2) + pow(Y[j] - Y[k], 2)),
            sqrt(pow(X[k] - X[i], 2) + pow(Y[k] - Y[i], 2))
            });

        S.push_back(
            0.5 * (X[j] * Y[k] - X[k] * Y[j] + X[i] * Y[j] - X[i] * Y[k] + X[k] * Y[i] - X[j] * Y[i])
        );

        int index_elem = element.first - 1;
        b.push_back({
            (Y[j] - Y[k]) / (2 * S[index_elem]),
            (Y[k] - Y[i]) / (2 * S[index_elem]),
            (Y[i] - Y[j]) / (2 * S[index_elem])
        });

        c.push_back({
            (X[k] - X[j]) / (2 * S[index_elem]),
            (X[i] - X[k]) / (2 * S[index_elem]),
            (X[j] - X[i]) / (2 * S[index_elem])
        });
    }

    // definition of local matrices heat
    Matrix g1 = local_matrix_heat(lambdaX, lambdaY, b[0], c[0], S[0]);
    Matrix g2 = local_matrix_heat(lambdaX, lambdaY, b[1], c[1], S[1]);
    Matrix g3 = local_matrix_heat(lambdaX, lambdaY, b[2], c[2], S[2]);

    g3.At(1, 1) += 2 * alfa1 * L[2][1] / 6;
    g3.At(1, 2) += 1 * alfa1 * L[2][1] / 6;
    g3.At(2, 1) += 1 * alfa1 * L[2][1] / 6;
    g3.At(2, 2) += 2 * alfa1 * L[2][1] / 6;

    Matrix g4 = local_matrix_heat(lambdaX, lambdaY, b[3], c[3], S[3]);
    g4.At(1, 1) += 2 * alfa1 * L[3][1] / 6;
    g4.At(1, 2) += 1 * alfa1 * L[3][1] / 6;
    g4.At(2, 1) += 1 * alfa1 * L[3][1] / 6;
    g4.At(2, 2) += 2 * alfa1 * L[3][1] / 6;

    Matrix f1(3, 1);
    Matrix f2(3, 1);
    Matrix f3(3, 1);
    Matrix f4(3, 1);

    for (size_t i = 0; i < 3; i++)
    {        
        f1.At(i, 0) = Q_v * S[0] / 3;
        f2.At(i, 0) = 0;
    }
    f3.At(0, 0) = q1 * L[2][0] / 2;
    f3.At(1, 0) = q1 * L[2][0] / 2 + T * alfa1 * L[2][1] / 2;
    f3.At(2, 0) = T * alfa1 * L[2][1] / 2;

    f4.At(0, 0) = 0;
    f4.At(1, 0) = T * alfa1 * L[3][1] / 2;
    f4.At(2, 0) = T * alfa1 * L[3][1] / 2;

    auto System = write_system_equations(g1, g2, g3, g4, f1, f2, f3, f4);
    
    Matrix G = std::get<0>(System);
    Matrix F = std::get<1>(System);

    Matrix t = solve_system_equations(G, F);

    cout << "Global matrix heat:" << endl;
    cout << G;

    cout << endl << "Global vector of heat load:" << endl;
    cout << F;

    cout << endl << "Distribution of temperature:" << endl;
    cout << t;


    // Решение: U = G^-1 * F
    return 0;
}
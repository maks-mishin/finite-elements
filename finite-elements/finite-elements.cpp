// finite-elements.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.`

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include "Matrix.h"

using namespace std;

using Task = map<int, vector<int>>;

Task local_numeration_nodes() {
    Task task;
    int num_elements;

    cout << "Enter number of elements: ";
    cin >> num_elements;

    for (size_t i = 0; i < num_elements; i++)
    {
        task.insert({});
        cout << "Number of elemnt " << i + 1 << ": ";
        for (size_t j = 0; j < 3; j++)
        {
            double node;
            cin >> node;
            task[i].push_back(node);
        }
    }

    return task;
}
void coeffs_shape_function();
void local_matrix_heat();
void local_vector_heat_load();
void write_system_equations();
void solve_system_equations();


int main() {
    double lambdaX = 20, lambdaY = 20, alfa1 = 250, q1 = 10000, T = 473, Q_v = 100000;
    
    Matrix LTCM(3, 3);
    Matrix LTLV(1, 3);

    // храним индексы узлов-1
    Task task = local_numeration_nodes();/*{
        {1, {0, 1, 3}},
        {2, {1, 3, 4}},
        {3, {1, 2, 4}},
        {4, {3, 4, 5}}
    };*/

    vector<double> X = {
        0, 0.01, 0.02, 0.01, 0.02, 0.02
    };
    vector<double> Y = {
        0, 0, 0, 0.01, 0.01, 0.02
    };

    // lengths of edges of elements
    vector<double> L1 = {
        sqrt(pow(X[0] - X[1], 2) + pow(Y[0] - Y[1], 2)),
        sqrt(pow(X[1] - X[3], 2) + pow(Y[1] - Y[3], 2)),
        sqrt(pow(X[3] - X[0], 2) + pow(Y[3] - Y[0], 2))
    };
    vector<double> L2 = {
        sqrt(pow(X[1] - X[4], 2) + pow(Y[1] - Y[4], 2)),
        sqrt(pow(X[4] - X[3], 2) + pow(Y[4] - Y[3], 2)),
        sqrt(pow(X[3] - X[1], 2) + pow(Y[3] - Y[1], 2))
    };
    vector<double> L3 = {
        sqrt(pow(X[1] - X[2], 2) + pow(Y[1] - Y[2], 2)),
        sqrt(pow(X[2] - X[4], 2) + pow(Y[2] - Y[4], 2)),
        sqrt(pow(X[4] - X[1], 2) + pow(Y[4] - Y[1], 2))
    };
    vector<double> L4 = {
        sqrt(pow(X[3] - X[4], 2) + pow(Y[3] - Y[4], 2)),
        sqrt(pow(X[4] - X[5], 2) + pow(Y[4] - Y[5], 2)),
        sqrt(pow(X[5] - X[3], 2) + pow(Y[5] - Y[3], 2))
    };

    for (size_t i = 0; i < 3; i++)
    {
        cout << "L1[" << i + 1 << "] = " << L1[i] << endl;
    }
    cout << endl;
    for (size_t i = 0; i < 3; i++)
    {
        cout << "L2[" << i + 1 << "] = " << L2[i] << endl;
    }
    cout << endl;
    for (size_t i = 0; i < 3; i++)
    {
        cout << "L3[" << i + 1 << "] = " << L3[i] << endl;
    }
    cout << endl;
    for (size_t i = 0; i < 3; i++)
    {
        cout << "L4[" << i + 1 << "] = " << L4[i] << endl;
    }

    vector<double> S = {
        (X[1] * Y[3] - X[4] * Y[1] + X[0] * Y[1] - X[0] * Y[3] + X[3] * Y[0] - X[1] * Y[0]) /2,
        (X[4] * Y[3] - X[3] * Y[4] + X[1] * Y[4] - X[1] * Y[3] + X[3] * Y[1] - X[4] * Y[1]) /2,
        (X[2] * Y[4] - X[4] * Y[2] + X[1] * Y[2] - X[1] * Y[4] + X[4] * Y[1] - X[4] * Y[1]) /2,
        (X[4] * Y[5] - X[5] * Y[4] + X[3] * Y[4] - X[3] * Y[5] + X[5] * Y[3] - X[4] * Y[3]) /2
    };

    for (size_t i = 0; i < 4; i++)
    {
        cout << "S[i] = " << S[i] << endl;
    }
    cout << endl;




    return 0;
}
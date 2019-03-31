#include <iostream>
#include <cassert>
#include <string>
#include <sstream>

#include "humanity.h"
#include "pain.h"


void healthyMatrixAsserts()
{
    HealthyMatrix<int, 3> matrix;
    assert(matrix.size() == 0);

    matrix.set(9, 0, 1, 2);
    assert(matrix.size() == 1);
    assert(matrix.get(0, 1, 2) == 9);

    matrix.set(8, 2, 3, 4);
    matrix.set(7, 3, 4, 5);
    matrix.set(0, 3, 4, 5);
    assert(matrix.size() == 2);
}

void healhtyMatrixDemo() 
{
    HealthyMatrix<int, 2> matrix;

    for (int i = 0, j = 9; i < 10, j >= 0; ++i, --j) {
        matrix.set(i, i, i);
        matrix.set(j, i, j);
    }

    std::cout << "===============" << std::endl;
    std::cout << "healthy matrix" << std::endl;
    std::cout << "===============" << std::endl;
    for (int i = 1; i < 9; ++i) {
        for (int j = 1; j < 9; ++j) {
            std::cout << matrix.get(i, j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "===============" << std::endl;

    std::cout << "size = " << matrix.size() << std::endl;
    std::cout << "===============" << std::endl;

    for (auto tuple: matrix) {
        int x, y, v;
        std::tie(x, y, v) = tuple;
        std::cout << x << y << v << std::endl;
    }
}

void matrixAsserts()
{
    SmokerMatrix<int, 2> matrix;
    assert(matrix.size() == 0);

    matrix[1][2] = 28;
    assert(matrix[1][2] == 28);
    assert(matrix.size() == 1);

    matrix[1][2] = 0;
    assert(matrix[1][2] == 0);
    assert(matrix.size() == 0);
}


void oops()
{
    SmokerMatrix<int, 2> matrix;
    assert(matrix.size() == 0);

    auto modify = [&matrix](int& n, int val) { 
        n = val; 
        assert(matrix.size() == 1);
    };

    modify(matrix[1][2], 28);
    assert(matrix[1][2] == 28);
    assert(matrix.size() == 1);

    matrix[1][2] = 0;
    assert(matrix[1][2] == 0);
    assert(matrix.size() == 0);
}

inline std::string itos(int i) 
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}

void stringMatrixDemo() {
    
    SmokerMatrix<std::string, 2> matrix("0");

    for (int i = 0, j = 9; i < 10, j >= 0; ++i, --j) {
        matrix[i][i] = itos(i).c_str();
        matrix[i][j] = itos(j);
    }

    std::cout << "===============" << std::endl;
    std::cout << "string matrix" << std::endl;
    std::cout << "===============" << std::endl;
    for (int i = 1; i < 9; ++i) {
        for (int j = 1; j < 9; ++j) {
            std::cout << static_cast<const std::string&>(matrix[i][j]) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "===============" << std::endl;

    std::cout << "size = " << matrix.size() << std::endl;
    std::cout << "===============" << std::endl;

    for (auto tuple: matrix) {
        int x, y;
        std::string v;
        std::tie(x, y, v) = tuple;
        std::cout << x << y << v << std::endl;
    }
}

int main()
{
    healthyMatrixAsserts();
    healhtyMatrixDemo();

    matrixAsserts();
    oops();

    stringMatrixDemo();
}
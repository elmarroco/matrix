#include <iostream>
#include <cstdlib>
#include "matrix.h"

using namespace std;

int main(int argc, char const *argv[])
{
    // X(row, column, fill)
    /*matrix<int> M(3, 2, 1), N(2, 4, 2), C(3, 4, 0);
    // X(row, column)
    matrix<int> Z(3, 4);
    for (size_t i = 0; i < M.row_size(); i++)
        for (size_t j = 0; j < N.col_size(); j++)
            for (size_t k = 0; k < M.col_size(); k++)
                C(i, j) += M(i, k) * N(k, j);
    matrix<int> D(C);
    Z = D;
    std::cout << C << D << Z;*/

    matrix<int> A(2, 3), B(3, 2), *C;

    A(0,0) = 5;
    A(0,1) = 4;
    A(0,2) = 3;
    A(1,0) = 6;
    A(1,1) = 2;
    A(1,2) = 1;

    B(0,0) = 9;
    B(0,1) = 6;
    B(1,0) = 8;
    B(1,1) = 5;
    B(2,0) = 7;
    B(2,1) = 4;

    cout << "  A\n" << A << endl;
    cout << "  B\n" << B << endl;

    //C = new matrix<int>(A.SerialMultiplication(B));
    C = new matrix<int>(A.ConcurrentMultiplication(B));

    cout << "  C\n" << *C << endl;

    return 0;
}
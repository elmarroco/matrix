#if !defined(MATRIX_H)
#define MATRIX_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <thread>
//#include <random>

//Forward declaration
template <typename T>
class matrix;
template <typename T>
std::ostream &operator<<(std::ostream &, const matrix<T> &);

//Class template definition
template <typename T>
class matrix
{
private:
    size_t row_num, col_num;
    std::vector<T> data;

public:
    /* @brief Default constructor */
    matrix();
    /* @brief initializer constructor receiving the matrix dimension */
    matrix(size_t, size_t, const T &value = T());
    /* @brief Copy constructor */
    matrix(const matrix<T> &);
    /* @brief Move constructor */
    matrix(matrix<T> &&);
    /* @brief Copy assignment operator*/
    matrix<T> &operator=(const matrix<T> &);
    /* @brief Move assignment operator*/
    matrix<T> &operator=(matrix<T> &&);

    /* @brief Serial matrix, cache sensitive */
    matrix<T> SerialMultiplication(const matrix<T> B);

    /* @brief Serial matrix, cache sensitive */
    matrix<T> SerialMultiplicationCache(const matrix<T> B);

    /* @brief Concurrent matrix multiplication */

    void One_Result(const matrix<T> B, int &C, int fila);
    matrix<T> ConcurrentMultiplication(const matrix<T> B);

    /* @brief Concurrent matrix multiplication, cache sensitive */

    void ConcurrentMultiplicationCache() {}

    //                                                <----------------------------------------   Here

    matrix<T> operator*(const matrix<T> &);
    /* @brief returns a reference to the value of index (i,j). No bound checking is performed */
    T &operator()(size_t, size_t);
    /* @brief returns the matrix dimension*/
    size_t size() { return data.size(); }
    /* @brief returns the number of rows*/
    size_t row_size() { return row_num; }
    /* @brief returns the number of columns*/
    size_t col_size() { return col_num; }
    /** @brief prints an object of class matrix*/
    //information about friend declaration: http://bit.ly/2VFZuIa
    friend std::ostream &operator<<<>(std::ostream &, const matrix<T> &);
};

//Member-function definitions

//Information about C++ constructors: http://bit.ly/2Qclolt

template <typename T>
matrix<T>::matrix() {}

template <typename T>
matrix<T>::matrix(size_t m, size_t n, const T &value)
    : row_num(m), col_num(n), data(m * n, value)
{
}

template <typename T>
matrix<T>::matrix(const matrix<T> &other)
    : row_num(other.row_num), col_num(other.col_num), data(row_num * col_num)
{
    std::copy(other.data.begin(), other.data.end(), data.begin());
}

template <typename T>
matrix<T>::matrix(matrix<T> &&other)
    : row_num(other.row_num), col_num(other.col_num)
{
    data = std::move(other.data);
    //other.dimension = 0;
    other.row_num = 0;
    other.col_num = 0;
}

template <typename T>
matrix<T> &matrix<T>::operator=(const matrix<T> &other)
{
    if (this != &other)
    {
        row_num = other.row_num;
        col_num = other.col_num;
        std::copy(other.data.begin(), other.data.end(), data.begin());
    }
    return *this;
}

template <typename T>
matrix<T> &matrix<T>::operator=(matrix<T> &&other)
{
    if (this != &other)
    {
        data.clear();
        row_num = other.row_num;
        col_num = other.col_num;
        data = std::move(other.data);
    }
    return *this;
}

template <typename T>
T &matrix<T>::operator()(size_t i, size_t j)
{
    return data[col_num * i + j];
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const matrix<T> &M)
{
    size_t col_counter = 0;
    for (auto &element : M.data)
    {
        os << element << ' ';
        if ((col_counter % M.col_num) == (M.col_num - 1))
            os << '\n';
        col_counter++;
    }
    return os;
}

template <typename T>
matrix<T> matrix<T>::SerialMultiplication(const matrix<T> B)
{
    matrix<T> C(row_num, B.col_num, 0);
    size_t i, j, k;

    for (i = 0; i < row_num; i++)
        for (j = 0; j < B.col_num; j++)
            for (k = 0; k < col_num; k++)
                C(i, j) += data[k + i * col_num] * B.data[j + k * B.col_num];

    return C;
}

template <typename T>
matrix<T> matrix<T>::SerialMultiplicationCache(const matrix<T> B)
{
    matrix<T> C(row_num, B.col_num, 0);
};

template <typename T>
matrix<T> matrix<T>::ConcurrentMultiplication(const matrix<T> B)
{
    matrix<T> C(row_num, B.col_num, 0);
    size_t i, j;
    std::vector<std::thread> task(row_num * B.col_num);

    for (i = 0; i < row_num; i++)
        for (j = 0; j < B.col_num; j++)
            task[j] = thread(One_Result, B, std::ref(C(i, j)), i);

    for (auto &t : task)
        t.join();

    return C;
}

template <typename T>
void matrix<T>::One_Result(const matrix<T> B, int &C, int fila)
{
    size_t i;
    for (i = 0; i < col_num; i++)
        C += data[i + fila * col_num] * B.data[fila * B.row_num]; //Posible pedo <-------------
}

#endif // MATRIX_H

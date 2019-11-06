#ifndef NUMERICMATRIX_H
#define NUMERICMATRIX_H

#pragma once
#include <QDebug>
#include <iostream>
#include <sstream>
#include <math.h>

class NumericMatrix
{
private:
    std::pair<int, int> size;
    double **data;
    int rows, cols;
    int self_id;

    static int id;

public:
    NumericMatrix(int _rows, int _cols)
    {
        ++id;
        self_id = id / 4;
        rows = _rows;
        cols = _cols;

        data = new double*[rows];
        for (int i = 0; i < rows; ++i)
            data[i] = new double[cols] {0};
    }

    ~NumericMatrix()
    {
        --id;
        // Crashes?
//        for (int i = 0; i < rows; ++i)
//            delete[] data[i];
//        delete[] data;
    }

    static int get_id(NumericMatrix& m)
    {
        return m.self_id;
    }

    static NumericMatrix* transpose_m(NumericMatrix& m)
    {
        int _rows = m.get_c(), _cols = m.get_r();
        NumericMatrix *transposed = new NumericMatrix(_rows, _cols);

        for (int i = 0; i < m.get_r(); ++i)
            for (int j = 0; j < m.get_c(); ++j)
                transposed->set_v(j, i, m.get_v(i, j));
        return transposed;
    }

    static NumericMatrix* inverse_m(NumericMatrix& m)
    {
        if (!(m.D_is_not_0()))
            throw "Cannot inverse matrix with D = 0!";

        double factor = (1 / NumericMatrix::determinant(m));
        int _rows = m.get_r(), _cols = m.get_c();

        NumericMatrix *acm = new NumericMatrix(_rows, _cols);
        for (int i = 0; i < _rows; ++i)
            for (int j = 0; j < _cols; ++j)
            {
                acm->set_v(i, j, m.algebraic_complement(i, j, m));
            }
        NumericMatrix *acm_T = NumericMatrix::transpose_m(*acm);
        return &acm_T->operator*(factor);
    }

    double algebraic_complement(int i, int j, NumericMatrix& m)
    {
        NumericMatrix *minor_m = NumericMatrix::reduce_m(i, j, m);
        double D = NumericMatrix::determinant(*minor_m);
        return pow(-1, ((i + 1) + (j + 1))) * D;
    }

    static NumericMatrix* reduce_m(int i, int j, NumericMatrix& m)
    {
        int _rows = m.get_r();
        NumericMatrix* reduced = new NumericMatrix(_rows, _rows);

        // Copy values
        for (int i = 0; i < _rows; ++i)
            for (int j = 0; j < _rows; ++j)
                reduced->set_v(i, j, m.get_v(i, j));

        NumericMatrix::remove_r(i, *reduced);
        NumericMatrix::remove_c(j, *reduced);
        return reduced;
    }

    static double determinant(NumericMatrix& m)
    {
        if ((m.get_r() < 1) || !m.quadratic())
            throw "Invalid matrix to calculate D of!";

        if (m.get_r() == 1)
            return m.get_v(0, 0);

        if (m.get_r() == 2)
            return (m.get_v(0, 0) * m.get_v(1, 1)) - (m.get_v(1, 0) * m.get_v(0, 1));

        double D = 0.0;
        for (int j = 0; j < m.get_c(); ++j)
        {
            NumericMatrix *reduced = NumericMatrix::reduce_m(0, j, m);
            D += pow(-1, (2 + j))
               * m.get_v(0, j)
               * NumericMatrix::determinant(*reduced);
        }
        return D;
    }

    bool D_is_not_0()
    {
        double D = determinant(*this);
        return (D > 0) || (D < 0);
    }

    // Setters & Getters
    void set_size(int _rows, int _cols)
    {
        set_r(_rows);
        set_c(_cols);
        size = std::make_pair(_rows, _cols);
    }

    std::pair<int, int> get_size()
    {
        std::pair<int, int> pair;
        pair = std::make_pair(rows, cols);
        return pair;
    }

    void set_r(int _rows)
    {
        _rows < rows ? dec_r(_rows) : inc_r(_rows);
    }

    void dec_r(int _rows)
    {
        int diff = rows - _rows;
        for (int r = rows; r != (rows - diff); --r)
            delete[] data[r - 1];
        rows = _rows;
    }

    void inc_r(int _rows)
    {
        int diff = _rows - rows;
        for (int r = rows - 1; r <= diff; ++r)
        {
            data[r + 1] = new double[cols];
            for (int j = 0; j < cols; ++j)
                data[r + 1][j] = {0};
        }
        rows = _rows;
    }

    static void remove_r(int ri, NumericMatrix& m)
    {
        if(ri >= m.get_r())
            throw "Row index out of bounds!";
        --m.rows;
        for (int r = ri; r < m.get_r(); ++r)
            m.data[r] = m.data[r + 1];
    }

    int get_r()
    {
        return rows;
    }

    void set_c(int _cols)
    {
        _cols < cols ? dec_c(_cols) : inc_c(_cols);
    }

    void dec_c(int _cols)
    {
        double *resized = new double[_cols];

        for (int i = 0; i < rows; ++i)
        {
            for (int c = 0; c < _cols; ++c)
                resized[c] = data[i][c];

            data[i] = new double[_cols];

            for (int v = 0; v < _cols; ++v)
                data[i][v] = resized[v];
        }
        cols = _cols;
    }

    void inc_c(int _cols)
    {
        double *resized = new double[_cols] {0};

        for (int i = 0; i < rows; ++i)
        {
            for (int c = 0; c < cols; ++c)
                resized[c] = data[i][c];

            data[i] = new double[_cols];

            for (int c = 0; c < _cols; ++c)
                data[i][c] = resized[c];
        }

        cols = _cols;
    }

    static void remove_c(int ci, NumericMatrix& m)
    {
        if (ci >= m.get_c())
            throw "Column index of out bounds!";

        double **_data = new double*[m.get_r()];
        double *_row;
        int ni;

        for (int i = 0; i < m.get_r(); ++i)
        {
            _row = new double[m.get_c() - 1];
            ni = 0;

            for(int j = 0; j < m.get_c(); ++j)
                if (j != ci)
                    _row[ni++] = m.get_v(i, j);

            _data[i] = _row;
        }

        m.data = _data;
        m.cols--;
    }

    int get_c()
    {
        return cols;
    }

    void set_v(int row, int col, double _value)
    {
        if ((rows < row) || (cols < col))
            throw "Size out of bound!";
        data[row][col] = _value;
    }

    double& get_v(int row, int col)
    {
        if ((rows < row) || (cols < col))
            throw "Size out of bound!";
        return data[row][col];
    }

    void show()
    {
        std::cout << "<NumericMatrix{" << get_r() << ", " << get_c() << "}:\n";
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
                std::cout << data[i][j] << "\t";
            std::cout << "\n";
        }
    }

    double** get_data()
    {
        return data;
    }

    std::string get_str_data()
    {
        std::ostringstream oss;
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
                oss << data[i][j] << "  ";
            oss << "\n";
        }

        return oss.str();
    }

    bool zeros()
    {
        int zeros_counter = 0;
        for (int i = 0; i < this->rows; ++i)
            for (int j = 0; j < this->cols; ++j)
                if (!(this->data[i][j] > 0.0) && !(this->data[i][j] < 0.0))
                    zeros_counter++;
        if (zeros_counter == (this->rows * this->cols))
            return true;

        return false;
    }

    bool ones()
    {
        int ones_counter = 0;
        for (int i = 0; i < this->rows; ++i)
            for (int j = 0; j < this->cols; ++j)
                if (!(this->data[i][j] > 1.0) && !(this->data[i][j] < 1.0))
                    ones_counter++;
        if (ones_counter == (this->rows * this->cols))
            return true;

        return false;
    }

    bool quadratic()
    {
        return rows == cols;
    }

    /* Operators */

    // Size comparison operators
    friend bool operator+=(NumericMatrix& l, NumericMatrix &r)
    {
        // l & r are acceptable for summarizing
        return ((l.get_r() == r.get_r()) && (l.get_c() == r.get_c()));
    }

    friend bool operator-=(NumericMatrix& l, NumericMatrix &r)
    {
        // l & r are acceptable for subscription
        return ((l.get_r() == r.get_r()) && (l.get_c() == r.get_c()));
    }

    friend bool operator*=(NumericMatrix& l, NumericMatrix &r)
    {
        // l & r are acceptable for multiplication
        return l.get_c() == r.get_r();
    }

    friend bool operator/=(NumericMatrix& l, NumericMatrix &r)
    {
        // l & r are acceptable for division (A / B == A * B^(-1))
        return r.quadratic() && r.D_is_not_0() && (l.get_c() == NumericMatrix::inverse_m(r)->get_r());
    }

    // Arithmetic operations with a constant
    NumericMatrix& operator+(double c_term)
    {
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                data[i][j] += c_term;
        return *this;
    }

    NumericMatrix& operator+(NumericMatrix& other)
    {
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                data[i][j] += other.get_v(i, j);
        return *this;
    }

    NumericMatrix& operator-(double c_subtrahend)
    {
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                data[i][j] -= c_subtrahend;
        return *this;
    }

    NumericMatrix& operator-(NumericMatrix& other)
    {
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                data[i][j] -= other.get_v(i, j);
        return *this;
    }

    NumericMatrix& operator*(double c_factor)
    {
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                data[i][j] *= c_factor;
        return *this;
    }

    NumericMatrix& operator*(NumericMatrix& other)
    {
        if (!(*this *= other))
            throw "Operator '*' requires matrices to be same size";

        int _rows_1 = this->get_r(),
            _cols_1 = this->get_c(),
            _cols_2 = other.get_c();

        NumericMatrix *result = new NumericMatrix(_rows_1, _cols_2);
        double value;

        for (int i = 0; i < _rows_1; ++i)
        {
            for (int j = 0; j < _cols_2; ++j)
            {
                value = 0;
                for (int k = 0; k < _cols_1; ++k)
                {
                    value += (this->get_v(i, k) * other.get_v(k, j));
                }
                result->set_v(i, j, value);
            }
        }

        return *result;
    }

    NumericMatrix& operator/(double c_delimeter)
    {
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                data[i][j] = int(data[i][j] / c_delimeter);
        return *this;
    }

    NumericMatrix& operator/(NumericMatrix& other)
    {
        if (!(*this /= other))
            throw "Operator '/' requires matrices to be same size";
        return (*this) * *NumericMatrix::inverse_m(other);
    }

    NumericMatrix& operator%(int c_modulus)
    {
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
            {
                double leftover = int(data[i][j]) - data[i][j];
                data[i][j] = (int(data[i][j]) % c_modulus) + leftover;
            }
        return *this;
    }

    double& operator()(int row, int col)
    {
        if ((rows < row) || (cols < col))
            throw "Size out of bound!";
        return data[row][col];
    }

    // Data comparison operators
    friend bool operator>(const NumericMatrix& l, const NumericMatrix& r)
    {
        if ((r.rows != l.rows) || (r.cols != l.cols))
            throw "Operator '>' requires matrices to be same size";

        for (int i = 0; i < l.rows; ++i)
            for (int j = 0; j < l.cols; ++j)
                if (l.data[i][j] <= r.data[i][j]) return false;
        return true;
    }

    friend bool operator>=(const NumericMatrix& l, const NumericMatrix& r)
    {
        if ((r.rows != l.rows) || (r.cols != l.cols))
            throw "Operator '>=' requires matrices to be same size";

        for (int i = 0; i < l.rows; ++i)
            for (int j = 0; j < l.cols; ++j)
                if (l.data[i][j] < r.data[i][j]) return false;
        return true;
    }

    friend bool operator<(const NumericMatrix& l, const NumericMatrix& r)
    {
        if ((r.rows != l.rows) || (r.cols != l.cols))
            throw "Operator '<' requires matrices to be same size";

        for (int i = 0; i < l.rows; ++i)
            for (int j = 0; j < l.cols; ++j)
                if (l.data[i][j] >= r.data[i][j]) return false;
        return true;
    }

    friend bool operator<=(const NumericMatrix& l, const NumericMatrix& r)
    {
        if ((r.rows != l.rows) || (r.cols != l.cols))
            throw "Operator '<=' requires matrices to be same size";

        for (int i = 0; i < l.rows; ++i)
            for (int j = 0; j < l.cols; ++j)
                if (l.data[i][j] > r.data[i][j]) return false;
        return true;
    }

    friend bool operator==(const NumericMatrix& l, const NumericMatrix& r)
    {
        if ((r.rows != l.rows) || (r.cols != l.cols))
            throw "Operator '==' requires matrices to be same size";

        for (int i = 0; i < l.rows; ++i)
            for (int j = 0; j < l.cols; ++j)
                if ((l.data[i][j] > r.data[i][j]) || (l.data[i][j] < r.data[i][j]))
                    return false;
        return true;
    }
};
#endif // NUMERICMATRIX_H

#ifndef NUMERICMATRIX_H
#define NUMERICMATRIX_H
#include <iostream>
#include <QDebug>

class NumericMatrix
{
public:
    NumericMatrix(int _rows, int _cols)
    {
        rows = _rows;
        cols = _cols;

        data = new double*[rows];

        for (int i = 0; i < rows; ++i)
        {
            data[i] = new double[cols];
            for (int j = 0; j < cols; ++j)
            {
                data[i][j] = {0};
            }
        }

    }

    ~NumericMatrix()
    {
        for (int i = 0; i < rows; ++i)
            delete[] data[i];
        delete[] data;
    }

    // Setters & Getters
    void set_size(int _rows, int _cols)
    {
        set_r_count(_rows);
        set_c_count(_cols);
    }

    std::pair<int, int> get_size()
    {
        std::pair<int, int> pair;
        pair = std::make_pair(rows, cols);
        return pair;
    }

    void set_r_count(int _rows)
    {
        _rows < rows ? dec_r(_rows) : inc_r(_rows);
    }

    void dec_r(int _rows)
    {
        int diff = rows - _rows;
        for (int r = rows; r != (rows - diff); --r)
        {
            delete[] data[r - 1];
        }
        rows = _rows;
    }

    void inc_r(int _rows)
    {
        int diff = _rows - rows;
        for (int r = rows - 1; r <= diff; ++r)
        {
            data[r + 1] = new double[cols];
            for (int j = 0; j < cols; ++j)
            {
                data[r + 1][j] = {0};
            }
        }
        rows = _rows;
    }

    double get_r()
    {
        return rows;
    }

    void set_c_count(int _cols)
    {
        cols < _cols ? dec_c(_cols) : inc_c(_cols);
    }

    void dec_c(int _cols)
    {
        cols = _cols;
    }

    void inc_c(int _cols)
    {
        cols = _cols;
    }

    double get_c()
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
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                std::cout << data[i][j] << "\t";
            }
            std::cout << "\n";
        }
    }

    // Operators
    double& operator()(int row, int col)
    {
        if ((rows < row) || (cols < col))
            throw "Size out of bound!";
        return data[row][col];
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

    friend bool operator>(const NumericMatrix& l, const NumericMatrix& r)
    {
        if (!(r.rows != l.rows) && (r.cols == l.cols))
            throw "Operator '>' requires matrices to be same size";

        for (int i = 0; i < l.rows; ++i)
            for (int j = 0; j < l.cols; ++j)
                if (l.data[i][j] <= r.data[i][j]) return false;
        return true;
    }

    friend bool operator>=(const NumericMatrix& l, const NumericMatrix& r)
    {
        if (!(r.rows != l.rows) && (r.cols == l.cols))
            throw "Operator '>=' requires matrices to be same size";

        for (int i = 0; i < l.rows; ++i)
            for (int j = 0; j < l.cols; ++j)
                if (l.data[i][j] < r.data[i][j]) return false;
        return true;
    }

    friend bool operator<(const NumericMatrix& l, const NumericMatrix& r)
    {
        if (!(r.rows != l.rows) && (r.cols == l.cols))
            throw "Operator '<' requires matrices to be same size";

        for (int i = 0; i < l.rows; ++i)
            for (int j = 0; j < l.cols; ++j)
                if (l.data[i][j] >= r.data[i][j]) return false;
        return true;
    }

    friend bool operator<=(const NumericMatrix& l, const NumericMatrix& r)
    {
        if (!(r.rows != l.rows) && (r.cols == l.cols))
            throw "Operator '<=' requires matrices to be same size";

        for (int i = 0; i < l.rows; ++i)
            for (int j = 0; j < l.cols; ++j)
                if (l.data[i][j] > r.data[i][j]) return false;
        return true;
    }

    friend bool operator==(const NumericMatrix& l, const NumericMatrix& r)
    {
        if (!(r.rows != l.rows) && (r.cols == l.cols))
            throw "Operator '==' requires matrices to be same size";

        for (int i = 0; i < l.rows; ++i)
            for (int j = 0; j < l.cols; ++j)
                if ((l.data[i][j] > r.data[i][j]) || (l.data[i][j] < r.data[i][j]))
                    return false;
        return true;
    }

private:
    double **data;
    int rows, cols;
};

#endif // NUMERICMATRIX_H

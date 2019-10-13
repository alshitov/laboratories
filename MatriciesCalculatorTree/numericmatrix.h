#ifndef NUMERICMATRIX_H
#define NUMERICMATRIX_H

class NumericMatrix
{
public:
    NumericMatrix(int _rows, int _cols)
    {
        rows = _rows;
        cols = _cols;

        this->data = new double*[rows];

        for (int i = 0; i < int(this->rows); ++i)
            this->data[i] = new double[this->cols];
    }

    ~NumericMatrix()
    {
        for (int i = 0; i < this->rows; ++i)
            delete this->data[i];
        delete this->data;
    }

    // Setters & Getters
    void setRowCount(int _rows)
    {
        rows = _rows;
    }

    void setColCount(int _cols)
    {
        cols = _cols;
    }

    void setValue(int row, int col, double _value)
    {
        this->data[row][col] = _value;
    }


    double& getValue(int row, int col)
    {
        return this->data[row][col];
    }

    // Operators
    double& operator()(int i, int j)
    {
        return this->data[i][j];
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
                if ((l.data[i][j] > r.data[i][j]) || (l.data[i][j] < r.data[i][j])) return false;
        return true;
    }

private:
    double **data;
    int rows, cols;
};

#endif // NUMERICMATRIX_H

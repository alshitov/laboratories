#ifndef MATRICESCALCULATOR_H
#define MATRICESCALCULATOR_H

#include <numericmatrix.h>
#include <vector>

class MatricesCalculator
{

public:
    MatricesCalculator();
    ~MatricesCalculator();

    // Unary
    NumericMatrix* transpose_M(NumericMatrix *m);
    NumericMatrix* inverse_M(NumericMatrix *m);

    // Between themselves (2 or more matrices)
    NumericMatrix* sum_Ms(std::vector<NumericMatrix*> ms);
    NumericMatrix* sub_Ms(std::vector<NumericMatrix*> ms);
    NumericMatrix* mul_Ms(std::vector<NumericMatrix*> ms);
    NumericMatrix* div_Ms(std::vector<NumericMatrix*> ms);
};

#endif // MATRICESCALCULATOR_H

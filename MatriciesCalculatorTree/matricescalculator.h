#ifndef MATRICESCALCULATOR_H
#define MATRICESCALCULATOR_H

#include <numericmatrix.h>
#include <vector>
#include <map>

class MatricesCalculator
{

public:
    MatricesCalculator();
    ~MatricesCalculator();

    std::map<std::string, std::vector<std::string>> permutations();
    std::vector<NumericMatrix*>* performCalculations(std::vector<NumericMatrix*>& ms);

    NumericMatrix* transpose_M(NumericMatrix *m);
    NumericMatrix* inverse_M(NumericMatrix *m);
    NumericMatrix* sum_Ms(std::vector<NumericMatrix*>& ms);
    NumericMatrix* sub_Ms(std::vector<NumericMatrix*>& ms);
    NumericMatrix* mul_Ms(std::vector<NumericMatrix*>& ms);
    NumericMatrix* div_Ms(std::vector<NumericMatrix*>& ms);
};

#endif // MATRICESCALCULATOR_H

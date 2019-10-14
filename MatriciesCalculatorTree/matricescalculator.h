#ifndef MATRICESCALCULATOR_H
#define MATRICESCALCULATOR_H

#include <numericmatrix.h>
#include <vector>
#include <map>

// Dictionary: { NumericMatrix: [NumericMatrix] };
struct m_dict
{
    const NumericMatrix&          m;
    std::vector<NumericMatrix*>&  ms;
};

// List: [NumericMatrix];
struct m_list
{
    std::vector<NumericMatrix*>& ms;
};

struct conditions
{
    bool is_sum_acceptable(NumericMatrix& l, NumericMatrix& r)
    {
        return l += r;
    }

    bool is_sub_acceptable(NumericMatrix& l, NumericMatrix& r)
    {
        return l -= r;
    }

    bool is_mul_acceptable(NumericMatrix& l, NumericMatrix& r)
    {
        return l *= r;
    }

    bool is_div_acceptable(NumericMatrix& l, NumericMatrix& r)
    {
        return l /= r;
    }
};

class MatricesCalculator
{

public:
    MatricesCalculator();
    ~MatricesCalculator();

    // Reduce (m_dict[m] -> ?>condition(m, m_list[index]) -> m_dict)
    m_dict* permutations(m_dict& mdict);
    // Map (m_dict[m] -> arithmetic_action(m, m_list[index]) -> m_list)
    m_list* perform_calculations(m_dict& mdict);

    NumericMatrix* transpose_M(NumericMatrix *m);
    NumericMatrix* inverse_M(NumericMatrix *m);

    m_list* sum_Ms(m_dict& mset);
    m_list* sub_Ms(m_dict& mset);
    m_list* mul_Ms(m_dict& mset);
    m_list* div_Ms(m_dict& mset);
};

#endif // MATRICESCALCULATOR_H

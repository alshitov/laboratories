#ifndef MATRICESCALCULATOR_H
#define MATRICESCALCULATOR_H

#include <QtCore>
#include <QtWidgets>

#include <numericmatrix.h>
#include <vector>

// List: [NumericMatrix];
struct mlist
{
    std::vector<NumericMatrix> ms;
};

struct minfo
{
    minfo()
    {
        m  = new NumericMatrix(0, 0);
        mT = new NumericMatrix(0, 0);
        mR = new NumericMatrix(0, 0);
    }

    ~minfo()
    {
        delete m;
        delete mT;
        delete mR;
    }

    void set_m_name(const std::string& _name)
    {
        name = _name;
    }

    std::string get_m_name()
    {
        return name;
    }

    void set_m(NumericMatrix& _m)
    {
        m = &_m;
    }

    NumericMatrix* get_m()
    {
        return m;
    }

    NumericMatrix* m;       // Matrix
    std::string name;       // Matrix Name
    NumericMatrix* mT;      // Matrix Transposed
    NumericMatrix* mR;      // Matrix Reversed

    mlist sum_ms;       // Matrices acceptable for summarizing
    mlist sub_ms;       // Matrices acceptable for subscription
    mlist mul_ms;       // Matrices acceptable for multiplication
    mlist div_ms;       // Matrices acceptable for division

    // Results
    mlist _done_sum_ms;
    mlist _done_sub_ms;
    mlist _done_mul_ms;
    mlist _done_div_ms;
};

struct conditions
{
    static bool is_inversion_acceptable(NumericMatrix& m)
    {
        return m.D_is_not_0();
    }

    static bool is_sum_acceptable(NumericMatrix& l, NumericMatrix& r)
    {
        return l += r;
    }

    static bool is_sub_acceptable(NumericMatrix& l, NumericMatrix& r)
    {
        return l -= r;
    }

    static bool is_mul_acceptable(NumericMatrix& l, NumericMatrix& r)
    {
        return l *= r;
    }

    static bool is_div_acceptable(NumericMatrix& l, NumericMatrix& r)
    {
        return l /= r;
    }
};

class MatricesCalculator : public QObject
{
    Q_OBJECT

private:
    std::vector<minfo*> msinfo;
    conditions _conditions;

public:
    MatricesCalculator();
    ~MatricesCalculator();

    void add_m(NumericMatrix& m);
    void edit_m(NumericMatrix& m, NumericMatrix& nm);
    void remove_m(NumericMatrix& m);

    mlist search_sum_acceptable(NumericMatrix& m, mlist& others);
    mlist search_sub_acceptable(NumericMatrix& m, mlist& others);
    mlist search_mul_acceptable(NumericMatrix& m, mlist& others);
    mlist search_div_acceptable(NumericMatrix& m, mlist& others);
    void  search_all_acceptable(NumericMatrix& m, minfo& _minfo);

    minfo* find_by_m(NumericMatrix& m);
    mlist extract_ms();

    void run_calc();
    void perform_calculations(minfo& _minfo);

    void do_transpose_m(minfo& _minfo);
    void do_inverse_m(minfo& _minfo);
    bool mlist_changed(mlist& _ms, mlist& _new_ms);
    void do_sum_ms(minfo& _minfo);
    void do_sub_ms(minfo& _minfo);
    void do_mul_ms(minfo& _minfo);
    void do_div_ms(minfo& _minfo);
};

#endif // MATRICESCALCULATOR_H

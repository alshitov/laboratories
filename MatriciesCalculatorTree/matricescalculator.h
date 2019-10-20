#ifndef MATRICESCALCULATOR_H
#define MATRICESCALCULATOR_H

#include <QtCore>
#include <QtWidgets>

#include <numericmatrix.h>
#include <vector>

// List: [NumericMatrix];
class MList
{
private:
    std::vector<NumericMatrix*>* ms;

public:
    MList();
    ~MList()
    {
        delete ms;
    }

    void set_ms(std::vector<NumericMatrix*>* _ms) { ms = _ms; }
    std::vector<NumericMatrix*>* get() { return ms; }
};

// Dictionary: { NumericMatrix: [NumericMatrix] };
class MDict
{
private:
    NumericMatrix* m;
    MList* ms;

public:
    MDict();
    ~MDict()
    {
        delete m;
        delete ms;
    }

    void set_ms(MList* _ms) { ms = _ms; }
    void set_m(NumericMatrix* _m) { m = _m; }
    NumericMatrix* get_m() { return m; }
    MList* get_ms() { return ms; }

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

class MatricesCalculator : public QObject
{
    Q_OBJECT

private:
    conditions _conditions;
    MList _m_list;
    NumericMatrix *last_received_m;
    MDict _m_dict;

public:
    MatricesCalculator();
    ~MatricesCalculator();

    void add_m(NumericMatrix& m);

    // Reduce (m_dict[m] -> ?>condition(m, m_list[index]) -> m_dict)
    MDict* permutations(MDict& mdict);

    // Map (m_dict[m] -> arithmetic_action(m, m_list[index]) -> m_list)
    void perform_calculations(MDict& mdict);

    void sum_ms(MList& ml);
    void sub_ms(MList& ml);
    void mul_ms(MList& ml);
    void div_ms(MList& ml);

public slots:
    void m_received(NumericMatrix& m);

signals:
    void calc_done(MList&);
};

#endif // MATRICESCALCULATOR_H

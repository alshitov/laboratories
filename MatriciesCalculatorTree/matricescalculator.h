#ifndef MATRICESCALCULATOR_H
#define MATRICESCALCULATOR_H

#include <QtCore>
#include <QtWidgets>
#include <QDebug>
#include <vector>
#include <map>

#pragma once
#include "numericmatrix.h"

struct mlist
{
    std::vector<NumericMatrix> ms;

    void __repr__()
    {
        std::cout << "\n*** MLIST ***\n";
        for (unsigned long i = 0; i < ms.size(); ++i)
        {
            ms.at(i).show();
        }
        std::cout << "\n** END MLIST **\n";
    }
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

    void set_m_name(const std::string& _name) { name = _name; }
    void set_m(NumericMatrix& _m) { m = &_m; }

    std::string get_m_name() { return name; }
    NumericMatrix* get_m()  { return m; }
    NumericMatrix* get_mT() { return mT; }
    NumericMatrix* get_mR() { return mR; }

    mlist* get_sum_ms() { return &sum_ms; }
    mlist* get_sub_ms() { return &sub_ms; }
    mlist* get_mul_ms() { return &mul_ms; }
    mlist* get_div_ms() { return &div_ms; }

    void __repr__()
    {
        std::cout << "\n----- MINFO -----\n"
                  << "Name: " << name
                  << "\nMatrix:\n"; m->show();

        std::cout << "Transposed:\n"; mT->show();
        std::cout << "Inversed:\n";   mR->show();

        std::cout << "\nSum Ms:"; sum_ms.__repr__();
        std::cout << "\nSub Ms:"; sub_ms.__repr__();
        std::cout << "\nMul Ms:"; mul_ms.__repr__();
        std::cout << "\nDiv Ms:"; div_ms.__repr__();

        std::cout << "\nDone sum Ms:"; _done_sum_ms.__repr__();
        std::cout << "\nDone sub Ms:"; _done_sub_ms.__repr__();
        std::cout << "\nDone mul Ms:"; _done_mul_ms.__repr__();
        std::cout << "\nDone div Ms:"; _done_div_ms.__repr__();

        std::cout << "----- END MINFO -----\n";
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
    static bool is_inversion_acceptable(NumericMatrix& m) { return m.D_is_not_0(); }
    static bool is_sum_acceptable(NumericMatrix& l, NumericMatrix& r) { return l += r; }
    static bool is_sub_acceptable(NumericMatrix& l, NumericMatrix& r) { return l -= r; }
    static bool is_mul_acceptable(NumericMatrix& l, NumericMatrix& r) { return l *= r; }
    static bool is_div_acceptable(NumericMatrix& l, NumericMatrix& r) { return l /= r; }
};

class MatricesCalculator : public QObject
{
    Q_OBJECT

private:
    std::vector<minfo*> msinfo;
    conditions _conditions;

public:
    MatricesCalculator();
    virtual ~MatricesCalculator() {  }

    void show();
    void add_m(NumericMatrix& m, QString name);
    std::vector<minfo*> get_msinfo();
    minfo* get_minfo(int matrix_id);
    void edit_m(NumericMatrix& m, NumericMatrix& nm);
    void remove_m(NumericMatrix& m);

    void process_all_ms();

    mlist* search_sum_acceptable(NumericMatrix* m, mlist* others);
    mlist* search_sub_acceptable(NumericMatrix* m, mlist* others);
    mlist* search_mul_acceptable(NumericMatrix* m, mlist* others);
    mlist* search_div_acceptable(NumericMatrix* m, mlist* others);

    minfo* find_by_id(int _id);
    mlist* extract_ms();

    void run_calc();
    void perform_calculations(minfo* _minfo);

    void do_transpose_m(minfo* _minfo);
    void do_inverse_m(minfo* _minfo);
    void do_sum_ms(minfo* _minfo);
    void do_sub_ms(minfo* _minfo);
    void do_mul_ms(minfo* _minfo);
    void do_div_ms(minfo* _minfo);
};

#endif // MATRICESCALCULATOR_H

#include "matricescalculator.h"
#include "numericmatrix.h"
#include <map>
#include <vector>

MatricesCalculator::MatricesCalculator()
{

}

MatricesCalculator::~MatricesCalculator()
{

}

void MatricesCalculator::add_m(NumericMatrix &m)
{
    minfo *new_minfo = new minfo();
    new_minfo->set_m(m);
    search_all_acceptable(m, *new_minfo);
    msinfo.push_back(new_minfo);
    run_calc();
}

void MatricesCalculator::edit_m(NumericMatrix &m, NumericMatrix &nm)
{
    minfo* target_minfo = find_by_m(m);
    target_minfo->set_m(nm);
    search_all_acceptable(nm, *target_minfo);
    run_calc();
}

void MatricesCalculator::remove_m(NumericMatrix& m)
{

}

mlist MatricesCalculator::search_sum_acceptable(NumericMatrix& m, mlist& others)
{
    mlist _sum_ms;
    for (unsigned long it = 0; it < others.ms.size(); ++it)
        if (_conditions.is_sum_acceptable(m, others.ms[it]))
            _sum_ms.ms.push_back(others.ms[it]);

    return _sum_ms;
}

mlist MatricesCalculator::search_sub_acceptable(NumericMatrix& m, mlist& others)
{
    mlist _sub_ms;
    for (unsigned long it = 0; it < others.ms.size(); ++it)
        if (_conditions.is_sub_acceptable(m, others.ms[it]))
            _sub_ms.ms.push_back(others.ms[it]);

    return _sub_ms;
}

mlist MatricesCalculator::search_mul_acceptable(NumericMatrix& m, mlist& others)
{
    mlist _mul_ms;
    for (unsigned long it = 0; it < others.ms.size(); ++it)
        if (_conditions.is_mul_acceptable(m, others.ms[it]))
            _mul_ms.ms.push_back(others.ms[it]);

    return _mul_ms;
}

mlist MatricesCalculator::search_div_acceptable(NumericMatrix& m, mlist& others)
{
    mlist _div_ms;
    for (unsigned long it = 0; it < others.ms.size(); ++it)
        if (_conditions.is_div_acceptable(m, others.ms[it]))
            _div_ms.ms.push_back(others.ms[it]);

    return _div_ms;
}

void  MatricesCalculator::search_all_acceptable(NumericMatrix& m, minfo& _minfo)
{
    mlist _other_ms = extract_ms();

    _minfo.sum_ms = search_sum_acceptable(m, _other_ms);
    _minfo.sub_ms = search_sub_acceptable(m, _other_ms);
    _minfo.mul_ms = search_mul_acceptable(m, _other_ms);
    _minfo.div_ms = search_div_acceptable(m, _other_ms);
}

minfo* MatricesCalculator::find_by_m(NumericMatrix& m)
{
    // Get minfo by center matrix
    unsigned long it;
    for (it = 0; it < msinfo.size(); ++it)
        if (m == *(msinfo.at(it)->m))
            break;

    return msinfo.at(it);
}

mlist MatricesCalculator::extract_ms()
{
    // Get matrices from all minfos
    mlist all_ms;
    for (unsigned long it = 0; it < msinfo.size(); ++it)
        all_ms.ms.push_back(*(msinfo.at(it)->m));

    return all_ms;
}

void MatricesCalculator::run_calc()
{
    for (unsigned long it = 0; it < msinfo.size(); ++it)
    {
        minfo *target_minfo = msinfo.at(it);
        do_transpose_m(*target_minfo);
        do_inverse_m(*target_minfo);
//        do_sum_ms(*target_minfo);
//        do_sub_ms(*target_minfo);
//        do_mul_ms(*target_minfo);
//        do_div_ms(*target_minfo);
    }
}

void MatricesCalculator::do_transpose_m(minfo &_minfo)
{
    _minfo.mT = NumericMatrix::transpose_m(*_minfo.m);
}

void MatricesCalculator::do_inverse_m(minfo &_minfo)
{
    if (!_conditions.is_inversion_acceptable(*_minfo.m)) return;
    _minfo.mR = NumericMatrix::inverse_m(*_minfo.m);
}

bool MatricesCalculator::mlist_changed(mlist& _ms, mlist& _new_ms)
{
    return _ms.ms.size() == _new_ms.ms.size();
}

void MatricesCalculator::do_sum_ms(minfo& _minfo)
{
    if (!mlist_changed(_minfo.sum_ms, _minfo._done_sum_ms)) return;

    for (unsigned long idx = 0; idx < _minfo.sum_ms.ms.size(); ++idx)
    {
        try {
            _minfo._done_sum_ms.ms.push_back(
                *(_minfo.m) + _minfo.sum_ms.ms.at(idx)
            );
        } catch(...) {
            std::cout << "Fuck u 1";
        }
    }
}

void MatricesCalculator::do_sub_ms(minfo& _minfo)
{
    if (!mlist_changed(_minfo.sub_ms, _minfo._done_sub_ms)) return;

    for (unsigned long idx = 0; idx < _minfo.sub_ms.ms.size(); ++idx)
    {
        try {
            _minfo._done_sub_ms.ms.push_back(
                *(_minfo.m) - _minfo.sub_ms.ms.at(idx)
            );
        } catch(...) {
            std::cout << "Fuck u 2";
        }
    }
}

void MatricesCalculator::do_mul_ms(minfo& _minfo)
{
    if (!mlist_changed(_minfo.mul_ms, _minfo._done_mul_ms)) return;

    for (unsigned long idx = 0; idx < _minfo.mul_ms.ms.size(); ++idx)
    {
        try {
            _minfo._done_mul_ms.ms.push_back(
                *(_minfo.m) * _minfo.mul_ms.ms.at(idx)
            );
        } catch(...) {
            std::cout << "Fuck u 3";
        }
    }
}

void MatricesCalculator::do_div_ms(minfo& _minfo)
{
    if (!mlist_changed(_minfo.div_ms, _minfo._done_div_ms)) return;

    for (unsigned long idx = 0; idx < _minfo.div_ms.ms.size(); ++idx)
    {
        try {
            _minfo._done_div_ms.ms.push_back(
                *(_minfo.m) / _minfo.div_ms.ms.at(idx)
            );
        } catch(...) {
            std::cout << "Fuck u 4";
        }
    }
}

#include "matricescalculator.h"

MatricesCalculator::MatricesCalculator()
{

}

void MatricesCalculator::show()
{
    std::cout << "\n---- CALCULATOR STATE ----\n"
              << "Size: " << msinfo.size();
    for (unsigned long i = 0; i < msinfo.size(); ++i)
    {
        msinfo.at(i)->__repr__();
    }
    std::cout << "\n--- END CALCULATOR STATE ---\n";
}

std::vector<minfo*> MatricesCalculator::get_msinfo()
{
    return msinfo;
}

minfo* MatricesCalculator::get_minfo(int matrix_id)
{
    bool found = false;
    unsigned long i;

    for (i = 0; i < msinfo.size(); ++i)
    {
        if (NumericMatrix::get_id(*msinfo.at(i)->get_m()) == matrix_id)
        {
            found = true;
            break;
        }
    }

    if (found) return msinfo.at(i);
    else exit(0);
}

void MatricesCalculator::add_m(NumericMatrix &m, QString name)
{
    minfo *new_minfo = new minfo();
    new_minfo->set_m(m);
    new_minfo->set_m_name(name.toUtf8().constData());
    msinfo.push_back(new_minfo);
    do_transpose_m(new_minfo);
    do_inverse_m(new_minfo);

    process_all_ms();
    run_calc();
    show();
}

void MatricesCalculator::remove_m(NumericMatrix &m)
{
    int id = NumericMatrix::get_id(m);
    for (unsigned long it = 0; it < msinfo.size(); ++it)
    {
        if (NumericMatrix::get_id(*msinfo.at(it)->get_m()) == id)
        {
            msinfo.erase(msinfo.begin() + int(it));
            break;
        }
    }

    process_all_ms();
    run_calc();
    show();
}

void MatricesCalculator::edit_m(NumericMatrix &m, NumericMatrix &nm)
{
    minfo* target_minfo = find_by_id(NumericMatrix::get_id(m));
    target_minfo->set_m(nm);
    do_transpose_m(target_minfo);
    do_inverse_m(target_minfo);

    process_all_ms();
    run_calc();
    show();
}

void MatricesCalculator::process_all_ms()
{
    mlist *_all_ms = extract_ms();

    for (unsigned long i = 0; i < msinfo.size(); ++i)
    {
        minfo *_minfo = msinfo.at(i);
        NumericMatrix* m = _minfo->get_m();

        _minfo->sum_ms = *search_sum_acceptable(m, _all_ms);
        _minfo->sub_ms = *search_sub_acceptable(m, _all_ms);
        _minfo->mul_ms = *search_mul_acceptable(m, _all_ms);
        _minfo->div_ms = *search_div_acceptable(m, _all_ms);
    }
}

void MatricesCalculator::do_transpose_m(minfo *_minfo)
{
    _minfo->mT = NumericMatrix::transpose_m(*_minfo->m);
}

void MatricesCalculator::do_inverse_m(minfo *_minfo)
{
    if (!_conditions.is_inversion_acceptable(*_minfo->m)) return;
    _minfo->mR = NumericMatrix::inverse_m(*_minfo->m);
}

mlist* MatricesCalculator::search_sum_acceptable(NumericMatrix* m, mlist* others)
{
    mlist *_sum_ms = new mlist();
    for (unsigned long it = 0; it < others->ms.size(); ++it)
    {
        if (_conditions.is_sum_acceptable(*m, others->ms[it]))
        {
            _sum_ms->ms.push_back(others->ms[it]);
        }
    }

    return _sum_ms;
}

mlist* MatricesCalculator::search_sub_acceptable(NumericMatrix* m, mlist* others)
{
    mlist *_sub_ms = new mlist();
    for (unsigned long it = 0; it < others->ms.size(); ++it)
    {
        if (_conditions.is_sub_acceptable(*m, others->ms[it]))
        {
            _sub_ms->ms.push_back(others->ms[it]);
        }
    }

    return _sub_ms;
}

mlist* MatricesCalculator::search_mul_acceptable(NumericMatrix* m, mlist* others)
{
    mlist *_mul_ms = new mlist();
    for (unsigned long it = 0; it < others->ms.size(); ++it)
    {
        if (_conditions.is_mul_acceptable(*m, others->ms[it]))
        {
            _mul_ms->ms.push_back(others->ms[it]);
        }
    }

    return _mul_ms;
}

mlist* MatricesCalculator::search_div_acceptable(NumericMatrix* m, mlist* others)
{
    mlist *_div_ms = new mlist();
    for (unsigned long it = 0; it < others->ms.size(); ++it)
    {
        if (_conditions.is_div_acceptable(*m, others->ms[it]))
        {
            _div_ms->ms.push_back(others->ms[it]);
        }
    }
    return _div_ms;
}

minfo* MatricesCalculator::find_by_id(int _id)
{
    bool found = false;
    unsigned long it;
    for (it = 0; it < msinfo.size(); ++it)
    {
        if (NumericMatrix::get_id(*msinfo.at(it)->m) == _id)
        {
            found = true;
            break;
        }
    }

    if (!found) std::cerr << "Not found";

    return msinfo.at(it);
}

mlist* MatricesCalculator::extract_ms()
{
    mlist *all_ms = new mlist();
    for (unsigned long it = 0; it < msinfo.size(); ++it)
        all_ms->ms.push_back(*(msinfo.at(it)->m));

    return all_ms;
}

void MatricesCalculator::run_calc()
{
    for (unsigned long it = 0; it < msinfo.size(); ++it)
    {
        minfo *target_minfo = msinfo.at(it);
        do_sum_ms(target_minfo);
        do_sub_ms(target_minfo);
        do_mul_ms(target_minfo);
        do_div_ms(target_minfo);
    }
}

void MatricesCalculator::do_sum_ms(minfo* _minfo)
{
    for (unsigned long idx = 0; idx < _minfo->sum_ms.ms.size(); ++idx)
    {
        _minfo->_done_sum_ms.ms.push_back(
            (*(_minfo->m) + _minfo->sum_ms.ms.at(idx))
        );
    }
}

void MatricesCalculator::do_sub_ms(minfo* _minfo)
{
    for (unsigned long idx = 0; idx < _minfo->sub_ms.ms.size(); ++idx)
    {
        _minfo->_done_sub_ms.ms.push_back(
            (*(_minfo->m) - _minfo->sub_ms.ms.at(idx))
        );
    }
}

void MatricesCalculator::do_mul_ms(minfo* _minfo)
{
    for (unsigned long idx = 0; idx < _minfo->mul_ms.ms.size(); ++idx)
    {
        _minfo->_done_mul_ms.ms.push_back(
            (*(_minfo->m) * _minfo->mul_ms.ms.at(idx))
        );
    }
}

void MatricesCalculator::do_div_ms(minfo* _minfo)
{
    for (unsigned long idx = 0; idx < _minfo->div_ms.ms.size(); ++idx)
    {
        _minfo->_done_div_ms.ms.push_back(
            (*(_minfo->m) / _minfo->div_ms.ms.at(idx))
        );
    }
}

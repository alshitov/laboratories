#include "matricescalculator.h"
#include "numericmatrix.h"
#include <map>
#include <vector>

MList::MList()
{

}

MDict::MDict()
{

}

MatricesCalculator::MatricesCalculator()
{
}

MatricesCalculator::~MatricesCalculator()
{

}

void MatricesCalculator::m_received(NumericMatrix& m)
{
    std::cout << "received from main:" << std::endl;
    m.show();
    _m_dict.set_m(&m);
    perform_calculations(_m_dict);
}

void MatricesCalculator::perform_calculations(MDict &mdict)
{
    std::cout << "emit calc_done" << std::endl;
    emit calc_done(_m_list);
}

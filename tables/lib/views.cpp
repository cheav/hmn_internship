#include "views.h"
#include "tables.h"
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

//-------------------------------------------------------------------------------------------------
TableView::TableView(BasicTable& t) : m_pTable(&t) {}
TableView::~TableView() {}
void TableView::lineOut(ostream& os)
{
    os << "-----------------------------------------------------------------------------------------"
          "--------------------------";
}
void TableView::out(ostream& os){ }
//-------------------------------------------------------------------------------------------------
HorizontalTableView::HorizontalTableView(BasicTable& t) : TableView(t) {}
void HorizontalTableView::out(ostream& os)
{
    size_t tableSize = m_pTable->Size();
    for (size_t i = 0; i < tableSize; ++i)
        os << setw(12) << (m_pTable->Data(i)).first;
    os << endl;
    for (size_t i = 0; i < tableSize; ++i)
        os << setw(12) << (m_pTable->Data(i)).second;
    os << endl;
    lineOut(os);
}
//-------------------------------------------------------------------------------------------------
VerticalTableView::VerticalTableView(BasicTable& t) : TableView(t) {}
void VerticalTableView::out(ostream& os)
{
    os << endl;
    size_t tableSize = m_pTable->Size();
    for (size_t i = 0; i < tableSize; ++i)
        os << setw(12) << (m_pTable->Data(i)).first << " " << (m_pTable->Data(i)).second << endl;
    lineOut(os);
}
//-------------------------------------------------------------------------------------------------
HorizontalBarTableView::HorizontalBarTableView(BasicTable& t) : TableView(t) {}
void HorizontalBarTableView::out(ostream& os)
{
    os << endl;
    size_t tableSize = m_pTable->Size();
    for (size_t i = 0; i < tableSize; ++i)
        os << setw(12) << (m_pTable->Data(i)).first;
    os << endl;
    for (size_t i = 0; i < tableSize; ++i)
    {
        size_t value = (m_pTable->Data(i)).second;
        string str = "";
        while (value != 0) { str += 'X'; value--; }
        os << setw(12) << str;
    }
    os << endl;
    lineOut(os);
}
//-------------------------------------------------------------------------------------------------
VerticalBarTableView::VerticalBarTableView(BasicTable& t) : TableView(t) {}
void VerticalBarTableView::out(ostream& os)
{
    size_t tableSize = m_pTable->Size();
    size_t hmax = m_pTable->maxValue() + 1;
    for (size_t j = 0; j < hmax; ++j){
        for (size_t i = 0; i < tableSize; ++i)
        {
            size_t value = (m_pTable->Data(i)).second;
            size_t sum = value + j;
            if (sum < hmax) os << setw(12) << ' ';
            else os << setw(12) << 'X';
        }
        os << endl;
    }
    for (size_t i = 0; i < tableSize; ++i)
        os << setw(12) << (m_pTable->Data(i)).first;
    os << endl;
    lineOut(os);
}
//-------------------------------------------------------------------------------------------------

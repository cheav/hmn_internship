#include "tables.h"
#include "views.h"
#include <iomanip>
#include <iostream>
using namespace std;

//-------------------------------------------------------------------------------------------------
BasicTable::BasicTable(const string& name) : m_strName(name) {}
BasicTable::~BasicTable(){}

string BasicTable::Name() const { return m_strName; }
size_t BasicTable::Size() const { return m_unmapTable.size(); }

void BasicTable::addData(const string& valueName, size_t value)
{
    m_unmapTable.insert({valueName, value});
}
const pair<const string, size_t>& BasicTable::Data(size_t npos) const
{	
    unordered_map<string, size_t>::const_iterator it;
    if (npos > m_unmapTable.size() - 1u){ it = m_unmapTable.cend(); return *it; }
    it = m_unmapTable.cbegin();
    for (;it != m_unmapTable.cend(); ++it, --npos) if(npos == 0) break;
    return *it;
}
size_t BasicTable::maxValue() const
{
    size_t value = 0;
    size_t tempValue = 0;
    size_t size = Size();
    for(size_t pos = 0; pos < size; ++pos)
    {
        tempValue = (Data(pos)).second;
        if(value < tempValue) value = tempValue;
    }
    return value;
}
//-------------------------------------------------------------------------------------------------
Table::Table(const std::string& name, size_t nViewsCount) : BasicTable(name)
{
    TableView *pTableView{};

    switch (nViewsCount) {
    case 1:
        pTableView = new HorizontalTableView(*this);
        m_vecTableViews.push_back(pTableView);
        break;
    case 2:
        pTableView = new HorizontalTableView(*this);
        m_vecTableViews.push_back(pTableView);
        pTableView = new VerticalTableView(*this);
        m_vecTableViews.push_back(pTableView);
        break;
    case 3:
        pTableView = new HorizontalTableView(*this);
        m_vecTableViews.push_back(pTableView);
        pTableView = new VerticalTableView(*this);
        m_vecTableViews.push_back(pTableView);
        pTableView = new HorizontalBarTableView(*this);
        m_vecTableViews.push_back(pTableView);
        break;
    case 4:
        pTableView = new HorizontalTableView(*this);
        m_vecTableViews.push_back(pTableView);
        pTableView = new VerticalTableView(*this);
        m_vecTableViews.push_back(pTableView);
        pTableView = new HorizontalBarTableView(*this);
        m_vecTableViews.push_back(pTableView);
        pTableView = new VerticalBarTableView(*this);
        m_vecTableViews.push_back(pTableView);
        break;
    default:
        break;
    }
}
Table::~Table()
{
    for(TableView *pTableView : m_vecTableViews)
        delete pTableView;
}
const std::vector<TableView*>& Table::tableViews() const
{
    return m_vecTableViews;
}
//-------------------------------------------------------------------------------------------------

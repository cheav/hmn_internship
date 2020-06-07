#ifndef TABLES_H
#define TABLES_H

#include <cstddef>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class TableView;

class BasicTable
{
    std::string m_strName;
    std::unordered_map<std::string, size_t> m_unmapTable;
public:
    BasicTable(const std::string& name);
    virtual ~BasicTable();
    std::string Name() const;
    size_t Size() const;
    void addData(const std::string& valueName, size_t value);
    const std::pair<const std::string, size_t>& Data(size_t npos) const;
    size_t maxValue() const;
};
//-------------------------------------------------------------------------------------------------
class Table : public BasicTable
{
public:
    Table(const std::string& name, size_t nViewsCount);
    ~Table();
    const std::vector<TableView *> &tableViews() const;
private:
    std::vector<TableView*> m_vecTableViews;
};

//-------------------------------------------------------------------------------------------------
#endif // TABLES_H

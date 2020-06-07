#ifndef VIEWS_H
#define VIEWS_H

#include <iosfwd>

class BasicTable;

class TableView
{
protected:
    BasicTable *m_pTable;
public:
    TableView(BasicTable& t);
    virtual ~TableView();
    virtual void out(std::ostream& os);
    void lineOut(std::ostream& os);
};
//
class HorizontalTableView : public TableView
{
public:
    HorizontalTableView(BasicTable& t);
    void out(std::ostream& os) override;
};
//
class VerticalTableView : public TableView
{
public:
    VerticalTableView(BasicTable& t);
    void out(std::ostream& os) override;
};
class HorizontalBarTableView : public TableView
{
public:
    HorizontalBarTableView(BasicTable& t);
    void out(std::ostream& os) override;
};
//
class VerticalBarTableView : public TableView
{
public:
    VerticalBarTableView(BasicTable& t);
    void out(std::ostream& os) override;
};

#endif // VIEWS_H

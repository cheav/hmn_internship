#include "cout.h"
#include <iostream>

std::ostream& operator << (std::ostream& os, TableView& tw)
{
	tw.out(os);
	return os;
}

std::ostream& operator << (std::ostream& os, Table& t)
{
    auto TableViews = t.tableViews();
    for(auto tw : TableViews)
    {
        tw->out(os);
    }
    return os;
}

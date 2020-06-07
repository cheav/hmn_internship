#ifndef COUT_H
#define COUT_H
#include "tables.h"
#include "views.h"

// polymorphic operator - tableviews out directly:
std::ostream& operator << (std::ostream& os, TableView& tw);

// operator - tables out using tableviews operator:
std::ostream& operator << (std::ostream& os, Table& t);

#endif // COUT_H



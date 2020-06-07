#ifndef COUT_H
#define COUT_H
#include <iostream>
#include "complex.h"
using std::cout; using std::endl; using std::ostream;

ostream& operator << (ostream& os, const Complex& z);

#endif // COUT_H



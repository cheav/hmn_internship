#include "cout.h"

ostream& operator << (ostream& os, const Complex& z)
{
	os << "(" << z.m_r << "; " << z.m_phi << ")";
	return os;
}

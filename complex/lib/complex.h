#ifndef COMPLEX_H
#define COMPLEX_H
#include <iostream>
#include <cmath>
const double Pi = 3.1415926535897932384626433832795;
const double e = 1.0e-10;

struct ComplexA;
struct Complex
{
	double m_r;
	double m_phi;
	Complex(double r = 0.0, double phi = 0.0);
	ComplexA toAlgebraic();
	Complex toTrigonometric(ComplexA z);
	void Normalize();
	Complex operator + (Complex z);
	Complex operator - (Complex z);
	Complex operator * (Complex z);
	Complex operator / (Complex z);
	Complex power(int n);
};
struct ComplexA
{
	double m_x;
	double m_y;
	ComplexA(double x = 0.0, double y = 0.0);
};

#endif // COMPLEX_H
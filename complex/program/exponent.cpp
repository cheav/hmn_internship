#include "exponent.h"

double factorial(double n)
{
	if (n == 0.0) return 1.0;
	double fact = 1.0;
	for (double i = 1.0; i <= n; ++i) fact *= i;
	return fact;
}
Complex ExpTaylor(Complex z, double eps)
{
	Complex row;
	for (double n = 0.0; ; ++n)
	{
		Complex item = z.power(n) / Complex(factorial(n), 0.0);
		if (item.m_r < eps) break;
		row = row + item;
	}
	return row;
}
Complex ExpEuler(Complex z)
{
	ComplexA za = z.toAlgebraic();
	double exp_real = exp(za.m_x) * cos(za.m_y);
	double exp_imag = exp(za.m_x) * sin(za.m_y);
	ComplexA exp_za(exp_real, exp_imag);
	Complex exp_zt = (Complex()).toTrigonometric(exp_za);
	exp_zt.Normalize();
	return exp_zt;
}


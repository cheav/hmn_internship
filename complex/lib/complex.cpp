#include "Complex.h"

Complex::Complex(double r, double phi) : m_r(r), m_phi(phi) {}

ComplexA Complex::toAlgebraic()
{
	return ComplexA(m_r * cos(m_phi), m_r * sin(m_phi));
}
Complex Complex::toTrigonometric(ComplexA z)
{
	Complex zt;
	if (fabs(z.m_x) < e && fabs(z.m_y) > e) { zt.m_phi = Pi / 2.0;  zt.m_r = z.m_y; }
	else if (fabs(z.m_x) < e && fabs(z.m_y) < e) { zt.m_r = 0.0; zt.m_phi = 0.0; }
    else
    {
		zt.m_r = sqrt(z.m_x*z.m_x + z.m_y*z.m_y);  
		if(fabs(fabs(z.m_y / z.m_x) - 1.0) < e) zt.m_phi = atan2(z.m_y, z.m_x);
		else zt.m_phi =  Pi/2.0 - atan2(z.m_x, z.m_y);
	}
	return zt;
}
void Complex::Normalize()
{
	double phi = m_phi / Pi;
	int nphi = (int) phi;
	if (nphi >= 0 && nphi < 2) return;
	else if (nphi >= 2) 
	{ 
		if(nphi % 2 == 0) phi -= (double)nphi; 
		else phi -= (double)(nphi - 1.0);
	}
	else if (nphi < 0)
	{
		if (abs(nphi) % 2 == 0) phi += (fabs(nphi) + 2.0);
		else phi += ((fabs(nphi) - 1.0) + 2.0);
	}
	m_phi = phi * Pi;
}
Complex Complex::operator + (Complex z)
{
	z.Normalize();
	double x = m_r * cos(m_phi) + z.m_r*cos(z.m_phi);
	double y = m_r * sin(m_phi) + z.m_r*sin(z.m_phi);
	ComplexA za(x,y);
	Complex sum = toTrigonometric(za); 
	sum.Normalize();
	return sum;
}
Complex Complex::operator - (Complex z)
{
	z.Normalize();
	double x = m_r * cos(m_phi) - z.m_r*cos(z.m_phi);
	double y = m_r * sin(m_phi) - z.m_r*sin(z.m_phi);
	ComplexA za(x, y);
	Complex sub = toTrigonometric(za);
	sub.Normalize();
	return sub;
}
Complex Complex::operator * (Complex z)
{
	Complex mult(m_r * z.m_r, m_phi + z.m_phi);
	mult.Normalize();
	return mult;
}
Complex Complex::operator / (Complex z)
{
	if (z.m_r == 0.0) return Complex(0.0, 0.0);
	Complex div(m_r/z.m_r, m_phi - z.m_phi);
	div.Normalize();
	return div;
}
Complex Complex::power(int n)
{
	Complex p(pow(m_r, n), (double)(n*m_phi));
	p.Normalize();
	return p;
}
//-------------------------------------------------------------------------------------------------
ComplexA::ComplexA(double x, double y) : m_x(x), m_y(y) {}
//-------------------------------------------------------------------------------------------------

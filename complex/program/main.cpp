#include <iostream>
#include "exponent.h"
#include "cout.h"

int main()
{
	Complex z1(3.0, 1.0*Pi), z2(2.0, -40.0*Pi);

	cout << "Sum " << z1 << " + " << z2 << " = " << z1 + z2 << endl;
	cout << "Sub " << z1 << " - " << z2 << " = " << z1 - z2 << endl;
	cout << "Mult " << z1 << " * " << z2 << " = " << z1 * z2 << endl;
	cout << "Div " << z1 << " / " << z2 << " = " << z1 / z2 << endl;
	cout << "Pow " << z1 << " ^ " << 3 << " = " << z1.power(3) << endl << endl;
	
	Complex z3(4.0, 6.7*Pi);
	double eps = 1.0e-5;
	cout << "Exp Taylor (r,phi) from " << z3 << " = " << ExpTaylor(z3, eps) << endl;
	cout << "Exp Euler (r,phi) from " << z3 << " = " << ExpEuler(z3) << endl << endl;

    //system("pause");
	return 0;
}

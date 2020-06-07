#include <iostream>
#include "mystring.h"
using std::cout; using std::endl; using std::ostream;

int main()
{
	String str1("Word1");
	String str2(str1);
	String str3("Word3");

	cout << "str1: " << str1 << endl;
	cout << "copyStr1: " << str2 << endl;
	cout << "str3: " << str3 << endl;
	str3 = str1;
	cout << "str3: " << str3 << endl;

    //system("pause");
	return 0;
}

#include <iostream>
#include <cstring>
#include <cstdlib>
using std::cout; using std::endl; 

void replace(char* pszResult, const char *pcszSubstr, const char *pcszStr)
{
	size_t len1 = std::strlen(pcszSubstr);
	size_t len2 = std::strlen(pcszStr);
	int num = (int)len1;
	bool coin = false; // true - совпадение найдено
	while (*pszResult != '\0')
	{
		if (coin) // замена подстроки
		{
			char *pLast = pszResult + len2 - 1;
			const char *pcszStrTemp = pcszStr;
			if (len2 <= len1) 
			{
				while (pszResult <= pLast) { *pszResult = *pcszStrTemp; ++pszResult; ++pcszStrTemp; }
				if (len2 < len1)
				{    
					char *pszDest = pszResult;
					pszResult += (len1 - len2);
					size_t n = std::strlen(pszResult);
					std::memmove(pszDest, pszResult, n);
					pszDest += n; *pszDest = '\0';
				}
			}
			else {
				size_t n = std::strlen(pszResult);
				char *pszDest = pszResult + (len2 - len1);
				std::memmove(pszDest, pszResult, n);
				pszDest += n; *pszDest = '\0';
				while (pszResult <= pLast) { *pszResult = *pcszStrTemp; ++pszResult; ++pcszStrTemp; }
			}
			coin = false;
		}
		if (*pszResult == *pcszSubstr) // поиск подстроки
		{
			num -= 1; ++pszResult; ++pcszSubstr;
			if (num > 0) { coin = false; }
			else { num = len1; pcszSubstr -= len1; pszResult -= len1; coin = true; }		
			continue;
		}
		++pszResult;
	}
	*pszResult = '\0';
}

int main()
{
	char res[100] = "ABABCDCDCD--ABABCD__";
	char subStr[] = "ABCD";
	char str[] = "0F3N";
	cout << "initial  string: " << res << endl;
	cout << "substirng: " << subStr << endl;
	cout << "replace string: " << str << endl;
	replace(res, subStr, str);
	cout << "replaced string: " << res << endl << endl;
	
	char subStr2[] = "0F3N";
	char str2[] = "QWE";
	cout << "initial  string: " << res << endl;
	cout << "substirng: " << subStr2 << endl;
	cout << "replace string: " << str2 << endl;
	replace(res, subStr2, str2);
	cout << "replaced string: " << res << endl << endl;

	char subStr3[] = "Q";
	char str3[] = "R";
	cout << "initial  string: " << res << endl;
	cout << "substirng: " << subStr3 << endl;
	cout << "replace string: " << str3 << endl;
	replace(res, subStr3, str3);
	cout << "replaced string: " << res << endl << endl;

	char subStr4[] = "RWE";
	char str4[] = "OPKLM";
	cout << "initial  string: " << res << endl;
	cout << "substirng: " << subStr4 << endl;
	cout << "replace string: " << str4 << endl;
	replace(res, subStr4, str4);
	cout << "replaced string: " << res << endl << endl;
	
    //system("pause");
	return 0;
}

#ifndef MYSTRING_H
#define MYSTRING_H
#include <cstdlib>
#include <cstring>

class String
{
       char *m_pszStr;
public:
       String(const char *pcszStr);
       ~String();
	   String(const String& strCopy);
	   String& operator = (const String &strCopy);
       operator const char *() const;
};

#endif // MYSTRING_H

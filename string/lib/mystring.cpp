#include "mystring.h"

String::String(const char *pcszStr)
    : m_pszStr((char *) std::calloc(1 + std::strlen(pcszStr), sizeof (char)))
{ 
    if(m_pszStr != nullptr)
        std::strcpy(m_pszStr, pcszStr);
    else
        m_pszStr = nullptr;
}
String::~String()
{
    if(m_pszStr != nullptr) std::free(m_pszStr);
}
String::String(const String& strCopy) 
{
	if (strCopy.m_pszStr != nullptr)
	{
		m_pszStr = (char*)std::calloc(1 + std::strlen(strCopy.m_pszStr), sizeof(char));
		std::strcpy(m_pszStr, strCopy.m_pszStr);
	}
	else m_pszStr = nullptr;
}
String &String::operator = (const String &strCopy) 
{
	if ((this != &strCopy) && (strCopy.m_pszStr != nullptr))
	{
		if (m_pszStr != nullptr) std::free(m_pszStr);
		m_pszStr = (char*)std::calloc(1 + std::strlen(strCopy.m_pszStr), sizeof(char));
		std::strcpy(m_pszStr, strCopy.m_pszStr);
	}
	return *this;
}
String::operator const char *() const
{
    return m_pszStr;
}


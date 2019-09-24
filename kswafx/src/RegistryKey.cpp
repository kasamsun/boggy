#include "RegistryKey.h"


C_RegistryKey::C_RegistryKey()
{
	m_hKey = NULL;
}

C_RegistryKey::~C_RegistryKey()
{
	Close();
}

LONG C_RegistryKey::Open(HKEY hKeyRoot, LPCTSTR lpszPath)
{
	DWORD dw;

	strcpy(m_szPath,lpszPath);

	return RegCreateKeyEx(hKeyRoot, lpszPath, 0L, NULL,
		REG_OPTION_VOLATILE, KEY_ALL_ACCESS, NULL,
		&m_hKey, &dw);
}

void C_RegistryKey::Close()
{
	if (m_hKey)
	{
		RegCloseKey(m_hKey);
		m_hKey = NULL;
	}
}

LONG C_RegistryKey::Write(LPCTSTR lpszKey, DWORD dwVal)
{
//	ASSERT(m_hKey);
//	ASSERT(lpszKey);

	return RegSetValueEx(m_hKey, lpszKey, 0L, REG_DWORD,
		(CONST unsigned char*) &dwVal, sizeof(DWORD));
}

LONG C_RegistryKey::Write(LPCTSTR lpszKey, LPCTSTR lpszVal)
{
//	ASSERT(m_hKey);
//	ASSERT(lpszKey);
//	ASSERT(lpszVal);
//	ASSERT(AfxIsValidAddress(lpszVal, strlen(lpszVal), FALSE));

	return RegSetValueEx(m_hKey, lpszKey, 0L, REG_SZ,
		(CONST unsigned char*) lpszVal, strlen(lpszVal) + 1);
}

LONG C_RegistryKey::Write(LPCTSTR lpszKey, const unsigned char* pData, DWORD dwLen)
{
//	ASSERT(m_hKey);
//	ASSERT(lpszKey);
//	ASSERT(pData && dwLen > 0);
//	ASSERT(AfxIsValidAddress(pData, dwLen, FALSE));

	return RegSetValueEx(m_hKey, lpszKey, 0L, REG_BINARY,
		pData, dwLen);
}

LONG C_RegistryKey::Read(LPCTSTR lpszKey, DWORD& dwVal)
{
//	ASSERT(m_hKey);
//	ASSERT(lpszKey);

	DWORD	dwType;
	DWORD	dwSize = sizeof(DWORD);
	DWORD	dwDest;

	LONG lRet = RegQueryValueEx(m_hKey, (LPSTR)lpszKey, NULL,
		&dwType, (unsigned char*) &dwDest, &dwSize);

	if (lRet == ERROR_SUCCESS)
		dwVal = dwDest;

	return lRet;
}

LONG C_RegistryKey::Read(LPCTSTR lpszKey, LPSTR szVal, int nBufLen)
{
//	ASSERT(m_hKey);
//	ASSERT(lpszKey);

	DWORD	dwType;
	DWORD	dwSize = MAX_REGISTRY_READ_BUFFER;
	char	str[MAX_REGISTRY_READ_BUFFER];

	LONG lRet = RegQueryValueEx(m_hKey, (LPSTR)lpszKey, NULL,
		&dwType, (unsigned char*) str, &dwSize);

    // now i don't check output buffer size. do it later
	if (lRet == ERROR_SUCCESS)
    {
        if ( strlen(str) > (UINT)nBufLen )
            strncpy(szVal,str,nBufLen-1);
        else
    		strcpy(szVal,str);
    }
	return lRet;
}

LONG C_RegistryKey::Read(LPCTSTR lpszKey, unsigned char* pData, DWORD dwLen)
{
//	ASSERT(m_hKey);
//	ASSERT(lpszKey);

	DWORD	dwType;

	return 	RegQueryValueEx(m_hKey, (LPSTR)lpszKey, NULL,
		&dwType, pData, &dwLen);
}

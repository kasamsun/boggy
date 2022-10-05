#ifndef _C_REGISTERKEY_H_
#define _C_REGISTERKEY_H_

#include <windows.h>
#include <winreg.h>
// Import library : advapi32.lib

#define MAX_REGISTRY_PATH 			256
#define MAX_REGISTRY_READ_BUFFER	256

class C_RegistryKey
{
public:
	C_RegistryKey();
	virtual ~C_RegistryKey();

public:
	LONG Open(HKEY hKeyRoot, LPCTSTR lpszPath);
	void Close();

	LONG Write(LPCTSTR lpszKey, DWORD dwVal);
	LONG Write(LPCTSTR lpszKey, LPCTSTR lpszVal);
	LONG Write(LPCTSTR lpszKey, const unsigned char* pData, DWORD dwLen);

	LONG Read(LPCTSTR lpszKey, DWORD& dwVal);
	LONG Read(LPCTSTR lpszKey, LPSTR szVal, int nBufLen);
	LONG Read(LPCTSTR lpszKey, unsigned char* pData, DWORD dwLen);

protected:
	HKEY m_hKey;
	char m_szPath[MAX_REGISTRY_PATH];
};

#endif
#ifndef _MAINAFX_H_
#define _MAINAFX_H_

/////////////////// WINDOWS API (WIN32API) //////////////////
#include <windows.h>

//////////////////////// TRACE MACRO ////////////////////////
#ifdef _DEBUG
// ======= DEBUG version ============
#include <stdio.h>
#include <stdarg.h>

#define MAX_TRACE_BUFFER 256
#ifdef __cplusplus
extern "C"
#endif
void _Trace(LPCTSTR lpszFormat, ...);
#define TRACE              _Trace

#else
//======== RELEASE version =========
inline void _Trace(LPCTSTR, ...) { }
#define TRACE              1 ? (void)0 : _Trace
#endif

//////////////// General Message Handler MACRO //////////////////

#define BEGIN_HANDLE_MSG(classname,baseclassname) \
LRESULT classname::WndProc(UINT iMessage,WPARAM wParam,LPARAM lParam) \
{ \
	switch (iMessage) \
	{ \
	default: \
		return baseclassname::WndProc( iMessage, wParam, lParam );

#define BEGIN_BASE_HANDLE_MSG(classname) \
LRESULT classname::WndProc(UINT iMessage,WPARAM wParam,LPARAM lParam) \
{ \
	switch (iMessage) \
	{ \
	default: \
		return DefWindowProc( m_hWnd, iMessage, wParam, lParam );

#define HANDLE_MSG(message, fnMember) \
	case message:       \
		return fnMember(wParam,lParam);

#define END_HANDLE_MSG() \
	} \
}

////////////////////// BASE WINDOW & APP CLASS //////////////////////

LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage,
								 WPARAM wParam, LPARAM lParam );
class C_Wnd;

class C_App
{
public:
	C_App() : m_pMainWnd(NULL) {}
    virtual ~C_App() {}
    
	static HINSTANCE m_hInstance;
	static HINSTANCE m_hPrevInstance;
	static LPSTR m_lpszCmdLine;
	static int m_nCmdShow;

	static int MessageLoop( void );
	C_Wnd * GetMainWnd() { return m_pMainWnd; }
	void SetMainWnd(C_Wnd * pMainWnd) { m_pMainWnd = pMainWnd; }

	void GetExecDirectory(char *szDirPath,int size);

protected:
	C_Wnd * m_pMainWnd;

};

class C_Wnd
{
public:
	C_Wnd() : m_hWnd(NULL),m_hDC(NULL),m_pParentWnd(NULL) {}
	virtual ~C_Wnd() {}

	HWND GetHandle( void ) { return m_hWnd; }
	void SetHandle( HWND hWnd) { m_hWnd = hWnd; }

	// GetParent return topmost parent, so i do it myself
	C_Wnd * GetParentWnd() { return m_pParentWnd; }

	// Pure virtual function makes Window an abstract class.
	virtual LRESULT WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam ) = 0;

protected:
	HWND m_hWnd;
	HDC m_hDC;

	C_Wnd * m_pParentWnd;
private:

};

#endif
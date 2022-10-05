#ifndef _C_COOLSTATIC_H_
#define _C_COOLSTATIC_H_

#include <Static.h>

class C_Wnd;

class C_CoolStatic : public C_Static
{
public:
	C_CoolStatic();
	~C_CoolStatic();

	BOOL Create(LPCTSTR lpszCaption,DWORD dwExStyle,DWORD dwStyle,
				const RECT& rect,C_Wnd* pParentWnd,
				UINT nID);
    int SubclassControl(HWND hWnd,C_Wnd* pParentWnd);

	LRESULT WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam );
    LRESULT OnPaint( WPARAM, LPARAM );

private:
	int PaintText(HDC);
	int PaintIcon(HDC);
	int PaintBitmap(HDC);
};

#endif
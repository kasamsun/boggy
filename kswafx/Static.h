#ifndef _C_STATIC_H_
#define _C_STATIC_H_

#include <Control.h>

class C_Wnd;

class C_Static : public C_Control
{
public:
	C_Static();
	~C_Static();

	BOOL Create(LPCTSTR lpszCaption,DWORD dwExStyle,DWORD dwStyle,
				const RECT& rect,C_Wnd* pParentWnd,
				UINT nID);

	LRESULT WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam );

protected:

private:

};

#endif
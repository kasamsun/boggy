#ifndef _C_GENERICWINDOW_H_
#define _C_GENERICWINDOW_H_

#include "MainAfx.h"

class C_GenericWnd : public C_Wnd
{
public:
	static void Register(UINT style,HICON,HCURSOR,HBRUSH,
						LPCTSTR menuname,LPCTSTR classname);
	C_GenericWnd(C_Wnd *pParentWnd=NULL);
	~C_GenericWnd();

	LRESULT WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam );
protected:
	static char m_szClassName[16];
};

#endif
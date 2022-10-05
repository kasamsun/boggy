#ifndef _C_BKGWINDOW_H_
#define _C_BKGWINDOW_H_

#include "MainAfx.h"
#include "GenericWnd.h"

class C_BoggyWnd;

class C_BkgWnd : public C_GenericWnd
{
public:
	C_BkgWnd();
	~C_BkgWnd();

    LRESULT WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam );
#ifdef _DEBUG
    LRESULT OnPaint(WPARAM,LPARAM);
#endif
	LRESULT OnDestroy(WPARAM,LPARAM);
private:
	C_BoggyWnd  *pBoggyWnd;

};

#endif
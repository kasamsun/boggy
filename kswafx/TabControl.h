#ifndef _C_TABCONTROL_H_
#define _C_TABCONTROL_H_

#include <Control.h>

class C_Wnd;

class C_TabControl : public C_Control
{
public:
	C_TabControl();
	~C_TabControl();

	BOOL Create(const RECT& rect,C_Wnd* pParentWnd);
    int InsertItem(int iItem,UINT mask,LPSTR pszText,int cchTextMax);

	LRESULT WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam );

protected:

private:
};

#endif
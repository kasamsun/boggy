#ifndef _C_EDIT_H_
#define _C_EDIT_H_

#include <Control.h>

class C_Wnd;

class C_Edit : public C_Control
{
public:
	C_Edit();
	~C_Edit();

	BOOL Create(LPCTSTR lpszCaption,DWORD dwExStyle,DWORD dwStyle,
				const RECT& rect,C_Wnd* pParentWnd,
				UINT nID);

	int GetLimitText();
	void SetLimitText(short cbMax);

	LRESULT WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam );

protected:

private:
};

#endif
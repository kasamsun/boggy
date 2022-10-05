#ifndef _C_BUTTON_H_
#define _C_BUTTON_H_

#include <Control.h>

class C_Wnd;

class C_Button : public C_Control
{
public:
	C_Button();
	~C_Button();

	BOOL Create(LPCTSTR lpszCaption,DWORD dwExStyle,DWORD dwStyle,
				const RECT& rect,C_Wnd* pParentWnd,
				UINT nID);

    HANDLE GetImage(WORD fImageType);
    HANDLE SetImage(WORD fImageType,HANDLE hImage);
	LRESULT GetCheck();
	void SetCheck(WORD fCheck);

	LRESULT WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam );

protected:

private:
};

#endif
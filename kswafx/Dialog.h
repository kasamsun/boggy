#ifndef _C_DIALOG_H_
#define _C_DIALOG_H_

#include <MainAfx.h>

#define	CENTER_ON_PARENT	0
#define CENTER_ON_SCREEN	1

class C_Dialog : public C_Wnd
{
public:
	C_Dialog();
	~C_Dialog();

	virtual void Refresh() {}
	BOOL Create( UINT nIDTemplate, C_Wnd* pParentWnd = NULL );
	int DoModal();


	LRESULT WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam );
private:
	UINT m_nDialogID;
};

#endif
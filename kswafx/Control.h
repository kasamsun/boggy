#ifndef _C_CONTROL_H_
#define _C_CONTROL_H_

#include <MainAfx.h>

#define SUBCLASS_CONTROL(ID,Button) \
    if ( GetDlgItem(m_hWnd,ID) != NULL ) \
        Button->SubclassControl(GetDlgItem(m_hWnd,ID),this);

class C_Control : public C_Wnd
{
public:
	C_Control();
	~C_Control();

    // =============== undefined interface ==========================
	BOOL Create(LPCTSTR lpszCaption,
						DWORD dwExStyle,
						DWORD dwStyle,
						const RECT& rect,
						C_Wnd* pParentWnd,
						UINT nID);

    // ================ Interface ===================================
	HFONT GetFont();
	void SetFont(HFONT hFont,BOOL bRedraw);
	int SetText(LPCTSTR lpsz);

    BOOL InvalidateParentRect(BOOL bErase);

    // ============= Subclass function ==============================
    int SubclassControl(HWND hWnd,C_Wnd *pParentWnd);
	LRESULT CleanupSubclass(UINT iMessage,WPARAM wParam,LPARAM lParam);

	LRESULT WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam );
protected:
    WNDPROC m_lpfnDefWndProc;

private:
};

#endif
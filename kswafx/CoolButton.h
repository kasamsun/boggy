#ifndef _C_COOLBUTTON_H_
#define _C_COOLBUTTON_H_

#include <Button.h>

#define CURSOR_INSIDE		0x0001
#define CURSOR_LPRESSED     0x0002


class C_Wnd;

class C_CoolButton : public C_Button
{
public:
	C_CoolButton();
	~C_CoolButton();

	BOOL Create(LPCTSTR lpszCaption,DWORD dwExStyle,DWORD dwStyle,
				const RECT& rect,C_Wnd* pParentWnd,
				UINT nID);
    int SubclassControl(HWND hWnd,C_Wnd *pParentWnd);
	void EnableKBMsgParentNotify(int bFlag);
	void EnableFocusOnMouseMove(int bFlag);

	LRESULT WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam );
    LRESULT OnMouseMove(WPARAM,LPARAM);
    LRESULT OnEraseBkgnd(WPARAM,LPARAM);
    LRESULT OnPaint(WPARAM,LPARAM);
	LRESULT OnSetFocus(WPARAM wParam,LPARAM lParam);
    LRESULT OnKillFocus(WPARAM,LPARAM);
    LRESULT OnLButtonDown(WPARAM,LPARAM);
    LRESULT OnLButtonUp(WPARAM,LPARAM);
	LRESULT OnLButtonDblClk(WPARAM wParam,LPARAM lParam);
    LRESULT OnKeyDown(WPARAM,LPARAM);
    LRESULT OnKeyUp(WPARAM,LPARAM);
    LRESULT OnChar(WPARAM,LPARAM);
	LRESULT OnEnable(WPARAM wParam,LPARAM lParam);

    LRESULT OnBMSetState(WPARAM,LPARAM);
    LRESULT OnBMSetCheck(WPARAM,LPARAM);

protected:
	UINT GetFormatFromStyle(long lCtlStyle);

	int m_nMouseStatus;
	int m_bKBMsgParentNotify;
	int m_bFocusOnMouseMove;
private:
	int PaintText(HDC hDC);
	int PaintIcon(HDC hDC);
	int PaintBitmap(HDC hDC);
	int PaintCheck(HDC hDC);
	int PaintRadio(HDC hDC);
	int PaintGroupBox(HDC hDC);

};

#endif
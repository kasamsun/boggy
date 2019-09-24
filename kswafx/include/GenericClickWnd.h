#ifndef _C_GENERICCLICKWND_H_
#define _C_GENERICCLICKWND_H_

//=============================================================
// Note : if derive class implement WM_NCLBUTTONDOWN
//		to move entire window. you must implement WM_MOUSEMOVE
//==============================================================

#define WM_GENERICCLICKWND_BEGIN_DRAG		WM_USER+100
#define WM_GENERICCLICKWND_END_DRAG			WM_USER+101
// WM_GENERICCLICKWND_DRAG_XXXX  will be sent to parent to notify that
//                               window has begun/ended drag
//  WPARAM - xPos = LOWORD(wParam);  // horizontal position of window
//           yPos = HIWORD(wParam);  // vertical position of window
//  LPARAM - genericclickwnd pointer
#define WM_GENERICCLICKWND_LBUTTONCLICK		WM_USER+110
#define WM_GENERICCLICKWND_RBUTTONCLICK		WM_USER+111
//  LPARAM - genericclickwnd pointer


#include <GenericWnd.h>

class C_GenericClickWnd : public C_GenericWnd
{
public:
	C_GenericClickWnd(C_Wnd *pParentWnd=NULL);
	~C_GenericClickWnd();

	void SetActiveCursor(HCURSOR hActiveCursor);
	void SetClickCursor(HCURSOR hClickCursor);
	void SetDragCursor(HCURSOR hDragCursor);

	void EnableDragWindow(int bFlag);
	void EnableKBMsgParentNotify(int bFlag);
    void EnableClickToTop(int bFlag);
	void EnableClickNotify(int bFlag);

	void EnableMoveBkgWindow(int bFlag);
    void MoveBkgWindow(int left,int top);
    void Refresh();
    virtual void Paint(HDC hDC,int x,int y) {}

	LRESULT WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam );
    LRESULT OnLButtonDown(WPARAM,LPARAM);
    LRESULT OnLButtonUp(WPARAM,LPARAM);
    LRESULT OnRButtonDown(WPARAM,LPARAM);
    LRESULT OnRButtonUp(WPARAM,LPARAM);
    LRESULT OnMouseMove(WPARAM,LPARAM);
    LRESULT OnKillFocus(WPARAM,LPARAM);
    LRESULT OnChar(WPARAM,LPARAM);
	LRESULT OnNCCalcSize(WPARAM wParam,LPARAM lParam);
    LRESULT OnEraseBkgnd(WPARAM wParam,LPARAM lParam);

protected:
	int m_bMouseAbove;
	int m_bLPressed;
    int m_bRPressed;

	int m_bDragWindow;
	int m_bKBMsgParentNotify;
	int m_bClickToTop;
	int m_bClickNotify;

    // using Move with transparent foreground image
   	HDC m_hdcBkg;
	HBITMAP m_hbmpBkg,m_hbmpBkgOld;
    void SetBkgImage();
    void ClearBkgImage();
    int m_bMoveBkgWindow;

private:
	int m_nClickPosX;
	int m_nClickPosY;
    int m_nPosX;
    int m_nPosY;

    // ========= for MoveBkgWindow
	int m_bMoveWithNoBitCopy;
    int m_bInitBkg;
    // ===========================

	HCURSOR m_hActiveCursor;
	HCURSOR m_hClickCursor;
	HCURSOR m_hDragCursor;
	HCURSOR m_hTmpCursor;
};

#endif
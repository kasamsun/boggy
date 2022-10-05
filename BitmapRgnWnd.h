#ifndef _C_BITMAPRGNWND_H_
#define _C_BITMAPRGNWND_H_

#include "kswafx/GenericWnd.h"

class C_BitmapRgnWnd : public C_GenericWnd
{
public:
	C_BitmapRgnWnd(HWND hwndParent,int nResID);
	~C_BitmapRgnWnd();

	///////////////////////////////////////////////////////
    LRESULT WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam );
	LRESULT OnPaint(WPARAM,LPARAM);
	LRESULT OnLButtonDown(WPARAM,LPARAM);
	LRESULT OnMouseMove(WPARAM,LPARAM);
	LRESULT OnLButtonUp(WPARAM,LPARAM);
	LRESULT OnRButtonUp(WPARAM,LPARAM);
	LRESULT OnDestroy(WPARAM,LPARAM);
	///////////////////////////////////////////////////////

	HRGN CreateBitmapRgn(HBITMAP hbmpMsk,COLORREF cTransparentColor);

	int RunModalLoop();//DWORD dwFlags);
	BOOL ContinueModal();
	void EndModalLoop();//int nResult);
	DWORD GetStyle() { return (DWORD)GetWindowLong(m_hWnd, GWL_STYLE); }
	BOOL PumpMessage();
	BOOL IsIdleMessage(MSG* pMsg);
	////////////////////
	UINT m_nFlags;      // see WF_ flags above
	int m_nModalResult; // for return values from CWnd::RunModalLoop

	POINT m_ptCursorLast;      // last mouse position
	UINT m_nMsgLast;            // last mouse message
protected:
	BOOL m_bLPressed;
	int m_nPosX,m_nPosY;
private:
	HBITMAP	m_hbmpDialog,m_hbmpDialogOld;
	HDC		m_hdcDialog;

};

#endif
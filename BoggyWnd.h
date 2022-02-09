#ifndef _C_BOGGY_H_
#define _C_BOGGY_H_

#include <MainAfx.h>
#include <GenericWnd.h>

#include "BoggyAI.h"
#include "BoggyAction.h"


typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)(HWND hWnd, COLORREF cr,BYTE bAlpha, DWORD dwFlags);


class C_BoggyWnd : public C_GenericWnd
{
	friend class C_BkgWnd;
public:
	C_BoggyWnd( C_Wnd * pParentWnd );
	~C_BoggyWnd();

    LRESULT WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam );
	LRESULT OnPaint(WPARAM,LPARAM);
	LRESULT OnActivate(WPARAM,LPARAM);
	LRESULT OnSize(WPARAM,LPARAM);
	LRESULT OnMove(WPARAM,LPARAM);
	LRESULT OnNCCalcSize(WPARAM,LPARAM);
	LRESULT OnTimer(WPARAM,LPARAM);
	LRESULT OnEraseBkgnd(WPARAM,LPARAM);
	LRESULT OnLButtonDown(WPARAM,LPARAM);
	LRESULT OnMouseMove(WPARAM,LPARAM);
	LRESULT OnLButtonUp(WPARAM,LPARAM);
	LRESULT OnLButtonDblClk(WPARAM,LPARAM);
	LRESULT OnRButtonDblClk(WPARAM,LPARAM);
	LRESULT OnRButtonUp(WPARAM,LPARAM);
	LRESULT OnDestroy(WPARAM,LPARAM);

	void CreateBoggySaveBitmap(int Mode);
	void DeleteBoggySaveBitmap();

	void MoveBoggyWindow(int left,int top);
	void BoggyTransparentBlt (HDC hdc,
					 int xStart, int yStart,
					 int xWidth, int yHeight,
					 HDC hdcSrc,
					 int xPos, int yPos);
	void Still();

protected:
	int m_bFirstActivated;
	int m_bDestroy;
	WORD m_nPosX,m_nPosY;
private:

	int m_nWidth,m_nHeight;
	int m_nScreenSize;
	BOOL m_bLPressed;
	BOOL m_bMoveWithNoBitCopy;

	lpfnSetLayeredWindowAttributes m_pSetLayeredWindowAttributes;

	C_BoggyAI * m_pBoggyAI;
	C_BoggyAction *m_pCurAction;
	C_BoggyAction *m_pAction[10];
};


#endif
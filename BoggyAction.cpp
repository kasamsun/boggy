#include "BoggyAction.h"

#include "BoggyApp.h"
#include "BoggyWnd.h"


C_BoggyAction::C_BoggyAction()
{
}

C_BoggyAction::C_BoggyAction(int totalframe,int delay,int movestep,int ID,int nMode)
{
	m_nTotalFrame = totalframe;
	m_nDelay = delay;
	m_nMoveStep = movestep;
	m_nResID = ID;
	CreateBoggyBitmap(nMode);
}

C_BoggyAction::~C_BoggyAction()
{
	DeleteBoggyBitmap();
}

void C_BoggyAction::CreateBoggyBitmap(int nMode)
{
	HDC hdctmp = CreateCompatibleDC(NULL);
	HBITMAP hbmptmp = LoadBitmap(C_App::m_hInstance,MAKEINTRESOURCE(m_nResID));
	HBITMAP hbmptmpold = (HBITMAP)SelectObject(hdctmp,hbmptmp);

	BITMAP bm;
	GetObject(hbmptmp,sizeof(BITMAP),&bm);
	int nImgWd = bm.bmWidth*nMode;
	int nImgHg = bm.bmHeight*nMode;

	m_hdcImg = CreateCompatibleDC(NULL);
	m_hbmpImg = CreateCompatibleBitmap(hdctmp,nImgWd,nImgHg);
	m_hbmpImgOld = (HBITMAP) SelectObject(m_hdcImg,m_hbmpImg);

	// load and stretch from source picture
	StretchBlt(m_hdcImg,0,0,nImgWd,nImgHg,
				hdctmp,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);

	m_hdcImgMsk = CreateCompatibleDC(NULL);
	m_hbmpImgMsk = CreateBitmap(nImgWd,nImgHg, 1, 1, NULL);
	m_hbmpImgMskOld = (HBITMAP) SelectObject(m_hdcImgMsk,m_hbmpImgMsk);

	COLORREF cColor = SetBkColor(m_hdcImg, TRANSPARENT_COLOR);

	// create mask picture
	BitBlt(m_hdcImgMsk,0,0,nImgWd,nImgHg,m_hdcImg,0,0,SRCCOPY);
	SetBkColor(m_hdcImg, cColor);

	HDC hdctmp2;
	HBITMAP hbmptmp2,hbmptmpold2;

	hdctmp2 = CreateCompatibleDC(NULL);
	hbmptmp2 = CreateBitmap(nImgWd,nImgHg,1,1,NULL);
	hbmptmpold2 = (HBITMAP) SelectObject(hdctmp2,hbmptmp2);

	BitBlt(hdctmp2,0,0,nImgWd,nImgHg,m_hdcImgMsk,0,0,NOTSRCCOPY);
	// create mask around org picture with ~of mask picture
	BitBlt(m_hdcImg,0,0,nImgWd,nImgHg,hdctmp2,0,0,SRCAND);

	DeleteObject(SelectObject(hdctmp,hbmptmpold));
	DeleteDC(hdctmp);
	DeleteObject(SelectObject(hdctmp2,hbmptmpold2));
	DeleteDC(hdctmp2);
	/*/
	hdcImg = CreateCompatibleDC(NULL);
	hbmpImg = LoadBitmap(C_Main::hInstance,MAKEINTRESOURCE(nResID));
	SelectObject(hdcImg,hbmpImg);
	*/
}

void C_BoggyAction::DeleteBoggyBitmap()
{
	DeleteObject(SelectObject(m_hdcImg,m_hbmpImgOld));
	DeleteObject(SelectObject(m_hdcImgMsk,m_hbmpImgMskOld));
	DeleteDC(m_hdcImg);
	DeleteDC(m_hdcImgMsk);
}

int C_BoggyAction::IsFinish()
{
	if ( m_nCurFrame > m_nTotalFrame && m_nDelayCounter == 1 )
		return TRUE;
	else
		return FALSE;
}

void C_BoggyAction::StartAction(int nDirection)
{
	m_nCurFrame = 1;
	m_nDelayCounter = 1;
	m_nDirection = nDirection;
}

int C_BoggyAction::DoAction()
{
	if ( m_nDelay == 1 )
	{
		if ( m_nMoveStep )
			return MOVE;
		else return STILL;
	}
	else if ( m_nDelay > 1 )
	{
		if ( m_nDelayCounter == 1 )
			return STILL;
		else return NOTHING;
	}
	return STILL;
}

void C_BoggyAction::ResumeAction()
{
	m_nDelayCounter++;
	if ( m_nDelayCounter > m_nDelay )
	{
		m_nDelayCounter = 1;
		m_nCurFrame++;
	}
}

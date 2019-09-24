#ifndef _C_BOGGYACTION_H_
#define _C_BOGGYACTION_H_

#include <windows.h>

class C_BoggyAction
{
#ifdef _DEBUG
public:  // public to show value in BkgWnd
#else
private:
#endif
	int m_nResID;
	int m_nTotalFrame;
	int m_nDelay;
	int m_nDelayCounter;

public:
	int m_nDirection;
	int m_nCurFrame;
	int m_nMoveStep;

	C_BoggyAction();
	C_BoggyAction(	int totalframe,	// total frame
					int delay,		// delay in times of 50 ms
					int movestep,	// window mode in pixel
									// * limited in H or V direction only
					int ID,			// image resource ID
					int Mode);		// size of image
	~C_BoggyAction();

	void CreateBoggyBitmap(int mode);
	void DeleteBoggyBitmap();

	int IsFinish();
	void StartAction(int d);
	int DoAction();
	void ResumeAction();

public:
	HDC m_hdcImg;
	HBITMAP m_hbmpImg,m_hbmpImgOld;
	HDC m_hdcImgMsk;
	HBITMAP m_hbmpImgMsk,m_hbmpImgMskOld;
};


#endif
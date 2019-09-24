#ifndef _C_COOLDLG_H_
#define _C_COOLDLG_H_

#include <windows.h>
#include <MyTempl.h>
#include <Control.h>
#include <Dialog.h>

typedef MyList<C_Control> MYCTL_LIST;

class C_CoolDlg : public C_Dialog
{
public:
	C_CoolDlg();
	~C_CoolDlg();

	int CoolDlgAutoSubclass();
	int CoolDlgSubclassDialog();
	int CoolDlgSubclassControl(HWND hDlg);

	void InitializeCoolDlg();
	void Paint(HDC);
	void UninitializeCoolDlg();

// ====== test =============
	void SetCoolDlgRgn();
	void DrawCoolDlgFrame(HWND hWnd,HDC hDC);
	void DrawCoolDlgCaption(HWND hWnd,HDC hDC);
// =========================

	LRESULT WndProc(UINT iMessage,WPARAM wParam,LPARAM lParam);

	static int WINAPI CoolDlgSetImage(HINSTANCE hInstance,LPCTSTR lpszName);
	static int WINAPI CoolDlgClearImage();
	static void WINAPI CoolDlgEnableFading(int bFlag=TRUE);
	static int WINAPI CoolDlgIsUsingFading() { return m_bFading; }
private:
	static HDC m_hdcCoolDlgImage;
	static HBITMAP m_hbmpCoolDlgImage;
	static HBITMAP m_hbmpTmp;
	static int m_bFading;

	MYCTL_LIST m_CtlList;
	DLGPROC m_lpfnDefCoolDlgProc;
	
	HDC m_hdcCoolBkMem;
	HBITMAP m_hbmpCoolBkMem,m_hbmpCoolBkTmp;
};

int WINAPI CoolDlgAutoSubclass();
int WINAPI CoolDlgTerminate();
LRESULT WINAPI CoolDlgHook(int nCode,WPARAM wParam,LPARAM lParam);
LRESULT CoolDlgProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam);

#endif

#include "PreferenceWnd.h"

BEGIN_HANDLE_MSG(C_PreferenceWnd,C_BitmapRgnWnd)


END_HANDLE_MSG()

C_PreferenceWnd::C_PreferenceWnd( HWND hwndParent,int nResID,int nMskResID )
		:C_BitmapRgnWnd(hwndParent,nResID,nMskResID)
{
	// enter modal loop
	RunModalLoop();
}

C_PreferenceWnd::~C_PreferenceWnd()
{
}



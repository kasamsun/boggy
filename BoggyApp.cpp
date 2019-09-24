#include <MainAfx.h>
#include <GenericWnd.h>

#include "BkgWnd.h"
#include "resource.h"

#ifndef _MSC_VER
#include <vcl/vcl.h>
USEUNIT("BitmapRgnWnd.cpp");
USEUNIT("BkgWnd.cpp");
USEUNIT("BoggyAction.cpp");
USEUNIT("BoggyAI.cpp");
USEUNIT("PreferenceWnd.cpp");
USEUNIT("..\kswafx\src\MainAfx.cpp");
USERC("boggy.rc");
USEUNIT("..\kswafx\src\GenericWnd.cpp");
USEUNIT("BoggyWnd.cpp");
//---------------------------------------------------------------------------
#endif

int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine,
                    int nCmdShow )
{
    C_App::m_hInstance = hInstance;
    C_App::m_hPrevInstance = hPrevInstance;
	C_App::m_lpszCmdLine = lpszCmdLine;
    C_App::m_nCmdShow = nCmdShow;

    if ( ! C_App::m_hPrevInstance )
	{
		// created common class here
		C_GenericWnd::Register( CS_DBLCLKS,
                        LoadIcon(C_App::m_hInstance,MAKEINTRESOURCE(IDI_BOGGY)),
                        LoadCursor(C_App::m_hInstance,MAKEINTRESOURCE(IDC_BOGGY)),
                        (HBRUSH)GetStockObject(WHITE_BRUSH),
                        NULL,
                        "Boggy" );
    }

    C_BkgWnd BkgWnd;	// make hidden window

    return C_App::MessageLoop();
}


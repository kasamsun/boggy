#ifndef _C_MYWINSOCK_H_
#define _C_MYWINSOCK_H_

#ifdef _MSC_VER
#include <winsock.h>
#endif
#include <GenericWnd.h>
#include <MyTempl.h>

#define		WM_WSA_ACTIVITY			WM_USER+700
#define		WM_WSA_KILLSOCKET		WM_USER+701

class C_MyWinsock : public C_GenericWnd
{
public:
	C_MyWinsock();
	~C_MyWinsock();

	BOOL Create(LPCTSTR lpszCaption,
						DWORD dwExStyle,
						DWORD dwStyle,
						const RECT& rect,
						C_Wnd* pParentWnd,
                        C_MyWinsock *pParentSock=NULL);
	BOOL Initialize();
	BOOL CloseSocket();
	BOOL CloseAllSocket();

	SOCKET GetSocketFD();
	// === server creating function ===
	BOOL CreateServerSocket(unsigned short nPortNum);

	// === client creating function ===
	BOOL CreateClientSocket(unsigned short nPortNum,char *szAddr);

    // transmit msg function
	int SocketSend(LPSTR lpszBuf, int nLen);
	int SocketRecv(LPSTR lpszBuf, int nLen);

    // service function
	BOOL GetPeerName(char *pszPeerAddr, UINT *pPeerPort);
	BOOL GetSockName(char *pszSockAddr, UINT *pSockPort);
	BOOL GetMyHostAddr(char *pszLocalHostAddr, UINT *nAddrCount);

    // event handling function
	LRESULT WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam );
	LRESULT OnWSAActivity(WPARAM wParam,LPARAM lParam);
	LRESULT OnWSAKillSocket(WPARAM wParam,LPARAM lParam);

    MyList<C_MyWinsock> * GetClientList() { return &m_pClientList; }

protected:
	BOOL AcceptSocket(C_MyWinsock *pAcceptSock);

	SOCKET m_fdSock;

	C_MyWinsock *m_pParentSock;
	MyList<C_MyWinsock> m_pClientList;

	virtual void OnAccept(int nErrCode) = 0;
	virtual void OnConnect(int nErrCode) = 0;
	virtual void OnRecv(int nErrCode) = 0;
	virtual void OnSend(int nErrCode) = 0;
	virtual void OnClose(int nErrCode) = 0;
	virtual void OnOOB(int nErrCode) = 0;

private:
	static int m_nRefCount;
	// === server function ===
	// call after accept , bind wnd to socket
	BOOL CreateConnectedSocket(SOCKET sfd);
};

#endif

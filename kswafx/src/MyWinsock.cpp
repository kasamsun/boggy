#include <MyWinsock.h>

int C_MyWinsock::m_nRefCount = 0;

BEGIN_HANDLE_MSG(C_MyWinsock,C_GenericWnd)
	HANDLE_MSG(WM_WSA_ACTIVITY,OnWSAActivity)
	HANDLE_MSG(WM_WSA_KILLSOCKET,OnWSAKillSocket)
END_HANDLE_MSG()

C_MyWinsock::C_MyWinsock()
{
	m_pClientList.DeleteObjects();
	m_fdSock = INVALID_SOCKET;
    m_pParentWnd = NULL;
    m_hWnd = NULL;

	if ( !Initialize() ) exit (FALSE);
}

C_MyWinsock::~C_MyWinsock()
{
	if ( m_fdSock != INVALID_SOCKET ) CloseSocket();
	if ( IsWindow(m_hWnd) ) DestroyWindow(m_hWnd);
	m_pClientList.DeleteObjects();

	// ============ Check reference count
	// Only last instance call WSACleanup()
	m_nRefCount--;
	if ( m_nRefCount <= 0 ) WSACleanup();
}

BOOL C_MyWinsock::Create(LPCTSTR lpszCaption,
						DWORD dwExStyle,
						DWORD dwStyle,
						const RECT& rect,
						C_Wnd* pParentWnd,
                        C_MyWinsock *pParentSock)
{
	m_hWnd = CreateWindowEx(  dwExStyle,
    						m_szClassName,
                            lpszCaption,
                            dwStyle,
                            rect.left,
                            rect.top,
                            rect.right-rect.left,
                            rect.bottom-rect.top,
                            pParentWnd->GetHandle(),
                            NULL,
                            C_App::m_hInstance,
                            (LPSTR) this );
	 if ( ! m_hWnd )
		  exit(FALSE);
	m_pParentWnd = pParentWnd;
    m_pParentSock = pParentSock;

	return TRUE;
}

BOOL C_MyWinsock::Initialize()
{
	// check ref count here
	m_nRefCount++;
	if ( m_nRefCount > 1 ) return TRUE;

	///////////////////////////////
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
    // requested version is 1.1
	wVersionRequested = MAKEWORD(1,1);

	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)
	{
		MessageBox(GetActiveWindow(),"Couldn't find a useable winsock dll","Winsock",MB_OK|MB_ICONSTOP);
		return FALSE;
	}

	/* Confirm that the Windows Sockets DLL supports 1.1.*/
	/* Note that if the DLL supports versions greater */
	/* than 1.1 in addition to 1.1, it will still return */
	/* 1.1 in wVersion since that is the version we */
	/* requested. */
	if ( LOBYTE( wsaData.wVersion ) != 1 ||
			HIBYTE( wsaData.wVersion ) != 1 )
	{
		WSACleanup();
		MessageBox(GetActiveWindow(),"winsock version is not v1.1","Winsock",MB_OK|MB_ICONSTOP);
		return FALSE;
	}
//	TRACE("WSAStartup OK\n");
//	TRACE("wVersion %0x\n",wsaData.wVersion);
//	TRACE("wHighVersion %0x\n",wsaData.wHighVersion);
//	TRACE("Description [%s]\n",wsaData.szDescription);
//	TRACE("SystemStatus [%s]\n",wsaData.szSystemStatus);
//	TRACE("MaxSocket %d\n",wsaData.iMaxSockets);
//	TRACE("MaxUdpDg %d\n",wsaData.iMaxUdpDg);
//	TRACE("VendorInfo [%s]\n",wsaData.lpVendorInfo);

	return TRUE;
}

SOCKET C_MyWinsock::GetSocketFD()
{
	return m_fdSock;
}

BOOL C_MyWinsock::CreateServerSocket(unsigned short nPortNum)
{
	if ( m_fdSock != INVALID_SOCKET ) return FALSE;
    if ( m_hWnd == NULL ) return FALSE;


	if ( (m_fdSock = socket(PF_INET,SOCK_STREAM,0)) == INVALID_SOCKET )
	{
//		TRACE("# socket error\n");
		return FALSE;
	}
	SOCKADDR_IN sin;
	u_short alport = nPortNum;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = 0;
	sin.sin_port = htons(alport);
	if (bind(m_fdSock, (LPSOCKADDR)&sin, sizeof (sin)) == SOCKET_ERROR )
	{
//		TRACE("# bind error\n");
		CloseSocket();
		return FALSE;
	}
	// maximun queue may grow = 5 (default)
	if ( listen(m_fdSock,SOMAXCONN) == SOCKET_ERROR )
	{
//		TRACE("# listen error\n");
		CloseSocket();
		return FALSE;
	}

	// accept 'll be called on hNotifyWnd
	if ( WSAAsyncSelect(m_fdSock,m_hWnd,WM_WSA_ACTIVITY ,FD_OOB|FD_READ|FD_WRITE|FD_CONNECT|FD_ACCEPT|FD_CLOSE) == SOCKET_ERROR )
	{
		CloseSocket();
		return FALSE;
	}

	TRACE("server socket running\n");
	return TRUE;
}

BOOL C_MyWinsock::AcceptSocket(C_MyWinsock *pAcceptSock)
{
	if ( m_fdSock == INVALID_SOCKET ) return FALSE;

	SOCKET sfdIncomingSock;

	if ( (sfdIncomingSock = accept(m_fdSock,NULL,NULL)) == INVALID_SOCKET )
	{
//		TRACE("accept fail\n");
		closesocket(sfdIncomingSock);
		return FALSE;
	}

	if ( !pAcceptSock->CreateConnectedSocket(sfdIncomingSock) )
	{
		closesocket(pAcceptSock->GetSocketFD());
		return FALSE;
	}
    m_pClientList.Add(pAcceptSock);
	return TRUE;
}

int C_MyWinsock::CreateConnectedSocket(SOCKET sfd)
{
	m_fdSock = sfd;
	if ( WSAAsyncSelect(sfd,m_hWnd,WM_WSA_ACTIVITY ,FD_OOB|FD_READ|FD_WRITE|FD_CONNECT|FD_ACCEPT|FD_CLOSE) == SOCKET_ERROR )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL C_MyWinsock::CreateClientSocket(unsigned short nPortNum,char *szAddr)
{
	if ( m_fdSock != INVALID_SOCKET ) return FALSE;
    if ( m_hWnd == NULL ) return FALSE;

	if ( (m_fdSock = socket(PF_INET,SOCK_STREAM,0)) == INVALID_SOCKET )
	{
//		TRACE("# socket error\n");
		return FALSE;
	}

	SOCKADDR_IN	sin;
	u_short alport = nPortNum;
	sin.sin_port = htons(alport);
	sin.sin_family = AF_INET;
	memset(sin.sin_zero,0x00,sizeof(sin.sin_zero));
	memset(&sin.sin_addr,0x00,sizeof(sin.sin_addr));
	if ( inet_addr(szAddr) == INADDR_NONE )
	{
		// now don't try to resolve the name
		// only available on IP address
		CloseSocket();
		return FALSE;
		// address error try to resolve the name
		/*
		char lpszDotAddr[64];
		memset(lpszDotAddr,0x00,sizeof(lpszDotAddr));

		hostent *Host = gethostbyname(szAddr);
		if ( Host == NULL )
			return FALSE;
		else
		{
			struct in_addr addr;
			memcpy(&addr, Host->h_addr_list[0], sizeof(struct in_addr));
			strcpy(lpszDotAddr,inet_ntoa(addr));
		}
		sin.sin_addr.S_un.S_addr=inet_addr(lpszDotAddr);
		*/
	}
	else
	{
		sin.sin_addr.S_un.S_addr=inet_addr(szAddr);
	}

	if ( WSAAsyncSelect(m_fdSock,m_hWnd,WM_WSA_ACTIVITY ,FD_OOB|FD_READ|FD_WRITE|FD_CONNECT|FD_ACCEPT|FD_CLOSE) == SOCKET_ERROR)
	{
//		TRACE("# asyncselect fail\n");
		CloseSocket();
		return FALSE;
	}

	connect(m_fdSock,(const struct sockaddr *)&sin ,sizeof(struct sockaddr_in));

	return TRUE;
}


BOOL C_MyWinsock::CloseSocket()
{
	if ( closesocket(m_fdSock) == SOCKET_ERROR )
		return FALSE;
	m_fdSock = INVALID_SOCKET;
	return TRUE;
}

BOOL C_MyWinsock::CloseAllSocket()
{
	m_pClientList.DeleteObjects();
	return CloseSocket();
}

int C_MyWinsock::SocketSend(LPSTR lpszBuf, int nLen)
{
	if ( m_fdSock == INVALID_SOCKET ) return FALSE;

	int nsend;

	nsend = send(m_fdSock,lpszBuf,nLen,0);
	if ( nsend == SOCKET_ERROR || nsend < nLen )
		return FALSE;
	else
		return nsend;
}

int C_MyWinsock::SocketRecv(LPSTR lpszBuf, int nLen)
{
	if ( m_fdSock == INVALID_SOCKET ) return FALSE;

	int	nrecv;

	nrecv = recv(m_fdSock,lpszBuf,nLen,0);
	if ( nrecv == SOCKET_ERROR || nrecv == 0)
	{
		return FALSE;
	}
	else
	{
		return nrecv;
	}
}

//   Note : pszPeerAddr should be at least 16 bytes in size
BOOL C_MyWinsock::GetPeerName(char *pszPeerAddr, UINT *pPeerPort)
{
	SOCKADDR_IN s;
	int nLen = sizeof(s);

	memset(&s,0x00, sizeof(s));
	*pPeerPort = 6400;

	if ( getpeername(m_fdSock,(SOCKADDR*)&s, &nLen) == SOCKET_ERROR )
		return FALSE;

	*pPeerPort = ntohs(s.sin_port);
	strcpy(pszPeerAddr,inet_ntoa(s.sin_addr));
	return TRUE;
}

//   Note : pszSockAddr should be at least 16 bytes in size
//   Return name of port currently connected
BOOL C_MyWinsock::GetSockName(char *pszSockAddr, UINT *pSockPort)
{
	SOCKADDR_IN s;
	int nLen = sizeof(s);

	memset(&s,0x00, sizeof(s));
	*pSockPort = 0;

	if ( getsockname(m_fdSock,(SOCKADDR*)&s, &nLen) == SOCKET_ERROR )
		return FALSE;

	*pSockPort = ntohs(s.sin_port);
	strcpy(pszSockAddr,inet_ntoa(s.sin_addr));
	return TRUE;
}

//   Note : pszSockAddr should be at least 64 bytes in size (dot notation)
//	 return : all IP address , 
//			nAddrCount show number of IP specified to this machine
BOOL C_MyWinsock::GetMyHostAddr(char *pszLocalHostAddr,UINT *nAddrCount)
{
	char szHost[64];

	if ( gethostname(szHost,sizeof(szHost)) == SOCKET_ERROR )
		return FALSE;

	hostent *Host;
	int j = 0;

	if ( (Host = gethostbyname(szHost)) == NULL )
		return FALSE;
	else
	{
		int i;
		for ( i = 0; Host->h_addr_list[i] != 0; i++ )
		{
			struct in_addr addr;
			memcpy(&addr, Host->h_addr_list[i], sizeof(struct in_addr));
			strcpy(pszLocalHostAddr+j,inet_ntoa(addr));
			j += strlen(inet_ntoa(addr))+1;
		}    
		*nAddrCount = i;
	}
	return TRUE;
}

LRESULT C_MyWinsock::OnWSAActivity(WPARAM ,LPARAM lParam)
{
	int nErrCode = WSAGETSELECTERROR(lParam);
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
		this->OnRecv(nErrCode);
		break;
	case FD_WRITE:
		this->OnSend(nErrCode);
		break;
	case FD_OOB:
		this->OnOOB(nErrCode);
		break;
	case FD_ACCEPT:
		this->OnAccept(nErrCode);
		break;
	case FD_CONNECT:
		this->OnConnect(nErrCode);
		break;
	case FD_CLOSE:
		this->OnClose(nErrCode);
		// child notify parent to kill child socket
		if ( m_pParentSock != NULL )
			PostMessage(m_pParentSock->GetHandle(),
						WM_WSA_KILLSOCKET,0,(long)this);
		break;
	}
	return 0;
}

LRESULT C_MyWinsock::OnWSAKillSocket(WPARAM ,LPARAM lParam)
{
	for ( int i=0 ; i<m_pClientList.Count() ; i++ )
    {
		if ( (C_MyWinsock*)lParam == m_pClientList.Items(i) )
        {
			delete (C_MyWinsock*)lParam;
            m_pClientList.Remove(i);
        }
    }
	return 0;
}
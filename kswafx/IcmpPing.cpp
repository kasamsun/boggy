#include <IcmpPing.h>

C_IcmpPing::C_IcmpPing()
{
	m_hIcmpLib = LoadLibrary("ICMP.DLL");
    m_lpfnCreateFile = (LPFNICMPCREATEFILE)GetProcAddress(m_hIcmpLib,"IcmpCreateFile");
    m_lpfnCloseHandle = (LPFNICMPCLOSEHANDLE)GetProcAddress(m_hIcmpLib,"IcmpCloseHandle");
    m_lpfnSendEcho = (LPFNICMPSENDECHO)GetProcAddress(m_hIcmpLib,"IcmpSendEcho");

	if ((m_lpfnCreateFile == NULL) || (m_lpfnCloseHandle == NULL) ||
    	(m_lpfnSendEcho == NULL))
    {
    	m_lpfnCreateFile = NULL;
        m_lpfnCloseHandle = NULL;
        m_lpfnSendEcho = NULL;
		if ( m_hIcmpLib != NULL ) FreeLibrary(m_hIcmpLib);
    }
}

C_IcmpPing::~C_IcmpPing()
{
	if ( m_hIcmpLib != NULL ) FreeLibrary(m_hIcmpLib);
}

void C_IcmpPing::Ping(DWORD dwAddr,int nTimeOut)
{
	m_dwAddr = dwAddr;
    m_nTimeOut = nTimeOut;

    Create(NULL);
}

DWORD C_IcmpPing::MyThreadFunction()
{
	if ( m_hIcmpLib == NULL )
    {
    	this->OnResponse(ICMP_NOTINITIALIZED,NULL);
    	return 0;
    }

    // Open the ping service
    HANDLE hIP = m_lpfnCreateFile();

    if (hIP == INVALID_HANDLE_VALUE)
    {
    	// Unable to open ping service
        this->OnResponse(ICMP_OPENSERVICEFAIL,NULL);
        return 0;
    }

    // Build ping packet
    char szPingBuffer[64];
    memset(szPingBuffer, '\x00', sizeof(szPingBuffer));
    LPIP_ECHO_REPLY pIpe = (LPIP_ECHO_REPLY) new char[sizeof(IP_ECHO_REPLY) + sizeof(szPingBuffer)];
    if (pIpe == 0)
    {
        // Failed to allocate ping packet buffer
        this->OnResponse(ICMP_ALLOCATEBUFFERFAIL,NULL);
	    m_lpfnCloseHandle(hIP);
        return 0;
    }

    pIpe->Data = szPingBuffer;
    pIpe->DataSize = sizeof(szPingBuffer);
    // Send the ping packet
    DWORD dwStatus = m_lpfnSendEcho(hIP, m_dwAddr,
            szPingBuffer, sizeof(szPingBuffer), NULL, pIpe,
            sizeof(IP_ECHO_REPLY) + sizeof(szPingBuffer), m_nTimeOut);
    if (dwStatus != 0)
    {
    	this->OnResponse(0,pIpe);
    }
    else
    {
        // Error obtaining info from ping packet
        this->OnResponse(ICMP_PINGTIMEOUT,NULL);
    }

    // Shut down...
    m_lpfnCloseHandle(hIP);
	delete[] (char*)pIpe;
	return 0;
}

/*
// output in case of complete
				Address x.x.x.x
                int(LOBYTE(LOWORD(pIpe->Address)))
                int(HIBYTE(LOWORD(pIpe->Address)))
                int(LOBYTE(HIWORD(pIpe->Address)))
                int(HIBYTE(HIWORD(pIpe->Address)))

                RTT xx ms.
                int(pIpe->RoundTripTime)
                TTL
                int(pIpe->Options.Ttl)
*/

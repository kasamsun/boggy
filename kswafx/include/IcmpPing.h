#ifndef _C_ICMPPING_H_
#define _C_ICMPPING_H_

#include <MyThread.h>

// Structures required to use functions in ICMP.DLL
typedef struct _IP_OPTION_INFORMATION
{
   unsigned char Ttl;                                           // Time To Live
   unsigned char Tos;                                        // Type Of Service
   unsigned char Flags;                                      // IP header flags
   unsigned char OptionsSize;                  // Size in bytes of options data
   unsigned char *OptionsData;                       // Pointer to options data
} IP_OPTION_INFORMATION, * LPIP_OPTION_INFORMATION;

typedef struct _IP_ECHO_REPLY
{
   DWORD Address;                                           // Replying address
   unsigned long  Status;                                       // Reply status
   unsigned long  RoundTripTime;                         // RTT in milliseconds
   unsigned short DataSize;                                   // Echo data size
   unsigned short Reserved;                          // Reserved for system use
   void *Data;                                      // Pointer to the echo data
   IP_OPTION_INFORMATION Options;                              // Reply options
} IP_ECHO_REPLY, * LPIP_ECHO_REPLY;

// Return Code
enum { ICMP_PINGOK,
		ICMP_NOTINITIALIZED,
        ICMP_OPENSERVICEFAIL,
        ICMP_ALLOCATEBUFFERFAIL,
        ICMP_PINGTIMEOUT };

typedef HANDLE (WINAPI* LPFNICMPCREATEFILE)(VOID);
typedef BOOL (WINAPI* LPFNICMPCLOSEHANDLE)(HANDLE);
typedef DWORD (WINAPI* LPFNICMPSENDECHO)(HANDLE, DWORD, LPVOID, WORD,
        LPIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD);

class C_IcmpPing : public C_MyThread
{
public:
	C_IcmpPing();
    virtual ~C_IcmpPing();

    void Ping(DWORD dwAddr,int nTimeOut);

protected:
	virtual DWORD MyThreadFunction();

    HINSTANCE m_hIcmpLib;

    LPFNICMPCREATEFILE m_lpfnCreateFile;
    LPFNICMPCLOSEHANDLE m_lpfnCloseHandle;
    LPFNICMPSENDECHO m_lpfnSendEcho;

    DWORD m_dwAddr;		// IP Address in network order format
    int m_nTimeOut;

	virtual void OnResponse(int nErrCode,LPIP_ECHO_REPLY pEchoReply)=0;

};

#endif
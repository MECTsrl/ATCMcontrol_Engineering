#include "stdafx.h"
#include <stdio.h>
#include <native.h>
#include "serialPeer.h"

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>


// Java headers
// disable some nasty warnings caused from generated header
#pragma warning (disable : 4510) // C4510: 'ClassArrayOfCOM_softing_fc_CC_io_SerialPeer' : default constructor could not be generated
#pragma warning (disable : 4512) // C4512: 'ClassArrayOfCOM_softing_fc_CC_io_SerialPeer' : assignment operator could not be generated
#pragma warning (disable : 4610) // C4610: struct 'ClassArrayOfCOM_softing_fc_CC_io_SerialPeer' can never be instantiated - user defined constructor required
#include "COM_softing_fc_CC_io_SerialPeer.h"
#include "COM_softing_fc_CC_io_IODatagram.h"
#pragma warning (default : 4510)
#pragma warning (default : 4512)
#pragma warning (default : 4610)

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
// #define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//----  Global variables:         ----------------------------------*
HANDLE      hmodDLL;        // handle of this DLL instance
int         udpInit = -1;

/////////////////////////////////////////////////////////////////////
//------------- DLL support functions
/////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * DLL entry point
 *
 * @param           hInst
 * @param           dwReason
 * @param           lpReserved
 * @return          != 0 if ok
 * @exception       -
*/
BOOL APIENTRY
    DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID /*lpReserved*/)
{
    switch (ul_reason_for_call)
	{
    case DLL_PROCESS_ATTACH:
        hmodDLL = hModule;
//        AfxSetResourceHandle((HINSTANCE)hModule);
        DisableThreadLibraryCalls((HMODULE)hModule);
        udpInit = 0;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * version number of RNI
 *
 * @param           void
 * @return          version of this RNI impl.
 * @exception       -
*/
DWORD SERIALDLL_API __cdecl RNIGetCompatibleVersion()
{
    return RNIVER;
}


BOOL SerialPeerClearCommError(struct HCOM_softing_fc_CC_io_SerialPeer *me, 
                              LPDWORD lpErrors, LPCOMSTAT lpStat)
{
    DWORD commError;

    if (!ClearCommError((HANDLE)me->m_comHandle, &commError, lpStat))
        return FALSE;

    if (lpErrors)
        *lpErrors = commError;

    // update m_commErrRead and m_commErrWrite
    // write errors
    if (commError & CE_TXFULL)
    {
        commError &= (~CE_TXFULL);
        me->m_commErrWrite |= CE_TXFULL;
    }

    // read errors
    if (commError & CE_RXOVER)
    {
        commError &= (~CE_RXOVER);
        me->m_commErrRead |= CE_RXOVER;
    }
    if (commError & CE_OVERRUN)
    {
        commError &= (~CE_OVERRUN);
        me->m_commErrRead |= CE_OVERRUN;
    }
    if (commError & CE_RXPARITY)
    {
        commError &= (~CE_RXPARITY);
        me->m_commErrRead |= CE_RXPARITY;
    }
    if (commError & CE_FRAME)
    {
        commError &= (~CE_FRAME);
        me->m_commErrRead |= CE_FRAME;
    }
    if (commError & CE_BREAK)
    {
        commError &= (~CE_BREAK);
        me->m_commErrRead |= CE_BREAK;
    }

    // unspecified read and write errors
    me->m_commErrRead |= commError;
    me->m_commErrWrite |= commError;

    return TRUE;
}

void SerialPeerSetStatusRead(struct HCOM_softing_fc_CC_io_SerialPeer *me)
{
    DWORD comError = me->m_commErrRead;
    if (comError & CE_RXOVER)
        me->m_statusRead = -CE_RXOVER;
    else if (comError & CE_OVERRUN)
        me->m_statusRead = -CE_OVERRUN;
    else if (comError & CE_RXPARITY)
        me->m_statusRead = -CE_RXPARITY;
    else if (comError & CE_FRAME)
        me->m_statusRead = -CE_FRAME;
    else if (comError & CE_BREAK)
        me->m_statusRead = -CE_BREAK;
    else if (comError != 0)
        me->m_statusRead = ERROR_READ_FAULT;
}

void SerialPeerSetStatusWrite(struct HCOM_softing_fc_CC_io_SerialPeer *me)
{
    DWORD comError = me->m_commErrWrite;
    if (comError & CE_TXFULL)
        me->m_statusWrite = -CE_TXFULL;
    else  if (comError != 0)
        me->m_statusWrite = ERROR_WRITE_FAULT;
}

__declspec(dllexport) void __cdecl 
    COM_softing_fc_CC_io_SerialPeer_initDll (struct HCOM_softing_fc_CC_io_SerialPeer *me)
{
    me->m_pAsyncReadBuf = NULL;
    me->m_pAsyncWriteBuf = NULL;
    me->m_pAsyncReadStruct = NULL;
    me->m_pAsyncWriteStruct = NULL;

    // create OVERLAPPED structure
    OVERLAPPED *pOverlapped;

    pOverlapped = new OVERLAPPED;
    if (pOverlapped == NULL)
    {
        me->m_statusConn = ERROR_NOT_ENOUGH_MEMORY;
        return;
    }
    pOverlapped->hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
    pOverlapped->Offset = 0;
    pOverlapped->OffsetHigh = 0;
    me->m_pAsyncReadStruct = (long)pOverlapped;

    pOverlapped = new OVERLAPPED;
    if (pOverlapped == NULL)
    {
        me->m_statusConn = ERROR_NOT_ENOUGH_MEMORY;
        return;
    }
    pOverlapped->hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
    pOverlapped->Offset = 0;
    pOverlapped->OffsetHigh = 0;
    me->m_pAsyncWriteStruct = (long)pOverlapped;

}


__declspec(dllexport) void __cdecl 
    COM_softing_fc_CC_io_SerialPeer_releaseDll (struct HCOM_softing_fc_CC_io_SerialPeer *me)
{
    OVERLAPPED *pOverlapped;

    // delete OVERLAPPED structure
    if (me->m_pAsyncReadStruct)
    {
        pOverlapped = (OVERLAPPED *)me->m_pAsyncReadStruct;
        CloseHandle(pOverlapped->hEvent);
        delete pOverlapped;
        me->m_pAsyncReadStruct = NULL;
    }
    if (me->m_pAsyncWriteStruct)
    {
        pOverlapped = (OVERLAPPED *)me->m_pAsyncWriteStruct;
        CloseHandle(pOverlapped->hEvent);
        delete pOverlapped;
        me->m_pAsyncWriteStruct = NULL;
    }
}


//------------------------------------------------------------------*
/**
 * openPort
 *
 * @param           me          this ptr of Java class
 * @return          HANDLE if port can be opened, 0 otherwise
 * @exception       -
*/
__declspec(dllexport) long __cdecl 
    COM_softing_fc_CC_io_SerialPeer_openPort (struct HCOM_softing_fc_CC_io_SerialPeer *me)
{
    // open port
    TCHAR   portName[32];
    wsprintf(portName, _T("\\\\.\\COM%ld"), me->m_portNb);//d-155: removed trailing ':' from file name.
    
    HANDLE hComPort;
    hComPort = CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 
                          FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

    if (hComPort == INVALID_HANDLE_VALUE)
    {
        int iError = GetLastError();
        if (iError == ERROR_FILE_NOT_FOUND || iError == ERROR_ACCESS_DENIED)
            me->m_statusConn = iError;
        else
            me->m_statusConn = ERROR_OPEN_FAILED;
        return 0;
    }

    // init m_statusConn
    me->m_statusConn = 0;

    // set com port settings...
    DCB myDCB;
    GetCommState(hComPort, &myDCB);     // fDummy2, wReserved, wReserved1

    myDCB.DCBlength = sizeof(DCB);
    myDCB.fBinary = 1;
    myDCB.fDsrSensitivity = 0;
    myDCB.fTXContinueOnXoff = 1;
    myDCB.fErrorChar = 0;               // enable error replacement
    myDCB.fNull = 0;                    // enable null stripping
    myDCB.fAbortOnError = 1;            // abort reads/writes on error
    //probably depends on hw-driver?
    //myDCB.XonLim = 2048;
    //myDCB.XoffLim = 512;
    myDCB.XonChar = 17;
    myDCB.XoffChar = 19;
    //myDCB.ErrorChar = ' ';              // not used (fErrorChar = 0)
    //myDCB.EofChar = 26;                  // not used (fBinary = 1)
    myDCB.EvtChar = 0;      

    myDCB.BaudRate = (unsigned long)me->m_baudrate; /* Baudrate at which running */
    myDCB.ByteSize = (unsigned char)me->m_databits;	/* number of bits/byte, 4-8 */
    myDCB.StopBits = (unsigned char)me->m_stopbits;	/* 0, 1, 2 = 1, 1.5, 2 */
    myDCB.Parity = (unsigned char)me->m_parity;     /* 0-4=no,odd,even,mark,space */

    if (myDCB.Parity == 0)
        myDCB.fParity = 0;
    else
        myDCB.fParity = 1;

    switch(me->m_protocol)
    {
    case 0: // NO
        myDCB.fOutxCtsFlow = 0;
        myDCB.fOutxDsrFlow = 0;
        myDCB.fOutX = 0;
        myDCB.fInX = 0;
        myDCB.fRtsControl = RTS_CONTROL_ENABLE;
        myDCB.fDtrControl = DTR_CONTROL_ENABLE;
        break;
    case 1: // SW
        myDCB.fOutxCtsFlow = 0;
        myDCB.fOutxDsrFlow = 0;
        myDCB.fOutX = 1;
        myDCB.fInX = 1;
        myDCB.fRtsControl = RTS_CONTROL_ENABLE;
        myDCB.fDtrControl = DTR_CONTROL_ENABLE;
        break;
    case 2: // HW
        myDCB.fOutxCtsFlow = 1;
        myDCB.fOutxDsrFlow = 1;
        myDCB.fOutX = 0;
        myDCB.fInX = 0;
        myDCB.fRtsControl = RTS_CONTROL_HANDSHAKE;
        myDCB.fDtrControl = DTR_CONTROL_HANDSHAKE;
        break;
    default: // => NO
        myDCB.fOutxCtsFlow = 0;
        myDCB.fOutxDsrFlow = 0;
        myDCB.fOutX = 0;
        myDCB.fInX = 0;
        myDCB.fRtsControl = RTS_CONTROL_ENABLE;
        myDCB.fDtrControl = DTR_CONTROL_ENABLE;
        break;
    }

    if (SetCommState(hComPort, &myDCB) == FALSE)
    {
        me->m_statusConn = ERROR_INVALID_PARAMETER;  
        CloseHandle(hComPort);
        return 0;
    }

    me->m_isOpen = TRUE;

    // set comm timeouts: timeouts are not used because they are handled in the CommFB status maschine!
    COMMTIMEOUTS commTimeouts;
    if (GetCommTimeouts(hComPort, &commTimeouts))
    {
        commTimeouts.ReadIntervalTimeout = 0;
        commTimeouts.ReadTotalTimeoutMultiplier = 0;
        commTimeouts.ReadTotalTimeoutConstant = 0;
        commTimeouts.WriteTotalTimeoutMultiplier = 0;
        commTimeouts.WriteTotalTimeoutConstant = 0;

        SetCommTimeouts(hComPort, &commTimeouts);
    }
    return ((long)hComPort);
}

//------------------------------------------------------------------*
/**
 * closePort
 *
 * @param           me          this ptr of Java class
 * @return          true if port is closed, false otherwise   
 * @exception       -
*/
__declspec(dllexport) /*boolean*/ long __cdecl 
    COM_softing_fc_CC_io_SerialPeer_closePort (struct HCOM_softing_fc_CC_io_SerialPeer *me)
{
    BOOL bRet = CloseHandle((HANDLE)me->m_comHandle);
    me->m_comHandle = NULL;
    me->m_isOpen = FALSE;

    if (bRet)
        me->m_statusConn = 0;
    else
        me->m_statusConn = ERROR_INVALID_HANDLE;

    return bRet;
}

//------------------------------------------------------------------*
/**
 * checkConn
 *
 * @param           me          this ptr of Java class
 * @param           chPortNr    COM port number; 0 = COM1, etc.
 * @return          true if port exists on system
 * @exception       -
*/
__declspec(dllexport) /*boolean*/ long __cdecl
    COM_softing_fc_CC_io_SerialPeer_checkConn (struct HCOM_softing_fc_CC_io_SerialPeer *me)
{
    if ((HANDLE)me->m_comHandle == INVALID_HANDLE_VALUE || me->m_comHandle == 0)
    {
        me->m_statusConn = ERROR_FILE_INVALID;
        return false;
    }

    return true;
}


//------------------------------------------------------------------*
/**
 * dataAvailable
 *
 * @param           me          this ptr of Java class
 * @return          number of bytes available to read
 * @exception       -
*/
__declspec(dllexport) long __cdecl 
    COM_softing_fc_CC_io_SerialPeer_dataAvailable (struct HCOM_softing_fc_CC_io_SerialPeer *me)
{
    COMSTAT comStat;
    int     iBytesInQue = 0;

    if (!me->m_isOpen)
    {
        me->m_statusRead = ERROR_DEVICE_NOT_CONNECTED;
        return 0;
    }

    // get status info from com port
    if (SerialPeerClearCommError(me, NULL, &comStat))
        iBytesInQue = comStat.cbInQue;

    // set m_statusRead
    SerialPeerSetStatusRead(me);

    return iBytesInQue;
}

//------------------------------------------------------------------*
/**
 * getDataAsync
 *
 * @param           me          this ptr of Java class
 * @return          true        if read request ist started
 * @exception       -
*/
__declspec(dllexport) /*boolean*/ long __cdecl 
    COM_softing_fc_CC_io_SerialPeer_getDataAsync (struct HCOM_softing_fc_CC_io_SerialPeer *me)
{
    DWORD nbBytesRead;
    int   error;

    if (!me->m_isOpen)
    {
        me->m_statusRead = ERROR_DEVICE_NOT_CONNECTED;
        return false;
    }

    if (me->m_pAsyncReadBuf)
    {
        me->m_statusRead = ERROR_READ_FAULT;
        return false;
    }

    // init m_statusRead
    me->m_statusRead = 0;

    OVERLAPPED *pOverlapped = (OVERLAPPED *)me->m_pAsyncReadStruct;
    //pOverlapped->hEvent = NULL;
    pOverlapped->Offset = 0;
    pOverlapped->OffsetHigh = 0;

    // create byte array
    BYTE *pDataBuf = new BYTE[me->m_readLen];
    me->m_pAsyncReadBuf = (long)pDataBuf;
    if (pDataBuf == NULL)
    {
        me->m_statusRead = ERROR_NOT_ENOUGH_MEMORY;
        return false;
    }

    if (!ReadFile((HANDLE)me->m_comHandle, pDataBuf, me->m_readLen, &nbBytesRead, pOverlapped))
    {
        error = GetLastError();
        if (error != ERROR_IO_PENDING)
        {
            // clear error
            if (!SerialPeerClearCommError(me, NULL, NULL))
                me->m_statusRead = ERROR_READ_FAULT;

            // set m_statusRead
            SerialPeerSetStatusRead(me);

            delete [] pDataBuf;
            me->m_pAsyncReadBuf = NULL;

            if (me->m_statusRead == 0)
                me->m_statusRead = ERROR_READ_FAULT;

            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------*
/**
 * hasReadCompleted
 *
 * @param           me          this ptr of Java class
 * @return          true        if no request is pending
 * @exception       -
*/
__declspec(dllexport) /*boolean*/ long __cdecl 
    COM_softing_fc_CC_io_SerialPeer_hasReadCompleted (struct HCOM_softing_fc_CC_io_SerialPeer *me)
{
    DWORD nbBytesTransferred = 0;
    BOOL bRet;

    if (!me->m_isOpen)
    {
        me->m_statusRead = ERROR_DEVICE_NOT_CONNECTED;
        return true;
    }

    if (!me->m_pAsyncReadBuf)
    {
        me->m_statusRead = ERROR_READ_FAULT;
        return true;
    }

    bRet = GetOverlappedResult((HANDLE)me->m_comHandle, (OVERLAPPED *)me->m_pAsyncReadStruct, 
                               &nbBytesTransferred, false /*bWait*/);
    if (!bRet)
    {
        int iError = GetLastError();
        if (iError == ERROR_IO_INCOMPLETE)
            return false;
        me->m_statusRead = ERROR_READ_FAULT;
    }

    // call SerialPeerClearCommError() in any case!
    // check if error has occured
    if (!SerialPeerClearCommError(me, NULL, NULL))
        me->m_statusRead = ERROR_READ_FAULT;

    // set m_statusRead
    SerialPeerSetStatusRead(me);

    // if no error has been occured: check if all bytes has been transferred
    if (me->m_statusRead == 0 && nbBytesTransferred < (DWORD)me->m_readLen)
    {
        me->m_statusRead = ERROR_READ_FAULT;
    }

    if (me->m_statusRead != 0)
    {
        // cancel read request
        bRet = PurgeComm((HANDLE)me->m_comHandle, PURGE_RXABORT|PURGE_RXCLEAR);
        //bRet = GetOverlappedResult((HANDLE)me->m_comHandle, (OVERLAPPED *)me->m_pAsyncReadStruct, 
        //    &nbBytesTransferred, true /*bWait*/);
    }
    else
    {
        GCFrame gcf;
        struct HCOM_softing_fc_CC_io_SerialPeer *moveme;
        HArrayOfByte *pByteArray;
        BYTE *pByte = (BYTE *)me->m_pAsyncReadBuf;

        GCFramePush(&gcf, &moveme, sizeof(moveme));
        moveme = me;
        // create new JAVA byte array
        pByteArray = MakeByteString((const char *)pByte, me->m_readLen);
        me = moveme;
        GCFramePop(&gcf);

        //the obvious solution "me->m_readBuf = pByteArray" is illegal!
        GCSetObjectReferenceForObject((HObject**)&me->m_readBuf, (HObject*)pByteArray);

        if (pByteArray == NULL)
        {
            me->m_statusRead = ERROR_NOT_ENOUGH_MEMORY;
        }
    }

    // delete read buffer
    BYTE *pByte = (BYTE *)me->m_pAsyncReadBuf;
    delete [] pByte;
    me->m_pAsyncReadBuf = NULL;

    return true;
}


//------------------------------------------------------------------*
/**
 * putDataAsync
 *
 * @param           me          this ptr of Java class
 * @return          true        if write request ist started
 * @exception       -
*/
__declspec(dllexport) /*boolean*/ long __cdecl 
    COM_softing_fc_CC_io_SerialPeer_putDataAsync (struct HCOM_softing_fc_CC_io_SerialPeer *me)
{
    DWORD   nbBytesWritten;
    int     error;

    if (!me->m_isOpen)
    {
        me->m_statusWrite = ERROR_DEVICE_NOT_CONNECTED;
        return false;
    }

    if (me->m_pAsyncWriteBuf)
    {
        me->m_statusWrite = ERROR_WRITE_FAULT;
        return false;
    }

    // init m_statusWrite
    me->m_statusWrite = 0;

    OVERLAPPED *pOverlapped = (OVERLAPPED *)me->m_pAsyncWriteStruct;
    //pOverlapped->hEvent = NULL;
    pOverlapped->Offset = 0;
    pOverlapped->OffsetHigh = 0;

    // copy byte array
    BYTE *pDataBuf = new BYTE[me->m_writeLen];
    me->m_pAsyncWriteBuf = (long)pDataBuf;
    if (pDataBuf == NULL)
    {
        me->m_statusWrite = ERROR_NOT_ENOUGH_MEMORY;
        return false;
    }
    memcpy(pDataBuf, me->m_writeBuf->body, me->m_writeLen);

    if (!WriteFile((HANDLE)me->m_comHandle, pDataBuf, me->m_writeLen, &nbBytesWritten, pOverlapped))
    {
        error = GetLastError();
        if (error != ERROR_IO_PENDING)
        {
            // clear error
            if (!SerialPeerClearCommError(me, NULL, NULL))
                me->m_statusWrite = ERROR_WRITE_FAULT;

            // set m_statusWrite
            SerialPeerSetStatusWrite(me);

            delete [] pDataBuf;
            me->m_pAsyncWriteBuf = NULL;

            if (me->m_statusWrite == 0)
                me->m_statusWrite = ERROR_WRITE_FAULT;

            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------*
/**
 * hasWriteCompleted
 *
 * @param           me          this ptr of Java class
 * @return          true        if no request is pending
 * @exception       -
*/
__declspec(dllexport) /*boolean*/ long __cdecl 
    COM_softing_fc_CC_io_SerialPeer_hasWriteCompleted (struct HCOM_softing_fc_CC_io_SerialPeer *me)
{
    DWORD nbBytesTransferred = 0;
    BOOL bRet;

    if (!me->m_isOpen)
    {
        me->m_statusWrite = ERROR_DEVICE_NOT_CONNECTED;
        return true;
    }

    if (!me->m_pAsyncWriteBuf)
    {
        me->m_statusWrite = ERROR_WRITE_FAULT;
        return true;
    }

    bRet = GetOverlappedResult((HANDLE)me->m_comHandle, (OVERLAPPED *)me->m_pAsyncWriteStruct, 
                               &nbBytesTransferred, false /*bWait*/);
    if (!bRet)
    {
        int iError = GetLastError();
        if (iError == ERROR_IO_INCOMPLETE)
            return false;
        me->m_statusWrite = ERROR_WRITE_FAULT;
    }
    // call SerialPeerClearCommError() in any case!
    // check if error has occured
    if (!SerialPeerClearCommError(me, NULL, NULL))
        me->m_statusWrite = ERROR_WRITE_FAULT;

    // set m_statusWrite
    SerialPeerSetStatusWrite(me);

    // if no error has been occured: check if all bytes has been transferred
    if (me->m_statusWrite == 0 && nbBytesTransferred < (DWORD)me->m_writeLen)
    {
        me->m_statusWrite = ERROR_WRITE_FAULT;
    }

    if (me->m_statusWrite != 0)
    {
        // cancel write request
        bRet = PurgeComm((HANDLE)me->m_comHandle, PURGE_TXABORT|PURGE_TXCLEAR);
        //bRet = GetOverlappedResult((HANDLE)me->m_comHandle, (OVERLAPPED *)me->m_pAsyncWriteStruct, 
        //    &nbBytesTransferred, true /*bWait*/);
    }

    // delete write buffer
    BYTE *pByte = (BYTE *)me->m_pAsyncWriteBuf;
    delete [] pByte;
    me->m_pAsyncWriteBuf = NULL;

    return true;
}


//------------------------------------------------------------------*
/**
 * cancelReadRequest
 *
 * @param           me          this ptr of Java class
 * @exception       -
*/
__declspec(dllexport) void __cdecl 
    COM_softing_fc_CC_io_SerialPeer_cancelReadRequest (struct HCOM_softing_fc_CC_io_SerialPeer *me)
{
    DWORD nbBytesTransferred;
    BOOL bRet;
    BYTE *pDataBuf;

    if (me->m_pAsyncReadBuf != NULL)
    {
        // cancel read request
        bRet = PurgeComm((HANDLE)me->m_comHandle, PURGE_RXABORT|PURGE_RXCLEAR);
        bRet = GetOverlappedResult((HANDLE)me->m_comHandle, (OVERLAPPED *)me->m_pAsyncReadStruct, 
                &nbBytesTransferred, true /*bWait*/);

        // delete buffer
        pDataBuf = (BYTE *)me->m_pAsyncReadBuf;
        delete [] pDataBuf;
        me->m_pAsyncReadBuf = NULL;
    }
}

//------------------------------------------------------------------*
/**
 * cancelWriteRequest
 *
 * @param           me          this ptr of Java class
 * @exception       -
*/
__declspec(dllexport) void __cdecl 
    COM_softing_fc_CC_io_SerialPeer_cancelWriteRequest (struct HCOM_softing_fc_CC_io_SerialPeer *me)
{
    DWORD nbBytesTransferred;
    BOOL bRet;
    BYTE *pDataBuf;

    if (me->m_pAsyncWriteBuf != NULL)
    {
        // cancel write request
        bRet = PurgeComm((HANDLE)me->m_comHandle, PURGE_TXABORT|PURGE_TXCLEAR);
        bRet = GetOverlappedResult((HANDLE)me->m_comHandle, (OVERLAPPED *)me->m_pAsyncWriteStruct, 
                &nbBytesTransferred, true /*bWait*/);

        // delete buffer
        pDataBuf = (BYTE *)me->m_pAsyncWriteBuf;
        delete [] pDataBuf;
        me->m_pAsyncWriteBuf = NULL;
    }
}






// ***********************************************************************
// *                       UDP stuff
// ***********************************************************************


// !! these constants are declared again in:
//    COM.softing.fc.CC.commFB.UDPSocketHandle.java !!
#define UDP_ERR_SOCK_INIT_FAILED  -201
#define UDP_ERR_SOCK_NOT_OPENED   -202
#define UDP_ERR_UNKNOWNHOST       -203
#define UDP_ERR_SEND              -204
#define UDP_ERR_RECV              -205
#define UDP_ERR_RECV_MSG_LONGER   -206
#define UDP_ERR_SEND_MSG_TOO_LONG -207
#define UDP_ERR_SEND_HOST_UNREACH -208


/*
 * initDLL
 */
__declspec(dllexport) void __cdecl 
    COM_softing_fc_CC_io_IODatagram_initDll (struct HCOM_softing_fc_CC_io_IODatagram *mySocket)
{  
    struct
    {
        HCOM_softing_fc_CC_io_IODatagram *phSockSafe;
    } gc;

    GCFrame gcf;
    GCFramePush(&gcf, &gc, sizeof(gc));
    gc.phSockSafe = mySocket;
    GCEnable();
        if (udpInit==0)
        {
            WSADATA wsaData;

            WSAStartup(MAKEWORD(1,1), &wsaData);
            udpInit = 1;
        }
    GCDisable();

    SOCKET sock = INVALID_SOCKET;
    gc.phSockSafe->socket = (long)sock;

    GCFramePop(&gcf);
}


/*
 * closeDll
 */
__declspec(dllexport) void __cdecl 
    COM_softing_fc_CC_io_IODatagram_closeDll (struct HCOM_softing_fc_CC_io_IODatagram *mySocket)
{
    SOCKET sock = (SOCKET)(mySocket->socket);

    GCEnable();
        if (sock!=INVALID_SOCKET)
        {
            closesocket(sock);
        }
    GCDisable();
   
    //WSACleanup();
}


/*
 * udp_open
 */
__declspec(dllexport) long __cdecl 
    COM_softing_fc_CC_io_IODatagram_udp_open (struct HCOM_softing_fc_CC_io_IODatagram *mySocket)
{

    SOCKET sock;
    SOCKADDR_IN saUdpCli;
    int  err = 0;    
    BOOL fBool = TRUE;
    int  timeout = 1;
    BOOL broadcast = (BOOL)mySocket->m_broadcast;
    BOOL multicast = (BOOL)mySocket->m_multicast;
    int  mcTTL     = (int)mySocket->m_mcTTL;
    char *mcAddr = NULL;
    unsigned short port = (unsigned short)mySocket->m_port;    

    int   nlen;
    char  locName[100];
    int   locPort=0;
    int   nSize;


    struct
    {
        HCOM_softing_fc_CC_io_IODatagram *phSockSafe;
    } gc;
    GCFrame gcf;

    if (multicast)
    {
        mcAddr = new char[javaStringLength(mySocket->m_mcAddr)+1];
        javaString2CString(mySocket->m_mcAddr, mcAddr, javaStringLength(mySocket->m_mcAddr)+1);
    }

    GCFramePush(&gcf, &gc, sizeof(gc));
    gc.phSockSafe = mySocket;
    GCEnable();

        sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock==INVALID_SOCKET)
        {
            err = UDP_ERR_SOCK_INIT_FAILED;
            goto errorExit;
        }

        // broadcast socket
        if (broadcast)
        {
            err = setsockopt(
                        sock, 
                        SOL_SOCKET,
                        SO_BROADCAST,
                        (char *) &fBool,
                        sizeof(BOOL)
                   );
            if (err==SOCKET_ERROR)
            {
                err = UDP_ERR_SOCK_INIT_FAILED;
                goto errorExit;
            }
        }

        err = setsockopt(
                    sock, 
                    SOL_SOCKET,
                    SO_REUSEADDR,
                    (char *) &fBool,
                    sizeof(BOOL)
               );
        if (err==SOCKET_ERROR)
        {
            err = UDP_ERR_SOCK_INIT_FAILED;
            goto errorExit;
        }

        // bind to a local socket
        saUdpCli.sin_family = AF_INET;
        saUdpCli.sin_addr.s_addr = htonl(INADDR_ANY);
        saUdpCli.sin_port = htons(port);

        err = bind(sock, (SOCKADDR *) &saUdpCli, sizeof(SOCKADDR_IN));
        if (err==SOCKET_ERROR)
        {
            err = UDP_ERR_SOCK_INIT_FAILED;
            goto errorExit;
        }

        err = setsockopt(
                    sock, 
                    SOL_SOCKET,
                    SO_RCVTIMEO,
                    (char *) &timeout,
                    sizeof(int)
               );
        if (err==SOCKET_ERROR)
        {
            err = UDP_ERR_SOCK_INIT_FAILED;
            goto errorExit;
        }

        err = setsockopt(
                    sock, 
                    SOL_SOCKET,
                    SO_SNDTIMEO,
                    (char *) &timeout,
                    sizeof(int)
               );

        if (err==SOCKET_ERROR)
        {
            err = UDP_ERR_SOCK_INIT_FAILED;
            goto errorExit;
        }


        // multicast socket:
        if (multicast)
        {
            struct ip_mreq mreq;
            mreq.imr_multiaddr.s_addr = inet_addr (mcAddr);
            mreq.imr_interface.s_addr = INADDR_ANY;
  
            delete mcAddr;

            // join multicast group
            err = setsockopt(sock, 
                             IPPROTO_IP, 
                             IP_ADD_MEMBERSHIP, 
                             (char FAR *)&mreq, 
                             sizeof (mreq));
            if (err == SOCKET_ERROR)
            {
                err = UDP_ERR_SOCK_INIT_FAILED;
                goto errorExit;
            }

            // Set the Time-to-Live of the multicast.
            err = setsockopt(sock, 
                             IPPROTO_IP, 
                             IP_MULTICAST_TTL, 
                             (char FAR *)&mcTTL, 
                              sizeof (int));
            if (err == SOCKET_ERROR)
            {
                err = UDP_ERR_SOCK_INIT_FAILED;
                goto errorExit;
            }
        }

        // get local host address
        SOCKADDR_IN saUdp;
        nSize = sizeof(SOCKADDR_IN);
        nlen = getsockname(sock, (SOCKADDR FAR *)&saUdp, &nSize);
        if (nlen != SOCKET_ERROR)
        {
            locPort = (long)(ntohs(saUdp.sin_port));
            nlen = gethostname(locName, 100);
        }
        
    GCDisable();
    gc.phSockSafe->socket = (long)sock;
    if (nlen!=SOCKET_ERROR)
    {
        gc.phSockSafe->m_localPortNr = locPort;
        Hjava_lang_String* pJavaString;
        pJavaString = makeJavaString(locName, strlen(locName));
        GCSetObjectReferenceForObject((HObject**)&(gc.phSockSafe->m_localHostAddr), (HObject*)pJavaString);
    }
    GCFramePop(&gcf);
    return 0;


errorExit:
        if (sock!=INVALID_SOCKET)
        {
            closesocket(sock);
            sock = INVALID_SOCKET;
        }
    GCDisable();
    gc.phSockSafe->socket = (long)sock;
    GCFramePop(&gcf);
    return err;
}


/*
 * udp_close
 */
__declspec(dllexport) long __cdecl 
    COM_softing_fc_CC_io_IODatagram_udp_close (struct HCOM_softing_fc_CC_io_IODatagram *mySocket)
{
    SOCKET sock = (SOCKET)(mySocket->socket);

    struct
    {
        HCOM_softing_fc_CC_io_IODatagram *phSockSafe;
    } gc;
    GCFrame gcf;

    GCFramePush(&gcf, &gc, sizeof(gc));
    gc.phSockSafe = mySocket;
    GCEnable();

        if (sock!=INVALID_SOCKET)
        {
            closesocket(sock);
            sock = INVALID_SOCKET;
        }
    
    GCDisable();
    gc.phSockSafe->socket = (long)sock;
    GCFramePop(&gcf);

    return 0;
}


/*
 * udp_send
 */
__declspec(dllexport) long __cdecl 
    COM_softing_fc_CC_io_IODatagram_udp_send (struct HCOM_softing_fc_CC_io_IODatagram *mySocket)
{
    SOCKADDR_IN saUdpServ;
    int err = 0;
    char* buf;
    int len;
    SOCKET sock = (SOCKET)(mySocket->socket);
    unsigned short port = (unsigned short)(mySocket->m_toPort);

    char* addrBuf;

    struct
    {
        HCOM_softing_fc_CC_io_IODatagram *phSockSafe;
    } gc;
    GCFrame gcf;

    if (sock==INVALID_SOCKET)
    {
        return UDP_ERR_SOCK_NOT_OPENED;
    }

    addrBuf = new char[javaStringLength(mySocket->m_toAddr)+1];
    javaString2CString(mySocket->m_toAddr, addrBuf, javaStringLength(mySocket->m_toAddr)+1);

    len = (int)(mySocket->m_packLen);
    buf = new char[len];
    memcpy(buf, mySocket->m_packet->body, len);


    GCFramePush(&gcf, &gc, sizeof(gc));
    gc.phSockSafe = mySocket;
    GCEnable();

        err = 0;

        saUdpServ.sin_family = AF_INET;
        saUdpServ.sin_port = htons(port);

        if (strcmp(addrBuf, "255.255.255.255")==0)
        {
            saUdpServ.sin_addr.s_addr = htonl(INADDR_BROADCAST);   
        }
        else
        {
            saUdpServ.sin_addr.s_addr = inet_addr(addrBuf);
            if (saUdpServ.sin_addr.s_addr == INADDR_NONE)
            {
                err = UDP_ERR_UNKNOWNHOST;
            }
        }
       
        delete addrBuf;

        if (err==0)
        {
            err = sendto(
                    sock,
		            buf,
		            len,
		            0,
		            (SOCKADDR *) &saUdpServ,
		            sizeof(SOCKADDR_IN)
		          );

            if (err==SOCKET_ERROR)
            {
                if (WSAGetLastError()==WSAEMSGSIZE)
                {
                    err = UDP_ERR_SEND_MSG_TOO_LONG ;
                }
                else if (WSAGetLastError()==WSAEHOSTUNREACH)
                {
                    err = UDP_ERR_SEND_HOST_UNREACH;
                }
                else
                {
                    err = UDP_ERR_SEND;
                }
            }
            else
            {
                err = 0;
            }
        }

    GCDisable();
    GCFramePop(&gcf);

    delete buf;

    return err;
}


/*
 * udp_rcv
 */
__declspec(dllexport) long __cdecl 
    COM_softing_fc_CC_io_IODatagram_udp_recv (struct HCOM_softing_fc_CC_io_IODatagram *mySocket)
{
    SOCKADDR_IN saUdpServ;
    int nSize, err;
    int nRead;

    SOCKET sock = (SOCKET)(mySocket->socket);

    char* addrBuf;
    char* buf;
    int len;

    struct timeval timeout = {0,0};
    fd_set readSet;

    struct
    {
        HCOM_softing_fc_CC_io_IODatagram *phSockSafe;
    } gc;
    GCFrame gcf;

    if (sock==INVALID_SOCKET)
    {
        return UDP_ERR_SOCK_NOT_OPENED;
    }

    len = (int)(mySocket->m_packLen);
    buf = new char[len];

    saUdpServ.sin_port = 0; // avoid warning
    saUdpServ.sin_addr.S_un.S_addr=0;

    GCFramePush(&gcf, &gc, sizeof(gc));
    gc.phSockSafe = mySocket;
    GCEnable();

        err = 0;
        nRead = 0;
   
        FD_ZERO(&readSet);
        FD_SET(sock, &readSet);

        if (select(0, &readSet, NULL, NULL, &timeout)<1)
        {
            err = UDP_ERR_RECV;
        }

        if (FD_ISSET(sock, &readSet))
        {        
            nSize = sizeof(SOCKADDR_IN);
            nRead = recvfrom(
                        sock,
                        buf,
                        len,
                        0,
                        (SOCKADDR FAR *)&saUdpServ,
                        &nSize
                  );
    
            if (nRead==SOCKET_ERROR)
            {
                if (WSAGetLastError()==WSAEMSGSIZE)
                {
                    err = UDP_ERR_RECV_MSG_LONGER;
                    nRead = len;
                }
                else
                {
                    err = UDP_ERR_RECV;
                }
            } 
            else
            {
                err = 0;
            }
        }
        else
        {
            // mark timeout (here error, in IODatagram, buffers/length will be set)
            err = -1;
        }

    GCDisable();

    if (err==0 || err==UDP_ERR_RECV_MSG_LONGER)
    {
        memcpy(gc.phSockSafe->m_packet->body, buf, nRead);

        gc.phSockSafe->m_anzRead = nRead;

        gc.phSockSafe->m_fromPort = (long)(ntohs(saUdpServ.sin_port));
        addrBuf = inet_ntoa(saUdpServ.sin_addr);

        Hjava_lang_String* pJavaString;
        pJavaString = makeJavaString(addrBuf, strlen(addrBuf));
        GCSetObjectReferenceForObject((HObject**)&(gc.phSockSafe->m_fromAddr), (HObject*)pJavaString);
    }
    else
    {
        gc.phSockSafe->m_anzRead = 0;
        gc.phSockSafe->m_fromPort = 0;
        // timeout, only clear buffer, do not signal error
        if (err==-1)
        {
            err = 0;
        }
    }
    
    GCFramePop(&gcf);

    delete buf;

    return err;
}



/*
 * udp_lookup
 */
__declspec(dllexport) long __cdecl 
   COM_softing_fc_CC_io_IODatagram_udp_lookup (struct HCOM_softing_fc_CC_io_IODatagram *mySocket)
{
    char *hostName = NULL;
    char *hostAddr;
    int err = 0;

    struct
    {
        HCOM_softing_fc_CC_io_IODatagram *phSockSafe;
    } gc;
    GCFrame gcf;

    hostName = new char[javaStringLength(mySocket->m_luHostName)+1];
    javaString2CString(mySocket->m_luHostName, hostName, javaStringLength(mySocket->m_luHostName)+1);    
    hostAddr = NULL;

    GCFramePush(&gcf, &gc, sizeof(gc));
    gc.phSockSafe = mySocket;
    GCEnable();

    struct hostent *hp;
    struct in_addr inAddr;
    char *ap;
    
    // check if name or address(xxx.yyy.zzz...)
    if (inet_addr(hostName)!=INADDR_NONE)
    {
        hostAddr = hostName;
    }
    else
    {
        // try to find the address for the name
        hp = gethostbyname(hostName);

        if (hp==NULL)
        {
            err = UDP_ERR_UNKNOWNHOST;   
        }
        else
        {
            // if found get the dotted string representing the address
            memset(&inAddr, 0, sizeof(struct in_addr));
            memcpy(&(inAddr), hp->h_addr, sizeof(struct in_addr));
            ap = inet_ntoa(inAddr);
            if (ap==NULL)
            {
                err = UDP_ERR_UNKNOWNHOST;
            }
            else
            {
                hostAddr = ap;
            }
        }
    }

    GCDisable();

    if (err==0)
    {
        Hjava_lang_String* pJavaString;
        pJavaString = makeJavaString(hostAddr, strlen(hostAddr));
        GCSetObjectReferenceForObject((HObject**)&(gc.phSockSafe->m_luHostAddr), (HObject*)pJavaString);
    }

    GCFramePop(&gcf);

    delete hostName;
   
    return err;
}



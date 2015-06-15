/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/comhelper.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: comhelper.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/comhelper.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           INC
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *== COM helper macros and classes
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  08.02.2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef __COMHELPER_H_
#define __COMHELPER_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

//------------------------------------------------------------------*
/**
 *  class CGuid - wrapper/ helper class to wrap a GUID (Globally
 *  unique identifier) used with COM/OLE functions.
 *  Provides methods for conversion into strings or from strings.
 *  Supports assigment of REFGUID objects and provides comparision
 *  operators.
 *  <b>Prerequisite</b>: USES_CONVERSION and conversion macros have to 
 *  be defined.
 *
 *  @see    SDK functions ::CLSIDFromString(),::StringFromCLSID()<br>
 *  @see    SDK documentation for GUID,CLSID.
*/
class CGuid
{
    GUID m_guid;      /**< GUID storage */
    LPTSTR m_pszGuid; /**< temporary string representation*/
public:

    //------------------------------------------------------------------*
    /**
     * Default constructor - sets the GUID to all zeros.
     *
     * @see     CGuid class description             
    */
    CGuid() 
    { 
        memset(&m_guid,0,sizeof(GUID));
        m_pszGuid = NULL;
    }


    //------------------------------------------------------------------*
    /**
     * Constructor taking a reference to an GUID and storing it.
     *
     * @param   rGuid - reference to GUID to be stored.
     * @see     CGuid class description
    */
    CGuid(REFGUID rGuid)
    {
        m_guid = rGuid;
        m_pszGuid = NULL;
    }

    //------------------------------------------------------------------*
    /**
     * Constructor taking a reference to an GUID and storing it.
     *
     * @param   rGuid - reference to GUID to be stored.
     * @see     CGuid class description
    */
    CGuid(GUID & rGuid)
    {
        m_guid = rGuid;
        m_pszGuid = NULL;
    }


    //------------------------------------------------------------------*
    /**
     * Conversion constructor for passing a BSTR. The passed BSTR string
     * will be converted to a GUID using ::CLSIDFromString(). The stored
     * GUID may be all zeros if the passed BSTR is null or if the 
     * conversion via ::CLSIDFromString() fails.
     *
     * @param   sGuid - BSTR string representation of a GUID. If sGuid 
     *          is NULL or if the conversion fails the stored GUID is
     *          all zeros.
     * @see     CGuid class description
    */
    CGuid(BSTR sGuid)
    {
        memset(&m_guid,0,sizeof(GUID));
        m_pszGuid = NULL;
        
        if(sGuid == NULL) {
            return;
        }
        ::CLSIDFromString(sGuid,&m_guid);
    }


    //------------------------------------------------------------------*
    /**
     * Conversion constructor for passing a LPCTSTR. The passed LPCTSTR string
     * will be converted to a GUID using ::CLSIDFromString(). The stored
     * GUID may be all zeros if the passed LPCTSTR is null or if the 
     * conversion via ::CLSIDFromString() fails.
     *
     * @param   pszGuid - LPCTSTR string representation of a GUID. If pszGuid 
     *          is NULL or if the conversion fails the stored GUID is
     *          all zeros.
     * @see     CGuid class description
    */
    CGuid(LPCTSTR pszGuid)
    {
        m_pszGuid = NULL;
        memset(&m_guid,0,sizeof(GUID));

        if(pszGuid == NULL) {
            return;
        }

        LPTSTR psz = const_cast<LPTSTR>(pszGuid);
        USES_CONVERSION;
        LPOLESTR szGuid = T2OLE(psz);
        ::CLSIDFromString(szGuid,&m_guid);
    }

    //------------------------------------------------------------------*
    /**
     * Copy Constructor taking a reference to an CGuid object and storing it.
     * 
     * @param   other - reference to CGuid to be copied.
     * @see     CGuid class description
    */
    CGuid(const CGuid & other)
    {
        m_pszGuid = NULL;
        if(this != &other) {
            m_guid = other.m_guid;
        }
    }

    //------------------------------------------------------------------*
    /**
     * Destructor of CGuid object. Deletes stored temporary string of an
     * prior LPTSTR conversion operation.
     *
     * @see  CGuid class description, operator LPTSTR()
    */
    ~CGuid()
    {
        DeleteString();
    }

    //------------------------------------------------------------------*
    /**
     * Assignment operator taking a reference to an CGuid object and storing it.
     * 
     * @param   other - reference to CGuid to be assigned.
     * @see     CGuid class description
    */
    const CGuid & operator = (const CGuid & other) 
    {
        if(this != &other) {
            m_guid = other.m_guid;
        }
        return *this;
    }

    //------------------------------------------------------------------*
    /**
     * Assignment operator taking a reference to an GUID and storing it.
     * 
     * @param   rGuid - reference to CGuid to be assigned.
     * @see     CGuid class description
    */
    const CGuid & operator = (REFGUID rGuid) 
    {
        m_guid = rGuid;
        return *this;
    }

    //------------------------------------------------------------------*
    /**
     * Assignment operator taking a reference to an GUID and storing it.
     * 
     * @param   rGuid - reference to CGuid to be assigned.
     * @see     CGuid class description
    */
    const CGuid & operator = (GUID & rGuid)
    {
        m_guid = rGuid;
        return *this;
    }

    //------------------------------------------------------------------*
    /**
     * assigment operator for assigning a BSTR. The passed BSTR string
     * will be converted to a GUID using ::CLSIDFromString(). The stored
     * GUID may be all zeros if the passed BSTR is null or if the 
     * conversion via ::CLSIDFromString() fails.
     *
     * @param   sGuid - BSTR string representation of a GUID. If sGuid 
     *          is NULL or if the conversion fails the stored GUID is
     *          all zeros.
     * @see     CGuid class description
    */
    const CGuid & operator = (BSTR sGuid)
    {
        memset(&m_guid,0,sizeof(GUID));
        
        if(sGuid == NULL) {
            return *this;
        }
        ::CLSIDFromString(sGuid,&m_guid);
        return *this;
    }


    //------------------------------------------------------------------*
    /**
     * Converts the stored GUID into an BSTR. The BSTR is allocated inside
     * the method and stored in the BSTR reference passed as argument.
     * Uses SDK function ::StringFromCLSID() to convert the GUID into the 
     * string representation.
     *
     * @param           psGUID - reference to an BSTR. Shall not be NULL.<br>
     *                  Call ::SysFreeString() after you called the method.
     *                  and you don't need the returned BSTR any more.
     * @return          E_POINTER - if passed reference is NULL.
     * @return          HRESULT - of StringFromCLSID if operation fails.                 
     * @see             
    */
    HRESULT ToBSTR(BSTR* psGuid)
    {
		LPOLESTR szGuid;
        HRESULT hr;

        if(psGuid == NULL) {
            return E_POINTER;
        }
		
        hr = ::StringFromCLSID(m_guid, &szGuid);
        if(FAILED(hr)) {
            return hr;
        }

		*psGuid = ::SysAllocString(szGuid);
        if(szGuid) {
            ::CoTaskMemFree(szGuid);
        }
        return S_OK;
    }

    //------------------------------------------------------------------*
    /**
     * Conversion operator to LPTSTR. The returned LPTSTR is only temporary.
     * Hence it is only valid within the scope resp. life time of the 
     * corresponding CGuid object.
     * <b>NOTE: Do not store the returned reference out of the scope of 
     * the object that returned the reference.</b>
     *
     * @return          LPTSTR representation of GUID. NULL in case of 
     *                  an out of memory situation or if ::StringFromCLSID
     *                  failed.
     * 
     * @see             ::StringFromCLSID(), ~CGuid()
    */
    operator LPTSTR()
    {
		LPOLESTR szGuid;
        LPTSTR pszTemp = NULL;
        HRESULT hr;

        USES_CONVERSION;
		
        hr = ::StringFromCLSID(m_guid, &szGuid);
        if(FAILED(hr)) {
            return NULL;
        }

        pszTemp = OLE2T(szGuid);
        if(!pszTemp) {
            if(szGuid) {
                ::CoTaskMemFree(szGuid);
            }
            return NULL;
        }

        DeleteString();

        m_pszGuid = new TCHAR[_tcslen(pszTemp) +1];
        if(!m_pszGuid) {
            if(szGuid) {
                ::CoTaskMemFree(szGuid);
            }
            return NULL;
        }

        _tcscpy(m_pszGuid,pszTemp);
        if(szGuid) {
            ::CoTaskMemFree(szGuid);
        }
        return m_pszGuid;
    }

    //------------------------------------------------------------------*
    /**
     * Conversion operator to a GUID pointer. Returns the address of the
     * stored GUID structure.
     *
     * @return          Address of stored GUID.
     * 
     * @see             REFGUID() operator.
    */
    operator GUID* ()
    {
        return & m_guid;
    }

    //------------------------------------------------------------------*
    /**
     * Conversion operator to a REFGUID reference.
     *
     * @return          Reference to stored GUID.
     * 
     * @see             GUID* () operator.
    */
    operator REFGUID ()
    {
        return m_guid;
    }

    //------------------------------------------------------------------*
    /**
     * Conversion operator to a non constant GUID reference.
     *
     * @return          Reference to stored GUID.
     * 
     * @see             GUID* () operator.
    */
    operator GUID & ()
    {
        return m_guid;
    }

    //------------------------------------------------------------------*
    /**
     * Empties the CGuid object. The stored GUID will be set to all zeros.
     * A temporary string resulting from a previous call to the LPTSTR()
     * operator will be deleted as well.
     *
     * @see             LPTSTR()
    */
    void Empty()
    {
        memset(&m_guid,0,sizeof(GUID));
        DeleteString();
    }

    /**
     * Returns true if the stored GUID is an null GUID containing
     * all zeros. Otherwise returns false.
     *
     * @return          true - stored GUID is an null GUID.<br>
     * @return          false - stored GUID isn't an null GUID.
     * @see             operator == ()
    */
    bool IsEmpty()
    {
        return *this == CGuid(GUID_NULL);
    }




    //------------------------------------------------------------------*
    /**
     * Creates a new guid by calling ::CoCreateGuid().
     * The CGuid object will be emptied by calling Empty() prior to the
     * call of ::CoCreateGuid().
     * @return		HRESULT code of the call to ::CoCreateGuid()
     * @see			Empty()
    */
    HRESULT Create()
    {
        HRESULT hr = S_OK;
        Empty();

    	hr = CoCreateGuid(&m_guid);
	    return hr;
    }

    friend inline bool operator == (CGuid & op1, CGuid & op2);
    friend inline bool operator != (CGuid & op1, CGuid & op2);

private:
    void DeleteString()
    {
        if(m_pszGuid) {
            delete [] m_pszGuid;
            m_pszGuid = NULL;
        }
    }
};

//------------------------------------------------------------------*
/**
 * CGuid friend operator == compares to CGuid objects and returns 
 * true if the stored GUIDs are equal.
 *
 * @param           op1 - CGuid reference of the first operand.
 * @param           op2 - CGuid reference of the second operand.
 * @return          true - both GUIDs stored are equal
 * @return          false - both GUIDs stored are not equal
 * @see             CGuid class description
*/
inline bool operator == (CGuid & op1, CGuid & op2)
{
    BOOL b = IsEqualGUID(op1.m_guid,op2.m_guid);
    return b == TRUE ? true : false;
}

//------------------------------------------------------------------*
/**
 * CGuid friend operator != compares to CGuid objects and returns 
 * true if the stored GUIDs are not equal.
 *
 * @param           op1 - CGuid reference of the first operand.
 * @param           op2 - CGuid reference of the second operand.
 * @return          true - both GUIDs stored are not equal
 * @return          false - both GUIDs stored are equal
 * @see             CGuid class description
*/
inline bool operator != (CGuid & op1, CGuid & op2)
{
    BOOL b = IsEqualGUID(op1.m_guid,op2.m_guid);
    return b == FALSE ? true : false;
}


//------------------------------------------------------------------*
/**
 * Waits for an event with a given time out while processing the 
 * message loop as well. 
 * Waits for the object to be signaled, meanwhile dispatching window 
 * messages as needed. This is useful if you want to wait for an 
 * object's event to happen and be notified of it happening, but allow 
 * window messages to be dispatched while waiting.
 * Function is most commonly used to wait for an event in an STA thread 
 * that is in turn signalled by an MTA thread.
 * To allow the STA thread for processing messages which includes incoming
 * COM calls to the STA thread this function shall be used.
 * Note that a blocking of the STA thread will occur and no incoming COM
 * calls are processed when using WaitForSingle/MultipleObjects instead of
 * using this function.
 * @param			HANDLE hEvent - event handle
 * @param			DWORD dwTimeout - timeout to wait for
 * @return			DWORD - return value: Maybe WAIT_OBJECT_0 for an 
 *                  event signalled or WAIT_TIMEOUT if timeout elapsed.
 *
 * @see	AtlWaitWithMessageLoop().
*/
inline DWORD WaitWithMessageLoop(HANDLE hEvent,DWORD dwTimeout)
{
	DWORD dwRet,dwTicks,dw;
    DWORD dwTO = dwTimeout;
	MSG msg;

    //get current time ticks
    dwTicks = ::GetTickCount();

	while(1)
	{
		dwRet = MsgWaitForMultipleObjects(1, &hEvent, FALSE, dwTO , QS_ALLINPUT);

		if (dwRet == WAIT_OBJECT_0)
			break;    // The event was signaled

		if (dwRet != WAIT_OBJECT_0 + 1)
			break;          // Something else happened

        // There is one or more window message available. Dispatch them
		while(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
            dwRet = WaitForSingleObject(hEvent, 0);
            if (dwRet == WAIT_OBJECT_0) {
				return dwRet; // Event is now signaled.
            }

		    // Build the difference to determine next time out value
            // consider wrap around of tick count as well
            dw = ::GetTickCount();
            if(dw >= dwTicks) {
                dwTicks = dw - dwTicks;
            }
            else {
                //wrap around case
                dwTicks = UINT_MAX - dwTicks + dw;
            }
            //for debug purposes
            //TRACE(_T("dwTO(n) = %ld, dwTicks(n+1) = %ld, dwTO(n+1) = %ld\n"),dwTO,dwTicks,dwTO-dwTicks);
            //determine new time out value.
            if(dwTO > dwTicks) { //actual timeout greater continue
                dwTO = dwTO - dwTicks;
                //get next tick count
                dwTicks = ::GetTickCount();
            }
            else {
                dwRet = WAIT_TIMEOUT; //Time out elapsed return.
                return dwRet;
            }
		}
	}
	return dwRet;
}


#endif // __COMHELPER_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: comhelper.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 7  *****************
 * User: Su           Date: 29.08.01   Time: 19:07
 * Updated in $/4Control/COM/softing/fc/inc
 * WaitWithMessageLoop()
 * 
 * *****************  Version 6  *****************
 * User: Su           Date: 9.05.01    Time: 12:52
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 5  *****************
 * User: Su           Date: 5.04.01    Time: 19:29
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 4  *****************
 * User: Su           Date: 28.03.01   Time: 11:59
 * Updated in $/4Control/COM/softing/fc/inc
 * Added CGuid::Create()
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 15.02.01   Time: 12:00
 * Updated in $/4Control/COM/softing/fc/inc
 * friend operators must be inline.
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 13.02.01   Time: 11:19
 * Updated in $/4Control/COM/softing/fc/inc
 * assigment operator for assigning a BSTR, conversion operators, method
 * to empty a CGuid object and to determine wheter it is empty
 * (Empty,IsEmpty).
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 8.02.01    Time: 19:05
 * Created in $/4Control/COM/softing/fc/inc
 * Initial rev.
 *==
 *----------------------------------------------------------------------------*
*/

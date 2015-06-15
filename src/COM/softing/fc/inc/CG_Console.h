/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/CG_Console.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CG_Console.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/CG_Console.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CCG
 *
 * =COMPONENT           CG_Console
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
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  01.08.01  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/

#ifndef __CG_CONSOLE_H_
#define __CG_CONSOLE_H_


//----  Aggregate Includes:   --------------------------------------*
#include <tchar.h>
#include <unknwn.h>

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
#ifdef UNICODE
#error "CG_Console.h cannot be used with unicode, using TCHAR is only for future versions"
#endif

//----  Globals:    ------------------------------------------------*
//coclass supports the IID_ICG_IECFront interface ID:
EXTERN_C const GUID CLSID_CG_Console;

//----  Prototypes:    ---------------------------------------------*

//+-------------------------------------------------------------------------
EXTERN_C const GUID FAR  IID_ICG_Console;
class ICG_Console : public IUnknown
{
public:


    /**
     * genConsoleForTarget: gets only called if target could be compiler with no erorrs
	 *
	 * @param     pIFront          initialised and loaded front end where 
	 *                             to get info for resource. 
	 *                             Is a interface of type ICG_IECFront, QI 
	 *                             for IID_ICG_IECFront.
	 * @param     sTargetInstName  the name of the target to build the console
	 * @param     sResourceName    the resource IEC name assigned to the target
	 * @param     sWebServerAddr   address where to download the console files     
	 * @param     sControlAddr     address of control
	 * @param     sTargetTypeName  the target type name 
	 * @param     plErrors         receives the number of errors 
	 * @param     plWarnings       receives the number of warnings 
	 *  
     * @return   S_OK or E_FAIL	   if E_FAIL a meaning full diag msg was 
	 *                             already submitted by this method.
     */
    virtual HRESULT STDMETHODCALLTYPE genConsoleForTarget(
        /*[in] */ IUnknown*  pIFront,           
        /*[in] */ BSTR       sTargetInstName,     
        /*[in] */ BSTR       sResourceName,
        /*[in] */ BSTR       sWebServerAddr,
        /*[in] */ BSTR       sControlAddr,
        /*[in] */ BSTR       sTargetTypeName,
        /*[out]*/ long*      plErrors,
        /*[out]*/ long*      plWarnings
    ) = 0;

    /**
     * genConsoleForLibrary: buils all files to be checked into a library
     *
     * @param     pIFront          initialised and loaded front end where 
     *                             to get info of POU header/body. 
     *                             Is a interface of type ICG_IECFront, QI 
     *                             for IID_ICG_IECFront.
     * @param     sTargetTypeName  the target type name 
     * @param     plErrors         receives the number of errors 
     * @param     plWarnings       receives the number of warnings 
     *  
     * @return   S_OK or E_FAIL	   if E_FAIL a meaning full diag msg was 
     *                             already submitted by this method.
     */
    virtual HRESULT STDMETHODCALLTYPE genConsoleForLibrary(
        /*[in] */ IUnknown*  pIFront,           
        /*[in] */ BSTR       sTargetTypeName,
        /*[out]*/ long*      plErrors,
        /*[out]*/ long*      plWarnings
    ) = 0;   
};





#endif // __CG_CONSOLE_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: CG_Console.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 6  *****************
 * User: Ca           Date: 27.08.01   Time: 12:18
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 16.08.01   Time: 13:00
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 16.08.01   Time: 12:49
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 16.08.01   Time: 11:39
 * Updated in $/4Control/COM/softing/fc/inc
 * add custom vis domains
 * 
 * *****************  Version 2  *****************
 * User: Ca           Date: 7.08.01    Time: 8:51
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ca           Date: 6.08.01    Time: 11:06
 * Created in $/4Control/COM/softing/fc/inc
 *==
 *----------------------------------------------------------------------------*
*/

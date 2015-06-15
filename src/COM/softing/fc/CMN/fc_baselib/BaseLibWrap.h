/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_baselib/BaseLibWrap.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BaseLibWrap.h $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_baselib/BaseLibWrap.h $
 *
 * =PROJECT             7VH102  ATCMControl
 *
 * =SWKE                4CMN
 *
 * =COMPONENT           BaseLib
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *           $Revision: 1 $
 *
 * =ENVIRONMENT             Win32:
 *                      MS Windows NT 4.0
 *                      MSVC++ V5.0 / MFC 4.2 / ATL 2.0 / DAO 3.5
 *                      MS Java 2.0
 *                      MS Visual SourceSafe 5.0
 *
 * =REFERENCES
 *==
 *----------------------------------------------------------------------------*
 *

 

 

 *==
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    ATCM BaseLib COM wrapper
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  1998-02-11  Je      File created
 *  see history at end of file !
 *==
*******************************************************************************
H<<*/

#ifndef _BASELIB_WRAP_H_
#define _BASELIB_WRAP_H_

//----  Aggregate Includes:   --------------------------------------*
#include "BaseLibCOM.h"     // interfaces
#include "BaseLib.h"
#include "resource.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*




/////////////////////////////////////////////////////////////////////////////
// CFC_BaseLib
class ATL_NO_VTABLE CFC_BaseLib : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CFC_BaseLib, &CLSID_FCBaseLib>,
	public IDispatchImpl<IBL_Conversion, &IID_IBL_Conversion, &LIBID_CMN_FCBaseLib>,
    public IObjectSafetyImpl<CFC_BaseLib,INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
    public IProvideClassInfo2Impl<&CLSID_FCBaseLib, &IID_IBL_Conversion, &LIBID_CMN_FCBaseLib>
{
//--------------------------
// =Constructor
public:
            	            CFC_BaseLib();
            	            ~CFC_BaseLib();


//--------------------------
// =Interface map
BEGIN_COM_MAP(CFC_BaseLib)
	COM_INTERFACE_ENTRY(IBL_Conversion)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
    COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

//--------------------------
// =Connection point map
BEGIN_CONNECTION_POINT_MAP(CFC_BaseLib)
END_CONNECTION_POINT_MAP()

DECLARE_REGISTRY_RESOURCEID(IDR_FCBASELIB)

//--------------------------
// =Operations
public:

//--------------------------
// =Overrides
public:
    HRESULT                 FinalConstruct();
    void                    FinalRelease();

//--------------------------
// =Interface
// IBL_Conversion
public:
	STDMETHOD(CheckString)(/*[in, string]*/ BSTR sValue, /*[in, string]*/ BSTR sDatatype, /*[in, string]*/ BSTR sFormat, long options, /*[out, retval]*/ long *phResult);
	STDMETHOD(VariantToString)(VARIANT vtIn, /*[in, string]*/ BSTR sDatatype, /*[in, string]*/ BSTR sFormat, long options, long lMaxCharsWanted, /*[out, string]*/ BSTR *psOut);
	STDMETHOD(StringToVariant)(/*[in, string]*/ BSTR sValue, /*[in, string]*/ BSTR sDatatype, /*[in, string]*/ BSTR sFormat, long options, /*[out]*/ VARIANT *pvtOut);
    STDMETHOD(ConvertSecondsToDate)(/*[in]*/ long seconds, /*[out, retval]*/ DATE *result);

//--------------------------
// Helpers
private:

//--------------------------
// =Attributes
private:
};

/////////////////////////////////////////////////////////////////////
#endif // _BASELIB_WRAP_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: BaseLibWrap.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_baselib
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_baselib
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 23.01.01   Time: 17:12
 * Created in $/4Control/COM/softing/fc/CMN/fc_baselib
 * utils and baselib from V1.3
 * 
 * *****************  Version 4  *****************
 * User: Ed           Date: 5.04.00    Time: 13:16
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * add a method to convert the timestamps from CSC_simpleOPC to OLE DATEs
 * 
 * *****************  Version 3  *****************
 * User: Ed           Date: 18.09.98   Time: 14:07
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * changes in IObjectSafetyImpl for C 6.0
 * 
 * *****************  Version 2  *****************
 * User: Je           Date: 8.07.98    Time: 18:47
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * * moved BaseLib COM wrapper into BaseLib itself
 * * CheckString w/o HRESULT error if baselib syntax error
 * 
 * *****************  Version 1  *****************
 * User: Je           Date: 8.07.98    Time: 11:21
 * Created in $/4Control/COM/softing/fc/CMN/baselib
 *==
 *----------------------------------------------------------------------------*
*/

// 1998-02-11/Je  Generated by PERL5 script d:\bin\perl5/mkproto.bat Version 1.2
// ***************************  E O F  ******************************
// $Workfile: BaseLibWrap.h $

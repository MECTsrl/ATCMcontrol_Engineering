/* $Header: /4CReleased/V2.20.00/Target/4CWin/ONLINE/OnlServ/OnlServ.cpp 1     28.02.07 19:11 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: OnlServ.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/ONLINE/OnlServ/OnlServ.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Target / Online
 *
 * =COMPONENT			OnlServ
 *
 * =CURRENT 	 $Date: 28.02.07 19:11 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"

#include "Connection.h"
#include "CommissioningSession.h"
#include "Config.h"
#include "ControlMgt.h"
#include "Config.h"

/* ----  Local Defines:   ----------------------------------------------------- */

#define OPC_SERV_BIG_ENDIAN 0

/* ----  Global Variables:   -------------------------------------------------- */

/* ----  Local Functions:   --------------------------------------------------- */

/* ----  Implementations:   --------------------------------------------------- */


#if OPC_SERV_BIG_ENDIAN == 1	/* -------------------------------------------- */

void SwapDouble(double *val, double *result)
{
  BYTE *pResult = (BYTE*)result;
  BYTE *pVal = (BYTE*)val;
  pResult[0] = pVal[7];
  pResult[1] = pVal[6];
  pResult[2] = pVal[5];
  pResult[3] = pVal[4];
  pResult[4] = pVal[3];
  pResult[5] = pVal[2];
  pResult[6] = pVal[1];
  pResult[7] = pVal[0];
  return;
}

void SwapFloat(float *val, float *result)
{
  BYTE *pResult = (BYTE*)result;
  BYTE *pVal = (BYTE*)val;
  pResult[0] = pVal[3];
  pResult[1] = pVal[2];
  pResult[2] = pVal[1];
  pResult[3] = pVal[0];
  return;
}

__int64 SwapI64(__int64 val)
{
  __int64 result;
  BYTE *pResult = (BYTE*)&result;
  BYTE *pVal = (BYTE*)&val;
  pResult[0] = pVal[7];
  pResult[1] = pVal[6];
  pResult[2] = pVal[5];
  pResult[3] = pVal[4];
  pResult[4] = pVal[3];
  pResult[5] = pVal[2];
  pResult[6] = pVal[1];
  pResult[7] = pVal[0];
  return result;
}

long SwapLong(long val)
{
  long result;
  BYTE *pResult = (BYTE*)&result;
  BYTE *pVal = (BYTE*)&val;
  pResult[0] = pVal[3];
  pResult[1] = pVal[2];
  pResult[2] = pVal[1];
  pResult[3] = pVal[0];
  return result;
}

short SwapShort(short val)
{
  short result;
  BYTE *pResult = (BYTE*)&result;
  BYTE *pVal = (BYTE*)&val;
  pResult[0] = pVal[1];
  pResult[1] = pVal[0];
  return result;
}

#else	/* -------------------------------------------------------------------- */

void SwapDouble(double *val, double *result)
{
	*result = *val;
}

void SwapFloat(float *val, float *result)
{
	*result = *val;
}

__int64 SwapI64(__int64 val)
{
	return val;
}

long SwapLong(long val)
{
	return val;
}

short SwapShort(short val)
{
	return val;
}

#endif /* -------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------- */
/**
 * Adapt_StartOnlineChange
 *
 */
STDMETHODIMP CCommissioningSession::Adapt_StartOnlineChange(long lKey, CSC_TRANSFER_TARGET target, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_CommitOnlineChange
 *
 */
STDMETHODIMP CCommissioningSession::Adapt_CommitOnlineChange(long lKey, CSC_TRANSFER_TARGET target, /*[in, string]*/ BSTR sResource, /*[in, string] */ BSTR sProjectName, /*[in, string]*/ BSTR sProjectGUID, /*[in, string]*/ BSTR sDownloadGUID, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_CancelOnlineChange
 *
 */
STDMETHODIMP CCommissioningSession::Adapt_CancelOnlineChange(long lKey, CSC_TRANSFER_TARGET target, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_DownloadDomain
 *
 */
STDMETHODIMP CCommissioningSession::Adapt_DownloadDomain(long lKey, CSC_TRANSFER_TARGET target, /*[in, string]*/ BSTR sDomainName, /*[in]*/ SAFEARRAY/*(unsigned char)*/ **ppContent, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_UploadDomain
 *
 */
STDMETHODIMP CCommissioningSession::Adapt_UploadDomain(long lKey, CSC_TRANSFER_TARGET target, /*[in, string]*/ BSTR sDomainName, /*[out, retval]*/ SAFEARRAY/*(unsigned char)*/ **ppContent, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_DeleteAllDomains
 *
 */
STDMETHODIMP CCommissioningSession::Adapt_DeleteAllDomains(long lKey, CSC_TRANSFER_TARGET target, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_DeleteDomain
 *
 */
STDMETHODIMP CCommissioningSession::Adapt_DeleteDomain(long lKey, CSC_TRANSFER_TARGET target, /*[in, string]*/ BSTR sDomainName, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_GetDomainList
 *
 */
STDMETHODIMP CCommissioningSession::Adapt_GetDomainList(long lKey, CSC_TRANSFER_TARGET target, /*[out, retval]*/ SAFEARRAY **ppListDomainNames, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_get_DLoadAttribute
 *
 */
STDMETHODIMP CCommissioningSession::Adapt_get_DLoadAttribute(/*[in]*/ long lKey, /*[in]*/ CSC_TRANSFER_TARGET src, /*[in, string]*/ BSTR sAtributeName, /*[out, retval]*/ VARIANT *pVal, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_ClearResource
 *
 */
STDMETHODIMP CCommissioningSession::Adapt_ClearResource(long lKey, CSC_TRANSFER_TARGET target, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_FlashResource
 *
 */
STDMETHODIMP CCommissioningSession::Adapt_FlashResource(/*[in]*/ long lKey, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}




/* ---------------------------------------------------------------------------- */
/**
 * Adapt_SetBreakpoint
 *
 */
STDMETHODIMP CCommissioningSession::Adapt_SetBreakpoint(long lKey, BSTR sInstanceName, long lBreakpointNumber, long *hBP, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_ClearBreakpoint
 *
 */
STDMETHODIMP CCommissioningSession::Adapt_ClearBreakpoint(long lKey, long hBP, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_ClearAllBreakpoints
 *
 */
STDMETHODIMP CCommissioningSession::Adapt_ClearAllBreakpoints(long lKey, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_ContinueTask
 *
 */
STDMETHODIMP CCommissioningSession::Adapt_ContinueTask(long lKey, BSTR sTaskName, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_SingleStep
 *
 */
STDMETHODIMP CCommissioningSession::Adapt_SingleStep(long lKey, BSTR sTaskName, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_StepOver
 *
 */
STDMETHODIMP CCommissioningSession::Adapt_StepOver(long lKey, BSTR sTaskName, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_StepOut
 *
 */
STDMETHODIMP CCommissioningSession::Adapt_StepOut(long lKey, BSTR sTaskName, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_DownloadToControl
 *
 */
HRESULT CCommissioningSession::Adapt_DownloadToControl(BSTR sDownloadGuid, BOOL *bpHandled)
{
	*bpHandled = FALSE;
	return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_Login
 *
 */
STDMETHODIMP CControlMgt::Adapt_Login(/*[in,string]*/ BSTR sUser, /*[in,string]*/ BSTR sPassword, /*[out,retval]*/ long* plKey, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_Logout
 *
 */
STDMETHODIMP CControlMgt::Adapt_Logout(/*[in]*/ long lKey, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_ColdStartResource
 *
 */
STDMETHODIMP CControlMgt::Adapt_ColdStartResource(/*[in]*/ long lKey, /*[out]*/ BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_WarmStartResource
 *
 */
STDMETHODIMP CControlMgt::Adapt_WarmStartResource(/*[in]*/ long lKey, /*[out]*/ BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_PauseResource
 *
 */
STDMETHODIMP CControlMgt::Adapt_PauseResource(/*[in]*/ long lKey, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_ContinueResource
 *
 */
STDMETHODIMP CControlMgt::Adapt_ContinueResource(/*[in]*/ long lKey, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_StartTaskOnResource
 *
 */
STDMETHODIMP CControlMgt::Adapt_StartTaskOnResource(/*[in]*/ long lKey, /*[in, string]*/ BSTR sTask, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_StopTaskOnResource
 *
 */
STDMETHODIMP CControlMgt::Adapt_StopTaskOnResource(/*[in]*/ long lKey, /*[in, string]*/ BSTR sTask, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_StartAllTasksOnResource
 *
 */
STDMETHODIMP CControlMgt::Adapt_StartAllTasksOnResource(/*[in]*/ long lKey, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt_StopAllTasksOnResource
 *
 */
STDMETHODIMP CControlMgt::Adapt_StopAllTasksOnResource(/*[in]*/ long lKey, BOOL *bpHandled)
{
	*bpHandled = FALSE;
    return S_OK;
}


/* ---------------------------------------------------------------------------- */
/**
 * CustCmd2
 *
 */
STDMETHODIMP CControlMgt::CustCmd2(/*[in]*/  SAFEARRAY/*(unsigned char)*/ **ppDataDown, /*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppDataUp)
{
    return E_NOTIMPL;
}

/* ---------------------------------------------------------------------------- */
/**
 * CustCmd3
 *
 */
STDMETHODIMP CControlMgt::CustCmd3(/*[in]*/  SAFEARRAY/*(unsigned char)*/ **ppDataDown, /*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppDataUp)
{
    return E_NOTIMPL;
}

/* ---------------------------------------------------------------------------- */
/**
 * CustCmd4
 *
 */
STDMETHODIMP CControlMgt::CustCmd4(/*[in]*/  SAFEARRAY/*(unsigned char)*/ **ppDataDown, /*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppDataUp)
{
    return E_NOTIMPL;
}

/* ---------------------------------------------------------------------------- */
/**
 * CustCmd5
 *
 */
STDMETHODIMP CControlMgt::CustCmd5(/*[in]*/  SAFEARRAY/*(unsigned char)*/ **ppDataDown, /*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppDataUp)
{
    return E_NOTIMPL;
}


/* ---------------------------------------------------------------------------- */
/**
 * Adapt_SendErrToEng
 *
 * Check if an error message should be sent to the engineering or not
 * return TRUE to send message, FALSE to suppress message.
 */
BOOL CCommObj::Adapt_SendErrToEng(UINT uErrNo)
{
    return TRUE;
}



/* ---------------------------------------------------------------------------- */

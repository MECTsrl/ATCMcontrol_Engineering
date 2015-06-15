/* $Header: /4CReleased/V2.20.00/TargetBase/inc/4CLayer.h 1     1.03.07 12:35 Gen4c $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: 4CLayer.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/inc/4CLayer.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibComm
 *
 * =CURRENT 	 $Date: 1.03.07 12:35 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _4CLAYER_H_
#define _4CLAYER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>

#ifndef _COMMPARA_H_
#include "CommPara.h"
#endif

#define WAIT_FOR_TRANSFER		500



class CLogLayer;

/**
 *  class C4CLayer
 *
 */
class _ONLCOMM C4CLayer : public CObject  
{
	DECLARE_DYNCREATE(C4CLayer)
	protected:
		C4CLayer();

	public:
		virtual ~C4CLayer();

	protected:
		CLogLayer			*m_pLog;
		SCommPara			m_Para;
		CString				m_sConnect;
		CCriticalSection	*m_pCrit;

	public:
		virtual BOOL Initialize(CLogLayer *pLog, SCommPara *pPara, LPCTSTR cszConnect);

	public:
		virtual BOOL IsConnected();
		virtual BOOL IsCommOpen();

	public:
		virtual SCommPara *GetCommPara();

	// Communication functions
	public:
		virtual BOOL OpenComm();
		virtual BOOL CloseComm();

		virtual BOOL Connect();
		virtual BOOL Disconnect();

		virtual BOOL Transfer(BYTE *bypCommand, BYTE **ppData, UINT *upLen);
		virtual BOOL Transfer(BYTE *bypCommand, LPCTSTR cszFile, BYTE **ppData, UINT *upLen);

		virtual BOOL GetVal20(BYTE *bypCommand, BYTE **ppData, UINT *upLen);
		virtual BOOL GetVal21(BYTE *bypCommand, BYTE **ppData, UINT *upLen);

	protected:
		virtual BOOL TransferData(BYTE *byCommand, BYTE **ppData, UINT *upLen);

	// Message formatting
	public:
        virtual BOOL GetMsgString(BYTE byCommand, BYTE *pData, UINT uLen, BSTR *pMessage);
        virtual CLogLayer* GetLogLayer();

	protected:
		virtual void FormatOK(BYTE byCmd, BYTE *pData, UINT uLen, CString &sRes);
		virtual void FormatContinued(BYTE byCmd, BYTE *pData, UINT uLen, CString &sRes);
		virtual void FormatError(BYTE byCmd, UINT uErrNo, BYTE *pData, UINT uLen, CString &sRes);

	// Progress bar for data transfer
	public:
		virtual void SetProgressCallBack(PROGRESS_CALLBACK fpCallBack);
		virtual void ClearProgressCallBack();

	public:
		USHORT	TtoH(USHORT uShort);
		USHORT	HtoT(USHORT uShort);
};

__int64 GetTime64();

typedef C4CLayer *(*LPCONSTRUCT4CLAYER)(LPCTSTR);
typedef VOID (*LPDISPOSE4CLAYER)(C4CLayer*);

extern "C" {
/**
 * create a completely initialized C4CLayer Object.
 * @param lpParam    optional layer-specific initialization information
 * @return           the new C4CLayer Object
 */
_ONLCOMM C4CLayer* Construct4CLayer(LPCTSTR lpParam);

/**
 * destruct a completely initialized C4CLayer Object.
 * @param lpLayer      the layer object to be deleted
 */
_ONLCOMM VOID Dispose4CLayer(C4CLayer* lpLayer);


/**
 * If this function exists, the OnlComm DLL supports progress bar callback.
 * (Just to test the existence.)
 */
_ONLCOMM BOOL SupportProgressCallBack(VOID);
}

#endif

/* ---------------------------------------------------------------------------- */

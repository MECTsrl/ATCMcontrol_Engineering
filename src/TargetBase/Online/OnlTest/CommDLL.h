/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlTest/CommDLL.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: CommDLL.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlTest/CommDLL.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlTest
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#if !defined(AFX_COMMDLL_H__FA490E9B_357B_4ABB_8E55_604ABA2F59A0__INCLUDED_)
#define AFX_COMMDLL_H__FA490E9B_357B_4ABB_8E55_604ABA2F59A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommBase.h"

#define _ONLCOMM __declspec(dllimport)

class _ONLCOMM C4CLayer;


/* CCommDLL
 * ----------------------------------------------------------------------------
 */
class CCommDLL : public CCommBase  
{
	DECLARE_DYNAMIC(CCommDLL);

	public:
		CCommDLL();
		virtual ~CCommDLL();
		
	protected:
		HMODULE		m_hModule;
		C4CLayer	*m_pLayer;
		BOOL		m_bSwap;

	public:
		BOOL	Transfer(BYTE *byCommand, BYTE **ppData, UINT *upLen);
		void	ShowError(LPCTSTR szMessage, BYTE *pData);		
		void	SetSwap(BOOL bSwap);

	public:
		virtual BYTE	*Alloc(UINT uLen);
		virtual void	Free(BYTE **ppMemory);

		virtual unsigned short		Swap16(unsigned short	uVal);
		virtual unsigned int		Swap32(unsigned int		ulVal);
		virtual unsigned __int64	Swap64(unsigned __int64	ullVal);
		
		virtual void	Load(CDialog *pDlg, LPCTSTR szFile, LPCTSTR szPara);
		virtual void	Unload(CDialog *pDlg);
		virtual void	Connect(CDialog *pDlg, LPCTSTR szPara);
		virtual void	Disconnect(CDialog *pDlg);
		virtual void	Login(CDialog *pDlg);
		virtual void	Logout(CDialog *pDlg);

		virtual USHORT	GetChildren(CDialog *pDlg, BYTE *pParent, USHORT uParent, BYTE **ppChildren, USHORT *upChild);
		virtual USHORT	GetAddress(CDialog *pDlg, BYTE *pVar, USHORT uVar, BYTE **ppAddr, USHORT *upAddr);

		virtual USHORT	GetValue(CDialog *pDlg, BYTE *pVar, USHORT uVar, BYTE **ppVal, USHORT *upVal);
		virtual USHORT	SetValue(CDialog *pDlg, BYTE *pVar, USHORT uVar);

		virtual USHORT	GetState(CDialog *pDlg, UINT *upState);

}; // CCommDLL

#endif // !defined(AFX_COMMDLL_H__FA490E9B_357B_4ABB_8E55_604ABA2F59A0__INCLUDED_)

/* ---------------------------------------------------------------------------- */

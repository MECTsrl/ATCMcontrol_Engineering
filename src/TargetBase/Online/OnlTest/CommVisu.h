/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlTest/CommVisu.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: CommVisu.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlTest/CommVisu.h $
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

#if !defined(AFX_COMMVISU_H__24CD7BC7_6152_4A07_BEF1_B1B40F69DC5C__INCLUDED_)
#define AFX_COMMVISU_H__24CD7BC7_6152_4A07_BEF1_B1B40F69DC5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommBase.h"


/* CCommVisu
 * ----------------------------------------------------------------------------
 */
class CCommVisu : public CCommBase  
{
	DECLARE_DYNAMIC(CCommVisu);

	public:
		CCommVisu();
		virtual ~CCommVisu();

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

}; // CCommVisu

#endif // !defined(AFX_COMMVISU_H__24CD7BC7_6152_4A07_BEF1_B1B40F69DC5C__INCLUDED_)

/* ---------------------------------------------------------------------------- */

/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlTest/CommBase.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: CommBase.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlTest/CommBase.h $
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

#if !defined(AFX_COMMBASE_H__F5A83AB9_395A_4E79_B09E_EC2381B0DA2A__INCLUDED_)
#define AFX_COMMBASE_H__F5A83AB9_395A_4E79_B09E_EC2381B0DA2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define OCT_NO_TYPE					0xffffffffu
#define OCT_ONLCOMM_DLL				0x00000000u
#define OCT_VISUCOMM_LIB			0x00000001u


/* CCommBase
 * ----------------------------------------------------------------------------
 */
class CCommBase : public CObject  
{
	DECLARE_DYNAMIC(CCommBase);

	private:
		CCommBase();

	public:
		CCommBase(DWORD dwType);
		virtual ~CCommBase();


	protected:
		DWORD	m_dwType;

		BOOL	m_bLoaded;
		BOOL	m_bConnected;
		BOOL	m_bLogedIn;

	public:
		DWORD	GetType();
		
		BOOL	IsLoaded();
		BOOL	IsConnected();
		BOOL	IsLogedIn();

	public:
		USHORT	AddVar(BYTE **ppData, USHORT *upLen, LPCTSTR x);

	public:
		virtual BYTE	*Alloc(UINT uLen)		= 0;
		virtual void	Free(BYTE **ppMemory)	= 0;
		
		virtual unsigned short		Swap16(unsigned short	uVal)	= 0;
		virtual unsigned int		Swap32(unsigned int		ulVal)	= 0;
		virtual unsigned __int64	Swap64(unsigned __int64	ullVal)	= 0;

		virtual void	Load(CDialog *pDlg, LPCTSTR szFile, LPCTSTR szPara)		= 0;
		virtual void	Unload(CDialog *pDlg)					= 0;
		virtual void	Connect(CDialog *pDlg, LPCTSTR szPara)	= 0;
		virtual void	Disconnect(CDialog *pDlg)				= 0;
		virtual void	Login(CDialog *pDlg)					= 0;
		virtual void	Logout(CDialog *pDlg)					= 0;
		
		virtual USHORT	GetChildren(CDialog *pDlg, BYTE *pParent, USHORT uParent, BYTE **ppChildren, USHORT *upChild)		= 0;
		virtual USHORT	GetAddress(CDialog *pDlg, BYTE *pVar, USHORT uVar, BYTE **ppAddr, USHORT *upAddr)					= 0;

		virtual USHORT	GetValue(CDialog *pDlg, BYTE *pVar, USHORT uVar, BYTE **ppVal, USHORT *upVal)		= 0;
		virtual USHORT	SetValue(CDialog *pDlg, BYTE *pVar, USHORT uVar)									= 0;

		virtual USHORT	GetState(CDialog *pDlg, UINT *upState)		= 0;

	private:
		void	Init();

}; // CCommBase

#endif // !defined(AFX_COMMBASE_H__F5A83AB9_395A_4E79_B09E_EC2381B0DA2A__INCLUDED_)

/* ---------------------------------------------------------------------------- */

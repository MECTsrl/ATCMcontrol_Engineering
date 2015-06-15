/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/inc/Hash.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Hash.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/inc/Hash.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			NCCBase
 *
 * =CURRENT 	 $Date: 28.02.07 19:04 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#if !defined(AFX_HASH_H__0FA35BA4_29D3_11D5_A05C_0050BF490551__INCLUDED_)
#define AFX_HASH_H__0FA35BA4_29D3_11D5_A05C_0050BF490551__INCLUDED_

class C__objects_hash  
{
	CWordArray		CodeBlockIndexList;	// CodeBlockIndex from "__objects.hash"
	CStringArray	CodeBlockNameList;
public:
	C__objects_hash();
	virtual ~C__objects_hash();

	void	collect(CString& strFileName);
	CString	isCodeBlock(unsigned short Index);
};

#endif

/* ---------------------------------------------------------------------------- */

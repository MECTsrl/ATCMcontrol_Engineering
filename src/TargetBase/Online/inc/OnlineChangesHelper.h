/* $Header: /4CReleased/V2.20.00/TargetBase/Online/inc/OnlineChangesHelper.h 3     13.04.07 18:48 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: OnlineChangesHelper.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/Online/inc/OnlineChangesHelper.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
 *
 * =CURRENT 	 $Date: 13.04.07 18:48 $
 *			 $Revision: 3 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef __ONLINECHANGESHELPER_H_
#define __ONLINECHANGESHELPER_H_

#include "vmSharedDef.h"


/* *.def2 column definitions
 * ----------------------------------------------------------------------------
 */
#define CSV_IDX_NAME		1		/* Member name											*/
#define CSV_IDX_DECL_SPEC	2		/* Declaration specification S,N,G,S*,N* (* == pointer) */
#define CSV_IDX_SEGMENT 	3		/* Segment if appropriate, else empty					*/
#define CSV_IDX_TYPE		4		/* BOOL, LREAL, MyType, ... 							*/
#define CSV_IDX_BYTE_OFFSET 5		/* Offset in byte of member in parent structure/segment */
#define CSV_IDX_BIT_OFFSET	6		/* Offset in bit (0-7) for BOOL variables, else empty	*/
#define CSV_IDX_BIT_SIZE	7		/* Size of member in bit								*/
#define CSV_IDX_STRLEN		8		/* Maximum number of characters if STRING[], else empty */


struct OC_Pair
{
	CString  name;
	IEC_UINT oldIndex;
	IEC_UINT oldSize;
	IEC_UINT newIndex;
	IEC_UINT newSize;
};

struct VarDecl
{
	CString   name;
	CString   type;
	IEC_UDINT bitoffset;
	IEC_UDINT bitsize;

	VarDecl(): bitoffset(0), bitsize(0) {};
};

struct CopyReg
{
	IEC_UDINT oldOffset;
	IEC_UDINT newOffset;
	IEC_UDINT size;
};

struct SOCInstCopy
{
	IEC_UINT	uOld;			/* Old index of changed data object */
	IEC_UINT	uNew;			/* New index of changed data object */
	IEC_UINT	uOldOff;		/* Byte offset of old data			*/
	IEC_UINT	uNewOff;		/* Byte offset of new data			*/
	IEC_UDINT	ulSize; 		/* Data size to be copied in bit	*/
	IEC_UINT	uOldBit;		/* Bit	offset of old data			*/
	IEC_UINT	uNewBit;		/* Bit	offset of new data			*/
};

struct SOnlineChange
{
	IEC_UINT	uCode;			/* No. of changed code objects		*/
	IEC_UINT	uData;			/* No. of changed data objects		*/
	IEC_UINT	uToCopy;		/* No. of instance regions			*/
	IEC_UINT	uDirVars;		/* No. of direct (%) variables		*/
	IEC_INT 	iCodeDiff;		/* Change in code object list		*/
	IEC_INT 	iDataDiff;		/* Change in data object list		*/
	IEC_UINT	bTaskInfo;		/* True, if task info downloaded	*/
};

typedef CList<OC_Pair, OC_Pair&>		 PairList;
typedef CList<SOCInstCopy, SOCInstCopy&> InstCopyList;
typedef CList<VarDecl, VarDecl&>		 VarDecls;
typedef CArray<VarDecls, VarDecls&> 	 VarDeclsArray;
typedef CList<CopyReg, CopyReg&>		 CopyRegs;

class COnlineChangesHelper
{
public:
	SOnlineChange	 m_OCConfig;

private:
	InstCopyList	 m_ChangesList;

	// for internal use
	CMapStringToPtr* m_pInstancesMap;
	CArray<IEC_UINT, IEC_UINT> m_map;
	CArray<CString, CString&> m_objType;
	
	VarDeclsArray	 m_newDirectVars;
	
	int 			 m_iNewObjNr;
	int 			 m_iOldObjNr;

	BOOL			 m_isInit;
	BOOL			 m_processObjectsInit;

	// for info export
	BYTE*			 m_pbBuffer;
	int 			 m_iBufferSize;

	BOOL	addIndexPair(CString& typeName, CString& instName, IEC_UINT oldIndex, IEC_UINT oldSize, 
						 IEC_UINT newIndex, IEC_UINT newSize);
	HRESULT processObjectsHash(LPCTSTR lpszObjectsHashName, IN SAFEARRAY **ppsaContent);
	HRESULT processGlobalsDef(LPCTSTR lpszObjectsHashName, IN SAFEARRAY **ppsaContent);
	HRESULT processDefFile(LPCTSTR lpszObjectsHashName, IN SAFEARRAY **ppsaContent);
	HRESULT generateClassesDiff(LPCTSTR lpszDiffName, LPDWORD lpdwDiffSize, 
							LPCTSTR lpszDomainName, IN SAFEARRAY **ppsaContent,
							IEC_UINT* pNoOfDiffs, IEC_INT* pDelta);
	HRESULT generateObjectsDiff(LPCTSTR lpszDiffName, LPDWORD lpdwDiffSize, 
							LPCTSTR lpszDomainName, int oldObjNr,
							IN SAFEARRAY **ppsaContent, int newObjNr,
							IEC_UINT* pNoOfDiffs, IEC_INT* pDelta, 
							IEC_UINT* pNoOfDirectVars);
	void		   Cleanup();
public:
	COnlineChangesHelper();
	~COnlineChangesHelper();
	HRESULT createOCConfig();
	void	destroyOCConfig();
	BYTE*	getOCConfig()	  { return m_pbBuffer;	  }
	int 	getOCConfigSize() { return m_iBufferSize; }

	BOOL	isOCConfigInit()  { return m_isInit;	  }
	HRESULT processFileOC(const CString& strDomain, IN SAFEARRAY **ppsaContent,
						  CString& strFile, LPDWORD lpdwSize);
	HRESULT startOC();
	HRESULT finishOC();
};

#endif

/* ---------------------------------------------------------------------------- */

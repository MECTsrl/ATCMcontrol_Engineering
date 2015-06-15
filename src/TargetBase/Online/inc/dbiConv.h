/* $Header: /4CReleased/V2.20.00/TargetBase/Online/inc/dbiConv.h 2     18.04.07 19:56 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: dbiConv.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/Online/inc/dbiConv.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlineLibServ
 *
 * =CURRENT 	 $Date: 18.04.07 19:56 $
 *			 $Revision: 2 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _DBICONV_H_
#define _DBICONV_H_


/* class CTokenizer
 * ----------------------------------------------------------------------------
 */
class CTokenizer : public CObject
{
	public:
		CTokenizer();

	protected:

		#define MAX_TOKEN_LEN		65535

		INT		m_iNextToken;
		TCHAR	m_szLastToken[MAX_TOKEN_LEN + 1];

	public:
		LPCTSTR	GetFirstToken(CString const &s, LPCTSTR cszDelimiter);
		LPCTSTR GetNextToken(CString const &s, LPCTSTR cszDelimiter);
		BOOL	IsLastToken(CString const &s, LPCTSTR cszDelimiter);

		INT		FindOneOf(CString const &s, LPCTSTR cszDelimiter, INT iStart) const;
		BOOL	IsOneOf(CString const &s, LPCTSTR cszDelimiter, INT iToTest) const;

}; // class CTokenizer


/* class CElem
 * ----------------------------------------------------------------------------
 */
class CElem : public CObject
{
	friend class CElemArray;

	protected:
		CElem();

	public:
		CElem(LPCTSTR szName);
		virtual ~CElem();

	public:
		CString m_sName;
		
	protected:
		CElem	*m_pNext;

}; // class CElem


/* class CElemArray
 * ----------------------------------------------------------------------------
 */
class CElemArray
{
	public:
		CElemArray();
		virtual ~CElemArray();
	
	protected:
		CElem		*m_pRoot;
		unsigned     m_uCount;

		CElem		*m_pIterate;

	public:
		UINT	 GetCount();
		
		CElem	*FindElem(LPCSTR szElem);
		
		CElem	*GetElemAt(int iIndex);
		CElem	*GetFirst();
		CElem	*GetNext();

		IEC_UINT AddElem(CElem *pElem);
		void     DeleteAllElem(void);

}; // class CElemArray


/* class CInstance
 * ----------------------------------------------------------------------------
 */
class CInstance : public CElem
{
	protected:
		CInstance();

	public:
		CInstance(LPCTSTR szName);
		virtual ~CInstance();

	public:
		CInstance *AddInstance(LPCTSTR szInst);

	public:
		virtual IEC_UINT Serialize(HANDLE hFile, UINT *upSize, UINT *upOffset);

	protected:
		CElemArray	m_aChildren;

	public:
		IEC_UINT	m_uType;
		IEC_UINT	m_uInst;
		
		IEC_UINT	m_uIndex;			/* Reference to type array			*/

}; // CInstance


/* class CVar
 * ----------------------------------------------------------------------------
 */
class CVar : public CElem
{
	protected:
		CVar();

	public:
		CVar(LPCTSTR szName);
		virtual ~CVar();

	public:
		virtual IEC_UINT Serialize(HANDLE hVar, UINT *upSize);

	public:
		IEC_USINT  m_usType;
		IEC_USINT  m_usDataType;
		IEC_USINT  m_usBit;
		IEC_UINT   m_uLen;
		IEC_UINT   m_uInst;
		IEC_UDINT  m_ulOffset;

}; // CVar


/* class CType
 * ----------------------------------------------------------------------------
 */
class CType : public CElem
{
	protected:
		CType();

	public:
		CType(LPCTSTR szName);
		virtual ~CType();

	public:
		CVar *AddVariable(LPCTSTR szName);

	public:
		virtual IEC_UINT Serialize(HANDLE hType, HANDLE hVar, UINT *upSize);

	protected:
		CElemArray m_aMember;

}; // CType


/* class CConvert
 * ----------------------------------------------------------------------------
 */
class CConvert : public CObject
{
	public:
		CConvert();
		virtual ~CConvert();

	public:
		IEC_UINT ImportProject(LPCTSTR szPath, LPCTSTR szFile);
		IEC_UINT ImportType(LPCTSTR szPath, LPCTSTR szType, IEC_UINT *upIndex);

	public:
		IEC_UINT FindType(LPCTSTR szType);
		IEC_UINT AddType(LPCTSTR szType);

	public:
		IEC_UINT Serialize(LPCTSTR szFile);

	protected:
		CInstance *m_pInstRoot;

		CObArray  *m_pTypes;

}; // class CConvert


#endif /* _DBICONV_H_ */

/* ---------------------------------------------------------------------------- */


#ifndef __CVMAUTODECL_H_
#define __CVMAUTODECL_H_

#include "resource.h" 
#include "VMAutodecl.h"
#include "VMAutoDeclDef.h" 

#define MAX_LENGTH_OF_SECT_NAME 120
#define MAX_LENGTH_OF_LINE 120
#define MAX_LENGTH_OF_ADDRESS 120
#define MAX_LENGTH_OF_VAR_TYPE 120
#define MAX_LENGTH_OF_VAR_NAME 20

//////////////////////////////////////////////////////////////////////
// Class CVMVarSection
//////////////////////////////////////////////////////////////////////

class CVMVarSection  
{
public:
	CVMVarSection(int iVarType, TCHAR* szSectName,
						int	iSeg,
						int iDataType,
						long lCount,
						int	iStringLength,
						long lAddress);
	virtual ~CVMVarSection();

	void                    Add(CVMVarSection *lpNext);
	inline CVMVarSection* GetNext() {return m_lpNext;}; 
	inline void             SetNext(CVMVarSection *lpNext) {m_lpNext=lpNext;};
  
	inline int				GetVarType(void){return m_iVarType;}
	inline TCHAR*			GetName(void){return m_szSectName;}
    inline int				GetSegment(void){return m_iSeg;}
    inline int				GetDataType(void){return m_iDataType;}
	inline long				GetCount(void){return m_lCount;}
	inline int				GetStringLength(void){return m_iStringLength;}
	inline long				GetAddress(void){return m_lAddress;}

private:  
	CVMVarSection*		m_lpNext;
	int						m_iVarType; //I, Q or M
	TCHAR					m_szSectName[MAX_LENGTH_OF_SECT_NAME];//Variable name
	int						m_iSeg;//Segment number
	int						m_iDataType;//DATA_TYPE
	long					m_lCount;//Variable count
	int						m_iStringLength;//relevant  only for STRING: string length
	long					m_lAddress;//Byte offset

};


//////////////////////////////////////////////////////////////////////
// CVMAutoDecl
//////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CVMAutoDecl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVMAutoDecl, &CLSID_VM_AutoDecl>,
	public ICG_AutoDecl
{
public:
	CVMAutoDecl();
	virtual ~CVMAutoDecl();  

DECLARE_REGISTRY_RESOURCEID(IDR_AUTODECL)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CVMAutoDecl)
	COM_INTERFACE_ENTRY(ICG_AutoDecl)
END_COM_MAP()

// ICG_AutoDecl::getOnlStringFromAddr
    STDMETHOD(getOnlStringFromAddr)( 
        BSTR sIECAddr,
        BSTR sIECType,
        BSTR* psOnlString
    );

// ICG_AutoDecl::getAddressInfo
	STDMETHOD(getAddressInfo)(
        BSTR   sID, 
        BSTR*  psIECAddr, 
        BSTR*  psIECType,
        BOOL*  pbReadOnly
   );
// ICG_AutoDecl::init
    STDMETHOD(init)(       
        BSTR  sTargetTypeName, // target type name as used by engineering
        BSTR  sConfigFile      //file and fullpath of the variable description file.
    );

private:

	CVMVarSection*	m_pVarSectionList;
	int					m_iNumberOfVarSection;

	TCHAR				m_szIECAddress[MAX_LENGTH_OF_ADDRESS];
	TCHAR				m_szIECDataType[MAX_LENGTH_OF_VAR_TYPE];
	int					m_iIECDataType;
	int					m_iVarType;

    int                 SetDataTypeString(DATA_TYPE CurrentDataType, int iStringLength);
    bool                GenerateAddress(CVMVarSection* pCurrentVarSection, long lIndex, long lSubIndex, int iByteIncrement);
	bool				InitList(TCHAR* szConfigFileName);
	bool				IsVMVar(TCHAR* szVarName);
	int					NextToken(TCHAR* pszSource, _TCHAR* lpszToken, TCHAR cSep = COMMA);	
    HRESULT             CVMAutoDecl::CheckIdentifier(TCHAR* pszIdentifier);
    inline bool			IsDigit(TCHAR tc){ 
							return (tc>=_T('0'))&&(tc<=_T('9'));}
};

#endif //__CVMAUTODECL_H_

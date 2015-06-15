
#if !defined(AFX_OBJPROPDECLBASE_H__9AD5ACC4_EC57_11D4_B8D3_002018641833__INCLUDED_)
#define AFX_OBJPROPDECLBASE_H__9AD5ACC4_EC57_11D4_B8D3_002018641833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxcmn.h>
#include "afxtempl.h"
#include "ObjProp.h"

typedef CArray<CString, CString>* pDataArrayType;

// indices of column indices in m_aiCol
// i.e. column for name has index m_aiCol[COL_NAME]
#define COL_NAME            0
#define COL_TYPE            1
#define COL_INITVAL         2
#define COL_ALIAS           3
#define COL_COMMENT         4
#define COL_STORAGECLASS    5

#define NUM_COLS            6


class CObjPropDeclBase : public CObjProp  
{
public:
	CObjPropDeclBase(DIALOGTYPE DialogType, 
                     UINT nIDTemplate, 
                     CPropertySheetBase *PropertySheet,
                     int iColumnStrId);
	virtual ~CObjPropDeclBase();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();

	SAFEARRAY * m_pMembers;
	CListCtrl	m_FbMembers;

    class CObjPropData
    {
    public:
        CArray<CString, CString> m_sStringArray;
        CArray<CString, CString>* m_pDataArray;
        
        CObjPropData & operator = (CObjPropData & other);
    };
    
    CArray<CObjPropData, CObjPropData&> m_tObjDataList;

    BOOL DoesVarNameExist(CString strVarName, int i);

    CArray<pDataArrayType, pDataArrayType> m_DataArray;


protected:
	int m_iColumnStrId;
	const TCHAR ** m_pszSClasses;
    virtual BOOL OnInitDialog();
	void OnAddMember();
	void OnModifyMember();
	void OnDeleteMember();
	void OnClickFbmembers(NMHDR* pNMHDR, LRESULT* pResult);
    void InitColumnHeaders();
    void CheckStringValue(CString& rstrValue, const CString& crstrType, BOOL bOutput);

	DECLARE_MESSAGE_MAP()

protected:
	int m_iColumns;
	BOOL m_bInit;
    int m_aiCol[NUM_COLS];
};

#endif // !defined(AFX_OBJPROPDECLBASE_H__9AD5ACC4_EC57_11D4_B8D3_002018641833__INCLUDED_)

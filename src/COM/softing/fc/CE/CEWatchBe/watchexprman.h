#ifndef __WATCHEXPRMAN_H_
#define __WATCHEXPRMAN_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CExpressionHolder;

#include "fccallback.h"
#include "watchexpr.h"
#include "DbgInfo.h"


FC_CALLBACK_DEF1 (ExpressionChangedCB, CExpressionHolder *);


class CExpressionHolder : public CExpressionNode
{
public:
    struct Range
    {
        Range ()
        : m_lLower (0),
          m_lUpper (0)
        {
        }

        Range (long lLower, long lUpper)
        : m_lLower (lLower),
          m_lUpper (lUpper)
        {
        };

        long m_lLower;
        long m_lUpper;
    };

    CExpressionHolder (CComPtr<IDebugInfo> pDebugInfo, long lHandle);
    virtual ~CExpressionHolder ();

    CExpressionNode *NewLiteral (__int64 llValue);
    CExpressionNode *NewLiteral (__int64 llValue, BL_IEC_TYP iecType);
    CExpressionNode *NewLiteral (double dblValue);
    CExpressionNode *NewLiteral (double dblValue, BL_IEC_TYP iecType);
    CExpressionNode *NewLiteral (bool bValue);
    CExpressionNode *NewLiteral (const CString &strValue);
    CExpressionNode *NewScopedName (CEExpandMode expandMode, const CString &strScope, const CString &strName, CString &strPrefix);
    CExpressionNode *NewConfiguration (const CString &strName, bool bAddress);
    CExpressionNode *NewComponent (CExpressionNode *pAggregate, const CString &strFieldName);
    CExpressionNode *NewIndexing (CExpressionNode *pArray, CExpressionNode *pIndex);
    CExpressionNode *NewConversion (CExpressionNode *pOperand, BL_IEC_TYP sourceType, BL_IEC_TYP targetType);
    CExpressionNode *NewUnaryArithmetic (CExpressionUnaryArithmetic::Arithmetic arithmetic, CExpressionNode *pOperand);
    CExpressionNode *NewUnaryLogical (CExpressionUnaryLogical::Logical logical, CExpressionNode *pOperand);
    CExpressionNode *NewBinaryArithmetic (CExpressionBinaryArithmetic::Arithmetic arithmetic, CExpressionNode *pLeftOperand, CExpressionNode *pRightOperand);
    CExpressionNode *NewFunctionalArithmetic (CExpressionBinaryArithmetic::Arithmetic arithmetic, CList<CExpressionNode *, CExpressionNode *> *pOperands);
    CExpressionNode *NewRelation (CExpressionRelation::Relation relation, CExpressionNode *pLeftOperand, CExpressionNode *pRightOperand);
    CExpressionNode *NewBinaryLogical (CExpressionBinaryLogical::Logical logical, CExpressionNode *pLeftOperand, CExpressionNode *pRightOperand);
    CExpressionNode *NewFunctionalLogical (CExpressionBinaryLogical::Logical logical, CList<CExpressionNode *, CExpressionNode *> *pOperands);
    CExpressionNode *NewPower (CExpressionNode *pLeftOperand, CExpressionNode *pRightOperand);
    CExpressionNode *GetRoot ();

    void SetChangedCB (FC_CALLBACK_BASE (ExpressionChangedCB) *pChangedCB);

    void SetText (const CString &strText, CEWatchItemState state);
    void SetExpression (const CString &strText, CExpressionNode *pRoot);
    void SetFormat (const CString &strText, BL_FormatInfo *pFormatInfo);
    void SetRanges (const CString &strText, const CArray<Range, const Range &> *pRanges);

    long GetClientData ();
    void SetClientData (long lClientData);

    long GetHandle ();
    CString GetItemText ();
    CEWatchItemState GetItemState ();
    CString GetExpressionText ();
    CEExpressionState GetExpressionState ();
    CString GetFormatText ();
    CEFormatState GetFormatState ();
    CENumberBase GetFormatBase ();
    CString GetRangesText ();
    CERangesState GetRangesState ();
    HRESULT GetRangeCount (unsigned long &ulCount);
    HRESULT GetRange (unsigned long ulIndex, Range &range);
    HRESULT GetValueText (bool bFormatted, CString &strText);

    unsigned long GetSubItemCount ();
    HRESULT GetSubItem (unsigned long ulIndex, CExpressionHolder *pSubItem);

    CEWriteState GetWriteState ();
    HRESULT Write (const CComVariant &varValue);

    bool IsConnected ();
    virtual void Connect ();
    virtual void Disconnect ();

    virtual void Evaluate ();

protected:
    CList<CExpressionNode *, CExpressionNode *> m_Nodes;

    CEWatchItemState m_ItemState;
    CEFormatState    m_FormatState;
    CERangesState    m_RangesState;

    CExpressionNode *m_pRoot;
    bool            m_bRootSet;

    FC_CALLBACK_BASE (ExpressionChangedCB) *m_pChangedCB;

    long m_lHandle;

    long m_lClientData;

    CString                      m_strItemText;
    CString                      m_strExpressionText;
    CString                      m_strFormatText;
    BL_FormatInfo                *m_pFormatInfo;
    CString                      m_strRangesText;
    CArray<Range, const Range &> *m_pRanges;

    virtual void Changed ();

private:
    void AddNode (CExpressionNode *pNode);
    void DisposeNodes (CExpressionNode *pNotMe);
    void SetFormatBase (CENumberBase base);
    unsigned long GetArraySubItemCount ();
    HRESULT GetArraySubItem (unsigned long ulIndex, CExpressionHolder *pSubItem);
    HRESULT GuessingConversion (const CComVariant &varValue, bool bFormatted, CString &strValue);
    HRESULT BaseLibToString (const CComVariant &varValue, BL_IEC_TYP iecType, bool bFormatted, CString &strValue);
    HRESULT GetScopedName (const CString &strName, const CString &strScope, CExpressionNode *&pExpression, CString &strPrefix);
    HRESULT GetExpandedName (const CString &strName, CEExpandMode expandMode, CExpressionNode *&pExpression, CString &strPrefix);
    HRESULT LookupChild (const CString &strName, const CComPtr<IUnknown> &pParent, CString &strPrefix);
    CExpressionNode *BuildPrefixedExpression (const CString &strName, const CString &strPrefix);
    HRESULT GetTypeDBIForScope (const CString &strScope, CComPtr<ITypeDBI> &pTypeDBI);
    HRESULT GetProjectDBI (CComPtr<IProjectDBI> &pProjectDBI);
    HRESULT GetDBIItemInfo (const CComPtr<IUnknown> &pDBI, CString &strName, unsigned long &ulChildrenCount);
    HRESULT GetChildDBI (CComPtr<IUnknown> pDBI, unsigned long ulIndex, CComPtr<IUnknown> &pChildDBI);
    bool CheckAddress(const CString & strName);

};


class CExpressionFactory
{
public:
    CExpressionFactory ();

    CExpressionHolder *NewHolder (CComPtr<IDebugInfo> pDebugInfo);

    CExpressionNode *NewLiteral (CComPtr<IDebugInfo> pDebugInfo, __int64 llValue);
    CExpressionNode *NewLiteral (CComPtr<IDebugInfo> pDebugInfo, __int64 llValue, BL_IEC_TYP iecType);
    CExpressionNode *NewLiteral (CComPtr<IDebugInfo> pDebugInfo, double dblValue);
    CExpressionNode *NewLiteral (CComPtr<IDebugInfo> pDebugInfo, double dblValue, BL_IEC_TYP iecType);
    CExpressionNode *NewLiteral (CComPtr<IDebugInfo> pDebugInfo, bool bValue);
    CExpressionNode *NewLiteral (CComPtr<IDebugInfo> pDebugInfo, const CString &strValue);
    CExpressionNode *NewConfiguration (CComPtr<IDebugInfo> pDebugInfo, const CString &strName, bool bAddress);
    CExpressionNode *NewComponent (CComPtr<IDebugInfo> pDebugInfo, CExpressionNode *pAggregate, const CString &strFieldName);
    CExpressionNode *NewIndexing (CComPtr<IDebugInfo> pDebugInfo, CExpressionNode *pArray, CExpressionNode *pIndex);
    CExpressionNode *NewConversion (CComPtr<IDebugInfo> pDebugInfo, CExpressionNode *pOperand, BL_IEC_TYP sourceType, BL_IEC_TYP targetType);
    CExpressionNode *NewUnaryArithmetic (CComPtr<IDebugInfo> pDebugInfo, CExpressionUnaryArithmetic::Arithmetic, CExpressionNode *pOperand);
    CExpressionNode *NewUnaryLogical (CComPtr<IDebugInfo> pDebugInfo, CExpressionUnaryLogical::Logical logical, CExpressionNode *pOperand);
    CExpressionNode *NewBinaryArithmetic (CComPtr<IDebugInfo> pDebugInfo, CExpressionBinaryArithmetic::Arithmetic arithmetic, CExpressionNode *pLeftOperand, CExpressionNode *pRightOperand);
    CExpressionNode *NewRelation (CComPtr<IDebugInfo> pDebugInfo, CExpressionRelation::Relation relation, CExpressionNode *pLeftOperand, CExpressionNode *pRightOperand);
    CExpressionNode *NewBinaryLogical (CComPtr<IDebugInfo> pDebugInfo, CExpressionBinaryLogical::Logical logical, CExpressionNode *pLeftOperand, CExpressionNode *pRightOperand);
    CExpressionNode *NewPower (CComPtr<IDebugInfo> pDebugInfo, CExpressionNode *pLeftOperand, CExpressionNode *pRightOperand);

protected:
    long m_lNextHandle;
};


#endif // __WATCHEXPRMAN_H_


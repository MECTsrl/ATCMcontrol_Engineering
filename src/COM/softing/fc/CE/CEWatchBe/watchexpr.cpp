

//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


#include "stdafx.h"
#include <math.h>
#include "watchexpr.h"
#include "subscription.h"
#include "cesysdef.h"
#include "baselib.h"
#include "cewatchbe.h"
#include "cewatchbeCP.h"
#include "DbgInfo.h"

extern CSubscriptionManager *CEGetSubscriptionManager ();
extern void CEAddDelayedAdapt (CExpressionNode *pExpression);
extern void CERemoveDelayedAdapt (CExpressionNode *pExpression);
extern CCESessionManager *CEGetSessionManager ();


CExpressionNode::CExpressionNode (CComPtr<IDebugInfo> pDebugInfo)
: m_bCommunicated (false),
  m_State (CEEmptyValue),
  m_pType (NULL),
  m_uiConnectCount (0),
  m_bModified (false),
  m_SubscriptionModifiedCB (this, OnSubscriptionModified),
  m_sQuality (OPC_QUALITY_GOOD),
  m_Timestamp (0),
  m_pSubscription (NULL),
  m_pDebugInfo (pDebugInfo)
{
}


CExpressionNode::~CExpressionNode ()
{
    // ED, 4.3.2000: somehow it can happen that m_uiConnectCount == 0 and
    // m_pSubscription != NULL; I dont know what this means for the design
    // of this component, but the following lines seem to fix some problems that
    // were very hard to track down. (Symptoms: like Pure virtual function calls...)

    if (m_pSubscription != NULL)
    {
        m_pSubscription->UnregisterModifiedCB (&m_SubscriptionModifiedCB);
        m_pSubscription->Unreference ();
        m_pSubscription = NULL;
    };

    CERemoveDelayedAdapt (this);

    ASSERT (m_uiConnectCount == 0);
}


CComPtr<IDebugInfo> CExpressionNode::GetDebugInfo ()
{
    return (m_pDebugInfo);
}


void CExpressionNode::BuildType (BL_IEC_TYP iecType)
{
    HRESULT hr;
    CComPtr<IUnknown> pUnknown;

    ASSERT (m_pDebugInfo != NULL);
    if (m_pDebugInfo == NULL)
    {
        return;
    };

    hr = m_pDebugInfo->GetSimpleType ((short)iecType, (IUnknown **)&pUnknown);
    if (FAILED (hr))
    {
        return;
    };
    ASSERT (pUnknown != NULL);
    if (pUnknown == NULL)
    {
        return;
    };

    hr = pUnknown->QueryInterface (IID_ITypeDBI, (LPVOID *)&m_pType);
    if (FAILED (hr))
    {
        m_pType = NULL;
        return;
    };

    ASSERT (m_pType != NULL);
}


bool CExpressionNode::IsCommunicated ()
{
    return (m_bCommunicated);
}

void CExpressionNode::SetCommunicated(bool bComm)
{
    m_bCommunicated = bComm;
}

CSubscription *CExpressionNode::GetSubscription ()
{
    return (m_pSubscription);
}


CComPtr<ITypeDBI> CExpressionNode::GetType ()
{
    return (m_pType);
}


CEExpressionState CExpressionNode::GetState ()
{
    return (m_State);
}


CComVariant CExpressionNode::GetValue ()
{
    return (m_varValue);
}


short CExpressionNode::GetQuality ()
{
    return (m_sQuality);
}


DATE CExpressionNode::GetTimestamp ()
{
    return (m_Timestamp);
}


void CExpressionNode::AddParent (CExpressionNode *pParent)
{
    m_Parents.AddTail (pParent);
}


void CExpressionNode::RemoveParent (CExpressionNode *pParent)
{
    POSITION pPosition;

    pPosition = m_Parents.Find (pParent);
    if (pPosition != NULL)
    {
        m_Parents.RemoveAt (pPosition);

        if (m_Parents.IsEmpty ())
        {
            delete this;
        };
    };
}


void CExpressionNode::Connect ()
{
    ++m_uiConnectCount;
    Adapt ();
}


void CExpressionNode::Disconnect ()
{
    ASSERT (m_uiConnectCount != 0);
    if (m_uiConnectCount == 0)
    {
        return;
    };

    --m_uiConnectCount;
    Adapt ();
}


#ifndef DEBUG
void CExpressionNode::Evaluate ()
{
    SetState (CEEmptyValue);
    SetValue (CComVariant ());
}
#endif


void CExpressionNode::Adapt ()
{
    if (!PropagateFailures ())
    {
        Evaluate ();
    };

    // Assume an expression x[i].y where i changes its value.
    // Value change is propagated via i's Changed method to
    // x[i]'s Adapt method which as x[i] is not communicated
    // does not change any of x[i]'s properties. Thus x[i]'s
    // Adapt method will not call Changed and x[i].a's Adapt
    // method is not called. This means x[i].a is never re-
    // subscribed and seems to never change its value.
    // We solve this problem by calling x[i]'s Changed method
    // whenever x[i] is adapted, x[i] is not communicated and
    // some parent of x[i] (e.g. x[i].y) could be communicated.
    // If x[i] is communicated x[i]'s Adapt method will always
    // change x[i]'s state property to CEReloadingValue and
    // x[i]'s Changed method is called anyway.

    if (m_bModified || !m_bCommunicated && ParentMaybeCommunicated ())
    {
        m_bModified = false;
        Changed ();
    };
}


bool CExpressionNode::ParentMaybeCommunicated ()
{

    // Most expression nodes cannot have parents
    // that change communicated state from false
    // to true. Exceptions are e.g. indexing or
    // component reference.

    return (false);
}


bool CExpressionNode::PropagateFailures ()
{
    return (false);
}


bool CExpressionNode::AllowPropagation (CExpressionNode *pChild)
{

    // Generally children failures are propagated to parents.
    // If some parent does not like propagation of states to
    // itself it has to override this method and return false.

    return (true);
}


void CExpressionNode::SynthesizeQuality ()
{
}


void CExpressionNode::SynthesizeTimestamp ()
{
}


void CExpressionNode::Changed ()
{
    POSITION pPosition;

    pPosition = m_Parents.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CExpressionNode *pNode;

        pNode = m_Parents.GetNext (pPosition);
        pNode->Adapt ();
    };
}


bool CExpressionNode::FetchCommunicatedValue ()
{
    bool bMatch=false;
    CComVariant varValue;
    BL_IEC_TYP iecType;
    HRESULT hr;

    if (m_pSubscription == NULL)
    {
        if (m_varValue.vt == VT_EMPTY)
        {
            SetState (CEEmptyValue);
            return (false);
        };

        return (true);
    };

    if (!m_pSubscription->HaveData ())
    {
        SetState (CEEmptyValue);
        return (false);
    };

    varValue = m_pSubscription->GetVariantValue ();
    if (varValue.vt == VT_EMPTY)
    {
        SetState (CEEmptyValue);
        return (false);
    };


    SetValue (varValue);
    SetQuality (m_pSubscription->GetQuality ());
    SetTimestamp (m_pSubscription->GetTimestamp ());


    hr = TransformCommunicatedValue (m_varValue);
    if (hr != S_OK)
    {
        SetState (CEEmptyValue);
        return (false);
    };


    // If there is no expected type matching succeeds always.

    if (m_pType == NULL)
    {
        return (true);
    };


    // If quality is bad matching succeeds always.

    if ((m_sQuality & OPC_QUALITY_MASK) == OPC_QUALITY_BAD)
    {
        return (true);
    };


    // Get the expressions expected simple type. It is
    // an error for an expression which has a value to
    // have some non-simple expected type.

    if (!IsSimpleType (iecType))
    {
        ASSERT (false);
        SetState (CEValueTypeError);
        return (false);
    };


    // Check value type expected from IEC type against actual value type

    if (!CheckValueType (iecType, m_varValue.vt))
    {
        SetState (CEValueTypeError);
        return (false);
    };

    return (true);
}


bool CExpressionNode::CheckValueType (BL_IEC_TYP iecType, VARTYPE valueType)
{
    bool bMatch;

    switch (iecType)
    {
    case BL_BOOL:
        bMatch = valueType == VT_BOOL;
        break;

    case BL_BYTE:
        bMatch = valueType == VT_I1 || valueType == VT_I2 || valueType == VT_I4;
        break;

    case BL_WORD:
        bMatch = valueType == VT_I2 || valueType == VT_I4;
        break;

    case BL_DWORD:
        bMatch = valueType == VT_I4;
        break;

    case BL_LWORD:
        bMatch = valueType == VT_DECIMAL;
        break;

    case BL_INT:
        bMatch = valueType == VT_I2 || valueType == VT_I4;
        break;

    case BL_DINT:
        bMatch = valueType == VT_I4;
        break;

    case BL_LINT:
        bMatch = valueType == VT_DECIMAL;
        break;

    case BL_REAL:
        bMatch = valueType == VT_R4 || valueType == VT_R8;
        break;

    case BL_LREAL:
        bMatch = valueType == VT_R8;
        break;

    case BL_TIME:
        bMatch = valueType == VT_DECIMAL;
        break;

    case BL_DT:
        bMatch = valueType == VT_DECIMAL;
        break;

    case BL_DATE:
        bMatch = valueType == VT_I4;
        break;

    case BL_TOD:
        bMatch = valueType == VT_I4;
        break;

    case BL_STRING:
    case BL_WSTRING:
        bMatch = valueType == VT_BSTR;
        break;

    case BL_UINT:
        bMatch = valueType == VT_I4;
        break;

    case BL_UDINT:
        bMatch = valueType == VT_I4;
        break;

    case BL_ULINT:
        bMatch = valueType == VT_DECIMAL;
        break;
    };

    return (bMatch);
}


void CExpressionNode::SetState (CEExpressionState state)
{
    if (state != m_State)
    {
        SetModified ();
    };

    m_State = state;
}


void CExpressionNode::SetValue (const CComVariant &varValue)
{

    // CComVariant does not support comparison of VT_DECIMAL variants

    if (varValue.vt == VT_DECIMAL || m_varValue.vt == VT_DECIMAL)
    {
        if (varValue.vt != m_varValue.vt ||
            varValue.decVal.signscale != m_varValue.decVal.signscale ||
            varValue.decVal.Hi32 != m_varValue.decVal.Hi32 ||
            varValue.decVal.Lo64 != m_varValue.decVal.Lo64)
        {
            SetModified ();
        };
    }
    else
    {
        if (varValue != m_varValue)
        {
            SetModified ();
        };
    };

    m_varValue = varValue;
}


void CExpressionNode::SetQuality (short sQuality)
{
    if (sQuality != m_sQuality)
    {
        SetModified ();
    };

    m_sQuality = sQuality;
}


void CExpressionNode::SetTimestamp (DATE timestamp)
{
    if (timestamp != m_Timestamp)
    {
        SetModified ();
    };

    m_Timestamp = timestamp;
}


void CExpressionNode::SetModified ()
{
    m_bModified = true;
}


bool CExpressionNode::IsSimpleType (BL_IEC_TYP &iecType)
{
    HRESULT hr;
    DBITypeKind kind;
    short sIECType;

    iecType = BL_ANY;

    ASSERT (m_pType != NULL);
    if (m_pType == NULL)
    {
        return (false);
    };

    hr = m_pType->get_Kind (&kind);
    if (hr != S_OK || kind != DBISimpleType)
    {
        return (false);
    };

    hr = m_pType->get_IECType (&sIECType);
    if (hr != S_OK)
    {
        return (true);
    };

    iecType = (BL_IEC_TYP)sIECType;
    return (true);
}


bool CExpressionNode::IsArrayType (CComPtr<ITypeDBI> &pElementType, long &lLowerBound, long &lUpperBound)
{
    HRESULT hr;
    DBITypeKind kind;
    CComPtr<IUnknown> pUnknown;

    pElementType = NULL;
    lLowerBound = 0;
    lUpperBound = 0;

    ASSERT (m_pType != NULL);
    if (m_pType == NULL)
    {
        return (false);
    };

    hr = m_pType->get_Kind (&kind);
    if (hr != S_OK || kind != DBIArrayType)
    {
        return (false);
    };

    hr = m_pType->get_ElementType ((IUnknown **)&pUnknown);
    if (hr == S_OK)
    {
        hr = pUnknown->QueryInterface (IID_ITypeDBI, (LPVOID *)&pElementType);
        if (FAILED (hr))
        {
            pElementType = NULL;
        };
    };

    hr = m_pType->get_LowerBound (&lLowerBound);
    if (hr != S_OK)
    {
        lLowerBound = 0;
    };

    hr = m_pType->get_UpperBound (&lUpperBound);
    if (hr != S_OK)
    {
        lUpperBound = 0;
    };

    return (true);
}


bool CExpressionNode::CheckIntegerType ()
{
    BL_IEC_TYP iecType;

    // Be optimistic. Debug info could have been not available.
    if (m_pType == NULL)
    {
        return (true);
    };

    if (!IsSimpleType (iecType))
    {
        return (false);
    };

    switch (iecType)
    {
    //case BL_SINT:
    case BL_INT: case BL_DINT: case BL_LINT:
    //case BL_USINT: 
    case BL_UINT: case BL_UDINT: case BL_ULINT:
        return (true);
    };

    return (false);
}


bool CExpressionNode::CheckRealType ()
{
    BL_IEC_TYP iecType;

    // Be optimistic. Debug info could have been not available.
    if (m_pType == NULL)
    {
        return (true);
    };

    if (!IsSimpleType (iecType))
    {
        return (false);
    };

    switch (iecType)
    {
    case BL_REAL:
    case BL_LREAL:
        return (true);
    };

    return (false);
}


bool CExpressionNode::CheckBoolType ()
{
    BL_IEC_TYP iecType;

    // Be optimistic. Debug info could have been not available.
    if (m_pType == NULL)
    {
        return (true);
    };

    if (!IsSimpleType (iecType))
    {
        return (false);
    };

    switch (iecType)
    {
    case BL_BOOL:
        return (true);
    };

    return (false);
}


bool CExpressionNode::CheckBitType ()
{
    BL_IEC_TYP iecType;

    // Be optimistic. Debug info could have been not available.
    if (m_pType == NULL)
    {
        return (true);
    };

    if (!IsSimpleType (iecType))
    {
        return (false);
    };

    switch (iecType)
    {
    case BL_BYTE: case BL_WORD: case BL_DWORD: case BL_LWORD:
        return (true);
    };

    return (false);
}


bool CExpressionNode::CheckDateTimeType (bool bAllowTime, bool bAllowDate, bool bAllowTimeOfDay, bool bAllowDateTime)
{
    BL_IEC_TYP iecType;

    // Be optimistic. Debug info could have been not available.
    if (m_pType == NULL)
    {
        return (true);
    };

    if (!IsSimpleType (iecType))
    {
        return (false);
    };

    switch (iecType)
    {
    case BL_TIME: return (bAllowTime);
    case BL_DATE: return (bAllowDate);
    case BL_TOD : return (bAllowTimeOfDay);
    case BL_DT  : return (bAllowDateTime);
    };

    return (false);
}


bool CExpressionNode::SetupCommunication ()
{
    CString strName;
    CSubscriptionManager *pSubscriptionManager;
    HRESULT hr;
    bool bResubscribed=false;

    if (m_uiConnectCount == 0)
    {
        if (m_pSubscription != NULL)
        {
            m_pSubscription->UnregisterModifiedCB (&m_SubscriptionModifiedCB);
            m_pSubscription->Unreference ();
            m_pSubscription = NULL;
        };

        return (true);
    };

    strName = BuildSubscriptionPath ();
    ASSERT (!strName.IsEmpty ());
    if (strName.IsEmpty ())
    {
        SetState (CESubscribeError);
        return (false);
    };

    if (m_pSubscription != NULL)
    {
        if (strName != m_pSubscription->GetObject ())
        {
            bResubscribed = true;

            m_pSubscription->UnregisterModifiedCB (&m_SubscriptionModifiedCB);
            m_pSubscription->Unreference ();
            m_pSubscription = NULL;
        };
    };

    if (m_pSubscription == NULL)
    {
        pSubscriptionManager = CEGetSubscriptionManager ();
        ASSERT (pSubscriptionManager != NULL);
        if (pSubscriptionManager == NULL)
        {
            SetState (CESubscribeError);
            return (false);
        };

        hr = pSubscriptionManager->GetSubscription (strName, BL_ANY, m_pSubscription);
        if (FAILED (hr))
        {
            SetState (CESubscribeError);
            return (false);
        };
        ASSERT (m_pSubscription != NULL);
        if (m_pSubscription == NULL)
        {
            SetState (CESubscribeError);
            return (false);
        };

        m_pSubscription->RegisterModifiedCB (&m_SubscriptionModifiedCB);
    };

    if (!m_pSubscription->IsKnown ())
    {
        SetState (CEUnknownSubscription);
        return (false);
    };

    if (!m_pSubscription->HaveData ())
    {
        SetState (bResubscribed ? CEReloadingValue : CELoadingValue);
        return (false);
    };

    return (true);
}


CString CExpressionNode::BuildSubscriptionPath ()
{
    ASSERT (false);
    return ("");
}


HRESULT CExpressionNode::TransformCommunicatedValue (CComVariant &varValue)
{
    return (S_OK);
}


void CExpressionNode::OnSubscriptionModified (CSubscription *pSubscription)
{

    // Subscription could have become valid by connecting
    // the variable session. We get to know about that by
    // a call of this method.

    CEAddDelayedAdapt (this);
}


void CExpressionNode::OnDelayedAdapt ()
{
    Adapt ();
}


__int64 CExpressionNode::GetInt64 (const CComVariant &varValue)
{
    __int64 llValue;

    switch (varValue.vt)
    {
    case VT_I1: llValue = varValue.bVal; break;
    case VT_I2: llValue = varValue.iVal; break;
    case VT_I4: llValue = varValue.lVal; break;

    case VT_DECIMAL:
        llValue = varValue.decVal.Lo64;
        if (varValue.decVal.sign & DECIMAL_NEG)
        {
            llValue = -llValue;
        };
        break;

    default:
        ASSERT (false);
        llValue = 0;
        break;
    };

    return (llValue);
}


double CExpressionNode::GetDouble (const CComVariant &varValue)
{
    double dblValue;

    switch (varValue.vt)
    {
    case VT_R4: dblValue = varValue.fltVal; break;
    case VT_R8: dblValue = varValue.dblVal; break;

    default:
        ASSERT (false);
        dblValue = 0.0;
        break;
    };

    return (dblValue);
}


bool CExpressionNode::GetBool (const CComVariant &varValue)
{
    bool bValue;

    switch (varValue.vt)
    {
    case VT_BOOL: bValue = varValue.boolVal != 0; break;

    default:
        ASSERT (false);
        bValue = false;
        break;
    };

    return (bValue);
}


void CExpressionNode::SetInt64 (__int64 llValue, CComVariant &varValue)
{
    BL_IEC_TYP iecType=BL_LINT;

    if (m_pType != NULL && IsSimpleType (iecType))
    {
        ASSERT (iecType == BL_BYTE ||
                iecType == BL_WORD ||
                iecType == BL_DWORD ||
                iecType == BL_LWORD ||
                iecType == BL_INT ||
                iecType == BL_DINT ||
                iecType == BL_LINT ||
                iecType == BL_TIME ||
                iecType == BL_DATE ||
                iecType == BL_TOD ||
                iecType == BL_DT ||
                iecType == BL_UINT ||
                iecType == BL_UDINT ||
                iecType == BL_ULINT
             );
    };

    switch (iecType)
    {
    case BL_BYTE:
        varValue = (char)llValue;
        break;

    case BL_INT:
    case BL_WORD:
        varValue = (short)llValue;
        break;

    case BL_DINT:
    case BL_DWORD:
    case BL_UINT:
    case BL_TOD:
    case BL_DATE:
        varValue = (long)llValue;
        break;

    case BL_LINT:
    case BL_LWORD:
    case BL_ULINT:
    case BL_TIME:
    case BL_DT:
        varValue.vt = VT_DECIMAL;
        varValue.decVal.sign = llValue < 0 ? DECIMAL_NEG : 0;
        varValue.decVal.scale = 0;
        varValue.decVal.Hi32 = 0;
        varValue.decVal.Lo64 = llValue < 0 ? -llValue : llValue;
        break;
    case BL_UDINT: //HACK: nobody will ever understand this. The only solution is 
                   //to put all the constant folding into a clever class and put it into the compiler...
        varValue.vt = VT_DECIMAL;
        varValue.decVal.sign = 0;
        varValue.decVal.scale = 0;
        varValue.decVal.Hi32 = 0;
        varValue.decVal.Lo64 = (unsigned long)llValue;
        break;
    };
}


void CExpressionNode::SetDouble (double dblValue, CComVariant &varValue)
{
    BL_IEC_TYP iecType;

    if (m_pType != NULL && IsSimpleType (iecType))
    {
        ASSERT (iecType == BL_LREAL || iecType == BL_REAL);
    };

    varValue = dblValue;
}


void CExpressionNode::SetBool (bool bValue, CComVariant &varValue)
{
    BL_IEC_TYP iecType;

    if (m_pType != NULL && IsSimpleType (iecType))
    {
        ASSERT (iecType == BL_BOOL);
    };

    varValue = bValue;
}


CExpressionLeaf::CExpressionLeaf (CComPtr<IDebugInfo> pDebugInfo, CComPtr<ITypeDBI> pType)
: CExpressionNode (pDebugInfo)
{
    m_pType = pType;
}


CExpressionUnary::CExpressionUnary (CComPtr<IDebugInfo> pDebugInfo, CExpressionNode *pChild)
: CExpressionNode (pDebugInfo),
  m_pChild (pChild)
{
    pChild->AddParent (this);
}


CExpressionUnary::~CExpressionUnary ()
{
    m_pChild->RemoveParent (this);
}


CExpressionNode *CExpressionUnary::GetChild ()
{
    return (m_pChild);
}


void CExpressionUnary::Connect ()
{
    m_pChild->Connect ();
    CExpressionNode::Connect ();
}


void CExpressionUnary::Disconnect ()
{
    CExpressionNode::Disconnect ();
    m_pChild->Disconnect ();
}


bool CExpressionUnary::PropagateFailures ()
{
    bool bFailures=false;
    CEExpressionState state;

    if (CExpressionNode::PropagateFailures ())
    {
        bFailures = true;
    };

    if (AllowPropagation (m_pChild))
    {
        state = m_pChild->GetState ();
        if (state != CEGoodExpression)
        {
            SetState (state);
            bFailures = true;
        };
    };

    return (bFailures);
}


void CExpressionUnary::SynthesizeQuality ()
{
    SetQuality (m_pChild->GetQuality ());
}


void CExpressionUnary::SynthesizeTimestamp ()
{
    SetTimestamp (m_pChild->GetTimestamp ());
}


CExpressionBinary::CExpressionBinary (CComPtr<IDebugInfo> pDebugInfo, CExpressionNode *pLeftChild, CExpressionNode *pRightChild)
: CExpressionNode (pDebugInfo),
  m_pLeftChild (pLeftChild),
  m_pRightChild (pRightChild)
{
    pLeftChild->AddParent (this);
    pRightChild->AddParent (this);
}


CExpressionBinary::~CExpressionBinary ()
{
    m_pLeftChild->RemoveParent (this);
    m_pRightChild->RemoveParent (this);
}


CExpressionNode *CExpressionBinary::GetLeftChild ()
{
    return (m_pLeftChild);
}


CExpressionNode *CExpressionBinary::GetRightChild ()
{
    return (m_pRightChild);
}


void CExpressionBinary::Connect ()
{
    m_pLeftChild->Connect ();
    m_pRightChild->Connect ();
    CExpressionNode::Connect ();
}


void CExpressionBinary::Disconnect ()
{
    CExpressionNode::Disconnect ();
    m_pLeftChild->Disconnect ();
    m_pRightChild->Disconnect ();
}


bool CExpressionBinary::PropagateFailures ()
{
    bool bFailures=false;
    CEExpressionState leftState=CEGoodExpression;
    CEExpressionState rightState=CEGoodExpression;
    unsigned int uiLeftSeverity;
    unsigned int uiRightSeverity;
    CEExpressionState state;

    if (CExpressionNode::PropagateFailures ())
    {
        bFailures = true;
    };

    if (AllowPropagation (m_pLeftChild))
    {
        leftState = m_pLeftChild->GetState ();
    };

    if (AllowPropagation (m_pRightChild))
    {
        rightState = m_pRightChild->GetState ();
    };

    uiLeftSeverity = GetStateSeverity (leftState);
    uiRightSeverity = GetStateSeverity (rightState);

    state = uiLeftSeverity > uiRightSeverity ? leftState : rightState;

    if (state != CEGoodExpression)
    {
        SetState (state);
        bFailures = true;
    };

    return (bFailures);
}


unsigned int CExpressionBinary::GetStateSeverity (CEExpressionState state)
{
    unsigned int uiSeverity;

    switch (state)
    {
    case CEGoodExpression:
        uiSeverity = 0;
        break;

    case CEEmptyValue:
    case CELoadingValue:
    case CEReloadingValue:
        uiSeverity = 1;
        break;

    case CESubscribeError:
    case CEUnknownSubscription:
        uiSeverity = 2;
        break;

    default:
        uiSeverity = 3;
        break;
    };

    return (uiSeverity);
}


void CExpressionBinary::SynthesizeQuality ()
{
    short sLeftQuality;
    short sRightQuality;
    short sQuality;

    sLeftQuality = m_pLeftChild->GetQuality ();
    sRightQuality = m_pRightChild->GetQuality ();

    if ((sLeftQuality & OPC_QUALITY_MASK) == OPC_QUALITY_BAD)
    {
        sQuality = sLeftQuality;
    }
    else if ((sRightQuality & OPC_QUALITY_MASK) == OPC_QUALITY_BAD)
    {
        sQuality = sRightQuality;
    }
    else if ((sLeftQuality & OPC_QUALITY_MASK) == OPC_QUALITY_UNCERTAIN)
    {
        sQuality = sLeftQuality;
    }
    else if ((sRightQuality & OPC_QUALITY_MASK) == OPC_QUALITY_UNCERTAIN)
    {
        sQuality = sRightQuality;
    }
    else
    {
        sQuality = sLeftQuality;
    };

    SetQuality (sQuality);
}


void CExpressionBinary::SynthesizeTimestamp ()
{
    DATE leftTimestamp;
    DATE rightTimestamp;

    leftTimestamp = m_pLeftChild->GetTimestamp ();
    rightTimestamp = m_pRightChild->GetTimestamp ();

    SetTimestamp (leftTimestamp > rightTimestamp ? leftTimestamp : rightTimestamp);
}


CExpressionLiteral::CExpressionLiteral (CComPtr<IDebugInfo> pDebugInfo, __int64 llValue, BL_IEC_TYP iecType)
: CExpressionLeaf (pDebugInfo),
  m_bUntypedInteger (false),
  m_bUntypedReal (false)
{
    ASSERT (iecType == BL_INT  ||
            iecType == BL_DINT  ||
            iecType == BL_LINT  ||
            iecType == BL_BYTE  ||
            iecType == BL_WORD  ||
            iecType == BL_DWORD ||
            iecType == BL_LWORD ||
            iecType == BL_TIME ||
            iecType == BL_DATE ||
            iecType == BL_TOD ||
            iecType == BL_DT ||
            iecType == BL_UINT ||
            iecType == BL_UDINT ||
            iecType == BL_ULINT
            );

    BuildType (iecType);
    m_State = CEGoodExpression;
    SetInt64 (llValue, m_varValue);
}


CExpressionLiteral::CExpressionLiteral (CComPtr<IDebugInfo> pDebugInfo, __int64 llValue)
: CExpressionLeaf (pDebugInfo),
  m_bUntypedInteger (true)
{
    m_State = CEGoodExpression;
    SetInt64 (llValue, m_varValue);
}


CExpressionLiteral::CExpressionLiteral (CComPtr<IDebugInfo> pDebugInfo, double dblValue, BL_IEC_TYP iecType)
: CExpressionLeaf (pDebugInfo),
  m_bUntypedInteger (false),
  m_bUntypedReal (false)
{
    ASSERT (iecType == BL_LREAL || iecType == BL_REAL);

    BuildType (iecType);
    m_State = CEGoodExpression;
    m_varValue = dblValue;
}


CExpressionLiteral::CExpressionLiteral (CComPtr<IDebugInfo> pDebugInfo, double dblValue)
: CExpressionLeaf (pDebugInfo),
  m_bUntypedInteger (false),
  m_bUntypedReal (true)
{
    m_State = CEGoodExpression;
    m_varValue = dblValue;
}


CExpressionLiteral::CExpressionLiteral (CComPtr<IDebugInfo> pDebugInfo, bool bValue)
: CExpressionLeaf (pDebugInfo),
  m_bUntypedInteger (false),
  m_bUntypedReal (false)
{
    BuildType (BL_BOOL);
    m_State = CEGoodExpression;
    m_varValue = bValue;
}


CExpressionLiteral::CExpressionLiteral (CComPtr<IDebugInfo> pDebugInfo, const CString &strValue)
: CExpressionLeaf (pDebugInfo),
  m_bUntypedInteger (false),
  m_bUntypedReal (false)
{
    BuildType (BL_WSTRING);
    m_State = CEGoodExpression;
    m_varValue = strValue;
}


bool CExpressionLiteral::CheckIntegerType ()
{
    if (m_bUntypedInteger)
    {
        return (true);
    };

    return (CExpressionNode::CheckIntegerType ());
}


bool CExpressionLiteral::CheckRealType ()
{
    if (m_bUntypedReal)
    {
        return (true);
    };

    return (CExpressionNode::CheckRealType ());
}


void CExpressionLiteral::Evaluate ()
{
}


CExpressionConfiguration::CExpressionConfiguration (CComPtr<IDebugInfo> pDebugInfo, const CString &strName, bool bAddress)
: CExpressionLeaf (pDebugInfo),
  m_pDBI (NULL),
  m_pSession (NULL),
  m_SessionModifiedCB (this, OnSessionModified)
{
    CCESessionManager *pSessionManager;

    if (pDebugInfo != NULL)
    {
        ReadDebugInfo (strName, pDebugInfo);


        // If debug info did not tell us a data type
        // we use a constructed configuration data type.

        if (m_pType == NULL)
        {
            HRESULT hr;
            CComPtr<IUnknown> pUnknown;

            hr = pDebugInfo->GetConfigurationType ((IUnknown **)&pUnknown);
            ASSERT (SUCCEEDED (hr));
            if (SUCCEEDED (hr))
            {
                ASSERT (pUnknown != NULL);
                if (pUnknown != NULL)
                {
                    hr = pUnknown->QueryInterface (IID_ITypeDBI, (LPVOID *)&m_pType);
                    ASSERT (SUCCEEDED (hr));
                    if (FAILED (hr))
                    {
                        m_pType = NULL;
                    };
                };
            };
        };
    };

    pSessionManager = CEGetSessionManager ();
    if (pSessionManager != NULL && !m_strMachineAddress.IsEmpty ())
    {
        HRESULT hr;

        hr = pSessionManager->GetSession (m_strMachineAddress, m_pSession);

        if (SUCCEEDED (hr))
        {
            ASSERT (m_pSession != NULL);
            if (m_pSession != NULL)
            {
                m_pSession->RegisterModifiedCB (&m_SessionModifiedCB);

                if (!m_pSession->IsOpen ())
                {
                    hr = m_pSession->Open ();
                };
            };
        };
    };
}


CExpressionConfiguration::~CExpressionConfiguration ()
{
    if (m_pSession != NULL)
    {
        m_pSession->UnregisterModifiedCB (&m_SessionModifiedCB);
        m_pSession->Unreference ();
        m_pSession = NULL;
    };
}


void CExpressionConfiguration::Evaluate ()
{
    CSC_SESSION_STATE state;

    if (m_pSession == NULL)
    {
        SetState (CENotConnected);
        return;
    };

    state = m_pSession->GetState ();
    if (state != RW_established && state != exclusive_established)
    {
        SetState (CENotConnected);
        return;
    };

    SetState (CEGoodExpression);
    SetValue (CComVariant ());
}


CString CExpressionConfiguration::GetName ()
{
    return (m_strName);
}


CComPtr<IConfigurationDBI> CExpressionConfiguration::GetDBI ()
{
    return (m_pDBI);
}


CString CExpressionConfiguration::BuildSubscriptionPath ()
{
    CString strPath;

    if (m_strMachineAddress.IsEmpty ())
    {
        return ("");
    };

    if(m_strMachineAddress[0] == _T('{')) {
        strPath.Format ("%s", (LPCTSTR)m_strMachineAddress);
        return strPath;
    }
    strPath.Format ("{%s}", (LPCTSTR)m_strMachineAddress);
    return (strPath);
}


void CExpressionConfiguration::OnSessionModified (CCESession *pSession, bool bDebug)
{
    if (!bDebug)
    {
        CEAddDelayedAdapt (this);
    };
}


void CExpressionConfiguration::ReadDebugInfo (const CString &strName, CComPtr<IDebugInfo> pDebugInfo)
{
    HRESULT hr;
    CComPtr<IUnknown> pUnknown;
    CComQIPtr<IProjectDBI> pProjectDBI;
    CComBSTR s;

    hr = pDebugInfo->get_Project ((IUnknown **)&pUnknown);
    if (hr != S_OK)
    {
        return;
    };
    ASSERT (pUnknown != NULL);
    if (pUnknown == NULL)
    {
        return;
    };

    pProjectDBI = pUnknown;
    ASSERT (pProjectDBI != NULL);
    if (pProjectDBI == NULL)
    {
        return;
    };

    pUnknown = NULL;
    //try first with the name
    hr = pProjectDBI->get_ConfigurationByName (CComBSTR (strName), (IUnknown **)&pUnknown);
    if (FAILED (hr)) {
        return;
    };

    if(hr == S_FALSE) {
        //it's not the configuration name
        // 2. try with the address
        hr = pProjectDBI->get_ConfigurationByAddress (CComBSTR (strName), (IUnknown **)&pUnknown);
        if(hr == S_FALSE) {
            //it's not the address
            //3. try with machine e.g. target instance name
            hr = pProjectDBI->get_ConfigurationByMachine (CComBSTR (strName), (IUnknown **)&pUnknown);
            if(hr != S_OK) {
                //failed, it's not the configuration name, target name or address
                return;
            }
        }

    }        

    if(!pUnknown) {
        return;
    }
    
    hr = pUnknown->QueryInterface (IID_IConfigurationDBI, (LPVOID *)&m_pDBI);
    if (FAILED (hr))
    {
        m_pDBI = NULL;
        return;
    };

    ASSERT (m_pDBI != NULL);
    if (m_pDBI == NULL)
    {
        return;
    };

    pUnknown = NULL;
    hr = m_pDBI->get_Type ((IUnknown **)&pUnknown);
    if (hr == S_OK) {
        ASSERT (pUnknown != NULL);
        if (pUnknown != NULL) {
            hr = pUnknown->QueryInterface (IID_ITypeDBI, (LPVOID *)&m_pType);
            pUnknown = NULL;
            ASSERT (SUCCEEDED (hr));
            if (FAILED (hr)) {
                m_pType = NULL;
            }
        }
    }

    //set all the data from the debug info manager
    hr = m_pDBI->get_Name (&s);
    if (hr == S_OK) {
        m_strName = s;
    }

    hr = m_pDBI->get_MachineName (&s);
    if (hr == S_OK){
        m_strMachineName = s;
    }

    hr = m_pDBI->get_MachineAddress (&s);
    if (hr == S_OK) {
        m_strMachineAddress = s;
    }
}


CExpressionComponent::CExpressionComponent (CComPtr<IDebugInfo> pDebugInfo, CExpressionNode *pAggregate, const CString &strFieldName)
: CExpressionUnary (pDebugInfo, pAggregate),
  m_strFieldName (strFieldName),
  m_bGoodAggregateType (true),
  m_bGoodField (true),
  m_bInOut (false),
  m_bConstant (false),
  m_SpecialAccess (AccessNormal),
  m_lStepNumber (-1),
  m_lActionNumber (-1)
{
    CExpressionConfiguration *pConfiguration;

    m_bCommunicated = true;

    if ((pConfiguration = dynamic_cast<CExpressionConfiguration *> (pAggregate)) != NULL)
    {
        CComPtr<IConfigurationDBI> pConfigurationDBI;

        pConfigurationDBI = pConfiguration->GetDBI ();
        if (pConfigurationDBI != NULL)
        {
            GetResourceField (pConfigurationDBI);
        };

        m_bCommunicated = false;
    }
    else
    {
        GetNonResourceField ();

        if (m_pType != NULL)
        {
            BL_IEC_TYP iecType;

            if (!IsSimpleType (iecType))
            {
                m_bCommunicated = false;
            };
        };

        if (m_bConstant)
        {
            m_bCommunicated = false;
        };
    };


    //SU: Check communication flag - only the last component shall have m_bCommunicated set to true
    //it may be possible that the type info of components is not available and it is not a
    //constant. Hence ensure that previous components do not have the m_bCommunicated flag
    //set.
    if(pAggregate) {
        if(pAggregate->IsCommunicated()) {
            pAggregate->SetCommunicated(false);
        }
    }

}


void CExpressionComponent::GetResourceField (CComPtr<IConfigurationDBI> pConfigurationDBI)
{
    HRESULT hr;
    CComPtr<IUnknown> pUnknown;
    CComPtr<IResourceDBI> pResourceDBI;

    hr = pConfigurationDBI->get_ResourceByName (CComBSTR (m_strFieldName), (IUnknown **)&pUnknown);
    if (hr != S_OK)
    {
        // Cannot check field existance for predefined resources
        // like __srt0, __system, __console. But if IEC resource name
        // is used, we enforce strict conformance to the debug info...
        if (0 != m_strFieldName.Find("__"))
             m_bGoodField = false;
        return;
    };
    ASSERT (pUnknown != NULL);
    if (pUnknown == NULL)
    {
        return;
    };

    hr = pUnknown->QueryInterface (IID_IResourceDBI, (LPVOID *)&pResourceDBI);
    pUnknown = NULL;
    if (FAILED (hr))
    {
        pResourceDBI = NULL;
        return;
    };
    ASSERT (pResourceDBI != NULL);
    if (pResourceDBI == NULL)
    {
        return;
    };

    hr = pResourceDBI->get_Type ((IUnknown **)&pUnknown);
    if (hr != S_OK)
    {
        return;
    };

    hr = pUnknown->QueryInterface (IID_ITypeDBI, (LPVOID *)&m_pType);
    pUnknown = NULL;
    if (FAILED (hr))
    {
        m_pType = NULL;
        return;
    };
    ASSERT (m_pType != NULL);
}


void CExpressionComponent::GetNonResourceField ()
{
    HRESULT hr;
    CComPtr<ITypeDBI> pAggregateType;
    DBITypeKind kind;

    pAggregateType = m_pChild->GetType ();
    if (pAggregateType == NULL)
    {
        if (m_strFieldName.Find("__") == 0)
        {
            CExpressionComponent *pChild = dynamic_cast<CExpressionComponent *> (m_pChild);
            if (pChild != NULL)
                pChild->m_bGoodField = true; //HACK to fix "PC1.resource1.task1.__state"
        }
        return;
    };

    hr = pAggregateType->get_Kind (&kind);
    if (hr != S_OK)
    {
        return;
    };

    switch (kind)
    {
    case DBIResourceType:
    case DBIFunctionBlockType:
    case DBIProgramType:
    case DBIStructType:
       hr = GetVariableField ();
        if (hr == S_FALSE)
        {
            hr = GetStepField ();
            if (hr == S_FALSE)
            {
                hr = GetActionField ();
            };
        };
        if (hr != S_OK)
        {
            m_bGoodField = false;
            return;
        };
        break;

    case DBIStepType:
        hr = GetVariableField ();
        if (hr != S_OK)
        {
            return;
        };

        if (m_strFieldName.CompareNoCase (SFCHelper::StepFieldX) == 0)
        {
            m_SpecialAccess = AccessStepX;
        };
        break;

    case DBIActionType:
        hr = GetVariableField ();
        if (hr != S_OK)
        {
            return;
        };

        if (m_strFieldName.CompareNoCase (SFCHelper::ActionFieldA) == 0)
        {
            m_SpecialAccess = AccessActionA;
        };
        break;

    default:
        m_bGoodAggregateType = false;
        return;
    };
}


HRESULT CExpressionComponent::GetVariableField ()
{
    HRESULT hr;
    CComPtr<ITypeDBI> pAggregateType;
    DBITypeKind kind;
    CComPtr<IUnknown> pUnknown;
    CComPtr<IVariableDBI> pVariableDBI;
    DBIVariableKind variableKind;

    pAggregateType = m_pChild->GetType ();
    if (pAggregateType == NULL)
    {
        return (S_FALSE);
    };

    hr = pAggregateType->get_Kind (&kind);
    if (hr != S_OK)
    {
        return (hr);
    };

    hr = pAggregateType->get_VariableByName (CComBSTR (m_strFieldName), (IUnknown **)&pUnknown);
    if (hr != S_OK)
    {

        // SFC control variables are never present in debug info.
        // Thus they are good even if not found in debug info.

        if (m_strFieldName.Find("__") == 0 || //accept tags like "PC1.resource1.__system.resourcestate"
            m_strFieldName.CompareNoCase (SFCHelper::VisName) == 0 ||
            m_strFieldName.CompareNoCase (SFCHelper::ManualModeName) == 0 ||
            m_strFieldName.CompareNoCase (SFCHelper::DoNextName) == 0 ||
            m_strFieldName.CompareNoCase (SFCHelper::OnceForcedActionsName) == 0 ||
            m_strFieldName.CompareNoCase (SFCHelper::ForcedActionsName) == 0 ||
            m_strFieldName.CompareNoCase (SFCHelper::BlockedActionsName) == 0 ||
            m_strFieldName.CompareNoCase (SFCHelper::AllActionsOffName) == 0 ||
            m_strFieldName.CompareNoCase (SFCHelper::EnabledTransitionsName) == 0 ||
            m_strFieldName.CompareNoCase (SFCHelper::OnceForcedTransitionsName) == 0 ||
            m_strFieldName.CompareNoCase (SFCHelper::ForcedTransitionsName) == 0 ||
            m_strFieldName.CompareNoCase (SFCHelper::BlockedTransitionsName) == 0 ||
            m_strFieldName.CompareNoCase (SFCHelper::AllTransitionsOnName) == 0 ||
            m_strFieldName.CompareNoCase (SFCHelper::ManCmdName) == 0)
        {
            return (S_OK);
        };

        return (hr);
    };
    ASSERT (pUnknown != NULL);
    if (pUnknown == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = pUnknown->QueryInterface (IID_IVariableDBI, (LPVOID *)&pVariableDBI);
    pUnknown = NULL;
    if (FAILED (hr))
    {
        return (E_UNEXPECTED);
    };
    ASSERT (pVariableDBI != NULL);
    if (pVariableDBI == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = pVariableDBI->get_Type ((IUnknown **)&pUnknown);
    if (FAILED (hr))
    {
        return (hr);
    };
    if (hr != S_OK)
    {
        return (S_OK);
    };
    ASSERT (pUnknown != NULL);
    if (pUnknown == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = pUnknown->QueryInterface (IID_ITypeDBI, (LPVOID *)&m_pType);
    pUnknown = NULL;
    if (FAILED (hr))
    {
        m_pType = NULL;
        return (E_UNEXPECTED);
    };
    ASSERT (m_pType != NULL);
    if (m_pType == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = pVariableDBI->get_Kind (&variableKind);
    if (FAILED (hr))
    {
        return (hr);
    };

    BSTR sOnlString = NULL;
    hr = pVariableDBI->get_OnlString(&sOnlString);
    if (FAILED (hr))
    {
        return (hr);
    };
    if(sOnlString)
    {
        m_strOnlString = sOnlString;
        SysFreeString(sOnlString);
    }
    else
    {
        m_strOnlString = "";
    }
    
    if (hr != S_OK)
    {
        return (E_UNEXPECTED);
    };

    if (variableKind == DBIConst)
    {
        CComVariant varValue;

        m_bConstant = true;

        hr = pVariableDBI->get_Value (&varValue);
        if (FAILED (hr))
        {
            return (hr);
        };
        if (hr != S_OK)
        {
            return (E_UNEXPECTED);
        };

        m_varConstantValue = varValue;
    }
    else if (variableKind == DBIVarInOut)
    {
        m_bInOut = true;
    };

    return (S_OK);
}


HRESULT CExpressionComponent::GetStepField ()
{
    HRESULT hr;
    CComPtr<ITypeDBI> pAggregateType;
    DBITypeKind kind;
    CComPtr<IUnknown> pUnknown;
    CComPtr<IStepDBI> pStepDBI;

    pAggregateType = m_pChild->GetType ();
    if (pAggregateType == NULL)
    {
        return (S_FALSE);
    };

    hr = pAggregateType->get_Kind (&kind);
    if (hr != S_OK)
    {
        return (hr);
    };

    hr = pAggregateType->get_StepByName (CComBSTR (m_strFieldName), (IUnknown **)&pUnknown);
    if (hr != S_OK)
    {
        return (hr);
    };
    ASSERT (pUnknown != NULL);
    if (pUnknown == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = pUnknown->QueryInterface (IID_IStepDBI, (LPVOID *)&pStepDBI);
    pUnknown = NULL;
    if (FAILED (hr))
    {
        return (E_UNEXPECTED);
    };
    ASSERT (pStepDBI != NULL);
    if (pStepDBI == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = pStepDBI->get_Number (&m_lStepNumber);
    if (FAILED (hr))
    {
        return (hr);
    };
    if (hr != S_OK)
    {
        return (E_UNEXPECTED);
    };

    hr = pStepDBI->get_Type ((IUnknown **)&pUnknown);
    if (FAILED (hr))
    {
        return (hr);
    };
    if (hr != S_OK)
    {
        return (S_OK);
    };
    ASSERT (pUnknown != NULL);
    if (pUnknown == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = pUnknown->QueryInterface (IID_ITypeDBI, (LPVOID *)&m_pType);
    pUnknown = NULL;
    if (FAILED (hr))
    {
        m_pType = NULL;
        return (E_UNEXPECTED);
    };
    ASSERT (m_pType != NULL);
    if (m_pType == NULL)
    {
        return (E_UNEXPECTED);
    };

    return (S_OK);
}


HRESULT CExpressionComponent::GetActionField ()
{
    HRESULT hr;
    CComPtr<ITypeDBI> pAggregateType;
    DBITypeKind kind;
    CComPtr<IUnknown> pUnknown;
    CComPtr<IActionDBI> pActionDBI;

    pAggregateType = m_pChild->GetType ();
    if (pAggregateType == NULL)
    {
        return (S_FALSE);
    };

    hr = pAggregateType->get_Kind (&kind);
    if (hr != S_OK)
    {
        return (hr);
    };

    hr = pAggregateType->get_ActionByName (CComBSTR (m_strFieldName), (IUnknown **)&pUnknown);
    if (hr != S_OK)
    {
        return (hr);
    };
    ASSERT (pUnknown != NULL);
    if (pUnknown == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = pUnknown->QueryInterface (IID_IActionDBI, (LPVOID *)&pActionDBI);
    pUnknown = NULL;
    if (FAILED (hr))
    {
        return (E_UNEXPECTED);
    };
    ASSERT (pActionDBI != NULL);
    if (pActionDBI == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = pActionDBI->get_Number (&m_lActionNumber);
    if (FAILED (hr))
    {
        return (hr);
    };
    if (hr != S_OK)
    {
        return (E_UNEXPECTED);
    };

    hr = pActionDBI->get_Type ((IUnknown **)&pUnknown);
    if (FAILED (hr))
    {
        return (hr);
    };
    if (hr != S_OK)
    {
        return (S_OK);
    };
    ASSERT (pUnknown != NULL);
    if (pUnknown == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = pUnknown->QueryInterface (IID_ITypeDBI, (LPVOID *)&m_pType);
    pUnknown = NULL;
    if (FAILED (hr))
    {
        m_pType = NULL;
        return (E_UNEXPECTED);
    };
    ASSERT (m_pType != NULL);
    if (m_pType == NULL)
    {
        return (E_UNEXPECTED);
    };

    return (S_OK);
}


CString CExpressionComponent::GetFieldName ()
{
    return (m_strFieldName);
}


long CExpressionComponent::GetStepNumber ()
{
    return (m_lStepNumber);
}


long CExpressionComponent::GetActionNumber ()
{
    return (m_lActionNumber);
}


bool CExpressionComponent::AllowPropagation (CExpressionNode *pChild)
{

    // Children that try subscribing at CSC_Online and fail
    // in name validation have state CEUnknownSubscription.
    // CExpressionComponent nodes that try subscribing at
    // CSC_Online may extend their childs invalid subscrip-
    // tion name to some valid subscription name and thus
    // do not want CEUnknownSubscription state to be propa-
    // gated to themselves.

    if (pChild->GetState () == CEUnknownSubscription)
    {
        return (false);
    };


    // Children that try subscribing at CSC_Online and fail
    // in name validation receive a notification with empty
    // value on disconnect of variable session (although
    // they never receive notifications with non-empty value).
    // For the moment do not propagate CEEmptyValue state if
    // we are communicated ourselves. Better solution would
    // be to never get notifications.

    if (pChild->GetState () == CEEmptyValue)
    {
        return (false);
    };

    return (true);
}


void CExpressionComponent::Evaluate ()
{
    if (!m_bGoodAggregateType)
    {
        SetState (CEInvalidAggregateType);
        return;
    };

    if (!m_bGoodField)
    {
        SetState (CEUnknownField);
        return;
    };

    if (m_bInOut)
    {
        SetState (CEContainsInOut);
        return;
    };

    if (m_bCommunicated)
    {
        if (!SetupCommunication ())
        {
            return;
        };

        if (!FetchCommunicatedValue ())
        {
            return;
        };
    };

    if (m_bConstant)
    {
        m_varValue = m_varConstantValue;
    };

    SetState (CEGoodExpression);
}


bool CExpressionComponent::ParentMaybeCommunicated ()
{
    // x[i].y could be not communicated but x[i].y.a could be.
    return (true);
}


CString CExpressionComponent::BuildSubscriptionPath ()
{
    CString strAggregatePath;
    CString strFieldName;
    CString strPath;

    strAggregatePath = m_pChild->BuildSubscriptionPath ();
    if (strAggregatePath.IsEmpty ())
    {
        return ("");
    };


    if (m_SpecialAccess == AccessNormal)
    {
        strFieldName = m_strOnlString.IsEmpty() ? m_strFieldName : m_strOnlString;
    }
    else
    {
        int iPos;

        strFieldName = m_strFieldName;
        // Aggregate expression is a step or action expression.
        // We have to strip the step or action part of the path
        // and append the __sfcvis variable name which contains
        // information about all steps and actions in the POU.

        iPos = strAggregatePath.ReverseFind (_T ('.'));
        if (iPos != -1)
        {
            strAggregatePath = strAggregatePath.Left (iPos);
            strFieldName = SFCHelper::VisName;
        };
    };

    strPath.Format ("%s.%s", (LPCTSTR)strAggregatePath, (LPCTSTR)strFieldName);
    return (strPath);
}


HRESULT CExpressionComponent::TransformCommunicatedValue (CComVariant &varValue)
{
    CExpressionComponent *pChild;

    switch (m_SpecialAccess)
    {
    case AccessNormal:
        break;

    case AccessStepX:
        pChild = dynamic_cast<CExpressionComponent *> (m_pChild);
        ASSERT (pChild != NULL);
        if (pChild == NULL)
        {
            return (E_UNEXPECTED);
        };

        ASSERT (varValue.vt == VT_BSTR);
        if (varValue.vt != VT_BSTR)
        {
            return (E_UNEXPECTED);
        };

        varValue = SFCHelper::IsStepInVis (varValue.bstrVal, pChild->GetStepNumber ());
        break;

    case AccessActionA:
        pChild = dynamic_cast<CExpressionComponent *> (m_pChild);
        ASSERT (pChild != NULL);
        if (pChild == NULL)
        {
            return (E_UNEXPECTED);
        };

        ASSERT (varValue.vt == VT_BSTR);
        if (varValue.vt != VT_BSTR)
        {
            return (E_UNEXPECTED);
        };

        varValue = SFCHelper::IsActionInVis (varValue.bstrVal, pChild->GetActionNumber ());
        break;

    default:
        ASSERT (false);
        return (E_UNEXPECTED);
    };

    return (S_OK);
}


CExpressionIndexing::CExpressionIndexing (CComPtr<IDebugInfo> pDebugInfo, CExpressionNode *pArray, CExpressionNode *pIndex)
: CExpressionBinary (pDebugInfo, pArray, pIndex),
  m_bGoodArrayType (true),
  m_bGoodElementType (true),
  m_bGoodIndexType (true)
{
    CComPtr<ITypeDBI> pChildType;

    m_bCommunicated = true;

    if (pArray->GetType () != NULL)
    {
        CComPtr<ITypeDBI> pElementType;
        long lLowerBound;
        long lUpperBound;
        BL_IEC_TYP iecType;

        if (!pArray->IsArrayType (pElementType, lLowerBound, lUpperBound))
        {
            m_bCommunicated = false;
            m_bGoodArrayType = false;
            return;
        };

        m_pType = pElementType;
        if (m_pType == NULL)
        {
            m_bCommunicated = false;
            m_bGoodElementType = false;
            return;
        };

        if (!IsSimpleType (iecType))
        {
            m_bCommunicated = false;
        };
    };

    if (!pIndex->CheckIntegerType ())
    {
        m_bGoodIndexType = false;
    };
}


bool CExpressionIndexing::AllowPropagation (CExpressionNode *pChild)
{

    if (pChild != m_pLeftChild)
    {
        return (true);
    };


    // Children that try subscribing at CSC_Online and fail
    // in name validation have state CEUnknownSubscription.
    // CExpressionIndexing nodes that try subscribing at
    // CSC_Online may extend their left childs invalid sub-
    // scription name to some valid subscription name and
    // thus do not want CEUnknownSubscription state to be
    // propagated from their left child to themselves.

    if (pChild->GetState () == CEUnknownSubscription)
    {
        return (false);
    };


    // Children that try subscribing at CSC_Online and fail
    // in name validation receive a notification with empty
    // value on disconnect of variable session (although
    // they never receive notifications with non-empty value).
    // For the moment do not propagate CEEmptyValue state if
    // we are communicated ourselves. Better solution would
    // be to never get notifications.

    if (pChild->GetState () == CEEmptyValue)
    {
        return (false);
    };

    return (true);
}


void CExpressionIndexing::Evaluate ()
{
    long lBase0Index;

    if (!m_bGoodArrayType)
    {
        SetState (CENotArrayType);
        return;
    };

    if (!m_bGoodElementType)
    {
        SetState (CEUnknownElementType);
        return;
    };

    if (!m_bGoodIndexType)
    {
        SetState (CEInvalidIndexType);
        return;
    };

    if (!CheckIndex (lBase0Index))
    {
        SetState (CEIndexRangeError);
        return;
    };

    if (m_bCommunicated)
    {
        if (!SetupCommunication ())
        {
            return;
        };

        if (!FetchCommunicatedValue ())
        {
            return;
        };
    };

    SetState (CEGoodExpression);
}


bool CExpressionIndexing::ParentMaybeCommunicated ()
{
    // x[i] could be not communicated but x[i].y could be.
    return (true);
}


bool CExpressionIndexing::CheckIndex (long &lBase0Index)
{
    __int64 llIndex;
    CComPtr<ITypeDBI> pElementType;
    long lLowerBound;
    long lUpperBound;

    llIndex = GetInt64 (m_pRightChild->GetValue ());
    if ((__int64)(long)llIndex != llIndex)
    {
        return (false);
    };

    if (m_pLeftChild->GetType () == NULL ||
        !m_pLeftChild->IsArrayType (pElementType, lLowerBound, lUpperBound))
    {
        lBase0Index = (long)llIndex;
        return (true);
    };

    if ((long)llIndex < lLowerBound || (long)llIndex > lUpperBound)
    {
        return (false);
    };

    lBase0Index = (long)llIndex - lLowerBound;
    return (true);
}


CString CExpressionIndexing::BuildSubscriptionPath ()
{
    CString strArrayPath;
    long lIndex;
    CString strPath;

    strArrayPath = (LPCTSTR)m_pLeftChild->BuildSubscriptionPath ();
    if (strArrayPath.IsEmpty ())
    {
        return ("");
    };

    if (!CheckIndex (lIndex))
    {
        return ("");
    };

    strPath.Format ("%s.%ld", (LPCTSTR)strArrayPath, lIndex);
    return (strPath);
}


CExpressionConversion::CExpressionConversion (CComPtr<IDebugInfo> pDebugInfo, CExpressionNode *pOperand, BL_IEC_TYP sourceType, BL_IEC_TYP targetType)
: CExpressionUnary (pDebugInfo, pOperand),
  m_SourceType (sourceType),
  m_TargetType (targetType),
  m_bGoodOperandType (true),
  m_bGoodTargetType (true)
{
    if (pOperand->GetType () != NULL)
    {
        BL_IEC_TYP iecType;

        if (!pOperand->IsSimpleType (iecType))
        {
            m_bGoodOperandType = false;
        }
        else
        {
            if (iecType != sourceType)
            {
                m_bGoodOperandType = false;
            }
            else
            {
                switch (sourceType)
                {
                //case BL_SINT: 
                //case BL_USINT: 
                case BL_UINT: case BL_UDINT: case BL_ULINT:
                case BL_INT: case BL_DINT: case BL_LINT:
                case BL_BYTE: case BL_WORD: case BL_DWORD: case BL_LWORD:
                case BL_TIME: case BL_DATE: case BL_TOD: case BL_DT:
                    m_EvaluationKind = IntegerEvaluation;
                    break;

                case BL_REAL:
                case BL_LREAL:
                    m_EvaluationKind = RealEvaluation;
                    break;

                default:
                    ASSERT (false);
                    m_bGoodOperandType = false;
                };
            };
        };
    };

    switch (targetType)
    {
    //case BL_SINT: 
    //case BL_USINT: 
    case BL_UINT: case BL_UDINT: case BL_ULINT:
    case BL_INT: case BL_DINT: case BL_LINT:
    case BL_BYTE: case BL_WORD: case BL_DWORD: case BL_LWORD:
    case BL_TIME: case BL_DATE: case BL_TOD: case BL_DT:
    case BL_REAL:
    case BL_LREAL:
        break;

    default:
        ASSERT (false);
        m_bGoodTargetType = false;
    };

    if (m_bGoodTargetType)
    {
        BuildType (targetType);
    };
}


void CExpressionConversion::Evaluate ()
{
    CComVariant varValue;
    CComVariant varResult;

    if (!m_bGoodOperandType)
    {
        SetState (CEInvalidConvOperandType);
        return;
    };

    if (!m_bGoodTargetType)
    {
        SetState (CEInvalidConvTargetType);
        return;
    };

    varValue = m_pChild->GetValue ();

    if (m_EvaluationKind == IntegerEvaluation)
    {
        __int64 llValue;

        switch (varValue.vt)
        {
        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL:
            break;

        default:
            SetState (CEInvalidConvOperandType);
            return;
        };

        llValue = GetInt64 (varValue);

        switch (m_TargetType)
        {
        //case BL_SINT: case BL_USINT:
        case BL_BYTE:
            SetInt64 (llValue & 0xff, varResult);
            break;

        case BL_UINT: case BL_INT: case BL_WORD:
            SetInt64 (llValue & 0xffff, varResult);
            break;

        case BL_UDINT: case BL_DINT: case BL_DWORD: case BL_DATE: case BL_TOD:
            SetInt64 (llValue & 0xffffffff, varResult);
            break;

        case BL_ULINT: case BL_LINT: case BL_LWORD: case BL_TIME: case BL_DT:
            SetInt64 (llValue, varResult);
            break;

        case BL_REAL:
        case BL_LREAL:
            SetDouble ((double)llValue, varResult);
            break;
        };
    }
    else if (m_EvaluationKind == RealEvaluation)
    {
        double dblValue;

        switch (varValue.vt)
        {
        case VT_R4: case VT_R8:
            break;

        default:
            SetState (CEInvalidConvOperandType);
            return;
        };

        dblValue = GetDouble (varValue);

        switch (m_TargetType)
        {
        case BL_REAL:
        case BL_LREAL:
            SetDouble (dblValue, varResult);
            break;

        //case BL_SINT: case BL_USINT:
        case BL_BYTE:
            SetInt64 ((__int8)floor (dblValue + 0.5), varResult);
            break;

        case BL_UINT: case BL_INT: case BL_WORD:
            SetInt64 ((__int16)floor (dblValue + 0.5), varResult);
            break;

        case BL_UDINT: case BL_DINT: case BL_DWORD:
            SetInt64 ((__int32)floor (dblValue + 0.5), varResult);
            break;

        case BL_ULINT: case BL_LINT: case BL_LWORD:
            SetInt64 ((__int64)floor (dblValue + 0.5), varResult);
            break;
        };
    };

    SetState (CEGoodExpression);
    SetValue (varResult);
    SynthesizeQuality ();
    SynthesizeTimestamp ();
}


CExpressionUnaryArithmetic::CExpressionUnaryArithmetic (CComPtr<IDebugInfo> pDebugInfo, Arithmetic arithmetic, CExpressionNode *pOperand)
: CExpressionUnary (pDebugInfo, pOperand),
  m_Arithmetic (arithmetic),
  m_EvaluationKind (GuessingEvaluation),
  m_bGoodOperandType (false)
{
    if (pOperand->CheckIntegerType () || pOperand->CheckRealType ())
    {
        m_bGoodOperandType = true;

        if (pOperand->GetType () != NULL)
        {
            BL_IEC_TYP iecType;

            if (!pOperand->IsSimpleType (iecType))
            {
                ASSERT (false);
            }
            else
            {
                MakeEvaluationKind (iecType);
            };
        };
    };

    if (m_bGoodOperandType)
    {
        m_pType = pOperand->GetType ();
    };
}


void CExpressionUnaryArithmetic::Evaluate ()
{
    CComVariant varValue;
    EvaluationKind kind;
    __int64 llResult;
    double dblResult;
    CComVariant varResult;

    if (!m_bGoodOperandType)
    {
        SetTypeError ();
        return;
    };

    varValue = m_pChild->GetValue ();

    kind = m_EvaluationKind;
    if (kind == GuessingEvaluation)
    {
        switch (varValue.vt)
        {
        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL:
            kind = IntegerEvaluation;
            break;

        case VT_R4: case VT_R8:
            kind = RealEvaluation;
            break;

        default:
            SetTypeError ();
            return;
        };
    };

    switch (kind)
    {
    case IntegerEvaluation:
        switch (varValue.vt)
        {
        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL: break;

        default:
            SetTypeError ();
            return;
        };

        EvaluateInteger (llResult, GetInt64 (varValue));
        SetInt64 (llResult, varResult);
        break;

    case RealEvaluation:
        switch (varValue.vt)
        {
        case VT_R4: case VT_R8: break;

        default:
            SetTypeError ();
            return;
        };

        EvaluateReal (dblResult, GetDouble (varValue));
        SetDouble (dblResult, varResult);
        break;
    };

    SetState (CEGoodExpression);
    SetValue (varResult);
    SynthesizeQuality ();
    SynthesizeTimestamp ();
}


void CExpressionUnaryArithmetic::MakeEvaluationKind (BL_IEC_TYP iecType)
{
    EvaluationKind kind;

    switch (iecType)
    {
    //case BL_SINT: 
    //case BL_USINT: 
    case BL_UINT: case BL_UDINT: case BL_ULINT:
    case BL_INT: case BL_DINT: case BL_LINT:
        kind = IntegerEvaluation;
        break;

    case BL_REAL:
    case BL_LREAL:
        kind = RealEvaluation;
        break;

    default:
        ASSERT (false);
        kind = GuessingEvaluation;
        break;
    };

    m_EvaluationKind = kind;
}


void CExpressionUnaryArithmetic::SetTypeError ()
{
    switch (m_Arithmetic)
    {
    case Plus : SetState (CEInvalidPlusOperandType);  break;
    case Minus: SetState (CEInvalidMinusOperandType); break;
    };
}


void CExpressionUnaryArithmetic::EvaluateInteger (__int64 &llResult, __int64 llOperand)
{
    switch (m_Arithmetic)
    {
    case Plus:
        llResult = llOperand;
        break;

    case Minus:
        llResult = -llOperand;
        break;
    };
}


void CExpressionUnaryArithmetic::EvaluateReal (double &dblResult, double dblOperand)
{
    switch (m_Arithmetic)
    {
    case Plus:
        dblResult = dblOperand;
        break;

    case Minus:
        dblResult = -dblOperand;
        break;
    };
}


CExpressionUnaryLogical::CExpressionUnaryLogical (CComPtr<IDebugInfo> pDebugInfo, Logical logical, CExpressionNode *pOperand)
: CExpressionUnary (pDebugInfo, pOperand),
  m_Logical (logical),
  m_EvaluationKind (GuessingEvaluation),
  m_bGoodOperandType (false)
{
    if (pOperand->CheckBitType () || pOperand->CheckBoolType ())
    {
        m_bGoodOperandType = true;

        if (pOperand->GetType () != NULL)
        {
            BL_IEC_TYP iecType;

            if (!pOperand->IsSimpleType (iecType))
            {
                ASSERT (false);
            }
            else
            {
                MakeEvaluationKind (iecType);
            };
        };
    };

    if (m_bGoodOperandType)
    {
        m_pType = pOperand->GetType ();
    };
}


void CExpressionUnaryLogical::Evaluate ()
{
    CComVariant varValue;
    EvaluationKind kind;
    __int64 llResult;
    bool bResult;
    CComVariant varResult;

    if (!m_bGoodOperandType)
    {
        SetTypeError ();
        return;
    };

    varValue = m_pChild->GetValue ();

    kind = m_EvaluationKind;
    if (kind == GuessingEvaluation)
    {
        switch (varValue.vt)
        {
        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL:
            kind = IntegerEvaluation;
            break;

        case VT_BOOL:
            kind = BoolEvaluation;
            break;

        default:
            SetTypeError ();
            return;
        };
    };

    switch (kind)
    {
    case IntegerEvaluation:
        switch (varValue.vt)
        {
        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL: break;

        default:
            SetTypeError ();
            return;
        };

        EvaluateInteger (llResult, GetInt64 (varValue));
        SetInt64 (llResult, varResult);
        break;

    case BoolEvaluation:
        switch (varValue.vt)
        {
        case VT_BOOL: break;

        default:
            SetTypeError ();
            return;
        };

        EvaluateBool (bResult, GetBool (varValue));
        SetBool (bResult, varResult);
        break;
    };

    SetState (CEGoodExpression);
    SetValue (varResult);
    SynthesizeQuality ();
    SynthesizeTimestamp ();
}


void CExpressionUnaryLogical::MakeEvaluationKind (BL_IEC_TYP iecType)
{
    EvaluationKind kind;

    switch (iecType)
    {
    case BL_BYTE: case BL_WORD: case BL_DWORD: case BL_LWORD:
        kind = IntegerEvaluation;
        break;

    case BL_BOOL:
        kind = BoolEvaluation;
        break;

    default:
        ASSERT (false);
        kind = GuessingEvaluation;
        break;
    };

    m_EvaluationKind = kind;
}


void CExpressionUnaryLogical::SetTypeError ()
{
    switch (m_Logical)
    {
    case Not: SetState (CEInvalidNotOperandType); break;
    };
}


void CExpressionUnaryLogical::EvaluateInteger (__int64 &llResult, __int64 llOperand)
{
    switch (m_Logical)
    {
    case Not:
        llResult = ~llOperand;
        break;
    };
}


void CExpressionUnaryLogical::EvaluateBool (bool &bResult, bool bOperand)
{
    switch (m_Logical)
    {
    case Not:
        bResult = !bOperand;
        break;
    };
}


CExpressionBinaryArithmetic::CExpressionBinaryArithmetic (CComPtr<IDebugInfo> pDebugInfo, Arithmetic arithmetic, CExpressionNode *pLeftOperand, CExpressionNode *pRightOperand)
: CExpressionBinary (pDebugInfo, pLeftOperand, pRightOperand),
  m_Arithmetic (arithmetic),
  m_EvaluationKind (GuessingEvaluation),
  m_bGoodLeftType (false),
  m_bGoodRightType (false),
  m_bGoodTypeCombination (false)
{
    CComPtr<ITypeDBI> pLeftType;
    CComPtr<ITypeDBI> pRightType;

    pLeftType = pLeftOperand->GetType ();
    pRightType = pRightOperand->GetType ();

    if (pLeftOperand->CheckIntegerType () ||
        m_Arithmetic != Modulo && pLeftOperand->CheckRealType () ||
        m_Arithmetic == Add && pLeftOperand->CheckDateTimeType (true, false, true, true) ||
        m_Arithmetic == Subtract && pLeftOperand->CheckDateTimeType (true, true, true, true) ||
        (m_Arithmetic == Multiply || m_Arithmetic == Divide) && pLeftOperand->CheckDateTimeType (true, false, false, false))
    {
        m_bGoodLeftType = true;

        if (pLeftType != NULL)
        {
            BL_IEC_TYP iecType;

            if (!pLeftOperand->IsSimpleType (iecType))
            {
                ASSERT (false);
            }
            else
            {
                MakeEvaluationKind (iecType);
            };
        };
    };

    if (pRightOperand->CheckIntegerType () ||
        m_Arithmetic != Modulo && pRightOperand->CheckRealType () ||
        m_Arithmetic == Add && pRightOperand->CheckDateTimeType (true, false, false, false) ||
        m_Arithmetic == Subtract && pRightOperand->CheckDateTimeType (true, true, true, true))
    {
        m_bGoodRightType = true;

        if (pRightType != NULL)
        {
            BL_IEC_TYP iecType;

            if (!pRightOperand->IsSimpleType (iecType))
            {
                ASSERT (false);
            }
            else if (m_EvaluationKind == GuessingEvaluation)
            {
                MakeEvaluationKind (iecType);
            };
        };
    };

    if (m_bGoodLeftType && m_bGoodRightType)
    {
        if (pLeftType == NULL)
        {
            m_bGoodTypeCombination = true;

            // If the left operand type is unknown and the right
            // operand type is not a date or time type the result
            // type is the right operand type.

            if (!pRightOperand->CheckDateTimeType (true, true, true, true))
            {
                m_pType = pRightType;
            };
        }
        else if (pRightType == NULL)
        {
            m_bGoodTypeCombination = true;

            // If we have a multiplication or division, the left operand type
            // is TIME and although we have no right operand type we know the
            // right operand must be of integer type (as in case of integer
            // literals) the result type is TIME.

            if ((m_Arithmetic == Multiply || m_Arithmetic == Divide) &&
                pLeftOperand->CheckDateTimeType (true, false, false, false) &&
                pRightOperand->CheckIntegerType ())
            {
                m_pType = pLeftType;
            }

            // If the right operand type is unknown and either the left
            // operand type is not a date or time type or we have an
            // addition the result type is the left operand type.

            else if (!pLeftOperand->CheckDateTimeType (true, true, true, true) || m_Arithmetic == Add)
            {
                m_pType = pLeftType;
            };
        }
        else
        {
            if (m_Arithmetic == Add)
            {
                if (pRightOperand->CheckDateTimeType (true, false, false, false))
                {
                    if (pLeftOperand->CheckDateTimeType (true, false, true, true))
                    {
                        m_bGoodTypeCombination = true;
                        m_pType = pLeftType;
                    };
                }
                else
                {
                    if (!pLeftOperand->CheckDateTimeType (true, false, true, true) &&
                        pLeftType == pRightType)
                    {
                        m_bGoodTypeCombination = true;
                        m_pType = pLeftType;
                    };
                };
            }
            else if (m_Arithmetic == Subtract)
            {
                if (pLeftOperand->CheckDateTimeType (true, true, true, true))
                {
                    if (pRightOperand->CheckDateTimeType (true, false, false, false))
                    {
                        m_bGoodTypeCombination = true;
                        m_pType = pLeftType;
                    }
                    else if (pLeftType == pRightType)
                    {
                        m_bGoodTypeCombination = true;
                        BuildType (BL_TIME);
                    };
                }
                else
                {
                    if (pLeftType == pRightType)
                    {
                        m_bGoodTypeCombination = true;
                        m_pType = pLeftType;
                    };
                };
            }
            else if (m_Arithmetic == Multiply || m_Arithmetic == Divide)
            {
                if (pLeftOperand->CheckDateTimeType (true, false, false, false) &&
                    pRightOperand->CheckIntegerType ())
                {
                    m_bGoodTypeCombination = true;
                    m_pType = pLeftType;
                }
                else
                {
                    if (pLeftType == pRightType)
                    {
                        m_bGoodTypeCombination = true;
                        m_pType = pLeftType;
                    };
                };
            }
            else
            {
                if (pLeftType == pRightType)
                {
                    m_bGoodTypeCombination = true;
                    m_pType = pLeftType;
                };
            };
        };
    };
}


void CExpressionBinaryArithmetic::Evaluate ()
{
    CComVariant varLeftValue;
    CComVariant varRightValue;
    EvaluationKind kind;
    __int64 llResult;
    double dblResult;
    CComVariant varResult;

    if (!m_bGoodLeftType)
    {
        SetTypeError (-1);
        return;
    };

    if (!m_bGoodRightType)
    {
        SetTypeError (1);
        return;
    };

    if (!m_bGoodTypeCombination)
    {
        SetTypeError (0);
        return;
    };

    varLeftValue = m_pLeftChild->GetValue ();
    varRightValue = m_pRightChild->GetValue ();

    kind = m_EvaluationKind;
    if (kind == GuessingEvaluation)
    {
        switch (varLeftValue.vt)
        {
        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL:
            kind = IntegerEvaluation;
            break;

        case VT_R4: case VT_R8:
            if (m_Arithmetic == Modulo)
            {
                SetTypeError (-1);
                return;
            };
            kind = RealEvaluation;
            break;

        default:
            SetTypeError (-1);
            return;
        };
    };

    switch (kind)
    {
    case IntegerEvaluation:
        switch (varLeftValue.vt)
        {
        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL: break;

        case VT_R4: case VT_R8: case VT_BOOL:
            SetTypeError (0);
            return;

        default:
            SetTypeError (-1);
            return;
        };

        switch (varRightValue.vt)
        {
        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL: break;

        case VT_R4: case VT_R8: case VT_BOOL:
            SetTypeError (0);
            return;

        default:
            SetTypeError (1);
            return;
        };

        if (!EvaluateInteger (llResult, GetInt64 (varLeftValue), GetInt64 (varRightValue)))
        {
            return;
        };
        SetInt64 (llResult, varResult);
        break;

    case RealEvaluation:
        switch (varLeftValue.vt)
        {
        case VT_R4: case VT_R8: break;

        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL: case VT_BOOL:
            SetTypeError (0);
            return;

        default:
            SetTypeError (-1);
            return;
        };

        switch (varRightValue.vt)
        {
        case VT_R4: case VT_R8: break;

        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL: case VT_BOOL:
            SetTypeError (0);
            return;

        default:
            SetTypeError (1);
            return;
        };

        if (!EvaluateReal (dblResult, GetDouble (varLeftValue), GetDouble (varRightValue)))
        {
            return;
        };
        SetDouble (dblResult, varResult);
        break;
    };

    SetState (CEGoodExpression);
    SetValue (varResult);
    SynthesizeQuality ();
    SynthesizeTimestamp ();
}


void CExpressionBinaryArithmetic::MakeEvaluationKind (BL_IEC_TYP iecType)
{
    EvaluationKind kind;

    switch (iecType)
    {
    //case BL_SINT: 
    //case BL_USINT: 
    case BL_UINT: case BL_UDINT: case BL_ULINT:
    case BL_INT: case BL_DINT: case BL_LINT:
    case BL_TIME: case BL_DATE: case BL_TOD: case BL_DT:
        kind = IntegerEvaluation;
        break;

    case BL_REAL:
    case BL_LREAL:
        kind = RealEvaluation;
        break;

    default:
        ASSERT (false);
        kind = GuessingEvaluation;
        break;
    };

    m_EvaluationKind = kind;
}


void CExpressionBinaryArithmetic::SetTypeError (int which)
{
    if (which < 0)
    {
        switch (m_Arithmetic)
        {
        case Add     : SetState (CEInvalidAddLeftType);  break;
        case Subtract: SetState (CEInvalidSubLeftType);  break;
        case Multiply: SetState (CEInvalidMultLeftType); break;
        case Divide  : SetState (CEInvalidDivLeftType);  break;
        case Modulo  : SetState (CEInvalidModLeftType);  break;
        };
    }
    else if (which > 0)
    {
        switch (m_Arithmetic)
        {
        case Add     : SetState (CEInvalidAddRightType);  break;
        case Subtract: SetState (CEInvalidSubRightType);  break;
        case Multiply: SetState (CEInvalidMultRightType); break;
        case Divide  : SetState (CEInvalidDivRightType);  break;
        case Modulo  : SetState (CEInvalidModRightType);  break;
        };
    }
    else
    {
        switch (m_Arithmetic)
        {
        case Add     : SetState (CEInvalidAddTypeCombi);  break;
        case Subtract: SetState (CEInvalidSubTypeCombi);  break;
        case Multiply: SetState (CEInvalidMultTypeCombi); break;
        case Divide  : SetState (CEInvalidDivTypeCombi);  break;
        case Modulo  : SetState (CEInvalidModTypeCombi);  break;
        };
    };
}


bool CExpressionBinaryArithmetic::EvaluateInteger (__int64 &llResult, __int64 llLeftValue, __int64 llRightValue)
{
    switch (m_Arithmetic)
    {
    case Add:
        llResult = llLeftValue + llRightValue;
        break;

    case Subtract:
        llResult = llLeftValue - llRightValue;
        break;

    case Multiply:
        llResult = llLeftValue * llRightValue;
        break;

    case Divide:
        if (llRightValue == 0)
        {
            SetState (CEDivisionByZero);
            return (false);
        };

        llResult = llLeftValue / llRightValue;
        break;

    case Modulo:
        if (llRightValue == 0)
        {
            SetState (CEModuloZero);
            return (false);
        };

        llResult = llLeftValue % llRightValue;
        break;
    };

    return (true);
}


bool CExpressionBinaryArithmetic::EvaluateReal (double &dblResult, double dblLeftValue, double dblRightValue)
{
    switch (m_Arithmetic)
    {
    case Add:
        dblResult = dblLeftValue + dblRightValue;
        break;

    case Subtract:
        dblResult = dblLeftValue - dblRightValue;
        break;

    case Multiply:
        dblResult = dblLeftValue * dblRightValue;
        break;

    case Divide:
        if (dblRightValue == 0.0)
        {
            SetState (CEDivisionByZero);
            return (false);
        };

        dblResult = dblLeftValue / dblRightValue;
        break;
    };

    return (true);
}


CExpressionRelation::CExpressionRelation (CComPtr<IDebugInfo> pDebugInfo, 
                                          Relation relation,
                                          CExpressionNode *pLeftOperand,
                                          CExpressionNode *pRightOperand)
: CExpressionBinary (pDebugInfo, pLeftOperand, pRightOperand),
  m_Relation (relation),
  m_EvaluationKind (GuessingEvaluation),
  m_bGoodLeftType (false),
  m_bGoodRightType (false),
  m_bGoodTypeCombination (false),
  m_bUnsigned (false)
{
    CComPtr<ITypeDBI> pLeftType;
    CComPtr<ITypeDBI> pRightType;

    pLeftType = pLeftOperand->GetType ();
    pRightType = pRightOperand->GetType ();

    if (pLeftOperand->CheckIntegerType () ||
        pLeftOperand->CheckRealType () ||
        pLeftOperand->CheckBoolType () ||
        pLeftOperand->CheckBitType () ||
        pLeftOperand->CheckDateTimeType (true, true, true, true))
    {
        m_bGoodLeftType = true;

        if (pLeftOperand->GetType () != NULL)
        {
            BL_IEC_TYP iecType;

            if (!pLeftOperand->IsSimpleType (iecType))
            {
                ASSERT (false);
            }
            else
            {
                MakeEvaluationKind (iecType);
            };
        };
    };

    if (pRightOperand->CheckIntegerType () ||
        pRightOperand->CheckRealType () ||
        pRightOperand->CheckBoolType () ||
        pRightOperand->CheckBitType () ||
        pRightOperand->CheckDateTimeType (true, true, true, true))
    {
        m_bGoodRightType = true;

        if (pRightOperand->GetType () != NULL)
        {
            BL_IEC_TYP iecType;

            if (!pRightOperand->IsSimpleType (iecType))
            {
                ASSERT (false);
            }
            else if (m_EvaluationKind == GuessingEvaluation)
            {
                MakeEvaluationKind (iecType);
            };
        };
    };

    if (m_bGoodLeftType && m_bGoodRightType)
    {
        if (pLeftType == NULL || pRightType == NULL || pLeftType == pRightType)
        {
            m_bGoodTypeCombination = true;
        };
    };

    BuildType (BL_BOOL);
}


void CExpressionRelation::Evaluate ()
{
    CComVariant varLeftValue;
    CComVariant varRightValue;
    bool bResult;
    CComVariant varResult;
    EvaluationKind kind;

    if (!m_bGoodLeftType)
    {
        SetTypeError (-1);
        return;
    };

    if (!m_bGoodRightType)
    {
        SetTypeError (1);
        return;
    };

    if (!m_bGoodTypeCombination)
    {
        SetTypeError (0);
        return;
    };

    varLeftValue = m_pLeftChild->GetValue ();
    varRightValue = m_pRightChild->GetValue ();

    kind = m_EvaluationKind;
    if (kind == GuessingEvaluation)
    {
        switch (varLeftValue.vt)
        {
        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL:
            kind = IntegerEvaluation;
            break;

        case VT_R4: case VT_R8:
            kind = RealEvaluation;
            break;

        case VT_BOOL:
            kind = BoolEvaluation;
            break;

        default:
            SetTypeError (-1);
            return;
        };
    };

    switch (kind)
    {
    case IntegerEvaluation:
        switch (varLeftValue.vt)
        {
        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL: break;

        case VT_R4: case VT_R8: case VT_BOOL:
            SetTypeError (0);
            return;

        default:
            SetTypeError (-1);
            return;
        };

        switch (varRightValue.vt)
        {
        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL: break;

        case VT_R4: case VT_R8: case VT_BOOL:
            SetTypeError (0);
            return;

        default:
            SetTypeError (1);
            return;
        };

        if (m_bUnsigned)
            EvaluateUInteger (bResult, GetInt64 (varLeftValue), GetInt64 (varRightValue));
        else
            EvaluateInteger (bResult, GetInt64 (varLeftValue), GetInt64 (varRightValue));
        SetBool (bResult, varResult);
        break;

    case RealEvaluation:
        switch (varLeftValue.vt)
        {
        case VT_R4: case VT_R8: break;

        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL: case VT_BOOL:
            SetTypeError (0);
            return;

        default:
            SetTypeError (-1);
            return;
        };

        switch (varRightValue.vt)
        {
        case VT_R4: case VT_R8: break;

        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL: case VT_BOOL:
            SetTypeError (0);
            return;

        default:
            SetTypeError (1);
            return;
        };

        EvaluateReal (bResult, GetDouble (varLeftValue), GetDouble (varRightValue));
        SetBool (bResult, varResult);
        break;

    case BoolEvaluation:
        switch (varLeftValue.vt)
        {
        case VT_BOOL: break;

        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL: case VT_R4: case VT_R8:
            SetTypeError (0);
            return;

        default:
            SetTypeError (-1);
            return;
        };

        switch (varRightValue.vt)
        {
        case VT_BOOL: break;

        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL: case VT_R4: case VT_R8:
            SetTypeError (0);
            return;

        default:
            SetTypeError (1);
            return;
        };

        EvaluateBool (bResult, GetBool (varLeftValue), GetBool (varRightValue));
        SetBool (bResult, varResult);
        break;
    };

    SetState (CEGoodExpression);
    SetValue (varResult);
    SynthesizeQuality ();
    SynthesizeTimestamp ();
}


void CExpressionRelation::MakeEvaluationKind (BL_IEC_TYP iecType)
{
    EvaluationKind kind;

    switch (iecType)
    {
    //case BL_SINT: 
    //case BL_USINT: 
    case BL_UINT: case BL_UDINT: case BL_ULINT:
    case BL_INT: case BL_DINT: case BL_LINT:
    case BL_TIME: case BL_DATE: case BL_TOD: case BL_DT:
    case BL_BYTE: case BL_WORD: case BL_DWORD: case BL_LWORD:
        kind = IntegerEvaluation;
        break;

    case BL_REAL:
    case BL_LREAL:
        kind = RealEvaluation;
        break;

    case BL_BOOL:
        kind = BoolEvaluation;
        break;

    default:
        ASSERT (false);
        kind = GuessingEvaluation;
        break;
    };

    m_EvaluationKind = kind;

    m_bUnsigned = (iecType == BL_BYTE || iecType == BL_WORD ||
                   iecType == BL_DWORD || iecType == BL_LWORD);
}


void CExpressionRelation::EvaluateInteger (bool &bResult, __int64 llLeftValue, __int64 llRightValue)
{
    switch (m_Relation)
    {
    case Equal       : bResult = llLeftValue == llRightValue; break;
    case Unequal     : bResult = llLeftValue != llRightValue; break;
    case Less        : bResult = llLeftValue <  llRightValue; break;
    case LessEqual   : bResult = llLeftValue <= llRightValue; break;
    case Greater     : bResult = llLeftValue >  llRightValue; break;
    case GreaterEqual: bResult = llLeftValue >= llRightValue; break;
    };
}


void CExpressionRelation::EvaluateUInteger (bool &bResult, unsigned __int64 llLeftValue, unsigned __int64 llRightValue)
{
    switch (m_Relation)
    {
    case Equal       : bResult = llLeftValue == llRightValue; break;
    case Unequal     : bResult = llLeftValue != llRightValue; break;
    case Less        : bResult = llLeftValue <  llRightValue; break;
    case LessEqual   : bResult = llLeftValue <= llRightValue; break;
    case Greater     : bResult = llLeftValue >  llRightValue; break;
    case GreaterEqual: bResult = llLeftValue >= llRightValue; break;
    };
}


void CExpressionRelation::EvaluateReal (bool &bResult, double dblLeftValue, double dblRightValue)
{
    switch (m_Relation)
    {
    case Equal       : bResult = dblLeftValue == dblRightValue; break;
    case Unequal     : bResult = dblLeftValue != dblRightValue; break;
    case Less        : bResult = dblLeftValue <  dblRightValue; break;
    case LessEqual   : bResult = dblLeftValue <= dblRightValue; break;
    case Greater     : bResult = dblLeftValue >  dblRightValue; break;
    case GreaterEqual: bResult = dblLeftValue >= dblRightValue; break;
    };
}


void CExpressionRelation::EvaluateBool (bool &bResult, bool bLeftValue, bool bRightValue)
{
    switch (m_Relation)
    {
    case Equal       : bResult = bLeftValue == bRightValue; break;
    case Unequal     : bResult = bLeftValue != bRightValue; break;
    case Less        : bResult = bLeftValue <  bRightValue; break;
    case LessEqual   : bResult = bLeftValue <= bRightValue; break;
    case Greater     : bResult = bLeftValue > bRightValue; break;
    case GreaterEqual: bResult = bLeftValue >= bRightValue; break;
    };
}


void CExpressionRelation::SetTypeError (int which)
{
    if (which < 0)
    {
        switch (m_Relation)
        {
        case Equal       : SetState (CEInvalidEQLeftType); break;
        case Unequal     : SetState (CEInvalidNELeftType); break;
        case Less        : SetState (CEInvalidLTLeftType); break;
        case LessEqual   : SetState (CEInvalidLELeftType); break;
        case Greater     : SetState (CEInvalidGTLeftType); break;
        case GreaterEqual: SetState (CEInvalidGELeftType); break;
        };
    }
    else if (which > 0)
    {
        switch (m_Relation)
        {
        case Equal       : SetState (CEInvalidEQRightType); break;
        case Unequal     : SetState (CEInvalidNERightType); break;
        case Less        : SetState (CEInvalidLTRightType); break;
        case LessEqual   : SetState (CEInvalidLERightType); break;
        case Greater     : SetState (CEInvalidGTRightType); break;
        case GreaterEqual: SetState (CEInvalidGERightType); break;
        };
    }
    else
    {
        switch (m_Relation)
        {
        case Equal       : SetState (CEInvalidEQTypeCombi); break;
        case Unequal     : SetState (CEInvalidNETypeCombi); break;
        case Less        : SetState (CEInvalidLTTypeCombi); break;
        case LessEqual   : SetState (CEInvalidLETypeCombi); break;
        case Greater     : SetState (CEInvalidGTTypeCombi); break;
        case GreaterEqual: SetState (CEInvalidGETypeCombi); break;
        };
    };
}


CExpressionBinaryLogical::CExpressionBinaryLogical (CComPtr<IDebugInfo> pDebugInfo, 
                                                    Logical logical,
                                                    CExpressionNode *pLeftOperand,
                                                    CExpressionNode *pRightOperand)
: CExpressionBinary (pDebugInfo, pLeftOperand, pRightOperand),
  m_Logical (logical),
  m_EvaluationKind (GuessingEvaluation),
  m_bGoodLeftType (false),
  m_bGoodRightType (false),
  m_bGoodTypeCombination (false)
{
    CComPtr<ITypeDBI> pLeftType;
    CComPtr<ITypeDBI> pRightType;

    pLeftType = pLeftOperand->GetType ();
    pRightType = pRightOperand->GetType ();

    if (pLeftOperand->CheckBitType () || pLeftOperand->CheckBoolType ())
    {
        m_bGoodLeftType = true;

        if (pLeftOperand->GetType () != NULL)
        {
            BL_IEC_TYP iecType;

            if (!pLeftOperand->IsSimpleType (iecType))
            {
                ASSERT (false);
            }
            else
            {
                MakeEvaluationKind (iecType);
            };
        };
    };

    if (pRightOperand->CheckBitType () || pRightOperand->CheckBoolType ())
    {
        m_bGoodRightType = true;

        if (pRightOperand->GetType () != NULL)
        {
            BL_IEC_TYP iecType;

            if (!pRightOperand->IsSimpleType (iecType))
            {
                ASSERT (false);
            }
            else if (m_EvaluationKind == GuessingEvaluation)
            {
                MakeEvaluationKind (iecType);
            };
        };
    };

    if (m_bGoodLeftType && m_bGoodRightType)
    {
        if (pLeftType == NULL)
        {
            m_bGoodTypeCombination = true;
            m_pType = pRightType;
        }
        else if (pRightType == NULL || pLeftType == pRightType)
        {
            m_bGoodTypeCombination = true;
            m_pType = pLeftType;
        };
    };
}


void CExpressionBinaryLogical::Evaluate ()
{
    CComVariant varLeftValue;
    CComVariant varRightValue;
    __int64 llResult;
    bool bResult;
    CComVariant varResult;
    EvaluationKind kind;

    if (!m_bGoodLeftType)
    {
        SetTypeError (-1);
        return;
    };

    if (!m_bGoodRightType)
    {
        SetTypeError (1);
        return;
    };

    if (!m_bGoodTypeCombination)
    {
        SetTypeError (0);
        return;
    };

    varLeftValue = m_pLeftChild->GetValue ();
    varRightValue = m_pRightChild->GetValue ();

    kind = m_EvaluationKind;
    if (kind == GuessingEvaluation)
    {
        switch (varLeftValue.vt)
        {
        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL:
            kind = BitEvaluation;
            break;

        case VT_BOOL:
            kind = BoolEvaluation;
            break;

        default:
            SetTypeError (-1);
            return;
        };
    };

    switch (kind)
    {
    case BitEvaluation:
        switch (varLeftValue.vt)
        {
        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL: break;

        case VT_BOOL:
            SetTypeError (0);
            return;

        default:
            SetTypeError (-1);
            return;
        };

        switch (varRightValue.vt)
        {
        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL: break;

        case VT_BOOL:
            SetTypeError (0);
            return;

        default:
            SetTypeError (1);
            return;
        };

        EvaluateBit (llResult, GetInt64 (varLeftValue), GetInt64 (varRightValue));
        SetInt64 (llResult, varResult);
        break;

    case BoolEvaluation:
        switch (varLeftValue.vt)
        {
        case VT_BOOL: break;

        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL:
            SetTypeError (0);
            return;

        default:
            SetTypeError (-1);
            return;
        };

        switch (varRightValue.vt)
        {
        case VT_BOOL: break;

        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL:
            SetTypeError (0);
            return;

        default:
            SetTypeError (1);
            return;
        };

        EvaluateBool (bResult, GetBool (varLeftValue), GetBool (varRightValue));
        SetBool (bResult, varResult);
        break;
    };

    SetState (CEGoodExpression);
    SetValue (varResult);
    SynthesizeQuality ();
    SynthesizeTimestamp ();
}


void CExpressionBinaryLogical::MakeEvaluationKind (BL_IEC_TYP iecType)
{
    EvaluationKind kind;

    switch (iecType)
    {
    case BL_BYTE: case BL_WORD: case BL_DWORD: case BL_LWORD:
        kind = BitEvaluation;
        break;

    case BL_BOOL:
        kind = BoolEvaluation;
        break;

    default:
        ASSERT (false);
        kind = GuessingEvaluation;
        break;
    };

    m_EvaluationKind = kind;
}


void CExpressionBinaryLogical::EvaluateBit (__int64 &llResult, __int64 llLeftValue, __int64 llRightValue)
{
    switch (m_Logical)
    {
    case And: llResult = llLeftValue & llRightValue; break;
    case Or : llResult = llLeftValue | llRightValue; break;
    case Xor: llResult = llLeftValue ^ llRightValue; break;
    };
}


void CExpressionBinaryLogical::EvaluateBool (bool &bResult, bool bLeftValue, bool bRightValue)
{
    switch (m_Logical)
    {
    case And: bResult = bLeftValue && bRightValue; break;
    case Or : bResult = bLeftValue || bRightValue; break;
    case Xor: bResult = bLeftValue != bRightValue; break;
    };
}


void CExpressionBinaryLogical::SetTypeError (int which)
{
    if (which < 0)
    {
        switch (m_Logical)
        {
        case And: SetState (CEInvalidAndLeftType); break;
        case Or : SetState (CEInvalidOrLeftType);  break;
        case Xor: SetState (CEInvalidXorLeftType); break;
        };
    }
    else if (which > 0)
    {
        switch (m_Logical)
        {
        case And: SetState (CEInvalidAndRightType); break;
        case Or : SetState (CEInvalidOrRightType);  break;
        case Xor: SetState (CEInvalidXorRightType); break;
        };
    }
    else
    {
        switch (m_Logical)
        {
        case And: SetState (CEInvalidAndTypeCombi); break;
        case Or : SetState (CEInvalidOrTypeCombi);  break;
        case Xor: SetState (CEInvalidXorTypeCombi); break;
        };
    };
}


CExpressionPower::CExpressionPower (CComPtr<IDebugInfo> pDebugInfo, CExpressionNode *pLeftOperand, CExpressionNode *pRightOperand)
: CExpressionBinary (pDebugInfo, pLeftOperand, pRightOperand),
  m_EvaluationKind (GuessingEvaluation),
  m_bGoodLeftType (false),
  m_bGoodRightType (false)
{
    CComPtr<ITypeDBI> pLeftType;
    CComPtr<ITypeDBI> pRightType;

    pLeftType = pLeftOperand->GetType ();
    pRightType = pRightOperand->GetType ();

    if (pLeftOperand->CheckRealType ())
    {
        m_bGoodLeftType = true;

        if (pLeftOperand->GetType () != NULL)
        {
            BL_IEC_TYP iecType;

            if (!pLeftOperand->IsSimpleType (iecType))
            {
                ASSERT (false);
            };
        };
    };

    if (pRightOperand->CheckIntegerType () || pRightOperand->CheckRealType ())
    {
        m_bGoodRightType = true;

        if (pRightOperand->GetType () != NULL)
        {
            BL_IEC_TYP iecType;

            if (!pRightOperand->IsSimpleType (iecType))
            {
                ASSERT (false);
            }
            else
            {
                MakeEvaluationKind (iecType);
            };
        };
    };

    m_pType = pLeftType;
}


void CExpressionPower::Evaluate ()
{
    CComVariant varLeftValue;
    CComVariant varRightValue;
    EvaluationKind kind;
    double dblResult;
    CComVariant varResult;

    if (!m_bGoodLeftType)
    {
        SetTypeError (false);
        return;
    };

    if (!m_bGoodRightType)
    {
        SetTypeError (true);
        return;
    };

    varLeftValue = m_pLeftChild->GetValue ();
    varRightValue = m_pRightChild->GetValue ();

    kind = m_EvaluationKind;
    if (kind == GuessingEvaluation)
    {
        switch (varRightValue.vt)
        {
        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL:
            kind = IntegerEvaluation;
            break;

        case VT_R4: case VT_R8:
            kind = RealEvaluation;
            break;

        default:
            SetTypeError (true);
            return;
        };
    };

    switch (kind)
    {
    case IntegerEvaluation:
        switch (varLeftValue.vt)
        {
        case VT_R4: case VT_R8: break;

        default:
            SetTypeError (false);
            return;
        };

        switch (varRightValue.vt)
        {
        case VT_I1: case VT_I2: case VT_I4: case VT_DECIMAL: break;

        default:
            SetTypeError (true);
            return;
        };

        EvaluateInteger (dblResult, GetDouble (varLeftValue), GetInt64 (varRightValue));
        SetDouble (dblResult, varResult);
        break;

    case RealEvaluation:
        switch (varLeftValue.vt)
        {
        case VT_R4: case VT_R8: break;

        default:
            SetTypeError (false);
            return;
        };

        switch (varRightValue.vt)
        {
        case VT_R4: case VT_R8: break;

        default:
            SetTypeError (true);
            return;
        };

        EvaluateReal (dblResult, GetDouble (varLeftValue), GetDouble (varRightValue));
        SetDouble (dblResult, varResult);
        break;
    };

    SetState (CEGoodExpression);
    SetValue (varResult);
    SynthesizeQuality ();
    SynthesizeTimestamp ();
}


void CExpressionPower::MakeEvaluationKind (BL_IEC_TYP iecType)
{
    EvaluationKind kind;

    switch (iecType)
    {
    //case BL_SINT: 
    //case BL_USINT: 
    case BL_UINT: case BL_UDINT: case BL_ULINT:
    case BL_INT: case BL_DINT: case BL_LINT:
        kind = IntegerEvaluation;
        break;

    case BL_REAL:
    case BL_LREAL:
        kind = RealEvaluation;
        break;

    default:
        ASSERT (false);
        kind = GuessingEvaluation;
        break;
    };

    m_EvaluationKind = kind;
}


void CExpressionPower::SetTypeError (bool bRight)
{
    if (!bRight)
    {
        SetState (CEInvalidPowerLeftType);
    }
    else
    {
        SetState (CEInvalidPowerRightType);
    };
}


void CExpressionPower::EvaluateInteger (double &dblResult, double dblLeftValue, __int64 llRightValue)
{
    unsigned __int64 ullExponent;

    dblResult = 1.0;

    if (llRightValue < 0)
    {
         dblLeftValue = 1.0 / dblLeftValue;
         llRightValue = -llRightValue;
    };

    ullExponent = (unsigned __int64)llRightValue;

    while (ullExponent != 0)
    { 
         if ((ullExponent & 1) != 0)
         {
             dblResult *= dblLeftValue;
         };

         ullExponent >>= 1;

         dblLeftValue *= dblLeftValue;
    };
}


void CExpressionPower::EvaluateReal (double &dblResult, double dblLeftValue, double dblRightValue)
{
    if (dblLeftValue == 0.0)
    {
        dblResult = 0.0;
        return;
    };

    dblResult = exp (log (dblLeftValue) * dblRightValue);
}


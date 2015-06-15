#ifndef __WATCHEXPR_H_
#define __WATCHEXPR_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


interface IDebugInfo;
interface IConfigurationDBI;
interface IResourceDBI;
interface IVariableDBI;
interface ITypeDBI;

#include "baselib.h"
#include "fccallback.h"
#include "Subscription.h"
#include "cewatchbe.h"
#include "CESession.h"


class CExpressionNode
{
public:
    CExpressionNode (CComPtr<IDebugInfo> pDebugInfo);
    virtual ~CExpressionNode ();

    CComPtr<IDebugInfo> GetDebugInfo ();

    bool IsCommunicated ();
    void SetCommunicated(bool);
    CSubscription *GetSubscription ();
    CComPtr<ITypeDBI> GetType ();

    CEExpressionState GetState ();
    CComVariant GetValue ();
    short GetQuality ();
    DATE GetTimestamp ();

    void AddParent (CExpressionNode *pParent);
    void RemoveParent (CExpressionNode *pParent);

    virtual void Connect ();
    virtual void Disconnect ();

#ifdef DEBUG
    virtual void Evaluate () = 0;
#else
    virtual void Evaluate ();
#endif

    bool IsSimpleType (BL_IEC_TYP &iecType);
    bool IsArrayType (CComPtr<ITypeDBI> &pElementType, long &lLowerBound, long &lUpperBound);

    virtual bool CheckIntegerType ();
    virtual bool CheckRealType ();
    virtual bool CheckBoolType ();
    virtual bool CheckBitType ();
    virtual bool CheckDateTimeType (bool bAllowTime, bool bAllowDate, bool bAllowTimeOfDay, bool bAllowDateTime);

    virtual CString BuildSubscriptionPath ();

    void OnDelayedAdapt ();

protected:
    CComPtr<IDebugInfo> m_pDebugInfo;

    CList<CExpressionNode *, CExpressionNode *> m_Parents;

    CEExpressionState m_State;
    CComVariant       m_varValue;
    short             m_sQuality;
    DATE              m_Timestamp;
    bool              m_bModified;

    CComPtr<ITypeDBI> m_pType;

    unsigned int  m_uiConnectCount;

    bool          m_bCommunicated;
    CSubscription *m_pSubscription;
    FC_CALLBACK (SubscriptionModifiedCB, CExpressionNode) m_SubscriptionModifiedCB;

    void BuildType (BL_IEC_TYP iecType);

    void SetState (CEExpressionState state);
    void SetValue (const CComVariant &varValue);
    void SetQuality (short sQuality);
    void SetTimestamp (DATE timestamp);
    void SetModified ();

    virtual bool PropagateFailures ();
    virtual bool AllowPropagation (CExpressionNode *pChild);

    virtual void SynthesizeQuality ();
    virtual void SynthesizeTimestamp ();

    bool SetupCommunication ();
    bool FetchCommunicatedValue ();

    virtual bool ParentMaybeCommunicated ();
    virtual void Changed ();

    virtual HRESULT TransformCommunicatedValue (CComVariant &varValue);

    __int64 GetInt64 (const CComVariant &varValue);
    double GetDouble (const CComVariant &varValue);
    bool GetBool (const CComVariant &varValue);
    void SetInt64 (__int64 llValue, CComVariant &varValue);
    void SetDouble (double dblValue, CComVariant &varValue);
    void SetBool (bool bValue, CComVariant &varValue);

    virtual bool CheckValueType (BL_IEC_TYP iecType, VARTYPE valueType);

private:
    void Adapt ();
    void OnSubscriptionModified (CSubscription *pSubscription);
};


class CExpressionLeaf : public CExpressionNode
{
public:
    CExpressionLeaf (CComPtr<IDebugInfo> pDebugInfo, CComPtr<ITypeDBI> pType=NULL);
};


class CExpressionUnary : public CExpressionNode
{
public:
    CExpressionUnary (CComPtr<IDebugInfo> pDebugInfo, CExpressionNode *pChild);
    ~CExpressionUnary ();

    CExpressionNode *GetChild ();

    virtual void Connect ();
    virtual void Disconnect ();

protected:
    CExpressionNode *m_pChild;

    virtual bool PropagateFailures ();

    virtual void SynthesizeQuality ();
    virtual void SynthesizeTimestamp ();
};


class CExpressionBinary : public CExpressionNode
{
public:
    CExpressionBinary (CComPtr<IDebugInfo> pDebugInfo, CExpressionNode *pLeftChild, CExpressionNode *pRightChild);
    ~CExpressionBinary ();

    CExpressionNode *GetLeftChild ();
    CExpressionNode *GetRightChild ();

    virtual void Connect ();
    virtual void Disconnect ();

protected:
    CExpressionNode *m_pLeftChild;
    CExpressionNode *m_pRightChild;

    virtual bool PropagateFailures ();

    virtual void SynthesizeQuality ();
    virtual void SynthesizeTimestamp ();

private:
    unsigned int GetStateSeverity (CEExpressionState state);
};


class CExpressionLiteral : public CExpressionLeaf
{
public:
    CExpressionLiteral (CComPtr<IDebugInfo> pDebugInfo, __int64 llValue);
    CExpressionLiteral (CComPtr<IDebugInfo> pDebugInfo, __int64 llValue, BL_IEC_TYP iecType);
    CExpressionLiteral (CComPtr<IDebugInfo> pDebugInfo, double dblValue);
    CExpressionLiteral (CComPtr<IDebugInfo> pDebugInfo, double dblValue, BL_IEC_TYP iecType);
    CExpressionLiteral (CComPtr<IDebugInfo> pDebugInfo, bool bValue);
    CExpressionLiteral (CComPtr<IDebugInfo> pDebugInfo, const CString &strValue);

    virtual bool CheckIntegerType ();
    virtual bool CheckRealType ();

    virtual void Evaluate ();

protected:
    bool m_bUntypedInteger;
    bool m_bUntypedReal;
};


class CExpressionConfiguration : public CExpressionLeaf
{
public:
    CExpressionConfiguration (CComPtr<IDebugInfo> pDebugInfo, const CString &strName, bool bAddress);
    virtual ~CExpressionConfiguration ();

    CString GetName ();
    CComPtr<IConfigurationDBI> GetDBI ();

    virtual void Evaluate ();

    virtual CString BuildSubscriptionPath ();

protected:
    CString                    m_strName;
    CString                    m_strMachineName;
    CString                    m_strMachineAddress;
    CComPtr<IConfigurationDBI> m_pDBI;
    CCESession                 *m_pSession;

    FC_CALLBACK (SessionModifiedCB, CExpressionConfiguration) m_SessionModifiedCB;

private:
    void OnSessionModified (CCESession *pSession, bool bDebug);
    void ReadDebugInfo (const CString &strName, CComPtr<IDebugInfo> pDebugInfo);
};


class CExpressionComponent : public CExpressionUnary
{
public:
    CExpressionComponent (CComPtr<IDebugInfo> pDebugInfo, CExpressionNode *pChild, const CString &strFieldName);

    CString GetFieldName ();
    long GetStepNumber ();
    long GetActionNumber ();

    virtual void Evaluate ();

    virtual CString BuildSubscriptionPath ();

protected:
    enum SpecialAccess { AccessNormal, AccessStepX, AccessActionA };

    CString       m_strFieldName;
    bool          m_bGoodAggregateType;
    bool          m_bGoodField;
    bool          m_bConstant;
    bool          m_bInOut;
    CComVariant   m_varConstantValue;
    SpecialAccess m_SpecialAccess;
    long          m_lStepNumber;
    long          m_lActionNumber;
    CString       m_strOnlString;

    virtual bool ParentMaybeCommunicated ();
    virtual bool AllowPropagation (CExpressionNode *pChild);

    virtual HRESULT TransformCommunicatedValue (CComVariant &varValue);

private:
    void GetResourceField (CComPtr<IConfigurationDBI> pConfigurationDBI);
    void GetNonResourceField ();
    HRESULT GetVariableField ();
    HRESULT GetStepField ();
    HRESULT GetActionField ();
};


class CExpressionIndexing : public CExpressionBinary
{
public:
    CExpressionIndexing (CComPtr<IDebugInfo> pDebugInfo, CExpressionNode *pArray, CExpressionNode *pIndex);

    virtual void Evaluate ();

    virtual CString BuildSubscriptionPath ();

protected:
    bool m_bGoodArrayType;
    bool m_bGoodElementType;
    bool m_bGoodIndexType;

    virtual bool ParentMaybeCommunicated ();
    virtual bool AllowPropagation (CExpressionNode *pChild);

private:
    bool CheckIndex (long &lBase0Index);
};


class CExpressionConversion : public CExpressionUnary
{
public:
    CExpressionConversion (CComPtr<IDebugInfo> pDebugInfo, CExpressionNode *pOperand, BL_IEC_TYP sourceType, BL_IEC_TYP targetType);

    virtual void Evaluate ();

protected:
    enum EvaluationKind { IntegerEvaluation, RealEvaluation };

    EvaluationKind m_EvaluationKind;

    BL_IEC_TYP m_SourceType;
    BL_IEC_TYP m_TargetType;

    bool m_bGoodOperandType;
    bool m_bGoodTargetType;
};


class CExpressionUnaryArithmetic : public CExpressionUnary
{
public:
    enum Arithmetic { Plus, Minus };

    CExpressionUnaryArithmetic (CComPtr<IDebugInfo> pDebugInfo, Arithmetic arithmetic, CExpressionNode *pOperand);

    virtual void Evaluate ();

protected:
    enum EvaluationKind { GuessingEvaluation, IntegerEvaluation, RealEvaluation };

    Arithmetic m_Arithmetic;

    EvaluationKind m_EvaluationKind;

    bool m_bGoodOperandType;

private:
    void MakeEvaluationKind (BL_IEC_TYP iecType);
    void SetTypeError ();
    void EvaluateInteger (__int64 &llResult, __int64 llOperand);
    void EvaluateReal (double &dblResult, double dblOperand);
};


class CExpressionUnaryLogical : public CExpressionUnary
{
public:
    enum Logical { Not };

    CExpressionUnaryLogical (CComPtr<IDebugInfo> pDebugInfo, Logical logical, CExpressionNode *pOperand);

    virtual void Evaluate ();

protected:
    enum EvaluationKind { GuessingEvaluation, IntegerEvaluation, BoolEvaluation };

    Logical m_Logical;

    EvaluationKind m_EvaluationKind;

    bool m_bGoodOperandType;

private:
    void MakeEvaluationKind (BL_IEC_TYP iecType);
    void SetTypeError ();
    void EvaluateInteger (__int64 &llResult, __int64 llOperand);
    void EvaluateBool (bool &bResult, bool bOperand);
};


class CExpressionBinaryArithmetic : public CExpressionBinary
{
public:
    enum Arithmetic { Add, Subtract, Multiply, Divide, Modulo };

    CExpressionBinaryArithmetic (CComPtr<IDebugInfo> pDebugInfo, Arithmetic arithmetic, CExpressionNode *pLeftOperand, CExpressionNode *pRightOperand);

    virtual void Evaluate ();

protected:
    enum EvaluationKind { GuessingEvaluation, IntegerEvaluation, RealEvaluation };

    Arithmetic m_Arithmetic;

    EvaluationKind m_EvaluationKind;

    bool m_bGoodLeftType;
    bool m_bGoodRightType;
    bool m_bGoodTypeCombination;

private:
    void MakeEvaluationKind (BL_IEC_TYP iecType);
    void SetTypeError (int which);
    bool EvaluateInteger (__int64 &llResult, __int64 llLeftValue, __int64 llRightValue);
    bool EvaluateReal (double &dblResult, double dblLeftValue, double dblRightValue);
};


class CExpressionRelation : public CExpressionBinary
{
public:
    enum Relation { Equal, Unequal, Less, LessEqual, Greater, GreaterEqual };

    CExpressionRelation (CComPtr<IDebugInfo> pDebugInfo, Relation relation, CExpressionNode *pLeftOperand, CExpressionNode *pRightOperand);

    virtual void Evaluate ();

protected:
    enum EvaluationKind { GuessingEvaluation, IntegerEvaluation, RealEvaluation, BoolEvaluation };

    Relation m_Relation;

    EvaluationKind m_EvaluationKind;

    bool m_bGoodLeftType;
    bool m_bGoodRightType;
    bool m_bGoodTypeCombination;

    bool m_bUnsigned;

private:
    void MakeEvaluationKind (BL_IEC_TYP iecType);
    void EvaluateInteger (bool &bResult, __int64 llLeftValue, __int64 llRightValue);
    void EvaluateUInteger (bool &bResult, unsigned __int64 llLeftValue, unsigned __int64 llRightValue);
    void EvaluateReal (bool &bResult, double dblLeftValue, double dblRightValue);
    void EvaluateBool (bool &bResult, bool bLeftValue, bool bRightValue);
    void SetTypeError (int which);
};


class CExpressionBinaryLogical : public CExpressionBinary
{
public:
    enum Logical { And, Or, Xor };

    CExpressionBinaryLogical (CComPtr<IDebugInfo> pDebugInfo, Logical logical, CExpressionNode *pLeftOperand, CExpressionNode *pRightOperand);

    virtual void Evaluate ();

protected:
    enum EvaluationKind { GuessingEvaluation, BoolEvaluation, BitEvaluation };

    Logical m_Logical;

    EvaluationKind m_EvaluationKind;

    bool m_bGoodLeftType;
    bool m_bGoodRightType;
    bool m_bGoodTypeCombination;

private:
    void MakeEvaluationKind (BL_IEC_TYP iecType);
    void EvaluateBit (__int64 &llResult, __int64 llLeftValue, __int64 bRightValue);
    void EvaluateBool (bool &bResult, bool bLeftValue, bool bRightValue);
    void SetTypeError (int which);
};


class CExpressionPower : public CExpressionBinary
{
public:
    CExpressionPower (CComPtr<IDebugInfo> pDebugInfo, CExpressionNode *pLeftOperand, CExpressionNode *pRightOperand);

    virtual void Evaluate ();

protected:
    enum EvaluationKind { GuessingEvaluation, IntegerEvaluation, RealEvaluation };

    EvaluationKind m_EvaluationKind;

    bool m_bGoodLeftType;
    bool m_bGoodRightType;

private:
    void MakeEvaluationKind (BL_IEC_TYP iecType);
    void SetTypeError (bool bRight);
    void EvaluateInteger (double &dblResult, double dblLeftValue, __int64 llRightValue);
    void EvaluateReal (double &dblResult, double dblLeftValue, double dblRightValue);
};


#endif // __WATCHEXPR_H_

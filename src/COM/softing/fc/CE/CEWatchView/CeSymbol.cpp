


#include "stdafx.h"
#include "atlbase.h"
//#include "limits.h"
#include "cewatchview.h"
#include "CeSymbol.h"

#include "CeWatchBackEnd.h"
#include "CESysDef.h"
#include "DbgInfo.h"
#include "DbgInfo_i.c"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//*****************************************************************************
CCeSymbol::CCeSymbol(bool bRoot/*=false*/)
//*****************************************************************************
{
	m_Index = 0;
	m_pParent = NULL;
	m_pChildrenArray = NULL;

	if (bRoot)
		m_pData = new CCeSymbolData;
	else
		m_pData = NULL;

	m_bExpandFlag = FALSE;	// #2149 27.10.04 SIS
}

//*****************************************************************************
CCeSymbol::~CCeSymbol()
//*****************************************************************************
{
	if (m_pChildrenArray != NULL)
	{
		delete [] m_pChildrenArray;
		m_pChildrenArray = NULL;
	}

	if (m_pData != NULL)
	{
		delete m_pData;
		m_pData = NULL;
	}

	m_Index = 0;
	m_pParent = NULL;
}


//*****************************************************************************
CCeSymbolData* CCeSymbol::GetData()
//*****************************************************************************
{
	if (m_pData == NULL) // array element
	{
		ASSERT(m_pParent != NULL);
		ASSERT(m_pParent->m_pChildrenArray != NULL);
		ASSERT(m_pParent->m_pChildrenArray[0].m_pData != NULL);
		return m_pParent->m_pChildrenArray[0].m_pData;
	}
	else
	{
		return m_pData;
	}
}

//*****************************************************************************
const CCeWatchType& CCeSymbol::GetType() const
//*****************************************************************************
{
	if (m_pData == NULL) // array element
	{
		ASSERT(m_pParent != NULL);
		ASSERT(m_pParent->m_pChildrenArray != NULL);
		ASSERT(m_pParent->m_pChildrenArray[0].m_pData != NULL);
		return m_pParent->m_pChildrenArray[0].m_pData->m_Type;
	}
	else
		return m_pData->m_Type;
}

//*****************************************************************************
long CCeSymbol::GetMinIndex() const
//*****************************************************************************
{
	if (m_pData == NULL) // array element
	{
		ASSERT(m_pParent != NULL);
		ASSERT(m_pParent->m_pChildrenArray != NULL);
		ASSERT(m_pParent->m_pChildrenArray[0].m_pData != NULL);
		return m_pParent->m_pChildrenArray[0].m_pData->m_iMinIndex;
	}
	else
		return m_pData->m_iMinIndex;
}

//*****************************************************************************
unsigned long CCeSymbol::GetChildrenCount() const
//*****************************************************************************
{
	if (m_pData == NULL) // array element
	{
		ASSERT(m_pParent != NULL);
		ASSERT(m_pParent->m_pChildrenArray != NULL);
		ASSERT(m_pParent->m_pChildrenArray[0].m_pData != NULL);
		return m_pParent->m_pChildrenArray[0].m_pData->m_nChildren;
	}
	else
		return m_pData->m_nChildren;
}

//*****************************************************************************
CString CCeSymbol::GetName() const
//*****************************************************************************
{
	if (m_pParent != NULL && m_pParent->GetType().IsArray())
	{
		CString str;
		str.Format("[%ld]", m_Index);
		return str;
	}
	else
	{
		return m_pData->m_strName;
	}
}


//*****************************************************************************
CString CCeSymbol::GetFullName() const
//*****************************************************************************
{
	CString strFullName, strName;
	const CCeSymbol* pParent = this;
	bool bFirst = true;
	bool bStop = false;
	do
	{
		strName = pParent->GetName();
		
		bStop = pParent->GetType().IsConfiguration() ? true : false;
		if (bFirst)
		{
			strFullName = strName;	
		}
		else
		{
			if (pParent->GetType().IsArray())
				strFullName = strName + strFullName;	
			else
				strFullName = strName + "." + strFullName;	
		}

		bFirst = false;
		pParent = pParent->GetParent();
	}
	while (pParent != NULL && !bStop);
	return strFullName;
}


//*****************************************************************************	
bool CCeSymbol::LoadChildren()
//*****************************************************************************
{
	bool rv = true;
	unsigned long iChildrenCount = GetChildrenCount();
	long iMinIndex = GetMinIndex();
	if (m_pChildrenArray == NULL && iChildrenCount > 0)
	{
		bool bIsArray = GetType().IsArray();
		ASSERT(bIsArray || (!bIsArray && iMinIndex == 0));

		m_pChildrenArray = new CCeSymbol[iChildrenCount];

		for (unsigned long ii = 0; (ii < iChildrenCount) && rv; ii++)
		{
			if (ii == 0 || !bIsArray)
				m_pChildrenArray[ii].m_pData = new CCeSymbolData();

			m_pChildrenArray[ii].m_pParent = this;
			
			if (bIsArray)
				m_pChildrenArray[ii].m_Index = iMinIndex + ii;

			if (ii == 0 || !bIsArray)
				rv &= m_pChildrenArray[ii].Load(ii);
		}
	}
	return rv;
}

//*****************************************************************************	
bool CCeSymbol::Load(IDebugInfo* pIDebugInfo)
//*****************************************************************************
{
	ASSERT(pIDebugInfo != NULL);
	ASSERT(m_pData != NULL);
	bool rv = false;
	m_pData->m_iMinIndex = 0;
	m_pData->m_nChildren = 0;

	if (m_pData != NULL && pIDebugInfo != NULL)
	{
		HRESULT hr;
		if (pIDebugInfo != NULL)
		{
			hr = pIDebugInfo->get_Project((IUnknown **)&m_pData->m_pIUnknown);
			ASSERT(SUCCEEDED(hr));
			ASSERT(m_pData->m_pIUnknown != NULL);
		}
		if (m_pData->m_pIUnknown != NULL)
		{
			IProjectDBI* pIProjectDBI = NULL;
			hr = m_pData->m_pIUnknown->QueryInterface(IID_IProjectDBI, (LPVOID *)&pIProjectDBI);
			ASSERT(SUCCEEDED(hr));
			ASSERT(pIProjectDBI != NULL);
			if (pIProjectDBI != NULL)
			{
				hr = pIProjectDBI->get_ConfigurationCount(&m_pData->m_nChildren);
				ASSERT(SUCCEEDED(hr));

				CComBSTR bstrName;
				pIProjectDBI->get_Name(&bstrName);
				m_pData->m_strName = bstrName;

				m_pData->m_Type.Set(CEWATCH_BASETYPE_PROJECT, "Project");
				m_pData->m_InterFaceType = CESYMBOL_ITYPE_PROJECT;

				pIProjectDBI->Release();

				rv = true;
			}
		}
	}
	return rv;
}


//*****************************************************************************	
bool CCeSymbol::Load(unsigned long iIndex)
//*****************************************************************************
{
	bool rv = false;
	ASSERT(m_pParent != NULL);
	if (m_pParent == NULL)
		return rv;
	CCeSymbolData* pParentData = m_pParent->GetData();
	ASSERT(pParentData != NULL);
	ASSERT(m_pData != NULL);
	if (m_pData == NULL ||  pParentData == NULL)
		return rv;


	m_pData->m_iMinIndex = 0;
	m_pData->m_nChildren = 0;
	int iParentInterfaceType = pParentData->m_InterFaceType;
	IUnknown* pParentIUnkown =  pParentData->m_pIUnknown;
	ASSERT(pParentIUnkown != NULL);
	if (pParentIUnkown == NULL)
		return rv;

	HRESULT hr;
	CComBSTR bstrName;

	ASSERT(m_pData != NULL);
	if (iParentInterfaceType == CESYMBOL_ITYPE_PROJECT)
	{
		IProjectDBI* pIProjectDBI = NULL;
		hr = pParentIUnkown->QueryInterface(IID_IProjectDBI, (LPVOID *)&pIProjectDBI);
		ASSERT(SUCCEEDED(hr));
		ASSERT(pIProjectDBI != NULL);
		if (pIProjectDBI != NULL)
		{
			hr = pIProjectDBI->get_ConfigurationByIndex(iIndex, &m_pData->m_pIUnknown);
			ASSERT(SUCCEEDED(hr));
			ASSERT(m_pData->m_pIUnknown != NULL);
			if (m_pData->m_pIUnknown != NULL)
			{
				IConfigurationDBI* pIConfigurationDBI = NULL;
				hr = m_pData->m_pIUnknown->QueryInterface(IID_IConfigurationDBI, (LPVOID *)&pIConfigurationDBI);
				ASSERT(SUCCEEDED(hr));
				ASSERT(pIConfigurationDBI != NULL);
				if (pIConfigurationDBI != NULL)
				{
					hr = pIConfigurationDBI->get_ResourceCount(&m_pData->m_nChildren);
					ASSERT(SUCCEEDED(hr));	

//						pIConfigurationDBI->get_MachineAddress(&bstrName);
					pIConfigurationDBI->get_Name(&bstrName);
					m_pData->m_strName = bstrName;
//						m_pData->m_strName = "{" + m_pData->m_strName + "}";

					m_pData->m_Type.Set(CEWATCH_BASETYPE_CONFIGURATION, "Configuration");
					m_pData->m_InterFaceType = CESYMBOL_ITYPE_CONFIGURATION;
					pIConfigurationDBI->Release();
					rv = true;
				}
			}
			pIProjectDBI->Release();
		}
	}
	else if (iParentInterfaceType == CESYMBOL_ITYPE_CONFIGURATION)
	{
		IConfigurationDBI* pIConfigurationDBI = NULL;
		hr = pParentIUnkown->QueryInterface(IID_IConfigurationDBI, (LPVOID *)&pIConfigurationDBI);
		ASSERT(SUCCEEDED(hr));
		ASSERT(pIConfigurationDBI != NULL);
		if (pIConfigurationDBI != NULL)
		{
			hr = pIConfigurationDBI->get_ResourceByIndex(iIndex, &m_pData->m_pIUnknown);
			ASSERT(SUCCEEDED(hr));
			ASSERT(m_pData->m_pIUnknown != NULL);
			if (m_pData->m_pIUnknown != NULL)
			{
				IResourceDBI* pIResourceDBI = NULL;
				hr = m_pData->m_pIUnknown->QueryInterface(IID_IResourceDBI, (LPVOID *)&pIResourceDBI);
				ASSERT(SUCCEEDED(hr));
				ASSERT(pIResourceDBI != NULL);
				if (pIResourceDBI != NULL)
				{
					hr = pIResourceDBI->get_VariableCount(&m_pData->m_nChildren);
					ASSERT(SUCCEEDED(hr));

					pIResourceDBI->get_Name(&bstrName);
					m_pData->m_strName = bstrName;

					m_pData->m_Type.Set(CEWATCH_BASETYPE_RESOURCE, "Resource");
					m_pData->m_InterFaceType = CESYMBOL_ITYPE_RESOURCE;
					pIResourceDBI->Release();
					rv = true;
				}
			}
			pIConfigurationDBI->Release();
		}
	}
	else if (iParentInterfaceType == CESYMBOL_ITYPE_RESOURCE)
	{
		IUnknown* pIVariable = NULL;
		IVariableDBI* pIVariableDBI = NULL;
		IResourceDBI* pIResourceDBI = NULL;

		hr = pParentIUnkown->QueryInterface(IID_IResourceDBI, (LPVOID *)&pIResourceDBI);
		ASSERT(SUCCEEDED(hr));
		ASSERT(pIResourceDBI != NULL);
		if (pIResourceDBI != NULL)
		{
			hr = pIResourceDBI->get_VariableByIndex(iIndex, &pIVariable);
			ASSERT(SUCCEEDED(hr));
			ASSERT(pIVariable != NULL);
			if (pIVariable != NULL)
			{
				hr = pIVariable->QueryInterface(IID_IVariableDBI, (LPVOID *)&pIVariableDBI);
				ASSERT(SUCCEEDED(hr));
				ASSERT(pIVariableDBI != NULL);
				if (pIVariableDBI != NULL)
				{
					hr = pIVariableDBI->get_Type(&m_pData->m_pIUnknown);
					ASSERT(SUCCEEDED(hr));
					ASSERT(m_pData->m_pIUnknown != NULL);
					if (m_pData->m_pIUnknown != NULL)
					{
						pIVariableDBI->get_Name(&bstrName);
						m_pData->m_strName = bstrName;
						rv = InitByType(m_pData->m_pIUnknown, &m_pData->m_iMinIndex, &m_pData->m_nChildren);
						m_pData->m_Type.SetVarKind(VarGlobal);
						m_pData->m_InterFaceType = CESYMBOL_ITYPE_TYPE;
					}
					pIVariableDBI->Release();
				}
				pIVariable->Release();
			}
			pIResourceDBI->Release();
		}
	}
	else if (iParentInterfaceType == CESYMBOL_ITYPE_TYPE)
	{
		ITypeDBI* pITypeDBI = NULL;
		hr = pParentIUnkown->QueryInterface(IID_ITypeDBI, (LPVOID *)&pITypeDBI);
		ASSERT(SUCCEEDED(hr));
		ASSERT(pITypeDBI != NULL);
		if (pITypeDBI != NULL)
		{
			DBITypeKind Kind;
			pITypeDBI->get_Kind(&Kind);

			if (Kind == DBIArrayType)
			{
				hr = pITypeDBI->get_ElementType(&m_pData->m_pIUnknown);
				ASSERT(SUCCEEDED(hr));
				ASSERT(m_pData->m_pIUnknown != NULL);
				if (m_pData->m_pIUnknown != NULL)
				{
					m_pData->m_strName = "";
					rv = InitByType(m_pData->m_pIUnknown, &m_pData->m_iMinIndex, &m_pData->m_nChildren);
					m_pData->m_InterFaceType = CESYMBOL_ITYPE_TYPE;
				}
			}
			else 
			{
				ASSERT(Kind != DBISimpleType);

				unsigned long nVars, nSteps, nActions;
				pITypeDBI->get_VariableCount(&nVars);
				pITypeDBI->get_StepCount(&nSteps);
				pITypeDBI->get_ActionCount(&nActions);
				unsigned long iVar0 = 0;
				unsigned long iStep0 = iVar0 + nVars;
				unsigned long iAction0 = iStep0 + nSteps;

				// load variable
				if (iIndex >= iVar0 && iIndex < iVar0+nVars)
				{
					IUnknown* pIElem = NULL;
					IVariableDBI* pIElemDBI = NULL;

					hr = pITypeDBI->get_VariableByIndex(iIndex-iVar0, &pIElem);
					ASSERT(SUCCEEDED(hr));
					ASSERT(pIElem != NULL);
					if (pIElem != NULL)
					{
						hr = pIElem->QueryInterface(IID_IVariableDBI, (LPVOID *)&pIElemDBI);
						ASSERT(SUCCEEDED(hr));
						ASSERT(pIElemDBI != NULL);
						if (pIElemDBI != NULL)
						{
							hr = pIElemDBI->get_Type(&m_pData->m_pIUnknown);
							ASSERT(SUCCEEDED(hr));
							ASSERT(m_pData->m_pIUnknown != NULL);
							if (m_pData->m_pIUnknown != NULL)
							{
								pIElemDBI->get_Name(&bstrName);
								m_pData->m_strName = bstrName;
								rv = InitByType(m_pData->m_pIUnknown, &m_pData->m_iMinIndex, &m_pData->m_nChildren);
								m_pData->m_InterFaceType = CESYMBOL_ITYPE_TYPE;
								m_pData->m_Type.SetVarKind(GetVarKind(pIElemDBI));
							}						
							pIElemDBI->Release();
						}
						pIElem->Release();
					}
				}
				// load step
				else if (iIndex >= iStep0 && iIndex < iStep0+nSteps)
				{
					IUnknown* pIElem = NULL;
					IStepDBI* pIElemDBI = NULL;

					hr = pITypeDBI->get_StepByIndex(iIndex-iStep0, &pIElem);
					ASSERT(SUCCEEDED(hr));
					ASSERT(pIElem != NULL);
					if (pIElem != NULL)
					{
						hr = pIElem->QueryInterface(IID_IStepDBI, (LPVOID *)&pIElemDBI);
						ASSERT(SUCCEEDED(hr));
						ASSERT(pIElemDBI != NULL);
						if (pIElemDBI != NULL)
						{
							hr = pIElemDBI->get_Type(&m_pData->m_pIUnknown);
							ASSERT(SUCCEEDED(hr));
							ASSERT(m_pData->m_pIUnknown != NULL);
							if (m_pData->m_pIUnknown != NULL)
							{
								pIElemDBI->get_Name(&bstrName);
								m_pData->m_strName = bstrName;
								rv = InitByType(m_pData->m_pIUnknown, &m_pData->m_iMinIndex, &m_pData->m_nChildren);
								m_pData->m_InterFaceType = CESYMBOL_ITYPE_TYPE;
							}						
							pIElemDBI->Release();
						}
						pIElem->Release();
					}
				}
				// load action
				else if (iIndex >= iAction0 && iIndex < iAction0+nActions)
				{
					IUnknown* pIElem = NULL;
					IActionDBI* pIElemDBI = NULL;

					hr = pITypeDBI->get_ActionByIndex(iIndex-iAction0, &pIElem);
					ASSERT(SUCCEEDED(hr));
					ASSERT(pIElem != NULL);
					if (pIElem != NULL)
					{
						hr = pIElem->QueryInterface(IID_IActionDBI, (LPVOID *)&pIElemDBI);
						ASSERT(SUCCEEDED(hr));
						ASSERT(pIElemDBI != NULL);
						if (pIElemDBI != NULL)
						{
							hr = pIElemDBI->get_Type(&m_pData->m_pIUnknown);
							ASSERT(SUCCEEDED(hr));
							ASSERT(m_pData->m_pIUnknown != NULL);
							if (m_pData->m_pIUnknown != NULL)
							{
								pIElemDBI->get_Name(&bstrName);
								m_pData->m_strName = bstrName;
								rv = InitByType(m_pData->m_pIUnknown, &m_pData->m_iMinIndex, &m_pData->m_nChildren);
								m_pData->m_InterFaceType = CESYMBOL_ITYPE_TYPE;
							}						
							pIElemDBI->Release();
						}
						pIElem->Release();
					}
				}
			}
			
			if (m_pParent->GetType().GetVarKind() > m_pData->m_Type.GetVarKind())
				m_pData->m_Type.SetVarKind(m_pParent->GetType().GetVarKind());

			pITypeDBI->Release();
		}
	}

	return rv;
}

//*****************************************************************************	
ECeWatchVarKind CCeSymbol::GetVarKind(IVariableDBI* pIVariableDBI)
//*****************************************************************************
{
	DBIVariableKind VarKind;								
	pIVariableDBI->get_Kind(&VarKind);
	switch (VarKind)
	{
		case DBIVarInput :
			return VarInput;
		case DBIVarOutput:
			return VarOutput;
		case DBIVarInOut :
			return VarInOut;
		case DBIVarGlobal :
			return VarGlobal;
		case DBIVarExternal :
			return VarExternal;
		case DBIConst :
			return Const;
		default :
			return Var;
	}
}

//*****************************************************************************	
bool CCeSymbol::InitByType(IUnknown* pIType, long* piMinIndex, unsigned long* piChildrenCount)
//*****************************************************************************
{
	return CCeSymbol::ConvertType(pIType, m_pData->m_Type, *piMinIndex, *piChildrenCount);
}

//*****************************************************************************	
bool CCeSymbol::ConvertType(IUnknown* pIType, CCeWatchType &type, long& MinIndex, unsigned long& ChildrenCount)
//*****************************************************************************
{
	bool rv = false;
	MinIndex = 0;
	ChildrenCount = 0;
	if (pIType != NULL)
	{
		ITypeDBI* pITypeDBI = NULL;
		HRESULT hr = pIType->QueryInterface(IID_ITypeDBI, (LPVOID *)&pITypeDBI);
		ASSERT(SUCCEEDED(hr));
		ASSERT(pITypeDBI != NULL);
		if (pITypeDBI != NULL)
		{
			DBITypeKind Kind;

			pITypeDBI->get_Kind(&Kind);
			if (Kind == DBISimpleType)
			{
				rv = type.SetSimpleType(CCeSymbol::GetTypeName(pITypeDBI));
			}
			else if (Kind == DBIArrayType)
			{
				long iLower, iUpper;
				pITypeDBI->get_LowerBound(&iLower);	
				pITypeDBI->get_UpperBound(&iUpper);	

				IUnknown* pIElemType = NULL;
				hr = pITypeDBI->get_ElementType(&pIElemType);
				ASSERT(SUCCEEDED(hr));
				ASSERT(pIElemType != NULL);
				if (pIElemType != NULL)
				{
					ITypeDBI* pIElemTypeDBI;
					HRESULT hr = pIElemType->QueryInterface(IID_ITypeDBI, (LPVOID *)&pIElemTypeDBI);
					ASSERT(SUCCEEDED(hr));
					ASSERT(pIElemTypeDBI != NULL);
					if (pIElemTypeDBI != NULL)
					{
						rv = type.SetArrayType(CCeSymbol::GetTypeName(pIElemTypeDBI), iLower, iUpper);
						if (rv)
						{
							MinIndex = iLower;
							ChildrenCount = iUpper - iLower + 1;
						}
						pIElemTypeDBI->Release();
					}
					pIElemType->Release();
				}
			}
			else
			{
                switch(Kind)
                {
                case DBIStructType:
					rv = type.Set(CEWATCH_BASETYPE_STRUCTURE, CCeSymbol::GetTypeName(pITypeDBI));
                    break;
                case DBIProgramType:
					rv = type.Set(CEWATCH_BASETYPE_PROGRAM, CCeSymbol::GetTypeName(pITypeDBI));
                    break;
                case DBIFunctionBlockType:
					rv = type.Set(CEWATCH_BASETYPE_FUNCTIONBLOCK, CCeSymbol::GetTypeName(pITypeDBI));
                    break;
                case DBIResourceType:
					rv = type.Set(CEWATCH_BASETYPE_RESOURCE, CCeSymbol::GetTypeName(pITypeDBI));
                    break;
                case DBIConfigurationType:
					rv = type.Set(CEWATCH_BASETYPE_CONFIGURATION, CCeSymbol::GetTypeName(pITypeDBI));
                    break;
                case DBIStepType:
					rv = type.Set(CEWATCH_BASETYPE_STEP, "STEP");
                    break;
                case DBIActionType:
					rv = type.Set(CEWATCH_BASETYPE_ACTION, "ACTION");
                    break;
                default:
                    rv = type.SetUnknown();
                }
				if (rv)
				{
					unsigned long n;
					ChildrenCount = 0;
					pITypeDBI->get_VariableCount(&n);
					ChildrenCount += n;
					pITypeDBI->get_StepCount(&n);
					ChildrenCount += n;
					pITypeDBI->get_ActionCount(&n);
					ChildrenCount += n;
					MinIndex = 0;
				}
			}

			pITypeDBI->Release();
		}
	}
	return rv;
}

//*****************************************************************************	
CString CCeSymbol::GetTypeName(ITypeDBI* pITypeDBI)
//*****************************************************************************
{
	CString strTypeName = "Unknown";
	if (pITypeDBI != NULL)
	{
		HRESULT hr;
		DBITypeKind Kind;

		pITypeDBI->get_Kind(&Kind);
		if (Kind == DBISimpleType)
		{
			short iIECType;
			hr = pITypeDBI->get_IECType(&iIECType);	
			ASSERT(SUCCEEDED(hr));
			strTypeName = BL_DatatypeToTypename((BL_IEC_TYP)iIECType, 0);
		}
		else
		{
			CComBSTR bstrTypeName;
			hr = pITypeDBI->get_TypeName(&bstrTypeName);						
			ASSERT(SUCCEEDED(hr));
			strTypeName = bstrTypeName;
		}
	}
	return strTypeName;
}

//*****************************************************************************	
CCeSymbol* CCeSymbol::GetChild(unsigned long iIndex)
//*****************************************************************************
{
	ASSERT(iIndex < GetChildrenCount());
	if (iIndex < GetChildrenCount())
	{
		if (m_pChildrenArray == NULL)
		{
			if (!LoadChildren())
            {
                delete [] m_pChildrenArray;
                m_pChildrenArray = NULL;
                m_pData->m_nChildren = 0;
                m_pData->m_iMinIndex = 0;
                m_pData->m_InterFaceType = -1;
                return NULL;
            }
			ASSERT(m_pChildrenArray != NULL);
		}
		if (m_pChildrenArray != NULL)
			return m_pChildrenArray + iIndex;
	}
	return NULL;
}

//*****************************************************************************
CCeSymbol* CCeSymbol::FindChild(LPCTSTR pName)
//*****************************************************************************
{
	CString strName = pName;
	strName.TrimLeft();
	strName.TrimRight();

	bool bShort = false;	
	if (strName.Find('.') < 0)
		bShort = true;

	return _FindChild(strName, bShort);
}

//*****************************************************************************
CCeSymbol* CCeSymbol::_FindChild(LPCTSTR pName, bool bShort)
//*****************************************************************************
{
	CString strChildName;

	CCeSymbol* pChild = NULL;
	unsigned long ii;
	unsigned long iCount = GetChildrenCount();
	for (ii = 0; ii < iCount; ii++)
	{
		pChild = GetChild(ii);
		if (bShort)
			strChildName = pChild->GetName();
		else
			strChildName = pChild->GetFullName();

		if (strChildName.CompareNoCase(pName) == 0)
			return pChild;
	}

	for (ii = 0; ii < iCount; ii++)
	{
		pChild = GetChild(ii)->_FindChild(pName, bShort);
		if (pChild != NULL)
			return pChild;
	}		

	return NULL;
}

// 10.10.06 SIS >>
CCeSymbol* CCeSymbol::FindChildRecursive(LPCTSTR pName)
//*****************************************************************************
{
	CCeSymbol* pReturn = this;

	CString	strName(pName);
	CString	strSubPath;

	strName.Replace(_T("["), _T(".["));

	if(strName.IsEmpty())
	{
		return pReturn;
	}

	int	iStart = 0;
	int	iStop;

	BOOL	bReady = FALSE;
	while(!bReady && pReturn)
	{
		iStop = strName.Find(_T('.'), iStart);
		if(iStop == -1)
		{
			iStop = strName.GetLength();
			bReady = TRUE;
		}
		strSubPath = strName.Mid(iStart, iStop-iStart);
		pReturn = pReturn->_FindChild(strSubPath, TRUE);
		iStart = iStop + 1;
	}
	return pReturn;
}
// 10.10.06 SIS <<


// #2149 27.10.04 SIS >>
BOOL CCeSymbol::GetExpandFlag()
{
	return m_bExpandFlag;
}

void CCeSymbol::SetExpandFlag(BOOL bExpandFlag)
{
	m_bExpandFlag = bExpandFlag;
}
// #2149 27.10.04 SIS <<



//*****************************************************************************
CCeSymbolData::CCeSymbolData()
//*****************************************************************************
{
	m_nChildren = 0;
	m_iMinIndex = 0;
	m_pIUnknown = NULL;
	m_InterFaceType = -1;
}

//*****************************************************************************
CCeSymbolData::~CCeSymbolData()
//*****************************************************************************
{
	m_nChildren = 0;
	m_iMinIndex = 0;

	if (m_pIUnknown != NULL)
	{
		m_pIUnknown->Release();
		m_pIUnknown = NULL;
	}

	m_InterFaceType = -1;
}


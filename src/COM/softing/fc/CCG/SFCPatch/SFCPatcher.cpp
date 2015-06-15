

#include "stdafx.h"
#include "SFCPatcher.h"
#include "XML_Dings.h"
#include "DbgInfo_i.c"
#include "XML_4Gr_Names.h"
#include "..\cg_tools\XML_DBI_Names.h"
#include "XML_Help.h"

#define XML_4GR_ELEM_PROGRAM _T("PROGRAM")
#define XML_4GR_ELEM_FUNCTION_BLOCK _T("FUNCTION_BLOCK")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSFCPatcher::CSFCPatcher(const CString& strProjectPath)
{
	m_pIDebugInfo=NULL;
	m_pIProjectDBI=NULL;
	m_strProjectPath=strProjectPath;
}

CSFCPatcher::~CSFCPatcher()
{
	if (m_pIDebugInfo != NULL)
		m_pIDebugInfo->Release();
	
	if (m_pIProjectDBI != NULL)
		m_pIProjectDBI->Release();
}

HRESULT CSFCPatcher::Patch(const CString& strOrigFile, const CString& strPatchedFile, const CString& strSfcPath, BOOL bDbiFile)
{
	HRESULT retValue=S_OK;
	CString strAllErrors;

	CSFCELEMARRAY sfcSteps, sfcTransitions, sfcActions;
	
	//
	// get debug info
	//
	if (bDbiFile == TRUE)
	{
		//
		// debug info from dbi FILE
		//
		CStdioFile dbiFile;
		CFileException fileError;

		BOOL bF=dbiFile.Open(strSfcPath, CFile::modeRead|CFile::typeBinary, &fileError);
		if (bF==FALSE)
		{
			dbiFile.Close();
			return SetError(E_FAIL, "can't open " + strSfcPath);
		}

		CXML_Dings xmlDbi(&dbiFile);

		try
		{
			xmlDbi.Load();

		}
		catch (HRESULT hrError)
		{
			hrError=hrError;
			CString strError;
			xmlDbi.GetLastParseError(strError);
			return SetError(E_FAIL, "Parse error " + dbiFile.GetFilePath() + " " + strError);
		}

		HRESULT hr=GetTransitionsDBIFile(xmlDbi, sfcTransitions);
		if ( hr != S_OK ) return SetError(hr, "");

		hr=GetStepsDBIFile(xmlDbi, sfcSteps);
		if ( hr != S_OK ) return SetError(hr, "");

		hr=GetActionsDBIFile(xmlDbi, sfcActions);
		if ( hr != S_OK ) return SetError(hr, "");
	}
	else
	{
		//
		// get debug info from debug interface
		//
		ITypeDBI* pITypeDBI;

		HRESULT hr=GetITypeDBI(strSfcPath, &pITypeDBI);
		if ( hr != S_OK )
		{
			pITypeDBI->Release();
			return SetError(hr, "");
		}
		hr = GetTransitionsDBI(pITypeDBI, sfcTransitions);
		if ( hr != S_OK )
		{
			pITypeDBI->Release();
			return SetError(hr, "");
		}
		hr = GetStepsDBI(pITypeDBI, sfcSteps);
		if ( hr != S_OK )
		{
			pITypeDBI->Release();
			return SetError(hr, "");
		}	
		hr = GetActionsDBI(pITypeDBI, sfcActions);
		if ( hr != S_OK )
		{
			pITypeDBI->Release();
			return SetError(hr, "");
		}
		pITypeDBI->Release();
	}

	//
	// load xml grafics
	//
	CStdioFile origFile, patchedFile;
	CFileException fileError;

	BOOL bF=origFile.Open(strOrigFile, CFile::modeRead|CFile::typeBinary, &fileError);
	if (bF==FALSE)
	{
		return SetError(E_FAIL, "can't open " + strOrigFile);
	}

	bF=patchedFile.Open(strPatchedFile, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary, &fileError);
	if (bF==FALSE)
	{
		return SetError(E_FAIL, "can't open " + strPatchedFile);
	}

	CXML_Dings xmlDoc(&origFile, &patchedFile);
	CRECTARRAY actionRects;

	HRESULT hr;
	try
	{
		xmlDoc.Load();

	}
	catch (HRESULT hrError)
	{
		hrError=hrError;
		CString strError;
		xmlDoc.GetLastParseError(strError);
		SetError(E_FAIL, "Parse error " + strOrigFile + " " + strError);
		goto ende;
	}

	strAllErrors = GetLastErrorMessage();
	hr=PatchSteps(&xmlDoc, sfcSteps);
	if (hr != S_OK)
	{
		strAllErrors += "\n" + GetLastErrorMessage();
		retValue=hr;
	}

	hr=PatchTransitions(&xmlDoc, sfcTransitions);
	if (hr != S_OK)
	{
		strAllErrors += "\n" + GetLastErrorMessage();
		retValue=hr;
	}

	hr=PatchActions(&xmlDoc, sfcActions);
	if (hr != S_OK)
	{
		strAllErrors += "\n" + GetLastErrorMessage();
		retValue=hr;
	}
	
	SetError(hr, strAllErrors);


	//
	// remove elements (except commments) in action/actionblocks
	//
	HRESULT hr2;
	hr2=GetActionRects(&xmlDoc, actionRects);
	hr2=RemoveElementsInRects(&xmlDoc, actionRects);
	hr2=PatchComments(&xmlDoc);

	xmlDoc.Flush();

ende:
	try
	{
		patchedFile.Close();
	}
	catch (CFileException ex)
	{
	}

	return SetError(retValue, strAllErrors);
}

HRESULT CSFCPatcher::PatchSteps(CXML_Dings* pXmlDoc, CSFCELEMARRAY& sfcSteps)
{
	return PatchElements(pXmlDoc, sfcSteps, XML_4GR_ELEM_STEP);
}

HRESULT CSFCPatcher::PatchTransitions(CXML_Dings* pXmlDoc, CSFCELEMARRAY& sfcTransitions)
{
	return PatchElements(pXmlDoc, sfcTransitions, XML_4GR_ELEM_TRANS);
}

HRESULT CSFCPatcher::PatchActions(CXML_Dings* pXmlDoc, CSFCELEMARRAY& sfcActions)
{
	return PatchElements(pXmlDoc, sfcActions, XML_4GR_ELEM_ACTION);
}

HRESULT CSFCPatcher::PatchElements(CXML_Dings* pXmlDoc, CSFCELEMARRAY& sfcElements, const CString& strElementName)
{
	HRESULT hr, hrReturn = S_OK;
	//
	// goto body
	//
	try
	{
		pXmlDoc->MoveToRoot();
		BOOL bN=pXmlDoc->MoveDown(XML_4GR_ELEM_BODY);
		if (bN!=TRUE)
		{
			return SetError(S_FALSE,"Body tag not found in 4gr file");
		}
	}
	catch (HRESULT hrNoBody)
	{
		hrNoBody=hrNoBody;
		return SetError(S_FALSE,"Body tag not found in 4gr file");
	}

	//
	// patch steps
	//
	ULONG ulComment;
	try
	{
		CString strAllErrors = GetLastErrorMessage();
		BOOL bS=pXmlDoc->MoveDown(strElementName);
		if (bS==TRUE)
		{
			BOOL bA=pXmlDoc->GetAttribute(XML_4GR_ELEM_COMMENT, ulComment);
			//
			//patch step
			//
			if ( !(bA==TRUE && ulComment==1))
			{
				hr=PatchElement(pXmlDoc, sfcElements);
				if (hr != S_OK)
				{
					hrReturn=hr;
					strAllErrors = strAllErrors + "\n" + GetLastErrorMessage();
					SetError(hr, strAllErrors);
				}
			}
			while (pXmlDoc->MoveNext() == TRUE)
			{
				BOOL bA=pXmlDoc->GetAttribute(XML_4GR_ELEM_COMMENT, ulComment);
				//
				// patch step
				//
				if ( !(bA==TRUE && ulComment==1) )
				{
					hr=PatchElement(pXmlDoc, sfcElements);
					if (hr != S_OK)
					{
						hrReturn=hr;
						strAllErrors = strAllErrors + "\n" + GetLastErrorMessage();
						SetError(hr, strAllErrors);
					}
				}
			}
		}
	}
	catch (HRESULT hrError)
	{
		hrError=hrError;
		CString strError;
		pXmlDoc->GetLastParseError(strError);
		return SetError(E_FAIL, strError);
	}

	return SetError(hrReturn);
}

HRESULT CSFCPatcher::PatchElement(CXML_Dings* pXmlDoc, CSFCELEMARRAY& sfcElements)
{
	//
	// read step
	//
	CString strName;
	CPoint ptOrigin;
	try
	{
		BOOL bA1=pXmlDoc->GetAttribute(XML_4GR_ATTR_NAME, strName);
		BOOL bA2=pXmlDoc->GetAttribute(XML_4GR_ATTR_ORIGIN, ptOrigin);
		if ( bA1 != TRUE || bA2 != TRUE )
			return SetError(E_FAIL,"Error reading attributes in 4GR file");		
	}
	catch (HRESULT hrError)
	{
		hrError=hrError;
		CString strError;
		pXmlDoc->GetLastParseError(strError);
		return SetError(E_FAIL,"Error reading attributes in 4GR file");
	}

	//
	// search step in dbi
	//
	BOOL bFound;
	unsigned long ulNumber=-1;
	for (int ii=0; ii< sfcElements.GetSize(); ii++)
	{
		CSFCElem sfcElem=sfcElements.GetAt(ii);
//		if ( strName.CompareNoCase(sfcElem.Name) == 0 
//			&& (long) sfcElem.X == ptOrigin.x && (long) sfcElem.Y == ptOrigin.y)
		if ( (long) sfcElem.X == ptOrigin.x && (long) sfcElem.Y == ptOrigin.y )
		{
			bFound = TRUE;
			ulNumber=sfcElem.Number;
			break;
		}
	}

	if (bFound != TRUE)
	{
        // special handling for sfc exception action 
        // don't show error message for this special action
        // that is not handled in dbi like other actions and 
        // that is not bound to a step.
        if (strName.Compare(_T("ExceptionAction"))!=0)
        {
		    CString strFormat(_T(" at (%d,%d)"));
		    CString strOrigin;
		    strOrigin.Format(strFormat, ptOrigin.x, ptOrigin.y);
		    return SetError(E_FAIL, "Element not found in 4GR file:" + strName + strOrigin) ;
        }
	}
	else
	{
		pXmlDoc->SetAttribute(XML_DBI_ATTR_ID,ulNumber);
	}

	return SetError(S_OK);
}

HRESULT CSFCPatcher::GetITypeDBI(const CString& strSfcPath,	ITypeDBI** pITypeDBI)
{
	HRESULT hr=S_OK;

	//
	// init debug info server
	//
	if (m_pIDebugInfo == NULL)
	{
		hr = CoCreateInstance(CLSID_DebugInfoServer, NULL, CLSCTX_ALL,
			IID_IDebugInfo, (void**)&m_pIDebugInfo);
		if (hr != S_OK) return SetError(hr, "CoCreateInstance DebugInfoServer failed");

		CString strDbiPath = m_strProjectPath + "\\gen";
		CComBSTR sDbiPath(strDbiPath);
		hr = m_pIDebugInfo->Init(sDbiPath);
		if ( hr != S_OK) return SetError(hr,"init of DebugInfoServer faild. Check project path");
	}
	
	//
	// get project interface
	//
	if (m_pIProjectDBI == NULL)
	{
		IUnknown* pUnk;
		hr=m_pIDebugInfo->get_Project(&pUnk);
		if ( hr != S_OK) return SetError(hr,"DebugInfoServer: can't get project");
		hr=pUnk->QueryInterface(IID_IProjectDBI, (LPVOID *)&m_pIProjectDBI);
		pUnk->Release();
		if (hr != S_OK) return SetError(hr,"DebugInfoServer: can't get project");
	}

	//
	// get instance interface
	//
	IInstanceDBI* pIInstanceDBI;
	IUnknown* pUnk;
	CComBSTR sSfcPath(strSfcPath);
	hr=m_pIProjectDBI->get_InstanceByPath(sSfcPath, &pUnk);
	if (hr != S_OK) return SetError(hr,"DebugInfoServer: can't get SFC instance");
	hr=pUnk->QueryInterface(IID_IInstanceDBI, (LPVOID *)&pIInstanceDBI);
	pUnk->Release();
	if (hr != S_OK) return SetError(hr, "DebugInfoServer: can't get SFC instance");

	//
	// get Type interface
	//
	IUnknown* pUnk2;
	hr=pIInstanceDBI->get_Type(&pUnk2);
	pIInstanceDBI->Release();
	if (hr != S_OK) return SetError(hr, "DebugInfoServer: can't get type interface");
	hr=pUnk2->QueryInterface(IID_ITypeDBI, (LPVOID *)pITypeDBI);
	pUnk2->Release();
	
	return SetError(hr);
}

HRESULT CSFCPatcher::GetStepsDBI(ITypeDBI* pITypeDBI, CSFCELEMARRAY& sfcElems)
{
	HRESULT hr=S_OK;
	
	sfcElems.RemoveAll();

	unsigned long ulCount;
	hr=pITypeDBI->get_StepCount(&ulCount);
	if (hr != S_OK) return SetError(hr, "DebugInfoServer: can't get steps");

	for (unsigned long ii=0; ii<ulCount; ii++)
	{
		IUnknown* pUnk;
		hr=pITypeDBI->get_StepByIndex(ii, &pUnk);
		ASSERT (hr == S_OK);
		IStepDBI* pStepDBI;
		hr = pUnk->QueryInterface(IID_IStepDBI, (LPVOID *) &pStepDBI);
		pUnk->Release();
		ASSERT(hr == S_OK);
		
		BSTR sName;
		CString strName;
		long lX, lY, lNumber; 
		
		hr=pStepDBI->get_Name(&sName);
		ASSERT(hr == S_OK);
		strName=sName;
		SysFreeString(sName);
		
		hr=pStepDBI->get_Number(&lNumber);
		ASSERT(hr == S_OK);

		hr=pStepDBI->get_X(&lX);
		ASSERT(hr == S_OK);

		hr=pStepDBI->get_Y(&lY);
		ASSERT(hr == S_OK);

		pStepDBI->Release();

		CSFCElem sfcElem(strName, lNumber, lX, lY);

		sfcElems.Add(sfcElem);
	}

	return SetError(S_OK, "");
}

HRESULT CSFCPatcher::GetTransitionsDBI(ITypeDBI* pITypeDBI, CSFCELEMARRAY& sfcElems)
{
	HRESULT hr=S_OK;
	
	sfcElems.RemoveAll();

	unsigned long ulCount;
	hr=pITypeDBI->get_TransitionCount(&ulCount);
	if (hr != S_OK) return SetError(hr, "DebugInfoServer: can't get transitions");

	for (unsigned long ii=0; ii<ulCount; ii++)
	{
		IUnknown* pUnk;
		hr=pITypeDBI->get_TransitionByIndex(ii, &pUnk);
		ASSERT (hr == S_OK);
		ITransitionDBI* pTransitionDBI;
		hr = pUnk->QueryInterface(IID_ITransitionDBI, (LPVOID *) &pTransitionDBI);
		pUnk->Release();
		ASSERT(hr == S_OK);
		
		BSTR sName;
		CString strName;
		long lX, lY, lNumber; 
		
		hr=pTransitionDBI->get_Name(&sName);
		ASSERT(hr == S_OK);
		strName=sName;
		SysFreeString(sName);
		
		hr=pTransitionDBI->get_Number(&lNumber);
		ASSERT(hr == S_OK);

		hr=pTransitionDBI->get_X(&lX);
		ASSERT(hr == S_OK);

		hr=pTransitionDBI->get_Y(&lY);
		ASSERT(hr == S_OK);

		pTransitionDBI->Release();

		CSFCElem sfcElem(strName, lNumber, lX, lY);

		sfcElems.Add(sfcElem);
	}

	return SetError(S_OK);
}

HRESULT CSFCPatcher::GetActionsDBI(ITypeDBI* pITypeDBI, CSFCELEMARRAY& sfcElems)
{
	HRESULT hr=S_OK;
	
	sfcElems.RemoveAll();

	unsigned long ulCount;
	hr=pITypeDBI->get_ActionCount(&ulCount);
	if (hr != S_OK) return SetError(hr, "DebugInfoServer: can't get actions");

	for (unsigned long ii=0; ii<ulCount; ii++)
	{
		IUnknown* pUnk;
		hr=pITypeDBI->get_ActionByIndex(ii, &pUnk);
		ASSERT (hr == S_OK);
		IActionDBI* pActionDBI;
		hr = pUnk->QueryInterface(IID_IActionDBI, (LPVOID *) &pActionDBI);
		pUnk->Release();
		ASSERT(hr == S_OK);
		
		BSTR sName;
		CString strName;
		long lX, lY, lNumber; 
		
		hr=pActionDBI->get_Name(&sName);
		ASSERT(hr == S_OK);
		strName=sName;
		SysFreeString(sName);
		
		hr=pActionDBI->get_Number(&lNumber);
		ASSERT(hr == S_OK);

		hr=pActionDBI->get_X(&lX);
		ASSERT(hr == S_OK);

		hr=pActionDBI->get_Y(&lY);
		ASSERT(hr == S_OK);

		pActionDBI->Release();

		CSFCElem sfcElem(strName, lNumber, lX, lY);

		sfcElems.Add(sfcElem);
	}

	return SetError(S_OK);
}
	
HRESULT CSFCPatcher::GetStepsDBIFile(CXML_Dings& xmlDbi, CSFCELEMARRAY& sfcElems)
{
	return GetElementsDBIFile(xmlDbi, XML_DBI_ELEM_STEP, sfcElems);
}

HRESULT CSFCPatcher::GetActionsDBIFile(CXML_Dings& xmlDbi, CSFCELEMARRAY& sfcElems)
{
	return GetElementsDBIFile(xmlDbi, XML_DBI_ELEM_ACTION, sfcElems);
}

HRESULT CSFCPatcher::GetTransitionsDBIFile(CXML_Dings& xmlDbi, CSFCELEMARRAY& sfcElems)
{
	return GetElementsDBIFile(xmlDbi, XML_DBI_ELEM_TRANSITION, sfcElems);
}

HRESULT CSFCPatcher::GetElementsDBIFile(CXML_Dings& xmlDoc, const CString& strElementName, CSFCELEMARRAY& sfcElems)
{
	HRESULT hr=S_OK;

	//
	// goto SFC
	//
	try
	{
		xmlDoc.MoveToRoot();
		BOOL bN=xmlDoc.MoveDown(XML_4GR_ELEM_PROGRAM);
		if (bN!=TRUE)
		{
			xmlDoc.MoveToRoot();
			bN=xmlDoc.MoveDown(XML_4GR_ELEM_FUNCTION_BLOCK);
			if (bN != TRUE)
				return SetError(S_FALSE,"Program or function block tag not found in dbi file");
		}
		bN=xmlDoc.MoveDown(XML_DBI_ELEM_SFC);
		if (bN!=TRUE)
		{
			return SetError(S_FALSE,"SFC tag not found in dbi file");
		}
	}
	catch (HRESULT hrNoBody)
	{
		hrNoBody=hrNoBody;
		return SetError(S_FALSE,"SFC tag not found in dbi file");
	}

	try
	{
		BOOL bS=xmlDoc.MoveDown(strElementName);
		if (bS==TRUE)
		{

			//
			// read element
			//
			CSFCElem sfcElem;
			BOOL bA1=xmlDoc.GetAttribute(XML_DBI_ATTR_NUM,  sfcElem.Number);
			BOOL bA2=xmlDoc.GetAttribute(XML_DBI_ATTR_X, sfcElem.X);
			BOOL bA3=xmlDoc.GetAttribute(XML_DBI_ATTR_Y, sfcElem.Y);
			if ( bA1 != TRUE || bA2 != TRUE  || bA3 != TRUE)
				return SetError(E_FAIL,"Error reading attributes in DBI file");
			sfcElems.Add(sfcElem);
			while (xmlDoc.MoveNext() == TRUE)
			{
				//
				// read element
				//
				CSFCElem sfcElem;
				BOOL bA1=xmlDoc.GetAttribute(XML_DBI_ATTR_NUM, sfcElem.Number);
				BOOL bA2=xmlDoc.GetAttribute(XML_DBI_ATTR_X, sfcElem.X);
				BOOL bA3=xmlDoc.GetAttribute(XML_DBI_ATTR_Y, sfcElem.Y);
				if ( bA1 != TRUE || bA2 != TRUE  || bA3 != TRUE)
					return SetError(E_FAIL,"Error reading attributes in DBI file");
				sfcElems.Add(sfcElem);
			}
		}
	}
	catch (HRESULT hrError)
	{
		hrError=hrError;
		CString strError;
		xmlDoc.GetLastParseError(strError);
		return SetError(E_FAIL, strError);
	}

	return SetError(hr);
}

HRESULT CSFCPatcher::GetActionRects(CXML_Dings* pXmlDoc, CRECTARRAY& actionRects)
{
	HRESULT hr, hrReturn = S_OK;

	actionRects.RemoveAll();
	//
	// goto body
	//
	try
	{
		pXmlDoc->MoveToRoot();
		BOOL bN=pXmlDoc->MoveDown(XML_4GR_ELEM_BODY);
		if (bN!=TRUE)
		{
			return SetError(S_FALSE,"Body tag not found in 4gr file");
		}
	}
	catch (HRESULT hrNoBody)
	{
		hrNoBody=hrNoBody;
		return SetError(S_FALSE,"Body tag not found in 4gr file");
	}

	IXMLElement2* pBody;	
	hr=pXmlDoc->GetCurrentElement(&pBody);
	ASSERT(hr==S_OK);

	IXMLElementCollection* pIXMLElementCollection;
	hr=pBody->get_children(&pIXMLElementCollection);
	ASSERT(hr==S_OK);
	pBody->Release();

	//
	// loop over children and find ACTION and ACTIONBLOCK
	//
	long lCnt=0;
	hr=pIXMLElementCollection->get_length(&lCnt);
	ASSERT(hr==S_OK);

	for (long ii=0; ii<lCnt; ii++)
	{
		IXMLElement2* pIElement;	
		hr=XML_CollectionGetItem(pIXMLElementCollection, ii, &pIElement);
		ASSERT(hr==S_OK);
		long lType;
		hr=pIElement->get_type(&lType);
		ASSERT(hr==S_OK);
		XMLELEM_TYPE type = (XMLELEM_TYPE) lType;
		if (type == XMLELEMTYPE_ELEMENT)
		{
			CString strName;
			hr=XML_ElementGetTagName(pIElement, strName);
			ASSERT(hr==S_OK);
			if (strName.CompareNoCase(XML_4GR_ELEM_ACTBLCK)==0 ||
				strName.CompareNoCase(XML_4GR_ELEM_ACTION)==0)
			{
				CPoint ptOrigin;
				CSize siSize;
				HRESULT hr1=XML_ElementGetAttribute(pIElement, XML_4GR_ATTR_ORIGIN, ptOrigin);
				HRESULT hr2=XML_ElementGetAttribute(pIElement, XML_4GR_ATTR_SIZE, siSize);
				if (hr1==S_OK && hr2==S_OK)
				{
					CRect rect(ptOrigin, siSize);
					actionRects.Add(rect);
				}
			}
		}
		
		pIElement->Release();
	}

	pIXMLElementCollection->Release();

	return S_OK;
}

HRESULT CSFCPatcher::RemoveLinesInRects(CXML_Dings* pXmlDoc, const CRECTARRAY& actionRects)
{
	HRESULT hr, hrReturn = S_OK;

	//
	// goto body
	//
	try
	{
		pXmlDoc->MoveToRoot();
		BOOL bN=pXmlDoc->MoveDown(XML_4GR_ELEM_BODY);
		if (bN!=TRUE)
		{
			return SetError(S_FALSE,"Body tag not found in 4gr file");
		}
	}
	catch (HRESULT hrNoBody)
	{
		hrNoBody=hrNoBody;
		return SetError(S_FALSE,"Body tag not found in 4gr file");
	}

	IXMLElement2* pIBody;	
	hr=pXmlDoc->GetCurrentElement(&pIBody);
	ASSERT(hr==S_OK);

	IXMLElementCollection* pIXMLElementCollection;
	hr=pIBody->get_children(&pIXMLElementCollection);
	ASSERT(hr==S_OK);

	//
	// loop over children, find LINES and delete them if they are in a rect
	//
	long lCnt=0;
	hr=pIXMLElementCollection->get_length(&lCnt);
	ASSERT(hr==S_OK);

	for (long ii=0; ii<lCnt; ii++)
	{
		IXMLElement2* pIElement;	
		hr=XML_CollectionGetItem(pIXMLElementCollection, ii, &pIElement);
		ASSERT(hr==S_OK);
		long lType;
		hr=pIElement->get_type(&lType);
		ASSERT(hr==S_OK);
		XMLELEM_TYPE type = (XMLELEM_TYPE) lType;
		if (type == XMLELEMTYPE_ELEMENT)
		{
			CString strName;
			hr=XML_ElementGetTagName(pIElement, strName);
			ASSERT(hr==S_OK);
			if (strName.CompareNoCase(XML_4GR_ELEM_LINE)==0 )
			{
				CPoint ptOrigin;
				CSize siSize;
				HRESULT hr1=XML_ElementGetAttribute(pIElement, XML_4GR_ATTR_ORIGIN, ptOrigin);
				HRESULT hr2=XML_ElementGetAttribute(pIElement, XML_4GR_ATTR_SIZE, siSize);
				if (hr1==S_OK && hr2==S_OK)
				{
					if (LineInRects(ptOrigin, siSize, actionRects) == TRUE)
					{
						hr=pIBody->removeChild(pIElement);
						if (hr==S_OK)
						{
							hr=pIXMLElementCollection->get_length(&lCnt);
							ii--;
						}
					}
				}
			}
		}
		
		pIElement->Release();
	}

	pIBody->Release();
	pIXMLElementCollection->Release();

	return S_OK;
}

HRESULT CSFCPatcher::RemoveElementsInRects(CXML_Dings* pXmlDoc, const CRECTARRAY& actionRects)
{
	HRESULT hr, hrReturn = S_OK;

	//
	// goto body
	//
	try
	{
		pXmlDoc->MoveToRoot();
		BOOL bN=pXmlDoc->MoveDown(XML_4GR_ELEM_BODY);
		if (bN!=TRUE)
		{
			return SetError(S_FALSE,"Body tag not found in 4gr file");
		}
	}
	catch (HRESULT hrNoBody)
	{
		hrNoBody=hrNoBody;
		return SetError(S_FALSE,"Body tag not found in 4gr file");
	}

	IXMLElement2* pIBody;	
	hr=pXmlDoc->GetCurrentElement(&pIBody);
	ASSERT(hr==S_OK);

	IXMLElementCollection* pIXMLElementCollection;
	hr=pIBody->get_children(&pIXMLElementCollection);
	ASSERT(hr==S_OK);

	//
	// loop over children, find elements and delete them if they are in a rect
	//
	long lCnt=0;
	hr=pIXMLElementCollection->get_length(&lCnt);
	ASSERT(hr==S_OK);

	for (long ii=0; ii<lCnt; ii++)
	{
		IXMLElement2* pIElement;	
		hr=XML_CollectionGetItem(pIXMLElementCollection, ii, &pIElement);
		ASSERT(hr==S_OK);
		long lType;
		hr=pIElement->get_type(&lType);
		ASSERT(hr==S_OK);
		XMLELEM_TYPE type = (XMLELEM_TYPE) lType;
		if (type == XMLELEMTYPE_ELEMENT)
		{
			CString strName;
			hr=XML_ElementGetTagName(pIElement, strName);
			ASSERT(hr==S_OK);
			if (strName.CompareNoCase(XML_4GR_ELEM_COMMENT)!=0 )
			{
				CPoint ptOrigin;
				CSize siSize;
				HRESULT hr1=XML_ElementGetAttribute(pIElement, XML_4GR_ATTR_ORIGIN, ptOrigin);
				HRESULT hr2=XML_ElementGetAttribute(pIElement, XML_4GR_ATTR_SIZE, siSize);
				if (hr1==S_OK && hr2==S_OK)
				{
					if (LineInRects(ptOrigin, siSize, actionRects) == TRUE)
					{
						hr=pIBody->removeChild(pIElement);
						if (hr==S_OK)
						{
							hr=pIXMLElementCollection->get_length(&lCnt);
							ii--;
						}
					}
				}
			}
		}
		
		pIElement->Release();
	}

	pIBody->Release();
	pIXMLElementCollection->Release();

	return S_OK;
}

BOOL CSFCPatcher::LineInRects(const CPoint& ptOrigin, const CSize& siSize, const CRECTARRAY& actionRects)
{
	BOOL bRet=FALSE;

	CPoint ptOrigin2;
	ptOrigin2.x=ptOrigin.x + siSize.cx;
	ptOrigin2.y=ptOrigin.y + siSize.cy;
	
	for (int ii=0; ii<actionRects.GetSize(); ii++)
	{
		CRect rect=actionRects.GetAt(ii);
		if (rect.PtInRect(ptOrigin) && rect.PtInRect(ptOrigin2))
		{
			bRet=TRUE;
			break;
		}
	}

	return bRet;
}

HRESULT CSFCPatcher::RemoveNonSFCElements(CXML_Dings* pXmlDoc)
{
	HRESULT hr, hrReturn = S_OK;

	//
	// goto body
	//
	try
	{
		pXmlDoc->MoveToRoot();
		BOOL bN=pXmlDoc->MoveDown(XML_4GR_ELEM_BODY);
		if (bN!=TRUE)
		{
			return SetError(S_FALSE,"Body tag not found in 4gr file");
		}
	}
	catch (HRESULT hrNoBody)
	{
		hrNoBody=hrNoBody;
		return SetError(S_FALSE,"Body tag not found in 4gr file");
	}

	IXMLElement2* pIBody;	
	hr=pXmlDoc->GetCurrentElement(&pIBody);
	ASSERT(hr==S_OK);

	IXMLElementCollection* pIXMLElementCollection;
	hr=pIBody->get_children(&pIXMLElementCollection);
	ASSERT(hr==S_OK);

	//
	// loop over children, find non SFC Elements and delete them
	//
	long lCnt=0;
	hr=pIXMLElementCollection->get_length(&lCnt);
	ASSERT(hr==S_OK);

	for (long ii=0; ii<lCnt; ii++)
	{
		IXMLElement2* pIElement;	
		hr=XML_CollectionGetItem(pIXMLElementCollection, ii, &pIElement);
		ASSERT(hr==S_OK);
		long lType;
		hr=pIElement->get_type(&lType);
		ASSERT(hr==S_OK);
		XMLELEM_TYPE type = (XMLELEM_TYPE) lType;
		if (type == XMLELEMTYPE_ELEMENT)
		{
			CString strName;
			hr=XML_ElementGetTagName(pIElement, strName);
			ASSERT(hr==S_OK);
			if (! IsSFCElement(strName) == TRUE)
			{
				hr=pIBody->removeChild(pIElement);
				if (hr==S_OK)
				{
					hr=pIXMLElementCollection->get_length(&lCnt);
					ii--;
				}
			}
		}
		
		pIElement->Release();
	}

	pIBody->Release();
	pIXMLElementCollection->Release();

	return S_OK;
}

HRESULT CSFCPatcher::PatchComments(CXML_Dings* pXmlDoc)
{
	HRESULT hr, hrReturn = S_OK;

	//
	// goto body
	//
	try
	{
		pXmlDoc->MoveToRoot();
		BOOL bN=pXmlDoc->MoveDown(XML_4GR_ELEM_BODY);
		if (bN!=TRUE)
		{
			return SetError(S_FALSE,"Body tag not found in 4gr file");
		}
	}
	catch (HRESULT hrNoBody)
	{
		hrNoBody=hrNoBody;
		return SetError(S_FALSE,"Body tag not found in 4gr file");
	}

	IXMLElement2* pIBody;	
	hr=pXmlDoc->GetCurrentElement(&pIBody);
	ASSERT(hr==S_OK);

	IXMLElementCollection* pIXMLElementCollection;
	hr=pIBody->get_children(&pIXMLElementCollection);
	ASSERT(hr==S_OK);

	//
	// loop over children, find 4GR Comments
	//
	long lCnt=0;
	hr=pIXMLElementCollection->get_length(&lCnt);
	ASSERT(hr==S_OK);

	for (long ii=0; ii<lCnt; ii++)
	{
		IXMLElement2* pIElement;	
		hr=XML_CollectionGetItem(pIXMLElementCollection, ii, &pIElement);
		ASSERT(hr==S_OK);
		long lType;
		hr=pIElement->get_type(&lType);
		ASSERT(hr==S_OK);
		XMLELEM_TYPE type = (XMLELEM_TYPE) lType;
		if (type == XMLELEMTYPE_ELEMENT)
		{
			CString strName;
			hr=XML_ElementGetTagName(pIElement, strName);
			ASSERT(hr==S_OK);
			if (strName.CompareNoCase(XML_4GR_ELEM_COMMENT) == 0 )
			{
				hr=PatchComment(pIElement);
			}
		}
		
		pIElement->Release();
	}

	pIBody->Release();
	pIXMLElementCollection->Release();

	return S_OK;

}

HRESULT CSFCPatcher::PatchComment(IXMLElement2* pI4GRComment)
{
	HRESULT hr;
	IXMLElementCollection* pIXMLElementCollection;
	hr=pI4GRComment->get_children(&pIXMLElementCollection);
	ASSERT(hr==S_OK);
	
	//
	// loop over children, find 4GR ObjComments
	//
	long lCnt=0;
	hr=pIXMLElementCollection->get_length(&lCnt);
	ASSERT(hr==S_OK);

	for (long ii=0; ii<lCnt; ii++)
	{
		IXMLElement2* pIElement;	
		hr=XML_CollectionGetItem(pIXMLElementCollection, ii, &pIElement);
		ASSERT(hr==S_OK);
		long lType;
		hr=pIElement->get_type(&lType);
		ASSERT(hr==S_OK);
		XMLELEM_TYPE type = (XMLELEM_TYPE) lType;
		if (type == XMLELEMTYPE_ELEMENT)
		{
			CString strName;
			hr=XML_ElementGetTagName(pIElement, strName);
			ASSERT(hr==S_OK);
			if (strName.CompareNoCase(XML_4GR_ATTR_OBJCOMMENT) == 0 )
			{
				hr=PatchObjComment(pIElement);
			}
		}
		
		pIElement->Release();
	}
	pIXMLElementCollection->Release();
	
	return S_OK;
}

HRESULT CSFCPatcher::PatchObjComment(IXMLElement2* pIObjComment)
{
	HRESULT hr;
	IXMLElementCollection* pIXMLElementCollection;
	hr=pIObjComment->get_children(&pIXMLElementCollection);
	ASSERT(hr==S_OK);
	
	//
	// loop over children, find text
	//
	long lCnt=0;
	hr=pIXMLElementCollection->get_length(&lCnt);
	ASSERT(hr==S_OK);

	for (long ii=0; ii<lCnt; ii++)
	{
		IXMLElement2* pIElement;	
		hr=XML_CollectionGetItem(pIXMLElementCollection, ii, &pIElement);
		ASSERT(hr==S_OK);
		long lType;
		hr=pIElement->get_type(&lType);
		ASSERT(hr==S_OK);
		XMLELEM_TYPE type = (XMLELEM_TYPE) lType;
		if (type == XMLELEMTYPE_TEXT)
		{
			//
			// get text
			//
			BSTR    sText;
			hr = pIElement->get_text(&sText);
			ASSERT(hr==S_OK);
			CString strBuf = sText;
			CString strDecoded;
			CXML_Dings::DecodeString(sText, strDecoded);
			
			CComBSTR sTextDecoded(strDecoded);
			hr=pIElement->put_text(sTextDecoded);
			ASSERT(hr == S_OK);
			hr=pIElement->get_text(&sText);
			strBuf = sText;
			SysFreeString(sText);
		}
		
		pIElement->Release();
	}
	pIXMLElementCollection->Release();
	
	return S_OK;
}

BOOL CSFCPatcher::IsSFCElement(const CString& strName1)
{
	CString strName=strName1;
	strName.MakeUpper();
	if ( 
		strName == XML_4GR_ELEM_LINE ||
		strName == XML_4GR_ELEM_STEP ||
		strName == XML_4GR_ELEM_TRANS ||
		strName == XML_4GR_ELEM_ALTDIV ||
		strName == XML_4GR_ELEM_SIMDIV ||
		strName == XML_4GR_ELEM_ACTION ||
		strName == XML_4GR_ELEM_ACTBLCK ||
		strName == XML_4GR_ELEM_STACTION ||
		strName == XML_4GR_ELEM_COMMENT 
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

HRESULT CSFCPatcher::SetError(HRESULT hr, const CString& errorMessage)
{
	if (errorMessage!="" && hr != S_OK)
		m_lastErrorMessage = errorMessage;
	m_lastErrorId = hr;
	return hr;
}

HRESULT CSFCPatcher::GetLastError(CString& strErrorMessage)
{
	strErrorMessage = m_lastErrorMessage;
	return m_lastErrorId;
}

CString CSFCPatcher::GetLastErrorMessage()
{
	return m_lastErrorMessage;
}

HRESULT CSFCPatcher::Copy(const CString& strOrigFile, const CString& strPatchedFile)
{
	//
	// load xml grafics
	//
	CStdioFile origFile, patchedFile;
	CFileException fileError;

	BOOL bF=origFile.Open(strOrigFile, CFile::modeRead|CFile::typeBinary, &fileError);
	if (bF==FALSE)
	{
		return SetError(E_FAIL, "can't open " + strOrigFile);
	}

	bF=patchedFile.Open(strPatchedFile, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary, &fileError);
	if (bF==FALSE)
	{
		return SetError(E_FAIL, "can't open " + strPatchedFile);
	}

	CXML_Dings xmlDoc(&origFile, &patchedFile);

	try
	{
		xmlDoc.Load();

	}
	catch (HRESULT hrError)
	{
		hrError=hrError;
		CString strError;
		xmlDoc.GetLastParseError(strError);
		SetError(E_FAIL, "Parse error " + strOrigFile + " " + strError);
		goto ende;
	}

	PatchComments(&xmlDoc);
	xmlDoc.Flush();

ende:
	try
	{
		patchedFile.Close();
	}
	catch (CFileException ex)
	{
	}

	return SetError(S_OK);
}
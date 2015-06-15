
#if !defined(_SFCPATCHER_H)
#define _SFCPATCHER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DbgInfo.h" 
#include "SFCElem.h"
#include "XML_Dings.h"

typedef CArray<CSFCElem, CSFCElem&> CSFCELEMARRAY;
typedef CArray<CRect, CRect&> CRECTARRAY;
class CXML_Dings;

class CSFCPatcher  
{
public:
	CSFCPatcher(const CString& strProjectPath);
	virtual ~CSFCPatcher();

	HRESULT Patch(const CString& strOrigFile, const CString& strPatchedFile, const CString& strSfcPath, BOOL bDbiFile);
	HRESULT GetLastError(CString& strErrorMessage);
	CString GetLastErrorMessage();
	HRESULT Copy(const CString& strOrigFile, const CString& strPatchedFile);

protected:
	HRESULT GetITypeDBI(const CString& sfcPath, ITypeDBI** ppITypeDBI);
	HRESULT GetStepsDBI(ITypeDBI* pITypeDBI, CSFCELEMARRAY& sfcElems);
	HRESULT GetActionsDBI(ITypeDBI* pITypeDBI, CSFCELEMARRAY& sfcElems);

	HRESULT GetElementsDBIFile(CXML_Dings& xmlDbi, const CString& strElementName, CSFCELEMARRAY& sfcElems);
	HRESULT GetStepsDBIFile(CXML_Dings& xmlDbi, CSFCELEMARRAY& sfcElems);
	HRESULT GetActionsDBIFile(CXML_Dings& xmlDbi, CSFCELEMARRAY& sfcElems);
	HRESULT GetTransitionsDBIFile(CXML_Dings& xmlDbi, CSFCELEMARRAY& sfcElems);

	HRESULT GetTransitionsDBI(ITypeDBI* pITypeDBI, CSFCELEMARRAY& sfcElems);

	HRESULT PatchElements(CXML_Dings* pXmlDoc, CSFCELEMARRAY& sfcElements, const CString& strElementName);
	HRESULT PatchElement(CXML_Dings* pXmlDoc, CSFCELEMARRAY& sfcSteps);
	HRESULT PatchSteps(CXML_Dings* pXmlDoc, CSFCELEMARRAY& sfcSteps);
	HRESULT PatchTransitions(CXML_Dings* pXmlDoc, CSFCELEMARRAY& sfcTransitions);
	HRESULT PatchActions(CXML_Dings* pXmlDoc, CSFCELEMARRAY& sfcActions);

	HRESULT PatchComments(CXML_Dings* pXmlDoc);
	HRESULT PatchComment(IXMLElement2* pI4GRComment);
	HRESULT PatchObjComment(IXMLElement2* pIObjComment);

	HRESULT GetActionRects(CXML_Dings* pXmlDoc, CRECTARRAY& actionRects);
	HRESULT RemoveLinesInRects(CXML_Dings* pXmlDoc, const CRECTARRAY& actionRects);
	BOOL LineInRects(const CPoint& ptOrigin, const CSize& siSize, const CRECTARRAY& actionRects);
	HRESULT RemoveElementsInRects(CXML_Dings* pXmlDoc, const CRECTARRAY& actionRects);

	HRESULT RemoveNonSFCElements(CXML_Dings* pXmlDoc);
	BOOL IsSFCElement(const CString& strName);

	HRESULT SetError(HRESULT hr, const CString& errorMessage="");

private:
	CString m_strProjectPath;
	IDebugInfo* m_pIDebugInfo;
	IProjectDBI* m_pIProjectDBI;

	CString m_lastErrorMessage ;
	HRESULT m_lastErrorId;
};

#endif // !defined(_SFCPATCHER_H)

#include "stdafx.h"
#include "CpObjectList.h"
#include "CpInternet.h"
#include "resource.h"
#include "VariablePath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//*****************************************************************************
CCpObjectList::CCpObjectList(CCpObjectList* pParent /*=NULL*/)
//*****************************************************************************
{
	m_pParent = pParent;

	m_pThread = NULL;
	m_bHaltThread = FALSE;
	
	if (m_pParent == NULL)
	{
		m_ObjectCount = 0;
		m_pRootObject = this;

		char buff[32];
		DWORD buffSize = sizeof(buff);
		::GetComputerName(buff, &buffSize);
		m_ComputerName = buff;
		m_ComputerName.MakeLower();
	}
	else
	{
		m_pRootObject = m_pParent->GetRootObject();;
	}
	
	ASSERT((m_pParent == NULL) || (m_pParent != NULL && m_pRootObject->m_ObjectCount > 0));

	m_pRootObject->m_ObjectCount++;

	Clear();
}

//*****************************************************************************
CCpObjectList::~CCpObjectList()
//*****************************************************************************
{
	StopLoadAsync();
	Clear();
	m_pRootObject->m_ObjectCount--;
	ASSERT((m_pParent == NULL && m_pRootObject->m_ObjectCount == 0) || (m_pParent != NULL && m_pRootObject->m_ObjectCount > 0));
}

//*****************************************************************************
void CCpObjectList::Clear( )
//*****************************************************************************
{
	for (unsigned ii = 0; ii < GetChildCount(); ii++)
	{
		if (GetChild(ii) != NULL)
			delete GetChild(ii);
	}
	m_ChildrenList.RemoveAll();
	
	m_TypeId = FCNAV_ITEMTYPE_INVALID;
	m_Properties = 0;
	m_Name.Empty();
	m_ErrorText.Empty();
	m_AspFile.Empty();
	m_ConfFile.Empty();
	m_Server.Empty();
	m_ChildrenFile.Empty();
	m_bChildrenLoaded = FALSE;
	m_UserData = 0;
}

#define CPNAV_MAXLEVEL 32
//*****************************************************************************
BOOL CCpObjectList::Load(
	LPCTSTR ObjectListURL,
	CWnd *pMsgWnd /*=NULL*/)
//*****************************************************************************
{
	Clear();

	CCpObjectList* pObjectStack[CPNAV_MAXLEVEL];
	memset(pObjectStack, 0, sizeof(pObjectStack));
	pObjectStack[0] = this;

	int LastLevel = -1;

	BOOL bOK = Load2(ObjectListURL, &LastLevel, pObjectStack, pMsgWnd);

	if ((GetObjectCount() == 1 &&  GetTypeId() != FCNAV_ITEMTYPE_PROJECT))
	{
		m_Name.LoadString(IDS_EMPTY);
		m_TypeId = FCNAV_ITEMTYPE_PROJECT;
	}

	return bOK;
}
//*****************************************************************************
 BOOL CCpObjectList::Load2(
	LPCTSTR ObjectListURL,
	int *pLastLevel,
	CCpObjectList* pObjectStack[],
	CWnd *pMsgWnd /*=NULL*/)
//*****************************************************************************
{
	unsigned IdsErr = 0;
	CCpInternet tr;
	if (!tr.OpenFile(ObjectListURL))
	{
		if (*pLastLevel < 0) 
		{
			if (pMsgWnd != NULL && IsWindow(pMsgWnd->m_hWnd))
					pMsgWnd->PostMessage(WM_FCNAV_LOADSTATUS, FCNAV_LOADSTATUS_READY, 0);
		}
		return FALSE;
	}


	CString Line;
	while (tr.ReadLine(Line))
	{
		if (Line.GetLength() > 0)
		{
			Line.TrimLeft();
			Line.TrimLeft();
		}
		if (Line.GetLength() > 0 && Line[0] != '#') // skip comments
		{
			int Level; 
			unsigned TypeId;
			unsigned Properties;
			CString Name;
			CString ClassName;
			CString AspFile;
			CString ConfFile;
			CString ChildrenFile;
			CString Server;
			CString strDummy;

			if(!ParseObject(
				Line, Level, TypeId, Properties, Name, ClassName, Server, AspFile, ConfFile, ChildrenFile))
			{
				// if there is no valid root object, abort the whole thing				
				if (*pLastLevel < 0) 
				{
					if (pMsgWnd != NULL && IsWindow(pMsgWnd->m_hWnd))
							pMsgWnd->PostMessage(WM_FCNAV_LOADSTATUS, FCNAV_LOADSTATUS_READY, 0);
					return FALSE;
				}

				IdsErr = IDS_ERR_CONF_SYNTAX;
			}

			if (*pLastLevel < 0)
			{
				if (Level != 0) // Level must be 0 for root object
				{
					if (IdsErr == 0)
						IdsErr = IDS_ERR_CONF_INVALID_LEVEL;
					Level = 0;
				}
			}
			else
			{
				if (Level <= 0 || Level >= CPNAV_MAXLEVEL || Level > (*pLastLevel + 1))
				{
					if (IdsErr == 0)
						IdsErr = IDS_ERR_CONF_INVALID_LEVEL;
					Level = *pLastLevel + 1;
					Level = min(Level, CPNAV_MAXLEVEL - 1);
				}
			}

			//Level is now >= 0

			CCpObjectList* pParent = NULL;
			if (Level > 0)
			{
				pParent = pObjectStack[Level-1];
				pObjectStack[Level] = new CCpObjectList(pParent);
			}

			CCpObjectList* pO = pObjectStack[Level];

			if (IdsErr == 0)
				pO->m_TypeId	= TypeId;
			else
				pO->m_TypeId	= FCNAV_ITEMTYPE_INVALID;

			pO->m_Level			= Level;		
			pO->m_Properties	= Properties;		
			pO->m_Name			= Name;
			pO->m_ClassName		= ClassName;
			pO->m_AspFile		= AspFile;
			pO->m_ConfFile		= ConfFile;
			pO->m_ChildrenFile	= ChildrenFile;
			pO->m_ErrorText.Empty();

			// get server name from URL for root object
			if (Level == 0)
			{
				DWORD ServType;
				CString sDummy;
				INTERNET_PORT nPort; 
				if (!AfxParseURL(ObjectListURL, ServType, pO->m_Server, sDummy, nPort))
				{
					IdsErr = IDS_ERR_CONF_INVALID_URL;
					tr.CloseFile();
				}
			}
			// get server from object lsit
			else
			{
				pO->m_Server = Server;
			}

			pO->m_Server.MakeLower();

			if (pMsgWnd != NULL && IsWindow(pMsgWnd->m_hWnd) && Level <= 2)
				pMsgWnd->PostMessage(WM_FCNAV_LOADSTATUS, FCNAV_LOADSTATUS_LOADING, (LPARAM)pO);
			
			if (IdsErr != 0)
			{
				pO->m_ErrorText.LoadString(IdsErr);
			}

			if (Level > 0)
				pParent->m_ChildrenList.Add(pO);

			*pLastLevel = Level;

			// check if loading has been aborted
			if (pObjectStack[0]->m_bHaltThread)
			{
				tr.CloseFile();
				if (pMsgWnd != NULL && IsWindow(pMsgWnd->m_hWnd))
					pMsgWnd->PostMessage(WM_FCNAV_LOADSTATUS, FCNAV_LOADSTATUS_ABORTED, (LPARAM)pO);

				return TRUE;
			}
			
			// load child objects
			if (pO->m_ChildrenFile.GetLength() > 0)
			{
				if ((pParent == NULL) || (pO->GetServer() == pParent->GetServer()))
				{
					pO->m_bChildrenLoaded = Load2(pO->GetChildrenUrl(), pLastLevel, pObjectStack, pMsgWnd);				
				}
			}
		}
	}

	tr.CloseFile();

	if (pMsgWnd != NULL && IsWindow(pMsgWnd->m_hWnd))
		pMsgWnd->PostMessage(WM_FCNAV_LOADSTATUS, FCNAV_LOADSTATUS_READY, (LPARAM)pObjectStack[0]);

	return TRUE;
}

//*****************************************************************************
const CString & CCpObjectList::GetServer() const
//*****************************************************************************
{
	const CCpObjectList *pParent = this;	
	do
	{
		if (!pParent->m_Server.IsEmpty())
			return pParent->m_Server;
		pParent = pParent->GetParent();
	} while (pParent != NULL);
	return m_Server;
}

//*****************************************************************************
BOOL CCpObjectList::Filter(
	unsigned TypeFilter,
	LPCSTR pName) const
//*****************************************************************************
{
	if (TypeFilter != 0 && (TypeFilter & GetTypeId()) == 0)
		return FALSE;
	if (pName != NULL && (GetName() != pName))
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Helper functions
#define FCNAV_N_TOKEN 9
//*****************************************************************************
BOOL CCpObjectList::ParseObject(
	const CString& ItemDesc,
	int& Level, 
	unsigned& TypeId, 
	unsigned& Properties, 
	CString& Name,
	CString& ClassName,
	CString& Server,
	CString& AspFile,
	CString& ConfFile,
	CString& ChildrenFile)
//*****************************************************************************
{
	// initialization
	BOOL bValid = FALSE;
	TypeId = 0; 
	Properties = 0; 
	Name.Empty(); 
	ClassName.Empty(); 
	AspFile.Empty();
	ConfFile.Empty();
	ChildrenFile.Empty();
	Server.Empty();

	// extract tokens from text line
	// ItemDesc : "<hierarchy>;<typeid>;<properties>;<name>;<server>;<AspFile>;<ConfFile>;<Children URL>"
	CString sToken[FCNAV_N_TOKEN];
	unsigned TokenCount = 0;
	for (int ii = 0; ii < ItemDesc.GetLength() && TokenCount < FCNAV_N_TOKEN; ii++)
	{
		if (ItemDesc[ii] == ';')
		{
			sToken[TokenCount].TrimLeft();
			sToken[TokenCount].TrimRight();
			TokenCount++;
			if (ii == ItemDesc.GetLength() - 1)
				TokenCount++;
		}
		else
		{
			sToken[TokenCount] += ItemDesc[ii];
			if (ii == ItemDesc.GetLength() - 1)
			{
				sToken[TokenCount].TrimLeft();
				sToken[TokenCount].TrimRight();
				TokenCount++;
			}
		}
	}

	// convert and check tokens 
	if (TokenCount >= 4)
	{
		// the first 4 parameters are mandatory
		int iToken;
		bValid = Token2Integer(sToken[0], iToken);
		bValid &= (iToken >= 0);
		Level = iToken;

		bValid &= Token2Integer(sToken[1], iToken);
		bValid &= (IsValidTypeId(iToken));
		TypeId = (unsigned)iToken;

		bValid &= Token2Integer(sToken[2], iToken);
		Properties = (unsigned)iToken;

		Name = sToken[3];

		// the following parameters are optional
		if (TokenCount >= 5)
			ClassName = sToken[4];
		if (TokenCount >= 6)
			Server = sToken[5];
		if (TokenCount >= 7)
			AspFile = sToken[6];
		if (TokenCount >= 8)
			ConfFile = sToken[7];
		if (TokenCount >= 9)
			ChildrenFile = sToken[8];
	}
	return bValid;
}

//*****************************************************************************
BOOL CCpObjectList::Token2Integer(
	const CString& sToken, 
	int& iToken)
//*****************************************************************************
{
	iToken = 0;
	if (sToken.GetLength() > 0)
	{
		iToken = atoi(sToken);
		if (iToken != 0 || (sToken == '0' && sToken.GetLength() == 1))
			return TRUE;
	}
	return FALSE;
}

//****************************************************************************
void CCpObjectList::LoadAsync(
	LPCTSTR ObjectListURL,
	CWnd *pMsgWnd)
//****************************************************************************
{
	ASSERT(this == m_pRootObject);

	static TCpLoadThreadInfo inf;
	inf.pObject = this;
	inf.ObjectListUrl = ObjectListURL;
	inf.pMsgWnd = pMsgWnd;

	StopLoadAsync();

    m_pThread = AfxBeginThread(LoadingThread, &inf, THREAD_PRIORITY_BELOW_NORMAL);
	ASSERT(m_pThread != NULL);		
}

//****************************************************************************
void CCpObjectList::StopLoadAsync() 
//****************************************************************************
{
	if (this == m_pRootObject && m_pThread != NULL)
	{
		ASSERT(m_pThread->m_hThread != NULL);

		m_bHaltThread = TRUE;
		while (::WaitForSingleObject(m_pThread->m_hThread, 1000) == WAIT_TIMEOUT);
	}
	m_bHaltThread = FALSE;
}

//****************************************************************************
UINT CCpObjectList::LoadingThread(LPVOID pParam)
//****************************************************************************
{
    ASSERT(pParam != NULL);
	
	TCpLoadThreadInfo* pInf = (TCpLoadThreadInfo *)pParam;
	ASSERT(pInf->pObject == pInf->pObject->m_pRootObject);

	// load objects
	pInf->pObject->Load(pInf->ObjectListUrl, pInf->pMsgWnd);

	pInf->pObject->m_pThread = NULL;
    return 0;
}

CCpObjectList* CCpObjectList::FindChildByName(const CString& strName)
{
	unsigned nn=GetChildCount();
	for (unsigned ii=0; ii<nn; ii++)
	{
		CCpObjectList* pObj=GetChild(ii);
		ASSERT(pObj);
		CString strObjName=pObj->GetName();
		if ( strName.CompareNoCase(strObjName) == 0 )
			return pObj;
	}

	return 0;
}


CCpObjectList* CCpObjectList::FindGrandChildByName(const CString& strName)
{
	unsigned nn=GetChildCount();
	for (unsigned ii=0; ii<nn; ii++)
	{
		CCpObjectList* pChild=GetChild(ii);
		CCpObjectList* pGrandChild=pChild->FindChildByName(strName);
		if (pGrandChild)
			return pGrandChild;
	}

	return 0;
}

/*
* strPath: <Resource><Program><FB>.....
*
*
*
*
*/
CCpObjectList* CCpObjectList::FindObjectByPath(const CString& strPath)
{
	CVariablePath variablePath(strPath);

	//
	// get root Object (project)
	//
	CCpObjectList* pRoot = GetRootObject();
	if (pRoot==0)
		return 0;

	CCpObjectList* pFoundObj=pRoot;
	for (;;)
	{
		CString strVariable=variablePath.Pop();
		if (strVariable=="")
			return pFoundObj;
		pFoundObj = pFoundObj->FindChildByName(strVariable);
		if (pFoundObj == 0)
			return 0;
	}

	return 0;
}

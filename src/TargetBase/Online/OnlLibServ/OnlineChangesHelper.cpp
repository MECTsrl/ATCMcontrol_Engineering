/* $Header: /4CReleased/V2.20.00/TargetBase/Online/OnlLibServ/OnlineChangesHelper.cpp 6     17.04.07 15:05 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: OnlineChangesHelper.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/Online/OnlLibServ/OnlineChangesHelper.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
 *
 * =CURRENT 	 $Date: 17.04.07 15:05 $
 *			 $Revision: 6 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "OnlineChangesHelper.h"
#include "config.h"
#include "connection.h"

//----	Static Initializations:   ----------------------------------*

static void parseGlobalsDef(CCsvParse& file, VarDeclsArray& globals)
{
	int 	  fieldNo = 1;
	VarDecl   Decl;
	CString   field1;
	char*	  stopchar;

	IEC_UINT  index = 0xffffu;

	while(file.ParseNextField(field1))
	{
		switch(fieldNo)
		{
			case CSV_IDX_NAME:
			{
				assert(field1.IsEmpty() == FALSE);

				Decl.name		= field1;
				Decl.type		= _T("");
				Decl.bitoffset	= 0;
				Decl.bitsize	= 0;
				
				index = 0xffffu;
				break;
			}

			case CSV_IDX_DECL_SPEC:
			{
				assert(field1.IsEmpty() == FALSE);

				if (field1.GetLength() > 1 && field1[1] == _T('*'))
				{
					/* A pointer (Should not happen for globals!)
					 */
					assert(FALSE);	
					Decl.name.Insert(0, _T('*'));
				}
				else if(field1[0] == 's' || field1[0] == 'S')
				{
					Decl.name.Insert(0, _T('#'));
				}
				break;
			}

			case CSV_IDX_SEGMENT:
			{
				if (! field1.IsEmpty())
				{
					index = (IEC_UINT)_tcstoul((LPCTSTR)field1, &stopchar, 10);
				}
				break;
			}

			case CSV_IDX_TYPE:
			{
				assert (field1.IsEmpty() == FALSE);

				Decl.type = field1;
				break;
			}

			case CSV_IDX_BYTE_OFFSET:
			{
				if (! field1.IsEmpty())
				{
					Decl.bitoffset = _tcstoul((LPCTSTR)field1, &stopchar, 10) * 8;
				}	
				break;
			}

			case CSV_IDX_BIT_OFFSET:
			{
				if (! field1.IsEmpty())
				{
					Decl.bitoffset += _tcstoul((LPCTSTR)field1, &stopchar, 10);
				}	
				break;
			}

			case CSV_IDX_BIT_SIZE:
			{
				if (! field1.IsEmpty())
				{
					Decl.bitsize = _tcstoul((LPCTSTR)field1, &stopchar, 10);
				}	
				break;
			}

			case CSV_IDX_STRLEN:
			{
				if (! field1.IsEmpty())
				{
					Decl.bitsize = (_tcstoul((LPCTSTR)field1, &stopchar, 10) + 2) * 8;
				}
				break;
			}

			default:
			{
				assert(! _T("Invalid *.def2 entry"));
				break;
			}

		} /* switch(fieldNo) */

		if(file.IsEndOfLine())
		{
			POSITION pos;

			// insert ordered
			// tip: start from end because the var decls are usually sorted already
			switch(index)
			{
			case IDX_INPUT:
			case IDX_OUTPUT:
			case IDX_MEMORY:
			case IDX_GLOBAL:
			case IDX_RETAIN:
				pos = globals[index].GetTailPosition();
				
				while(pos && globals[index].GetAt(pos).bitoffset > Decl.bitoffset)
					globals[index].GetPrev(pos);

				if(pos)
					globals[index].InsertAfter(pos, Decl);
				else
					globals[index].AddHead(Decl);
				break;

			case 0xffffu:	// ignore program and global fb instances
				break;

			default:
				assert(!_T("unexpected global variable index"));
			}

			fieldNo = 1;

		} /* if(file.IsEndOfLine()) */
		else
		{
			fieldNo++;
		}

	} /* while(file.ParseNextField(field1)) */

	assert(fieldNo == 1);
}

static void parseDefFile(CCsvParse& file, VarDecls& instMembers)
{
	int 	  fieldNo = 1;
	VarDecl   Decl;
	CString   field1;
	char*	  stopchar;

	while(file.ParseNextField(field1))
	{
		switch(fieldNo)
		{
			case CSV_IDX_NAME:
			{
				assert(field1.IsEmpty() == FALSE);

				Decl.name		= field1;
				Decl.type		= _T("");
				Decl.bitoffset	= 0;
				Decl.bitsize	= 0;
				break;
			}

			case CSV_IDX_DECL_SPEC:
			{
				assert(field1.IsEmpty() == FALSE);

				if (field1.GetLength() > 1 && field1[1] == _T('*'))
				{
					/* A pointer ...
					 */
					Decl.name.Insert(0, _T('*'));
				}
				else if(field1[0] == 's' || field1[0] == 'S')
				{
					/* A structure ...
					 */
					Decl.name.Insert(0, _T('#'));
				}
				break;
			}

			case CSV_IDX_SEGMENT:
			{
				// Skip
				break;
			}

			case CSV_IDX_TYPE:
			{
				assert (field1.IsEmpty() == FALSE);

				Decl.type = field1;
				break;
			}

			case CSV_IDX_BYTE_OFFSET:
			{
				assert (field1.IsEmpty() == FALSE);

				Decl.bitoffset = _tcstoul((LPCTSTR)field1, &stopchar, 10) * 8;
				break;
			}

			case CSV_IDX_BIT_OFFSET:
			{
				if (! field1.IsEmpty())
				{
					Decl.bitoffset += _tcstoul((LPCTSTR)field1, &stopchar, 10);
				}	
				break;
			}

			case CSV_IDX_BIT_SIZE:
			{
				assert (field1.IsEmpty() == FALSE);

				Decl.bitsize = _tcstoul((LPCTSTR)field1, &stopchar, 10);
				break;
			}

			case CSV_IDX_STRLEN:
			{
				if (! field1.IsEmpty())
				{
					Decl.bitsize = (_tcstoul((LPCTSTR)field1, &stopchar, 10) + 2) * 8;
				}
				break;
			}

			default:
			{
				assert(! _T("Invalid *.def2 entry"));
				break;
			}

		} /* switch(fieldNo) */

		if(file.IsEndOfLine())
		{
			// insert ordered
			// tip: start from the end because the var decls are usually sorted already
			POSITION pos = instMembers.GetTailPosition();
			
			while(pos && instMembers.GetAt(pos).bitoffset > Decl.bitoffset)
				instMembers.GetPrev(pos);

			if(pos)
				instMembers.InsertAfter(pos, Decl);
			else
				instMembers.AddHead(Decl);

			fieldNo = 1;

		} /* if(file.IsEndOfLine()) */

		else
		{
			fieldNo++;
		}

	} /* while(file.ParseNextField(field1)) */

	assert(fieldNo == 1);
}

// fill the empty spaces between variables' declarations with dummy vars
static void fillDeclBlanks(VarDecls& decls)
{
	POSITION pos1  = NULL;
	VarDecl  temp;
	VarDecl* pVar1 = NULL;
	VarDecl* pVar2 = NULL;

	ULONG ulBitSize 	= 0;
	ULONG ulMaxBitSize	= 0;

	temp.name = _T('@');
	pos1 = decls.GetHeadPosition();

	while(pos1 != NULL)
	{
		pVar1 = &decls.GetNext(pos1);

		ulBitSize += pVar1->bitsize;
		if (pVar1->bitsize > ulMaxBitSize && pVar1->bitsize <= sizeof(IEC_LREAL) * 8)
		{
			ulMaxBitSize = pVar1->bitsize;
		}

		if(pos1 == NULL)
		{
			if (ulMaxBitSize > 0)
			{
				/* ##TODO##: This works only for 8 byte alignment!
				 */

				temp.bitoffset	= 0;
				temp.bitsize	= 0;

				/* Append ulAliBit bits at the end of the object in order to create a 
				 * multiple of 8 / byte sized object.
				 */
				ULONG ulAliBit	= (ulBitSize % 8) ? 8 - ulBitSize % 8 : 0;

				/* ulByteSize is the resulting size of the object in bytes.
				 */
				ULONG ulByteSize = ulBitSize / 8 + (ulAliBit == 0 ? 0 : 1);

				/* Compute the largest variable embedded in the object (in bytes).
				 */
				ULONG ulMaxByteSize = ulMaxBitSize / 8;

				/* Append ulAliByte bytes at the end of the object in order to make the 
				 * size of the object a multiple of the largest variable embedded within 
				 * the object ...
				 */
				ULONG ulAliByte = 0;

				/* ... only if is a none-empty object and if the largest variable 
				   embedded in the object is at least a 16 bit variable.
				 */
				if (ulBitSize != 0 && ulMaxByteSize >= 2)
				{
					ulAliByte = ulByteSize % ulMaxByteSize;
				}

				if (ulAliByte != 0)
				{
					temp.bitoffset	= pVar1->bitoffset + pVar1->bitsize;
					temp.bitsize   += (ulMaxByteSize - ulAliByte) * 8;
				}

				if (ulAliBit != 0)
				{
					temp.bitoffset	= pVar1->bitoffset + pVar1->bitsize;
					temp.bitsize   += ulAliBit;
				}

				if (temp.bitsize != 0)
				{
					/* Add a dummy variable at the end to fulfill the alignment
					 * rule and obtain the correct sizeof() for this structure.
					 */
					decls.AddTail(temp);
				}

			} /* if (ulMaxBitSize > 0) */

			break;

		} /* if(pos1 == NULL) */

		pVar2 = &decls.GetAt(pos1);

		if(pVar1->bitoffset + pVar1->bitsize != pVar2->bitoffset)
		{
			assert(pVar1->bitoffset + pVar1->bitsize < pVar2->bitoffset);
			temp.bitoffset = pVar1->bitoffset + pVar1->bitsize;
			temp.bitsize   = pVar2->bitoffset - temp.bitoffset;

			decls.InsertBefore(pos1, temp);

			ulBitSize += temp.bitsize;
			if (temp.bitsize > ulMaxBitSize && temp.bitsize <= sizeof(IEC_LREAL) * 8)
			{
				ulMaxBitSize = temp.bitsize;
			}
		}

	} /* while(pos1 != NULL) */
}

/* ---------------------------------------------------------------------------- */
/**
 * matchDecls
 *
 * Returns TRUE if the declarations are the same, else FALSE
 */
static BOOL matchDecls(VarDecls& newMembers, VarDecls& oldMembers, CopyRegs& crs)
{
	POSITION posNew 	= NULL;
	POSITION posOld 	= NULL;
	VarDecl *pVarNew	= NULL;
	VarDecl *pVarOld	= NULL;

	BOOL bIdent = TRUE;

	if(newMembers.GetCount() != oldMembers.GetCount())
	{
		bIdent = FALSE;
	}

	// update size and fill the empty places between variables with dummy vars 
	fillDeclBlanks(oldMembers);
	fillDeclBlanks(newMembers);

	posNew = newMembers.GetHeadPosition();
	while(posNew)
	{
		BOOL bFound = FALSE;

		pVarNew = &newMembers.GetNext(posNew);

		/* Ignore temporary variables, alignment dummies, indexes and pointers
		 */
		if(pVarNew->name[0] == _T('@') || pVarNew->name[0] == _T('#') || pVarNew->name[0] == _T('*'))
		{
			continue;
		}

		posOld = oldMembers.GetHeadPosition();
		while(posOld != NULL)
		{
			pVarOld = &oldMembers.GetNext(posOld);

			if(pVarNew->name == pVarOld->name && pVarNew->type == pVarOld->type && pVarNew->bitsize == pVarOld->bitsize)
			{
				bFound = TRUE;
				if(pVarNew->bitoffset != pVarOld->bitoffset)
				{
					bIdent = FALSE;
				}
				break;
			}

		}

		if(bFound == FALSE)
		{
			bIdent = FALSE;
			continue;
		}
		
		CopyReg  cr;

		cr.newOffset = pVarNew->bitoffset;
		cr.oldOffset = pVarOld->bitoffset;
		cr.size 	 = pVarNew->bitsize;

		while(posNew != NULL && posOld != NULL)
		{
			/* Enlarge copy region with following matching decls.
			 */
			pVarNew = &newMembers.GetAt(posNew);
			pVarOld = &oldMembers.GetNext(posOld);

			if(pVarNew->name != pVarOld->name || pVarNew->type != pVarOld->type ||
				pVarNew->bitsize != pVarOld->bitsize || pVarNew->name[0] == _T('#'))
			{
				break;
			}	

			/* Identical, a match
			 */
			cr.size += pVarNew->bitsize;

			&newMembers.GetNext(posNew);
		}

		assert(cr.size > 0);

		crs.AddTail(cr);

	} /* while(posNew) */

	posNew = newMembers.GetHeadPosition();
	if(posNew)
	{
		pVarNew = &newMembers.GetNext(posNew);

		// if SFC POU differ dont copy data, generate a dummy copy region
		if(bIdent == FALSE && pVarNew->name.Compare(_T("__nsteps")) == 0)
		{
			crs.RemoveAll();		// ##TODO## Wieso nicht am Anfang überprüfen???

			CopyReg  cr;			
			cr.newOffset = 0;
			cr.oldOffset = 0;
			cr.size 	 = 0;

			crs.AddTail(cr);
		}
	}

	return bIdent;
}

static void emptyInstancesMap(CMapStringToPtr* pInsts)
{
	if(!pInsts)
		return;

	POSITION  pos = pInsts->GetStartPosition();
	CString   key;
	PairList* pList;
	
	while (pos != NULL)
	{
		pInsts->GetNextAssoc(pos, key, (void*&)pList);
		if(pList)
			pList->RemoveAll();
	}

	pInsts->RemoveAll();
}
// end static functions


COnlineChangesHelper::COnlineChangesHelper() : 
m_isInit(FALSE), m_pInstancesMap(NULL), m_pbBuffer(NULL), m_iBufferSize(0),
m_processObjectsInit(FALSE)
{
	m_newDirectVars.SetSize(IDX_MEMORY+1);
	m_iNewObjNr = m_iOldObjNr = 2; // reserve 2 objects for global and retain objects
	memset(&m_OCConfig, 0, sizeof(m_OCConfig));
}

COnlineChangesHelper::~COnlineChangesHelper()
{
	Cleanup();
}

void COnlineChangesHelper::Cleanup()
{
	destroyOCConfig();

	for(int i = 0; i < m_newDirectVars.GetSize(); i++)
		m_newDirectVars[i].RemoveAll();

	m_processObjectsInit = FALSE;

	m_iNewObjNr = m_iOldObjNr = 2; // reserve 2 objects for global and retain objects	
	memset(&m_OCConfig, 0, sizeof(m_OCConfig));
	if(m_pInstancesMap)
	{
		m_objType.RemoveAll();
		m_map.RemoveAll();
		emptyInstancesMap(m_pInstancesMap);
		delete m_pInstancesMap;
		m_pInstancesMap = NULL;
	}
	m_ChangesList.RemoveAll();
	m_isInit = FALSE;
}


BOOL COnlineChangesHelper::addIndexPair(CString& typeName, CString& instName, IEC_UINT oldIndex, IEC_UINT oldSize,
										IEC_UINT newIndex, IEC_UINT newSize)
{
	PairList* pList;
	OC_Pair   pair;

	pair.name	  = instName;
	pair.newIndex = newIndex;
	pair.newSize  = newSize;
	pair.oldIndex = oldIndex;
	pair.oldSize  = oldSize;

	if(!m_pInstancesMap->Lookup(typeName, (void*&)pList))
	{
		pList = new PairList;
		m_pInstancesMap->SetAt(typeName, pList);
	}
	pList->AddTail(pair);
	m_map.SetAtGrow(newIndex, oldIndex);
	m_objType.SetAtGrow(newIndex, typeName);

	return S_OK;
}


HRESULT COnlineChangesHelper::processFileOC(const CString& strDomain, IN SAFEARRAY **ppsaContent,
											 CString& strFile, LPDWORD lpdwSize)
{
	HRESULT hr = S_OK;

	if(strDomain.CompareNoCase(pszClassesBinName) == 0)
	{
		hr = generateClassesDiff(pszClassesDiffName, lpdwSize,
			pszClassesBinName, ppsaContent, 
			&m_OCConfig.uCode, &m_OCConfig.iCodeDiff);
		strFile = pszClassesDiffName;
	}
	else if(strDomain.CompareNoCase(pszObjectsInitName) == 0)
	{
		hr = generateObjectsDiff(pszObjectsDiffName, lpdwSize, 
			pszObjectsInitName, m_iOldObjNr, ppsaContent, m_iNewObjNr, 
			&m_OCConfig.uData, &m_OCConfig.iDataDiff, &m_OCConfig.uDirVars);
		m_processObjectsInit = TRUE;
		strFile = pszObjectsDiffName;
	}
	else if(strDomain.CompareNoCase(pszObjectsHashName) == 0)
	{
		hr = processObjectsHash(pszObjectsHashName, ppsaContent);
		strFile = strDomain;
	}
	else if(strDomain.CompareNoCase(GLOBALS_DEF_NAME) == 0)
	{
		hr = processGlobalsDef(GLOBALS_DEF_NAME, ppsaContent);
		strFile = strDomain;
	}
	else if(strDomain.CompareNoCase(pszCommonHdrName) == 0)
	{
		m_OCConfig.bTaskInfo = TRUE;
		strFile = strDomain;
	}
	else 
	{
		int i = strDomain.ReverseFind(_T('.'));

		if(i != -1 && strDomain.Right(strDomain.GetLength()-i).CompareNoCase(DEF_FILE_EXT) == 0)
			hr = processDefFile(strDomain, ppsaContent);

		strFile = strDomain;
	}
	
	return hr;
}

HRESULT COnlineChangesHelper::processGlobalsDef(LPCTSTR lpszGlobalDef, IN SAFEARRAY **ppsaContent)
{
	HRESULT 	  hr	= S_OK;

	CCsvParse	  File1;
	CString 	  strDomainName;
	VarDeclsArray oldGlobals;
	VarDeclsArray newGlobals;

	oldGlobals.SetSize(IDX_RETAIN+1);
	newGlobals.SetSize(IDX_RETAIN+1);

	// open file for reading
	strDomainName = GetConfigPath();
	strDomainName += lpszGlobalDef;

	hr = File1.Open(strDomainName);
	if (hr != S_OK)
	{
		TrcPrint(TRC_ERROR, _T("Opening domain '%s' for read via local copy: code=0x%08X\n"),
			(LPCTSTR) lpszGlobalDef, hr);
		return hr;
	}
	
	parseGlobalsDef(File1, oldGlobals);
	File1.Close();
	
	SAFEARRAY* psa	 = *ppsaContent;
	DWORD	   dwLen = psa->rgsabound[0].cElements;

	// write with local file write:
	LPBYTE pDomainData;
	hr = ::SafeArrayAccessData(psa, (LPVOID FAR*)&pDomainData);
	if (FAILED(hr)) {
		TrcPrint(TRC_ERROR, _T("Cannot access SafeArray while reading domain '%s': code=0x%08X\n"),
			(LPCTSTR) lpszGlobalDef, hr);
	}
	else
	{
		CCsvParse File2(pDomainData, dwLen);

		parseGlobalsDef(File2, newGlobals);
		::SafeArrayUnaccessData(psa);
	}

	CopyRegs	crs;
	SOCInstCopy ic;
	POSITION	pos1;

	////////////////////////////////
	// calculate global copy regions
	////////////////////////////////
	if(!matchDecls(newGlobals[IDX_GLOBAL], oldGlobals[IDX_GLOBAL], crs))
	{
		m_map[IDX_GLOBAL] = 0; // invalidate old global object

		/////////////////////////////
		// create global copy regions
		/////////////////////////////
		pos1 = crs.GetHeadPosition();
		while(pos1)
		{
			CopyReg* pCr = &crs.GetNext(pos1);

			ic.uOld = ic.uNew = IDX_GLOBAL;

			ic.uOldOff = (IEC_UINT)(pCr->oldOffset/8);
			ic.uOldBit = (IEC_UINT)(pCr->oldOffset%8);
			ic.uNewOff = (IEC_UINT)(pCr->newOffset/8);
			ic.uNewBit = (IEC_UINT)(pCr->newOffset%8);
			ic.ulSize  = pCr->size;

			m_ChangesList.AddTail(ic);
			m_OCConfig.uToCopy++;
		}
	}
	crs.RemoveAll();

	////////////////////////////////
	// calculate retain copy regions
	////////////////////////////////
	if(!matchDecls(newGlobals[IDX_RETAIN], oldGlobals[IDX_RETAIN], crs))
	{
		m_map[IDX_RETAIN] = 0; // invalidate old retain object

		/////////////////////////////
		// create retain copy regions
		/////////////////////////////
		pos1 = crs.GetHeadPosition();
		while(pos1)
		{
			CopyReg* pCr = &crs.GetNext(pos1);

			ic.uOld = ic.uNew = IDX_RETAIN;

			ic.uOldOff = (IEC_UINT)(pCr->oldOffset/8);
			ic.uOldBit = (IEC_UINT)(pCr->oldOffset%8);
			ic.uNewOff = (IEC_UINT)(pCr->newOffset/8);
			ic.uNewBit = (IEC_UINT)(pCr->newOffset%8);
			ic.ulSize  = pCr->size;

			m_ChangesList.AddTail(ic);
			m_OCConfig.uToCopy++;
		}
	}
	crs.RemoveAll();

	// add new direct variables
	for(int i = IDX_INPUT; i <= IDX_MEMORY; i++)
	{
		POSITION pos2;
		VarDecl* pNewVar;
		VarDecl* pOldVar;
		char	 found;

		assert(m_newDirectVars[i].GetCount() == 0);
		pos1 = newGlobals[i].GetHeadPosition();
		while (pos1)
		{
			pNewVar = &newGlobals[i].GetNext(pos1);
			found = 0;

			pos2 = oldGlobals[i].GetHeadPosition();
			while(pos2)
			{
				pOldVar = &oldGlobals[i].GetNext(pos2);
				if(pNewVar->name == pOldVar->name)
				{
					if(pNewVar->bitoffset == pOldVar->bitoffset && pNewVar->type == pOldVar->type)
						found = 1; // found and identic
	
					break;		   // found but different
				}
			}

			if(found == 0)
				m_newDirectVars[i].AddTail(*pNewVar);
		}
	}

	return hr;
}



HRESULT COnlineChangesHelper::processDefFile(LPCTSTR lpszDefFileName, IN SAFEARRAY **ppsaContent)
{
	HRESULT   hr		   = S_OK;
	CString   strClassName = lpszDefFileName;
	int 	  i;
	PairList* pList 	   = NULL;

	i = strClassName.ReverseFind(_T('.'));
	assert(i != -1);

	strClassName = strClassName.Left(i);

	if(m_pInstancesMap->Lookup(strClassName, (void*&)pList) == FALSE)
	{
		// new object type, nothing to copy
		return S_OK;
	}

	VarDecls  oldMembers;
	VarDecls  newMembers;
	CCsvParse File1;
	CString   strDomainName;

	// open file for reading
	strDomainName = GetConfigPath();
	strDomainName += lpszDefFileName;

	hr = File1.Open(strDomainName);
	if (hr != S_OK)
	{
		TrcPrint(TRC_ERROR, _T("Opening domain '%s' for read via local copy: code=0x%08X\n"),
			(LPCTSTR) lpszDefFileName, hr);
		return hr;
	}
	
	parseDefFile(File1, oldMembers);
	File1.Close();
	
	SAFEARRAY* psa	 = *ppsaContent;
	DWORD	   dwLen = psa->rgsabound[0].cElements;

	// write with local file write:
	LPBYTE pDomainData;
	hr = ::SafeArrayAccessData(psa, (LPVOID FAR*)&pDomainData);
	if (FAILED(hr)) {
		TrcPrint(TRC_ERROR, _T("Cannot access SafeArray while reading domain '%s': code=0x%08X\n"),
			(LPCTSTR) lpszDefFileName, hr);
	}
	else
	{
		CCsvParse File2(pDomainData, dwLen);

		parseDefFile(File2, newMembers);
		::SafeArrayUnaccessData(psa);
	}

	/////////////////////////
	// calculate copy regions
	/////////////////////////
	CopyRegs crs;
	matchDecls(newMembers, oldMembers, crs);

	////////////////////////////////////
	// create copy regions pro instances
	////////////////////////////////////
	SOCInstCopy ic;
	POSITION	pos1, pos2;
	OC_Pair*	pPair;
	pos1 = pList->GetHeadPosition();
	while(pos1)
	{
		pPair = &pList->GetNext(pos1);
		m_map[pPair->newIndex] = 0; // invalidate old object

		pos2 = crs.GetHeadPosition();
		while(pos2)
		{
			CopyReg* pCr = &crs.GetNext(pos2);

			ic.uOld    = pPair->oldIndex;
			ic.uNew    = pPair->newIndex;
			ic.uOldOff = (IEC_UINT)(pCr->oldOffset/8);
			ic.uOldBit = (IEC_UINT)(pCr->oldOffset%8);
			ic.uNewOff = (IEC_UINT)(pCr->newOffset/8);
			ic.uNewBit = (IEC_UINT)(pCr->newOffset%8);
			ic.ulSize  = pCr->size;

			m_ChangesList.AddTail(ic);
			m_OCConfig.uToCopy++;
		}
	}

	crs.RemoveAll();
	pList->RemoveAll();
	delete pList;
	m_pInstancesMap->RemoveKey(strClassName);

	return hr;
}

HRESULT COnlineChangesHelper::processObjectsHash(LPCTSTR lpszObjectsHashName, IN SAFEARRAY **ppsaContent)
{
	HRESULT hr = S_OK;

	// read new instances and match
	SAFEARRAY* psa	 = *ppsaContent;
	DWORD	   dwLen = psa->rgsabound[0].cElements;

	LPBYTE pDomainData;
	hr = ::SafeArrayAccessData(psa, (LPVOID FAR*)&pDomainData);
	if (FAILED(hr)) {
		TrcPrint(TRC_ERROR, _T("Cannot access SafeArray while reading domain '%s': code=0x%08X\n"),
			(LPCTSTR) lpszObjectsHashName, hr);
	}
	else
	{
		// old Instances Names are already in m_pInstancesMap
		CMapStringToPtr* pTempMap = m_pInstancesMap;
		m_pInstancesMap = new CMapStringToPtr;
		m_iNewObjNr = 2; // reserve 2 objects for global and retain objects

		CCsvParse File2(pDomainData, dwLen);
		CString   field1;
		int 	  fieldNo = 1;
		char*	  stopchar;
		IEC_UINT  newIndex = 0;
		IEC_UINT  newSize = 0;
		CString   newName;
		PairList* pList;

		while(File2.ParseNextField(field1))
		{
			switch(fieldNo)
			{
			case 1:
				newName = field1;
				break;
			case 3:
				newIndex = (IEC_UINT)_tcstoul((LPCTSTR)field1, &stopchar, 10);
				break;
			case 5:
				newSize = (IEC_UINT)_tcstoul((LPCTSTR)field1, &stopchar, 10);
				break;
			case 10:
				m_map.SetAtGrow(newIndex, 0);
				if(pTempMap->Lookup(field1, (void*&)pList))
				{
					POSITION pos = pList->GetHeadPosition();
					OC_Pair* pPair;

					while(pos)
					{
						pPair = &pList->GetNext(pos);

						if(newName == pPair->name)
						{
							addIndexPair(field1, newName, pPair->oldIndex, pPair->oldSize, newIndex, newSize);
							break;
						}
					}
				}
				m_iNewObjNr++;

				break;
			}

			if(File2.IsEndOfLine())
				fieldNo = 1;
			else
				fieldNo++;
		}
		assert(fieldNo == 1);
		::SafeArrayUnaccessData(psa);

		emptyInstancesMap(pTempMap);
		delete pTempMap;
	}

	return hr;
}

/*
	INPUT
	lpszDiffName	- name of the generated diference file
	lpdwDiffSize	- size of the generated diference file
	lpszDomainName	- name of the original file
	ppsaContent 	- new content of the file
	OUTPUT
	pNoOfDiff		- number of generated objects
	pDelta			- difference between nr of new Classed and nr. of old Classes

	return S_OK if diff file successfuly created 
		   E_FAILED = not know to diff this file
*/
HRESULT COnlineChangesHelper::generateClassesDiff(LPCTSTR lpszDiffName, LPDWORD lpdwDiffSize,
			LPCTSTR lpszDomainName, IN SAFEARRAY **ppsaContent,
			IEC_UINT* pNoOfDiffs, IEC_INT* pDelta)
{	 
	HRESULT hr;
	
	*pNoOfDiffs 	 = 0;
	*pDelta 		 = 0;
	*lpdwDiffSize	 = 0;

	// check safearray format:
	if (   ((*ppsaContent)->cDims != 1)
		|| ((*ppsaContent)->fFeatures & (FADF_BSTR|FADF_UNKNOWN|FADF_DISPATCH|FADF_VARIANT))
		|| ((*ppsaContent)->cbElements != sizeof(BYTE))
		|| ((*ppsaContent)->rgsabound[0].lLbound != 0))
	{
		return E_INVALIDARG;
	}
	   
	// open file for reading
	CString strDomainName = GetConfigPath();
	strDomainName += lpszDomainName;

	HANDLE hFile = ::CreateFile(strDomainName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		hr = HRESULT_FROM_WIN32(::GetLastError());
		TrcPrint(TRC_ERROR, _T("Opening domain '%s' for read via local copy: code=0x%08X\n"),
			(LPCTSTR) lpszDomainName, hr);
	}

	// open diff file for writting
	CString strDiffName = GetConfigPath();
	strDiffName += lpszDiffName;

	DWORD dwWritten = 0;
	DWORD dwTemp;
	DWORD pos1 = 0;
	DWORD pos2 = 0;
	const XDownHeader* pO1;
	const XDownHeader* pO2;

	SAFEARRAY* psa	  = *ppsaContent;
	DWORD	   dwLen2 = psa->rgsabound[0].cElements;
	DWORD	   dwLen1 = ::GetFileSize(hFile, NULL);
	if (dwLen1 == INVALID_FILE_SIZE)
	{
		hr = HRESULT_FROM_WIN32(::GetLastError());
		TrcPrint(TRC_ERROR, _T("Reading domain '%s' via local copy: code=0x%08X\n"),
			(LPCTSTR) strDomainName, hr);
		return hr;
	}

	LPBYTE lpBuff = new BYTE[dwLen1];

	BOOL bOK = ::ReadFile(hFile, lpBuff, dwLen1, &dwTemp, NULL);
	if (!bOK)
	{
		hr = HRESULT_FROM_WIN32(::GetLastError());
		TrcPrint(TRC_ERROR, _T("Reading domain '%s' via local copy: code=0x%08X\n"),
			(LPCTSTR) strDomainName, hr);
	}
	assert(dwLen1 == dwTemp);
	CloseHandle(hFile);

	HANDLE hDiffFile = ::CreateFile(strDiffName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
	if (hDiffFile == INVALID_HANDLE_VALUE)
	{
		hr = HRESULT_FROM_WIN32(::GetLastError());
		TrcPrint(TRC_ERROR, _T("Opening domain '%s' for write via local copy: code=0x%08X\n"),
			(LPCTSTR) lpszDiffName, hr);
		delete lpBuff;
		return hr;
	}

	// write with local file write:
	LPBYTE pDomainData;
	hr = ::SafeArrayAccessData(psa, (LPVOID FAR*)&pDomainData);
	if (FAILED(hr)) {
		TrcPrint(TRC_ERROR, _T("Cannot access SafeArray while writing domain '%s': code=0x%08X\n"),
			(LPCTSTR) lpszDiffName, hr);
		CloseHandle(hDiffFile);
		delete lpBuff;
		return hr;
	}
	
	while (pos1 < dwLen1 && pos2 < dwLen2)
	{
		if(pos1 + sizeof(XDownHeader) > dwLen1 ||
			pos2 + sizeof(XDownHeader) > dwLen2)
		{
			TrcPrint(TRC_ERROR, _T("Wrong file format domain '%s': code=0x%08X\n"),
				(LPCTSTR) lpszDiffName, hr);
			::SafeArrayUnaccessData(psa);
			CloseHandle(hDiffFile);
			delete lpBuff;
			return E_FAIL;
		}

		pO1 = (const XDownHeader*)(lpBuff + pos1);
		pO2 = (const XDownHeader*)(pDomainData + pos2); 
		if(pos1 + sizeof(XDownHeader) + TRANSLATE_LONG(pO1->ulSize) > dwLen1 ||
			pos2 + sizeof(XDownHeader) + TRANSLATE_LONG(pO2->ulSize) > dwLen2)
		{
			TrcPrint(TRC_ERROR, _T("Wrong file format domain '%s': code=0x%08X\n"),
				(LPCTSTR) lpszDiffName, hr);
			::SafeArrayUnaccessData(psa);
			CloseHandle(hDiffFile);
			delete lpBuff;
			return E_FAIL;
		}
		
		if(memcmp(lpBuff + pos1, pDomainData + pos2, sizeof(XDownHeader)+min(TRANSLATE_LONG(pO1->ulSize), TRANSLATE_LONG(pO2->ulSize))))
		{
			bOK = ::WriteFile(hDiffFile, pDomainData+pos2, sizeof(XDownHeader)+ TRANSLATE_LONG(pO2->ulSize), &dwTemp, NULL);
			if (!bOK)
			{
				hr = HRESULT_FROM_WIN32(::GetLastError());
				TrcPrint(TRC_ERROR, _T("Writting domain '%s' via local copy: code=0x%08X\n"),
					(LPCTSTR) strDiffName, hr);
			}
			assert(sizeof(XDownHeader) + TRANSLATE_LONG(pO2->ulSize) == dwTemp);
			dwWritten += dwTemp;
			(*pNoOfDiffs)++;
		}
		pos1 += sizeof(XDownHeader)+ TRANSLATE_LONG(pO1->ulSize);
		pos2 += sizeof(XDownHeader)+ TRANSLATE_LONG(pO2->ulSize);
	}

	while (pos2 < dwLen2)
	{
		if(pos2 + sizeof(XDownHeader) > dwLen2)
		{
			TrcPrint(TRC_ERROR, _T("Wrong file format domain '%s': code=0x%08X\n"),
				(LPCTSTR) lpszDiffName, hr);
			::SafeArrayUnaccessData(psa);
			CloseHandle(hDiffFile);
			delete lpBuff;
			return E_FAIL;
		}

		pO2 = (const XDownHeader*)(pDomainData + pos2); 
		if(pos2 + sizeof(XDownHeader) + TRANSLATE_LONG(pO2->ulSize) > dwLen2)
		{
			TrcPrint(TRC_ERROR, _T("Wrong file format domain '%s': code=0x%08X\n"),
				(LPCTSTR) lpszDiffName, hr);
			::SafeArrayUnaccessData(psa);
			CloseHandle(hDiffFile);
			delete lpBuff;
			return E_FAIL;
		}
		
		bOK = ::WriteFile(hDiffFile, pDomainData+pos2, sizeof(XDownHeader)+TRANSLATE_LONG(pO2->ulSize), &dwTemp, NULL);
		if (!bOK)
		{
			hr = HRESULT_FROM_WIN32(::GetLastError());
			TrcPrint(TRC_ERROR, _T("Writting domain '%s' via local copy: code=0x%08X\n"),
				(LPCTSTR) strDiffName, hr);
		}
		assert(sizeof(XDownHeader)+TRANSLATE_LONG(pO2->ulSize) == dwTemp);
		dwWritten += dwTemp;
		(*pNoOfDiffs)++;

		pos2 += sizeof(XDownHeader)+TRANSLATE_LONG(pO2->ulSize);
		(*pDelta)++;
	}
	::SafeArrayUnaccessData(psa);
	CloseHandle(hDiffFile);
	TRACE(_T("Domain '%s' written via local copy, %d bytes\n"), (LPCTSTR) lpszDiffName, dwWritten);

	while (pos1 < dwLen1)
	{
		if(pos1 + sizeof(XDownHeader) > dwLen1)
		{
			TrcPrint(TRC_ERROR, _T("Wrong file format domain '%s': code=0x%08X\n"),
				(LPCTSTR) lpszDiffName, hr);
			delete lpBuff;
			return E_FAIL;
		}

		pO1 = (const XDownHeader*)(lpBuff + pos1);
		if(pos1 + sizeof(XDownHeader) + TRANSLATE_LONG(pO1->ulSize) > dwLen1)
		{
			TrcPrint(TRC_ERROR, _T("Wrong file format domain '%s': code=0x%08X\n"),
				(LPCTSTR) lpszDiffName, hr);
			delete lpBuff;
			return E_FAIL;
		}
		
		pos1 += sizeof(XDownHeader)+TRANSLATE_LONG(pO1->ulSize);
		(*pDelta)--;
	}
	delete lpBuff;

	*lpdwDiffSize = dwWritten;

	return hr;
}

/*
	INPUT
	lpszDiffName	- name of the generated diference file
	lpdwDiffSize	- size of the generated diference file
	lpszDomainName	- name of the original file
	objNr			- number of objects in original file the rest are direct variables
					  if -1 then assume that the file contains only objects

	ppsaContent 	- new content of the file
	newObjNr		- number of objects in original file the rest are direct variables
					  if -1 then assume that the file contains only objects

	OUTPUT
	pNoOfDiff		- number of generated objects
	pDelta			- difference between nr of new Objects and nr. of old Objects
	pNoOfDirectVars - number of generated directVariables

	return S_OK if diff file successfuly created 
		   E_FAILED = not know to diff this file
*/
HRESULT COnlineChangesHelper::generateObjectsDiff(LPCTSTR lpszDiffName, LPDWORD lpdwDiffSize,
			LPCTSTR lpszDomainName, int oldObjNr,
			IN SAFEARRAY **ppsaContent, int newObjNr,
			IEC_UINT* pNoOfDiffs, IEC_INT* pDelta, 
			IEC_UINT* pNoOfDirectVars)
{	 
	HRESULT hr;
	
	*pNoOfDiffs 	 = 0;
	*pDelta 		 = 0;
	*pNoOfDirectVars = 0;
	*lpdwDiffSize	 = 0;

	// check safearray format:
	if (   ((*ppsaContent)->cDims != 1)
		|| ((*ppsaContent)->fFeatures & (FADF_BSTR|FADF_UNKNOWN|FADF_DISPATCH|FADF_VARIANT))
		|| ((*ppsaContent)->cbElements != sizeof(BYTE))
		|| ((*ppsaContent)->rgsabound[0].lLbound != 0))
	{
		return E_INVALIDARG;
	}
	   
	// open file for reading
	CString strDomainName = GetConfigPath();
	strDomainName += lpszDomainName;

	HANDLE hFile = ::CreateFile(strDomainName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		hr = HRESULT_FROM_WIN32(::GetLastError());
		TrcPrint(TRC_ERROR, _T("Opening domain '%s' for read via local copy: code=0x%08X\n"),
			(LPCTSTR) lpszDomainName, hr);
	}

	// open diff file for writting
	CString strDiffName = GetConfigPath();
	strDiffName += lpszDiffName;

	DWORD dwWritten = 0;
	DWORD dwTemp;
	DWORD pos1 = 0;
	DWORD pos2 = 0;
	const XDownHeader* pO1;
	const XDownHeader* pO2;

	SAFEARRAY* psa	  = *ppsaContent;
	DWORD	   dwLen2 = psa->rgsabound[0].cElements;
	DWORD	   dwLen1 = ::GetFileSize(hFile, NULL);
	if (dwLen1 == INVALID_FILE_SIZE)
	{
		hr = HRESULT_FROM_WIN32(::GetLastError());
		TrcPrint(TRC_ERROR, _T("Reading domain '%s' via local copy: code=0x%08X\n"),
			(LPCTSTR) strDomainName, hr);
		return hr;
	}

	LPBYTE lpBuff = new BYTE[dwLen1];

	BOOL bOK = ::ReadFile(hFile, lpBuff, dwLen1, &dwTemp, NULL);
	if (!bOK)
	{
		hr = HRESULT_FROM_WIN32(::GetLastError());
		TrcPrint(TRC_ERROR, _T("Reading domain '%s' via local copy: code=0x%08X\n"),
			(LPCTSTR) strDomainName, hr);
	}
	assert(dwLen1 == dwTemp);
	CloseHandle(hFile);

	HANDLE hDiffFile = ::CreateFile(strDiffName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
	if (hDiffFile == INVALID_HANDLE_VALUE)
	{
		hr = HRESULT_FROM_WIN32(::GetLastError());
		TrcPrint(TRC_ERROR, _T("Opening domain '%s' for write via local copy: code=0x%08X\n"),
			(LPCTSTR) lpszDiffName, hr);
		delete lpBuff;
		return hr;
	}

	// write with local file write:
	LPBYTE pDomainData;
	hr = ::SafeArrayAccessData(psa, (LPVOID FAR*)&pDomainData);
	if (FAILED(hr)) {
		TrcPrint(TRC_ERROR, _T("Cannot access SafeArray while writing domain '%s': code=0x%08X\n"),
			(LPCTSTR) lpszDiffName, hr);
		CloseHandle(hDiffFile);
		delete lpBuff;
		return hr;
	}
	
	while (pos1 < dwLen1 && oldObjNr-- > 0 && pos2 < dwLen2 && newObjNr-- > 0)
	{
		if(pos1 + sizeof(XDownHeader) > dwLen1 ||
			pos2 + sizeof(XDownHeader) > dwLen2)
		{
			TrcPrint(TRC_ERROR, _T("Wrong file format domain '%s': code=0x%08X\n"),
				(LPCTSTR) lpszDiffName, hr);
			::SafeArrayUnaccessData(psa);
			CloseHandle(hDiffFile);
			delete lpBuff;
			return E_FAIL;
		}

		pO1 = (const XDownHeader*)(lpBuff + pos1);
		pO2 = (const XDownHeader*)(pDomainData + pos2); 
		if(pos1 + sizeof(XDownHeader) + TRANSLATE_LONG(pO1->ulSize) > dwLen1 ||
			pos2 + sizeof(XDownHeader) + TRANSLATE_LONG(pO2->ulSize) > dwLen2)
		{
			TrcPrint(TRC_ERROR, _T("Wrong file format domain '%s': code=0x%08X\n"),
				(LPCTSTR) lpszDiffName, hr);
			::SafeArrayUnaccessData(psa);
			CloseHandle(hDiffFile);
			delete lpBuff;
			return E_FAIL;
		}

		// download if the object not identic with the previous object
		if(m_map[TRANSLATE_SHORT(pO2->uIndex)] != TRANSLATE_SHORT(pO1->uIndex) || TRANSLATE_LONG(pO1->ulSize) != TRANSLATE_LONG(pO2->ulSize))
		{
			bOK = ::WriteFile(hDiffFile, pDomainData+pos2, sizeof(XDownHeader)+TRANSLATE_LONG(pO2->ulSize), &dwTemp, NULL);
			if (!bOK)
			{
				hr = HRESULT_FROM_WIN32(::GetLastError());
				TrcPrint(TRC_ERROR, _T("Writting domain '%s' via local copy: code=0x%08X\n"),
					(LPCTSTR) strDiffName, hr);
			}
			assert(sizeof(XDownHeader)+TRANSLATE_LONG(pO2->ulSize) == dwTemp);
			dwWritten += dwTemp;
			(*pNoOfDiffs)++;
		}
		else
		{
			assert(TRANSLATE_SHORT(pO1->uIndex) == TRANSLATE_SHORT(pO2->uIndex) && TRANSLATE_LONG(pO1->ulSize) == TRANSLATE_LONG(pO2->ulSize)); // the same object only init values changed

			if(memcmp(lpBuff + pos1, pDomainData + pos2, sizeof(XDownHeader) + TRANSLATE_LONG(pO1->ulSize)) != 0)
			{
				CString type = m_objType[pO2->uIndex];
				PairList* pList 	   = NULL;

				// objects removed from m_pInstancesMap are already processed; ignore them
				if(m_pInstancesMap->Lookup(type, (void*&)pList) != FALSE)
				{
					VarDecls  members;
					CCsvParse File1;
					CString   strDomainName;

					// open file for reading
					strDomainName = GetConfigPath();
					strDomainName += type;
					strDomainName += DEF_FILE_EXT;

					hr = File1.Open(strDomainName);
					if (hr != S_OK)
					{
						strDomainName = type;
						strDomainName += DEF_FILE_EXT;
						TrcPrint(TRC_ERROR, _T("Opening domain '%s' for read via local copy: code=0x%08X\n"),
							(LPCTSTR) strDomainName, hr);
						return hr;
					}
					
					parseDefFile(File1, members);
					File1.Close();

					POSITION pos;
					CopyReg* pCr = NULL;
					CopyRegs crs;
					matchDecls(members, members, crs);

					pos = crs.GetHeadPosition();
					if(pos)
						pCr = &crs.GetAt(pos);

					// copy only if there are object indexes that should not be copied
					if(pos && pCr->size < pO2->ulSize*8)
					{
						SOCInstCopy ic;
						memset(&ic, 0, sizeof(ic));

						pos = crs.GetHeadPosition();
						while(pos)
						{
							pCr = &crs.GetNext(pos);

							ic.uOld    = pO2->uIndex;
							ic.uNew    = pO2->uIndex;
							ic.uOldOff = (IEC_UINT)(pCr->oldOffset/8);
							ic.uOldBit = (IEC_UINT)(pCr->oldOffset%8);
							ic.uNewOff = (IEC_UINT)(pCr->newOffset/8);
							ic.uNewBit = (IEC_UINT)(pCr->newOffset%8);
							ic.ulSize  = pCr->size;

							m_ChangesList.AddTail(ic);
							m_OCConfig.uToCopy++;
						}
						crs.RemoveAll();

						bOK = ::WriteFile(hDiffFile, pDomainData+pos2, sizeof(XDownHeader)+TRANSLATE_LONG(pO2->ulSize), &dwTemp, NULL);
						if (!bOK)
						{
							hr = HRESULT_FROM_WIN32(::GetLastError());
							TrcPrint(TRC_ERROR, _T("Writting domain '%s' via local copy: code=0x%08X\n"),
								(LPCTSTR) strDiffName, hr);
						}
						assert(sizeof(XDownHeader)+TRANSLATE_LONG(pO2->ulSize) == dwTemp);
						dwWritten += dwTemp;
						(*pNoOfDiffs)++;
					}
				}
			}
		}

		pos1 += sizeof(XDownHeader)+TRANSLATE_LONG(pO1->ulSize);
		pos2 += sizeof(XDownHeader)+TRANSLATE_LONG(pO2->ulSize);
	}

	while (pos2 < dwLen2 && newObjNr-- > 0)
	{
		if(pos2 + sizeof(XDownHeader) > dwLen2)
		{
			TrcPrint(TRC_ERROR, _T("Wrong file format domain '%s': code=0x%08X\n"),
				(LPCTSTR) lpszDiffName, hr);
			::SafeArrayUnaccessData(psa);
			CloseHandle(hDiffFile);
			delete lpBuff;
			return E_FAIL;
		}

		pO2 = (const XDownHeader*)(pDomainData + pos2); 
		if(pos2 + sizeof(XDownHeader) + TRANSLATE_LONG(pO2->ulSize) > dwLen2)
		{
			TrcPrint(TRC_ERROR, _T("Wrong file format domain '%s': code=0x%08X\n"),
				(LPCTSTR) lpszDiffName, hr);
			::SafeArrayUnaccessData(psa);
			CloseHandle(hDiffFile);
			delete lpBuff;
			return E_FAIL;
		}
		
		bOK = ::WriteFile(hDiffFile, pDomainData+pos2, sizeof(XDownHeader)+TRANSLATE_LONG(pO2->ulSize), &dwTemp, NULL);
		if (!bOK)
		{
			hr = HRESULT_FROM_WIN32(::GetLastError());
			TrcPrint(TRC_ERROR, _T("Writting domain '%s' via local copy: code=0x%08X\n"),
				(LPCTSTR) strDiffName, hr);
		}
		assert(sizeof(XDownHeader)+TRANSLATE_LONG(pO2->ulSize) == dwTemp);
		dwWritten += dwTemp;
		(*pNoOfDiffs)++;

		pos2 += sizeof(XDownHeader)+TRANSLATE_LONG(pO2->ulSize);
		(*pDelta)++;
	}

	// dump direct variables;
	while (pos2 < dwLen2)
	{
		const XDownDirect* pD2;
	
		if(pos2 + sizeof(XDownDirect) > dwLen2)
		{
			TrcPrint(TRC_ERROR, _T("Wrong file format domain '%s': code=0x%08X\n"),
				(LPCTSTR) lpszDiffName, hr);
			::SafeArrayUnaccessData(psa);
			CloseHandle(hDiffFile);
			delete lpBuff;
			return E_FAIL;
		}

		pD2 = (const XDownDirect*)(pDomainData + pos2); 
		if(pos2 + sizeof(XDownDirect) + (TRANSLATE_SHORT(pD2->uSize) + 7)/8 > dwLen2)
		{
			TrcPrint(TRC_ERROR, _T("Wrong file format domain '%s': code=0x%08X\n"),
				(LPCTSTR) lpszDiffName, hr);
			::SafeArrayUnaccessData(psa);
			CloseHandle(hDiffFile);
			delete lpBuff;
			return E_FAIL;
		}
		
		// check if the variable should be downloaded
		char	 insert = 0;
		VarDecl* decl;
		POSITION pos	= m_newDirectVars[pD2->bySegment].GetHeadPosition();
		while(pos)
		{
			decl = &m_newDirectVars[pD2->bySegment].GetAt(pos);

			if(decl->bitsize == (IEC_UDINT)TRANSLATE_SHORT(pD2->uSize) &&
				decl->bitoffset/8 == (IEC_UDINT)TRANSLATE_LONG(pD2->ulOffset) && 
				((decl->bitoffset%8)?1<<(decl->bitoffset%8 - 1):0) == pD2->byBit)
			{
				m_newDirectVars[pD2->bySegment].RemoveAt(pos);
				insert = 1;
				break;
			}
			m_newDirectVars[pD2->bySegment].GetNext(pos);
		}

		if(insert)
		{
			bOK = ::WriteFile(hDiffFile, pDomainData+pos2, sizeof(XDownDirect)+(TRANSLATE_SHORT(pD2->uSize)+7)/8, &dwTemp, NULL);
			if (!bOK)
			{
				hr = HRESULT_FROM_WIN32(::GetLastError());
				TrcPrint(TRC_ERROR, _T("Writting domain '%s' via local copy: code=0x%08X\n"),
					(LPCTSTR) strDiffName, hr);
			}
			assert(sizeof(XDownDirect)+(TRANSLATE_SHORT(pD2->uSize)+7)/8 == dwTemp);
			dwWritten += dwTemp;
			(*pNoOfDirectVars)++;
		}

		pos2 += sizeof(XDownDirect)+(TRANSLATE_SHORT(pD2->uSize)+7)/8;
	}

	assert(m_newDirectVars[IDX_INPUT].GetCount() == 0);
	assert(m_newDirectVars[IDX_OUTPUT].GetCount() == 0);
	assert(m_newDirectVars[IDX_MEMORY].GetCount() == 0);

	::SafeArrayUnaccessData(psa);
	CloseHandle(hDiffFile);
	TRACE(_T("Domain '%s' written via local copy, %d bytes\n"), (LPCTSTR) lpszDiffName, dwWritten);

	while (pos1 < dwLen1 && oldObjNr-- > 0)
	{
		if(pos1 + sizeof(XDownHeader) > dwLen1)
		{
			TrcPrint(TRC_ERROR, _T("Wrong file format domain '%s': code=0x%08X\n"),
				(LPCTSTR) lpszDiffName, hr);
			delete lpBuff;
			return E_FAIL;
		}

		pO1 = (const XDownHeader*)(lpBuff + pos1);
		if(pos1 + sizeof(XDownHeader) + TRANSLATE_LONG(pO1->ulSize) > dwLen1)
		{
			TrcPrint(TRC_ERROR, _T("Wrong file format domain '%s': code=0x%08X\n"),
				(LPCTSTR) lpszDiffName, hr);
			delete lpBuff;
			return E_FAIL;
		}
		
		pos1 += sizeof(XDownHeader)+TRANSLATE_LONG(pO1->ulSize);
		(*pDelta)--;
	}
	delete lpBuff;

	*lpdwDiffSize = dwWritten;

	return hr;
}


HRESULT COnlineChangesHelper::createOCConfig()
{ 
	assert(m_isInit == 0);
	assert(m_pbBuffer == 0);

	if(m_pbBuffer)
	{
		delete m_pbBuffer;
		m_pbBuffer = NULL;
	}

	POSITION pos;
	int 	 i	  = sizeof(XOnlineChange) + sizeof(XOCInstCopy) * m_ChangesList.GetCount();
	
#if 0
	if(m_OCConfig.bTaskInfo)
	{
		HRESULT hr;
		// open config file for reading
		CString strCommonHdrName = GetConfigPath();
		strCommonHdrName += pszCommonHdrName;
		HANDLE hFile = ::CreateFile(strCommonHdrName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			hr = HRESULT_FROM_WIN32(::GetLastError());
			TrcPrint(TRC_ERROR, _T("Opening domain '%s' for read: code=0x%08X\n"),
				(LPCTSTR) pszObjectsInitName, hr);
			return hr;
		}

		DWORD dwTemp;
		DWORD dwLen1  =  ::GetFileSize(hFile, NULL);
		i			  += dwLen1;
		m_pbBuffer	  =  new BYTE[i];
		m_iBufferSize =  i;

		BOOL bOK = ::ReadFile(hFile, m_pbBuffer + i - dwLen1, dwLen1, &dwTemp, NULL);
		if (!bOK)
		{
			hr = HRESULT_FROM_WIN32(::GetLastError());
			TrcPrint(TRC_ERROR, _T("Reading domain '%s': code=0x%08X\n"),
				(LPCTSTR) pszObjectsInitName, hr);
			delete m_pbBuffer;
			m_pbBuffer	  = NULL;
			m_iBufferSize = 0;
			return hr;
		}
		assert(dwLen1 == dwTemp);
		CloseHandle(hFile); 	
	}
	else
#endif
	{
		m_pbBuffer	  = new BYTE[i];
		m_iBufferSize = i;
	}

	XOnlineChange *pOC = (XOnlineChange *)m_pbBuffer;

	TrcPrint(TRC_INTERFACE, _T("Online Changes:\n"));	
	TrcPrint(TRC_INTERFACE, _T(" downloaded code objects:     %d\n"), m_OCConfig.uCode);	
	TrcPrint(TRC_INTERFACE, _T(" code objects difference:     %d\n"), m_OCConfig.iCodeDiff);	
	TrcPrint(TRC_INTERFACE, _T(" downloaded instances:        %d\n"), m_OCConfig.uData);	
	TrcPrint(TRC_INTERFACE, _T(" instances difference:        %d\n"), m_OCConfig.iDataDiff);	
	TrcPrint(TRC_INTERFACE, _T(" downloaded direct variables: %d\n"), m_OCConfig.uDirVars); 
	TrcPrint(TRC_INTERFACE, _T(" downloaded copy regions:     %d\n"), m_OCConfig.uToCopy);
	TrcPrint(TRC_INTERFACE, m_OCConfig.bTaskInfo?_T(" download tasks' infos\n"):_T(" no download of tasks' infos\n"));
	TrcPrint(TRC_INTERFACE, _T(" Copy regions:\n"));

	pOC->uCode		= TRANSLATE_SHORT(m_OCConfig.uCode);
	pOC->uData		= TRANSLATE_SHORT(m_OCConfig.uData);
	pOC->uToCopy	= TRANSLATE_SHORT(m_OCConfig.uToCopy);
	pOC->uDirVars	= TRANSLATE_SHORT(m_OCConfig.uDirVars);
	pOC->iCodeDiff	= TRANSLATE_SHORT(m_OCConfig.iCodeDiff);
	pOC->iDataDiff	= TRANSLATE_SHORT(m_OCConfig.iDataDiff);
	pOC->uTaskInfo	= TRANSLATE_SHORT((IEC_UINT)(m_OCConfig.bTaskInfo ? 1 : 0));

	i = sizeof(XOnlineChange);

	for(pos = m_ChangesList.GetHeadPosition(); pos; )
	{
		XOCInstCopy *pDst = (XOCInstCopy *)(m_pbBuffer + i);
		SOCInstCopy *pSrc = &m_ChangesList.GetNext(pos);

		TrcPrint(TRC_INTERFACE, _T("  %3d -> %3d  off: %04d.%d -> %04d.%d  size %d %s\n"),
			pSrc->uOld, pSrc->uNew, pSrc->uOldOff, pSrc->uOldBit, pSrc->uNewOff, pSrc->uNewBit, 
			pSrc->ulSize%8?pSrc->ulSize:pSrc->ulSize/8, pSrc->ulSize%8?_T("bit(s)"):_T("byte(s)"));

		pDst->uOld		= TRANSLATE_SHORT(pSrc->uOld);
		pDst->uNew		= TRANSLATE_SHORT(pSrc->uNew);
		pDst->uOldOff	= TRANSLATE_SHORT(pSrc->uOldOff);
		pDst->uNewOff	= TRANSLATE_SHORT(pSrc->uNewOff);
		
		pDst->ulSize	= TRANSLATE_LONG(pSrc->ulSize);

		pDst->usOldBit	= (IEC_USINT)(pSrc->uOldBit);
		pDst->usNewBit	= (IEC_USINT)(pSrc->uNewBit);

		i += sizeof(XOCInstCopy);
	}
	return S_OK;
}

void COnlineChangesHelper::destroyOCConfig()
{ 
	if(m_pbBuffer)
	{
		delete m_pbBuffer;
		m_pbBuffer = NULL;
	}
	m_iBufferSize = 0;
}



HRESULT COnlineChangesHelper::startOC()
{
	HRESULT hr;

	Cleanup();
	assert(m_pInstancesMap == 0);
	assert(m_ChangesList.GetCount() == 0);
	m_pInstancesMap = new CMapStringToPtr;

	// open file for reading
	CString strDomainName = GetConfigPath();
	strDomainName += pszObjectsHashName;

	CCsvParse File1;
	CString   field1;
	CString   type;
	int 	  fieldNo;
	OC_Pair   oldInst;
	char*	  stopchar;

	hr = File1.Open(strDomainName);
	if (hr != S_OK)
	{
		TrcPrint(TRC_ERROR, _T("Opening domain '%s' for read via local copy: code=0x%08X\n"),
			(LPCTSTR) pszObjectsHashName, hr);
		return hr;
	}

	fieldNo = 1;
	while(File1.ParseNextField(field1))
	{
		switch(fieldNo)
		{
		case 1:
			oldInst.name	 = field1;
			type			 = _T("");
			break;
		case 3:
			oldInst.newIndex = oldInst.oldIndex = (IEC_UINT)_tcstoul((LPCTSTR)field1, &stopchar, 10);
			break;
		case 5:
			oldInst.newSize  = oldInst.oldSize	= (IEC_UINT)_tcstoul((LPCTSTR)field1, &stopchar, 10);
			break;
		case 10:
			type = field1;
			break;
		}

		if(File1.IsEndOfLine())
		{
			addIndexPair(type, oldInst.name, oldInst.oldIndex, oldInst.oldSize, oldInst.newIndex, oldInst.newSize);

			m_iNewObjNr++; // assume no instance nr. changes (overwritten eventually on process of __objects.hash)
			m_iOldObjNr++;
			fieldNo = 1;
		}
		else
			fieldNo++;
	}
	assert(fieldNo == 1);
	File1.Close();

	m_map.SetAtGrow(IDX_RETAIN, IDX_RETAIN);
	m_map.SetAtGrow(IDX_GLOBAL, IDX_GLOBAL);
	m_isInit = TRUE;

	return hr;
}


HRESULT COnlineChangesHelper::finishOC()
{
	HRESULT   hr = S_OK;
	POSITION  pos1, pos2, pos3;
	CString   className;
	PairList* pPairs;
	OC_Pair*  pair = NULL;

	SOCInstCopy ic;
	memset(&ic, 0, sizeof(ic));

	pos1 = m_pInstancesMap->GetStartPosition();
	while(pos1)
	{
		m_pInstancesMap->GetNextAssoc(pos1, className, (void*&)pPairs);

		VarDecls  oldnewMembers;
		CCsvParse File1;
		CString   strDomainName;

		// open file for reading
		strDomainName = GetConfigPath();
		strDomainName += className;
		strDomainName += DEF_FILE_EXT;

		hr = File1.Open(strDomainName);
		if (hr != S_OK)
		{
			strDomainName = pair->name;
			strDomainName += DEF_FILE_EXT;
			TrcPrint(TRC_ERROR, _T("Opening domain '%s' for read via local copy: code=0x%08X\n"),
				(LPCTSTR) strDomainName, hr);
			return hr;
		}
		
		parseDefFile(File1, oldnewMembers);
		File1.Close();

		CopyRegs crs;
		matchDecls(oldnewMembers, oldnewMembers, crs);

		pos2 = pPairs->GetHeadPosition();
		while(pos2)
		{
			pair = &pPairs->GetNext(pos2);
			
			assert(pair->newSize == pair->oldSize);
			if(pair->newIndex != pair->oldIndex)
			{
				pos3 = crs.GetHeadPosition();
				while(pos3)
				{
					CopyReg* pCr = &crs.GetNext(pos3);

					ic.uOld    = pair->oldIndex;
					ic.uNew    = pair->newIndex;
					ic.uOldOff = (IEC_UINT)(pCr->oldOffset/8);
					ic.uOldBit = (IEC_UINT)(pCr->oldOffset%8);
					ic.uNewOff = (IEC_UINT)(pCr->newOffset/8);
					ic.uNewBit = (IEC_UINT)(pCr->newOffset%8);
					ic.ulSize  = pCr->size;

					m_ChangesList.AddTail(ic);
					m_OCConfig.uToCopy++;
				}
			}
		}
		crs.RemoveAll();
	}

	// check if there are direct variables with different name but with 
	// the same init values (there is no __object.init downloaded so take
	// the init values from the old __objects.init
	if(!m_processObjectsInit && 	
		(m_ChangesList.GetCount() != 0 ||
		m_newDirectVars[IDX_INPUT].GetCount()  != 0 ||
		m_newDirectVars[IDX_OUTPUT].GetCount() != 0 ||
		m_newDirectVars[IDX_MEMORY].GetCount() != 0))
	{
		assert(m_OCConfig.uData == 0);
		assert(m_OCConfig.uDirVars == 0);
		assert(m_iOldObjNr == m_iNewObjNr);
	
		// open file for reading
		CString strDomainName = GetConfigPath();
		strDomainName += pszObjectsInitName;

		HANDLE hFile = ::CreateFile(strDomainName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			hr = HRESULT_FROM_WIN32(::GetLastError());
			TrcPrint(TRC_ERROR, _T("Opening domain '%s' for read via local copy: code=0x%08X\n"),
				(LPCTSTR) pszObjectsInitName, hr);
		}

		// open diff file for writting
		CString strDiffName = GetConfigPath();
		strDiffName += pszObjectsDiffName;

		DWORD dwWritten = 0;
		DWORD dwTemp;
		DWORD pos = 0;
		const XDownHeader* pO1;
		int   objNr = m_iOldObjNr;


		DWORD  dwLen1 = ::GetFileSize(hFile, NULL);
		LPBYTE lpBuff = new BYTE[dwLen1];

		BOOL bOK = ::ReadFile(hFile, lpBuff, dwLen1, &dwTemp, NULL);
		if (!bOK)
		{
			hr = HRESULT_FROM_WIN32(::GetLastError());
			TrcPrint(TRC_ERROR, _T("Reading domain '%s' via local copy: code=0x%08X\n"),
				(LPCTSTR) strDomainName, hr);
		}
		assert(dwLen1 == dwTemp);
		CloseHandle(hFile);

		HANDLE hDiffFile = ::CreateFile(strDiffName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
		if (hDiffFile == INVALID_HANDLE_VALUE)
		{
			hr = HRESULT_FROM_WIN32(::GetLastError());
			TrcPrint(TRC_ERROR, _T("Opening domain '%s' for write via local copy: code=0x%08X\n"),
				(LPCTSTR) pszObjectsDiffName, hr);
			delete lpBuff;
			return hr;
		}

		// skip instances
		while (pos < dwLen1 && objNr--)
		{			
			if(pos + sizeof(XDownHeader) > dwLen1)
			{
				TrcPrint(TRC_ERROR, _T("Wrong file format domain '%s': code=0x%08X\n"),
					(LPCTSTR) pszObjectsDiffName, hr);
				CloseHandle(hDiffFile);
				delete lpBuff;
				return E_FAIL;
			}

			pO1 = (const XDownHeader*)(lpBuff + pos);
			if(pos + sizeof(XDownHeader) + TRANSLATE_LONG(pO1->ulSize) > dwLen1)
			{
				TrcPrint(TRC_ERROR, _T("Wrong file format domain '%s': code=0x%08X\n"),
					(LPCTSTR) pszObjectsDiffName, hr);
				CloseHandle(hDiffFile);
				delete lpBuff;
				return E_FAIL;
			}

			// download if the object not identic with the previous object
			if(m_map[TRANSLATE_SHORT(pO1->uIndex)] != TRANSLATE_SHORT(pO1->uIndex))
			{
				bOK = ::WriteFile(hDiffFile, lpBuff+pos, sizeof(XDownHeader)+TRANSLATE_LONG(pO1->ulSize), &dwTemp, NULL);
				if (!bOK)
				{
					hr = HRESULT_FROM_WIN32(::GetLastError());
					TrcPrint(TRC_ERROR, _T("Writting domain '%s' via local copy: code=0x%08X\n"),
						(LPCTSTR) strDiffName, hr);
				}
				assert(sizeof(XDownHeader)+TRANSLATE_LONG(pO1->ulSize) == dwTemp);
				dwWritten += dwTemp;
				m_OCConfig.uData++;
			}
			
			pos += sizeof(XDownHeader)+TRANSLATE_LONG(pO1->ulSize);
		}

		// dump direct variables;
		while (pos < dwLen1)
		{
			const XDownDirect* pD1;
		
			if(pos + sizeof(XDownDirect) > dwLen1)
			{
				TrcPrint(TRC_ERROR, _T("Wrong file format domain '%s': code=0x%08X\n"),
					(LPCTSTR) pszObjectsDiffName, hr);
				CloseHandle(hDiffFile);
				delete lpBuff;
				return E_FAIL;
			}

			pD1 = (const XDownDirect*)(lpBuff + pos);	
			if(pos + sizeof(XDownDirect) + (TRANSLATE_SHORT(pD1->uSize)+7)/8 > dwLen1)
			{
				TrcPrint(TRC_ERROR, _T("Wrong file format domain '%s': code=0x%08X\n"),
					(LPCTSTR) pszObjectsDiffName, hr);
				CloseHandle(hDiffFile);
				delete lpBuff;
				return E_FAIL;
			}
			
			// check if the variable should be downloaded
			char	 insert = 0;
			VarDecl* decl;
			pos1	= m_newDirectVars[pD1->bySegment].GetHeadPosition();
			while(pos1)
			{
				decl = &m_newDirectVars[pD1->bySegment].GetAt(pos1);

				if(decl->bitsize == (IEC_UDINT)TRANSLATE_SHORT(pD1->uSize) &&
					decl->bitoffset/8 == (IEC_UDINT)TRANSLATE_LONG(pD1->ulOffset) && 
					((decl->bitoffset%8)?1<<(decl->bitoffset%8 - 1):0) == pD1->byBit)
				{
					m_newDirectVars[pD1->bySegment].RemoveAt(pos1);
					insert = 1;
					break;
				}
				m_newDirectVars[pD1->bySegment].GetNext(pos1);
			}

			if(insert)
			{
				bOK = ::WriteFile(hDiffFile, lpBuff+pos, sizeof(XDownDirect)+(TRANSLATE_SHORT(pD1->uSize)+7)/8, &dwTemp, NULL);
				if (!bOK)
				{
					hr = HRESULT_FROM_WIN32(::GetLastError());
					TrcPrint(TRC_ERROR, _T("Writting domain '%s' via local copy: code=0x%08X\n"),
						(LPCTSTR) pszObjectsDiffName, hr);
				}
				assert(sizeof(XDownDirect)+(TRANSLATE_SHORT(pD1->uSize)+7)/8 == dwTemp);
				dwWritten += dwTemp;
				m_OCConfig.uDirVars++;
			}

			pos += sizeof(XDownDirect)+(TRANSLATE_SHORT(pD1->uSize)+7)/8;
		}
		CloseHandle(hDiffFile);
		delete lpBuff;

		assert(m_newDirectVars[IDX_INPUT].GetCount() == 0);
		assert(m_newDirectVars[IDX_OUTPUT].GetCount() == 0);
		assert(m_newDirectVars[IDX_MEMORY].GetCount() == 0);
		
		CString strType;			

			
		if(CSC_GetFileType(pszObjectsInitName, strType))
		{
			CString filename = GetConfigPath() + pszOnlineDomainName;
			FILE* pFile = _tfopen(filename, "a");
			if (FAILED(hr)) {
				FC_DEBUGPRINT1(_T("CSVPARS> file not found: %s\n"), filename);
				TrcPrint(TRC_ERROR, _T("CSVPARS> file not found: %s\n"), filename);
			}
			else
			{
				_ftprintf(pFile, _T("%s;%s\n"), strType, pszObjectsDiffName);
				fclose(pFile);
			}
		}		
		
		m_processObjectsInit = TRUE;
	}

	m_isInit = FALSE;
	return hr;
}

/* ---------------------------------------------------------------------------- */


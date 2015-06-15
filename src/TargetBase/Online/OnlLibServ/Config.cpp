/* $Header: /4CReleased/V2.20.00/TargetBase/Online/OnlLibServ/Config.cpp 4     16.04.07 18:47 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Config.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/Online/OnlLibServ/Config.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
 *
 * =CURRENT 	 $Date: 16.04.07 18:47 $
 *			 $Revision: 4 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "config.h"
#include "connection.h"
#include "fc_tools\fc_fileIO.h"
#define  FC_LIB_USE_CRT 1
#include "fc_tools\fc_libmem.h"

#include "dbiConv.h"


//***************************************************************************
// LOCAL DEFINES
//***************************************************************************

// hashes structure types by name
typedef CMap<CString, LPCTSTR, CObject*, CObject*> CMapStruct;
typedef CMap<CString, LPCTSTR, CSC_INSTANCEDEF, CSC_INSTANCEDEF&> CMapInstances;

static CCriticalSection s_ConfigLock;
static CMapStruct s_MapStruct;
static CMapInstances s_MapInstances;
static CMapStruct s_MapBreakpoints;

//***************************************************************************
// END LOCAL DEFINES
//***************************************************************************

CString GetConfigPath()
{
    assert(g_StartParam != NULL);
    CString retval;
    // save path to config directory
    LPTSTR szTrcFile = retval.GetBuffer(_MAX_PATH);
    GetModuleFileName(_Module.GetModuleInstance(), szTrcFile, _MAX_PATH);
    LPTSTR  psz = _tcsrchr(szTrcFile, _T('\\'));
    if (psz != NULL) {
        *psz = _T('\0');
    }
    retval.ReleaseBuffer();
    retval += _T("\\config\\_");
    retval += g_StartParam;
    retval += _T("_\\");
    return retval;
}

CCsvParse::CCsvParse()
{
	m_iLineNo = 1;
	m_iLinePos = 0;
	m_pFile = NULL;
}

CCsvParse::CCsvParse(BYTE* lpBuffer, UINT nBufferSize)
{
	m_iLineNo = 1;
	m_iLinePos = 0;
	m_pFile = new CMemFile(lpBuffer, nBufferSize);
}

CCsvParse::~CCsvParse(void)
{
	if(m_pFile)
		delete m_pFile;
}

HRESULT CCsvParse::Open(LPCTSTR pszFileName)
{
	if(m_pFile != NULL)
		return E_FAIL;

	BOOL res;

	//pszFileName always overwrites the current config in m_strFile
	m_pFile = new CFile;
	res = m_pFile->Open(pszFileName, CFile::modeRead);

	if(res)
		return S_OK;
	else
	{
		delete m_pFile;
		m_pFile = NULL;
		return E_FAIL;
	}
}

void CCsvParse::Close(void)
{
	if(m_pFile != NULL)
        m_pFile->Close();

	delete m_pFile;
	m_pFile = NULL;
}

BOOL CCsvParse::IsOpen(void)
{
	return m_pFile != NULL;
}

_TINT CCsvParse::NextChar(void)
/////////////////////////////////////////////////////////////////////////////////////////////
/**
*	<B>NextChar</B>.
*
*	Description = NextChar() returns the next stream character. Updates lineno and
*	linepos.
*	<p>
*	Specification = 
*	<p>
*	@return	- the next character	
*	<p>
*
*	@exception	CFileException when end of file condition occurs
*	<p>
*	@exception	CFileException when I/O error occurs (detemined by cause)
*	<p>
*	@see
*	<p>
*/
{
	TCHAR ch;
	assert(IsOpen());

	
	if(m_pFile->Read(&ch, sizeof(TCHAR)) != sizeof(TCHAR)) {
	  m_iLinePos=0;
	  return _TEOF;
	}

	if(ch == CSV_TAB) {
		m_iLinePos=(m_iLinePos/8 + 1)*8;
	}
	else if (ch == CSV_NL)	{
	  m_iLinePos=0;
	  m_iLineNo++;
	}
	else {
	  m_iLinePos++;
	}
	return ch;
}

// 04.04.06 SIS >>
void CCsvParse::SkipLine()
/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	SkipLine() skips the rest of a line
*     NextChar is called until a line feed is encountered
*
*	@see        NextChar()
*/
{
	assert(IsOpen());

	TCHAR ch = NextChar();
        
	while(ch != CSV_NL && ch != _TEOF) 
	{
		ch = NextChar();
    }
}
// 04.04.06 SIS <<


BOOL CCsvParse::ParseNextField(CString & strField)
/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>ParseNextField</B>.
*
*	Description = ParseNextField() returns the next input field. On success
*	the passed @param strField will be filled with the read field. Escape sequences
*	(double-'"') will be replaced by original char.
*	<p>
*	Specification = 
*	<p>
*	@param	strField - output parameter; will be filled with the CSV-Field read from
*			file.
*	<p>
*
*	@return	TRUE - success; FALSE: error (unexpected end of file, I/O errors).
*	<p>
*
*	@exception No
*	<p>
*	@see
*	<p>
*/
{
    _TINT ch;
	assert(IsOpen());

	strField.Empty();
	ch = NextChar();
    if (ch == _TEOF)
        return FALSE;

	if (ch == CSV_QUOTES) {
	  
	  for(;;) {
		ch = NextChar();
        if (ch == _TEOF)
            break;

		if (ch == CSV_QUOTES) {
			ch = NextChar();
			if (ch != CSV_QUOTES)
				  break;
		}
		
		strField += (_TCHAR)ch;
      }
	}
	else {
	  while (ch != _TEOF && ch != CSV_NL && ch != CSV_CR && ch != CSV_FSEP) {
		strField += (_TCHAR)ch;
		ch = NextChar();
	  }
	}

    if (ch == CSV_CR) {
        ch = NextChar(); // NL?
    }

    if (ch == _TEOF) {
      FC_DEBUGPRINT3(_T("CSVPARS> %s(%d:%d): unexpected EOF\n"), 
		  m_pFile->GetFileName(), m_iLineNo, m_iLinePos);
      TrcPrint(TRC_ERROR, _T("CSVPARS> %s(%d:%d): unexpected EOF\n"), 
		  m_pFile->GetFileName(), m_iLineNo, m_iLinePos);
      return FALSE;
    }
    if (ch != CSV_NL && ch != CSV_FSEP) {
	  FC_DEBUGPRINT3(_T("CSVPARS> %s(%d:%d): illegal csv file format\n"), 
		  m_pFile->GetFileName(), m_iLineNo, m_iLinePos);
      TrcPrint(TRC_ERROR, _T("CSVPARS> %s(%d:%d): illegal csv file format\n"), 
		  m_pFile->GetFileName(), m_iLineNo, m_iLinePos);
	  return FALSE;
	}
    return TRUE;
}


BOOL CCsvParse::IsEndOfLine()
/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>IsEndOfLine</B>.
*
*	Description = IsEndOfLine tests, if the cursor is at the end of a line.
*	<p>
*	Specification = 
*	<p>
*	@return true => EOL detected.
*			false => otherwise
*	<p>
*
*	@exception	No
*	<p>
*	@see
*	<p>
*/
{
	return m_iLinePos == 0;
}


int CCsvParse::GetLineNo()
{
    return m_iLineNo;
}

int CCsvParse::GetLinePos()
{
    return m_iLinePos;
}



CCriticalSection *CSC_GetConfigLock()
{
    return &s_ConfigLock;
}


//------------------------------------------------------------------*
/**
 * CSC_InitHashTables initializes the hash tables.
 *
 * @param      
 * @return     
 * @exception    -
 * @see        
 */
VOID CSC_InitHashTables()
{
    CSingleLock lock(CSC_GetConfigLock(), TRUE);
    CSC_CleanupHashTables();

    CCsvParse parser;
    CString filename = GetConfigPath() + _T("__objects.hash");
    HRESULT hr = parser.Open(filename);
    if (FAILED(hr)) {
        FC_DEBUGPRINT1(_T("CSVPARS> file not found: %s\n"), filename);
        TrcPrint(TRC_ERROR, _T("CSVPARS> file not found: %s\n"), filename);
        return;
    }

    BOOL syntax = TRUE;
    for (;;) {
      CSC_INSTANCEDEF instancedef;
      CString strName, strParam;
      if (!parser.IsEndOfLine()) {
        syntax = FALSE;
        break;
      }
      BOOL ok = parser.ParseNextField(strName);
      if (!ok)
          break;
      strName.MakeLower();
      syntax = parser.ParseNextField(strParam);
      if (!syntax)
          break;
      instancedef.task = _ttol(strParam);
      syntax = parser.ParseNextField(strParam);
      if (!syntax)
          break;
      instancedef.obj = _ttol(strParam);
      syntax = parser.ParseNextField(strParam);
      if (!syntax)
          break;
      instancedef.off = _ttol(strParam);
      syntax = parser.ParseNextField(strParam); // length: ignored...
      if (!syntax)
          break;
      syntax = parser.ParseNextField(strParam);
      if (!syntax)
          break;
      instancedef.r_obj = _ttol(strParam);
      syntax = parser.ParseNextField(strParam);
      if (!syntax)
          break;
      instancedef.r_off = _ttol(strParam);
      syntax = parser.ParseNextField(strParam); // length: ignored...
      if (!syntax)
          break;
      syntax = parser.ParseNextField(strParam);
      if (!syntax)
          break;
      instancedef.cls_id = _ttol(strParam);
      syntax = parser.ParseNextField(instancedef.strClassName);
      if (!syntax)
          break;
      s_MapInstances.SetAt(strName, instancedef);
    }
    
    if (!syntax) {
        FC_DEBUGPRINT3(_T("CSVPARS> syntax error in %s(%d:%d)\n"), filename, parser.GetLineNo(), parser.GetLinePos());
        TrcPrint(TRC_ERROR, _T("CSVPARS> syntax error in %s(%d:%d)\n"), filename, parser.GetLineNo(), parser.GetLinePos());
    }
    return;
}


//------------------------------------------------------------------*
/**
 * CSC_CleanupHashTables clears the hash tables.
 *
 * @param      
 * @return     
 * @exception    -
 * @see        
 */
VOID CSC_CleanupHashTables()
{
    CSingleLock lock(CSC_GetConfigLock(), TRUE);
    POSITION position = s_MapStruct.GetStartPosition();
    while(position!=NULL)
    {
        CString key;
        CObject *value;
        s_MapStruct.GetNextAssoc(position, key, value);
        delete value;
    }
    s_MapStruct.RemoveAll();
    s_MapInstances.RemoveAll();
    position = s_MapBreakpoints.GetStartPosition();
    while(position!=NULL)
    {
        CString key;
        CObject *value;
        s_MapBreakpoints.GetNextAssoc(position, key, value);
        delete value;
    }
    s_MapBreakpoints.RemoveAll();
}

//------------------------------------------------------------------*
/**
 * CSC_LookupStructDef
 *
 * @param      name
 * @return     CMapVarDef*
 * @exception    -
 * @see        
 */
CMapVarDef *CSC_LookupStructDef(LPCTSTR name)
{
	CObject *val;
	if (s_MapStruct.Lookup(name, val)) 
	{
		return (CMapVarDef*)val;
	}
	
	CMapVarDef *pStructDef = new CMapVarDef;
	s_MapStruct.SetAt(name, pStructDef);
	
	CCsvParse parser;
	CString filename = GetConfigPath() + name + DEF_FILE_EXT;
	HRESULT hr = parser.Open(filename);
	if (FAILED(hr)) 
	{
		FC_DEBUGPRINT1(_T("CSVPARS> file not found: %s\n"), filename);
		TrcPrint(TRC_ERROR, _T("CSVPARS> file not found: %s\n"), filename);
		return pStructDef;
	}
	
	CString strName, strDeclSpec, strType, strSegment;
	CString strOffset, strBitOffset, strBitSize, strStrLen;
	
	BOOL syntax = TRUE;
	for (;;) 
	{
		CSC_VARDEF vardef;
		
		if (!parser.IsEndOfLine()) 
		{
			syntax = FALSE;
			break;
		}
		
		/* CSV_IDX_NAME: Instance name
		 */
		BOOL ok = parser.ParseNextField(strName);
		if (!ok)
			break;
		strName.MakeLower();
		
		/* CSV_IDX_DECL_SPEC: Declaration Specification
		 */
		syntax = parser.ParseNextField(strDeclSpec);
		if (!syntax)
			break;
		strDeclSpec.MakeUpper();
		
		/* CSV_IDX_SEGMENT: Segment Information
		 */
		syntax = parser.ParseNextField(strSegment);
		if (!syntax)
			break;
		
		/* CSV_IDX_TYPE: Data Type
		 */
		syntax = parser.ParseNextField(strType);
		if (!syntax)
			break;
		
		/* CSV_IDX_BYTE_OFFSET: Byte Offset
		 */
		syntax = parser.ParseNextField(strOffset);
		if (!syntax)
			break;
		
		/* CSV_IDX_BIT_OFFSET: Bit Offset
		 */
		syntax = parser.ParseNextField(strBitOffset);
		if (!syntax)
			break;
		
		/* CSV_IDX_BIT_SIZE: Bit Size
		 */
		syntax = parser.ParseNextField(strBitSize);
		if (!syntax)
			break;
		
		/* CSV_IDX_STRLEN: String Length
		 */
		syntax = parser.ParseNextField(strStrLen);
		if (!syntax)
			break;
		
		if (strDeclSpec[0] == _T('S')) 
		{
			strType.MakeLower();
			
			vardef.vardefType	= CSC_VARDEF_STRUCT;
			vardef.iecType		= BL_ANY;
			vardef.segment		= 0;
			vardef.offset		= 0;
			vardef.bitnr		= 0;
			vardef.strlen		= 0;
			vardef.structType	= strType;
			
		} 
		else if (strDeclSpec[0] == _T('G')) 
		{
			vardef.vardefType	= CSC_VARDEF_GLOBAL;
			
			strType.MakeUpper();
			if (strType == _T("WSTRING"))
			{
				strType = _T("STRING");
			}	
			
			DWORD dummy;
			hr = BL_TypenameToDatatype(CComBSTR(strType), &vardef.iecType, &dummy);
			if (FAILED(hr)) 
			{
				syntax = FALSE;
				break;
			}
			
			vardef.segment	= _ttol(strSegment);
			vardef.offset	= _ttol(strOffset);
			vardef.bitnr	= 0;
			vardef.strlen	= 0;
			
			if (vardef.iecType == BL_BOOL) 
			{
				vardef.bitnr = _ttoi(strBitOffset);
			}
			if (vardef.iecType == BL_STRING) 
			{
				vardef.strlen = _ttoi(strStrLen);
			}
		}
		else if (strDeclSpec[0] == _T('N') || strDeclSpec[0] == _T('R')) 
		{
			vardef.vardefType = strDeclSpec[0] == _T('N') ? CSC_VARDEF_NORMAL : CSC_VARDEF_RETAIN;

			strType.MakeUpper();
			if (strType == _T("WSTRING"))
			{
				strType = _T("STRING");
			}	

			DWORD dummy;
			hr = BL_TypenameToDatatype(CComBSTR(strType), &vardef.iecType, &dummy);
			if (FAILED(hr)) 
			{
				syntax = FALSE;
				break;
			}

			vardef.segment	= 0;
			vardef.offset	= _ttol(strOffset);
			vardef.bitnr	= 0;
			vardef.strlen	= 0;

			if (vardef.iecType == BL_BOOL) 
			{
				vardef.bitnr = _ttoi(strBitOffset);
			}
			if (vardef.iecType == BL_STRING) 
			{
				vardef.strlen = _ttoi(strStrLen);
			}
		}
		else 
		{
			syntax = FALSE;
			break;
		}
		
		pStructDef->SetAt(strName, vardef);
		
	} /* for (;;) */
	
	if (!syntax) {
		FC_DEBUGPRINT3(_T("CSVPARS> syntax error in %s(%d:%d)\n"), filename, parser.GetLineNo(), parser.GetLinePos());
		TrcPrint(TRC_ERROR, _T("CSVPARS> syntax error in %s(%d:%d)\n"), filename, parser.GetLineNo(), parser.GetLinePos());
	}
	return pStructDef;
}

CMapVarDef *CSC_LookupStructDefByInstance(LPCTSTR instance)
{
   CMapVarDef *pStructDef = CSC_LookupStructDef(_T("__globals"));
   CString strInstance = instance;
   for (LPCTSTR tok = _tcstok(strInstance.GetBuffer(0), _T("."));
        tok!=NULL;
        tok = _tcstok(NULL, _T(".")))
   {
      CSC_VARDEF vardef;
      if (pStructDef->Lookup(tok, vardef)
          && vardef.vardefType == CSC_VARDEF_STRUCT)
      {
          pStructDef = CSC_LookupStructDef(vardef.structType);
      }
      else
      {
          return NULL;
      }
   }
   return pStructDef;
}


BOOL CSC_LookupInstanceDef(LPCTSTR instance, CSC_INSTANCEDEF &retval)
{
   return s_MapInstances.Lookup(instance, retval);
}


BOOL CSC_LookupVariableDef(LPCTSTR path, CSC_VARADDR &retval)
{
    CSingleLock lock(CSC_GetConfigLock(), TRUE);
    CString strPath = path;
    int dot = strPath.ReverseFind(_T('.'));
    CString strInstance = strPath.Left(dot);
    CString strName = strPath.Mid(dot+1);
    CMapVarDef *pVarDef = CSC_LookupStructDefByInstance(strInstance);
    if (!pVarDef)
        return FALSE;

    CSC_VARDEF vardef;
    if (!pVarDef->Lookup(strName, vardef))
        return FALSE;
    if (vardef.vardefType == CSC_VARDEF_STRUCT)
        return FALSE;
    if (vardef.vardefType == CSC_VARDEF_GLOBAL)
        {
          retval.task    = 0;
          retval.iecType = vardef.iecType;
          retval.segment = vardef.segment;
          retval.offset  = vardef.offset;
          retval.bitnr   = vardef.bitnr;
          retval.strlen  = vardef.strlen;
          return TRUE;
        }

    CSC_INSTANCEDEF instdef;
    if (!CSC_LookupInstanceDef(strInstance, instdef))
        return FALSE;

    retval.iecType = vardef.iecType;
    retval.bitnr   = vardef.bitnr;
    retval.strlen  = vardef.strlen;
    retval.task    = instdef.task;
    if (vardef.vardefType == CSC_VARDEF_NORMAL)
        {
          retval.segment = instdef.obj;
          retval.offset  = instdef.off + vardef.offset;
        }
    else
        {
          retval.segment = instdef.r_obj;
          retval.offset  = instdef.r_off + vardef.offset;
        }
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * CSC_ParseDomainList
 *
 * @return     CDomainList*
 * @exception    -
 * @see        
 */
CDomainList *CSC_ParseDomainList(BOOL fullDownload)
{
    CDomainList *retval = new CDomainList;

    CCsvParse parser;
    CString filename = GetConfigPath() + (fullDownload?pszDomainFileName:pszOnlineDomainName);
    HRESULT hr = parser.Open(filename);
    if (FAILED(hr)) {
        FC_DEBUGPRINT1(_T("CSVPARS> file not found: %s\n"), filename);
        TrcPrint(TRC_ERROR, _T("CSVPARS> file not found: %s\n"), filename);
        return retval;
    }

    BOOL syntax = TRUE;
    for (;;) {
      CSC_DOMAINENTRY domain;
      CString strType, strFile;
      if (!parser.IsEndOfLine()) {
        syntax = FALSE;
        break;
      }
      BOOL ok = parser.ParseNextField(strType);
      if (!ok)
          break;
      strType.MakeLower();
      if (strType == _T("code"))
          domain.type = CSC_DOMAINTYPE_CODE;
      else if (strType == _T("init"))
          domain.type = CSC_DOMAINTYPE_INITIAL;
      else if (strType == _T("config"))
          domain.type = CSC_DOMAINTYPE_CONFIG;
      else if (strType == _T("custom"))
          domain.type = CSC_DOMAINTYPE_CUSTOM;
      else if (strType == _T("debug"))
          domain.type = CSC_DOMAINTYPE_DEBUG;
      else
          domain.type = CSC_DOMAINTYPE_UNKNOWN;
      syntax = parser.ParseNextField(strFile);
      if (!syntax)
          break;
      domain.file = GetConfigPath() + strFile;
      retval->AddTail(domain);
      while (!parser.IsEndOfLine() && parser.ParseNextField(strFile))
          ;
    }
    if (!syntax) {
        FC_DEBUGPRINT3(_T("CSVPARS> syntax error in %s(%d:%d)\n"), filename, parser.GetLineNo(), parser.GetLinePos());
        TrcPrint(TRC_ERROR, _T("CSVPARS> syntax error in %s(%d:%d)\n"), filename, parser.GetLineNo(), parser.GetLinePos());
    }
    return retval;
}


//------------------------------------------------------------------*
/**
 * CSC_LookupTask
 *
 * @param      name        task name
 * @param      taskid      [OUT] the task id
 * @return     TRUE = success 
 * @exception    -
 * @see        
 */

BOOL CSC_LookupTask(LPCTSTR name, long &taskid)
{
    CCsvParse parser;
    CString filename = GetConfigPath() + pszTasksFileName;
    HRESULT hr = parser.Open(filename);
    if (FAILED(hr)) {
        FC_DEBUGPRINT1(_T("CSVPARS> file not found: %s\n"), filename);
        TrcPrint(TRC_ERROR, _T("CSVPARS> file not found: %s\n"), filename);
        return FALSE;
    }

    BOOL syntax = TRUE;
    for (;;) {
      CString task, str_taskid, loaded;
	  
	  if (CConnection::GetFirmware() >= 20900 && CConnection::GetFirmware() < 21000)
	  {
		  // 04.04.06 SIS: do not check for end of line here (only for OSAI V2.09)
	  }
	  else
	  {
		  if (!parser.IsEndOfLine()) {
			syntax = FALSE;
			break;
		  }
	  }
      
	  BOOL ok = parser.ParseNextField(task);
      if (!ok)
          break;
      task.MakeLower();
      syntax = parser.ParseNextField(str_taskid);
      if (!syntax)
          break;
	  
	  if (CConnection::GetFirmware() >= 20700)
	  {
		  // Skip loaded/unloaded flag
		  ok = parser.ParseNextField(loaded);
		  if (!ok)
			  break;
	  }

      if (task == name){
        taskid = _ttol(str_taskid);
        return TRUE;
      }

	  if (CConnection::GetFirmware() >= 20900 && CConnection::GetFirmware() < 21000)
	  {
		  parser.SkipLine();	// 04.04.06 SIS: skip rest of line (only for OSAI V2.09)
	  }
    }
    if (!syntax) {
        FC_DEBUGPRINT3(_T("CSVPARS> syntax error in %s(%d:%d)\n"), filename, parser.GetLineNo(), parser.GetLinePos());
        TrcPrint(TRC_ERROR, _T("CSVPARS> syntax error in %s(%d:%d)\n"), filename, parser.GetLineNo(), parser.GetLinePos());
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * CSC_ReverseLookupTask
 *
 * @param      taskid      the task id
 * @param      name        [OUT] task name
 * @return     TRUE = success 
 * @exception    -
 * @see        
 */

BOOL CSC_ReverseLookupTask(long taskid, CString &name)
{
    CCsvParse parser;
    CString filename = GetConfigPath() + pszTasksFileName;
    HRESULT hr = parser.Open(filename);
    if (FAILED(hr)) {
        FC_DEBUGPRINT1(_T("CSVPARS> file not found: %s\n"), filename);
        TrcPrint(TRC_ERROR, _T("CSVPARS> file not found: %s\n"), filename);
        return FALSE;
    }

    BOOL syntax = TRUE;
    for (;;) {
      CString task, str_taskid,loaded;

	  if (CConnection::GetFirmware() >= 20900 && CConnection::GetFirmware() < 21000)
	  {
		  // 04.04.06 SIS: do not check for end of line here (only for OSAI V2.09)
	  }
	  else
	  {
		  if (!parser.IsEndOfLine()) {
			syntax = FALSE;
			break;
		  }
	  }
	  
      BOOL ok = parser.ParseNextField(task);
      if (!ok)
          break;
      task.MakeLower();
      syntax = parser.ParseNextField(str_taskid);
      if (!syntax)
          break;

	  if (CConnection::GetFirmware() >= 20700)
	  {
		  // Skip loaded/unloaded flag
		  ok = parser.ParseNextField(loaded);
		  if (!ok)
			  break;
	  }

      if (taskid == _ttol(str_taskid)){
        name = task;
        return TRUE;
      }

	  if (CConnection::GetFirmware() >= 20900 && CConnection::GetFirmware() < 21000)
	  {
		parser.SkipLine();	// 04.04.06 SIS: skip rest of line
	  }
    }
    if (!syntax) {
        FC_DEBUGPRINT3(_T("CSVPARS> syntax error in %s(%d:%d)\n"), filename, parser.GetLineNo(), parser.GetLinePos());
        TrcPrint(TRC_ERROR, _T("CSVPARS> syntax error in %s(%d:%d)\n"), filename, parser.GetLineNo(), parser.GetLinePos());
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * CSC_LookupBreakpointList
 *
 * @return     CBreakpointList*
 * @exception    -
 * @see        
 */
CBreakpointList *CSC_LookupBreakpointList(LPCTSTR clsName)
{
    CObject *val;
    if (s_MapBreakpoints.Lookup(clsName, val)) {
        return (CBreakpointList*)val;
    }

    CBreakpointList *retval = new CBreakpointList;
    s_MapBreakpoints.SetAt(clsName, retval);

    CCsvParse parser;
    CString filename = GetConfigPath() + clsName + _T(".dbp");
    HRESULT hr = parser.Open(filename);
    if (FAILED(hr)) {
        FC_DEBUGPRINT1(_T("CSVPARS> file not found: %s\n"), filename);
        TrcPrint(TRC_ERROR, _T("CSVPARS> file not found: %s\n"), filename);
        return retval;
    }

    BOOL syntax = TRUE;
    for (;;) {
      CSC_BREAKPOINT line;
      CString str;
      if (!parser.IsEndOfLine()) {
        syntax = FALSE;
        break;
      }
      BOOL ok = parser.ParseNextField(str);
      if (!ok)
          break;
      line.bpnr = _ttol(str);
      syntax = parser.ParseNextField(str);
      if (!syntax)
          break;
      line.offset = _ttol(str);
      syntax = parser.ParseNextField(str); // 'I' | 'N'
      if (!syntax)
          break;

      if (str.CompareNoCase(_T("I")) && str.CompareNoCase(_T("N")) )
          continue;

      if (str.CompareNoCase(_T("N")) == 0)
          line.type = 1; // NATIVE CODE
      else
          line.type = 0; // INTERPRETER

      retval->AddTail(line);
    }
    if (!syntax) {
        FC_DEBUGPRINT3(_T("CSVPARS> syntax error in %s(%d:%d)\n"), filename, parser.GetLineNo(), parser.GetLinePos());
        TrcPrint(TRC_ERROR, _T("CSVPARS> syntax error in %s(%d:%d)\n"), filename, parser.GetLineNo(), parser.GetLinePos());
    }
    return retval;

}

BOOL CSC_LookupBreakpoint(LPCTSTR path, long bpnr, CSC_BREAKPOINT_INFO &info)
{
    CSingleLock lock(CSC_GetConfigLock(), TRUE);
    CSC_INSTANCEDEF instdef;
    if (!CSC_LookupInstanceDef(path, instdef))
        return FALSE;
    if (instdef.strClassName.IsEmpty())
        return FALSE;
    info.cls_id = instdef.cls_id;
    info.obj = instdef.obj;
    info.task = instdef.task;
    CBreakpointList *pList = CSC_LookupBreakpointList(instdef.strClassName);
    if (pList == NULL)
        return FALSE;
    POSITION pos = pList->GetHeadPosition();
    while (pos != NULL)
      {
          CSC_BREAKPOINT elem = pList->GetNext(pos);
          if (elem.bpnr == bpnr)
            {
              info.offset = elem.offset;
              info.type = elem.type;
              return TRUE;
            }
      }
    return FALSE;
}


BOOL CSC_ReverseLookupBreakpoint(const CSC_BREAKPOINT_INFO &info, CString &path, long &bpnr)
{
    CSingleLock lock(CSC_GetConfigLock(), TRUE);
    POSITION pos1 = s_MapInstances.GetStartPosition();
    while (pos1 != NULL) {
        CSC_INSTANCEDEF instdef;
        CString instPath;
        s_MapInstances.GetNextAssoc(pos1, instPath, instdef);
        if (info.obj==instdef.obj &&
            info.cls_id==instdef.cls_id) {

            CBreakpointList *pList = CSC_LookupBreakpointList(instdef.strClassName);
            if (pList == NULL)
                return FALSE;
            path = instPath;
            POSITION pos = pList->GetHeadPosition();
            while (pos != NULL)
              {
                  CSC_BREAKPOINT elem = pList->GetNext(pos);
                  if (elem.offset >= info.offset)
                    {
                      bpnr = elem.bpnr;
                      return TRUE;
                    }
              }
            return FALSE;
        }
    }
    return FALSE;
}

BOOL CSC_LookupNextBreakpoint(const CSC_BREAKPOINT_INFO &info, CSC_BREAKPOINT_INFO &prev, CSC_BREAKPOINT_INFO &next)
{
    CSingleLock lock(CSC_GetConfigLock(), TRUE);
    POSITION pos1 = s_MapInstances.GetStartPosition();
    while (pos1 != NULL) {
        CSC_INSTANCEDEF instdef;
        CString instPath;
        s_MapInstances.GetNextAssoc(pos1, instPath, instdef);
        if (info.obj==instdef.obj &&
            info.cls_id==instdef.cls_id) {

            CBreakpointList *pList = CSC_LookupBreakpointList(instdef.strClassName);
            if (pList == NULL)
                return FALSE;
            prev.cls_id = instdef.cls_id;
            prev.obj    = instdef.obj;
            prev.offset = info.offset;
            prev.task   = info.task;
            prev.type   = info.type;
            POSITION pos = pList->GetHeadPosition();
            while (pos != NULL)
              {
                  CSC_BREAKPOINT elem = pList->GetNext(pos);
                  if (elem.offset > info.offset)
                    {
                      next.cls_id = instdef.cls_id;
                      next.obj    = instdef.obj;
                      next.offset = elem.offset;
                      next.task   = info.task;
                      next.type   = elem.type;
                      return TRUE;
                    }
                  prev.offset = elem.offset;
                  prev.type   = elem.type;
              }
            return FALSE;
        }
    }
    return FALSE;
}

IEC_UINT CSC_DumpFile(HANDLE hResult, LPCTSTR szFile, LPCTSTR szPath)
{
	CString sFile = szPath;
	sFile += szFile;

	HANDLE hFile = CreateFile(sFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return ERR_ERROR;
	}

	DWORD dwSize = GetFileSize(hFile, NULL);
	if (dwSize == INVALID_FILE_SIZE)
	{
		return ERR_ERROR;
	}

	BYTE *pBuff = new BYTE [dwSize];
	DWORD dwRead = 0;

	if (! ReadFile(hFile, pBuff, dwSize, &dwRead, NULL))
	{
		delete pBuff;
		CloseHandle(hFile);
		return ERR_ERROR;
	}

	CloseHandle(hFile);

	if (dwRead != dwSize)
	{
		delete pBuff;
		return ERR_ERROR;
	}

	BYTE bySum1;
	BYTE bySum2;

	// calculate check sums
	for (unsigned i = 0; i < dwSize; i++)
	{
		bySum1 += pBuff[i];
		bySum2 += bySum1;
	}

	XFileDef xFileDef;

	xFileDef.ulDataLen	= TRANSLATE_LONG(dwSize);
	xFileDef.uNameLen	= TRANSLATE_SHORT(strlen(szFile));
	xFileDef.usCRC		= bySum1;
	xFileDef.usHash		= bySum2;

	DWORD dwWritten = 0;
	if (! WriteFile(hResult, &xFileDef, sizeof(XFileDef), &dwWritten, NULL))
	{
		delete pBuff;
		return ERR_ERROR;
	}
	if (dwWritten != sizeof(XFileDef))
	{
		delete pBuff;
		return ERR_ERROR;
	}

	dwWritten = 0;
	if (! WriteFile(hResult, szFile, strlen(szFile), &dwWritten, NULL))
	{
		delete pBuff;
		return ERR_ERROR;
	}
	if (dwWritten != strlen(szFile))
	{
		delete pBuff;
		return ERR_ERROR;
	}

	dwWritten = 0;
	if (! WriteFile(hResult, pBuff, dwSize, &dwWritten, NULL))
	{
		delete pBuff;
		return ERR_ERROR;
	}
	if (dwWritten != dwSize)
	{
		delete pBuff;
		return ERR_ERROR;
	}

	delete pBuff;

	return OK;
}

CString CSC_CreateDBIDownloadDomain_21020(BOOL bConvertDBI)
{
	IEC_UINT uRes = OK;
	CString  sDownloadFile;

	/* Convert DBI Data
	 */
	if (bConvertDBI == TRUE)
	{
		CConvert *pConv = new CConvert;

		uRes = pConv->ImportProject(GetConfigPath(), pszObjectsHashName);
		if (uRes != OK)
		{
			sDownloadFile.Empty();
			return sDownloadFile;
		}

		uRes = pConv->Serialize(GetConfigPath());
		if (uRes != OK)
		{
			sDownloadFile.Empty();
			return sDownloadFile;
		}

		delete pConv;
		pConv = NULL;
	}

	/* Create download file
	 */
    sDownloadFile = GetConfigPath() + _T("__dbiDownload.bin");

	HANDLE hResult = CreateFile(sDownloadFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hResult == INVALID_HANDLE_VALUE)
	{
		sDownloadFile.Empty();
		return sDownloadFile;
	}

	if (CSC_DumpFile(hResult, DBI_FILE_INSTANCE, GetConfigPath()) != OK)
	{
		CloseHandle(hResult);

		sDownloadFile.Empty();
		return sDownloadFile;
	}
	if (CSC_DumpFile(hResult, DBI_FILE_TYPE, GetConfigPath()) != OK)
	{
		CloseHandle(hResult);

		sDownloadFile.Empty();
		return sDownloadFile;
	}
	if (CSC_DumpFile(hResult, DBI_FILE_VAR, GetConfigPath()) != OK)
	{
		CloseHandle(hResult);

		sDownloadFile.Empty();
		return sDownloadFile;
	}

	CloseHandle(hResult);

	return sDownloadFile;
}

CString CSC_CreateDBIDownloadDomain(BOOL bConvertDBI)
{
	if (CConnection::GetFirmware() >= 21020)
	{
		return CSC_CreateDBIDownloadDomain_21020(bConvertDBI);
	}

    CDomainList *pDomainList = CSC_ParseDomainList();
    CString strDownloadFile = GetConfigPath() + _T("__dbiDownload.bin");

    FILE* pFile = fopen(strDownloadFile, "wb");

    POSITION pos = pDomainList->GetHeadPosition();

	ULONG ulOffset = 0;
    
    while(pos)
    {
        CSC_DOMAINENTRY domain = pDomainList->GetNext(pos);
        if (domain.type == CSC_DOMAINTYPE_DEBUG)
        {
            char*           pMem;
            unsigned char   bySum1 = 0;
            unsigned char   bySum2 = 0;
            unsigned long   lFSize = 0xffffffff;
            unsigned long   lFileSize = 0xffffffff;

            CString strFileName;
            CString strFullFileName = domain.file;
            strFileName = strFullFileName;
            int ipos;
            ipos = strFullFileName.ReverseFind(_T('\\'));
            if (ipos>0)
                strFileName = strFullFileName.Mid(ipos+1);

            int res = FC_LoadDiskBinFileToMem(strFullFileName, (void**) &pMem, &lFSize, NULL);
            if (res != 0)
            { 
                continue;
            }

            // calculate check sums
            for (unsigned long i=0; i<lFSize; i++)
            {
                bySum1 += pMem[i];
                bySum2 += bySum1;
            }

            // LITTLE / BIG ENDIAN
            lFileSize = lFSize;
            lFSize = TRANSLATE_LONG(lFSize);   // transform if necessary

			if (CConnection::GetFirmware() < 20700)
			{
				fwrite(strFileName, sizeof(char), _tcslen(strFileName) + 1, pFile);
				fwrite(&lFSize, sizeof(char), sizeof(long), pFile);
            
				// Checksums only for versions prior to 2.07!
				fwrite(&bySum1, sizeof(char), sizeof(char), pFile);
				fwrite(&bySum2, sizeof(char), sizeof(char), pFile);
			}
            else if (CConnection::GetFirmware() < 21000)
			{
				XDBIFile_208 xFile;

                unsigned long   ulOffsetTrans = TRANSLATE_LONG(ulOffset);

				xFile.ulLen		= lFSize;
				xFile.ulOffset	= ulOffsetTrans;

				ulOffset += lFileSize;

				strncpy((char *)xFile.szName, strFileName, VMM_MAX_DBI_FILE);
				xFile.szName[VMM_MAX_DBI_FILE] = _T('\0');

				fwrite(&xFile, sizeof(char), sizeof(XDBIFile_208), pFile);
			}
			else
			{
				XFileDef xFileDef;

				xFileDef.ulDataLen	= lFSize;
				xFileDef.uNameLen	= TRANSLATE_SHORT(strlen(strFileName));
				xFileDef.usCRC		= bySum1;
				xFileDef.usHash		= bySum2;

				fwrite(&xFileDef, sizeof(char), sizeof(XFileDef), pFile);
				fwrite(strFileName, sizeof(char), strlen(strFileName), pFile);
			}

			fwrite(pMem, sizeof(char), lFileSize, pFile);

            FC_LibFreeMemory(pMem);
        }
    }

    fclose(pFile);

    return strDownloadFile;
}



BOOL CSC_GetFileType(const CString& strFileName, CString& strFileType)
{
    CCsvParse parser;
	int found = 0;
	CString strFile;
	CString filename = GetConfigPath() + pszDomainFileName;

	HRESULT hr = parser.Open(filename);
	if (FAILED(hr)) {
		FC_DEBUGPRINT1(_T("CSVPARS> file not found: %s\n"), filename);
		TrcPrint(TRC_ERROR, _T("CSVPARS> file not found: %s\n"), filename);
	}
	else {
		while(1)
		{
			BOOL ok = parser.ParseNextField(strFileType);
			if (!ok)
				break;
			ok = parser.ParseNextField(strFile);
			if (!ok)
				break;
			if(strFileName.CompareNoCase(strFile) == 0)
			{
				found = 1;
				break;
			}
		}
		parser.Close();
	}

	if(!found) 
		strFileType = _T("unknown");

	return found;
}

/* ---------------------------------------------------------------------------- */

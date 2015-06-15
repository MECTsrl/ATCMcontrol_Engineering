/* $Header: /4CReleased/V2.20.00/TargetBase/Online/inc/Config.h 2     13.04.07 18:48 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Config.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/Online/inc/Config.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
 *
 * =CURRENT 	 $Date: 13.04.07 18:48 $
 *			 $Revision: 2 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef __CONFIG_H__INC
#define __CONFIG_H__INC

enum CSC_VARDEF_TYPE{
   CSC_VARDEF_NORMAL,
   CSC_VARDEF_RETAIN,
   CSC_VARDEF_GLOBAL,
   CSC_VARDEF_STRUCT,
};

struct CSC_VARDEF{
   CSC_VARDEF_TYPE vardefType;
   BL_IEC_TYP iecType;
   long segment;
   long offset;
   int  bitnr; // if iecType == BL_BOOL
   int  strlen; // if iecType == BL_STRING
   CString structType;
};


// __objects.hash
struct CSC_INSTANCEDEF{
    long task;
    long obj;
    long off;
    long r_obj;
    long r_off;
    long cls_id;
    CString strClassName;
};


// communication address of a variable
struct CSC_VARADDR{
    BL_IEC_TYP iecType;
    long task;
    long segment;
    long offset;
    int bitnr;
    int strlen;
};

// holds a single structure type
typedef CMap<CString, LPCTSTR, CSC_VARDEF, CSC_VARDEF&> CMapVarDef;

CString GetConfigPath();

CCriticalSection *CSC_GetConfigLock();

VOID CSC_InitHashTables();
VOID CSC_CleanupHashTables();

CMapVarDef *CSC_LookupStructDef(LPCTSTR name);

CMapVarDef *CSC_LookupStructDefByInstance(LPCTSTR instance);

BOOL CSC_LookupVariableDef(LPCTSTR path, CSC_VARADDR &retval);

enum CSC_DOMAINTYPE{
    CSC_DOMAINTYPE_CODE,
    CSC_DOMAINTYPE_INITIAL,
    CSC_DOMAINTYPE_CONFIG,
    CSC_DOMAINTYPE_CUSTOM,
    CSC_DOMAINTYPE_DEBUG,
    CSC_DOMAINTYPE_UNKNOWN,
};
struct CSC_DOMAINENTRY{
    CSC_DOMAINTYPE type;
    CString        file;
};

typedef CList<CSC_DOMAINENTRY, CSC_DOMAINENTRY&> CDomainList;

CDomainList *CSC_ParseDomainList(BOOL fullDownload = TRUE);

BOOL CSC_LookupTask(LPCTSTR name, long &taskid);
BOOL CSC_ReverseLookupTask(long taskid, CString &name);

struct CSC_BREAKPOINT{
    long bpnr;
    long offset;
    long type;
};

typedef CList<CSC_BREAKPOINT, CSC_BREAKPOINT&> CBreakpointList;

struct CSC_BREAKPOINT_INFO{
    long task;
    long obj;
    long cls_id;
    long offset;
    long type;
};

BOOL CSC_LookupBreakpoint(LPCTSTR path, long bpnr, CSC_BREAKPOINT_INFO &info);

BOOL CSC_ReverseLookupBreakpoint(const CSC_BREAKPOINT_INFO &info, CString &path, long &bpnr);

BOOL CSC_LookupNextBreakpoint(const CSC_BREAKPOINT_INFO &info, CSC_BREAKPOINT_INFO &prev, CSC_BREAKPOINT_INFO &next);

CString CSC_CreateDBIDownloadDomain(BOOL bConvertDBI);
CString CSC_CreateDBIDownloadDomain_21020(BOOL bConvertDBI);

const TCHAR pszObjectsHashName[]  = _T("__objects.hash");
const TCHAR pszClassesBinName[]   = _T("__classes.bin");
const TCHAR pszClassesDiffName[]  = _T("__classes.diff");
const TCHAR pszObjectsInitName[]  = _T("__objects.init");
const TCHAR pszObjectsDiffName[]  = _T("__objects.diff");
const TCHAR pszDomainFileName[]   = _T("__domain.list");
const TCHAR pszOnlineDomainName[] = _T("__online.list");
const TCHAR pszTasksFileName[]    = _T("__task.list");
const TCHAR pszCommonHdrName[]    = _T("__common.hdr");

#define DEF_FILE_EXT				_T(".def2")
#define GLOBALS_DEF_NAME			_T("__globals")DEF_FILE_EXT

enum SEG_IDX
{
	IDX_INPUT   = 0, // for %I variables
    IDX_OUTPUT  = 1, // for %Q variables
    IDX_MEMORY  = 2, // for %M variables
    IDX_GLOBAL  = 6, // for global variables
    IDX_RETAIN  = 7, // for retain variables
};

BOOL CSC_GetFileType(const CString& strFileName, CString& strFileType);


#define CSV_TAB _T('\t')
#define CSV_NL _T('\n')
#define CSV_CR _T('\r')
#define CSV_FSEP _T(';')
#define CSV_QUOTES _T('"')

class CCsvParse
{
	int m_iLineNo;
	int m_iLinePos;

	CFile *m_pFile;
public:
	CCsvParse();
	CCsvParse(BYTE* lpBuffer, UINT nBufferSize);
	virtual ~CCsvParse(void);
	
	HRESULT Open(LPCTSTR pszFileName);
	void Close(void);
	BOOL IsOpen(void);

    _TINT NextChar(void);
	void SkipLine();	// 04.04.06 SIS
	BOOL ParseNextField(CString & strField);
	BOOL IsEndOfLine();
    int GetLineNo();
    int GetLinePos();
};

#endif

/* ---------------------------------------------------------------------------- */

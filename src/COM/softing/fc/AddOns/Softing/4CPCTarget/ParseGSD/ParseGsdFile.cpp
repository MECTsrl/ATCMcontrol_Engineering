//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "pgsd.h"
#include "ParseGsdFile.h"
#include "gsd_Scan.h"
#include "gsd_Parser.h"
#include "gsdmessage.h"
//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParseGsdFile


//------------------------------------------------------------------*
CParseGsdFile::CParseGsdFile()
  : m_bTraceParsing(FALSE)
{
    // get path to TMP directory:
    m_sTraceFilePath = GetTempDir();
}

//------------------------------------------------------------------*
CParseGsdFile::~CParseGsdFile()
{
}

//------------------------------------------------------------------*
HRESULT CParseGsdFile::FinalConstruct()
{
    return S_OK;
}

//------------------------------------------------------------------*
void CParseGsdFile::FinalRelease()
{
}

//------------------------------------------------------------------*
HRESULT CParseGsdFile::OnDraw(ATL_DRAWINFO& di)
{
	RECT& rc = *(RECT*)di.prcBounds;
	Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);
	DrawText(di.hdcDraw, _T(""), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CParseGsdFile::ParseFile(BSTR sFileGSD, LPUNKNOWN *pGsdObj)
{
    HRESULT hr = S_OK;
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    try {
        CString sGSD,strFullPathGSD,sLog;
        OFSTRUCT of;
        HFILE hf;
        LPCTSTR pszLog = NULL;
        int ret;

        FC_PCHECK_THROW(pGsdObj,E_POINTER);
        *pGsdObj = NULL;

        sGSD = sFileGSD;
        hf = ::OpenFile(sGSD, &of, OF_EXIST);
        strFullPathGSD = of.szPathName;
        // now build name for scanner log file:

        if (m_bTraceParsing)
        {
            LPCTSTR pszTmp = m_sTraceFilePath;
            sLog = m_sTraceFilePath;
            if (pszTmp[_tcslen(pszTmp)] != '\\') {
                sLog += _T("\\");
            }
            sLog += GSD_PARSE_LOGFILE;
            sLog += GSD_PARSE_LOGFILE_EXT;
            pszLog = sLog;
        }
        else {
            pszLog = NULL;
        }
    
        CGsdScan scanner(strFullPathGSD, pszLog);
        if (!scanner.IsScannerReady()) { 
            //means in the essence file not found or unable to open
            throw (HRESULT) HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
        }

        // start parsing it
        gsd_Parser  parser(1000);
        parser.yydebug = 0;
        
        ret = parser.yyparse(&scanner);
        
        parser.PostProcess();
	    hr = parser.GetDP_Station(pGsdObj);

        /* TODO Check behaviour with product mgmt.
        switch(ret) {
        case -2: //version not supported
            parser.GetDP_Station(pGsdObj);
            FC_RELEASE_PTR(*pGsdObj);
            throw E_GSD_REVISION;
        default:
            //try to continue with all others
            break;
        }*/
    }
    catch(CMemoryException* e) {
        e->Delete();
        hr = E_OUTOFMEMORY;
    }
    catch(HRESULT hrRes) {
        hr = hrRes;
    }
	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP CParseGsdFile::get_bTraceParsing(BOOL * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }
    *pVal = m_bTraceParsing;

	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CParseGsdFile::put_bTraceParsing(BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_bTraceParsing = newVal;

	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CParseGsdFile::get_sTraceFilePath(BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }
    *pVal = m_sTraceFilePath.AllocSysString();

	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CParseGsdFile::put_sTraceFilePath(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_sTraceFilePath = newVal;

	return S_OK;
}

//------------------------------------------------------------------*
LPCTSTR CParseGsdFile::GetTempDir(void)
{
    LPTSTR  pszTmp = NULL;
    // get path to TMP directory:
    pszTmp = _tgetenv(_T("TMP"));
    if (pszTmp == NULL) {
        // try different spelling of TMP:
        pszTmp = _tgetenv(_T("TEMP"));
    }
    if (pszTmp == NULL) {
        // use default in current directory
        pszTmp = _T(".");
    }

    return pszTmp;
}


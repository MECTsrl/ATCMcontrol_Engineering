/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrarian.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CELibrarian.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrarian.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CL
 *
 * =COMPONENT           CELibrarian
 *
 * =CURRENT      $Date: 28.02.07 18:59 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  6/25/2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/



//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "CELibrarian.h"

#include "CLSysdef.h"
#include "CMainFrm.h"
#include "CIpFrame.h"
#include "CELibrarianDoc.h"
#include "CELibrarianView.h"
#include "CEHeaderView.h"
#include "toolbox.h"
#include "utilif.h"
#include "htmlhelp.h"
#include "CAboutGeneral.h"
#include "ConstDlg.h"
#include "ObjConvert_i.c"
#include "CG_4CLAccess_i.c"

//----  Static Initializations:   ----------------------------------*



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CELibrarianApp

BEGIN_MESSAGE_MAP(CELibrarianApp, CWinApp)
	//{{AFX_MSG_MAP(CELibrarianApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
	ON_COMMAND(ID_CL_LIBRARY_PRINT, OnPrintLibrary)
	ON_COMMAND(ID_CL_VIEW_CONSTANTS, OnViewConstants)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp) 
    ON_COMMAND(ID_CONTEXT_HELP, CWinApp::OnContextHelp) 
	//}}AFX_MSG_MAP
	// Dateibasierte Standard-Dokumentbefehle
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard-Druckbefehl "Seite einrichten"
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

CComModule _Module;


class CCECommandLineInfo : public CCommandLineInfo
{
public:
	CCECommandLineInfo ();

	virtual void ParseParam (LPCTSTR lpszParam, BOOL bFlag, BOOL bLast);

	bool m_bRegServer;
};


CCECommandLineInfo::CCECommandLineInfo ()
{
	m_bRegServer = false;
}


void CCECommandLineInfo::ParseParam (LPCTSTR lpszParam, BOOL bFlag, BOOL bLast)
{
	if (bFlag)
	{
		if (CString (lpszParam).CompareNoCase (_T ("regserver")) == 0)
		{
			m_bRegServer = true;
			return;
		};
	};

	CCommandLineInfo::ParseParam (lpszParam, bFlag, bLast);
}

/////////////////////////////////////////////////////////////////////////////
// CELibrarianApp Konstruktion

CELibrarianApp::CELibrarianApp()
{
	// ZU ERLEDIGEN: Hier Code zur Konstruktion einfügen
	// Alle wichtigen Initialisierungen in InitInstance platzieren
	m_pIObjConvert=NULL;
}

/////////////////////////////////////////////////////////////////////////////
// Das einzige CELibrarianApp-Objekt

CELibrarianApp theApp;

// Dieser Bezeichner wurde als statistisch eindeutig für Ihre Anwendung generiert.
// Sie können ihn ändern, wenn Sie einen bestimmten Bezeichnernamen bevorzugen.

//Old ID: {4721827F-4BD3-11D2-B304-006008736562}
//New ID: {49070009-D886-4491-8574-70E98FAB8C6B}
static const CLSID clsid =
{ 0x49070009, 0xD886, 0x4491, { 0x85, 0x74, 0x70, 0xE9, 0x8F, 0xAB, 0x8C, 0x6B } };
/////////////////////////////////////////////////////////////////////////////
// CELibrarianApp Initialisierung

BOOL CELibrarianApp::InitInstance()
{
	// OLE-Bibliotheken initialisieren
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

#ifdef LIBRARIAN_DEBUG
    DebugBreak();
#endif 

	HRESULT hr = ::CoInitializeSecurity (NULL, -1, NULL, NULL,
	                             RPC_C_AUTHN_LEVEL_NONE, 
	                             RPC_C_IMP_LEVEL_IMPERSONATE, 
	                             NULL, EOAC_NONE, NULL);
	ASSERT (SUCCEEDED (hr));    

	AfxEnableControlContainer();

	// Standardinitialisierung
	// Wenn Sie diese Funktionen nicht nutzen und die Größe Ihrer fertigen 
	//  ausführbaren Datei reduzieren wollen, sollten Sie die nachfolgenden
	//  spezifischen Initialisierungsroutinen, die Sie nicht benötigen, entfernen.

#ifdef _AFXDLL
	Enable3dControls();			// Diese Funktion bei Verwendung von MFC in gemeinsam genutzten DLLs aufrufen
#else
	Enable3dControlsStatic();	// Diese Funktion bei statischen MFC-Anbindungen aufrufen
#endif


	_Module.Init (ObjectMap, m_hInstance);

	// Ändern des Registrierungsschlüssels, unter dem unsere Einstellungen gespeichert sind.
	// Sie sollten dieser Zeichenfolge einen geeigneten Inhalt geben
	// wie z.B. den Namen Ihrer Firma oder Organisation.
	SetRegistryKey(_T("Softing\\4CONTROLV2"));

	LoadStdProfileSettings();  // Standard-INI-Dateioptionen einlesen (einschließlich MRU)

	// Dokumentvorlagen der Anwendung registrieren. Dokumentvorlagen
	//  dienen als Verbindung zwischen Dokumenten, Rahmenfenstern und Ansichten.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CELibrarianDoc),
		RUNTIME_CLASS(CMainFrame),       // Haupt-SDI-Rahmenfenster
		RUNTIME_CLASS(CELibrarianView));
	pDocTemplate->SetServerInfo(
		IDR_SRVR_EMBEDDED, IDR_SRVR_INPLACE,
		RUNTIME_CLASS(CInPlaceFrame));
	AddDocTemplate(pDocTemplate);
	m_pDocTemplate=pDocTemplate;

	// Verbinden des COleTemplateServer mit der Dokumentvorlage.
	//  Der COleTemplateServer legt auf Basis der Informationen in der
	//  Dokumentvorlage bei der Anforderung von OLE-Containern
	//  neue Dokumente an.
	m_server.ConnectTemplate(clsid, pDocTemplate, TRUE);
		// Hinweis: SDI-Anwendungen registrieren Server-Objekte nur dann, wenn /Embedding
		//   oder /Automation in der Befehlszeile enthalten ist.

	// DDE-Execute-Open aktivieren
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Befehlszeile parsen, um zu prüfen auf Standard-Umgebungsbefehle DDE, Datei offen
	CCECommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	//
	// instantiate ATCM Object converter
	//
	HRESULT hres = CoCreateInstance(CLSID_ObjConvert, NULL, CLSCTX_ALL,
		IID_IObjConvert, (void**)&m_pIObjConvert);
	if ( hres != S_OK )
	{
		//AfxMessageBox(_T("Can't create object converter control!"));
		m_pIObjConvert=NULL;
	}

	// Testen, ob Ausführung als OLE-Server
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Alle OLE-Server (-fabriken) als aktiv registrieren. Dies aktiviert die
		//  OLE-Bibliotheken, um Objekte von anderen Anwendungen zu erstellen.
		COleTemplateServer::RegisterAll();

		// Anwendung mit /Embedding oder /Automation gestartet. In diesem Fall
		//  kein Hauptfenster anzeigen.
		return TRUE;
	}

	// Wird eine Server-Anwendung im Standalone-Modus betrieben, ist es ratsam,
	//  die Systemregistrierung zu aktualisieren, falls diese beschädigt wurde.
	m_server.UpdateRegistry(OAT_DOC_OBJECT_SERVER);

	// automation ?
	COleObjectFactory::UpdateRegistryAll();

	//
	// register interfaces (for ATCMControl)
	//
	if (cmdInfo.m_bRegServer)
	{
		TCHAR szModule[_MAX_PATH+10];
		LPOLESTR pszModule;
		HRESULT hr;
		LPTYPELIB pTypeLib;
		USES_CONVERSION;

		::GetModuleFileName (m_hInstance, szModule, _MAX_PATH);
		pszModule = T2OLE (szModule);
		hr = LoadTypeLib (pszModule, &pTypeLib);
		if (SUCCEEDED (hr))
		{
			hr = RegisterTypeLib (pTypeLib, pszModule, NULL);
			pTypeLib->Release ();
		};

		return (FALSE);
	};

	// Prevents ProcessShellCommand from displaying
    // a message box when unregistering.
    if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
    {
        cmdInfo.m_bRunEmbedded = true;
    };

	// Verteilung der in der Befehlszeile angegebenen Befehle
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Das einzige Fenster ist initialisiert und kann jetzt angezeigt und aktualisiert werden.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	return TRUE;
}

int CELibrarianApp::ExitInstance() 
{
	if (m_pIObjConvert != NULL)
		m_pIObjConvert->Release();

	return CWinApp::ExitInstance();
}

CDocument* CELibrarianApp::GetDocument()
{
	POSITION pos=m_pDocTemplate->GetFirstDocPosition();
	CDocument* pDoc = m_pDocTemplate->GetNextDoc(pos);
	return pDoc;
}

IObjConvert* CELibrarianApp::GetObjConvert()
{
	return m_pIObjConvert;
}

void CELibrarianApp::OnHelpFinder() 
{
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen
    ShowHelp(0, HELP_FINDER);
    return;	
}

void CELibrarianApp::ShowHelp(DWORD dwData, UINT nCmd) {
    ASSERT(m_pMainWnd->m_hWnd != NULL);
    CString strManualPath;
    UINT    nStrLen = _MAX_PATH;

    HRESULT hr = UTIL_GetInstallPath(strManualPath.GetBuffer(_MAX_PATH), &nStrLen);
    strManualPath.ReleaseBuffer();
    if(SUCCEEDED(hr)) {
        strManualPath += CL_MANUAL_FILENAME;
        switch(nCmd) {
            case HELP_CONTEXT:
                if(dwData >= 0x30000) {
                    dwData -= 0x30000;
                } else if(dwData >= 0x20000) {
                    dwData -= 0x20000;
                } else if(dwData >= 0x10000) {
                    dwData -= 0x10000;
                }
                HtmlHelp(m_pMainWnd->m_hWnd, strManualPath, HH_HELP_CONTEXT, dwData);
                break;
            case HELP_INDEX:
                break;
            case HELP_FINDER:
                HtmlHelp(m_pMainWnd->m_hWnd, strManualPath, HH_DISPLAY_TOPIC, NULL);
                break;
        }
    }
    return;
}

// Anwendungsbefehl, um das Dialogfeld aufzurufen
void CELibrarianApp::OnAppAbout()
{
	// hook for debuggung Ole Doc Server
	CELibrarianDoc::DebugHook();
	CAboutGeneral aboutDlg;
	aboutDlg.DoModal();
}

int CELibrarianApp::Run()
{
	return CWinApp::Run();
}

void CELibrarianApp::OnViewConstants() 
{
	ConstDlg constDlg;
	constDlg.m_pDocument = (CELibrarianDoc*) GetDocument();
	constDlg.DoModal();
}


void CELibrarianApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	// TODO: Add your specialized code here and/or call the base class
	ShowHelp(dwData, nCmd);
}


/*
 *----------------------------------------------------------------------------*
 *  $History: CELibrarian.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 3  *****************
 * User: Jd           Date: 1.08.01    Time: 15:55
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:32p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 *==
 *----------------------------------------------------------------------------*
*/

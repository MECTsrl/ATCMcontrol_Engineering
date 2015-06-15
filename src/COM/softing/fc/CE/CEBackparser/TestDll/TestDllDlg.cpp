// TestDllDlg.cpp : implementation file
//

#pragma warning(disable: 4786)

#include "stdafx.h"
#include "TestDll.h"
#include "TestDllDlg.h"

#include "DLLInterface.h"
#include "StResource.h"
#include "Configuration.h"
//#include "keyrc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ofstream output("TestDLLOutput.txt");

/////////////////////////////////////////////////////////////////////////////
// CTestDllDlg dialog

CTestDllDlg::CTestDllDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDllDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDllDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDllDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestDllDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDllDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_TEST, OnTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDllDlg message handlers

BOOL CTestDllDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestDllDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestDllDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//================================================================================
void CTestDllDlg::OnTest()
//================================================================================
{
    try
    {
        //=== read in list of filenames to parse
        ListOfFileNames fileNameList;
        readListOfFileNames("files.txt", fileNameList);

        //=== for each file in list ...
        for(int i = 0; i < fileNameList.size(); ++i )
        {
            //=== get absolute path
            string absPath;
            getAbsolutePath(fileNameList[i], absPath);

            test_cvd_gen(absPath);
//            test_ops(absPath);
//            test_resource_positions(absPath);
//            test_configuration_positions(absPath);
//            test_globvarconstantlist_positions(absPath);
//            test_struct_positions(absPath);
//            test_get_var_name(absPath);
//            test_dates(absPath);
//            test_task_configuration(absPath);
        }
    }
    catch(string e)
    {
        output << "string exception: " << e << endl; output.flush();
        return;
    }
    catch(const char* e)
    {
        output << "char* exception: " << ( e == 0 ? "-null string" : e ) << endl; output.flush();
        return;
    }
    catch(exception e)
    {
        output << "exception: " << e.what() << endl; output.flush();
        return;
    }
    catch(...)
    {
        output << "unknown exception caught" << endl; output.flush();
        return;
    }
}

//================================================================================
void readListOfFileNames(const string& fileNameListFile,
                         ListOfFileNames& fileNameList)
//================================================================================
{
    string buf;
    readFileContent(fileNameListFile, buf);

    StringPos lineStart = buf.find_first_not_of(" \t\r\n");
    StringPos lineEnd   = buf.find_first_of("\r\n");
    while( lineStart < buf.size() && lineEnd < buf.size() )
    {
        if ( buf.at(lineStart) != '#' )
            fileNameList.push_back(buf.substr(lineStart, lineEnd - lineStart));

        lineStart = buf.find_first_not_of(" \t\r\n", lineEnd);
        lineEnd   = buf.find_first_of("\r\n", lineStart);
    }

    return;
}

//================================================================================
void getAbsolutePath(const string& relPath, string& absPath)
//================================================================================
{
    char full[_MAX_PATH];
    if( _fullpath( full, relPath.c_str(), _MAX_PATH ) != NULL )
         absPath = full;
      else
         throw string("invalid path: ") + relPath;
}

//================================================================================
void readFileContent(const string& fileName, string& buf)
//================================================================================
{
    ifstream input(fileName.c_str());

    if ( ! input )
        throw string("could not open file for reading: \"") + fileName + "\"";

    buf.erase();
    char ch;
    while( input.get(ch) )
        buf += ch;

    if ( ! input.eof() )
        throw string("error while reading file: \"") + fileName + "\"";

    input.close();

    return;
}


//================================================================================
void test_cvd_gen(const string& absFilePath)
//================================================================================
{
    //==== parse file content
    output << "parsing file: " << absFilePath << endl;
    string XmlText;
    doBackparse(absFilePath, XmlText);
    output << XmlText << endl;
}
  
//================================================================================
void test_ops(const string& absFilePath)
//================================================================================
{
    //==== parse file content
    output << "parsing file: " << absFilePath << endl;
    
//    StStruct* pou = 0;
    FunctionBlock* pou = 0;
//    Function* pou = 0;


    string SourceFile;
    BOOL grFile = FALSE;

    CString extension = CString(absFilePath.c_str()).Right(4);

    if (extension.Right(4).CompareNoCase(".4gr") == 0)
    {
        grFile = TRUE;
        ExtractHeader(absFilePath, SourceFile);
    }
    else
    {
        SourceFile = absFilePath;
    }

    //=== read file content
    string buf;
    readFileContent(SourceFile, buf);

    string err = parse_string(buf, &pou);

    StringPos insertPos;
    bool sectionExists;
    pou->getVarInsertPos("VAR_INPUT", "", insertPos, sectionExists);
//    pou->getVarInsertPos("STRUCT", "", insertPos, sectionExists);

    string varDecl = "    NewVar : DINT;\n";
    string varDeclSect = "\nVAR_INPUT\n" + varDecl + "END_VAR";
//    string varDeclSect = "\nSTRUCT\n" + varDecl + "END_VAR";

    if ( sectionExists )
    {
        buf.insert(insertPos, varDecl);
    }
    else
    {
        buf.insert(insertPos, varDeclSect);
    }

    output << buf << endl;

    if (grFile == TRUE)
    {
        string MHFile = "c:\\modifiedHeader.txt";
        ofstream modifiedHeader(MHFile.c_str());
        modifiedHeader << buf << endl;
        modifiedHeader.close();
        MergeHeader(absFilePath, MHFile, TRUE);
    }
    
    delete pou;
}

//================================================================================
void test_resource_positions(const string& absFilePath)
//================================================================================
{
    //==== parse file content
    output << "parsing file: " << absFilePath << endl;
    
    Resource* res = 0;

    //=== read file content
    string buf;
    readFileContent(absFilePath, buf);

    string err = parse_string(buf, &res);

    program_configurations_type program_configurations = res->getProgramConfigurations();
    program_configurations_citer ci_progconf;
    for(ci_progconf = program_configurations.begin();
        ci_progconf != program_configurations.end();
        ++ci_progconf)
    {
        string ProgClass = (*ci_progconf)->getProgramClass();
        StringPosRange ProgClassRange = (*ci_progconf)->getProgramClassRange();
        StringPosRange NameRange = (*ci_progconf)->getNameRange();
        StringPosRange Range = (*ci_progconf)->getRange();
    }

    task_configurations_type task_configurations = res->getTaskConfigurations();
    task_configurations_citer ci_taskconf;
    for(ci_taskconf = task_configurations.begin();
        ci_taskconf != task_configurations.end();
        ++ci_taskconf)
    {
        StringPosRange NameRange = (*ci_taskconf)->getNameRange();
        StringPosRange Range = (*ci_taskconf)->getRange();
    }

    // modify import directive
    import_directives_type import_directives = res->getImportDirectives();

    import_directives_citer ci_impdir;
    for(ci_impdir = import_directives.begin();
        ci_impdir != import_directives.end();
        ++ci_impdir)
    {
    }
    --ci_impdir;
    --ci_impdir;

    StringPosRange range = (*ci_impdir)->getImportDirectiveRange();
    buf.erase(range.getFirst() - 1, range.getLast() - range.getFirst() + 2);
    string NewImportDirective = "#IMPORT \"ModifiedGlobVarFile.gvl\"";
    buf.insert(range.getFirst() -1, NewImportDirective);

    // add import directive
    delete res;
    err = parse_string(buf, &res);
    NewImportDirective = "\n    #IMPORT \"AddedGlobVarFile1.gvl\"";
    buf.insert(res->getNextImpDirInsertPos(), NewImportDirective);

    // add import directive
    delete res;
    err = parse_string(buf, &res);
    NewImportDirective = "\n    #IMPORT \"AddedGlobVarFile2.gvl\"";
    buf.insert(res->getNextImpDirInsertPos(), NewImportDirective);

    // add task conf
    delete res;
    err = parse_string(buf, &res);
    string NewTaskConf = "\n    TASK AddedTask1(INTERVAL:=T#100ms, PRIORITY:=5);";
    buf.insert(res->getNextTaskConfInsertPos(), NewTaskConf);

    // add task conf
    delete res;
    err = parse_string(buf, &res);
    NewTaskConf = "\n    TASK AddedTask2(INTERVAL:=T#100ms, PRIORITY:=5);";
    buf.insert(res->getNextTaskConfInsertPos(), NewTaskConf);

    // add prog conf
    delete res;
    err = parse_string(buf, &res);
    string NewProgConf = "\n    PROGRAM AddedProg1 WITH TaskSimulate: Simulate;";
    buf.insert(res->getNextProgConfInsertPos(), NewProgConf);

    // add prog conf
    delete res;
    err = parse_string(buf, &res);
    NewProgConf = "\n    PROGRAM AddedProg2 WITH TaskSimulate: Simulate;";
    buf.insert(res->getNextProgConfInsertPos(), NewProgConf);

    output << buf << endl;

    delete res;
}

//================================================================================
void test_configuration_positions(const string& absFilePath)
//================================================================================
{
    //==== parse file content
    output << "parsing file: " << absFilePath << endl;
    
    Configuration* con = 0;

    //=== read file content
    string buf;
    readFileContent(absFilePath, buf);

    string err = parse_string(buf, &con);

    // modify import directive
    import_directives_type import_directives = con->getImportDirectives();

    import_directives_citer ci_impdir;
    for(ci_impdir = import_directives.begin();
        ci_impdir != import_directives.end();
        ++ci_impdir)
    {
    }
    --ci_impdir;
    --ci_impdir;

    StringPosRange range = (*ci_impdir)->getImportDirectiveRange();
    buf.erase(range.getFirst() - 1, range.getLast() - range.getFirst() + 2);
    string NewImportDirective = "#IMPORT \"ModifiedResource.res\"";
    buf.insert(range.getFirst() -1, NewImportDirective);

    // add import directive
    delete con;
    err = parse_string(buf, &con);
    NewImportDirective = "\n    #IMPORT \"AddedResource1.res\"";
    buf.insert(con->getNextImpDirInsertPos(), NewImportDirective);

    // add import directive
    delete con;
    err = parse_string(buf, &con);
    NewImportDirective = "\n    #IMPORT \"AddedResource2.res\"";
    buf.insert(con->getNextImpDirInsertPos(), NewImportDirective);

    output << buf << endl;

    delete con;
}

//================================================================================
void test_globvarconstantlist_positions(const string& absFilePath)
//================================================================================
{
    //==== parse file content
    output << "parsing file: " << absFilePath << endl;
    
    GlobVarConstList* gvcl = 0;

    //=== read file content
    string buf;
    readFileContent(absFilePath, buf);

    string err = parse_string(buf, &gvcl);

    if ( err.empty() )
    {
        StringPos insertPos;
        bool sectionExists;
        gvcl->getVarInsertPos("CONST", "", insertPos, sectionExists);

        string varDecl = "    NewVar : DINT;\n";
        string varDeclSect = "\nCONST\n" + varDecl + "END_CONST";

        if ( sectionExists )
        {
            buf.insert(insertPos, varDecl);
        }
        else
        {
            buf.insert(insertPos, varDeclSect);
        }

        output << buf << endl;
    }
    else
    {
       output << err << endl;
    }

    delete gvcl;
}

//================================================================================
void test_struct_positions(const string& absFilePath)
//================================================================================
{
    //==== parse file content
    output << "parsing file: " << absFilePath << endl;
    
    StStruct* st = 0;

    //=== read file content
    string buf;
    readFileContent(absFilePath, buf);

    string err = parse_string(buf, &st);

    if ( err.empty() )
    {
        StringPos insertPos;
        bool sectionExists;
        st->getVarInsertPos("STRUCT", "", insertPos, sectionExists);

        string varDecl = "    NewVar : DINT;\n";
        string varDeclSect = "\nVAR_INPUT\n" + varDecl + "END_VAR";

        if ( sectionExists )
        {
            buf.insert(insertPos, varDecl);
        }
        else
        {
            buf.insert(insertPos, varDeclSect);
        }

        output << buf << endl;
    }
    else
    {
       output << err << endl;
    }

    delete st;
}

//================================================================================
void test_get_var_name(const string& absFilePath)
//================================================================================
{
    //==== parse file content
    output << "parsing file: " << absFilePath << endl;
    
//    FunctionBlock* pou = 0;
    FunctionBlock* fb = 0;

    //=== read file content
    string buf;
    readFileContent(absFilePath, buf);

    string err = parse_string(buf, &fb);

    if ( err.empty() )
    {
        string name;
        StringPosRange range;
        const var_decl_sections_type vds = fb->getVarDeclSections();

        for(var_decl_sections_citer ci_vds = vds.begin();
            ci_vds != vds.end();
            ++ci_vds)
        {
            const var_declarations_type vd = (*ci_vds)->getVarDeclList();
            
            for(var_declarations_citer ci_vd = vd.begin();
                ci_vd != vd.end();
                ++ci_vd)
            {
                const vars_type v = (*ci_vd)->getVariables();

                for(vars_citer ci_v = v.begin();
                    ci_v != v.end();
                    ++ci_v)
                {
                    name = (*ci_v)->getName();
                    range = (*ci_v)->getNameRange();
                    break;
                }
                break;
            }
           break;
         }

       output << buf << endl;
       output << name << endl;
       output << range.getFirst() << endl;
       output << range.getLast() << endl;
       output << range.getLength() << endl;
    }
    else
    {
       output << err << endl;
    }

    delete fb;
}

//================================================================================
void test_dates(const string& absFilePath)
//================================================================================
{
    string CreateDate;
    string ModificationDate;
    HRESULT hr = GetDates(absFilePath, CreateDate, ModificationDate);
}

//================================================================================
void test_task_configuration(const string& absFilePath)
//================================================================================
{
    //==== parse file content
    output << "parsing file: " << absFilePath << endl;
    
    Resource* res = 0;

    //=== read file content
    string buf;
    readFileContent(absFilePath, buf);

    string err = parse_string(buf, &res);

    // modify import directive
    task_configurations_type task_configurations = res->getTaskConfigurations();

    task_configurations_citer ci_taskconf;
    for(ci_taskconf = task_configurations.begin();
        ci_taskconf != task_configurations.end();
        ++ci_taskconf)
    {
    }
    --ci_taskconf;
    --ci_taskconf;


    string name = (*ci_taskconf)->getName();
    StringPosRange nameRange = (*ci_taskconf)->getNameRange();

    string fftask = (*ci_taskconf)->getFftask();
    StringPosRange fftaskRange = (*ci_taskconf)->getFftaskRange();

    string interval = (*ci_taskconf)->getInterval();
    StringPosRange intervalRange = (*ci_taskconf)->getIntervalRange();

    string iosync = (*ci_taskconf)->getIosync();
    StringPosRange iosyncRange = (*ci_taskconf)->getIosyncRange();

    string priority = (*ci_taskconf)->getPriority();
    StringPosRange priorityRange = (*ci_taskconf)->getPriorityRange();

    string autostart = (*ci_taskconf)->getAutostart();
    StringPosRange autostartRange = (*ci_taskconf)->getAutostartRange();

    StringPosRange initlistrange = (*ci_taskconf)->getInitialisationListRange();

    output << buf << endl;

    delete res;
}
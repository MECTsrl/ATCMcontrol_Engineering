
#ifndef __OPERATION__
#define __OPERATION__

class CPropertySheetBase;

#include <atlbase.h>
#include <string>
#include <fstream>
#include "CEProjInfo.h"
#include "CEAddOnHandlerGlobal.h"
#include "keyrc.h"
#include "Var.h"
#include "afxtempl.h"
#include "pou.h"
#include "DllInterface.h"

using namespace std;

#define CEOBJ_GUIDLEN  64
#define SPACETAB (_T("    "))
#define CE_MAXUSERNAMELEN (256 + 1)

#define FILETYPE_ST         1 
#define FILETYPE_FBD        2
#define FILETYPE_SFC        3
#define FILETYPE_LD         4
#define FILETYPE_4GR        5
#define FILETYPE_IL         6 

#define MAX_PROPOSAL_COUNT  1000     // for name proposal index

//variable storage class <-> string conversion
typedef struct 
{
    int iStorageClass;
	int iIDS;
    LPCTSTR szDefault;
    LPCTSTR szFCCBDescr;
} STORAGETYPE_DESC;

static STORAGETYPE_DESC StorageClassTbl1[] = 
{
	{CVAR_TYPE_LOCAL,IDS_KEY_VAR,_T("VAR"),_T("")},
	{CVAR_TYPE_GLOBAL,IDS_KEY_VARGLOBAL,_T("VAR_GLOBAL"),_T("")},
	{CVAR_TYPE_IN,IDS_KEY_VARIN,_T("VAR_INPUT"),_T("")},
	{CVAR_TYPE_OUT,IDS_KEY_VAROUT,_T("VAR_OUTPUT"),_T("")},
	{CVAR_TYPE_INOUT,IDS_KEY_VARINOUT,_T("VAR_IN_OUT"),_T("")},
	{CVAR_TYPE_LOCAL | CVAR_TYPE_RETAIN,IDS_KEY_VAR_RETAIN,_T("VAR RETAIN"),_T("")},
	{CVAR_TYPE_GLOBAL | CVAR_TYPE_RETAIN,IDS_KEY_VARGLOBAL_RETAIN,_T("VAR_GLOBAL RETAIN"),_T("")},
	{CVAR_TYPE_IN    | CVAR_TYPE_RETAIN,IDS_KEY_VARIN_RETAIN,_T("VAR_INPUT RETAIN"),_T("")},
	{CVAR_TYPE_OUT   | CVAR_TYPE_RETAIN,IDS_KEY_VAROUT_RETAIN,_T("VAR_OUTPUT RETAIN"),_T("")},
	{CVAR_TYPE_LOCAL | CVAR_TYPE_NONRETAIN,IDS_KEY_VAR_NONRETAIN,_T("VAR NONRETAIN"),_T("")},
	{CVAR_TYPE_GLOBAL | CVAR_TYPE_NONRETAIN,IDS_KEY_VARGLOBAL_NONRETAIN,_T("VAR_GLOBAL NONRETAIN"),_T("")},
	{CVAR_TYPE_IN    | CVAR_TYPE_NONRETAIN,IDS_KEY_VARIN_NONRETAIN,_T("VAR_INPUT NONRETAIN"),_T("")},
	{CVAR_TYPE_OUT   | CVAR_TYPE_NONRETAIN,IDS_KEY_VAROUT_NONRETAIN,_T("VAR_OUTPUT NONRETAIN"),_T("")},
	{CVAR_TYPE_LOCAL | CVAR_TYPE_CONST,IDS_KEY_VAR_CONST,_T("VAR CONST"),_T("")},
	{CVAR_TYPE_GLOBAL | CVAR_TYPE_CONST,IDS_KEY_VARGLOBAL_CONST,_T("VAR_GLOBAL CONSTANT"),_T("")},
    {CVAR_TYPE_INEXT,IDS_KEY_VAR_INEXT,_T("VAR_IN_EXT"),_T("")},
    {CVAR_TYPE_TEMP,IDS_KEY_VAR_TEMP,_T("VAR_TEMP"),_T("")},
    {CCONST_TYPE,IDS_KEY_CONST,_T("CONST"),_T("")},
    {CVAR_TYPE_EXTERNAL,IDS_KEY_VAR_EXTERNAL,_T("VAR EXTERNAL"),_T("")}
};


class COperation
{
public:
	COperation(CPropertySheetBase *Dialog,
               BSTR sSourceFile,
               BSTR sId,
               ICEProjInfo *pICEProjectInfo,
               BSTR sAdditionalInfo);
	virtual ~COperation();

    CEADDONHANDLEREXPORT virtual BOOL Execute();
    CEADDONHANDLEREXPORT ICEProjInfo* GetProjInfo();

    void SetAutomationMode(BOOL bMode);

    virtual void SetFileName();
    virtual void Init();
    void InitFromFile(POU* Pou);
    CEADDONHANDLEREXPORT virtual void FillDialog();
    void WriteHeader(CString sPouText);
    void MergeHeaderIntoFile(CString sPouText);
    void WriteFile(CString sPouText);
    void InformProjectManager();
    void SetGUID(CString &StrGUID);
    void CreateGUID(); 
    CString & GetGUID();
    
    void GetStdAttribString(CString & strText, int iTabs = 1);
    int  WriteStdHeader();
    void AddQuotation(LPCTSTR pszText, CString & strQuotedText);
    void AppendCommentBegin(CString & strText, int iTabs = 1);
    void AppendCommentEnd(CString & strText, int iTabs = 1);
    void AppendAuthorsAndDates(CString & strText, int iTabs = 1);
    void AppendGuid(CString & strText, int iTabs = 1);
    void AppendUsrText(CString & strText, int iTabs = 1, bool bComma = true);
    void AppendAlias(CString & strText, int iTabs = 1);
    void AppendUsrURL(CString & strText, int iTabs = 1);
    void AppendHelpURL(CString & strText, int iTabs = 1);
    void AppendAttrib(CString &, string & key, string & value, int iTabs = 1);

    void GetCurrentUserName(CString & UserName);
    void SetPouDT(BOOL bAdd);
    void KillLastComma(CString & strText);
    int  VarStorageClassByString(LPCTSTR pszStorage);
    
    void SetCreateAuthor(CString &StrAuthor);
    CString & GetCreateAuthor();
    void SetModifyAuthor(CString &StrAuthor);
    CString & GetModifyAuthor();
    
    void AddVar(CVar & var);
    int  InsertVarList(CString & sPouText, POU*  Pou);
    int InsertNewVar(CVar & var, CString & sPouText, POU*  Pou);
    int ModifyVar(CVar & var, CString & sPouText, POU*  Pou);
    int ModifyVarStrClass(CVar & var, CString & sPouText, POU*  Pou);
    int DeleteVar(CVar & var, CString & sPouText, POU*  Pou);
    StringPos GetIP(CVar & var, bool & bSection, POU*  Pou);

    POU* ParsePou(CString & sPotText, POU*  Pou);
    void ReadFileContent(const CString fileName, string& buf);
    BOOL DoesFileExist(CString strFileName, BOOL bWithExt);
    
    CString GetNameProposal(CString sBasis);
    CString GetNameFromId();
    CString GetExtentionFromFileName(CString strFileName);
    void InsertSysComment(CString & PouText);
    void InsertSysComment2(CString & PouText, POU* pou);

    BOOL CreateUsrHelp();
    void CreateUsrHelpLink();
    
    POSITION GetVarPos(CString strVarName);

    CString GetAbsPath(const CString& crstrFileString, const CString& crstrProjPath);
    CEADDONHANDLEREXPORT BOOL IsSourceReadOnly();

    // get position after next newline (if not exists, crlf is appended)
    StringPos GetNextInsertPosition(string& text, StringPos startpos);

    void SetAlias(const CString& strAlias);
    void SetUsrText(int i, const CString& strText);
    void SetHelpUrl(const CString& strUrl);
    void SetUrl(const CString & strUrl);
    
//Attributes
    CString             m_sFileName;
    CComBSTR            m_ProjectPath;
    CList <CVar,CVar &>   m_aVarList;

//those were protected:
    ICEProjInfo         *m_pICEProjectInfo;
    CComBSTR            m_sSourceFile;
    CComBSTR            m_sId;
    CComBSTR            m_sAdditionalInfo;
    CString	            m_sName;
protected:
    CPropertySheetBase  *m_pDialog;
    
    CString             m_sObjGUID; 
    CString	            m_sAlias;
    CString	            m_sUsrText[CE_NUMUSRTXT];
	CString	            m_sHelpUrl;
    CString	            m_sHelpTemplate;
	CString	            m_sUrl;

	VARIANT	            m_vtCreateDate;
    CString             m_sCreateDate;
	CString	            m_sCreateUser;
	VARIANT	            m_vtLastModDate;
    CString             m_sLastModDate;
	CString	            m_sLastModUser;

    SAFEARRAY*          m_pMembers;
    LONG                m_lObjClassId;

    StringPos           m_iSysCmtPos;
    StringPos           m_iSysCmtLength;

    int                 m_FileType;
    BOOL                m_bSilent;          // silent mode
    BOOL                m_bSourceReadOnly;  // source file is read only?
    BOOL                m_bAutomationMode;
};


#endif

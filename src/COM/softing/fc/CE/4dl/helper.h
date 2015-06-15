
#ifndef __HELPER_H_
#define __HELPER_H_



//----  Aggregate Includes:   --------------------------------------*
#include <stdio.h>
#include "fc_todebug\fc_assert.h"
#include "fc_tools\fc_libmem.h"
#include "fc_tools\fc_cgarray.h"
#include "fc_tools\fc_cstring.h"
#include "fc_tools\fc_fileIO.h"
#include "fc_tools\fc_hash.h"
#include "baselib.h"
#include "OPCDA.h"
#include "CSC_Online.h"
#include "CSC_OnlineServer.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*
#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>

extern int g_bForceShutdown;

void InitModule();


//----  Prototypes:    ---------------------------------------------*
#define MAX_GROUP_SIZE 4096



class CMDL_INPUT //gets initialised by ParseCmdLine
{
public:
    bool         bDownload;
    bool         bMergeWarn;
    bool         bMergeOnly;
    bool         bDLChanges;
    bool         bDLDoFlash;
    bool         bStopTasksForUpload;

    FC_CStr      upload4PL;
    FC_CStr      uploadDestDir;

    FC_CStrList  trgNames;
    FC_CStrList  connectStr;

    FC_CStr      passWord;
    const TCHAR* pszPassWord; //NULL or points to passWord
    int          nMaxGS;  //max OPC group size 
    int          nVerbose;//more messages

    //only set if bDownload == true:
    FC_CStr      prjFile;
    FC_CStr      prjName;
    FC_CStr      prjPath;
    FC_CStr      genPath;
    BSTR         sGenPath;
    FC_CStr      destDirMerged;
    FC_CStrList  loaderLists; //with full path
    FC_CStrList  download4PL;
    FC_CStrList  safe4PL;



    CMDL_INPUT()
    {
        bDownload = false;
        pszPassWord = NULL;
        nMaxGS  = 50;
        nVerbose= 0;
        bDLChanges  = bDLDoFlash = false;
        bMergeOnly = bMergeWarn = false;
        bStopTasksForUpload = false;
        sGenPath    = NULL;
    }
    ~CMDL_INPUT()
    {
        FC_FREE_BSTR(sGenPath);
    }
};


bool ParseCmdLine(
    int             argc, 
    TCHAR*          argv[], 
    CMDL_INPUT*     pCmdl
);
bool DumpLogoAndHelp(
    int           argc, 
    TCHAR**       argv
);




class CConnection;
typedef HRESULT(*OnConfigChangedCB)(CConnection* pCnt, void* pCtx);


class CImpBPNotify;
class CConnection
{
    friend class CImpBPNotify;
    IFCSessions*                m_pISessions;
    FC_CStr                     m_strUser;
    FC_CStr                     m_strPassWd;
    FC_CStr                     m_strTrgName;
    FC_CStr                     m_strControlAddr;
    BSTR                        m_sUser;
    BSTR                        m_sPassWd;
    BSTR                        m_sTrgName;
    BSTR                        m_sControlAddr;

    IFCConnect*	                m_pIConnect;
   	IConnectionPoint*           m_pICPBPNotify;
    CComObject<CImpBPNotify>*   m_pIBPNotifyObj;
    DWORD                       m_dwBPNotifyCookie;


    //for access to the raw DCOM IF of the OPC-Server:
    BSTR                        m_sCompName; //getenv("COMPUTERNAME")
    BSTR                        m_sServerPCName;
    BSTR                        m_sCustCLSID;
    BSTR                        m_sCustInit;
    GUID                        m_custCLSID;

    //raw DCOM connection to OPC-Server:
    ICommissioningMgt*          m_pOSMgt;
    long                        m_lOSKey;    //Online Server Key
    IOPCServer*                 m_pOSOPCSrv; //Variable IF
    IOPCSyncIO*                 m_pSyncIO;   //Variable IF: sync IF 
    IOPCItemMgt*                m_pItemMgt;  //Variable IF: sync IF
    IControlMgt*                m_pICtrlMgt; //Start/Stop Control/Task
    long                        m_lCtrlMgtKey;
    bool                        m_bCtrlLoggedIn;

    ITransfer*                  m_pITransfer;//Domain Taransfer IF

    OnConfigChangedCB           m_pCBocch;
    void*                       m_pCBocchCtx;

    bool createSyncVarIF(
        const wchar_t* pszGroupName,
        IOPCSyncIO**   ppSyncIO,
        IOPCItemMgt**  ppItemMgt 
    );


public:


    CConnection(
        IFCSessions*   pISessions,
        const TCHAR*   pszTrgName,
        const TCHAR*   pszControlAddr,
        const TCHAR*   pszPassWd
    );
    ~CConnection();//dtor calls closeCommissioningSession

    //connect info strings as TCHAR* and BSTR:
    //available after called CConnection::CConnection:
    const TCHAR* getUser()       const{return m_strUser;}
    const TCHAR* getPassWd()     const{return m_strPassWd;}
    const TCHAR* getTrgName()    const{return m_strTrgName;}
    const TCHAR* getControlAddr()const{return m_strControlAddr;}
    BSTR getsUser()              const{return m_sUser;}
    BSTR getsPassWd()            const{return m_sPassWd;}
    BSTR getsTrgName()           const{return m_sTrgName;}
    BSTR getsControlAddr()       const{return m_sControlAddr;}

    //called open and close a commissioning session via CSC_Online:
    bool openCommissioningSession();
    void closeCommissioningSession();

    //get a IFCConnect, 
    //available after successfully called openCommissioningSession()
    IFCConnect*	  getIFCConnect() const{return m_pIConnect;}

    void setOnConfigChangedCallback(OnConfigChangedCB pCB, void* pCBocchCtx)
    {
        m_pCBocch    = pCB;
        m_pCBocchCtx = pCBocchCtx;
    }

    //Raw OPC-Server IF:
    bool openRawVarSession();  //OPC variable access
    bool openRawComSession();  //exclusive ITransfer access with login

    void closeRawVarSession();
    void closeRawCtrlMgt();
    void closeRawComSession();

    bool checkResourceState();
    
    IOPCServer*   getRawIOPCServer(
        IOPCSyncIO**  ppSyncIO, 
        IOPCItemMgt** ppItemMgt
    )const;
    
    ITransfer*    getRawOSTransfer(
        long* plKey
    )const;
    IControlMgt* getCtrlMgt(long* plKey);
};





//uses the Downloader and the CSC_Online:
bool DownloadControl(
    CConnection*          pcnt,
    const CMDL_INPUT*     pDLO,
    const FC_CStrList*    p4PL,
    int                   nVerbose,
    int                   nMaxGS
);


//uses the raw DCOM OPC-Server IF:
//return -1 if error 0 if no such domain 1 if all ok.
int UploadBinaryDomain(
    const CConnection* pCnt,
    const TCHAR*       pszDomain,
    BYTE**             ppData,
    DWORD*             pnBytes,
    const TCHAR*       pszDestFile
);



//uses the raw DCOM OPC-Server IF:
//return -1 if error 0 if no such domain 1 if all ok.
int UploadParameter(
    CConnection*       pCnt,         //an open or closed DCOM connection
    const TCHAR*       pszParamFile, //parameter file name
    const TCHAR*       pszDestFile,
    int                nVerbose,
    int                nMaxGS
);

bool MergeTargetParameterFiles(
    const TCHAR*        pszTrgName, 
    const TCHAR*        pszLoaderList, 
    const FC_CStrList*  pParamFilesOld,
    FC_CStrList*        pParamFilesMgt,
    bool                bWarn,
    int                 nVerbose,
    const TCHAR*        pszMergeDest,
    FC_CStrList*        pDeleteFileList
);


bool DownloadParameter(
    CConnection*       pCnt,            //an open or closed DCOM connection
    const TCHAR*       pszParamFile,    //new parameter file name to download
    int                nVerbose,
    int                nMaxGS
);








struct BINVAL_TYP
{
    BL_IEC_TYP   typ;
    union
    {
        BOOL                aBOOL;
        BYTE                aBYTE;
        WORD                aWORD;
        DWORD               aDWORD;
        unsigned __int64    aLWORD;
        short               aINT;
        LONG                aDINT;
        __int64             aLINT;
        float               aREAL;
        double              aLREAL;
        LONG                aTOD;
        LONG                aDATE;
        __int64             aTIME;
        __int64             aDT;
        struct
        {
            int     nStrlen;
            TCHAR*  pszStr;
        };
    };
};
inline void InitBinVal(BINVAL_TYP* p)
{
    p->typ    = BL_ANY;
    p->aDWORD = 0;
}
inline void FreeBinVal(BINVAL_TYP* p)
{
    if(p->typ==BL_STRING || p->typ==BL_WSTRING)
        delete(p->pszStr);

    p->typ    = BL_ANY;
    p->aDWORD = 0;
}
void CopyBinVal(
    BINVAL_TYP*       pDest, //must be initialised !
    const BINVAL_TYP* pSrc //must be initialised !
);
bool BinValToVariant(
    const BINVAL_TYP*    pVal,
    VARIANT*             pVar   //gets always initialised (if error by VariantInit)
);
bool VariantToBinVal(
    const VARIANT*       pVar,  
    BL_IEC_TYP           want,
    BINVAL_TYP*          pVal  //must be initialised on entry, existing content is freed
);
void BinValToStrSimle(
    const BINVAL_TYP*    pVal,
    FC_CString*          pStr          //c-like value string
);
bool StrSimleToBinVal(
    const TCHAR*   psz,
    BL_IEC_TYP     want,
    BINVAL_TYP*    pVal,
    TCHAR          szErrDescr[MAX_PATH]
);










class CVarItem
{
public:
    long         nLineNo;
    const TCHAR* pszInst;
    const TCHAR* pszMember;
    BL_IEC_TYP   m_typ;
    BINVAL_TYP   value;

    CVarItem(long inLineNo, const TCHAR* ipszInst, const TCHAR* ipszMember, BL_IEC_TYP typ)
    {
        nLineNo   = inLineNo;
        pszInst   = ipszInst;
        pszMember = ipszMember;
        m_typ     = typ;
        InitBinVal(&value);
    }
    ~CVarItem()
    {
        FreeBinVal(&value);
    }
};

struct MEMBER_LIST
{
    long                     m_arrDim;
    FC_CGArray<CVarItem*>    m_memberList;
};


typedef FC_CHashStr<CVarItem*>          MEMB_HASH;
typedef const MEMB_HASH::Pair*          MEMB_HASH_IT;
typedef FC_CHashStr<MEMB_HASH*>         INST_HASH;
typedef const INST_HASH::Pair*          INST_HASH_IT;
struct ITEMPOS_IT
{
    INST_HASH_IT pInst;
    MEMB_HASH_IT pMemb;
};

class CParamList
{
    FC_CStr                 m_fileName;
    FC_CStr                 m_targetTyp;
    FC_CStr                 m_targetVer;
    FC_CStr                 m_projectName;
    FC_CStr                 m_projectVer;
    FC_CStr                 m_resName;


    FC_CHashStrTable        m_strbag;
    INST_HASH               m_instHash;
    int                     m_maxPathLen; //not counting resource name

    bool parseHelp(FC_CSVMemFile* pCSV);
    bool registerItem(
        long         nLineNo, 
        const TCHAR* pszPath, 
        const TCHAR* pszMember, 
        const TCHAR* pszType,
        const TCHAR* pszValue
    );
    MEMB_HASH* insertInst(const TCHAR* pszInstReg);
    CVarItem*  lookUpItem(const TCHAR* pszPath, const TCHAR* pszMember);

public:
    CParamList();
    ~CParamList();
    void empty();

    bool load(
        const TCHAR*  pszFileName, 
        TCHAR*        pszContent, 
        unsigned long ulChars
    );
    int  merge(const CParamList* pOldPL, bool bWarn, long flags);
    bool dump(const TCHAR* pszFile, bool bHeader, const TCHAR* pszControlAddr);


    const TCHAR*     getResName() const{return m_resName;}
    const TCHAR*     getFileName()const{return m_fileName;}
    int              getMaxPathLen()const{return m_maxPathLen;}
    int              getItemCount()const;
    CVarItem*        getFirstVar(ITEMPOS_IT* pPos);
    CVarItem*        getNextVar(ITEMPOS_IT* pPos);

    void buildFullItemPath(
        const CVarItem* pItem, 
        FC_CString*      pJot,
        wchar_t**        ppszNew)const;
};



//mics helpers:
bool LoadTxtFileToMem(const TCHAR* pszFile, TCHAR** ppsz, unsigned long* pulChars);
BL_IEC_TYP ParseSimpleType(const TCHAR* pszType, long* pnArrDim);
BL_IEC_TYP ParseSimpleType(const TCHAR* pszType, long* pnArrDim);
VARTYPE      BLTypToVariantTyp(BL_IEC_TYP blTyp);
const TCHAR* BLTypToString(BL_IEC_TYP blTyp);
bool VariantToString(const VARIANT* pvd, BL_IEC_TYP want, FC_CString* pJot, bool bForDisplay);
bool StringToVariant(const TCHAR* pszTxt, int nChars, VARIANT* pvd, BL_IEC_TYP want);




bool GetConnectStringFromLoaderList(
    const TCHAR* pszListFile, 
    const TCHAR* pszTrgName,
    FC_CString*  pStrConnect
);

extern "C" void FC_CSVRecordToFILEA(
    FILE*        pFile,
    const char*  pBinStr, 
    int          iChars
);
extern "C" void FC_CSVRecordToFILEW( //TODO put into fc_tool.lib
    FILE*          pFile,
    const wchar_t* pBinStr, 
    int            iChars
);
#ifdef UNICODE
 #define FC_CSVRecordToFILE FC_CSVRecordToFILEW
#else
 #define FC_CSVRecordToFILE FC_CSVRecordToFILEA
#endif

extern "C" int FC_CSVNeedEscapeA(
    const char*  pBinStr, 
    int          iChars
);
extern "C" int FC_CSVNeedEscapeW(
    const wchar_t* pBinStr, 
    int            iChars
);
#ifdef UNICODE
 #define FC_CSVNeedEscape FC_CSVNeedEscapeW
#else
 #define FC_CSVNeedEscape FC_CSVNeedEscapeA
#endif


#endif // __HELPER_H_


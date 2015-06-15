
#ifndef __CG_CCONSOLE_H_
#define __CG_CCONSOLE_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include <cg_console.h>
#include <fc_tools\fc_defines.h>
#include <fc_todebug\fc_assert.h>
#include <fc_tools\fc_cstring.h>
#include <fc_tools\fc_hash.h>
#include <CGT_CDiagMsg.h>
#include "loaderlist.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
#define DEFAULT_ASP_TUNINGDISPLAY_POS 0 
#define DEFAULT_ASP_SFC_POS           1

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CG_CConsole : public ICG_Console  
{
public:
    CG_CConsole();
    virtual ~CG_CConsole();

    virtual HRESULT STDMETHODCALLTYPE genConsoleForTarget(
        /*[in] */ IUnknown*  pIUnkFront,           
        /*[in] */ BSTR       sTargetInstName,     
        /*[in] */ BSTR       sResourceName,
        /*[in] */ BSTR       sWebServerAddr,
        /*[in] */ BSTR       sControlAddr,
        /*[in] */ BSTR       sTargetTypeName,
        /*[out]*/ long*      plErrors,
        /*[out]*/ long*      plWarnings
    );

    virtual HRESULT STDMETHODCALLTYPE genConsoleForLibrary(
        /*[in] */ IUnknown*  pIUnkFront,           
        /*[in] */ BSTR       sTargetTypeName,
        /*[out]*/ long*      plErrors,
        /*[out]*/ long*      plWarnings
    );

    //-------------------- IUnknown methods --------------------:
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppObject)
    {
        *ppObject = NULL;
        if (riid==IID_IUnknown || riid==IID_ICG_Console) 
        {
            *ppObject=this;
        }
        else 
        {
            return E_NOINTERFACE;
        }
        AddRef();
        return NO_ERROR;
    }
    virtual ULONG STDMETHODCALLTYPE AddRef()
    {
        g_dwRefCount++;
        m_dwRefCount++;
        return m_dwRefCount;
    }
    virtual ULONG STDMETHODCALLTYPE Release()
    {
        g_dwRefCount--;
        m_dwRefCount--;
        if (m_dwRefCount==0) 
        {
            delete this;
            return 0;
        }
        return m_dwRefCount;
    }
private:
    unsigned long       m_lConfiFilesCount;     // starts with 2 for legacy/no reasons 
    ULONG               m_dwRefCount;
    CGT_CDiagMsg        m_dm;                   // messages output

    ICG_IECFront*       m_pIFront;
    BSTR                m_sTargetInstName;
    BSTR                m_sResourceName;
    BSTR                m_sWebServerAddr;
    BSTR                m_sControlAddr;
    BSTR                m_sTargetTypeName;

    FC_CString          m_sVisPath;

    CGT_CDmp            m_dmpChildren;
    CGT_CDmp            m_dmpTaskList;

    CLoaderList         m_LoaderList;

    FC_CStrList         m_aAspFiles;            // list of asp files processed; 
                                                //    default asp-s at specific positions:
                                                //    m_aAspFiles[0] = CP_TuningDisplay.asp
                                                //    m_aAspFiles[1] = CP_SFC.asp"

    FC_CHashPtr<const CG_Symtab*, long>         // hash of pairs of symbol table - asp file, 
                        m_hSymAsp;              // long is index in m_aAspFiles

protected:
    HRESULT gen4grvFile(const TCHAR* pszSourceFile);
    HRESULT checkOutLibCommonFiles();
    
    // check out console files of a certain resource return imediately if the resource 
    // was already checked out
    HRESULT checkOutAspFiles(const CG_Symtab* pSymTab, const TCHAR** pMainAsp);

    HRESULT checkOutLibFiles(const CG_LIBContext* pLibCtx, const TCHAR* sPOUName, const TCHAR* sFileSpec);

    HRESULT genVisFiles();
    HRESULT initErrMsgSink();
    HRESULT genTaskFiles(const CG_Task* pTask);

    HRESULT genConfiFiles(int level, const CG_SymEntry* pSymEntry, FC_CGArray<const CG_SymEntry*>* pInstancePath, bool bNoAsp = false);
    HRESULT genConfiFiles(int level, const CG_Symtab*   pSymTab,   FC_CGArray<const CG_SymEntry*>* pInstancePath, bool bNoAsp = false);

    HRESULT genSymEntry(int level, const CG_SymEntry* pSymEntry,  
                        CGT_CDmp*& pDmpConfiFile, FC_CString& sConfiFileName,
                        FC_CGArray<const CG_SymEntry*>* pInstancePath, bool bNoAsp = false); 
    HRESULT genTargets();
    
    HRESULT parseConsoleSection(BSTR sResourceName = NULL);
    HRESULT addCustomFilesToConsoleLinkFile(const CG_AttribList* al);
};

class COM_CConsoleFactory : public IClassFactory 
{
// Interface
public:
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppObject)
    {
        if (riid==IID_IUnknown || riid==IID_IClassFactory) 
        {
            *ppObject=this;
        }
        else 
        {
            return E_NOINTERFACE;
        }
        AddRef();
        return NO_ERROR;
    }
    ULONG STDMETHODCALLTYPE AddRef()
    {
        g_dwRefCount++;
        m_dwRefCount++;
        return m_dwRefCount;
    }

    ULONG STDMETHODCALLTYPE Release()
    {
        g_dwRefCount--;
        m_dwRefCount--;
        if(m_dwRefCount==0) 
            delete this;
        return m_dwRefCount;
    }


    HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown *pUnkOuter, REFIID riid, void** ppObject)
    {
        if (pUnkOuter!=NULL)
            return CLASS_E_NOAGGREGATION;

        if(g_szModulePath[0]==0)
        {
            if(GetModuleFileName(g_hinstDLL, g_szModulePath, FC_ARRAY_LEN(g_szModulePath))==0)
            {
               FC_ASSERT_LASTERR(_T("GetModuleFileName"));
               return false;
            }
            FC_StringCutFilePart(g_szModulePath);
        }

        CG_CConsole* pFront = new CG_CConsole();
        if(FAILED(pFront->QueryInterface(riid, ppObject))) 
        {
           delete pFront;
           *ppObject=NULL;
           return E_NOINTERFACE;
        }
        return NO_ERROR;
    }

    HRESULT STDMETHODCALLTYPE LockServer(BOOL fLock)
    {
        if(fLock)
            g_dwRefCount++;
        else
            g_dwRefCount--;
        
        return NO_ERROR;
    }

// Implementation
private:
    ULONG m_dwRefCount;
public:
    COM_CConsoleFactory()
    {
        m_dwRefCount = 0;
    }
};


#endif // __CG_CCONSOLE_H_


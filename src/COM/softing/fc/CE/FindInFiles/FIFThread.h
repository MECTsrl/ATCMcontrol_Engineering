#ifndef __FIFTHREAD_H_
#define __FIFTHREAD_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

//----  Aggregate Includes:   --------------------------------------*
#include "ThrdTmpl.h"
#include "StdEditorIF.h"
#include "SBLLexTree.h"

//----  Forward Class Definitions:   -------------------------------*
struct IFIFMsgInternal;
//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//------------------------------------------------------------------*
/**
 *  class CEFIFExtData
 *
 *  - data class for CEFIFThread containing extension and editor guid
 *
 *  @see    CEFIFThread
 */
class CEFIFExtData
{
public:
    CString     m_strExtension;
    CString     m_strGUID;
};

typedef CList<CEFIFExtData, CEFIFExtData&>  TExtDataList;



//------------------------------------------------------------------*
/**
 *  class CCEFIFThreadData
 *
 *  - data class for CEFIFThread containing search data
 *
 *  @see    CEFIFThread
 */
class CEFIFThreadData
{
public:
    CEFIFThreadData()
    {
    }

    void Reset() { m_tExtDataList.RemoveAll(); };

public:
    CString                     m_strExpression;
    CString                     m_strDirectory;
    DWORD                       m_dwFlags;
    TExtDataList                m_tExtDataList;
};



//------------------------------------------------------------------*
/**
 *  class CCEFIFThread
 *
 *  - thread object for the execution of a single search command
 *  - derived from receiver thread CSigThread<>
 *  - start / stop search
 *  - redirect messages to message view using CEFIFMsgThread
 *
 *  @see    CSigThread, CEFIFMsgThread
 */
class CEFIFThread : public CSigThread<CEFIFThreadData>
{
public:
    CEFIFThread();
    virtual ~CEFIFThread();

    virtual int OnInitialize();
    virtual int OnExit();
    virtual int Schedule();

    BOOL    Stop();
    void    Break();
    void    Continue();

    BOOL    IsThreadRunning();

    int     GetMatchCount();

protected:
    void ReleaseEditInterfaces();

    HRESULT FindInFiles(    const CString& crstrDirectory,
                            const CString& crstrExtension,
                            const CString& crstrGUID,
                            const CString& crstrExpression,
                            DWORD dwFlags,
                            int& riMatchCount);

    HRESULT FindStringInFile(   const CString& crstrFilePathName,
                                const CString& crstrExpression,
                                const CString& crstrGUID,
                                DWORD dwFlags, 
                                int& riMatchCount);

    HRESULT FindStringInTextFile(  const CString& crstrFilePathName,
                                    const CString& crstrExpression,
                                    DWORD dwFlags, 
                                    int& riMatchCount);


    ICEEdit* GetEditInterface(const CString& crstrGUID);
    ICEEdit* CreateEdit(const CString& crstrGUID);

    void OutputResult();


public:
    IStream*                m_pIStream;

protected:
    IFIFMsgInternal*        m_pIFIFMsg;
    HANDLE                  m_hRunEvent;
    CSBLLexTree<ICEEdit>    m_tEditTree;
    BOOL                    m_bExit;
    int                     m_iMatchCount;
};



#endif // __FIFTHREAD_H_
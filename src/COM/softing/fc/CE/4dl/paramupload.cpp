


//----  Includes:   -------------------------------------------*
#include "helper.h"


static bool UpdateILValues( 
    const CParamList*   pPL,
    const OPCITEMSTATE* pItemVAL,
    const HRESULT*      pError,
    const OPCITEMDEF*   opcItems,
    int                 nItems,
    int                 nVerbose
)
{
    int          i;
    bool         bOk;
    CVarItem*    pItem;
    bool         bBadItem = false;
    FC_CString   strValue(MAX_PATH);
    FC_CString   strItemID(MAX_PATH);
    FC_CString   resBeg(MAX_PATH);
    int          nTab;

    resBeg<<pPL->getResName()<<_T(".");
    nTab = pPL->getMaxPathLen()+1;


    for(i=0; i<nItems; i++)
    {
        if(g_bForceShutdown)
            return false;

        pItem = (CVarItem*)opcItems[i].hClient;

        if(FAILED(pError[i]))
        {
            strItemID.load(opcItems[i].szItemID);
            _tprintf(_T("4dl: error: upload '%s': read value failed: hr=0x%x\n"), 
               strItemID.psz(), pError[i]);
            bBadItem = true;
            break;
        }
        else if(pItemVAL[i].wQuality<192)
        {
            strItemID.load(opcItems[i].szItemID);
            _tprintf(_T("4dl: error: upload '%s': read value failed: bad OPC Quality=0x%x)\n"), 
               strItemID.psz(), pItemVAL[i].wQuality);
            bBadItem = true;
            break;
        }
        
        //copy to item date, clear dest first (may be second read on same value ?)
        if(VariantToBinVal(&pItemVAL[i].vDataValue, pItem->m_typ, &pItem->value))
        {
            if(nVerbose>=2)
            {
                strItemID.load(opcItems[i].szItemID);
                bOk = strItemID.stripBeginI(resBeg);
                assert(bOk);
                VariantToString(&pItemVAL[i].vDataValue, pItem->m_typ, &strValue, true);
                strItemID.tab(nTab)<<_T(" := ")<<strValue;
                _tprintf(_T("4dl: read: %s\n"), strItemID.psz());
            }
        }
        else
        {
            bBadItem = true;
            strItemID.load(opcItems[i].szItemID);
            _tprintf(_T("4dl: internal error: upload: '%s': 'VariantToBinVal' failed\n"), 
               strItemID.psz());
            break;
        }
    }

    return !bBadItem;
}



static bool CheckDownloadResult(
    const CParamList*   pPL,
    const HRESULT*      pError,
    const OPCITEMDEF*   opcItems,
    int                 nItems,
    int                 nVerbose
)
{
    int          i;
    CVarItem*    pItem;
    bool         bBadItem = false;
    FC_CString   strValue(MAX_PATH);
    FC_CString   strItemID(MAX_PATH);
    FC_CString   resBeg(MAX_PATH);
    int          nTab;

    resBeg<<pPL->getResName()<<_T(".");
    nTab = pPL->getMaxPathLen()+1;

    for(i=0; i<nItems; i++)
    {
        if(g_bForceShutdown)
            return false;

        pItem = (CVarItem*)opcItems[i].hClient;

        if(FAILED(pError[i]))
        {
            strItemID.load(opcItems[i].szItemID);
            bBadItem = true;
            _tprintf(_T("4dl: error: download: '%s': write value failed: hr=0x%x\n"), 
                strItemID.psz(), pError[i]);
            break;
        }
        else if(nVerbose>=2)
        {
            strItemID.load(opcItems[i].szItemID);
            bool bOk = strItemID.stripBeginI(resBeg);
            assert(bOk);
            VARIANT tmpvar;
            VariantInit(&tmpvar);
            BinValToVariant(&pItem->value, &tmpvar);
            VariantToString(&tmpvar, pItem->m_typ, &strValue, true);
            VariantClear(&tmpvar);
            strItemID.tab(nTab)<<_T(" := ")<<strValue;
            _tprintf(_T("4dl: write: %s\n"), strItemID.psz());
        }
    }

    return !bBadItem;
}




static bool TransferPLChunk(
    IOPCItemMgt*        pItemMgt, 
    IOPCSyncIO*         pSyncIO,
    bool                bDown,
    CParamList*         pPL,
    int                 nItems,
    CVarItem*           pItems[/*nItems*/],
    int                 nVerbose
)
{
    int                  i;
    HRESULT              hr;
    OPCITEMDEF           opcItems[MAX_GROUP_SIZE];
    OPCHANDLE            hServer[MAX_GROUP_SIZE];
    OPCITEMRESULT*       pResultAI = NULL;
    bool                 bOk = false;
    FC_CString           jot(MAX_PATH);


    if(nItems>MAX_GROUP_SIZE)
    {
        _tprintf(_T("4dl: internal error 'nItems>MAX_GROUP_SIZE'\n"));
        return false;
    }


    for(i=0; i<nItems; i++)
    {
        opcItems[i].szAccessPath = NULL;
        pPL->buildFullItemPath(pItems[i], &jot, &opcItems[i].szItemID);
        opcItems[i].bActive = false;
        opcItems[i].dwBlobSize = 0;
        opcItems[i].pBlob = NULL;
        opcItems[i].vtRequestedDataType = BLTypToVariantTyp(pItems[i]->m_typ);
        opcItems[i].wReserved = 0;
        opcItems[i].hClient = (OPCHANDLE)pItems[i];
    }



    {
        HRESULT* pErrorAI  = NULL;
        hr = pItemMgt->AddItems(nItems, opcItems, &pResultAI, &pErrorAI);
        if(FAILED(hr))
        {
            _tprintf(_T("4dl: error: upload: 'AddItems' failed: hr=0x%x\n"), hr);
            bOk = false;        
        }
        //pItemMgt->AddItems  was (partially !! ok hr may be S_FALSE too):
        hr = S_OK;
        bOk = true;
        for(i=0; i<nItems; i++)
        {
            if(FAILED(pErrorAI[i]))
            {
                bOk = false;
                jot.load(opcItems[i].szItemID);
                _tprintf(_T("%s(%d): error: OPC AddItems failed for '%s' hr=0x%x\n"), 
                    pPL->getFileName(), pItems[i]->nLineNo, jot.psz(), pErrorAI[i]);
                break;
            }
            hServer[i] = pResultAI[i].hServer;
        }
        CoTaskMemFree(pErrorAI);
        pErrorAI = NULL;
    }



    if(bOk && bDown)
    {
        VARIANT values[MAX_GROUP_SIZE];

        bOk = true;
        for(i=0; i<nItems; i++)
        {
            if(pItems[i]->value.typ == BL_ANY)
            {
                bOk = false;
                jot.load(opcItems[i].szItemID);
                _tprintf(_T("%s(%d): internal error: write item '%s': item has no value\n"), 
                    pPL->getFileName(), pItems[i]->nLineNo, jot.psz());

                for(int j=0; j<i; j++)
                    VariantClear(&values[j]);
                break;
            }
            VariantInit(&values[i]);
            BinValToVariant(&pItems[i]->value, &values[i]);
        }

        if(bOk)
        {
            HRESULT* pErrorW = NULL;
            hr = pSyncIO->Write(nItems, hServer, values, &pErrorW);
            for(int j=0; j<nItems; j++)
                VariantClear(&values[j]);

            if(FAILED(hr))
            {
                bOk = false;
                _tprintf(_T("4dl: error: download parameter: 'sync OPC Write' failed: hr=0x%x\n"), hr);
            }
            else
            {
                bOk = CheckDownloadResult(pPL, pErrorW, opcItems, nItems, nVerbose);
            }   
            if(pErrorW)
                CoTaskMemFree(pErrorW);
        }
    }
    else if(bOk)
    {
        OPCITEMSTATE* pItemVAL = NULL;
        HRESULT*      pErrorR = NULL;
        hr = pSyncIO->Read(OPC_DS_DEVICE, nItems, hServer, &pItemVAL, &pErrorR);
    
        if(SUCCEEDED(hr))
        {
            bOk = UpdateILValues(pPL, pItemVAL, pErrorR, opcItems, nItems, nVerbose);
        }
        else
        {
            bOk = false;
            _tprintf(_T("4dl: error: upload: 'sync OPC read' failed: hr=0x%x\n"), hr);
        }   
        if(pErrorR)
            CoTaskMemFree(pErrorR);
        
        if(pItemVAL)
        {
            for(i=0; i<nItems; i++)
                VariantClear(&pItemVAL[i].vDataValue);

            CoTaskMemFree(pItemVAL);
            pItemVAL = NULL;
        }
    }



    {
        HRESULT* pErrorRI = NULL;    
        hr = pItemMgt->RemoveItems(nItems, hServer, &pErrorRI);
        //ignore this hr...
        if(pErrorRI)
            CoTaskMemFree(pErrorRI);
    }    

    //clean up mem:
    for(i=0; i<nItems; i++)
        delete(opcItems[i].szItemID);

    if(pResultAI)
        CoTaskMemFree(pResultAI);

    return bOk;
}





static bool TransferPL(
    const CConnection*   pCnt, 
    CParamList*          pPL,
    bool                 bDown,
    int                  nVerbose,
    int                  nMaxGS
)
{
    CVarItem*           pItem;
    CVarItem*           pItems[MAX_GROUP_SIZE];
    IOPCSyncIO*         pSyncIO;
    IOPCItemMgt*        pItemMgt; 
    bool                bOk;
    ITEMPOS_IT          pos;
    int                 ig;
  
    if(nMaxGS==0)
        nMaxGS = 1;
    else if(nMaxGS>MAX_GROUP_SIZE)
        nMaxGS = MAX_GROUP_SIZE;

    pCnt->getRawIOPCServer(&pSyncIO, &pItemMgt);
    if(!pSyncIO || !pItemMgt)
    {
        assert(!"not connected");
        return false;
    }

    pItem = pPL->getFirstVar(&pos);
    if(!pItem)
    {
        _tprintf(_T("4dl: warning: param download '%s': empty paramter list\n"),
            pPL->getFileName());
        return true;
    }

    bOk = true;
    ig = 0;
    while(pItem && bOk)
    {
        if(g_bForceShutdown)
        {
            bOk = false;
            break;
        }

        pItems[ig++] = pItem;
        pItem = pPL->getNextVar(&pos);
        if(ig>=nMaxGS || !pItem)
        {
            bOk = TransferPLChunk(pItemMgt, pSyncIO, bDown, pPL, ig, pItems, nVerbose);
            ig = 0;
        }
    }
    return bOk;
}





int UploadParameter(
    CConnection*       pCnt,
    const TCHAR*       pszParamFile,
    const TCHAR*       pszDestFile,
    int                nVerbose,
    int                nMaxGS
)
{
    int            iret;
    FC_CString     tmpName(MAX_PATH);
    CParamList     list;

    //open sessions first:
    if(!pCnt->openRawComSession() || !pCnt->openRawVarSession())
        return -1;

    tmpName.load(_T("$tmp_"))<<pszParamFile;


    if(nVerbose>=1)
        _tprintf(_T("4dl: upload list   : '%s'\n"), pszParamFile);

    iret = UploadBinaryDomain(pCnt, pszParamFile, NULL, NULL, tmpName);
    if(iret<=0)
        return iret;
    
    if(!list.load(tmpName,  NULL,  0))
        return -1; //want to see the tmp file after error...

    if(nVerbose>=1)
        _tprintf(_T("4dl: upload values : %d items\n"), list.getItemCount());

    if(!TransferPL(pCnt, &list, false, nVerbose, nMaxGS))
    {
        //want to see the tmp file after error but not if Ctrl-C
        if(g_bForceShutdown)
            _tunlink(tmpName);//no is ok...
        return -1; 
    }

    _tunlink(tmpName);//no is ok...

    if(list.dump(pszDestFile, true, pCnt->getControlAddr()))
    {
        if(nVerbose>=1)
            _tprintf(_T("4dl: dump values to: '%s'\n"), pszDestFile);
        return 1;
    }

    return -1;
}



bool MergeTargetParameterFiles(
    const TCHAR*        pszTrgName, 
    const TCHAR*        pszLoaderList, 
    const FC_CStrList*  pParamFilesOld,
    FC_CStrList*        pParamFilesMgt,
    bool                bWarn,
    int                 nVerbose,
    const TCHAR*        pszMergeDest,
    FC_CStrList*        pDeleteFileList
)
{
    int            ip;
    CParamList     list4CG;
    CParamList     listOld;
    CParamList     listMgt;
    FC_CString     str4PLNoPathNoExt(MAX_PATH);
    FC_CString     strTrgNameLo(MAX_PATH);
    FC_CString     strTrgDir(MAX_PATH);
    FC_CString     str4CG4PL(MAX_PATH);
    const TCHAR*   pszOld4PL;
    FC_CString     strMgt4PL(MAX_PATH);
    
    pParamFilesMgt->clear();
    strTrgDir.load(pszLoaderList).stripFilePart();
    strTrgNameLo.load(FC_StringGetFilePart(strTrgDir)).toLower();

    if(pParamFilesOld->getUsed()==0)
        return true;

    if(nVerbose>=1)
        _tprintf(_T("4dl: start merge parameter lists\n"));

    if(_tcsicmp(strTrgNameLo, pszTrgName))
    {
        _tprintf(_T("4dl: internal error: file name mismatch\n"));
        return false;
    }

    for(ip=0; ip<pParamFilesOld->getUsed(); ip++)
    {
        pszOld4PL = pParamFilesOld->get(ip); //may have a path! (usually has...)
        str4PLNoPathNoExt.load(FC_StringGetFilePart(pszOld4PL));
         
        str4PLNoPathNoExt.changeFileExt(NULL);
        str4PLNoPathNoExt.changeFileExt(NULL);//HACK need the file name as extra info
                                              //on input we have maybe retain.4pl.tmp 
                                              //need to strip tmp and 4pl


        //HACK: better pars the loader list file ?... we simply look on disk
        //this is ok as long as the compiler cleans up the folder and nobody
        //copies 4pls into it !
        str4CG4PL.load(strTrgDir)<<_T("\\")<<str4PLNoPathNoExt<<_T(".4pl");


        if(!FC_Exist(str4CG4PL))
        {
            _tprintf(_T("4dl: warning: merge 4PL: target '%s' has no parameter file named '%s'\n"),
                     pszTrgName, FC_StringGetFilePart(str4CG4PL));
            continue;
        }


        if(pszMergeDest && pszMergeDest[0])
            strMgt4PL.load(pszMergeDest)<<_T("\\")<<str4PLNoPathNoExt<<_T(".merged.4pl");
        else
            strMgt4PL.load(str4PLNoPathNoExt)<<_T(".merged.4pl.tmp");

        if(!list4CG.load(str4CG4PL,  NULL,  0))
            return false;

        if(!listOld.load(pszOld4PL,  NULL,  0))
            return false;
        
        if(nVerbose>=1)
        {
            _tprintf(_T(" list  : '%s'\n"), str4CG4PL.psz());
            _tprintf(_T(" values: '%s'\n"), pszOld4PL);
            _tprintf(_T(" result: '%s'\n"), strMgt4PL.psz());
        }

        if(!list4CG.merge(&listOld, bWarn, 0))
            return false;
    
       
        if(!list4CG.dump(strMgt4PL, false, NULL))
            return false;

        pParamFilesMgt->add(strMgt4PL);
        if(!pszMergeDest || !pszMergeDest[0])
            pDeleteFileList->add(strMgt4PL);
    }

    return true;
}



bool DownloadParameter(
    CConnection*       pCnt,
    const TCHAR*       pszParamFile,
    int                nVerbose,
    int                nMaxGS
)
{
    CParamList     list;

    //open sessions first:
    if(!pCnt->openRawVarSession())
        return false;

    if(!list.load(pszParamFile,  NULL,  0))
        return false;

    if(nVerbose>=1)
        _tprintf(_T("4dl: start download parameter: '%s'\n"), pszParamFile);

    if(!TransferPL(pCnt, &list, true, nVerbose, nMaxGS))
        return false;

    return true;
}



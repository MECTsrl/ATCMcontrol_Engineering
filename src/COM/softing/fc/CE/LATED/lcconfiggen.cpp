
//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "lcconfiggen.h"

// caution: each entry must correspond to the enum entry in ETargetType
const char* ccaTargetPrefixes[] =
{
	_T("PC"),	// eTargetPC
	_T("EC"),	// eTargetEC
	_T("CMZ")	// eTargetCMZ
};
//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


CLCGen::CLCGen() {
    m_pfGenDTypeCMZ = NULL;
    m_pfGenDTypeEC = NULL;
    m_pfGenDTypePC = NULL;
    m_pfGenDConst = NULL;
    m_pfGenDStuffRecToCache = NULL;
    m_pfGenDStuffConst = NULL;
    m_pfGenDStuffUser = NULL;
    m_pfGenDStuffCacheType = NULL;
    m_pfGenDStuffGvl = NULL;
    m_pfGenDCMZSend = NULL;
    m_pfGenDECSend = NULL;
    m_pfGenDPCSend = NULL;
    m_pfGenDCMZRec = NULL;
    m_pfGenDECRec = NULL;
    m_pfGenDPCRec = NULL;

    m_pfGenLAddr = NULL;
	m_pfGenLCMZSendPrg = NULL;
    m_pfGenLECSendPrg = NULL;
    m_pfGenLPCSendPrg = NULL;
	m_pfGenLCMZRecPrg = NULL;
    m_pfGenLECRecPrg = NULL;
    m_pfGenLPCRecPrg = NULL;
    m_pfGenLGvlPrgCMZ = NULL;
    m_pfGenLGvlPrgEC = NULL;
    m_pfGenLGvlPrgPC = NULL;

    m_pConfig = NULL;

    m_bPCSend = false;
    m_bPCRec  = false;
    m_bECSend = false;
    m_bECRec  = false;
	m_bCMZSend = false;
	m_bCMZRec = false;
    m_bAnyPCRec = false;

}

void CLCGen::Generate(CLCConfig* pConfig)
{
    HRESULT hr = S_OK;
    CString strFileName;

    m_pConfig = pConfig;

    try {
        //add the this object to the config listeners.
        pConfig->AddConfigListener(this);
    
        m_strPrjPath = pConfig->GetPath();
        int i = m_strPrjPath.ReverseFind('\\');
        int j = m_strPrjPath.GetLength() - i;
        m_strPrjPath.Delete(i, j);

        // open the files to generate from link
        CString strShortFileName = _T("LCAddress.cst");
        strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileName);
        m_pfGenLAddr = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
        if(!m_pfGenLAddr) {
            AfxThrowMemoryException();        
        }

        CLCGenObj genObj;
        genObj.InitFile(strShortFileName);
        pConfig->AddGenObj(genObj);

        // add the libraries needed
        strShortFileName = _T("$(LIBDIR)\\4C_Utility.4cl");
        genObj.InitFile(strShortFileName);
        m_pConfig->AddGenObj(genObj);
        strShortFileName = _T("$(LIBDIR)\\4C_Communication.4cl");
        genObj.InitFile(strShortFileName);
        m_pConfig->AddGenObj(genObj);
        strShortFileName = _T("$(LIBDIR)\\4CEC_Communication.4cl");
        genObj.InitFile(strShortFileName);
        m_pConfig->AddGenObj(genObj);

        // generate the starting parts from link
        GenLAddrBeg(m_pfGenLAddr);

        // first reset used info of all datatypes!)
        pConfig->TraverseDatatypes(GenFromDataReset);
        
        // do all generations from link
        pConfig->TraverseLinks(GenFromLink);
        
        // do all generations on data (first generate from link!)
        pConfig->TraverseDatatypes(GenFromData);

        // generate the end parts from 
        GenLAddrEnd(m_pfGenLAddr);

        // close the files to generate
        if(m_pfGenLAddr) {
            m_pfGenLAddr->Close();
            delete(m_pfGenLAddr);
            m_pfGenLAddr = NULL;
        }

        pConfig->GenObjsExecute();

        pConfig->RemoveConfigListener(this);
    }
    catch(CMemoryException* e) {
        e->Delete();
        hr = E_OUTOFMEMORY;
    }
    catch(HRESULT hrRes) {
        hr = hrRes;
    }

    m_pConfig = NULL;

    return;
}

void CLCGen::OnDataType(int id, CLCDataType *pDataType)
{
    CString strFileName;
    CString strName;
    CString strShortFileName;
    CFileStatus rStatus;
    CLCGenObj genObj;
    bool bDoNotGenerate = false;
    
    try {
        pDataType->GetName(strName);
        switch(id) {
            case GenFromDataReset:
                pDataType->SetUsedInfo(false, false, false);
                break;
            case GenFromData:
                // open the files to generate
                if(pDataType->IsCMZUsed()) {
                    strShortFileName.Format(_T("LCCMZ_%s.sdt"), strName);
                    strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileName);
                    m_pfGenDTypeCMZ = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                    if(!m_pfGenDTypeCMZ) {
                        AfxThrowMemoryException();        
                    }
                    genObj.InitFile(strShortFileName);
                    m_pConfig->AddGenObj(genObj);
                }
                if(pDataType->IsECUsed()) {
                    strShortFileName.Format(_T("LCEC_%s.sdt"), strName);
                    strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileName);
                    m_pfGenDTypeEC = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                    if(!m_pfGenDTypeEC) {
                        AfxThrowMemoryException();        
                    }
                    genObj.InitFile(strShortFileName);
                    m_pConfig->AddGenObj(genObj);
                }
                if(pDataType->IsPCUsed()) {
                    strShortFileName.Format(_T("LCPC_%s.sdt"), strName);
                    strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileName);
                    m_pfGenDTypePC = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                    if(!m_pfGenDTypePC) {
                        AfxThrowMemoryException();        
                    }
                    genObj.InitFile(strShortFileName);
                    m_pConfig->AddGenObj(genObj);
                }

                strShortFileName.Format(_T("LC_%s.cst"), strName);
                strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileName);
                m_pfGenDConst = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                if(!m_pfGenDConst) {
                    AfxThrowMemoryException();        
                }
                genObj.InitFile(strShortFileName);
                m_pConfig->AddGenObj(genObj);

                strShortFileName = _T("LC.cst");
                strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileName);
                m_pfGenDStuffConst = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                if(!m_pfGenDStuffConst) {
                    AfxThrowMemoryException();        
                }
                genObj.InitFile(strShortFileName);
                m_pConfig->AddGenObj(genObj);

                strShortFileName = _T("LC_User.cst");
                strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileName);
                // check if exist
                if(!CFile::GetStatus(strFileName, rStatus)) {
                    m_pfGenDStuffUser = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                    if(!m_pfGenDStuffUser) {
                        AfxThrowMemoryException();        
                    }
                } else {
                    bDoNotGenerate = true;
                }
                genObj.InitFile(strShortFileName);
                m_pConfig->AddGenObj(genObj);

                if(m_bAnyPCRec) {
                    strShortFileName = _T("LCPCRecToCache.st");
                    strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileName);
                    m_pfGenDStuffRecToCache = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                    if(!m_pfGenDStuffRecToCache) {
                        AfxThrowMemoryException();        
                    }
                    genObj.InitFile(strShortFileName);
                    m_pConfig->AddGenObj(genObj);
                
                    strShortFileName = _T("LCCacheData.sdt");
                    strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileName);
                    m_pfGenDStuffCacheType = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                    if(!m_pfGenDStuffCacheType) {
                        AfxThrowMemoryException();        
                    }
                    genObj.InitFile(strShortFileName);
                    m_pConfig->AddGenObj(genObj);

                    strShortFileName = _T("LCPC.gvl");
                    strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileName);
                    m_pfGenDStuffGvl = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                    if(!m_pfGenDStuffGvl) {
                        AfxThrowMemoryException();        
                    }
                    genObj.InitFile(strShortFileName);
                    m_pConfig->AddGenObj(genObj);
                }

                if(pDataType->IsCMZUsed()) {
                    strShortFileName.Format(_T("LCES_%s.st"), strName);
                    strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileName);
                    m_pfGenDCMZSend = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                    if(!m_pfGenDCMZSend) {
                        AfxThrowMemoryException();        
                    }
                    genObj.InitFile(strShortFileName);
                    m_pConfig->AddGenObj(genObj);

                    strShortFileName.Format(_T("LCER_%s.st"), strName);
                    strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileName);
                    m_pfGenDCMZRec = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                    if(!m_pfGenDCMZRec) {
                        AfxThrowMemoryException();        
                    }
                    genObj.InitFile(strShortFileName);
                    m_pConfig->AddGenObj(genObj);
                }

                if(pDataType->IsECUsed()) {
                    strShortFileName.Format(_T("LCES_%s.st"), strName);
                    strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileName);
                    m_pfGenDECSend = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                    if(!m_pfGenDECSend) {
                        AfxThrowMemoryException();        
                    }
                    genObj.InitFile(strShortFileName);
                    m_pConfig->AddGenObj(genObj);

                    strShortFileName.Format(_T("LCER_%s.st"), strName);
                    strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileName);
                    m_pfGenDECRec = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                    if(!m_pfGenDECRec) {
                        AfxThrowMemoryException();        
                    }
                    genObj.InitFile(strShortFileName);
                    m_pConfig->AddGenObj(genObj);
                }

                if(pDataType->IsPCUsed()) {
                    strShortFileName.Format(_T("LCPS_%s.st"), strName);
                    strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileName);
                    m_pfGenDPCSend = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                    if(!m_pfGenDPCSend) {
                        AfxThrowMemoryException();        
                    }
                    genObj.InitFile(strShortFileName);
                    m_pConfig->AddGenObj(genObj);

                    strShortFileName.Format(_T("LCPR_%s.st"), strName);
                    strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileName);
                    m_pfGenDPCRec = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                    if(!m_pfGenDPCRec) {
                        AfxThrowMemoryException();        
                    }
                    genObj.InitFile(strShortFileName);
                    m_pConfig->AddGenObj(genObj);
                }

                // generate the starting part
                if(pDataType->IsCMZUsed()) {
                    GenDTypeBeg(m_pfGenDTypeCMZ, strName, eTargetCMZ);
                    GenDCMZSendBeg(m_pfGenDCMZSend, strName);
                    GenDCMZRecBeg(m_pfGenDCMZRec, strName);
                }
                if(pDataType->IsECUsed()) {
                    GenDTypeBeg(m_pfGenDTypeEC, strName, eTargetEC);
                    GenDECSendBeg(m_pfGenDECSend, strName);
                    GenDECRecBeg(m_pfGenDECRec, strName);
                }
                if(pDataType->IsPCUsed()) {
                    GenDTypeBeg(m_pfGenDTypePC, strName, eTargetPC);
                    GenDPCSendBeg(m_pfGenDPCSend, strName);
                    GenDPCRecBeg(m_pfGenDPCRec, strName);
                }
                GenDConst(m_pfGenDConst, pDataType);
                GenDStuffConst(m_pfGenDStuffConst);
                // check if exist
                if(!bDoNotGenerate) {
                    GenDStuffUser(m_pfGenDStuffUser);
                }
                if(m_bAnyPCRec) {
                    GenDStuffRecToCache(m_pfGenDStuffRecToCache);
                    GenDStuffCacheType(m_pfGenDStuffCacheType);
                    GenDStuffGvl(m_pfGenDStuffGvl);
                }
                break;
            default:
                break;
        }
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return;
}

void CLCGen::OnEndDataType(int id, CLCDataType *pDataType) 
{
    CString strName;

    try {
        pDataType->GetName(strName);
        switch(id) {
            case GenFromData:
                // generate the end part
                if(pDataType->IsCMZUsed()) {
                    GenDTypeEnd(m_pfGenDTypeCMZ);
                    GenDCMZSendEnd(m_pfGenDCMZSend, strName);
                    GenDCMZRecEnd(m_pfGenDCMZRec, strName);
                }
                if(pDataType->IsECUsed()) {
                    GenDTypeEnd(m_pfGenDTypeEC);
                    GenDECSendEnd(m_pfGenDECSend, strName);
                    GenDECRecEnd(m_pfGenDECRec, strName);
                }
                if(pDataType->IsPCUsed()) {
                    GenDTypeEnd(m_pfGenDTypePC);
                    GenDPCSendEnd(m_pfGenDPCSend, strName);
                    GenDPCRecEnd(m_pfGenDPCRec, strName);
                }

                // close the files to generate
                if(m_pfGenDTypeCMZ) {
                    m_pfGenDTypeCMZ->Close();
                    delete(m_pfGenDTypeCMZ);
                    m_pfGenDTypeCMZ = NULL;
                }
                if(m_pfGenDTypeEC) {
                    m_pfGenDTypeEC->Close();
                    delete(m_pfGenDTypeEC);
                    m_pfGenDTypeEC = NULL;
                }
                if(m_pfGenDTypePC) {
                    m_pfGenDTypePC->Close();
                    delete(m_pfGenDTypePC);
                    m_pfGenDTypePC = NULL;
                }
                if(m_pfGenDConst) {
                    m_pfGenDConst->Close();
                    delete(m_pfGenDConst);
                    m_pfGenDConst = NULL;
                }
                if(m_pfGenDStuffRecToCache) {
                    m_pfGenDStuffRecToCache->Close();
                    delete(m_pfGenDStuffRecToCache);
                    m_pfGenDStuffRecToCache = NULL;
                }
                if(m_pfGenDStuffConst) {
                    m_pfGenDStuffConst->Close();
                    delete(m_pfGenDStuffConst);
                    m_pfGenDStuffConst = NULL;
                }
                if(m_pfGenDStuffUser) {
                    m_pfGenDStuffUser->Close();
                    delete(m_pfGenDStuffUser);
                    m_pfGenDStuffUser = NULL;
                }
                if(m_pfGenDStuffCacheType) {
                    m_pfGenDStuffCacheType->Close();
                    delete(m_pfGenDStuffCacheType);
                    m_pfGenDStuffCacheType = NULL;
                }
                if(m_pfGenDStuffGvl) {
                    m_pfGenDStuffGvl->Close();
                    delete(m_pfGenDStuffGvl);
                    m_pfGenDStuffGvl = NULL;
                }
                if(m_pfGenDCMZSend) {
                    m_pfGenDCMZSend->Close();
                    delete(m_pfGenDCMZSend);
                    m_pfGenDCMZSend = NULL;
                }
                if(m_pfGenDECSend) {
                    m_pfGenDECSend->Close();
                    delete(m_pfGenDECSend);
                    m_pfGenDECSend = NULL;
                }
                if(m_pfGenDPCSend) {
                    m_pfGenDPCSend->Close();
                    delete(m_pfGenDPCSend);
                    m_pfGenDPCSend = NULL;
                }
                if(m_pfGenDCMZRec) {
                    m_pfGenDCMZRec->Close();
                    delete(m_pfGenDCMZRec);
                    m_pfGenDCMZRec = NULL;
                }
                if(m_pfGenDECRec) {
                    m_pfGenDECRec->Close();
                    delete(m_pfGenDECRec);
                    m_pfGenDECRec = NULL;
                }
                if(m_pfGenDPCRec) {
                    m_pfGenDPCRec->Close();
                    delete(m_pfGenDPCRec);
                    m_pfGenDPCRec = NULL;
                }
                break;
            default:
                break;
        }
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return;
}

void CLCGen::OnMember(int id, CLCDataType *pDataType, CLCMember*pMember)
{
    CString strDataName;
    CString strMemberName;
    CString strType;

    pDataType->GetName(strDataName);
    pMember->GetName(strMemberName);
    BL_IEC_TYP type = pMember->GetType();
    if(type == BL_DT) {
        strType = _T("DT");
    } else {
        strType = BL_DatatypeToTypename(type, 0);
    }

    switch(id) {
        case GenFromData:
            // generate the member part
            if(pDataType->IsCMZUsed()) {
                GenDTypeElem(m_pfGenDTypeCMZ, strMemberName, strType, type, pMember->IsSubscript(),pMember->GetDataLen(), eTargetCMZ);
                GenDCMZSendElem(m_pfGenDCMZSend, strDataName, strMemberName, strType, type);
                GenDCMZRecElem(m_pfGenDCMZRec, strDataName, strMemberName, strType, type, pMember->GetDataLen());
            }
            if(pDataType->IsECUsed()) {
                GenDTypeElem(m_pfGenDTypeEC, strMemberName, strType, type, pMember->IsSubscript(),pMember->GetDataLen(), eTargetEC);
                GenDECSendElem(m_pfGenDECSend, strDataName, strMemberName, strType, type);
                GenDECRecElem(m_pfGenDECRec, strDataName, strMemberName, strType, type, pMember->GetDataLen());
            }
            if(pDataType->IsPCUsed()) {
                if(type == BL_STRING) {
                    strType = BL_DatatypeToTypename(BL_WSTRING, 0);
                }
                GenDTypeElem(m_pfGenDTypePC, strMemberName, strType, type, pMember->IsSubscript(),pMember->GetDataLen(), eTargetPC);
                GenDPCSendElem(m_pfGenDPCSend, strDataName, strMemberName, strType, type, pMember->GetDataLen());
                GenDPCRecElem(m_pfGenDPCRec, strDataName, strMemberName, strType, type, pMember->GetDataLen());
            }
            break;
        default:
            break;
    }
    return;
}

void CLCGen::OnLink(int id, CLCLink *pLink)
{
    CString strName;
    CString strTargetType;
    CString strIPAddr;
    CString strFileName;
    CString strDataType;
    CString strResource;
    CLCDataType *pDataType;
    CString strShortFileName;
    CString strShortFileNamePCGvl;
    CString strShortFileNameECGvl;
    CString strShortFileNameCMZGvl;
    CLCDestLink *pDest;
    CLCGenObj genObj;


    if(pLink->GetConnType() != P2P) {
        // no action for Publisher / Subscriber at the moment
        return;
    }
    try {
        strName = pLink->GetTargetName();
        pLink->GetIP(strIPAddr);
        pDataType = pLink->GetDataType();
        if(!pDataType) {
            ASSERT(pDataType);
            return;
        }
        strTargetType = pLink->GetTargetType();
        pDataType->GetName(strDataType);
        m_bPCSend = false;
        m_bPCRec  = false;
        m_bECSend = false;
        m_bECRec  = false;
		m_bCMZSend = false;
		m_bCMZRec = false;
        if(!strTargetType.CompareNoCase(LATED_TARGET_TYPE_PC)) {
            m_bPCSend = true;
        }
        if(!strTargetType.CompareNoCase(LATED_TARGET_TYPE_EC)) {
            m_bECSend = true;
        }
		if(!strTargetType.CompareNoCase(LATED_TARGET_TYPE_CMZ)) {
			m_bCMZSend = true;
		}

        m_bPCRec  = pLink->ExistsDestOfType(LATED_TARGET_TYPE_PC);
        m_bECRec  = pLink->ExistsDestOfType(LATED_TARGET_TYPE_EC);
		m_bCMZRec = pLink->ExistsDestOfType(LATED_TARGET_TYPE_CMZ);
        m_bAnyPCRec |= m_bPCRec;
        pDataType->SetUsedInfo(m_bPCSend || m_bPCRec, m_bECSend || m_bECRec, m_bCMZSend || m_bCMZRec);
        pDataType->Release();
        switch(id) {
            case GenFromLink: {
                // open the files to generate
                if(m_bCMZSend) {
                    strShortFileName.Format(_T("LCCSP_%s%s.st"), strName, strDataType);
                    strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileName);
                    m_pfGenLCMZSendPrg = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                    if(!m_pfGenLCMZSendPrg) {
                        AfxThrowMemoryException();        
                    }
                    genObj.InitFile(strShortFileName);
                    m_pConfig->AddGenObj(genObj);
                }

                if(m_bECSend) {
                    strShortFileName.Format(_T("LCESP_%s%s.st"), strName, strDataType);
                    strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileName);
                    m_pfGenLECSendPrg = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                    if(!m_pfGenLECSendPrg) {
                        AfxThrowMemoryException();        
                    }
                    genObj.InitFile(strShortFileName);
                    m_pConfig->AddGenObj(genObj);
                }

                if(m_bPCSend) {
                    strShortFileName.Format(_T("LCPSP_%s%s.st"), strName, strDataType);
                    strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileName);
                    m_pfGenLPCSendPrg = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                    if(!m_pfGenLPCSendPrg) {
                        AfxThrowMemoryException();        
                    }
                    genObj.InitFile(strShortFileName);
                    m_pConfig->AddGenObj(genObj);
                }

                if(m_bCMZRec) {
                    strShortFileName.Format(_T("LCCRP_%s%s.st"), strName, strDataType);
                    strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileName);
                    m_pfGenLCMZRecPrg = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                    if(!m_pfGenLCMZRecPrg) {
                        AfxThrowMemoryException();        
                    }
                    genObj.InitFile(strShortFileName);
                    m_pConfig->AddGenObj(genObj);
                }

                if(m_bECRec) {
                    strShortFileName.Format(_T("LCERP_%s%s.st"), strName, strDataType);
                    strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileName);
                    m_pfGenLCMZRecPrg = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                    if(!m_pfGenLCMZRecPrg) {
                        AfxThrowMemoryException();        
                    }
                    genObj.InitFile(strShortFileName);
                    m_pConfig->AddGenObj(genObj);
                }

                if(m_bPCRec) {
                    strShortFileName.Format(_T("LCPRP_%s%s.st"), strName, strDataType);
                    strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileName);
                    m_pfGenLPCRecPrg = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                    if(!m_pfGenLPCRecPrg) {
                        AfxThrowMemoryException();        
                    }
                    genObj.InitFile(strShortFileName);
                    m_pConfig->AddGenObj(genObj);

                    // add the PC-Rec stuff to PC
                    genObj.InitLinkImp(strResource, _T("LCPC.gvl"));
                    m_pConfig->AddGenObj(genObj);
                }

                strShortFileNamePCGvl.Format(_T("LCPC_%s%s.gvl"), strName, strDataType);
                if(m_bPCRec || m_bPCSend) {
                    strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileNamePCGvl);
                    m_pfGenLGvlPrgPC = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                    if(!m_pfGenLGvlPrgPC) {
                        AfxThrowMemoryException();        
                    }
                    genObj.InitFile(strShortFileNamePCGvl);
                    m_pConfig->AddGenObj(genObj);
                    // now add this gvl file to all affected resources 
                    strResource = pLink->GetResourceName();
                    if(!strResource.IsEmpty()) {
                        strTargetType = pLink->GetTargetType();
                        if(!strTargetType.CompareNoCase(LATED_TARGET_TYPE_PC)) {
                            genObj.InitLinkImp(strResource, strShortFileNamePCGvl);
                            m_pConfig->AddGenObj(genObj);
                        }
                    }
                }
                strShortFileNameECGvl.Format(_T("LCEC_%s%s.gvl"), strName, strDataType);
                if(m_bECRec || m_bECSend) {
                    strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileNameECGvl);
                    m_pfGenLGvlPrgEC = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                    if(!m_pfGenLGvlPrgEC) {
                        AfxThrowMemoryException();        
                    }
                    genObj.InitFile(strShortFileNameECGvl);
                    m_pConfig->AddGenObj(genObj);
                    // now add this gvl file to all affected resources 
                    strResource = pLink->GetResourceName();
                    if(!strResource.IsEmpty()) {
                        strTargetType = pLink->GetTargetType();
                        if(!strTargetType.CompareNoCase(LATED_TARGET_TYPE_EC)) {
                            genObj.InitLinkImp(strResource, strShortFileNameECGvl);
                            m_pConfig->AddGenObj(genObj);
                        }
                    }
                }
                strShortFileNameCMZGvl.Format(_T("LCCMZ_%s%s.gvl"), strName, strDataType);
                if(m_bCMZRec || m_bCMZSend) {
                    strFileName.Format(_T("%s\\%s"), m_strPrjPath, strShortFileNameCMZGvl);
                    m_pfGenLGvlPrgCMZ = new CStdioFile(strFileName, CFile::modeCreate | CFile::modeWrite);   
                    if(!m_pfGenLGvlPrgCMZ) {
                        AfxThrowMemoryException();        
                    }
                    genObj.InitFile(strShortFileNameCMZGvl);
                    m_pConfig->AddGenObj(genObj);
                    // now add this gvl file to all affected resources 
                    strResource = pLink->GetResourceName();
                    if(!strResource.IsEmpty()) {
                        strTargetType = pLink->GetTargetType();
                        if(!strTargetType.CompareNoCase(LATED_TARGET_TYPE_CMZ)) {
                            genObj.InitLinkImp(strResource, strShortFileNameCMZGvl);
                            m_pConfig->AddGenObj(genObj);
                        }
                    }
                }

                // for all destLinks do ....
                for(int i = 0; i< pLink->GetDestLinkCount(); i++) {
                    pDest = pLink->GetDestLink(i);
                    if(!pDest) {
                        ASSERT(pDest);
                        continue;
                    }
                    strResource = pDest->GetResourceName();
                    if(!strResource.IsEmpty()) {
                        strTargetType = pDest->GetTargetType();
                        if(!strTargetType.CompareNoCase(LATED_TARGET_TYPE_PC)) {
                            genObj.InitLinkImp(strResource, strShortFileNamePCGvl);
                            m_pConfig->AddGenObj(genObj);

                            // add the PC-Rec stuff to PC
                            genObj.InitLinkImp(strResource, _T("LCPC.gvl"));
                            m_pConfig->AddGenObj(genObj);
                        }
                        if(!strTargetType.CompareNoCase(LATED_TARGET_TYPE_EC)) {
                            genObj.InitLinkImp(strResource, strShortFileNameECGvl);
                            m_pConfig->AddGenObj(genObj);
                        }
						if(!strTargetType.CompareNoCase(LATED_TARGET_TYPE_CMZ)) {
							genObj.InitLinkImp(strResource, strShortFileNameCMZGvl);
							m_pConfig->AddGenObj(genObj);
						}
                    }
                }
                // generate the starting part
                GenLAddrElem(m_pfGenLAddr, strName, strIPAddr, pLink->GetIP());
                if(m_bCMZSend) {
                    GenLCMZSendPrgBeg(m_pfGenLCMZSendPrg, strName, strDataType);
                }
                if(m_bECSend) {
                    GenLECSendPrgBeg(m_pfGenLECSendPrg, strName, strDataType);
                }
                if(m_bPCSend) {
                    GenLPCSendPrgBeg(m_pfGenLPCSendPrg, strName, strDataType);
                }
                if(m_bCMZRec) {
                    GenLCMZRecPrgBeg(m_pfGenLCMZRecPrg, strName, strDataType);
                }
                if(m_bECRec) {
                    GenLECRecPrgBeg(m_pfGenLECRecPrg, strName, strDataType);
                }
                if(m_bPCRec) {
                    GenLPCRecPrgBeg(m_pfGenLPCRecPrg, strName, strDataType);
                }
                if(m_bCMZRec || m_bCMZSend) {
                    GenLGvlBeg(m_pfGenLGvlPrgCMZ, strName, strDataType, eTargetCMZ);
                }
                if(m_bECRec || m_bECSend) {
                    GenLGvlBeg(m_pfGenLGvlPrgEC, strName, strDataType, eTargetEC);
                }
                if(m_bPCRec || m_bPCSend) {
                    GenLGvlBeg(m_pfGenLGvlPrgPC, strName, strDataType, eTargetPC);
                }
                break;
            }
            default:
                break;
        }
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return;
}

void CLCGen::OnDestLink(int id, CLCLink *pSource, CLCDestLink *pDest)
{
    CString strName;
    CString strSrcName;
    CString strIPAddr;
    CString strDataType;
    CLCDataType *pDataType;

    if(pSource->GetConnType() != P2P) {
        // no action for Publisher / Subscriber at the moment
        return;
    }
    try {
        pDest->GetName(strName);
        pDest->GetIP(strIPAddr);
        strSrcName = pSource->GetTargetName();
        pDataType = pSource->GetDataType();
        if(!pDataType) {
            ASSERT(pDataType);
            return;
        }
        pDataType->GetName(strDataType);
        pDataType->Release();
        switch(id) {
            case GenFromLink:
                // generate the dest link part
                GenLAddrElem(m_pfGenLAddr, strName, strIPAddr, pDest->GetIP());
				if(m_bCMZSend) {
					GenLCMZSendPrgElem(m_pfGenLCMZSendPrg, strName, strSrcName, strDataType);
				}
                if(m_bECSend) {
                    GenLECSendPrgElem(m_pfGenLECSendPrg, strName, strSrcName, strDataType);
                }
                if(m_bPCSend) {
                    GenLPCSendPrgElem(m_pfGenLPCSendPrg, strName, strSrcName, strDataType);
                }
                break;
            default:
                break;
        }
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return;
}

void CLCGen::OnEndLink(int id, CLCLink *pLink)
{
    CString strName;
    CString strIPAddr;

    if(pLink->GetConnType() != P2P) {
        // no action for Publisher / Subscriber at the moment
        return;
    }
    try {
        pLink->GetName(strName);
        pLink->GetIP(strIPAddr);
        switch(id) {
            case GenFromLink:
                // generate the end part
                if(m_bCMZSend) {
                    GenLCMZSendPrgEnd(m_pfGenLCMZSendPrg);
                }
                if(m_bECSend) {
                    GenLECSendPrgEnd(m_pfGenLECSendPrg);
                }
                if(m_bPCSend) {
                    GenLPCSendPrgEnd(m_pfGenLPCSendPrg);
                }

                // close the files to generate
                if(m_pfGenLCMZSendPrg) {
                    m_pfGenLCMZSendPrg->Close();
                    delete(m_pfGenLCMZSendPrg);
                    m_pfGenLCMZSendPrg = NULL;
                }
                if(m_pfGenLECSendPrg) {
                    m_pfGenLECSendPrg->Close();
                    delete(m_pfGenLECSendPrg);
                    m_pfGenLECSendPrg = NULL;
                }
                if(m_pfGenLPCSendPrg) {
                    m_pfGenLPCSendPrg->Close();
                    delete(m_pfGenLPCSendPrg);
                    m_pfGenLPCSendPrg = NULL;
                }
                if(m_pfGenLCMZRecPrg) {
                    m_pfGenLCMZRecPrg->Close();
                    delete(m_pfGenLCMZRecPrg);
                    m_pfGenLCMZRecPrg = NULL;
                }
                if(m_pfGenLECRecPrg) {
                    m_pfGenLECRecPrg->Close();
                    delete(m_pfGenLECRecPrg);
                    m_pfGenLECRecPrg = NULL;
                }
                if(m_pfGenLPCRecPrg) {
                    m_pfGenLPCRecPrg->Close();
                    delete(m_pfGenLPCRecPrg);
                    m_pfGenLPCRecPrg = NULL;
                }
                if(m_pfGenLGvlPrgPC) {
                    m_pfGenLGvlPrgPC->Close();
                    delete(m_pfGenLGvlPrgPC);
                    m_pfGenLGvlPrgPC = NULL;
                }
                if(m_pfGenLGvlPrgEC) {
                    m_pfGenLGvlPrgEC->Close();
                    delete(m_pfGenLGvlPrgEC);
                    m_pfGenLGvlPrgEC = NULL;
                }
                if(m_pfGenLGvlPrgCMZ) {
                    m_pfGenLGvlPrgCMZ->Close();
                    delete(m_pfGenLGvlPrgCMZ);
                    m_pfGenLGvlPrgCMZ = NULL;
                }
                break;
            default:
                break;
        }
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return;
}

bool CLCGen::GenDTypeBeg(CStdioFile* pfGen, CString strName, ETargetType eTargetType) 
{
	ASSERT(eTargetType >= eTargetPC && eTargetType <= eTargetCMZ);

	CString strTemplate = _T("\
(* this file is generated by ATCM LatComm Generator - do not edit! *)\n\
\nTYPE\n\
    LC%s_%s : STRUCT\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try
	{
		CString	strGen;
		
        strGen.Format(strTemplate, ccaTargetPrefixes[eTargetType], strName);
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenDTypeElem(CStdioFile* pfGen, CString strName, CString strType, BL_IEC_TYP type, bool bSubscript, size_t size, ETargetType eTargetType) {
    const CString str = _T("\
        %s: %s;\n\
");

    const CString strSubscript = _T("\
        %s: %s[%d];\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        CString strTypeToSet;

        if(eTargetType == eTargetPC && (type == BL_STRING)) {
            strTypeToSet = _T("WSTRING");
        } else {
            strTypeToSet = strType;
        }
        if(!bSubscript) {
            strGen.Format(str, strName, strTypeToSet);
        } else {
            strGen.Format(strSubscript,strName,strTypeToSet,size);
        }
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenDTypeEnd(CStdioFile* pfGen) {
    const CString str = _T("\
    END_STRUCT;\nEND_TYPE\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        pfGen->WriteString(str);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}


// SIS >>
bool CLCGen::GenDCMZSendBeg(CStdioFile* pfGen, CString strName) {
    const CString str = _T("\
(* this file is generated by ATCM LatComm Generator - do not edit! *)\n\
\n\
FUNCTION_BLOCK LCES_%s\n\
VAR_INPUT\n\
    bReq:           BOOL := FALSE;\n\
    dwDestAddr:     DWORD := 0;\n\
\n\
    Data:           LCCMZ_%s;\n\
END_VAR\n\
\n\
VAR_OUTPUT\n\
    bError:         BOOL := FALSE;\n\
    nError:         DINT := LCERR_NO_ERROR;\n\
    strError:       STRING := '';\n\
END_VAR\n\
\n\
VAR\n\
    fbUDPSend:      ECUDP_USEND;\n\
    baDataToSend:   ARRAY [0 .. LCDataLen_%s - 1] OF BYTE;\n\
    iPos:           DINT;\n\
END_VAR\n\
\n\
IF bReq THEN\n\
    (* convert the data *)\n\
    iPos := 0;\n\
\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        strGen.Format(str, strName, strName, strName);
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenDCMZSendElem(CStdioFile* pfGen, CString strDataName, CString strMemberName, CString strType, BL_IEC_TYP type) {
    const CString str = _T("\
    COPY_%s_TO_BYTE_ARRAY(Data.%s, iPos, baDataToSend);\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        strGen.Format(str, strType, strMemberName);
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenDCMZSendEnd(CStdioFile* pfGen, CString strName) {
    const CString str = _T("\
    IF(iPos = LC_INVALID_POS) THEN\n\
        (* CopyError *)\n\
        bError := TRUE;\n\
        nError := LCERR_COPY;\n\
        strError := LCERRTXT_EC_COPYERROR;\n\
        RETURN;\n\
    END_IF;\n\
\n\
    fbUDPSend(REQ := TRUE,\n\
              PARTNER_ADDR := dwDestAddr,\n\
              PARTNER_PORT := LC_PORT_4C,\n\
              LOCAL_PORT := LCDataId_%s,\n\
              SD_1 := baDataToSend,\n\
              ERROR => bError,\n\
              ERROR_NR => nError,\n\
              ERROR_STRING => strError);\n\
END_IF;\n\
\n\
END_FUNCTION_BLOCK\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        strGen.Format(str, strName);
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}
// SIS <<


bool CLCGen::GenDECSendBeg(CStdioFile* pfGen, CString strName) {
    const CString str = _T("\
(* this file is generated by ATCM LatComm Generator - do not edit! *)\n\
\n\
FUNCTION_BLOCK LCES_%s\n\
VAR_INPUT\n\
    bReq:           BOOL := FALSE;\n\
    dwDestAddr:     DWORD := 0;\n\
\n\
    Data:           LCEC_%s;\n\
END_VAR\n\
\n\
VAR_OUTPUT\n\
    bError:         BOOL := FALSE;\n\
    nError:         DINT := LCERR_NO_ERROR;\n\
    strError:       STRING := '';\n\
END_VAR\n\
\n\
VAR\n\
    fbUDPSend:      ECUDP_USEND;\n\
    baDataToSend:   ARRAY [0 .. LCDataLen_%s - 1] OF BYTE;\n\
    iPos:           DINT;\n\
END_VAR\n\
\n\
IF bReq THEN\n\
    (* convert the data *)\n\
    iPos := 0;\n\
\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        strGen.Format(str, strName, strName, strName);
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenDECSendElem(CStdioFile* pfGen, CString strDataName, CString strMemberName, CString strType, BL_IEC_TYP type) {
    const CString str = _T("\
    COPY_%s_TO_BYTE_ARRAY(Data.%s, iPos, baDataToSend);\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        strGen.Format(str, strType, strMemberName);
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenDECSendEnd(CStdioFile* pfGen, CString strName) {
    const CString str = _T("\
    IF(iPos = LC_INVALID_POS) THEN\n\
        (* CopyError *)\n\
        bError := TRUE;\n\
        nError := LCERR_COPY;\n\
        strError := LCERRTXT_EC_COPYERROR;\n\
        RETURN;\n\
    END_IF;\n\
\n\
    fbUDPSend(REQ := TRUE,\n\
              PARTNER_ADDR := dwDestAddr,\n\
              PARTNER_PORT := LC_PORT_4C,\n\
              LOCAL_PORT := LCDataId_%s,\n\
              SD_1 := baDataToSend,\n\
              ERROR => bError,\n\
              ERROR_NR => nError,\n\
              ERROR_STRING => strError);\n\
END_IF;\n\
\n\
END_FUNCTION_BLOCK\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        strGen.Format(str, strName);
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}


bool CLCGen::GenDPCSendBeg(CStdioFile* pfGen, CString strName) {
    const CString str = _T("\
(* this file is generated by ATCM LatComm Generator - do not edit! *)\n\
\n\
FUNCTION_BLOCK LCPS_%s\n\
VAR_INPUT\n\
    bReq:           BOOL := FALSE;\n\
    strDestAddr:    WSTRING := \"\";\n\
\n\
    Data:           LCPC_%s;\n\
END_VAR\n\
\n\
VAR_OUTPUT\n\
    bError:         BOOL := FALSE;\n\
    nError:         DINT := LCERR_NO_ERROR;\n\
    strError:       WSTRING := \"\";\n\
END_VAR\n\
\n\
VAR_IN_OUT\n\
    hSocket : UDP_SOCKET_HANDLE;\n\
END_VAR\n\
\n\
VAR\n\
    fbUDPSend:      UDP_USEND;\n\
    bsDataToSend:   BYTE_STRING;\n\
    iPos:           DINT;\n\
    iTempPos:       DINT;\n\
END_VAR\n\
\n\
iTempPos := 0;\n\
IF bReq THEN\n\
    (* convert the data *)\n\
    iPos := 0;\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        strGen.Format(str, strName, strName);
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenDPCSendElem(CStdioFile* pfGen, CString strDataName, CString strMemberName, CString strType, BL_IEC_TYP type, size_t size) {
    const CString str1 = _T("\
    iTempPos := iPos;\n\
    COPY_%s_TO_BYTE_STRING(Data.%s, iPos, bsDataToSend);\n\
    iPos := iTempPos + MIN(LEN_DINT(Data.%s), %d - 1);\n\
    COPY_BYTE_TO_BYTE_STRING(0, iPos, bsDataToSend);\n\
    iPos := iTempPos + %d;\n\
");

    const CString str2 = _T("\
    COPY_%s_TO_BYTE_STRING(Data.%s, iPos, bsDataToSend);\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        if((type == BL_WSTRING) || (type == BL_STRING)) {
            strGen.Format(str1, strType, strMemberName, strMemberName, size, size);
        } else {
            strGen.Format(str2, strType, strMemberName);
        }
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenDPCSendEnd(CStdioFile* pfGen, CString strName) {
    const CString str = _T("\
\n\
    IF(iPos = LC_INVALID_POS) THEN\n\
        (* CopyError *)\n\
        bError := TRUE;\n\
        nError := LCERR_COPY;\n\
        strError := LCERRTXT_PC_COPYERROR;\n\
        RETURN;\n\
    END_IF;\n\
\n\
    fbUDPSend(REQ := TRUE,\n\
              PARTNER_ADDR := strDestAddr,\n\
              PARTNER_PORT := LC_PORT_4C,\n\
              SOCKET_HANDLE := hSocket,\n\
              SD_1 := bsDataToSend,\n\
              ERROR => bError,\n\
              ERROR_NR => nError,\n\
              ERROR_STRING => strError);\n\
END_IF;\n\
END_FUNCTION_BLOCK\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        pfGen->WriteString(str);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}


// SIS >>
bool CLCGen::GenDCMZRecBeg(CStdioFile* pfGen, CString strName) {
    const CString str = _T("\
(* this file is generated by ATCM LatComm Generator - do not edit! *)\n\
\n\
FUNCTION_BLOCK LCER_%s\n\
VAR_INPUT\n\
    bReq:           BOOL := FALSE;\n\
    dwSrcAddr:      DWORD := 0;\n\
END_VAR\n\
\n\
VAR_OUTPUT\n\
    Data:           LCCMZ_%s;\n\
\n\
    bError:         BOOL := FALSE;\n\
    nError:         DINT := LCERR_NO_ERROR;\n\
    strError:       STRING := '';\n\
    bDataReady:     BOOL := FALSE;\n\
END_VAR\n\
\n\
VAR\n\
    fbUDPRcv:       ECUDP_URCV;\n\
    baDataRead:     ARRAY [0 .. LCDataLen_%s - 1] OF BYTE;\n\
    iPos:           DINT;\n\
    iTempPos:       DINT;\n\
END_VAR\n\
\n\
iTempPos := 0;\n\
IF bReq THEN\n\
    fbUDPRcv(REQ := TRUE,\n\
              DATA_LEN := LCDataLen_%s,\n\
              SRC_ADDR := dwSrcAddr,\n\
              SRC_PORT := LC_PORT_4C,\n\
              LOCAL_PORT := LCDataId_%s,\n\
              RD_1 := baDataRead,\n\
              NDR => bDataReady,\n\
              ERROR => bError,\n\
              ERROR_NR => nError,\n\
              ERROR_STRING => strError);\n\
    IF(bError) THEN\n\
        RETURN;\n\
    END_IF;\n\
    IF(bDataReady) THEN\n\
        iPos := 0;\n\
\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        strGen.Format(str, strName, strName, strName, strName, strName);
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenDCMZRecElem(CStdioFile* pfGen, CString strDataName, CString strMemberName, CString strType, BL_IEC_TYP type, size_t size) {
    const CString str1 = _T("\
        Data.%s := COPY_%s_FROM_BYTE_ARRAY(iPos, baDataRead);\n\
");

    const CString str2 = _T("\
        ClearTaskErrno();\n\
        Data.%s := COPY_%s_FROM_BYTE_ARRAY(iPos, baDataRead);\n\
        IF(GetTaskErrno() = FC_ERRNO_INVALID_ARG) THEN\n\
            (* TimeOverrunError *)\n\
            bError := TRUE;\n\
            nError := LCERR_TIME_CONVERT;\n\
            strError := LCERRTXT_CMZ_TIMECONV;\n\
            RETURN;\n\
        END_IF;\n\
");

    const CString str3 = _T("\
        iTempPos := iPos;\n\
        Data.%s := COPY_%s_FROM_BYTE_ARRAY(iPos, baDataRead);\n\
        iPos := iTempPos + %d;\n\
");
    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        switch(type) {
            case BL_TIME:
                strGen.Format(str2, strMemberName, strType);
                break;
            case BL_STRING:
                strGen.Format(str3, strMemberName, strType, size);
                break;
            default:
                strGen.Format(str1, strMemberName, strType);
                break;
        }
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}


bool CLCGen::GenDCMZRecEnd(CStdioFile* pfGen, CString strName) {
    const CString str = _T("\
\n\
        IF(iPos = LC_INVALID_POS) THEN\n\
            (* CopyError *)\n\
            bError := TRUE;\n\
            nError := LCERR_COPY;\n\
            strError := LCERRTXT_CMZ_COPYERROR;\n\
            RETURN;\n\
        END_IF;\n\
    END_IF;\n\
END_IF;\n\
\n\
END_FUNCTION_BLOCK\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        pfGen->WriteString(str);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}
// SIS <<

bool CLCGen::GenDECRecBeg(CStdioFile* pfGen, CString strName) {
    const CString str = _T("\
(* this file is generated by ATCM LatComm Generator - do not edit! *)\n\
\n\
FUNCTION_BLOCK LCER_%s\n\
VAR_INPUT\n\
    bReq:           BOOL := FALSE;\n\
    dwSrcAddr:      DWORD := 0;\n\
END_VAR\n\
\n\
VAR_OUTPUT\n\
    Data:           LCEC_%s;\n\
\n\
    bError:         BOOL := FALSE;\n\
    nError:         DINT := LCERR_NO_ERROR;\n\
    strError:       STRING := '';\n\
    bDataReady:     BOOL := FALSE;\n\
END_VAR\n\
\n\
VAR\n\
    fbUDPRcv:       ECUDP_URCV;\n\
    baDataRead:     ARRAY [0 .. LCDataLen_%s - 1] OF BYTE;\n\
    iPos:           DINT;\n\
    iTempPos:       DINT;\n\
END_VAR\n\
\n\
iTempPos := 0;\n\
IF bReq THEN\n\
    fbUDPRcv(REQ := TRUE,\n\
              DATA_LEN := LCDataLen_%s,\n\
              SRC_ADDR := dwSrcAddr,\n\
              SRC_PORT := LC_PORT_4C,\n\
              LOCAL_PORT := LCDataId_%s,\n\
              RD_1 := baDataRead,\n\
              NDR => bDataReady,\n\
              ERROR => bError,\n\
              ERROR_NR => nError,\n\
              ERROR_STRING => strError);\n\
    IF(bError) THEN\n\
        RETURN;\n\
    END_IF;\n\
    IF(bDataReady) THEN\n\
        iPos := 0;\n\
\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        strGen.Format(str, strName, strName, strName, strName, strName);
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenDECRecElem(CStdioFile* pfGen, CString strDataName, CString strMemberName, CString strType, BL_IEC_TYP type, size_t size) {
    const CString str1 = _T("\
        Data.%s := COPY_%s_FROM_BYTE_ARRAY(iPos, baDataRead);\n\
");

    const CString str2 = _T("\
        ClearTaskErrno();\n\
        Data.%s := COPY_%s_FROM_BYTE_ARRAY(iPos, baDataRead);\n\
        IF(GetTaskErrno() = FC_ERRNO_INVALID_ARG) THEN\n\
            (* TimeOverrunError *)\n\
            bError := TRUE;\n\
            nError := LCERR_TIME_CONVERT;\n\
            strError := LCERRTXT_EC_TIMECONV;\n\
            RETURN;\n\
        END_IF;\n\
");

    const CString str3 = _T("\
        iTempPos := iPos;\n\
        Data.%s := COPY_%s_FROM_BYTE_ARRAY(iPos, baDataRead);\n\
        iPos := iTempPos + %d;\n\
");
    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        switch(type) {
            case BL_TIME:
                strGen.Format(str2, strMemberName, strType);
                break;
            case BL_STRING:
                strGen.Format(str3, strMemberName, strType, size);
                break;
            default:
                strGen.Format(str1, strMemberName, strType);
                break;
        }
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenDECRecEnd(CStdioFile* pfGen, CString strName) {
    const CString str = _T("\
\n\
        IF(iPos = LC_INVALID_POS) THEN\n\
            (* CopyError *)\n\
            bError := TRUE;\n\
            nError := LCERR_COPY;\n\
            strError := LCERRTXT_EC_COPYERROR;\n\
            RETURN;\n\
        END_IF;\n\
    END_IF;\n\
END_IF;\n\
\n\
END_FUNCTION_BLOCK\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        pfGen->WriteString(str);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenDPCRecBeg(CStdioFile* pfGen, CString strName) {
    const CString str = _T("\
(* this file is generated by ATCM LatComm Generator - do not edit! *)\n\
\n\
FUNCTION_BLOCK LCPR_%s\n\
VAR_EXTERNAL\n\
    LCfbRecToCache: LCPCRecToCache;\n\
    LCbError: BOOL;\n\
END_VAR\n\
\n\
VAR_INPUT\n\
    bReq:           BOOL := FALSE;\n\
    strSrcAddr:     WSTRING := \"\";\n\
END_VAR\n\
\n\
VAR_OUTPUT\n\
    Data:           LCPC_%s;\n\
    bError:         BOOL := FALSE;\n\
    nError:         DINT := LCERR_NO_ERROR;\n\
    strError:       WSTRING := \"\";\n\
    bDataReady:     BOOL := FALSE;\n\
END_VAR\n\
\n\
VAR_IN_OUT\n\
    Cache: ARRAY OF LCCacheData;\n\
END_VAR\n\
\n\
VAR\n\
    bsDataRead:     BYTE_STRING;\n\
    iPos:           DINT;\n\
    i:              DINT;\n\
END_VAR\n\
\n\
IF bReq THEN\n\
    LCfbRecToCache(strError => strError);\n\
    IF(LCbError) THEN\n\
        RETURN;\n\
    END_IF;\n\
\n\
    (* now look for the message *)\n\
    bDataReady := FALSE;\n\
    FOR i := 0 TO LCPC_MAX_CACHE - 1 DO\n\
        IF (Cache[i].iDataID = LCDataId_%s) AND (Cache[i].sSrcAddr = strSrcAddr) THEN\n\
            bsDataRead := Cache[i].Data;\n\
            Cache[i].iDataID := LC_INVALID_PORT;\n\
            bDataReady := TRUE;\n\
            EXIT;\n\
        END_IF;\n\
    END_FOR;\n\
\n\
    IF(bDataReady) THEN\n\
        iPos := 0;\n\
\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        strGen.Format(str, strName, strName, strName);
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenDPCRecElem(CStdioFile* pfGen, CString strDataName, CString strMemberName, CString strType, BL_IEC_TYP type, size_t size) {
    const CString str1 = _T("\
        Data.%s := COPY_%s_FROM_BYTE_STRING(%d, iPos, bsDataRead);\n\
");

    const CString str2 = _T("\
        Data.%s := COPY_%s_FROM_BYTE_STRING(iPos, bsDataRead);\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        if((type == BL_WSTRING) || (type == BL_STRING)) {
            strGen.Format(str1, strMemberName, strType, size);
        } else {
            strGen.Format(str2, strMemberName, strType);
        }
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenDPCRecEnd(CStdioFile* pfGen, CString strName) {
    const CString str = _T("\
\n\
        IF(iPos = LC_INVALID_POS) THEN\n\
            (* CopyError *)\n\
            bError := TRUE;\n\
            nError := LCERR_COPY;\n\
            strError := LCERRTXT_PC_COPYERROR;\n\
            RETURN;\n\
        END_IF;\n\
    END_IF;\n\
END_IF;\n\
\n\
END_FUNCTION_BLOCK\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        pfGen->WriteString(str);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenDConst(CStdioFile* pfGen, CLCDataType *pDataType) {
    const CString str = _T("\
(* Constant file *)\n\
(* this file is generated by ATCM LatComm Generator - do not edit! *)\n\
CONST\n\
    LCDataId_%s: DINT := %u;\n\
    LCDataLen_%s: DINT := %u;\n\
END_CONST\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    CString strName;

    pDataType->GetName(strName);

    try{
        CString strGen;
        strGen.Format(str, strName, pDataType->GetDataID(), strName, pDataType->GetDataLen());
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenDStuffRecToCache(CStdioFile* pfGen) {
    const CString str1 = _T("\
(* this file is generated by ATCM LatComm Generator - do not edit! *)\n\
\n\
FUNCTION_BLOCK LCPCRecToCache\n\
VAR_EXTERNAL\n\
    LCPCCache: ARRAY [0 .. LCPC_MAX_CACHE - 1] OF LCCacheData;\n\
    LChSocket: UDP_SOCKET_HANDLE;\n\
    LCbConnected: BOOL;\n\
    LCbError: BOOL;\n\
    LCnError: DINT;\n\
END_VAR\n\
\n\
VAR_INPUT\n\
    strSrcAddr:     WSTRING := \"\";\n\
END_VAR\n\
\n\
VAR_OUTPUT\n\
    strError:       WSTRING := \"\";\n\
END_VAR\n\
\n\
VAR\n\
    fbUDPConnect:   UDP_CONNECT;\n\
    fbUDPRcv:       UDP_URCV;\n\
    bsDataRead:     BYTE_STRING;\n\
    iPartnerPort:   DINT;\n\
    strPartnerAddr: WSTRING;\n\
    bDataReady:     BOOL;\n\
    bCacheOK:       BOOL;\n\
    i:              DINT;\n\
    iRecCount:      DINT;\n\
END_VAR\n\
\n\
IF (NOT LCbConnected) THEN\n\
    LChSocket.LocalPort := LC_PORT_4C;\n\
    fbUDPConnect(\n\
              EN_C := TRUE,\n\
              SOCKET_HANDLE := LChSocket,\n\
              ERROR => LCbError,\n\
              ERROR_NR => LCnError,\n\
              ERROR_STRING => strError);\n\
    IF (LCbError) THEN\n\
        RETURN;\n\
    END_IF;\n\
    IF (fbUDPConnect.VALID) THEN\n\
        (* init the cache *)\n\
        FOR i := 0 TO LCPC_MAX_CACHE - 1 DO\n\
            LCPCCache[i].iDataID := LC_INVALID_PORT;\n\
        END_FOR;\n\
        LCbConnected := TRUE;\n\
    END_IF;\n\
END_IF;\n\
\n\
");

    const CString str2 = _T("\
IF (LCbConnected) THEN\n\
    FOR iRecCount := 0 TO LCPC_MAX_REC_TRIES - 1 DO\n\
        fbUDPRcv(EN_R := TRUE,\n\
                  DATA_LEN := LC_MAX_DATA_LEN,\n\
                  SOCKET_HANDLE := LChSocket,\n\
                  RD_1 => bsDataRead,\n\
                  PARTNER_ADDR => strPartnerAddr,\n\
                  PARTNER_PORT => iPartnerPort,\n\
                  NDR => bDataReady,\n\
                  ERROR => LCbError,\n\
                  ERROR_NR => LCnError,\n\
                  ERROR_STRING => strError);\n\
        IF (LCbError) THEN\n\
            RETURN;\n\
        END_IF;\n\
        IF (bDataReady) THEN\n\
            (* now write to cache *)\n\
            bCacheOK := FALSE;\n\
            FOR i := 0 TO LCPC_MAX_CACHE - 1 DO\n\
                IF (LCPCCache[i].iDataID = iPartnerPort) AND (LCPCCache[i].sSrcAddr = strPartnerAddr) THEN\n\
                    (* overwrite the old message *)\n\
                    LCPCCache[i].Data := bsDataRead;\n\
                    bCacheOK := TRUE;\n\
                    EXIT;\n\
                END_IF;\n\
                IF (LCPCCache[i].iDataID = LC_INVALID_PORT) THEN\n\
                    LCPCCache[i].iDataID := iPartnerPort;\n\
                    LCPCCache[i].sSrcAddr := strPartnerAddr;\n\
                    LCPCCache[i].Data := bsDataRead;\n\
                    bCacheOK := TRUE;\n\
                    EXIT;\n\
                END_IF;\n\
            END_FOR;\n\
            IF (NOT bCacheOK) THEN\n\
                LCbError := TRUE;\n\
                LCnError := LCERR_NO_MORE_CACHE;\n\
                strError := LCERRTXT_PC_NOCACHE;\n\
            END_IF;\n\
        END_IF;\n\
        IF NOT bDataReady THEN\n\
            EXIT; \n\
        END_IF;\n\
    END_FOR;\n\
END_IF;\n\
\n\
END_FUNCTION_BLOCK\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        pfGen->WriteString(str1);
        pfGen->WriteString(str2);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenDStuffConst(CStdioFile* pfGen) {
    const CString str = _T("\
(* Constant file *)\n\
(* this file is generated by ATCM LatComm Generator - do not edit! *)\n\
\n\
(* general LCConstants *)\n\
CONST\n\
    (* IP port, used for the lateral communication of ATCM *)\n\
    LC_PORT_4C:             DINT := 16000;\n\
    LC_INVALID_PORT:        DINT := -1;\n\
\n\
    (* data identifier *)\n\
    LC_MAX_DATAITEMS:       DINT := 255;\n\
    LC_INVALID_DATAID:      DINT := -1;\n\
    LC_FIRST_DATAID:        DINT := LC_PORT_4C + 1;\n\
    LC_LAST_DATAID:         DINT := LC_FIRST_DATAID + LC_MAX_DATAITEMS;\n\
\n\
    (* general *)\n\
    LC_INVALID_POS:         DINT := -1;\n\
    LC_MAX_DATA_LEN:        DINT := 1500;\n\
END_CONST\n\
\n\
(* LC error codes *)\n\
CONST\n\
    LCERR_NO_ERROR:         DINT := 0;\n\
    LCERR_COPY:             DINT := -1;     (* there was an error on copying data to or from the ARRAY OF BYTE *)\n\
    LCERR_TIME_CONVERT:     DINT := -2;     (* there was an attempt to copy a Time64 value to a Time32 variable *)\n\
    LCERR_NO_MORE_CACHE:    DINT := -3;     (* there is no cache for the message reveived *)\n\
END_CONST\n\
\n\
(* LC error texts *)\n\
CONST\n\
    LCERRTXT_EC_COPYERROR: STRING := 'CopyError';\n\
    LCERRTXT_EC_TIMECONV:  STRING := 'TimeConversionError';\n\
    LCERRTXT_PC_COPYERROR: WSTRING := \"CopyError\";\n\
    LCERRTXT_PC_TIMECONV:  WSTRING := \"TimeConversionError\";\n\
    LCERRTXT_PC_NOCACHE:   WSTRING := \"NoMoreCache\";\n\
END_CONST\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        pfGen->WriteString(str);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenDStuffUser(CStdioFile* pfGen) {
    const CString str = _T("\
(* Constant file *)\n\
(* this file is generated once by ATCM LatComm Generator - edit to adapt! *)\n\
\n\
(* user specific LCConstants *)\n\
CONST\n\
    (* cache definitions *)\n\
    LCPC_MAX_CACHE:         DINT := 16;      (* maximal number of entries in the receivers cach for PC *)\n\
    LCPC_MAX_REC_TRIES:     DINT := 100;     (* maximal number of tries to get a telegram in FB LCPCRecToCache *)\n\
\n\
END_CONST\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        pfGen->WriteString(str);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenDStuffCacheType(CStdioFile* pfGen) {
    const CString str = _T("\
(* this file is generated by ATCM LatComm Generator - do not edit! *)\n\
\n\
TYPE\n\
    LCCacheData : STRUCT\n\
        iDataID: DINT;\n\
        Data: BYTE_STRING;\n\
        sSrcAddr: WSTRING;\n\
    END_STRUCT;\n\
END_TYPE\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        pfGen->WriteString(str);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenDStuffGvl(CStdioFile* pfGen) {
    const CString str = _T("\
(*Global variable file*)\n\
(* this file is generated by ATCM LatComm Generator - do not edit! *)\n\
\n\
VAR_GLOBAL\n\
    LCPCCache: ARRAY [0 .. LCPC_MAX_CACHE - 1] OF LCCacheData;\n\
    LChSocket: UDP_SOCKET_HANDLE;\n\
    LCbConnected: BOOL := FALSE;\n\
    LCfbRecToCache: LCPCRecToCache;\n\
    LCbError: BOOL := FALSE;\n\
    LCnError: DINT := LCERR_NO_ERROR;\n\
END_VAR\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        pfGen->WriteString(str);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenLAddrBeg(CStdioFile* pfGen) {
    const CString str = _T("\
(*Constant file*)\n\
(* this file is generated by ATCM LatComm Generator - do not edit! *)\n\
\n\
CONST\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        m_TragetAddrs.RemoveAll();
        pfGen->WriteString(str);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenLAddrElem(CStdioFile* pfGen, CString strName, CString strIPAddr, ULONG ulIP) {
    const CString str = _T("\
    LCPCAddr_%s:  WSTRING := \"%s\";\n\
    LCECAddr_%s:  DWORD := 16#%08x;\n\
\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        if(m_TragetAddrs.Find(strName) == NULL) {
            m_TragetAddrs.AddTail(strName);
            CString strGen;
            strGen.Format(str, strName, strIPAddr, strName, ulIP);
            pfGen->WriteString(strGen);
        }
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenLAddrEnd(CStdioFile* pfGen) {
    const CString str = _T("\
END_CONST\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        pfGen->WriteString(str);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}


// SIS >>
bool CLCGen::GenLCMZSendPrgBeg(CStdioFile* pfGen, CString strName, CString strDataType) {
    const CString str = _T("\
(* this file is generated by ATCM LatComm Generator - do not edit! *)\n\
\n\
PROGRAM LCCSP_%s%s\n\
VAR_EXTERNAL\n\
    LCData_%s_%s : LCCMZ_%s;\n\
    LCbError_%s_%s: BOOL;\n\
    LCnError_%s_%s: DINT;\n\
END_VAR\n\
\n\
VAR\n\
    strError: STRING := '';\n\
    fbSend: LCES_%s;\n\
END_VAR\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        strGen.Format(str, strName, strDataType,
            strName, strDataType, strDataType,
            strName, strDataType,
            strName, strDataType,
            strDataType);
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenLCMZSendPrgElem(CStdioFile* pfGen, CString strName, CString strSrcName, CString strDataType) {
    const CString str = _T("\
\n\
    fbSend(bReq := TRUE,\n\
          dwDestAddr := LCCMZAddr_%s,\n\
          Data := LCData_%s_%s,\n\
          bError => LCbError_%s_%s,\n\
          nError => LCnError_%s_%s,\n\
          strError => strError);\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        strGen.Format(str, strName,
            strSrcName, strDataType,
            strSrcName, strDataType,
            strSrcName, strDataType);
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenLCMZSendPrgEnd(CStdioFile* pfGen) {
    const CString str = _T("\
END_PROGRAM\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        pfGen->WriteString(str);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}
// SIS <<

bool CLCGen::GenLECSendPrgBeg(CStdioFile* pfGen, CString strName, CString strDataType) {
    const CString str = _T("\
(* this file is generated by ATCM LatComm Generator - do not edit! *)\n\
\n\
PROGRAM LCESP_%s%s\n\
VAR_EXTERNAL\n\
    LCData_%s_%s : LCEC_%s;\n\
    LCbError_%s_%s: BOOL;\n\
    LCnError_%s_%s: DINT;\n\
END_VAR\n\
\n\
VAR\n\
    strError: STRING := '';\n\
    fbSend: LCES_%s;\n\
END_VAR\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        strGen.Format(str, strName, strDataType,
            strName, strDataType, strDataType,
            strName, strDataType,
            strName, strDataType,
            strDataType);
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenLECSendPrgElem(CStdioFile* pfGen, CString strName, CString strSrcName, CString strDataType) {
    const CString str = _T("\
\n\
    fbSend(bReq := TRUE,\n\
          dwDestAddr := LCECAddr_%s,\n\
          Data := LCData_%s_%s,\n\
          bError => LCbError_%s_%s,\n\
          nError => LCnError_%s_%s,\n\
          strError => strError);\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        strGen.Format(str, strName,
            strSrcName, strDataType,
            strSrcName, strDataType,
            strSrcName, strDataType);
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenLECSendPrgEnd(CStdioFile* pfGen) {
    const CString str = _T("\
END_PROGRAM\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        pfGen->WriteString(str);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenLPCSendPrgBeg(CStdioFile* pfGen, CString strName, CString strDataType) {
    const CString str = _T("\
(* this file is generated by ATCM LatComm Generator - do not edit! *)\n\
\n\
PROGRAM LCPSP_%s%s\n\
VAR_EXTERNAL\n\
    LCData_%s_%s : LCPC_%s;\n\
    LCbError_%s_%s: BOOL;\n\
    LCnError_%s_%s: DINT;\n\
END_VAR\n\
\n\
VAR\n\
    fbUDPConnect:   UDP_CONNECT;\n\
    fbSend: LCPS_%s;\n\
    hSocket: UDP_SOCKET_HANDLE;\n\
    bConnected:     BOOL := FALSE;\n\
    strError: WSTRING := \"\";\n\
END_VAR\n\
\n\
IF (NOT bConnected) THEN\n\
    hSocket.LocalPort := LCDataId_%s;\n\
    fbUDPConnect(\n\
              EN_C := TRUE,\n\
              SOCKET_HANDLE := hSocket,\n\
              ERROR => LCbError_%s_%s,\n\
              ERROR_NR => LCnError_%s_%s,\n\
              ERROR_STRING => strError);\n\
    IF (LCbError_%s_%s) THEN\n\
        RETURN;\n\
    END_IF;\n\
    IF (fbUDPConnect.VALID) THEN\n\
        bConnected := TRUE;\n\
    END_IF;\n\
END_IF;\n\
\n\
IF (bConnected) THEN\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        strGen.Format(str, strName, strDataType,
            strName, strDataType, strDataType,
            strName, strDataType,
            strName, strDataType,
            strDataType,
            strDataType,
            strName, strDataType,
            strName, strDataType,
            strName, strDataType);
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenLPCSendPrgElem(CStdioFile* pfGen, CString strName, CString strSrcName, CString strDataType) {
    const CString str = _T("\
\n\
    fbSend(bReq := TRUE,\n\
          hSocket := hSocket,\n\
          strDestAddr := LCPCAddr_%s,\n\
          Data := LCData_%s_%s,\n\
          bError => LCbError_%s_%s,\n\
          nError => LCnError_%s_%s,\n\
          strError => strError);\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        strGen.Format(str, strName,
            strSrcName, strDataType,
            strSrcName, strDataType,
            strSrcName, strDataType);
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenLPCSendPrgEnd(CStdioFile* pfGen) {
    const CString str = _T("\
END_IF;\n\
END_PROGRAM\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        pfGen->WriteString(str);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

// SIS >>
bool CLCGen::GenLCMZRecPrgBeg(CStdioFile* pfGen, CString strName, CString strDataType) {
    const CString str = _T("\
(* this file is generated by ATCM LatComm Generator - do not edit! *)\n\
\n\
PROGRAM LCCRP_%s%s\n\
VAR_EXTERNAL\n\
    LCData_%s_%s : LCCMZ_%s;\n\
    LCDataReady_%s_%s: BOOL;\n\
    LCbError_%s_%s: BOOL;\n\
    LCnError_%s_%s: DINT;\n\
END_VAR\n\
\n\
VAR\n\
    strError: STRING := '';\n\
    fbRec: LCER_%s;\n\
END_VAR\n\
\n\
    fbRec(bReq := TRUE,\n\
          dwSrcAddr := LCCMZAddr_%s,\n\
          Data => LCData_%s_%s,\n\
          bDataReady => LCDataReady_%s_%s,\n\
          bError => LCbError_%s_%s,\n\
          nError => LCnError_%s_%s,\n\
          strError => strError);\n\
END_PROGRAM\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        strGen.Format(str, strName, strDataType,
            strName, strDataType, strDataType,
            strName, strDataType,
            strName, strDataType,
            strName, strDataType,
            strDataType, 
            strName, 
            strName, strDataType, 
            strName, strDataType,
            strName, strDataType,
            strName, strDataType);
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}
// SIS <<

bool CLCGen::GenLECRecPrgBeg(CStdioFile* pfGen, CString strName, CString strDataType) {
    const CString str = _T("\
(* this file is generated by ATCM LatComm Generator - do not edit! *)\n\
\n\
PROGRAM LCERP_%s%s\n\
VAR_EXTERNAL\n\
    LCData_%s_%s : LCEC_%s;\n\
    LCDataReady_%s_%s: BOOL;\n\
    LCbError_%s_%s: BOOL;\n\
    LCnError_%s_%s: DINT;\n\
END_VAR\n\
\n\
VAR\n\
    strError: STRING := '';\n\
    fbRec: LCER_%s;\n\
END_VAR\n\
\n\
    fbRec(bReq := TRUE,\n\
          dwSrcAddr := LCECAddr_%s,\n\
          Data => LCData_%s_%s,\n\
          bDataReady => LCDataReady_%s_%s,\n\
          bError => LCbError_%s_%s,\n\
          nError => LCnError_%s_%s,\n\
          strError => strError);\n\
END_PROGRAM\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        strGen.Format(str, strName, strDataType,
            strName, strDataType, strDataType,
            strName, strDataType,
            strName, strDataType,
            strName, strDataType,
            strDataType, 
            strName, 
            strName, strDataType, 
            strName, strDataType,
            strName, strDataType,
            strName, strDataType);
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenLPCRecPrgBeg(CStdioFile* pfGen, CString strName, CString strDataType) {
    const CString str = _T("\
(* this file is generated by ATCM LatComm Generator - do not edit! *)\n\
\n\
PROGRAM LCPRP_%s%s\n\
\n\
VAR_EXTERNAL\n\
    LCData_%s_%s : LCPC_%s;\n\
    LCDataReady_%s_%s: BOOL;\n\
    LCbError_%s_%s: BOOL;\n\
    LCnError_%s_%s: DINT;\n\
    LCPCCache: ARRAY [0 .. LCPC_MAX_CACHE - 1] OF LCCacheData;\n\
END_VAR\n\
\n\
VAR\n\
    fbRec: LCPR_%s;\n\
    strError: WSTRING := \"\";\n\
END_VAR\n\
\n\
fbRec(bReq := TRUE,\n\
      strSrcAddr := LCPCAddr_%s,\n\
      Cache := LCPCCache,\n\
      Data => LCData_%s_%s,\n\
      bDataReady => LCDataReady_%s_%s,\n\
      bError => LCbError_%s_%s,\n\
      nError => LCnError_%s_%s,\n\
      strError => strError);\n\
\n\
END_PROGRAM\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try{
        CString strGen;
        strGen.Format(str, strName, strDataType,
            strName, strDataType, strDataType,
            strName, strDataType,
            strName, strDataType,
            strName, strDataType,
            strDataType,
            strName,
            strName, strDataType,
            strName, strDataType,
            strName, strDataType,
            strName, strDataType);
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

bool CLCGen::GenLGvlBeg(CStdioFile* pfGen, CString strName, CString strDataType, ETargetType eTargetType) 
{
	ASSERT(eTargetType >= eTargetPC && eTargetType <= eTargetCMZ);

    CString strTemplate = _T("\
(*Global variable file*)\n\
(* this file is generated by ATCM LatComm Generator - do not edit! *)\n\
\n\
VAR_GLOBAL\n\
    LCData_%s_%s : LC%s_%s;\n\
    LCDataReady_%s_%s: BOOL := FALSE;\n\
    LCbError_%s_%s: BOOL := FALSE;\n\
    LCnError_%s_%s: DINT := LCERR_NO_ERROR;\n\
END_VAR\n\
");

    if(!pfGen) {
        ASSERT(pfGen);
        return(false);
    }

    try
	{
        CString strGen;

        strGen.Format(strTemplate, strName, strDataType, ccaTargetPrefixes[eTargetType], strDataType, strName, strDataType, strName, strDataType, strName, strDataType);
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}

/*
bool CLCGen::GenLGvlEnd(CStdioFile* pfGen) {
    const CString str = _T("\
");

    try{
        CString strGen;
        strGen.Format(str, strName);
        pfGen->WriteString(strGen);
    }
    catch(CFileException* e) {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
    return(true);
}
*/


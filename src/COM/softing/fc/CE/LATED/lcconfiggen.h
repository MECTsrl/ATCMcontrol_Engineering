#ifndef __GENERATE_H_
#define __GENERATE_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "stdafx.h"
#include "lcconfig.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

typedef CList<CString,CString&> TargetAddrs;

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*
class CLCGen : public CLCConfigListener
{
public:
    CLCGen();
    void Generate(CLCConfig * pConfig);

    void OnDataType(int id, CLCDataType *);
    void OnEndDataType(int id, CLCDataType *);
    void OnMember(int id, CLCDataType *, CLCMember*);
    void OnLink(int id, CLCLink *pSrc);
    void OnDestLink(int id, CLCLink *pSource, CLCDestLink *pDest);
    void OnEndLink(int id, CLCLink *);
private:
    CLCConfig*  m_pConfig;
    CString     m_strPrjPath;
    TargetAddrs m_TragetAddrs;
    bool m_bPCSend, m_bPCRec;   // true if PC as sender / receiver
    bool m_bECSend, m_bECRec;   // true if EC as sender / receiver
	bool m_bCMZSend, m_bCMZRec;	// true if CMZ as sender / receiver
    bool m_bAnyPCRec;           // true if any PC receives 
    CStdioFile *m_pfGenDTypeCMZ;
    CStdioFile *m_pfGenDTypeEC;
    CStdioFile *m_pfGenDTypePC;
    CStdioFile *m_pfGenDConst;
    CStdioFile *m_pfGenDStuffRecToCache;
    CStdioFile *m_pfGenDStuffConst;
    CStdioFile *m_pfGenDStuffUser;
    CStdioFile *m_pfGenDStuffCacheType;
    CStdioFile *m_pfGenDStuffGvl;
    CStdioFile *m_pfGenDCMZSend;
    CStdioFile *m_pfGenDECSend;
    CStdioFile *m_pfGenDPCSend;
    CStdioFile *m_pfGenDCMZRec;
    CStdioFile *m_pfGenDECRec;
    CStdioFile *m_pfGenDPCRec;

    CStdioFile *m_pfGenLAddr;
    CStdioFile *m_pfGenLCMZSendPrg;
    CStdioFile *m_pfGenLECSendPrg;
    CStdioFile *m_pfGenLPCSendPrg;
    CStdioFile *m_pfGenLCMZRecPrg;
    CStdioFile *m_pfGenLECRecPrg;
    CStdioFile *m_pfGenLPCRecPrg;
	CStdioFile *m_pfGenLGvlPrgCMZ;
    CStdioFile *m_pfGenLGvlPrgEC;
    CStdioFile *m_pfGenLGvlPrgPC;

    bool GenDTypeBeg(CStdioFile* pfGen, CString strName, ETargetType eTargetType);
    bool GenDTypeElem(CStdioFile* pfGen, CString strName, CString strType, BL_IEC_TYP type, bool, size_t, ETargetType eTargetType);
    bool GenDTypeEnd(CStdioFile* pfGen);

    bool GenDCMZSendBeg(CStdioFile* pfGen, CString strName);
    bool GenDCMZSendElem(CStdioFile* pfGen, CString strDataName, CString strMemberName, CString strType, BL_IEC_TYP type);
    bool GenDCMZSendEnd(CStdioFile* pfGen, CString strName);

    bool GenDECSendBeg(CStdioFile* pfGen, CString strName);
    bool GenDECSendElem(CStdioFile* pfGen, CString strDataName, CString strMemberName, CString strType, BL_IEC_TYP type);
    bool GenDECSendEnd(CStdioFile* pfGen, CString strName);

    bool GenDPCSendBeg(CStdioFile* pfGen, CString strName);
    bool GenDPCSendElem(CStdioFile* pfGen, CString strDataName, CString strMemberName, CString strType, BL_IEC_TYP type, size_t size);
    bool GenDPCSendEnd(CStdioFile* pfGen, CString strName);

    bool GenDCMZRecBeg(CStdioFile* pfGen, CString strName);
    bool GenDCMZRecElem(CStdioFile* pfGen, CString strDataName, CString strMemberName, CString strType, BL_IEC_TYP type, size_t size);
    bool GenDCMZRecEnd(CStdioFile* pfGen, CString strName);

    bool GenDECRecBeg(CStdioFile* pfGen, CString strName);
    bool GenDECRecElem(CStdioFile* pfGen, CString strDataName, CString strMemberName, CString strType, BL_IEC_TYP type, size_t size);
    bool GenDECRecEnd(CStdioFile* pfGen, CString strName);

    bool GenDPCRecBeg(CStdioFile* pfGen, CString strName);
    bool GenDPCRecElem(CStdioFile* pfGen, CString strDataName, CString strMemberName, CString strType, BL_IEC_TYP type, size_t size);
    bool GenDPCRecEnd(CStdioFile* pfGen, CString strName);

    bool GenDConst(CStdioFile* pfGen, CLCDataType *pDataType);

    bool GenDStuffRecToCache(CStdioFile* pfGen);
    bool GenDStuffConst(CStdioFile* pfGen);
    bool GenDStuffUser(CStdioFile* pfGen);
    bool GenDStuffCacheType(CStdioFile* pfGen);
    bool GenDStuffGvl(CStdioFile* pfGen);


    bool GenLAddrBeg(CStdioFile* pfGen);
    bool GenLAddrElem(CStdioFile* pfGen, CString strName, CString strIPAddr, ULONG ulIP);
    bool GenLAddrEnd(CStdioFile* pfGen);

    bool GenLCMZSendPrgBeg(CStdioFile* pfGen, CString strName, CString strDataType);
    bool GenLCMZSendPrgElem(CStdioFile* pfGen, CString strName, CString strSrcName, CString strDataType);
    bool GenLCMZSendPrgEnd(CStdioFile* pfGen);

    bool GenLECSendPrgBeg(CStdioFile* pfGen, CString strName, CString strDataType);
    bool GenLECSendPrgElem(CStdioFile* pfGen, CString strName, CString strSrcName, CString strDataType);
    bool GenLECSendPrgEnd(CStdioFile* pfGen);

    bool GenLPCSendPrgBeg(CStdioFile* pfGen, CString strName, CString strDataType);
    bool GenLPCSendPrgElem(CStdioFile* pfGen, CString strName, CString strSrcName, CString strDataType);
    bool GenLPCSendPrgEnd(CStdioFile* pfGen);

    bool GenLCMZRecPrgBeg(CStdioFile* pfGen, CString strName, CString strDataType);

    bool GenLECRecPrgBeg(CStdioFile* pfGen, CString strName, CString strDataType);

    bool GenLPCRecPrgBeg(CStdioFile* pfGen, CString strName, CString strDataType);

    bool GenLGvlBeg(CStdioFile* pfGen, CString strName, CString strDataType, ETargetType eTargetType);

};

#endif // __GENERATE_H_

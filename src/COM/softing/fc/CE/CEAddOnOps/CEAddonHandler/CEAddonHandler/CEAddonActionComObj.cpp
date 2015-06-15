
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include <comdef.h>
#include "Methods.h"
#include "CEAddonHandler.h"
#include "CEAddonActionComObj.h"

#include "CEProjInfo.h"
#include "CEContainer.h"
#include "CEContainer_i.c"

// MFC
#include <afxdlgs.h>
#include <afxcmn.h>

// Dialogs for AddOnOperations
#include "DialogFunctionBlock.h"
#include "DialogFunction.h"
#include "DialogProgram.h"
#include "DialogDataType.h"
#include "DialogConst.h"
#include "DialogConstantFile.h"
#include "DialogGlobVar.h"
#include "DialogGlobVarFile.h"
#include "DialogPB.h"
#include "DialogOpc.h"
#include "DialogTask.h"
#include "DialogPc.h"
#include "DialogResource.h"
#include "DialogType.h"
#include "DialogProgInst.h"
#include "DialogProj.h"

// operation classes for AddOnOperations
#include "InsertFunctionBlock.h"
#include "PropFunctionBlock.h"
#include "InsertResource.h"
#include "PropResource.h"
#include "InsertTask.h"
#include "PropTask.h"
#include "InsertFunction.h"
#include "PropFunction.h"
#include "InsertProgram.h"
#include "PropProgram.h"
#include "InsertProgramInst.h"
#include "PropProgramInst.h"
#include "InsertConstantFile.h"
#include "InsertConstant.h"
#include "InsertGlobVarFile.h"
#include "InsertGlobVar.h"
#include "InsertDataType.h"
#include "PropDataType.h"
#include "DeleteObject.h"
#include "RenameObject1.h"
#include "DeleteResource.h"
#include "DeleteTask.h"
#include "DeleteProgramInst.h"
#include "RenameResource.h"
#include "RenameTask.h"
#include "RenameProgramInst.h"
#include "InsertGvlImport.h"
#include "DeleteGvlImport.h"
#include "PropProject.h"
#include "CESysDef.h"
#include "XMLHelper/XMLNode.h"

#include "CEAddOnActionEvents_i.c"


//----  Static Initializations:   ----------------------------------*


/////////////////////////////////////////////////////////////////////////////
// CCEAddonActionComObj

STDMETHODIMP CCEAddonActionComObj::doAction(BSTR sMethodName,
                                            BSTR sSourceFile,
                                            BSTR sId,
                                            ICEProjInfo * pICEProjectInfo,
                                            BSTR sAdditionalInfo)
{

    ASSERT(pICEProjectInfo);

    HRESULT hr = S_OK;

    try
    {
        // $wi$: this is a hack to initialize the ICEProjInfo for further actions
	    CDialogConst *DFB;
        DFB = CDialogConst::CreateNewObject(CVIEWINSERT);
        CInsertConstant IFB(DFB,
                            sSourceFile,
                            sId,
                            pICEProjectInfo,
                            sAdditionalInfo);
        DFB->SetOperation(&IFB);
        CDialogConst::DeleteObject(DFB);
        // end hack

        CString strMethodName(sMethodName);

        if(strMethodName == DISP_IDS[0])
        {
	        CDialogFunction *DF1;
            DF1 = CDialogFunction::CreateNewObject(CVIEWINSERT);
            DF1->DoModal();

            ASSERT(pICEProjectInfo);
            HRESULT hRes = 0;

            hRes = pICEProjectInfo->fileChangedHint(sSourceFile);
	        ASSERT(SUCCEEDED(hRes));

            if(1)
            {
	            bstr_t * psId = new bstr_t(L"Id");
	            BSTR psParentSource = L"ParentSource";
	            BSTR psParentId = L"ParentId";
                ICEContainer * pICEContainer = NULL;

                HRESULT hRes = pICEProjectInfo->getContainerIF(sSourceFile,
                                                           *psId,
                                                           &pICEContainer,
                                                           &psParentSource,
                                                           &psParentId);

                CString strMsg = _T("call: ICEProjInfo::getContainerIF\n");
                strMsg += _T("\npsSourceFile:\t");
                strMsg += sSourceFile;
                strMsg += _T("\npsId:\t\t");
                strMsg += *psId;
                strMsg += _T("\npsParentSource:\t");
                strMsg += psParentSource;
                strMsg += _T("\npsParentId:\t");
                strMsg += psParentId;
                AfxMessageBox(strMsg);

                {
                    HRESULT hRes = S_OK;

                    _bstr_t * psLocation = new _bstr_t(L"Location");
                    _bstr_t * psLinkName = new _bstr_t(L"LinkName");
                    _bstr_t * psOldLinkName = new _bstr_t(L"OldLinkName");

                    hRes = ((ICEContainer*)pICEContainer)->createLink(sSourceFile, *psLocation, *psLinkName);
                    ASSERT(SUCCEEDED(hRes));

                    hRes = ((ICEContainer*)pICEContainer)->deleteLink(sSourceFile, *psLocation, *psLinkName);
                    ASSERT(SUCCEEDED(hRes));

                    hRes = ((ICEContainer*)pICEContainer)->renameLink(sSourceFile, *psLocation, *psOldLinkName, *psLinkName);
                    ASSERT(SUCCEEDED(hRes));

                    delete psLocation;
                    delete psLinkName;
                    delete psOldLinkName;
                }

                if(pICEContainer != NULL)
                {
                    pICEContainer->Release();
                    pICEContainer = NULL;
                }
            
                delete psId;
            }
        }
        else if(strMethodName == DISP_IDS[1])
        {//DeleteConstantList
            CDeleteObject IFB(NULL, //No dialog is needed
                              sSourceFile,
                              sId,
                              pICEProjectInfo,
                              sAdditionalInfo,
                              this);
            IFB.Execute();
        }
        else if(strMethodName == DISP_IDS[2])
        {//DeleteFunction
	        CDeleteObject IFB(NULL, //No dialog is needed
                              sSourceFile,
                              sId,
                              pICEProjectInfo,
                              sAdditionalInfo,
                              this);
            IFB.Execute();
        }
        else if(strMethodName == DISP_IDS[3])
        {//DeleteFunctionBlock
	        CDeleteObject IFB(NULL, //No dialog is needed
                              sSourceFile,
                              sId,
                              pICEProjectInfo,
                              sAdditionalInfo,
                              this);
            IFB.Execute();
        }
        else if(strMethodName == DISP_IDS[4])
        {//DeleteGlobalVariableList
            CDeleteObject IFB(NULL, //No dialog is needed
                              sSourceFile,
                              sId,
                              pICEProjectInfo,
                              sAdditionalInfo,
                              this);
            IFB.Execute();
        }
        else if(strMethodName == DISP_IDS[5])
        {//DeleteGlobalVariableListRef
	        CDeleteGvlImport IFB(NULL, //No dialog is needed
                              sSourceFile,
                              sId,
                              pICEProjectInfo,
                              sAdditionalInfo);
            IFB.Execute();
        }
    //    else if(strMethodName == DISP_IDS[6])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[7])
    //    {
    //	    ;
    //    }
		// d-186 07.02.05 SIS >>
		// added case DeleteVariable
        else if(strMethodName == DISP_IDS[8])
        {
			CString		sVarId(sId);
			CComBSTR	sVar;
			int		iFound = sVarId.ReverseFind(_T('/'));
			sVar = sVarId.Mid(iFound + 1);
    	    DeleteVariable(sSourceFile, pICEProjectInfo, sVar);
        }
		// d-186 07.02.05 SIS <<
        else if(strMethodName == DISP_IDS[9])
        {//DeleteProgram
	        CDeleteObject IFB(NULL, //No dialog is needed
                              sSourceFile,
                              sId,
                              pICEProjectInfo,
                              sAdditionalInfo,
                              this);
            IFB.Execute();
        }
        else if(strMethodName == DISP_IDS[10])
        {//DeleteProgramInstance
	        CDeleteProgramInst IFB(NULL, //No dialog is needed
                                   sSourceFile,
                                   sId,
                                   pICEProjectInfo,
                                   sAdditionalInfo);
            IFB.Execute();
        }
        else if(strMethodName == DISP_IDS[11])
        {//DeleteResource
	        CDeleteResource IFB(NULL, //No dialog is needed
                                sSourceFile,
                                sId,
                                pICEProjectInfo,
                                sAdditionalInfo,
                                this);
            IFB.Execute();
        }
        else if(strMethodName == DISP_IDS[12])
        {//DeleteStructure
	        CDeleteObject IFB(NULL, //No dialog is needed
                              sSourceFile,
                              sId,
                              pICEProjectInfo,
                              sAdditionalInfo,
                              this);
            IFB.Execute();
        }
        else if(strMethodName == DISP_IDS[13])
        {//DeleteTask
	        CDeleteTask IFB(NULL, //No dialog is needed
                            sSourceFile,
                            sId,
                            pICEProjectInfo,
                            sAdditionalInfo);
            IFB.Execute();
        }
    //    else if(strMethodName == DISP_IDS[14])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[15])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[16])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[17])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[18])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[19])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[20])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[21])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[22])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[23])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[24])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[25])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[26])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[27])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[28])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[29])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[30])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[31])
    //    {
    //	    ;
    //    }
        else if(strMethodName == DISP_IDS[32])
        {//Insert Constant
	        CDialogConst *DFB;
            DFB = CDialogConst::CreateNewObject(CVIEWINSERT);
            CInsertConstant IFB(DFB,
                                sSourceFile,
                                sId,
                                pICEProjectInfo,
                                sAdditionalInfo);
            DFB->SetOperation(&IFB);

            DFB->DoModal();

            CDialogConst::DeleteObject(DFB);
        }
        else if(strMethodName == DISP_IDS[33])
        {  //Insert Constant List
	        CDialogConstantFile *DFB;
            DFB = CDialogConstantFile::CreateNewObject(CVIEWINSERT);
            CInsertConstantFile IFB(DFB,
                                    sSourceFile,
                                    sId,
                                    pICEProjectInfo,
                                    sAdditionalInfo);
            DFB->SetOperation(&IFB);

            DFB->DoModal();

            CDialogConstantFile::DeleteObject(DFB);
        }
        else if(strMethodName == DISP_IDS[34])
        {//Insert DataType
	        CDialogDataType *DFB;
            DFB = CDialogDataType::CreateNewObject(CVIEWINSERT);
            CInsertDataType IFB(DFB,
                                sSourceFile,
                                sId,
                                pICEProjectInfo,
                                sAdditionalInfo);
            DFB->SetOperation(&IFB);

            DFB->DoModal();

            CDialogDataType::DeleteObject(DFB);
        }
    //    else if(strMethodName == DISP_IDS[35])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[36])
    //    {
    //	    ;
    //    }
        else if(strMethodName == DISP_IDS[37])
        {//Insert Function
            CDialogFunction *DFB;
            DFB = CDialogFunction::CreateNewObject(CVIEWINSERT);
            CInsertFunction IFB(DFB,
                                sSourceFile,
                                sId,
                                pICEProjectInfo,
                                sAdditionalInfo);
            DFB->SetOperation(&IFB);
            DFB->DoModal();
            CDialogFunction::DeleteObject(DFB);
        }
        else if(strMethodName == DISP_IDS[38])
        {   // insert function block
	        CDialogFunctionBlock *DFB;
            DFB = CDialogFunctionBlock::CreateNewObject(CVIEWINSERT);
            CInsertFunctionBlock IFB(DFB,
                                     sSourceFile,
                                     sId,
                                     pICEProjectInfo,
                                     sAdditionalInfo);
            DFB->SetOperation(&IFB);
            DFB->DoModal();
            CDialogFunctionBlock::DeleteObject(DFB);
        }
        else if(strMethodName == DISP_IDS[39])
        {   // insert Global Variable
	        CDialogGlobVar *DFB;
            DFB = CDialogGlobVar::CreateNewObject(CVIEWINSERT);
            CInsertGlobVar IFB(DFB,
                               sSourceFile,
                               sId,
                               pICEProjectInfo,
                               sAdditionalInfo);
            DFB->SetOperation(&IFB);
            DFB->DoModal();
            CDialogGlobVar::DeleteObject(DFB);
        }
        else if(strMethodName == DISP_IDS[40])
        {   // insert Global Variable List
	        CDialogGlobVarFile *DFB;
            DFB = CDialogGlobVarFile::CreateNewObject(CVIEWINSERT);
            CInsertGlobVarFile IFB(DFB,
                                   sSourceFile,
                                   sId,
                                   pICEProjectInfo,
                                   sAdditionalInfo);
            DFB->SetOperation(&IFB);
            DFB->DoModal();
            CDialogGlobVarFile::DeleteObject(DFB);
        }
        else if(strMethodName == DISP_IDS[41])
        {   // insert Global Variable List Ref
            CInsertGvlImport IFB(NULL,
                                   sSourceFile,
                                   sId,
                                   pICEProjectInfo,
                                   sAdditionalInfo);
            IFB.Execute();
        }
    //    else if(strMethodName == DISP_IDS[42])
    //    {   
    //        ;
    //    }
        else if(strMethodName == DISP_IDS[43])
        {   // insert Program
	        CDialogProgram *DFB;
            DFB = CDialogProgram::CreateNewObject(CVIEWINSERT);
            CInsertProgram IFB(DFB,
                               sSourceFile,
                               sId,
                               pICEProjectInfo,
                               sAdditionalInfo);
            DFB->SetOperation(&IFB);
            DFB->DoModal();
            CDialogProgram::DeleteObject(DFB);
        }
        else if(strMethodName == DISP_IDS[44])
        {   // insert ProgramInstance
	        CDialogProgInst *DFB;
            DFB = CDialogProgInst::CreateNewObject(CVIEWINSERT);
            CInsertProgramInst IFB(DFB,
                                   sSourceFile,
                                   sId,
                                   pICEProjectInfo,
                                   sAdditionalInfo);
            DFB->SetOperation(&IFB);
            DFB->DoModal();
            CDialogProgInst::DeleteObject(DFB);
        }
        else if(strMethodName == DISP_IDS[45])
        {   //Insert Resource
	        CDialogResource *DRES;
            DRES = CDialogResource::CreateNewObject(CVIEWINSERT);
            CInsertResource IRES(DRES,
                                 sSourceFile,
                                 sId,
                                 pICEProjectInfo,
                                 sAdditionalInfo);
            DRES->SetOperation(&IRES);

            DRES->DoModal();

            CDialogResource::DeleteObject(DRES);
        }
        else if(strMethodName == DISP_IDS[46])
        {   //Insert Task
	        CDialogTask *DTASK;
            DTASK = CDialogTask::CreateNewObject(CVIEWINSERT);
            CInsertTask ITASK(DTASK,
                                  sSourceFile,
                                  sId,
                                  pICEProjectInfo,
                                  sAdditionalInfo);
            DTASK->SetOperation(&ITASK);

            DTASK->DoModal();

            CDialogTask::DeleteObject(DTASK);
        }
    //    else if(strMethodName == DISP_IDS[47])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[48])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[49])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[50])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[51])
    //    {   
    //        ;
    //    }
        else if(strMethodName == DISP_IDS[52])
        {   //Prop Function
            CDialogFunction *DFB;
            DFB = CDialogFunction::CreateNewObject(CVIEWPROP);
            CPropFunction IFB(DFB,
                              sSourceFile,
                              sId,
                              pICEProjectInfo,
                              sAdditionalInfo);
            DFB->SetOperation(&IFB);

            DFB->DoModal();

            CDialogFunction::DeleteObject(DFB);
        }
        else if(strMethodName == DISP_IDS[53])
        {   // Prop function block
	        CDialogFunctionBlock *DFB;
            DFB = CDialogFunctionBlock::CreateNewObject(CVIEWPROP);
            CPropFunctionBlock IFB(DFB,
                                   sSourceFile,
                                   sId,
                                   pICEProjectInfo,
                                   sAdditionalInfo);
            DFB->SetOperation(&IFB);

            DFB->DoModal();

            CDialogFunctionBlock::DeleteObject(DFB);
        }
    //    else if(strMethodName == DISP_IDS[54])
    //    {   
    //        ;
    //    }
    //    else if(strMethodName == DISP_IDS[55])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[56])
    //    {
    //	    ;
    //    }
        else if(strMethodName == DISP_IDS[57])
        {   //Prop Resource
	        CDialogResource *DRES;
            DRES = CDialogResource::CreateNewObject(CVIEWPROP);
            CPropResource IRES(DRES,
                               sSourceFile,
                               sId,
                               pICEProjectInfo,
                               sAdditionalInfo);
            DRES->SetOperation(&IRES);

            DRES->DoModal();

            CDialogResource::DeleteObject(DRES);
        }
        else if(strMethodName == DISP_IDS[58])
        {   //Prop structure
	        CDialogDataType *DFB;
            DFB = CDialogDataType::CreateNewObject(CVIEWPROP);
            CPropDataType IFB(DFB,
                              sSourceFile,
                              sId,
                              pICEProjectInfo,
                              sAdditionalInfo);
            DFB->SetOperation(&IFB);

            DFB->DoModal();

            CDialogDataType::DeleteObject(DFB);
        }
        else if(strMethodName == DISP_IDS[59])
        {   //Prop Task
	        CDialogTask *DTASK;
            DTASK = CDialogTask::CreateNewObject(CVIEWPROP);
            CPropTask ITASK(DTASK,
                            sSourceFile,
                            sId,
                            pICEProjectInfo,
                            sAdditionalInfo);
            DTASK->SetOperation(&ITASK);

            DTASK->DoModal();

            CDialogTask::DeleteObject(DTASK);
        }
    //    else if(strMethodName == DISP_IDS[60])
    //    {
    //	    ;
    //    }
        else if(strMethodName == DISP_IDS[61])
        {   //Prop Program
	        CDialogProgram *DFB;
            DFB = CDialogProgram::CreateNewObject(CVIEWPROP);
            CPropProgram IFB(DFB,
                             sSourceFile,
                             sId,
                             pICEProjectInfo,
                             sAdditionalInfo);
            DFB->SetOperation(&IFB);

            DFB->DoModal();

            CDialogProgram::DeleteObject(DFB);
        }
        else if(strMethodName == DISP_IDS[62])
        {   //Prop Resource
	        CDialogResource *DRES;
            DRES = CDialogResource::CreateNewObject(CVIEWPROP);
            CPropResource IRES(DRES,
                               sSourceFile,
                               sId,
                               pICEProjectInfo,
                               sAdditionalInfo);
            DRES->SetOperation(&IRES);

            DRES->DoModal();

            CDialogResource::DeleteObject(DRES);
        }
    //    else if(strMethodName == DISP_IDS[63])
    //    {
    //	    ;
    //    }
        else if(strMethodName == DISP_IDS[64])
        {//RenameFunction
	        CRenameObject IFB(NULL, //No dialog is needed
                              sSourceFile,
                              sId,
                              pICEProjectInfo,
                              sAdditionalInfo,
                              this);
            IFB.Execute(); 
        }
        else if(strMethodName == DISP_IDS[65])
        {//RenameFunctionBlock
	        CRenameObject IFB(NULL, //No dialog is needed
                              sSourceFile,
                              sId,
                              pICEProjectInfo,
                              sAdditionalInfo,
                              this);
            IFB.Execute();
        }
        else if(strMethodName == DISP_IDS[66])
        {//RenameProgram
	        CRenameObject IFB(NULL, //No dialog is needed
                              sSourceFile,
                              sId,
                              pICEProjectInfo,
                              sAdditionalInfo,
                              this);
            IFB.Execute();
        }
        else if(strMethodName == DISP_IDS[67])
        {//RenameProgramInstance
	        CRenameProgramInst IFB(NULL, //No dialog is needed
                                   sSourceFile,
                                   sId,
                                   pICEProjectInfo,
                                   sAdditionalInfo,
                                   this);
            IFB.Execute();
        }
        else if(strMethodName == DISP_IDS[68])
        {//RenameResource
	        CRenameResource IFB(NULL, //No dialog is needed
                                sSourceFile,
                                sId,
                                pICEProjectInfo,
                                sAdditionalInfo,
                                this);
            IFB.Execute();
        }
        else if(strMethodName == DISP_IDS[69])
        {//RenameStructure
	        CRenameObject IFB(NULL, //No dialog is needed
                              sSourceFile,
                              sId,
                              pICEProjectInfo,
                              sAdditionalInfo,
                              this);
            IFB.Execute();
        }
        else if(strMethodName == DISP_IDS[70])
        {//RenameTask
	        CRenameTask IFB(NULL, //No dialog is needed
                            sSourceFile,
                            sId,
                            pICEProjectInfo,
                            sAdditionalInfo,
                            this);
            IFB.Execute();
        }
    //    else if(strMethodName == DISP_IDS[71])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[72])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[73])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[74])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[75])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[76])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[77])
    //    {
    //	    ;
    //    }
    //    else if(strMethodName == DISP_IDS[78])
    //    {
    //	    ;
    //    }
        else if(strMethodName == DISP_IDS[79])
        {//Prop ProgramInst
	        CDialogProgInst *DFB;
            DFB = CDialogProgInst::CreateNewObject(CVIEWPROP);
            CPropProgramInst IFB(DFB,
                                 sSourceFile,
                                 sId,
                                 pICEProjectInfo,
                                 sAdditionalInfo);
            DFB->SetOperation(&IFB);

            DFB->DoModal();

            CDialogProgInst::DeleteObject(DFB);
        }
        else if(strMethodName == DISP_IDS[80])
        {//Prop Project
	        CDialogProj *DFB;
            DFB = CDialogProj::CreateNewObject(CVIEWPROP);
            CPropProject IFB(DFB,
                                 sSourceFile,
                                 sId,
                                 pICEProjectInfo,
                                 sAdditionalInfo);
            DFB->SetOperation(&IFB);

            DFB->DoModal();

            CDialogProj::DeleteObject(DFB);
        }
        else
        {
            ;
        }
    }
    catch(HRESULT hrRes) 
    {
        hr = hrRes;
    }
    catch(CMemoryException* e) 
    {
        hr = E_OUTOFMEMORY;
        e->Delete();
    }
    catch(CFileException* e) 
    {
        hr = E_FAIL;
        e->Delete();
    }
    catch(...)
    {
        hr = E_FAIL;
    }

	return hr;
}



//------------------------------------------------------------------*
/**
 * Function name			CCEAddonActionComObj::InsertVariable
 * Description		Used from the POU selection to insert a variable.	
 * @param			BSTR sFileName: file name where the changes has to occur
 * @param			ICEProjInfo * pProjInfo: project info sented from the project manager
 * @param			BSTR sName: name of the varaiable to be inserted
 * @param			BSTR sVarClass: storage class of the variable
 * @param			BSTR sDataType: IEC type of the variable
 * @param			BSTR sAddress: optional direct address
 * @param			BSTR sAlias: Alias of the variable
 * @param			BSTR sComment: Comment
 * @param			BSTR sInitValue: Initial value
 * @return			STDMETHODIMP : S_OK if succesful
 * @exception			
 * @see			    COperation, CVar, POU
*/
STDMETHODIMP CCEAddonActionComObj::InsertVariable(BSTR sFileName, ICEProjInfo * pProjInfo, BSTR sName, BSTR sVarClass, BSTR sDataType, BSTR sAddress, BSTR sAlias, BSTR sComment, BSTR sInitValue)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    
    HRESULT hr = S_OK;

    try
    {
        //locals
        CVar Var; //class to hold the variable info's
        COperation Operation(NULL, //Dialog
                             sFileName, //Source file
                             NULL, //sId
                             pProjInfo,
                             NULL); //Additional info
        POU* pPou = NULL; //Pou object sented from the parser

        //Get the params
        CString strFileName = sFileName;
        CString strVarName = sName;
        CString strVarClass = sVarClass;
        CString strDataType = sDataType;
        CString strAddress = sAddress;
        CString strAlias = sAlias;
        CString strComment = sComment;
        CString strInitValue = sInitValue;
        CString strHint = _T("Add"); //variable will be added
    
        //file name, project info, variable name , storage class and Datatype
        //could not be empty
        ASSERT((!strFileName.IsEmpty()) && pProjInfo
            && (!strVarName.IsEmpty()) && (!strVarClass.IsEmpty())
            && (!strDataType.IsEmpty()));
    
        //Initialise CVar
        Var.SetObjName(strVarName);
        int iClass = Operation.VarStorageClassByString(strVarClass);
        ASSERT(iClass != -1);
        Var.SetVarAttr(iClass);
        Var.SetVarStorageClass(strVarClass);
        Var.SetVarType(strDataType);
        Var.SetAddress(strAddress);
        Var.SetObjAlias(strAlias);
        Var.SetObjVarComment(strComment);
        Var.SetVarInitVal(strInitValue);
        Var.SetHint(strHint);
        //set the file name
        Operation.m_sFileName = strFileName;
        //Read the file content and Initialise from it
        string buf;
        Operation.ReadFileContent(Operation.m_ProjectPath + _T("\\") + strFileName, buf);
        CString strFileContent = buf.c_str();
        pPou = Operation.ParsePou(strFileContent, pPou);
        if (!pPou)
        {
            return E_FAIL;
        }
        Operation.InitFromFile(pPou); 
        //add the var
        Operation.AddVar(Var);
        //Write the file new
        Operation.InsertVarList(strFileContent, pPou);
        Operation.InsertSysComment(strFileContent);
        Operation.WriteHeader(strFileContent);
        //Inform the project manager about the changes
        hr = pProjInfo->fileChangedHint(sFileName);
        ASSERT(hr == S_OK);
    
        delete pPou;
    }
    catch(HRESULT hrRes) 
    {
        hr = hrRes;
    }
    catch(CMemoryException* e) 
    {
        hr = E_OUTOFMEMORY;
        e->Delete();
    }
    catch(CFileException* e) 
    {
        hr = E_FAIL;
        e->Delete();
    }
    catch(...)
    {
        hr = E_FAIL;
    }

    return hr;
}


//------------------------------------------------------------------*
/**
 * Function name			CCEAddonActionComObj::DeleteVariable
 * Description		Used from the pou selection to delete a variable	
 * @param			BSTR sFileName: file name where the changes will occur
 * @param			ICEProjInfo * pProjInfo: containing the info's about the project 
 * @param			BSTR sName: Name of the variable to be deleted
 * @return			STDMETHODIMP: S_OK if succesful 
 * @exception			
 * @see			    COperation, CVar, POU
*/
STDMETHODIMP CCEAddonActionComObj::DeleteVariable(BSTR sFileName, ICEProjInfo * pProjInfo, BSTR sName)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    HRESULT hr = S_OK;

    try
    {
        //locals
        CVar Var; //class used to hold the variable info's
        COperation Operation(NULL, //Dialog
                             sFileName, //Source file
                             NULL, //sId
                             pProjInfo,
                             NULL); //Additional info
        POU* pPou = NULL; //POU object sented from the parser
        POSITION Pos; //used to store the actaual position of the variable in the list

        //Get the params
        CString strFileName = sFileName;
        CString strVarName = sName;
        CString strHint = _T("Del"); //variable will be deleted 

        //file name, project info, variable name
        //could not be empty
        ASSERT((!strFileName.IsEmpty()) && pProjInfo
            && (!strVarName.IsEmpty()));

        //Free the BSTR
        SysFreeString(sName);
        //set the file name
        Operation.m_sFileName = strFileName;
        //Read the file content and Initialise from it
        string buf;
        Operation.ReadFileContent(Operation.m_ProjectPath + _T("\\") + strFileName, buf);
        CString strFileContent = buf.c_str();
        pPou = Operation.ParsePou(strFileContent, pPou);
        if (!pPou)
        {
            return E_FAIL;
        }
        Operation.InitFromFile(pPou); 
        //Get the variable index in the list
        Pos = Operation.GetVarPos(strVarName);
        if (Pos!=NULL)
        {
            Var = Operation.m_aVarList.GetAt(Pos);
            Var.SetHint(strHint);
            //replace the variable
            Operation.m_aVarList.SetAt(Pos, Var);
            //Write the file new
            Operation.InsertVarList(strFileContent, pPou);
            Operation.InsertSysComment(strFileContent);
            Operation.WriteHeader(strFileContent);
            //Inform the project manager about the changes
            hr = pProjInfo->fileChangedHint(sFileName);
            ASSERT(hr == S_OK);
        }    
        delete pPou;
    }
    catch(HRESULT hrRes) 
    {
        hr = hrRes;
    }
    catch(CMemoryException* e) 
    {
        hr = E_OUTOFMEMORY;
        e->Delete();
    }
    catch(CFileException* e) 
    {
        hr = E_FAIL;
        e->Delete();
    }
    catch(...)
    {
        hr = E_FAIL;
    }

    return hr;
}


//------------------------------------------------------------------*
/**
 * Function name			CCEAddonActionComObj::ModifyVariable
 * Description		Used from the pou selection to modify a variable	
 * @param			BSTR sFileName: file name where the changes has to be taken
 * @param			ICEProjInfo * pProjInfo: containing the project info's
 * @param			BSTR sOldName: old name of the variable
 * @param			BSTR sName: new name of the variable
 * @param			BSTR sVarClass: storage class
 * @param			BSTR sDataType: IEC data type
 * @param			BSTR sAddress: optional direct address
 * @param			BSTR sAlias: alias
 * @param			BSTR sComment: comment
 * @param			BSTR sInitValue: Initial value
 * @return			STDMETHODIMP : S_OK if succesful
 * @exception			
 * @see			CVar, POU, COperation
*/
STDMETHODIMP CCEAddonActionComObj::ModifyVariable(BSTR sFileName, ICEProjInfo * pProjInfo, BSTR sOldName, BSTR sName, BSTR sVarClass, BSTR sDataType, BSTR sAddress, BSTR sAlias, BSTR sComment, BSTR sInitValue)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    HRESULT hr = S_OK;

    try
    {
        //locals
        CVar Var; //class used to hold the variable info's
        COperation Operation(NULL, //Dialog
                             sFileName, //Source file
                             NULL, //sId
                             pProjInfo,
                             NULL); //Additional info
        POU* pPou = NULL; //POU object sented from the parser
        POSITION Pos; //used to store the actaual position of the variable in the list
        HRESULT hr = S_OK;
    
        //Get the params
        CString strFileName = sFileName;
        CString strVarName = sName;
        CString strOldVarName = sOldName;
        CString strVarClass = sVarClass;
        CString strDataType = sDataType;
        CString strAddress = sAddress;
        CString strAlias = sAlias;
        CString strComment = sComment;
        CString strInitValue = sInitValue;
        CString strHintMod = _T("Mod"); //variable will be modified
        CString strHintModStCl = _T("ModStrClass"); //storage class of the variable will be changed

        //file name, project info, variable name (old and new), storage class and Datatype
        //could not be empty
        ASSERT((!strFileName.IsEmpty()) && pProjInfo
            && (!strVarName.IsEmpty()) && (!strOldVarName.IsEmpty())
            && (!strVarClass.IsEmpty()) && (!strDataType.IsEmpty()));

    
        //set the file name
        Operation.m_sFileName = strFileName;
        //Read the file content and Initialise from it
        string buf;
        Operation.ReadFileContent(Operation.m_ProjectPath + _T("\\") + strFileName, buf);
        CString strFileContent = buf.c_str();
        pPou = Operation.ParsePou(strFileContent, pPou);
        if (!pPou)
        {
            return E_FAIL;
        }
        Operation.InitFromFile(pPou); 
        //Get the variable index in the list
        Pos = Operation.GetVarPos(strOldVarName);
        if (Pos==NULL)
        {
            // variable not found
            delete pPou;
            return S_FALSE;
        }
        Var = Operation.m_aVarList.GetAt(Pos);
        //Set the variable values
        if(!Var.GetVarStorageClass().CompareNoCase(strVarClass))
        {
            Var.SetHint(strHintMod);
        }
        else
        {//if the storage class was changed
            Var.SetHint(strHintModStCl);
        }
        Var.SetObjName(strVarName);
        int iClass = Operation.VarStorageClassByString(strVarClass);
        ASSERT(iClass != -1);
        Var.SetVarAttr(iClass);
        Var.SetVarStorageClass(strVarClass);
        Var.SetVarType(strDataType);
        Var.SetAddress(strAddress);
        Var.SetObjAlias(strAlias);
        Var.SetObjVarComment(strComment);
        Var.SetVarInitVal(strInitValue);
        //replace the variable
        Operation.m_aVarList.SetAt(Pos, Var);
        //Write the file new
        Operation.InsertVarList(strFileContent, pPou);
        Operation.InsertSysComment(strFileContent);
        Operation.WriteHeader(strFileContent);
        //inform the project manager about the changes
        hr = pProjInfo->fileChangedHint(sFileName);
        ASSERT(hr == S_OK);
   
        delete pPou;
    }
    catch(HRESULT hrRes) 
    {
        hr = hrRes;
    }
    catch(CMemoryException* e) 
    {
        hr = E_OUTOFMEMORY;
        e->Delete();
    }
    catch(CFileException* e) 
    {
        hr = E_FAIL;
        e->Delete();
    }
    catch(...)
    {
        hr = E_FAIL;
    }

    return hr;
}

// ICEDragDropAction
//------------------------------------------------------------------*
/**
 * OnDrop: drop a node onto another node
 *
 * @param           pDragNode:      pointer to drag node (IXMLDOMElement)
 * @param           sDropSource:    the source file of the drop node
 * @param           sDropId:        the id path to the drop node
 * @param           sDropType:      the KAD type of the drop node
 * @param           dwDropEffect:   the drop effect of the operation
 *                                  1: COPY (CTRL), 2: MOVE (SHIFT), 4: LINK (NOTHING)
 * @param           pICEProjectInfo : a pointer to the project manager interface
 * @exception       -
 * @see             
*/
STDMETHODIMP CCEAddonActionComObj::OnDrop
(
    IUnknown* pDragNode,
    BSTR sDropSource,
    BSTR sDropId,
    BSTR sDropType,
    DWORD dwDropEffect,
    ICEProjInfo* pICEProjectInfo
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    HRESULT hr = S_OK;

    try
    {
        CComBSTR    sAdditionalInfo;
        CXMLNode    tNode;
        CString     strDropSource(sDropSource);
        CString     strDropId(sDropId);
        CString     strDropType(sDropType);

        CString     strDragType;
        CString     strDragId;
        CString     strDragSource;

        if(!tNode.SetIXMLNode(pDragNode))
        {
            return E_FAIL;
        }
        if(!tNode.GetAttribute(CE_XMLATTR_TYPE, strDragType))
        {
            return E_FAIL;
        }
        if(!tNode.GetAttribute(CE_XMLATTR_ID, strDragId))
        {
            return E_FAIL;
        }

        if(strDragType == _T("PROGRAM"))
        {
            if(strDropType == _T("TASK"))
            {
                CInsertProgramInst IFB(NULL,
                                       sDropSource,
                                       sDropId,
                                       pICEProjectInfo,
                                       sAdditionalInfo);
                IFB.SetAutomationMode(TRUE);
                IFB.SetProgramClass(strDragId);
                IFB.Execute();
            }
        }
        else if(strDragType == _T("GLOBAL_VAR_FILE"))
        {
            if(!tNode.GetAttribute(CE_XMLATTR_SOURCE, strDragSource))
            {
                return E_FAIL;
            }
            if(strDropType == _T("RESOURCE"))
            {
                CInsertGvlImport   IFB(NULL,
                                       sDropSource,
                                       sDropId,
                                       pICEProjectInfo,
                                       sAdditionalInfo);
                IFB.SetAutomationMode(TRUE);
                IFB.AddImportFile(strDragSource);
                IFB.Execute();
            }
        }
    }
    catch(HRESULT hrRes) 
    {
        hr = hrRes;
    }
    catch(CMemoryException* e) 
    {
        hr = E_OUTOFMEMORY;
        e->Delete();
    }
    catch(CFileException* e) 
    {
        hr = E_FAIL;
        e->Delete();
    }
    catch(...)
    {
        hr = E_FAIL;
    }

    return hr;
}


// ICEIECResourceOp
STDMETHODIMP CCEAddonActionComObj::InsertGVLImport
(
    ICEProjInfo * pProjInfo,
    BSTR sResourceName, 
    BSTR sGVLName
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    HRESULT hr = S_OK;


    try
    {
        CString strResourceFile(sResourceName);
        CString strGVLName(sGVLName);

        int i = strResourceFile.ReverseFind(_T('.'));
        if (i<0)
        {
            strResourceFile += _T(".res");
        }
        i = strGVLName.ReverseFind(_T('.'));
        if (i<0)
        {
            strGVLName += _T(".gvl");
        }

        CComBSTR sRes(strResourceFile);
        CComBSTR sAdd(_T("SILENT"));

        CInsertGvlImport IFB(NULL,
                             sRes,
                             NULL,
                             pProjInfo,
                             sAdd);
        IFB.SetAutomationMode(TRUE);
        IFB.AddImportFile(strGVLName);
        IFB.Execute();




    }
    catch(HRESULT hrRes) 
    {
        hr = hrRes;
    }
    catch(CMemoryException* e) 
    {
        hr = E_OUTOFMEMORY;
        e->Delete();
    }
    catch(CFileException* e) 
    {
        hr = E_FAIL;
        e->Delete();
    }
    catch(...)
    {
        hr = E_FAIL;
    }


    return hr;
}

STDMETHODIMP CCEAddonActionComObj::RemoveGVLImport
(
    ICEProjInfo * pProjInfo,
    BSTR sResourceName, 
    BSTR sGVLName
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    HRESULT hr = S_OK;


    try
    {
        CString strResourceFile(sResourceName);
        CString strGVLName(sGVLName);

        int i = strResourceFile.ReverseFind(_T('.'));
        if (i<0)
        {
            strResourceFile += _T(".res");
        }
        i = strGVLName.ReverseFind(_T('.'));
        if (i<0)
        {
            strGVLName += _T(".gvl");
        }

        CComBSTR sRes(strResourceFile);
        CComBSTR sAdd(_T("SILENT"));

        CDeleteGvlImport IFB(NULL,
                             sRes,
                             NULL,
                             pProjInfo,
                             sAdd);
        IFB.SetAutomationMode(TRUE);
        IFB.AddDeleteFile(strGVLName);
        IFB.Execute();




    }
    catch(HRESULT hrRes) 
    {
        hr = hrRes;
    }
    catch(CMemoryException* e) 
    {
        hr = E_OUTOFMEMORY;
        e->Delete();
    }
    catch(CFileException* e) 
    {
        hr = E_FAIL;
        e->Delete();
    }
    catch(...)
    {
        hr = E_FAIL;
    }


    return hr;
}



STDMETHODIMP CCEAddonActionComObj::InsertProgramInstance
(
    ICEProjInfo* pProjInfo,
    BSTR sResourceName,
    BSTR sTaskName,
    BSTR sProgClass,
    BSTR sProgInstName,
    BOOL bPosFirst
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    HRESULT hr = S_OK;


    try
    {
        CString strResourceFile(sResourceName);

        int i = strResourceFile.ReverseFind(_T('.'));
        if (i<0)
        {
            strResourceFile += _T(".res");
        }

        CComBSTR sRes(strResourceFile);
        CComBSTR sAdd(_T("SILENT"));

        CInsertProgramInst IFB(NULL,
                               sRes,
                               sTaskName,
                               pProjInfo,
                               sAdd);
        IFB.SetAutomationMode(TRUE);
        IFB.SetProgramClass(sProgClass);
        IFB.SetProgramInstName(sProgInstName);
        IFB.SetProgramPos(bPosFirst);

        IFB.Execute();
    }
    catch(HRESULT hrRes) 
    {
        hr = hrRes;
    }
    catch(CMemoryException* e) 
    {
        hr = E_OUTOFMEMORY;
        e->Delete();
    }
    catch(CFileException* e) 
    {
        hr = E_FAIL;
        e->Delete();
    }
    catch(...)
    {
        hr = E_FAIL;
    }


    return hr;
}

STDMETHODIMP CCEAddonActionComObj::RemoveProgramInstance
(
    ICEProjInfo* pProjInfo,
    BSTR sResourceName,
    BSTR sProgInstName
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    HRESULT hr = S_OK;


    try
    {
        CString strResourceFile(sResourceName);

        int i = strResourceFile.ReverseFind(_T('.'));
        if (i<0)
        {
            strResourceFile += _T(".res");
        }

        CComBSTR sRes(strResourceFile);
        CComBSTR sAdd(_T("SILENT"));

        CDeleteProgramInst IFB(NULL,
                               sRes,
                               sProgInstName,
                               pProjInfo,
                               sAdd);
        IFB.SetAutomationMode(TRUE);
        IFB.Execute();
    }
    catch(HRESULT hrRes) 
    {
        hr = hrRes;
    }
    catch(CMemoryException* e) 
    {
        hr = E_OUTOFMEMORY;
        e->Delete();
    }
    catch(CFileException* e) 
    {
        hr = E_FAIL;
        e->Delete();
    }
    catch(...)
    {
        hr = E_FAIL;
    }


    return hr;
}

STDMETHODIMP CCEAddonActionComObj::InsertTask
(
    ICEProjInfo* pProjInfo,
    BSTR sResourceName,
    BSTR sTaskName,
    BSTR sInterval,
    BSTR sPriority,
    BOOL bAutostart,
	BOOL bLoaded,	// NFTASKHANDLING 19.05.05 SIS
    BSTR sIOSync,
    BOOL bPosFirst
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    HRESULT hr = S_OK;


    try
    {
        CString strResourceFile(sResourceName);

        int i = strResourceFile.ReverseFind(_T('.'));
        if (i<0)
        {
            strResourceFile += _T(".res");
        }

        CComBSTR sRes(strResourceFile);
        CComBSTR sAdd(_T("SILENT"));

        CInsertTask IFB(NULL,
                        sRes,
                        NULL,
                        pProjInfo,
                        sAdd);
        IFB.SetAutomationMode(TRUE);
        IFB.SetAutoStart(bAutostart);
		IFB.SetLoaded(bLoaded);		// NFTASKHANDLING 19.05.05 SIS
        IFB.SetTaskName(sTaskName);
        if (sInterval!=NULL)
        {
            IFB.SetInterval(sInterval);
        }
        if (sPriority!=NULL)
        {
            IFB.SetPriority(sPriority);
        }
        if (sIOSync!=NULL)
        {
            IFB.SetIOSync(sIOSync);
        }
        IFB.SetGenPos(bPosFirst);
        IFB.Execute();
    }
    catch(HRESULT hrRes) 
    {
        hr = hrRes;
    }
    catch(CMemoryException* e) 
    {
        hr = E_OUTOFMEMORY;
        e->Delete();
    }
    catch(CFileException* e) 
    {
        hr = E_FAIL;
        e->Delete();
    }
    catch(...)
    {
        hr = E_FAIL;
    }


    return hr;
}



STDMETHODIMP CCEAddonActionComObj::InsertTaskWithAttributes
(
    ICEProjInfo* pProjInfo,
    BSTR sResourceName,
    BSTR sTaskName,
    SAFEARRAY **pparrAttributes,
    BOOL bPosFirst
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    HRESULT hr = S_OK;

    try
    {
        CString strResourceFile(sResourceName);

        int i = strResourceFile.ReverseFind(_T('.'));
        if (i<0)
        {
            strResourceFile += _T(".res");
        }

        CComBSTR sRes(strResourceFile);
        CComBSTR sAdd(_T("SILENT"));

        CInsertTask IFB(NULL,
                        sRes,
                        NULL,
                        pProjInfo,
                        sAdd);
        IFB.SetAutomationMode(TRUE);
        IFB.SetTaskName(sTaskName);

        IFB.SetUseAttributes();

        // set Attributes
        BSTR HUGEP *pbstr;
        BSTR bstr;
        HRESULT hr;
        unsigned long iSA;

        // Get a pointer to the elements of the array.
        hr = ::SafeArrayAccessData(*pparrAttributes, (void HUGEP**)&pbstr);
        if (FAILED(hr))
        {
            return hr;
        }
        for (iSA = 0; iSA < ((*pparrAttributes)->rgsabound[0]).cElements; iSA++)
        {
            CString str;
            bstr = pbstr[iSA];
            str = bstr;
            IFB.SetAttribute(str);
        }

        ::SafeArrayUnaccessData(*pparrAttributes);

        IFB.SetGenPos(bPosFirst);
        IFB.Execute();
    }
    catch(HRESULT hrRes) 
    {
        hr = hrRes;
    }
    catch(CMemoryException* e) 
    {
        hr = E_OUTOFMEMORY;
        e->Delete();
    }
    catch(CFileException* e) 
    {
        hr = E_FAIL;
        e->Delete();
    }
    catch(...)
    {
        hr = E_FAIL;
    }


    return hr;
}




STDMETHODIMP CCEAddonActionComObj::RemoveTask
(
    ICEProjInfo* pProjInfo,
    BSTR sResourceName,
    BSTR sTaskName
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    HRESULT hr = S_OK;


    try
    {
        CString strResourceFile(sResourceName);

        int i = strResourceFile.ReverseFind(_T('.'));
        if (i<0)
        {
            strResourceFile += _T(".res");
        }

        CComBSTR sRes(strResourceFile);
        CComBSTR sAdd(_T("SILENT"));

        CDeleteTask IFB(NULL,
                        sRes,
                        sTaskName,
                        pProjInfo,
                        sAdd);
        IFB.SetAutomationMode(TRUE);
        IFB.Execute();
    }
    catch(HRESULT hrRes) 
    {
        hr = hrRes;
    }
    catch(CMemoryException* e) 
    {
        hr = E_OUTOFMEMORY;
        e->Delete();
    }
    catch(CFileException* e) 
    {
        hr = E_FAIL;
        e->Delete();
    }
    catch(...)
    {
        hr = E_FAIL;
    }


    return hr;
}


STDMETHODIMP CCEAddonActionComObj::ModifyTask
(
    ICEProjInfo* pProjInfo,
    BSTR sResourceName,
    BSTR sOldTaskName,
    BSTR sTaskName,
    SAFEARRAY **pparrAttributes,
    BSTR sAlias,
    BSTR sUserText1,
    BSTR sUserText2,
    BSTR sUserText3,
    BSTR sUserUrl,
    BSTR sHelpUrl
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    HRESULT hr = S_OK;

    try
    {
        CString strResourceFile(sResourceName);

        int i = strResourceFile.ReverseFind(_T('.'));
        if (i<0)
        {
            strResourceFile += _T(".res");
        }

        CComBSTR sRes(strResourceFile);
        CComBSTR sAdd(_T("SILENT"));
        CComBSTR sIdPath;

        hr = pProjInfo->getIdPathFromFileName(sRes, &sIdPath);
        sIdPath += _T("/");
        sIdPath += sOldTaskName;

        CPropTask IFB(NULL,
                        sRes,
                        sIdPath,
                        pProjInfo,
                        sAdd);
        IFB.SetAutomationMode(TRUE);

        IFB.FillDialog();

        IFB.SetTaskName(sTaskName);

        IFB.SetUseAttributes();

        // set Attributes
        BSTR HUGEP *pbstr;
        BSTR bstr;
        HRESULT hr;
        unsigned long iSA;

        // Get a pointer to the elements of the array.
        hr = ::SafeArrayAccessData(*pparrAttributes, (void HUGEP**)&pbstr);
        if (FAILED(hr))
        {
            return hr;
        }
        for (iSA = 0; iSA < ((*pparrAttributes)->rgsabound[0]).cElements; iSA++)
        {
            CString str;
            bstr = pbstr[iSA];
            str = bstr;
            IFB.SetAttribute(str);
        }

        ::SafeArrayUnaccessData(*pparrAttributes);

        IFB.SetAlias((CString)sAlias);
        IFB.SetUsrText(0, (CString)sUserText1);
        IFB.SetUsrText(1, (CString)sUserText2);
        IFB.SetUsrText(2, (CString)sUserText3);
        IFB.SetHelpUrl((CString)sHelpUrl);
        IFB.SetUrl((CString)sUserUrl);

        IFB.Execute();
    }
    catch(HRESULT hrRes) 
    {
        hr = hrRes;
    }
    catch(CMemoryException* e) 
    {
        hr = E_OUTOFMEMORY;
        e->Delete();
    }
    catch(CFileException* e) 
    {
        hr = E_FAIL;
        e->Delete();
    }
    catch(...)
    {
        hr = E_FAIL;
    }


    return hr;    
}



STDMETHODIMP CCEAddonActionComObj::ModifyProgramInstance
(
    ICEProjInfo* pProjInfo,
    BSTR sResourceName,
    BSTR sOldProgInstName,
    BSTR sOldTaskName,
    BSTR sTaskName,
    BSTR sProgClass,
    BSTR sProgInstName
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    HRESULT hr = S_OK;

    try
    {
        CString strResourceFile(sResourceName);

        int i = strResourceFile.ReverseFind(_T('.'));
        if (i<0)
        {
            strResourceFile += _T(".res");
        }

        CComBSTR sRes(strResourceFile);
        CComBSTR sAdd(_T("SILENT"));

        CComBSTR sIdPath;

        hr = pProjInfo->getIdPathFromFileName(sRes, &sIdPath);
        sIdPath += _T("/");
        sIdPath += sOldTaskName;
        sIdPath += _T("/");
        sIdPath += sOldProgInstName;

        CPropProgramInst IFB(NULL,
                             sRes,
                             sIdPath,
                             pProjInfo,
                             sAdd);
        IFB.SetAutomationMode(TRUE);
        IFB.FillDialog();

        IFB.SetProgramClass(sProgClass);
        IFB.SetProgramInstName(sProgInstName);
        IFB.SetTaskName(sTaskName);

        IFB.Execute();
    }
    catch(HRESULT hrRes) 
    {
        hr = hrRes;
    }
    catch(CMemoryException* e) 
    {
        hr = E_OUTOFMEMORY;
        e->Delete();
    }
    catch(CFileException* e) 
    {
        hr = E_FAIL;
        e->Delete();
    }
    catch(...)
    {
        hr = E_FAIL;
    }


    return hr;
}




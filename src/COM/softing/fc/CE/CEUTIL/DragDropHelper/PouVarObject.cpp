


#include "stdafx.h"
#include "DragDropHelper\PouVarObject.h"
#include "cedb.h"


#include "DragDropHelper\PouVarDefs.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




///////////////////////////////////////////////////////////////////////////
// CPouObject
///////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------*
/**
 * destructor
 *
 * @exception       -
 * @see             
*/
CPouObject::~CPouObject()
{
    Clear();
}


//------------------------------------------------------------------*
/**
 * InitFomLinkFile creates a PouObject from a link file 
 * of a library object.
 *
 * @param           pLinkFile: a pointer to a link file
 * @return          TRUE: on success, else FALSE
 * @exception       -
 * @see             
*/
BOOL CPouObject::InitFromLinkFile(CLinkFile* pLinkFile)
{
    Clear();

    CString strMangledName;
    if (!pLinkFile->ExistsKey(PVS_LINK_IECIF))
    {
        Clear();
        return FALSE;
    }

    strMangledName = pLinkFile->GetValue(PVS_LINK_IECIF);
    if (!InitFromMangledName(strMangledName))
    {
        Clear();
        return FALSE;
    }
    
    if (!pLinkFile->ExistsKey(PVS_LINK_POUCLASS))
    {
        Clear();
        return FALSE;
    }
    m_strNativeType = pLinkFile->GetValue(PVS_LINK_POUCLASS);
    
    m_strUserHelp = pLinkFile->GetValueWithAttribute(PVS_LINK_POUATTR, PVS_LINK_USERHELP);
    if(m_strUserHelp.IsEmpty())
    {
        m_strUserHelp = pLinkFile->GetValueWithAttribute(PVS_LINK_POUATTR, PVS_LINK_URL);
    }

    if (pLinkFile->ExistsKey(PVS_LINK_HEADER))
    {
        CString strHeaderWPos;
        strHeaderWPos = pLinkFile->GetValue(PVS_LINK_HEADER);
        int p = strHeaderWPos.Find(_T(';'));
        if (p>0)
        {
            m_strHeaderFile = strHeaderWPos.Left(p);
            CString strPos;
            strPos = strHeaderWPos.Mid(p+1);
            m_iHeaderEndPos = _ttoi(strPos);
        }
        else
        {
            m_strHeaderFile = strHeaderWPos;
            m_iHeaderEndPos = 0;
        }
    }

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * InitFromMangledName creates a PouObject from a mangled name.
 *
 * @param           strMangledName: the mangeld name
 * @return          TRUE: on success, else FALSE
 * @exception			
 * @see			
*/
BOOL CPouObject::InitFromMangledName(const CString& strMangledName)
{
    Clear();

    // Mangled Name:  <Type>|<pouname>(vardecl1, vardecl2, ...) xyz...
    //                vardecl: <varclass>|<datatype>|<varname>
    // function: return value: first var with same name as funciton
   
    // extract pou type  --> m_iecType
    CString strType;
    CString strRest;
    int pos = -1;
    pos = strMangledName.Find(PVS_MN_PART);
    if (pos<0)
    {
        return FALSE;
    }
    strType = strMangledName.Left(pos);
  
    if (strType.Compare(PVS_MN_IEC_TYPE_FB)==0)
    {
        m_iecType = IEC_FB;
        m_strNativeType = PVS_XML_FB;
    }
    else if (strType.Compare(PVS_MN_IEC_TYPE_FUN)==0)
    {
        m_iecType = IEC_FUN;
        m_strNativeType = PVS_XML_FUN;
    }
    else if (strType.Compare(PVS_MN_IEC_TYPE_STRUCT)==0)
    {
        m_iecType = IEC_SDT;
        m_strNativeType = PVS_XML_SDT;
    }
    else if (strType.Compare(PVS_MN_IEC_TYPE_PRG)==0)
    {
        m_iecType = IEC_PRG;
        m_strNativeType = PVS_XML_PRG;
    }
    else 
    {
        Clear();
        return FALSE;
    }

    if (strMangledName.GetLength() > pos+1)
    {
        strRest = strMangledName.Mid(pos+1);
    }
    else
    {
        Clear();
        return FALSE;
    }

    // extract pou name --> m_strName
    pos = strRest.Find(_T('('));
    if (pos<0)
    {
        return FALSE;
    }
    m_strName = strRest.Left(pos);
    if (strRest.GetLength()>pos+1)
    {
        strRest = strRest.Mid(pos+1);
    }
    else
    {
        return FALSE;
    }

    // find and process vars:
    // find first:
    BOOL bFound = FALSE;
    CString strVar;
    pos = strRest.FindOneOf(_T(",)"));
    if (pos>0)
    {
        bFound = TRUE;
        strVar = strRest.Left(pos);
        if (strRest.GetLength()>pos+1)
        {
            strRest = strRest.Mid(pos+1);
        }
        else
        {
            strRest.Empty();
        }
    }
    while (bFound)
    {
        // process found var
        CVarObject* pVarObj = NULL;
        pVarObj = new CVarObject();
        ASSERT(pVarObj);
        if (!pVarObj)
        {
            return FALSE;
        }
        if (!pVarObj->InitFromMangledNamePart(strVar))
        {
            delete pVarObj;
            pVarObj = NULL;
        }
        else
        {
            // funcition: first var name == function name 
            // --> return value ( clear name of var for DaD )
            if ((pVarObj->m_strName.CompareNoCase(m_strName)==0)
                && (m_iecType == IEC_FUN))
            {
                pVarObj->m_strName.Empty();
                // mangled name alway contains return for a function: 
                // if void: data type is VOID
                // do not enter VOID return pin
                if (pVarObj->m_strDataType.CompareNoCase(_T("VOID"))==0)
                {
                    delete pVarObj;
                    pVarObj = NULL;
                }
            }
            if (pVarObj)
            {
                m_listVars.AddTail(pVarObj);
            }
        }
        // find next
        bFound = FALSE;
        pos = strRest.FindOneOf(_T(",)"));
        if (pos>0)
        {
            bFound = TRUE;
            strVar = strRest.Left(pos);
            if (strRest.GetLength()>pos+1)
            {
                strRest = strRest.Mid(pos+1);
            }
            else
            {
                strRest.Empty();
            }
        }
    }

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * InitFromXMLNode creates a PouObject from a xml node of the class tree.
 *
 * @param           pNode: a pointer to the xml node.
 * @return          TRUE: on success, else FALSE
 * @exception       -
 * @see             
*/
BOOL CPouObject::InitFromXMLNode(CXMLNode* pNode)
{
    Clear();

    // get user help
    pNode->GetAttribute(CE_XMLATTR_USER_HELP, m_strUserHelp);
    pNode->GetAttribute(CE_XMLATTR_SOURCE, m_strProjectFile);

    CString strMangledName;

    if (!pNode->GetAttribute(CE_XMLATTR_ID, m_strName)
        ||!pNode->GetAttribute(CE_XMLATTR_TYPE, m_strNativeType))
    {
        Clear();
        return FALSE;
    }

    if (pNode->GetAttribute(CE_XMLATTR_MANGLEDNAME, strMangledName))
    {
        // get IEC type:
        return InitFromMangledName(strMangledName);
    }
    
    // create information for iec objects fun,fb,struct
    // TYPE$Name(parameterlist)
    // parameterlist: I|O|IO$DataType$Name, *
    if (m_strNativeType.CompareNoCase(PVS_XML_FB)==0)
    {
        m_iecType = IEC_FB;
        m_strNativeType = PVS_XML_FB;
    }
    else if (m_strNativeType.CompareNoCase(PVS_XML_FUN)==0)
    {
        m_iecType = IEC_FUN;
        m_strNativeType = PVS_XML_FUN;
    }
    else if (m_strNativeType.CompareNoCase(PVS_XML_SDT)==0)
    {
        m_iecType = IEC_SDT;
        m_strNativeType = PVS_XML_SDT;
    }
    else if (m_strNativeType.CompareNoCase(PVS_XML_PRG)==0)
    {
        m_iecType = IEC_PRG;
        m_strNativeType = PVS_XML_PRG;
    }
    else
    {
        Clear();
        return FALSE;
    }
     
    // return value of function as out parameter
    if (m_iecType == IEC_FUN)
    {
        CString strRetType;
        if (pNode->GetAttribute(CE_XMLATTR_DATATYPE, strRetType))
        {
            if (!strRetType.IsEmpty())
            {
                CVarObject* pVarObj = new CVarObject();
                ASSERT(pVarObj);
                if (!pVarObj)
                {
                    Clear();
                    return FALSE;
                }
                pVarObj->m_varClass = VARCLASS_OUTPUT;
                //pVarObj->m_strName = m_strName; --> empty name for DaD
                // TODO: extract detailed data type information
                pVarObj->m_strDataType = strRetType;
                m_listVars.AddTail(pVarObj);
            }
        }
    }

        // look at all all child nodes -> is it an interface var -> add
    CXMLNodeList childList;
    if (pNode->GetChildNodeList(childList))
    {
        childList.Reset();
        CXMLNode childNode;
        while(childList.Next(childNode))
        {
            CVarObject* pVarObj = new CVarObject();
            ASSERT(pVarObj);
            if (!pVarObj)
            {
                Clear();
                return FALSE;
            }
            if (pVarObj->InitFromXMLNode(&childNode))
            {
                m_listVars.AddTail(pVarObj);
            }
            else
            {
                delete pVarObj;
            }
        }              
    }
   
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * GetDragAndDropString creates a drag and drop string from the pou.
 *
 * @return          The drag&drop string, or an empty string on error.
 * @exception       -
 * @see             
*/
CString CPouObject::GetDragAndDropString()
{
    CString strDaD;
    CString strType;

    strDaD = _T("<FCCB>\n");

    strDaD += _T("<CBHEADER>");
    strDaD += _T("</CBHEADER>");


    strDaD += _T("<BODY>\n");

    strDaD += _T("<");

    switch(m_iecType)
    {
    case IEC_FB:
        strType = PVS_DAD_FB;
        break;
    case IEC_FUN:
        strType = PVS_DAD_FUN;
        break;
    case IEC_SDT:
        strType = PVS_DAD_SDT;
        break;
    default:
        return _T("");
    }

    strDaD += strType;

    strDaD += _T(" ORIGIN=\"0;0\" SIZE=\"0;0\" ");

    strDaD += _T("TYPENAME=\"");
    strDaD += m_strName;
    strDaD += _T("\"");
    strDaD += _T(">\n");

    strDaD += _T("<PINLIST>\n");

    // function return value:

    POSITION pos = m_listVars.GetHeadPosition();
    while(pos)
    {
        CVarObject* pVarObj = m_listVars.GetNext(pos);
        ASSERT(pVarObj);
        if (!pVarObj)
        {
            continue;
        }
        strDaD += pVarObj->GetDragAndDropPart();
    }

    strDaD += _T("</PINLIST>\n");

    strDaD += _T("</");
    strDaD += strType;
    strDaD += _T(">\n");
    strDaD += _T("</BODY>");
    strDaD += _T("</FCCB>\n");

    return strDaD;
}


//------------------------------------------------------------------*
/**
 * GetHeader returns the header text of the pou if available..
 *
 * @return          The header text, or an empty string if the header
 *                  is not avaliable.
 * @exception       -
 * @see             
*/
CString CPouObject::GetHeader()
{
    return _T("");
}


CString CPouObject::GetDefinition()
{
    CString str;

    if (m_iecType == IEC_FB)
    {
        CString strInput;
        CString strOutput;
        CString strInOut;

        BOOL bFirstIn = TRUE;
        BOOL bFirstInOut = TRUE;
        BOOL bFirstOut = TRUE;

        str = m_strName;
        str += _T("(");

        POSITION pos = m_listVars.GetHeadPosition();
        while(pos)
        {
            CVarObject* pVarObj = m_listVars.GetNext(pos);
            ASSERT(pVarObj);
            if (!pVarObj)
            {
                continue;
            }

            if (pVarObj->m_varClass==VARCLASS_INPUT 
                || pVarObj->m_varClass==VARCLASS_INEXT)
            {
                if (!bFirstIn)
                {
                    strInput += _T(", ");
                }

                strInput += pVarObj->m_strName;
                strInput += _T(" := (*");
                strInput += pVarObj->m_strDataType;
                strInput += _T("*)");

                bFirstIn = FALSE;
            }
            else if (pVarObj->m_varClass==VARCLASS_INOUT)
            {
                if (!bFirstInOut)
                {
                    strInOut += _T(", ");
                }

                strInOut += pVarObj->m_strName;
                strInOut += _T(" := (*");
                strInOut += pVarObj->m_strDataType;
                strInOut += _T("*)");

                bFirstInOut = FALSE;
            }
            else if (pVarObj->m_varClass==VARCLASS_OUTPUT
                     || pVarObj->m_varClass==VARCLASS_OUTEXT)
            {
                if (!bFirstOut)
                {
                    strOutput += _T(", ");
                }

                strOutput += pVarObj->m_strName;
                strOutput += _T(" => ");
                strOutput += pVarObj->m_strDataType;
                
                bFirstOut = FALSE;
            }

        }

        if (!strOutput.IsEmpty())
        {
            strOutput = _T("(* ") + strOutput + _T(" *)");
        }

        if (!strInput.IsEmpty() && !strInOut.IsEmpty())
        {
            strInOut = _T(", ") + strInOut;
        }

        str += strInput + strInOut + strOutput;

        str += _T(")");
    }
    else if (m_iecType == IEC_FUN)
    {
        str = m_strName;
        str += _T("(");

        CString strRet;
        BOOL bFirst = TRUE;

        POSITION pos = m_listVars.GetHeadPosition();
        while(pos)
        {
            CVarObject* pVarObj = m_listVars.GetNext(pos);
            ASSERT(pVarObj);
            if (!pVarObj)
            {
                continue;
            }

            // return value of function?
            if (pVarObj->m_strName.IsEmpty())
            {
                strRet = _T("(* Returns ");
                strRet += pVarObj->m_strDataType;
                strRet += _T(" *)");
            }
            else
            {
                if (!bFirst)
                {
                    str += _T(", ");
                }
                str += pVarObj->m_strName;
                str += _T(" (*");
                str += pVarObj->m_strDataType;
                str += _T("*)");
                bFirst = FALSE;
            }
        }

        str += _T(")");
        str += strRet;
    }
    else
    {
        return _T("");
    }

    return str;
}


BOOL CPouObject::GetMembers(CList<CString, CString&>& listMembers)
{
    POSITION pos = m_listVars.GetHeadPosition();

    if (!pos)
    {
        return FALSE;
    }

    while(pos)
    {
        CVarObject* pVarObj = m_listVars.GetNext(pos);
        ASSERT(pVarObj);
        if (pVarObj)
        {
            CString str = pVarObj->m_strName;
            listMembers.AddTail(str);
        }
    }

    return TRUE;
}




//------------------------------------------------------------------*
/**
 * Clear clears all variables of the pou object
 *
 * @exception       -
 * @see             
*/
void CPouObject::Clear()
{
    POSITION pos = m_listVars.GetHeadPosition();
    while(pos)
    {
        CVarObject* pVarObj = m_listVars.GetNext(pos);
        ASSERT(pVarObj);
        if (pVarObj)
        {
            delete pVarObj;
        }
    }
    m_listVars.RemoveAll();

    m_strName.Empty();
    m_strOrigin.Empty();

    m_iecType = IEC_UNK;
    m_strNativeType.Empty();

    m_strUserHelp.Empty();
    m_strHeaderFile.Empty();
    m_iHeaderEndPos = 0;
}



int CPouObject::GetDBObjectType()
{
    int iType = CEDB_InvalidObjClass;
    
    switch (m_iecType)
    {
    case IEC_FB : 
        iType = CEDB_FBObj;
        break;
    case IEC_FUN:
        iType = CEDB_FncObj;
        break;
    case IEC_SDT:
        iType = CEDB_DUTObj;
        break;
    case IEC_PRG:
        iType = CEDB_ProgramObj;
        break;
    }

    return iType;

}




///////////////////////////////////////////////////////////////////////////
// CVarObject
///////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------*
/**
 * InitFromMangledName creates a PouObject from a mangled name.
 *
 * @param           strMangledName: the mangeld name
 * @return          TRUE: on success, else FALSE
 * @exception			
 * @see			
*/
BOOL CVarObject::InitFromMangledNamePart(const CString& strMangledNamePart)
{
    Clear();

    CString strVarDef = strMangledNamePart;

    // find the three parts of a var definition
    // varclass+modifier $ datatype $ name
    CString strVarDir;
    CString strVarType;
    CString strVarName;

    int pos;
    pos = strVarDef.Find(PVS_MN_PART);
    if (pos>0)
    {
        strVarDir = strVarDef.Left(pos);
        if (strVarDef.GetLength()>pos+1)
        {
            strVarDef = strVarDef.Mid(pos+1);
        }
        else
        {
            Clear();
            return FALSE;
        }
    }
    else
    {
        Clear();
        return FALSE;
    }

    pos = strVarDef.Find(PVS_MN_PART);
    if (pos>0)
    {
        strVarType = strVarDef.Left(pos);
        if (strVarDef.GetLength()>pos+1)
        {
            strVarName = strVarDef.Mid(pos+1);
        }
        else
        {
            Clear();
            return FALSE;
        }
    }
    else
    {
        Clear();
        return FALSE;
    }
   
    // parse first part: VarClass and Modifier
    CString strVarClass;
    BOOL bExtended = FALSE;

    if (strVarDir.Find(PVS_MN_RETAIN_PARAM)>0)
    {
        m_modifier = MOD_RETAIN;
    } 
    else if (strVarDir.Find(PVS_MN_NONRETAIN_PARAM)>0)
    {
        m_modifier = MOD_NON_RETAIN;
    }
    else if (strVarDir.Find(PVS_MN_CONSTANT_PARAM)>0)
    {
        m_modifier = MOD_CONSTANT;
    }

    if (strVarDir.Find(PVS_MN_EXT_PARAM)>0)
    {
        bExtended = TRUE;
    }

    if (strVarDir[0]==PVS_MN_IN_PARAM)
    {
        if (!bExtended)
        {
            m_varClass = VARCLASS_INPUT;
            m_strNativeType = PVS_XML_VAR_INPUT;
        }
        else
        {
            m_varClass = VARCLASS_INEXT;
            m_strNativeType = PVS_XML_VAR_IN_EXT;
        }
    }
    else if (strVarDir[0]==PVS_MN_OUT_PARAM)
    {
        if (!bExtended)
        {
            m_varClass = VARCLASS_OUTPUT;
            m_strNativeType = PVS_XML_VAR_OUTPUT;
        }
        else
        {
            m_varClass = VARCLASS_OUTEXT;
            m_strNativeType = PVS_XML_VAR_OUT_EXT;
        }
    }
    else if (strVarDir[0]==PVS_MN_INOUT_PARAM)
    {
        m_varClass = VARCLASS_INOUT;
        m_strNativeType = PVS_XML_VAR_IN_OUT;
    }
    else if (strVarDir[0]==PVS_MN_VAR_PARAM)
    {
        m_varClass = VARCLASS_VAR;
        m_strNativeType = PVS_XML_VAR;
    }

    m_strName = strVarName;

    // parse data type to get detailed information
    strVarType.TrimLeft();
    strVarType.TrimRight();
    m_strDataType = strVarType;

    strVarType.MakeUpper();
    // edge modifier
    if (strVarType.CompareNoCase(PVS_MN_DT_BOOL_REDGE)==0)
    {
        strVarType = _T("BOOL");
        m_edgeModifier = EDGE_R;
    }
    else if (strVarType.CompareNoCase(PVS_MN_DT_BOOL_FEDGE)==0)
    {
        strVarType = _T("BOOL");
        m_edgeModifier = EDGE_F;
    }

    m_bsetDataType = CVarObject::GetDataTypeCodeFromText(strVarType);

    if (m_bsetDataType==0)
    {
        if (strVarType.Left(5).CompareNoCase(_T("ARRAY"))==0)
        {
            m_bIsFBorStructDataType = FALSE;
            m_bIsArray = TRUE;
        }
        else
        {
            m_bIsArray = FALSE;
            m_bIsFBorStructDataType = TRUE;
        }
    }
    else
    {
        m_bIsFBorStructDataType = FALSE;
        m_bIsArray = FALSE;
    }

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * InitFromXMLNode creates a PouObject from a xml node of the class tree.
 *
 * @param           pNode: a pointer to the xml node.
 * @return          TRUE: on success, else FALSE
 * @exception       -
 * @see             
*/
BOOL CVarObject::InitFromXMLNode(CXMLNode* pNode)
{
    if (!pNode->GetAttribute(CE_XMLATTR_ID, m_strName))
    {
        Clear();
        return FALSE;
    }

    CString strNodeType;
    if (!pNode->GetAttribute(CE_XMLATTR_TYPE, strNodeType))
    {
        Clear();
        return FALSE;
    }

    if (strNodeType.CompareNoCase(PVS_XML_VAR)==0
        ||strNodeType.CompareNoCase(PVS_XML_STRUCT_VAR)==0)
    {
        m_varClass = VARCLASS_VAR;
        m_strNativeType = PVS_XML_VAR;
    }
    else if (strNodeType.CompareNoCase(PVS_XML_VAR_INPUT)==0)
    {
        m_varClass = VARCLASS_INPUT;
        m_strNativeType = PVS_XML_VAR_INPUT;
    }
    else if (strNodeType.CompareNoCase(PVS_XML_VAR_OUTPUT)==0)
    {
        m_varClass = VARCLASS_OUTPUT;
        m_strNativeType = PVS_XML_VAR_OUTPUT;
    }
    else if (strNodeType.CompareNoCase(PVS_XML_VAR_IN_OUT)==0)
    {
        m_varClass = VARCLASS_INOUT;
        m_strNativeType = PVS_XML_VAR_IN_OUT;
    }
    else if (strNodeType.CompareNoCase(PVS_XML_VAR_IN_EXT)==0)
    {
        m_varClass = VARCLASS_INEXT;
        m_strNativeType = PVS_XML_VAR_IN_EXT;
    }
    else if (strNodeType.CompareNoCase(PVS_XML_VAR_OUT_EXT)==0)
    {
        m_varClass = VARCLASS_OUTEXT;
        m_strNativeType = PVS_XML_VAR_OUT_EXT;
    }
    else if (strNodeType.CompareNoCase(PVS_XML_VAR_GLOBAL)==0)
    {
        m_varClass = VARCLASS_GLOBAL;
        m_strNativeType = PVS_XML_VAR_GLOBAL;
    }
    else if (strNodeType.CompareNoCase(PVS_XML_VAR_EXTERNAL)==0)
    {
        m_varClass = VARCLASS_EXTERNAL;
        m_strNativeType = PVS_XML_VAR_EXTERNAL;
    }
    else if (strNodeType.CompareNoCase(PVS_XML_VAR_TEMP)==0)
    {
        m_varClass = VARCLASS_TEMP;
        m_strNativeType = PVS_XML_VAR_TEMP;
    }
    else if (strNodeType.CompareNoCase(PVS_XML_CONST)==0)
    {
        m_varClass = VARCLASS_CONSTANT;
        m_strNativeType = PVS_XML_CONST;
    }
    else
    {
        Clear();
        return FALSE;
    }

    CString strModifier;
    if (pNode->GetAttribute(CE_XMLATTR_MODIFIER, strModifier))
    {
        if (strModifier.CompareNoCase(PVS_XML_RETAIN)==0)
        {
            m_modifier = MOD_RETAIN;
        }
        else if (strModifier.CompareNoCase(PVS_XML_NON_RETAIN)==0)
        {
            m_modifier = MOD_NON_RETAIN;
        }
        else if (strModifier.CompareNoCase(PVS_XML_CONSTANT)==0)
        {
            m_modifier = MOD_CONSTANT;
        }
    }

    m_edgeModifier = EDGE_NO;

    CString strEdge;
    if (pNode->GetAttribute(CE_XMLATTR_EDGE, strEdge))
    {
        if (strEdge.CompareNoCase(PVS_XML_R_EDGE)==0)
        {
            m_edgeModifier = EDGE_R;
        }
        else if (strEdge.CompareNoCase(PVS_XML_F_EDGE)==0)
        {
            m_edgeModifier = EDGE_F;
        }
    }

    pNode->GetAttribute(CE_XMLATTR_ADDRESS, m_strAddress);
    pNode->GetAttribute(CE_XMLATTR_INITVAL, m_strInitValue);
    pNode->GetAttribute(CE_XMLATTR_ALIAS, m_strAlias);
    pNode->GetAttribute(CE_XMLATTR_COMMENT, m_strComment);

    // extract detailed information from data type
    CString strVarType;
    pNode->GetAttribute(CE_XMLATTR_DATATYPE, strVarType);
    
    strVarType.TrimLeft();
    strVarType.TrimRight();
    m_strDataType = strVarType;

    strVarType.MakeUpper();
    // edge modifier
    if (strVarType.CompareNoCase(PVS_MN_DT_BOOL_REDGE)==0)
    {
        strVarType = _T("BOOL");
        m_edgeModifier = EDGE_R;
    }
    else if (strVarType.CompareNoCase(PVS_MN_DT_BOOL_FEDGE)==0)
    {
        strVarType = _T("BOOL");
        m_edgeModifier = EDGE_F;
    }

    m_bsetDataType = CVarObject::GetDataTypeCodeFromText(strVarType);

    if (m_bsetDataType==0)
    {
        if (strVarType.Left(5).CompareNoCase(_T("ARRAY"))==0)
        {
            m_bIsFBorStructDataType = FALSE;
            m_bIsArray = TRUE;
        }
        else
        {
            m_bIsArray = FALSE;
            m_bIsFBorStructDataType = TRUE;
        }
    }
    else
    {
        m_bIsFBorStructDataType = FALSE;
        m_bIsArray = FALSE;
    }

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * GetDragAndDropString creates a drag and drop string from the var object.
 *
 * @return          The drag&drop string, or an empty string on error.
 * @exception       -
 * @see             
*/
CString CVarObject::GetDragAndDropString()
{
    // only for constants:
    //if (m_varClass != VARCLASS_CONSTANT)
    //{
    //    return _T("");
    //}

    CString strDaD;

    strDaD = _T("<FCCB>\n");

    strDaD += _T("<CBHEADER>");
    strDaD += _T("</CBHEADER>");

    strDaD += _T("<BODY>\n");
    
    strDaD += _T("<");
    strDaD += _T("CONST");
    strDaD += _T(" ORIGIN=\"0;0\" SIZE=\"0;0\" OUTORIGIN=\"0;0\" ");

    strDaD += _T(" NAME=\"");
    strDaD += m_strName;
    strDaD += _T("\"");

    if (m_edgeModifier == EDGE_R)
    {
        strDaD += _T(" TYPE=\"BOOL\" R_EDGE=\"1\"");
    }
    else if (m_edgeModifier == EDGE_F)
    {
        strDaD += _T(" TYPE=\"BOOL\" F_EDGE=\"1\"");
    }
    else
    {
        strDaD += _T(" TYPE=\"");
        strDaD += m_strDataType;
        strDaD += _T("\"");
    }

    strDaD += _T(" INITVAL=\"");
    strDaD += m_strInitValue;
    strDaD += _T("\"");
    strDaD += _T("/>\n");
    strDaD += _T("</BODY>");
    strDaD += _T("</FCCB>\n");

    return strDaD;
}


//------------------------------------------------------------------*
/**
 * GetDragAndDropPart create a pin for variable.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
CString CVarObject::GetDragAndDropPart()
{
    CString strDaD;

    strDaD += _T("<PIN ");
    strDaD += _T("CLASS=\"");

    if (m_varClass == VARCLASS_INPUT)
    {
        strDaD += PVS_DAD_INPUT;
    }
    else if (m_varClass == VARCLASS_OUTPUT)
    {
        strDaD += PVS_DAD_OUTPUT;
    }
    else if (m_varClass == VARCLASS_INOUT)
    {
        strDaD += PVS_DAD_INOUT;
    }
    else if (m_varClass == VARCLASS_INEXT)
    {
        strDaD += PVS_DAD_INEXT;
    }
    else if (m_varClass == VARCLASS_OUTEXT)
    {
        strDaD += PVS_DAD_OUTEXT;
    }
    else
    {
        return _T("");
    }

    strDaD += _T("\" ");

    strDaD += _T("NAME=\"");
    strDaD += m_strName;
    strDaD += _T("\" ");
    
    if (m_edgeModifier == EDGE_R)
    {
        strDaD += _T(" TYPE=\"BOOL\" R_EDGE=\"1\"");
    }
    else if (m_edgeModifier == EDGE_F)
    {
        strDaD += _T(" TYPE=\"BOOL\" F_EDGE=\"1\"");
    }
    else
    {
        strDaD += _T(" TYPE=\"");
        strDaD += m_strDataType;
        strDaD += _T("\"");
    }

    strDaD += _T(" ORIGIN=\"0;0\" />");

    return strDaD;
}


//------------------------------------------------------------------*
/**
 * GetHeader returns the header text of the pou if available..
 *
 * @return          The header text, or an empty string if the header
 *                  is not avaliable.
 * @exception       -
 * @see             
*/
CString CVarObject::GetHeader()
{
    return _T("");
}


//------------------------------------------------------------------*
/**
 * Clear clears all variables of the pou object
 *
 * @exception       -
 * @see             
*/
void CVarObject::Clear()
{
    m_strName.Empty();
    m_strOrigin.Empty();

    m_strDataType.Empty();
    m_varClass = VARCLASS_UNKNOWN;
    m_modifier = MOD_NONE;
    m_edgeModifier = EDGE_NO;
    m_strAddress.Empty();
    m_strInitValue.Empty();
    m_strAlias.Empty();
    m_strComment.Empty();
    m_bsetDataType = 0;
    m_bIsArray = FALSE;
    m_bIsFBorStructDataType = FALSE;
}


CString CVarObject::GetVarClassText()
{
    if (m_varClass==VARCLASS_UNKNOWN)
    {
        return _T("");
    }
    else if (m_varClass==VARCLASS_VAR)
    {
        return PVS_XML_VAR;
    }
    else if (m_varClass==VARCLASS_INPUT)
    {
        return PVS_XML_VAR_INPUT;
    }
    else if (m_varClass==VARCLASS_OUTPUT)
    {
        return PVS_XML_VAR_OUTPUT;
    }
    else if (m_varClass==VARCLASS_INOUT)
    {
        return PVS_XML_VAR_IN_OUT;
    }
    else if (m_varClass==VARCLASS_INEXT)
    {
        return PVS_XML_VAR_IN_EXT;
    }
    else if (m_varClass==VARCLASS_OUTEXT)
    {
        return PVS_XML_VAR_OUT_EXT;
    }
    else if (m_varClass==VARCLASS_GLOBAL)
    {
        return PVS_XML_VAR_GLOBAL;
    }
    else if (m_varClass==VARCLASS_EXTERNAL)
    {
        return PVS_XML_VAR_EXTERNAL;
    }
    else if (m_varClass==VARCLASS_TEMP)
    {
        return PVS_XML_VAR_TEMP;
    }
    else if (m_varClass==VARCLASS_CONSTANT)
    {
        return PVS_XML_CONST;
    }
    else
    {
        return _T("");
    }
}

CString CVarObject::GetModifierText()
{
    if (m_modifier==MOD_NONE)
    {
        return _T("");
    }
    else if (m_modifier==MOD_RETAIN)
    {
        return PVS_XML_RETAIN;
    }
    else if (m_modifier==MOD_NON_RETAIN)
    {
        return PVS_XML_NON_RETAIN;
    }
    else if (m_modifier==MOD_CONSTANT)
    {
        return PVS_XML_CONSTANT;
    }
    else 
    {
        return _T("");
    }
}

CString CVarObject::GetVisModifierText()
{
    if (m_modifier==MOD_NONE)
    {
        return _T("");
    }
    else if (m_modifier==MOD_RETAIN)
    {
        return PVS_XML_RETAIN;
    }
    else if (m_modifier==MOD_NON_RETAIN)
    {
        return PVS_XML_NONRETAIN;
    }
    else if (m_modifier==MOD_CONSTANT)
    {
        return PVS_XML_CONST;
    }
    else 
    {
        return _T("");
    }
}

CString CVarObject::GetVarClassWithModifierText()
{
    return (GetVarClassText() + (GetVisModifierText().IsEmpty() ? _T("") : _T(" ")) + GetVisModifierText());
}


int CVarObject::GetDBObjectType()
{
    return CEDB_ConstObj;
}





BOOL CVarObject::m_gbDataTypeMapInit = FALSE;
CMap<CString, LPCTSTR, PVS_BITSET, PVS_BITSET> CVarObject::m_gmapDataTypes;



PVS_BITSET CVarObject::GetDataTypeCodeFromText(const CString& strDataType)
{
    PVS_BITSET bsetDataType = 0;

    if (!m_gbDataTypeMapInit)
    {
        InitDataTypeMap();
        m_gbDataTypeMapInit = TRUE;
    }

    CString strUpperDataType = strDataType;
    strUpperDataType.MakeUpper();
    if (!m_gmapDataTypes.Lookup(strUpperDataType, bsetDataType))
    {   
        bsetDataType = 0;
    }

    return bsetDataType;
}

BOOL CVarObject::GetDataTypeTextFromCode(const PVS_BITSET bsetCode, 
                                                CList<CString, CString&>& listDataTypes)
{
    listDataTypes.RemoveAll();

    if ((bsetCode & PVS_TBS_VOID)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_VOID);
    }
    if ((bsetCode & PVS_TBS_BOOL_R_EDGE)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_BOOL_R_EDGE);
    }
    if ((bsetCode & PVS_TBS_BOOL_F_EDGE)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_BOOL_F_EDGE);
    }
    if ((bsetCode & PVS_TBS_BOOL)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_BOOL);
    }
    if ((bsetCode & PVS_TBS_BYTE)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_BYTE);
    }
    if ((bsetCode & PVS_TBS_WORD)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_WORD);
    }
    if ((bsetCode & PVS_TBS_DWORD)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_DWORD);
    }
    if ((bsetCode & PVS_TBS_LWORD)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_LWORD);
    }
    if ((bsetCode & PVS_TBS_REAL)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_REAL);
    }
    if ((bsetCode & PVS_TBS_LREAL)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_LREAL);
    }
    if ((bsetCode & PVS_TBS_SINT)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_SINT);
    }
    if ((bsetCode & PVS_TBS_INT)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_INT);
    }
    if ((bsetCode & PVS_TBS_DINT)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_DINT);
    }
    if ((bsetCode & PVS_TBS_LINT)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_LINT);
    }
    if ((bsetCode & PVS_TBS_USINT)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_USINT);
    }
    if ((bsetCode & PVS_TBS_UINT)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_UINT);
    }
    if ((bsetCode & PVS_TBS_UDINT)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_UDINT);
    }
    if ((bsetCode & PVS_TBS_ULINT)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_ULINT);
    }
    if ((bsetCode & PVS_TBS_DT)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_DT);
    }
    if ((bsetCode & PVS_TBS_TOD)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_TOD);
    }
    if ((bsetCode & PVS_TBS_DATE)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_DATE);
    }
    if ((bsetCode & PVS_TBS_TIME)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_TIME);
    }
    if ((bsetCode & PVS_TBS_STRING)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_STRING);
    }
    if ((bsetCode & PVS_TBS_WSTRING)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_WSTRING);
    }
    if ((bsetCode & PVS_TBS_QVT_BOOL)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_QVT_BOOL);
    }
    if ((bsetCode & PVS_TBS_QVT_BYTE)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_QVT_BYTE);
    }
    if ((bsetCode & PVS_TBS_QVT_WORD)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_QVT_WORD);
    }
    if ((bsetCode & PVS_TBS_QVT_DWORD)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_QVT_DWORD);
    }
    if ((bsetCode & PVS_TBS_QVT_LWORD)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_QVT_LWORD);
    }
    if ((bsetCode & PVS_TBS_QVT_REAL)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_QVT_REAL);
    }
    if ((bsetCode & PVS_TBS_QVT_LREAL)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_QVT_LREAL);
    }
    if ((bsetCode & PVS_TBS_QVT_SINT)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_QVT_SINT);
    }
    if ((bsetCode & PVS_TBS_QVT_INT)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_QVT_INT);
    }
    if ((bsetCode & PVS_TBS_QVT_DINT)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_QVT_DINT);
    }
    if ((bsetCode & PVS_TBS_QVT_LINT)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_QVT_LINT);
    }
    if ((bsetCode & PVS_TBS_QVT_USINT)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_QVT_USINT);
    }
    if ((bsetCode & PVS_TBS_QVT_UINT)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_QVT_UINT);
    }
    if ((bsetCode & PVS_TBS_QVT_UDINT)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_QVT_UDINT);
    }
    if ((bsetCode & PVS_TBS_QVT_ULINT)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_QVT_ULINT);
    }
    if ((bsetCode & PVS_TBS_QVT_DT)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_QVT_DT);
    }
    if ((bsetCode & PVS_TBS_QVT_TOD)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_QVT_TOD);
    }
    if ((bsetCode & PVS_TBS_QVT_DATE)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_QVT_DATE);
    }
    if ((bsetCode & PVS_TBS_QVT_TIME)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_QVT_TIME);
    }
    if ((bsetCode & PVS_TBS_QVT_STRING)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_QVT_STRING);
    }
    if ((bsetCode & PVS_TBS_QVT_WSTRING)>0)
    {
        listDataTypes.AddTail((CString)PVS_TEXT_QVT_WSTRING);
    }
        
    return FALSE;
}

void CVarObject::InitDataTypeMap()
{
    //m_gmapDataTypes.SetAt(PVS_TEXT_UNDEF,     PVS_TBS_UNDEF);           
    m_gmapDataTypes.SetAt(PVS_TEXT_VOID,        PVS_TBS_VOID);            
    m_gmapDataTypes.SetAt(PVS_TEXT_BOOL_R_EDGE, PVS_TBS_BOOL_R_EDGE);      
    m_gmapDataTypes.SetAt(PVS_TEXT_BOOL_F_EDGE, PVS_TBS_BOOL_F_EDGE);    
    m_gmapDataTypes.SetAt(PVS_TEXT_BOOL,        PVS_TBS_BOOL);
    m_gmapDataTypes.SetAt(PVS_TEXT_BYTE,        PVS_TBS_BYTE);          
    m_gmapDataTypes.SetAt(PVS_TEXT_WORD,        PVS_TBS_WORD);         
    m_gmapDataTypes.SetAt(PVS_TEXT_DWORD,       PVS_TBS_DWORD);        
    m_gmapDataTypes.SetAt(PVS_TEXT_LWORD,       PVS_TBS_LWORD);      
    m_gmapDataTypes.SetAt(PVS_TEXT_REAL,        PVS_TBS_REAL);    
    m_gmapDataTypes.SetAt(PVS_TEXT_LREAL,       PVS_TBS_LREAL);     
    m_gmapDataTypes.SetAt(PVS_TEXT_SINT,        PVS_TBS_SINT);      
    m_gmapDataTypes.SetAt(PVS_TEXT_INT,         PVS_TBS_INT);    
    m_gmapDataTypes.SetAt(PVS_TEXT_DINT,        PVS_TBS_DINT);         
    m_gmapDataTypes.SetAt(PVS_TEXT_LINT,        PVS_TBS_LINT);           
    m_gmapDataTypes.SetAt(PVS_TEXT_USINT,       PVS_TBS_USINT);
    m_gmapDataTypes.SetAt(PVS_TEXT_UINT,        PVS_TBS_UINT);         
    m_gmapDataTypes.SetAt(PVS_TEXT_UDINT,       PVS_TBS_UDINT);         
    m_gmapDataTypes.SetAt(PVS_TEXT_ULINT,       PVS_TBS_ULINT);      
    m_gmapDataTypes.SetAt(PVS_TEXT_DT,          PVS_TBS_DT);         
    m_gmapDataTypes.SetAt(PVS_TEXT_TOD,         PVS_TBS_TOD);         
    m_gmapDataTypes.SetAt(PVS_TEXT_DATE,        PVS_TBS_DATE);         
    m_gmapDataTypes.SetAt(PVS_TEXT_TIME,        PVS_TBS_TIME);        
    m_gmapDataTypes.SetAt(PVS_TEXT_STRING,      PVS_TBS_STRING);
    m_gmapDataTypes.SetAt(PVS_TEXT_WSTRING,     PVS_TBS_WSTRING);      
    // m_gmapDataTypes.SetAt(PVS_TEXT_INSTANCE, PVS_TBS_INSTANCE);        
    // m_gmapDataTypes.SetAt(PVS_TEXT_STRUCT,   PVS_TBS_STRUCT);    
    // m_gmapDataTypes.SetAt(PVS_TEXT_PROGRAM,  PVS_TBS_PROGRAM);     
    // m_gmapDataTypes.SetAt(PVS_TEXT_ARRAY,    PVS_TBS_ARRAY);   
    m_gmapDataTypes.SetAt(PVS_TEXT_QVT_BOOL,    PVS_TBS_QVT_BOOL);
    m_gmapDataTypes.SetAt(PVS_TEXT_QVT_BYTE,    PVS_TBS_QVT_BYTE);      
    m_gmapDataTypes.SetAt(PVS_TEXT_QVT_WORD,    PVS_TBS_QVT_WORD);     
    m_gmapDataTypes.SetAt(PVS_TEXT_QVT_DWORD,   PVS_TBS_QVT_DWORD);      
    m_gmapDataTypes.SetAt(PVS_TEXT_QVT_LWORD,   PVS_TBS_QVT_LWORD);   
    m_gmapDataTypes.SetAt(PVS_TEXT_QVT_REAL,    PVS_TBS_QVT_REAL);  
    m_gmapDataTypes.SetAt(PVS_TEXT_QVT_LREAL,   PVS_TBS_QVT_LREAL);   
    m_gmapDataTypes.SetAt(PVS_TEXT_QVT_SINT,    PVS_TBS_QVT_SINT);   
    m_gmapDataTypes.SetAt(PVS_TEXT_QVT_INT,     PVS_TBS_QVT_INT);     
    m_gmapDataTypes.SetAt(PVS_TEXT_QVT_DINT,    PVS_TBS_QVT_DINT);       
    m_gmapDataTypes.SetAt(PVS_TEXT_QVT_LINT,    PVS_TBS_QVT_LINT);       
    m_gmapDataTypes.SetAt(PVS_TEXT_QVT_USINT,   PVS_TBS_QVT_USINT);    
    m_gmapDataTypes.SetAt(PVS_TEXT_QVT_UINT,    PVS_TBS_QVT_UINT);      
    m_gmapDataTypes.SetAt(PVS_TEXT_QVT_UDINT,   PVS_TBS_QVT_UDINT);   
    m_gmapDataTypes.SetAt(PVS_TEXT_QVT_ULINT,   PVS_TBS_QVT_ULINT);  
    m_gmapDataTypes.SetAt(PVS_TEXT_QVT_DT,      PVS_TBS_QVT_DT);     
    m_gmapDataTypes.SetAt(PVS_TEXT_QVT_TOD,     PVS_TBS_QVT_TOD);       
    m_gmapDataTypes.SetAt(PVS_TEXT_QVT_DATE,    PVS_TBS_QVT_DATE);        
    m_gmapDataTypes.SetAt(PVS_TEXT_QVT_TIME,    PVS_TBS_QVT_TIME);      
    m_gmapDataTypes.SetAt(PVS_TEXT_QVT_STRING,  PVS_TBS_QVT_STRING);      
    m_gmapDataTypes.SetAt(PVS_TEXT_QVT_WSTRING, PVS_TBS_QVT_WSTRING);  
    
    m_gmapDataTypes.SetAt(PVS_TEXT_ANY,         PVS_TBS_ANY);  
    m_gmapDataTypes.SetAt(PVS_TEXT_ANY_REAL,    PVS_TBS_ANY_REAL);
    m_gmapDataTypes.SetAt(PVS_TEXT_ANY_SIGN_INT,PVS_TBS_ANY_SIGN_INT);
    m_gmapDataTypes.SetAt(PVS_TEXT_ANY_USIGN_INT,PVS_TBS_ANY_USIGN_INT);   
    m_gmapDataTypes.SetAt(PVS_TEXT_ANY_INT,     PVS_TBS_ANY_INT);
    m_gmapDataTypes.SetAt(PVS_TEXT_ANY_NUM,     PVS_TBS_ANY_NUM);     
    m_gmapDataTypes.SetAt(PVS_TEXT_ANY_BIT,     PVS_TBS_ANY_BIT);      
    m_gmapDataTypes.SetAt(PVS_TEXT_ANY_DATE,    PVS_TBS_ANY_DATE);       
    m_gmapDataTypes.SetAt(PVS_TEXT_ANY_SIGNED,  PVS_TBS_ANY_SIGNED);       
    m_gmapDataTypes.SetAt(PVS_TEXT_ANY_STRING,  PVS_TBS_ANY_STRING);   
    m_gmapDataTypes.SetAt(PVS_TEXT_ANY_ELEMENTARY,PVS_TBS_ANY_ELEMENTARY);
    m_gmapDataTypes.SetAt(PVS_TEXT_ANY_DERIVED, PVS_TBS_ANY_DERIVED);
    m_gmapDataTypes.SetAt(PVS_TEXT_ANY_MAGNITUDE,PVS_TBS_ANY_MAGNITUDE);
    m_gmapDataTypes.SetAt(PVS_TEXT_ANY_QVT,     PVS_TBS_ANY_QVT);
    m_gmapDataTypes.SetAt(PVS_TEXT_ANY_SIMPLE,  PVS_TBS_ANY_SIMPLE);
}


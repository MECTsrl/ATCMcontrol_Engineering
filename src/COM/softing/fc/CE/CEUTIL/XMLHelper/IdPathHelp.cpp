
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "StdAfx.h"
#include "IdPathHelp.h"


//----  Static Initializations:   ----------------------------------*

//------------------------------------------------------------------*
/**
 * strip id from id path - separator is '/'
 *
 * @param           - CString& cs_path - id path
 *                     this will be shortened by id
 * @param           - CString& cs_id - stripped id
 * @return          - TRUE if successful else FALSE
 * @see             
*/
BOOL StripIdFromIdPath(CString& cs_path, CString& cs_id, BOOL bFromBack /*=FALSE*/ )
{
    int         i_found;

    // if path is empty -> nothing can be stripped
    if(cs_path.IsEmpty())
    {
        return(FALSE);
    }

    if(bFromBack)
    {
        i_found = cs_path.ReverseFind(CE_XML_IDPATH_SEP_CHAR);
        if(i_found == -1)
        {
            cs_id = cs_path;
            cs_path.Empty();
        }
        else
        {
            cs_id = cs_path.Mid(i_found + 1);
            cs_path = cs_path.Left(i_found);
        }
    }
    else
    {
        i_found = cs_path.Find(CE_XML_IDPATH_SEP_CHAR, 0);

        if(i_found == -1)
        {
            cs_id = cs_path;
            cs_path.Empty();
        }
        else
        {
            cs_id = cs_path.Left(i_found);
            cs_path = cs_path.Mid(i_found + 1);
        }
    }
    return(TRUE);
}


//------------------------------------------------------------------*
/**
 * GetParentIdPath extracts the id path of the parent node,
 * it removes the last ection of the path
 *
 * @param           strIdPath: the id path
 * @param           strParentIdPath: the id path of the parent node
 * @return          
 * @exception       -
 * @see             
*/
BOOL GetParentIdPath(const CString& strIdPath, CString& strParentIdPath)
{
    int iFound;
    int iLen = strIdPath.GetLength();
    iFound = strIdPath.ReverseFind(CE_XML_IDPATH_SEP_CHAR);
    if (iFound<=0)
    {
        strParentIdPath.Empty();
        return FALSE;
    }

    strParentIdPath = strIdPath.Left(iFound);

    return TRUE;
}


BOOL IsPrefixOfIdPath(const CString& strPrefix, const CString& strIdPath)
{
    if (strIdPath.GetLength()<strPrefix.GetLength())
    {
        return FALSE;
    }

    CString strIdStart;
    strIdStart = strIdPath.Left(strPrefix.GetLength());
    if (strIdStart.Compare(strPrefix)==0)
    {
        return TRUE;
    }

    return FALSE;
}



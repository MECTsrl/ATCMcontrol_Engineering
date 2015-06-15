/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/Dragitem.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: Dragitem.h $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/Dragitem.h $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                4CE
 *
 * =COMPONENT           CEMAIN
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *           $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *== dragitem.h Header file of the drag item objects. The objects will be trans-
 * ferred during a drag and drop operation. It uses a CArchive to read/write the
 * data into the object. The data will be dragged from a source and dropped into
 * a target. The source will be identified by a COleDataSource object. This object
 * will receive an object of class CDragItem and "send" this object to clipboard.
 * This operation is typically initiated by a OnBeginDrag handler. The target will
 * be identified by a COleDropTarget object. The container of COleDropTarget (or a
 * derived class) has to implement the functions OnDragOver and OnDrop (a virtual
 * override of COleDropTarget::OnDragOver/OnDrop or CView::OnDragOver/OnDrop
 * see sample below.
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  31.01.01  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/
 
#ifndef __DRAGITEM_H_
#define __DRAGITEM_H_
 
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
 
 
#include <afxole.h>
#include <afxadv.h>
#include <assert.h>


 
#define CB_FORMAT_4CLIB _T("CELIB_CLIP_FORMAT")
#define CB_FORMAT_4CLIB_VERSION (1)

#define CB_FORMAT_4CXML _T("CB_FORMAT_4CXML")
#define CB_FORMAT_4CXML_VERSION (1)

#define CB_FORMAT_4CWATCHEXPR _T("CB_FORMAT_4CWATCHEXPR")
#define CB_FORMAT_4CWATCHEXPR_VERSION (1)

#define CB_FORMAT_4CXMLNODE _T("CB_FORMAT_4CXMLNODE")
#define CB_FORMAT_4CXMLNODE_VERSION (1)

//TODO obsolete define - delete
#define CELIB_CLIP_FORMAT CB_FORMAT_4CLIB

/////////////////////////////////////////////////////////////////////////////
// CDragItem command target
// ATTENTION: Do not forget to call IMPLEMENT_SERIAL in exactly one module
// of your application

class CDragItem : public CObject
{
    DECLARE_SERIAL(CDragItem)

    CDragItem() // protected constructor used by dynamic creation
    	: m_strObjName(),m_strKeyword(),m_astrFilePath()
    {
        m_lObjClass = 0L;
    }
    
    CDragItem(LONG lObjClass,LPCTSTR pszObjName,LPCTSTR pszKeyword=NULL)
        : m_lObjClass(lObjClass), m_strObjName(pszObjName) ,
	    m_astrFilePath(), m_strKeyword()
    {
	    if(pszKeyword)
		    m_strKeyword = pszKeyword;
    }
    
    virtual ~CDragItem(void)
    {
        m_astrFilePath.RemoveAll();
    }
// Attributes
public:
    LONG	m_lObjClass;	// object class id -> see cedb.h
	CString m_strObjName;	// object name
	CString m_strKeyword;	// key word string according to m_lObjClass...

// array of files identified by absolute file path
	CStringArray m_astrFilePath;

// Operations
public:
	BOOL	AppendFile(LPCTSTR pszFilePath)
    {
	    assert(pszFilePath);

	    try{
		    m_astrFilePath.Add(pszFilePath);
	    }
	    catch(CMemoryException* e){
		    e->Delete();
		    return FALSE;
	    }
	    return TRUE;
    }

    virtual void Serialize(CArchive& ar)
    {
        int iSize;
	    CObject::Serialize(ar);

        if(ar.IsStoring()) { //write to archive
            ar << m_lObjClass;
		    ar << m_strObjName;
		    ar << m_strKeyword;
		    iSize = m_astrFilePath.GetSize();
		    ar << iSize;
		    for(int i = 0; i < iSize; i++) {
			    ar << m_astrFilePath[i];
		    }

        } else { //read from archive
		    CString strFile;

            ar >> m_lObjClass;
		    ar >> m_strObjName;
		    ar >> m_strKeyword;
		    ar >> iSize;
		    for(int i = 0; i < iSize; i++) {
			    ar >> strFile;
			    if(!AppendFile(strFile)) //out of memory ?
				    break;
		    }
        }
        return;
    }

// Overrides

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////
// CXMLDragItem command target
// ATTENTION: Do not forget to call IMPLEMENT_SERIAL in exactly one module
// of your application

class CXMLDragItem : public CObject
{
    DECLARE_SERIAL(CXMLDragItem)

    CXMLDragItem() // protected constructor used by dynamic creation
    : m_strSourceFile() , m_strSource()
    {
        m_lObjClass = 0L;
    }

    CXMLDragItem(LPCTSTR pszSourceFile,LONG lObjClass,BSTR sSource)
    : m_strSourceFile(pszSourceFile), m_strSource()
    //Exceptions: CMemoryException
    {
        m_lObjClass = lObjClass;
        if(sSource) {
            m_strSource = sSource;
        }
    }

    virtual ~CXMLDragItem(void)
    {}
// Attributes
public:
    LONG	m_lObjClass;        // object class id -> see cedb.h
    CString m_strSourceFile;    // source file
    CString m_strSource;        // source stream

// Operations
public:
    virtual void Serialize(CArchive& ar)
    {
	    CObject::Serialize(ar);

        if(ar.IsStoring()) { //write to archive
            ar << m_lObjClass;
		    ar << m_strSourceFile;
		    ar << m_strSource;

        } 
        else { //read from archive
            ar >> m_lObjClass;
		    ar >> m_strSourceFile;
		    ar >> m_strSource;
        }
    }
// Overrides

// Implementation
protected:
};


/////////////////////////////////////////////////////////////////////////////
// CWatchExpressionDragItem command target
// ATTENTION: Do not forget to call IMPLEMENT_SERIAL in exactly one module
// of your application

class CWatchExpressionDragItem : public CObject
{
    DECLARE_SERIAL (CWatchExpressionDragItem)

public:
    CWatchExpressionDragItem () { }

    CWatchExpressionDragItem (const CString &strExpression, const CString &strScope)
    : m_strExpression (strExpression),
      m_strScope (strScope)
    {
    }

    CString GetExpression () { return (m_strExpression); }
    CString GetScope () { return (m_strScope); }

    virtual void Serialize (CArchive &ar)
    {
	    CObject::Serialize (ar);

        if (ar.IsStoring ())
        {
            ar << m_strExpression;
		    ar << m_strScope;
        } 
        else
        {
		    ar >> m_strExpression;
		    ar >> m_strScope;
        };
    }

protected:
    CString m_strExpression;
    CString m_strScope;
};


class CXMLNodeDragItem : public CObject
{
    DECLARE_SERIAL (CXMLNodeDragItem)

public:
    CXMLNodeDragItem () { }

    void SetData(   const CString& strText,
                    const CString& strId,
                    const CString& strType,
                    const CString& strSource,
                    const CString& strProjPath,
                    const CString& strXMLString)
    {
        m_strText = strText;
        m_strId = strId;
        m_strType = strType;
        m_strSource = strSource;
        m_strProjPath = strProjPath;
        m_strXMLString = strXMLString;
    }

    void SetCBFormatId(UINT uiCBFormat)
    {
        m_uiCBFormat = uiCBFormat;
    }

    BOOL CopyToClipboard(COleDataSource* pDataSource)
    {
        CFileStatus fStatus;
        BSTR sStream = NULL;

        try
        {
	        CSharedFile	GlobFile;
		    CArchive ar(&GlobFile, CArchive::store);
		    Serialize(ar);
		    ar.Close();
		    pDataSource->CacheGlobalData(m_uiCBFormat, GlobFile.Detach());
	    }
	    catch(CMemoryException *e)
        {
            //out of memory
            //TODO status output
		    e->Delete();
            return FALSE;
	    }
        catch(CFileException* e)
        {
            //file op failed
            //TODO status output
		    e->Delete();
            return FALSE;
        }
        catch(CArchiveException* e)
        {
            //archive op failed
            //TODO status output
		    e->Delete();
            return FALSE;
        }
	    catch(HRESULT)
        {
            return FALSE;
	    }

        ::SysFreeString(sStream);
        return TRUE;
    }


    BOOL GetFromClipboard()
    {
        COleDataObject  tDataObject;
        tDataObject.AttachClipboard();
        return LoadFromClipboard(&tDataObject);
    }

    BOOL LoadFromClipboard(COleDataObject* pDataObject)
    {
        if (pDataObject->IsDataAvailable(m_uiCBFormat))
	    {
			HGLOBAL hGlobal = pDataObject->GetGlobalData(m_uiCBFormat);
			if (hGlobal != NULL)
			{
				CSharedFile file;
				CArchive ar (&file, CArchive::load);
				file.SetHandle (hGlobal, FALSE);
				Serialize (ar);
				ar.Close();
				file.Detach();
				return TRUE;
			}
	    }
	    return FALSE;
    }


    virtual void Serialize (CArchive &ar)
    {
	    CObject::Serialize (ar);

        if (ar.IsStoring ())
        {
            ar << m_strText;
		    ar << m_strId;
		    ar << m_strType;
		    ar << m_strSource;
            ar << m_strProjPath;
		    ar << m_strXMLString;
        } 
        else
        {
            ar >> m_strText;
		    ar >> m_strId;
		    ar >> m_strType;
		    ar >> m_strSource;
            ar >> m_strProjPath;
		    ar >> m_strXMLString;
        };
    }

public:
    CString     m_strText;      // node text
    CString     m_strId;        // id text
    CString     m_strType;      // type id string
    CString     m_strSource;    // source file
    CString     m_strProjPath;  // project path
    CString     m_strXMLString; // XML code for node
    UINT        m_uiCBFormat;   // format id
};



/////////////////////////////////////////////////////////////////////////////
// sample code for implementing a ole drop target (for a view)
/*
(1) In view class declare a COleDropTarget member.
(2) In view classes constructor call RegisterClipboardFormat(CELIB_CLIP_FORMAT).
(3) store the returned clip board format value in a member var.
(4) In view's OnCreate function call COleDropTargets method Register(this).
(5) use classwizard to declare overwrites OnDragOver and OnDrop.
(6) Following code shows possible implementation for OnDragOver. This function tells
the calling framework, wether a drop is possible or not.

DROPEFFECT CMyView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) {
	// TODO: Add your specialized code here and/or call the base class

		//check first wether the data is available - use stored clipboard format here (2)
		if(pDataObject->IsDataAvailable(m_uiClipFormatId)) {
			
			//get global handle from the data object
			HGLOBAL hGlob = pDataObject->GetGlobalData(m_uiClipFormatId);
			if(hGlob != NULL) {
				//use a shared file for read operation

				CSharedFile GlobFile;
				GlobFile.SetHandle(hGlob, FALSE); //init shared file with global handle
				
				//init archive
				CArchive ar(&GlobFile, CArchive::load);
				CDragItem DragItem;
				//use serialize method
				DragItem.Serialize(ar);

				//now the data is inside DragItem...
				//check wether a drop is possible and return
				//DROPEFFECT value DROPEFFECT_LINK,DROPEFFECT_NONE ...
				...

			}
		}
		break;
	
	//return with base classes implementation
	return CView::OnDragOver(pDataObject, dwKeyState, point);
}

(7) OnDrop is approximately the same.

*/

#endif // __DRAGITEM_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: Dragitem.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 3  *****************
 * User: Sis          Date: 26.02.01   Time: 19:20
 * Updated in $/4Control/COM/softing/fc/inc
 * CXMLNodeDragItem
 * 
 * *****************  Version 2  *****************
 * User: Scd          Date: 31.01.01   Time: 12:24
 * Updated in $/4Control/COM/softing/fc/inc
 * new headers added
 *==
 *----------------------------------------------------------------------------*
*/

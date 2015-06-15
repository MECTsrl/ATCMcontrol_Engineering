



//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "XMLDocument.h"
#include "XMLNode.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//------------------------------------------------------------------*
/**
 * Constructor.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
CXMLDocument::CXMLDocument()
{
    HRESULT hr = m_domDocument.CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER);
    ASSERT(hr == S_OK);
    // create processing instruction node
    CComPtr<IXMLDOMProcessingInstruction> pPINode;
    CComBSTR sTarget("xml");
    CComBSTR sData("version=\"1.0\" encoding=\"UTF-8\"");
    hr = m_domDocument->createProcessingInstruction(sTarget, sData, &pPINode);
    if(hr == S_OK)
    {
        // add processing instruction node to document
        CComQIPtr<IXMLDOMNode, &IID_IXMLDOMNode> docNode = m_domDocument;
        CComPtr<IXMLDOMNode> pOutNode;
        hr = docNode->appendChild(pPINode, &pOutNode);
		ASSERT(hr == S_OK);
    }
}


//------------------------------------------------------------------*
/**
 * destructor.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
CXMLDocument::~CXMLDocument()
{

}


//------------------------------------------------------------------*
/**
 * Load
 * load a file into the xml document.
 *
 * @param           [in] strFileName: the name of the file to be opened
 * @return          TRUE:   on success
 *                  FALSE:  else
 * @exception       -
 * @see             
*/
BOOL CXMLDocument::Load(const CString& strFileName)
{
    CComVariant vFileName = strFileName;
	VARIANT_BOOL vbIsSuccessful = VARIANT_FALSE;

    HRESULT hr =  m_domDocument->load(vFileName, &vbIsSuccessful);
	if (hr != S_OK || (vbIsSuccessful==VARIANT_FALSE))
	{
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


//------------------------------------------------------------------*
/**
 * Save
 * save the xml document to a file.
 *
 * @param           [in] strFileName: the name of the file that should be
 *                               saved
 * @return          TRUE:  on success
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CXMLDocument::Save(const CString& strFileName) const
{
    CComVariant vFileName = strFileName;

    HRESULT hr = m_domDocument->save(vFileName);
    if (hr != S_OK)
	{
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


//------------------------------------------------------------------*
/**
 * LoadXMLString
 * initialize the document with a string containing the xml representation.
 *
 * @param           [in] strXMLString: the xml representation in a string
 * @return          TRUE: on success
 *                  FLASE: else
 * @exception       -
 * @see             
*/
BOOL CXMLDocument::LoadXMLString(const CString& strXMLString)
{
    // HRESULT loadXML(BSTR bstrXML,VARIANT_BOOL * isSuccessful);
    CComBSTR sString = strXMLString;
	VARIANT_BOOL vbIsSuccessful = VARIANT_FALSE;

    HRESULT hr =  m_domDocument->loadXML(sString, &vbIsSuccessful);
	if (hr != S_OK || (vbIsSuccessful==VARIANT_FALSE))
	{
        IXMLDOMParseError * pXMLError = NULL;
        hr = m_domDocument->get_parseError(&pXMLError);
        if(SUCCEEDED(hr))
        {
            long l=0,c=0;
            BSTR bs;
            pXMLError->get_line(&l);
            pXMLError->get_linepos(&c);

            hr = pXMLError->get_reason(&bs);
           
            pXMLError->Release();
        }

        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


//------------------------------------------------------------------*
/**
 * GetXMLString
 * get the xml document as xml string representation.
 *
 * @param           [out] strXMLString : the xml representation of the document     
 * @return          TRUE:  on success
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CXMLDocument::GetXMLString(CString &strXMLString) const
{
    CComBSTR sXML;

    m_domDocument->put_preserveWhiteSpace(VARIANT_FALSE);
    HRESULT hr = m_domDocument->get_xml(&sXML);

    if (hr != S_OK)
    {
        return FALSE;
    }

    strXMLString = sXML;
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * CreateNode
 * create a new xml node.
 *
 * @param           [out] node :        The created dom node.
 *                  [in]  strTagName:   The name of the node.
 * @return          TRUE:  on success
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CXMLDocument::CreateNode(CXMLNode& node, const CString& strTagName)
{
    CComBSTR tagName;
    CComPtr<IXMLDOMNode> newNode;
    CComVariant nodeType(NODE_ELEMENT);
    CComBSTR namespaceURI;

    if (!m_domDocument)
    {
        return FALSE;
    }

    tagName = strTagName;
    HRESULT hr = m_domDocument->createNode(nodeType, tagName, namespaceURI, &newNode);
    if (hr != S_OK)
    {
        return FALSE;
    }
    
    node.SetIXMLNode(newNode);
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * GetDocNode
 * get the xml document as xml node, this node is the root node containing
 * all other xml nodes.
 *
 * @param           [out] node:     the document as xml node, this
 *                                  is the document as node interface pointer
 * @return          TRUE:  on success
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CXMLDocument::GetDocNode(CXMLNode& node) const
{
    CComQIPtr<IXMLDOMNode, &IID_IXMLDOMNode> docNode = m_domDocument;
    node.SetIXMLNode(docNode);
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * Get the root node of the xml tree. 
 *
 * @param           [out] node:     the root node, this
 *                                  is the parent node for all other nodes
 * @return          TRUE:  on success
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CXMLDocument::GetRootNode(CXMLNode& node) const
{
    CComPtr<IXMLDOMElement> rootElement;
    CComQIPtr<IXMLDOMNode, &IID_IXMLDOMNode> rootNode;

    if (!m_domDocument)
    {
        return FALSE;
    }

    HRESULT hr = m_domDocument->get_documentElement(&rootElement);
    if (hr != S_OK)
    {
        return FALSE;
    }
    rootNode = rootElement;
    if (!rootNode)
    {
        return FALSE;
    }
    
    node.SetIXMLNode(rootNode);

    return TRUE;

}

//------------------------------------------------------------------*
/**
 * SetIXMLDocument
 * initialize the document with an xml interface pointer
 *
 * @param           - IXMLDOMDocument* pDocument - interface pointer to document
 * @return          - TRUE if successful, else FALSE
*/
BOOL CXMLDocument::SetIXMLDocument(IXMLDOMDocument* p_domDocument)
{
    if (m_domDocument)
    {
        m_domDocument.Release();
    }
    m_domDocument = p_domDocument;
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * SetIXMLDocument
 * initialize the document with an xml interface pointer
 *
 * @param           - IUnknown* pDocument - interface pointer to document
 * @return          - TRUE if successful, else FALSE
*/
BOOL CXMLDocument::SetIXMLDocument(IUnknown* p_domDocument)
{
    if (m_domDocument)
    {
        m_domDocument.Release();
    }
    m_domDocument = p_domDocument;
    if (!m_domDocument)
    {
        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * Get the dom document as xml interface pointer.
 *
 * @param           - IXMLDOMDocument* pDocument - interface pointer to document
 * @return          - TRUE if successful, else FALSE
*/
BOOL CXMLDocument::GetIXMLDocument(IXMLDOMDocument** pp_domDocument)
{
    ASSERT(pp_domDocument);
    
    if (!m_domDocument)
    {
        return FALSE;
    }

    *pp_domDocument = (IXMLDOMDocument*)m_domDocument;
    (*pp_domDocument)->AddRef();

    return TRUE;
}

//------------------------------------------------------------------*
/**
 * Get the document interface pointer.
 *
 * @param           - [out] IUnknown** pp_domDocument - interface pointer to document
 * @return          - TRUE if successful, else FALSE
*/
BOOL CXMLDocument::GetIXMLDocumentUnknown(IUnknown** pp_domDocument)
{
    ASSERT(pp_domDocument);

    if (!m_domDocument)
    {
        return FALSE;
    }

    *pp_domDocument = (IUnknown*)((IXMLDOMDocument*)m_domDocument);
    (*pp_domDocument)->AddRef();

    return TRUE;
}


// Extract.h : Deklaration von CExtract

#ifndef __EXTRACT_H_
#define __EXTRACT_H_

#include "resource.h"       // Hauptsymbole


#define CEEX_HEADERFILE_PREFIX  _T("hdr")
#define CEEX_TEMPFILE_PREFIX    _T("cex")
#define CEEX_HEADER             _T("HEADER")
#define CEEX_CDATA              _T("<![CDATA[")
#define CEEX_CDATA_END          _T("]]>")

// HACK HACK HACK
// [/]* at the end of the next 4 regular expressions is a hack, 
// to allow <BODY ... /> xml constructs
// Now also wrong xml construchs <BODY ... ////> are recognized.
#define CE_EXTRACT_BEGINTAGREGEX_ATTR   _T("<[ \t]*%S[ \t]*%S[ \t]*=[ \t]*\"%S\"[^>/]*[/]*>")
#define CE_EXTRACT_BEGINTAGREGEX_NOATTR _T("<[ \t]*%S[^/>]*[/]*>")
#define CE_EXTRACT_EMPTYTAGREGEX_ATTR   _T("<[ \t]*%S[ \t]*%S[ \t]*=[ \t]*\"%S\"[^>/]*(/)[/]*>")
#define CE_EXTRACT_EMPTYTAGREGEX_NOATTR _T("<[ \t]*%S[^/>]*(/)[/]*>")
#define CE_EXTRACT_ENDTAGREGEX          _T("[\r\n\t ]*<[ \t]*/%S[ \t]*>")
#define CE_EXTRACT_CDATAREGEX           _T("<!\\[CDATA\\[.*\\]\\]>")
#define CE_EXTRACT_ATTR                 _T("%S[ \t]*=[ \t]*\"([^\"]*)\"")
#define CE_EXTRACT_XML_VERSION          _T("<\\?xml version=\"[^\"]*\"[ \t]+encoding=\"[^\"]*\"\\?>[\r\n]*")
#define CE_EXTRACT_CRLF                 _T("[\r\n]*")

#define CEEXTRACT_COPYBUF_SIZE  65536  // must be even!!!
//TODO: Revise the extract algorithm! It wont work for big files! (ED, 29.8.99)

/////////////////////////////////////////////////////////////////////////////
// CExtract
class ATL_NO_VTABLE CExtract : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CExtract, &CLSID_Extract>,
	public IDispatchImpl<IExtract, &IID_IExtract, &LIBID_CEEXTRACTLib>,
	public IDispatchImpl<IExtract2, &IID_IExtract2, &LIBID_CEEXTRACTLib>
{
public:
	CExtract()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_EXTRACT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CExtract)
	COM_INTERFACE_ENTRY(IExtract)
	COM_INTERFACE_ENTRY(IExtract2)
END_COM_MAP()

// IExtract
public:
	STDMETHOD(Merge)(/*[in, string]*/ BSTR sMergeFile, /*[in, string]*/ BSTR sHeaderFile, /*[in]*/ BOOL bDelHeaderFile);
	STDMETHOD(Extract)(/*[in, string]*/ BSTR sSrcFile, /*[out, string]*/ BSTR * sHeaderFile);

// IExtract2
	STDMETHOD(FindTaggedBlock)(/*[in, string]*/ BSTR sSrcFile, /*[in]*/ long lOffset,
                               /*[in, string]*/ BSTR sTagName, /*[in, string]*/ BSTR sAttrName,
                               /*[in, string]*/ BSTR sAttrValue, /*[out]*/ long * plStartPos, 
                               /*[out]*/ long * plLength);
	STDMETHOD(FindTag)(/*[in, string]*/ BSTR sSrcFile, /*[in]*/ long lOffset,
                       /*[in, string]*/ BSTR sTagName, /*[in, string]*/ BSTR sAttrName,
                       /*[in, string]*/ BSTR sAttrValue, /*[out]*/ long * plStartPos, 
                       /*[out]*/ long * plLength);
	STDMETHOD(FindBeginTag)(/*[in, string]*/ BSTR sSrcFile, /*[in]*/ long lOffset,
                            /*[in, string]*/ BSTR sTagName, /*[in, string]*/ BSTR sAttrName,
                            /*[in, string]*/ BSTR sAttrValue, /*[out]*/ long * plStartPos, 
                            /*[out]*/ long * plLength);
	STDMETHOD(ExtractTaggedBlock)(/*[in, string]*/ BSTR sSrcFile, /*[in]*/ long lOffset, 
		                          /*[in, string]*/ BSTR sTagName, /*[in, string]*/ BSTR sAttrName, 
								  /*[in, string]*/ BSTR sAttrValue, /*[in, string]*/ BSTR sFileExt,
                                  /*[out, string]*/ BSTR * sExtractedFile, /*[in]*/ BOOL bCDataOnly);
	STDMETHOD(MergeTaggedBlock)(/*[in, string]*/ BSTR sSrcFile, /*[in]*/ long lOffset, 
								/*[in, string]*/ BSTR sTagName, /*[in, string]*/ BSTR sAttrName, 
								/*[in, string]*/ BSTR sAttrValue, /*[in, string]*/ BSTR sMergeFile,
								/*[in]*/ BOOL bAsCData, /*[in]*/ BOOL bDelFile, /*[in]*/ BOOL bRemoveXMLVersion);
    STDMETHOD(GetXMLAttributeValue)(/*[in, string]*/ BSTR sSrcFile, /*[in]*/ long lOffset, 
		                            /*[in, string]*/ BSTR sTagName, 
                                    /*[in, string]*/ BSTR sKeyAttrName, /*[in, string]*/ BSTR sKeyAttrValue, 
                                    /*[in, string]*/ BSTR sAttrName, /*[out, string]*/ BSTR * sAttrValue,
                                    /*[out]*/ long * plPos);
    STDMETHOD(SetXMLAttributeValue)(/*[in, string]*/ BSTR sSrcFile, /*[in]*/ long lOffset, 
		                            /*[in, string]*/ BSTR sTagName, 
                                    /*[in, string]*/ BSTR sKeyAttrName, /*[in, string]*/ BSTR sKeyAttrValue, 
                                    /*[in, string]*/ BSTR sAttrName, /*[in, string]*/ BSTR sAttrValue);

private:
    HRESULT FindXMLTag(BSTR sSrcFile, long lOffset, BSTR sTagName, BSTR sAttrName,
                       BSTR sAttrValue, long * lStartPos, long * lLength, int iFindMode, BOOL bCRLF = TRUE);

    enum
    {
        enumFindBlockBetween,
        enumFindBeginTag,
        enumFindBeginToEndTag
    };

};

#endif //__EXTRACT_H_

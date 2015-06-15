

#include "stdafx.h"
#include "FcHtmlEd.h"

#include "FcHtmlEdDoc.h"
#include "FcHtmlEdView.h"
#include "SrvrItem.h"

#include "FcHtmlEdIF_i.c"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFcHtmlEdDoc

IMPLEMENT_DYNCREATE(CFcHtmlEdDoc, CStdEditorDoc)

BEGIN_MESSAGE_MAP(CFcHtmlEdDoc, CStdEditorDoc)
	//{{AFX_MSG_MAP(CFcHtmlEdDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CFcHtmlEdDoc, CStdEditorDoc)
	//{{AFX_DISPATCH_MAP(CFcHtmlEdDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IFcHtmlEd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {A04CED76-B51D-11D2-9FD9-00A024363DFC}
static const IID IID_IFcHtmlEd =
{ 0xa04ced76, 0xb51d, 0x11d2, { 0x9f, 0xd9, 0x0, 0xa0, 0x24, 0x36, 0x3d, 0xfc } };

BEGIN_INTERFACE_MAP(CFcHtmlEdDoc, CStdEditorDoc)
	INTERFACE_PART(CFcHtmlEdDoc, IID_IFcHtmlEd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFcHtmlEdDoc construction/destruction

CFcHtmlEdDoc::CFcHtmlEdDoc()
{
	EnableAutomation();

	AfxOleLockApp();

}

CFcHtmlEdDoc::~CFcHtmlEdDoc()
{
	AfxOleUnlockApp();
	ResetEdDocFile();
}

BOOL CFcHtmlEdDoc::OnNewDocument()
{
	ResetEdDocFile();

	if (!COleServerDoc::OnNewDocument())
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFcHtmlEdDoc server implementation

COleServerItem* CFcHtmlEdDoc::OnGetEmbeddedItem()
{
	// OnGetEmbeddedItem is called by the framework to get the COleServerItem
	//  that is associated with the document.  It is only called when necessary.

	CFcHtmlEdSrvrItem* pItem = new CFcHtmlEdSrvrItem(this);
	ASSERT_VALID(pItem);
	return pItem;
}

/////////////////////////////////////////////////////////////////////////////
// CFcHtmlEdDoc Active Document server implementation

CDocObjectServer *CFcHtmlEdDoc::GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite)
{
	return new CDocObjectServer(this, pDocSite);
}



/////////////////////////////////////////////////////////////////////////////
// CFcHtmlEdDoc serialization

void CFcHtmlEdDoc::Serialize(CArchive& ar)
{
	SetEdDocFile(ar);
	
	POSITION pos = GetFirstViewPosition();
	CFcHtmlEdView* pView = (CFcHtmlEdView*)GetNextView(pos);

    if (ar.IsStoring())
	{
		pView->Save();
        /*CFile file;
		if (file.Open(m_EdDocFileName, CFile::modeRead))
		{
			char pBuff[1024];
			UINT nBytes;
			while ((nBytes = file.Read(pBuff, sizeof(pBuff))) > 0)
			{
				ar.Write(pBuff, nBytes);
			}
			file.Close();
		}*/
        // --> HACK: to filter generated absolute file paths to the project directory
        //           these can't be used in library help ...
        //           (and are never a real solution for a html document)
        CopyFileAndFilterAbsPath(ar);
	}
	else
	{

		CFile file;
		if (file.Open(m_EdDocFileName, CFile::modeCreate | CFile::modeWrite))
		{
			char pBuff[1024];
			UINT nBytes;
			while ((nBytes = ar.Read(pBuff, sizeof(pBuff))) > 0)
			{
				file.Write(pBuff, nBytes);
			}
			file.Close();
		}

        pView->Load ();
	}
}


void CFcHtmlEdDoc::CopyFileAndFilterAbsPath(CArchive& ar)
{
    CFile file;
    if (file.Open(m_EdDocFileName, CFile::modeRead))
    {
        CString prjPath = file.GetFilePath();
        int bs;
        bs = prjPath.ReverseFind('\\');
        if (bs>0)
            prjPath = prjPath.Left(bs);

	    char pBuff[1050];
	    UINT nBytes;
        CString source;
        source.Empty();

        int tp[3];
        int tpend[3];
        int ftp;
        int ftpend;
        int puffSize = 10;

        while ((nBytes = file.Read(pBuff, 1024)) > 0)
	    {
            pBuff[nBytes] = 0; // termiate read string
            source += CString(pBuff);  // add the newly read part to the already read part

            do 
            {
                // search for a tag of the following: src, href, backgound
                tp[0] = source.Find("src");
                tpend[0] = tp[0]+3;
                tp[1] = source.Find("href");
                tpend[1] = tp[1]+4;
                tp[2] = source.Find("background");
                tpend[2] = tp[2]+10;

                ftp = -1;
                for (int i=0; i<3; i++)
                {
                    if (tp[i]>0 && (tp[i]<ftp || ftp==-1))
                    {
                        ftp = tp[i];
                        ftpend = tpend[i];
                    }
                }
                
                if (ftp>0)
                {
                    if (!(source[ftp-1]==' ' || source[ftp-1]=='\n') || source[ftpend]!='=')
                    {
                        // it is not really a tag, so until here no tag found, 
                        // write that part back
                        ar.Write(source, ftpend);
                        source = source.Right(source.GetLength()-ftpend);
                    }
                    else
                    {
                        // write back
                        ar.Write(source, ftpend+1);
                        source = source.Mid(ftpend+1);

                        CString fileAbs;
                        CString fileName;
                        CString filePath;
                        int startPos;
                        int endPos;

                        if (source[0]=='\"')
                        {
                            startPos = 1;
                            endPos = source.Find("\"", 1);
                        }
                        else
                        {
                            startPos = 0;
                            endPos = source.Find(" ", 0);
                            if (source.Find("\n",0)<endPos)
                                endPos = source.Find("\n",0);
                        }

                        if (endPos>0)
                        {
                            endPos;
                            fileAbs = source.Mid(startPos, endPos-startPos);
                            source = source.Mid(endPos+1);

                            bs = fileAbs.ReverseFind('\\');
                            if (bs>0)
                            {
                                filePath = fileAbs.Left(bs);
                                // look if file path starts with file: (//)
                                if (filePath.Left(5).CompareNoCase("file:")==0)
                                {
                                    filePath = filePath.Mid(5);
                                    if (filePath.Left(2).Compare("//")==0)
                                        filePath = filePath.Mid(2);
                                }
                                fileName = fileAbs.Mid(bs+1);
                                if (filePath.CompareNoCase(prjPath)==0)
                                {
                                    ar.Write("\""+fileName+"\"", fileName.GetLength()+2);
                                }
                                else
                                {
                                    ar.Write("\""+fileAbs+"\"", fileAbs.GetLength()+2); 
                                }
                            }
                            else
                            {
                                ar.Write("\""+fileAbs+"\"", fileAbs.GetLength()+2); 
                            }
                        }      
                    }
                }            
            } while(ftp>0); // until there are no more tags found in that part

            // write back the rest, but buffer a few characters, 
            // the tags could overlap this part and the next part 
            if (source.GetLength()>puffSize)
            {
                ar.Write(source, source.GetLength()-puffSize);
                source = source.Right(puffSize);
            }   
        }

        if (!source.IsEmpty())
            ar.Write(source, source.GetLength());

	    file.Close();
    }
}


void CFcHtmlEdDoc::OnSerializeDocument (CArchive &ar)
{
	Serialize(ar);
	if (ar.IsLoading())
		UpdateAllViews(NULL);
}

void CFcHtmlEdDoc::SetEdDocFile(CArchive& ar)
{
	ResetEdDocFile();

	const CFile* fp = ar.GetFile();
	CString FilePath = fp->GetFilePath();
	if (FilePath.GetLength() <= 0)
	{
		char TmpPath[MAX_PATH];
		GetTempPath(sizeof(TmpPath), TmpPath);
		FilePath = TmpPath;
	}
	else
	{
		CString FileName = fp->GetFileName();
		FilePath = FilePath.Left(FilePath.GetLength() - FileName.GetLength());
	}

	GetTempFileName(FilePath, "FCH", 0, m_EdDocFileName.GetBuffer(MAX_PATH));
	m_EdDocFileName.ReleaseBuffer();
}

void CFcHtmlEdDoc::ResetEdDocFile()
{
	if (m_EdDocFileName.GetLength() > 0)
	{
		CFile::Remove(m_EdDocFileName);
		m_EdDocFileName.Empty();
	}
}



/////////////////////////////////////////////////////////////////////////////
// CFcHtmlEdDoc diagnostics

#ifdef _DEBUG
void CFcHtmlEdDoc::AssertValid() const
{
	COleServerDoc::AssertValid();
}

void CFcHtmlEdDoc::Dump(CDumpContext& dc) const
{
	COleServerDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFcHtmlEdDoc commands

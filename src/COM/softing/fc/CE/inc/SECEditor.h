// SECEditor.h: interface for the CSECEditor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SECEDITOR_H__56F3ED24_28CC_11D5_B203_00E029440C3D__INCLUDED_)
#define AFX_SECEDITOR_H__56F3ED24_28CC_11D5_B203_00E029440C3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SECEdit.h"
#include "StdEdAttr.h"

class CDocument;


class CSECEditor :public SECEdit
{
public:
    enum SmartState
    {
        SmartError,
        SmartIdentifier,
        SmartComponent,
        SmartIndexing
    };

    CSECEditor();
    virtual ~CSECEditor();
    int     GetLinesInFile(){return m_iLinesInFile; }

    CString GetTextLine    (int iLine);
    CString GetSmartText   (const CStdEditorPosition &position, CStdEditorPosition &startPosition, CStdEditorPosition &endPosition);
    CString GetTrimmedText (const CStdEditorPosition &startPosition, const CStdEditorPosition &endPosition, CStdEditorPosition &trimmedStart, CStdEditorPosition &trimmedEnd);

    CStdEditorPosition ToOffsetPosition (const CStdEditorPosition &position);
    CStdEditorPosition ToDisplayPosition(const CStdEditorPosition &position);


    bool ShowBreakpoint (int iLine, bool bShow);
    bool ShowStoppoint  (int iLine, bool bShow);
    bool ShowStep       (int iLine, bool bShow);
    bool ShowAction     (int iLine, bool bShow);
    bool ShowTransition (int iLine, bool bShow);

    bool ExpandLine         (PLINEDESC pLine, CString &strText);
    bool FindKeywordPosition(const CStdEditorPosition &idPosition, CStdEditorPosition &keywordPosition, CStdEditorPosition &behindIdPosition);
    
    HRESULT DecodeLocation  (const CString &strLocation, CStdEditorPosition &position);

protected:
    bool IsBeginning        (const TEXTPOS &textPos);
    bool IsEnd              (const TEXTPOS &textPos);
    char GetCharacter       (const TEXTPOS &textPos);
    CString GetTextBetween  (const TEXTPOS &startTextPos, const TEXTPOS &endTextPos);
    bool GetSmartTextLimits (const TEXTPOS &textPos, TEXTPOS &startTextPos, TEXTPOS &endTextPos);


    void MoveForward        (TEXTPOS &textPos);
    void MoveBack           (TEXTPOS &textPos);
    void SkipSpacesForward  (TEXTPOS &textPos);
    void SkipSpacesBack     (TEXTPOS &textPos);

    bool IsIdentifier       (char c);
    bool IsValidLine        (int iLine);

    bool MoveForwardComponent  (TEXTPOS &textPos);
    bool MoveForwardIndexing   (TEXTPOS &textPos);
    bool MoveForwardIdentifier (TEXTPOS &textPos);
    bool MoveBackComponent     (TEXTPOS &textPos);
    bool MoveBackIndexing      (TEXTPOS &textPos);
    bool MoveBackIdentifier    (TEXTPOS &textPos);

    SmartState GetInitialSmartState (TEXTPOS &textPos);
};

#endif // !defined(AFX_SECEDITOR_H__56F3ED24_28CC_11D5_B203_00E029440C3D__INCLUDED_)

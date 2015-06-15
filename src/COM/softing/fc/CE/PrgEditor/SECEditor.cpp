// SECEditor.cpp: implementation of the CSECEditor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "SECEditor.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSECEditor::CSECEditor()
{

}

CSECEditor::~CSECEditor()
{

}

char CSECEditor::GetCharacter (const TEXTPOS &textPos)
{
    return (textPos.pLine->pText[textPos.iOffset]);
}


bool CSECEditor::IsBeginning (const TEXTPOS &textPos)
{
    return (textPos.iOffset == 0 && textPos.pLine->pPrev == NULL);
}


bool CSECEditor::IsEnd (const TEXTPOS &textPos)
{
    return (textPos.iOffset >= textPos.pLine->iTxtSize - 1 && textPos.pLine->pNext == NULL);
}


void CSECEditor::MoveForward (TEXTPOS &textPos)
{
	// #352 20.02.07 SIS >>
	// avoid invalid positions
	int iNewOffset = textPos.iOffset + 1;
    if (iNewOffset == textPos.pLine->iTxtSize)
    {
		// do not move beyond last line
		if(textPos.pLine->pNext)
		{
			textPos.pLine = textPos.pLine->pNext;
			textPos.iOffset = 0;
			iNewOffset = 0;
		}
    }
	// do not move beyond last character of line
	if(iNewOffset < textPos.pLine->iTxtSize)
	{
		textPos.iOffset = iNewOffset;
	}
	// #352 20.02.07 SIS <<
}


void CSECEditor::MoveBack (TEXTPOS &textPos)
{
    if (--textPos.iOffset < 0)
    {
        if(textPos.pLine->pPrev)
        {
           textPos.pLine = textPos.pLine->pPrev;
           textPos.iOffset = textPos.pLine->iTxtSize - 1;
        }
    };
}


void CSECEditor::SkipSpacesForward (TEXTPOS &textPos)
{
    while (!IsEnd (textPos) && isspace (GetCharacter (textPos)))
    {
        MoveForward (textPos);
    };
}


void CSECEditor::SkipSpacesBack (TEXTPOS &textPos)
{
    while (!IsBeginning (textPos) && isspace (GetCharacter (textPos)))
    {
        MoveBack (textPos);
    };
}


CString CSECEditor::GetTrimmedText (const CStdEditorPosition &startPosition, const CStdEditorPosition &endPosition, CStdEditorPosition &trimmedStart, CStdEditorPosition &trimmedEnd)
{
    CString strText;
    int iStartOffset;
    int iStartLine;
    PLINEDESC pStartLine;
    int iEndOffset;
    int iEndLine;
    PLINEDESC pEndLine;
    TEXTPOS startTextPos;
    TEXTPOS endTextPos;

    ToOffsetPosition (startPosition).GetXYPosition (iStartOffset, iStartLine);
    pStartLine = GoToLineNo (iStartLine);
    iStartOffset = -iStartOffset;
    if (iStartOffset > pStartLine->iTxtSize)
    {
        iStartOffset = pStartLine->iTxtSize;
    };

    ToOffsetPosition (endPosition).GetXYPosition (iEndOffset, iEndLine);
    pEndLine = GoToLineNo (iEndLine);
    iEndOffset = -iEndOffset;
    if (iEndOffset >= pEndLine->iTxtSize)
    {
        iEndOffset = pEndLine->iTxtSize;
    };

    if (iStartLine < iEndLine || iStartLine == iEndLine && iStartOffset < iEndOffset)
    {
        while (iStartLine < iEndLine || iStartLine == iEndLine && iStartOffset < iEndOffset)
        {
            while (iStartOffset < pStartLine->iTxtSize && isspace (pStartLine->pText[iStartOffset]))
            {
                ++iStartOffset;
            };

            if (iStartOffset < pStartLine->iTxtSize)
                break;

            pStartLine = pStartLine->pNext;
            ++iStartLine;
            iStartOffset = 0;
        };

        while (iEndLine > iStartLine || iEndLine == iStartLine && iEndOffset > iStartOffset)
        {
            while (iEndOffset > 0 && isspace (pEndLine->pText[iEndOffset - 1]))
            {
                --iEndOffset;
            };

            if (iEndOffset > 0)
                break;

            pEndLine = pEndLine->pPrev;
            --iEndLine;
            iEndOffset = pEndLine->iTxtSize;
        };
    };

    startTextPos.pLine = pStartLine;
    startTextPos.iOffset = iStartOffset;
    endTextPos.pLine = pEndLine;
    endTextPos.iOffset = iEndOffset;

    strText = GetTextBetween (startTextPos, endTextPos);

    trimmedStart = ToDisplayPosition (CStdEditorPosition (EDP_STLINE, -iStartOffset, iStartLine));
    trimmedEnd = ToDisplayPosition (CStdEditorPosition (EDP_STLINE, -iEndOffset, iEndLine));

    return (strText);
}


CString CSECEditor::GetTextBetween (const TEXTPOS &startTextPos, const TEXTPOS &endTextPos)
{
    PLINEDESC pLine;
    int iLine;
    int iCount=0;
    int iTextOffset=0;
    int iOffset;


    // First determine an upper limit of characters between start
    // position and end position (number of characters in concerned
    // lines).

    pLine = startTextPos.pLine;
    for (iLine = startTextPos.pLine->iLineNo; iLine <= endTextPos.pLine->iLineNo; ++iLine)
    {
        iCount += pLine->iTxtSize;
        pLine = pLine->pNext;
    };

    CString strText (_T (' '), iCount);

    iOffset = startTextPos.iOffset;
    pLine = startTextPos.pLine;
    for (iLine = startTextPos.pLine->iLineNo; iLine < endTextPos.pLine->iLineNo; ++iLine)
    {
        while (iOffset < pLine->iTxtSize)
        {
            char c;

            c = pLine->pText[iOffset];

            if (c != '\r')
            {
                if (c == '\n')
                {
                    c = ' ';
                };

                if (iTextOffset >= iCount)
                {
                    return ("");
                };

                strText.SetAt (iTextOffset++, _T (c));
            };

            ++iOffset;
        };

        pLine = pLine->pNext;
        iOffset = 0;
    };

    while (iOffset < endTextPos.iOffset)
    {
        char c;

        c = pLine->pText[iOffset];
        if (c != '\r' && c != '\n')
        {
            if (iTextOffset >= iCount)
            {
                return ("");
            };

            strText.SetAt (iTextOffset++, _T (c));
        };

        ++iOffset;
    };

    return (strText.Left (iTextOffset));
}


CString CSECEditor::GetSmartText (const CStdEditorPosition &position, CStdEditorPosition &startPosition, CStdEditorPosition &endPosition)
{
    CStdEditorPosition offsetPosition;
    int iLine;
    int iColumn;
    PLINEDESC pLine;
    TEXTPOS textPos;
    char c;
    TEXTPOS startTextPos;
    TEXTPOS endTextPos;

    offsetPosition = ToOffsetPosition (position);
    offsetPosition.GetXYPosition (iColumn, iLine);

    pLine = iLine? GoToLineNo (iLine): NULL;
    textPos.pLine = pLine;
    textPos.iOffset = -iColumn;

    if (!textPos.pLine || textPos.iOffset >= textPos.pLine->iTxtSize)
    {
        return ("");
    };

    c = GetCharacter (textPos);

    if (c != '[' && c != ']' && c != '.' && !IsIdentifier (c))
    {
        return ("");
    };

    if (!GetSmartTextLimits (textPos, startTextPos, endTextPos))
    {
        return ("");
    };

    startPosition = ToDisplayPosition (CStdEditorPosition (EDP_STLINE, -startTextPos.iOffset, startTextPos.pLine->iLineNo));
    endPosition   = ToDisplayPosition (CStdEditorPosition (EDP_STLINE, -endTextPos.iOffset,   endTextPos.pLine->iLineNo));

    return (GetTextBetween (startTextPos, endTextPos));
}


bool CSECEditor::GetSmartTextLimits (const TEXTPOS &textPos, TEXTPOS &startTextPos, TEXTPOS &endTextPos)
{
    bool bReady;
    SmartState state;
    TEXTPOS lastTextPos;

    startTextPos = textPos;

    state = GetInitialSmartState (startTextPos);
    if (state == SmartError)
    {
        return (false);
    };

    bReady = false;
    while (!bReady)
    {
        switch (state)
        {
        case SmartIdentifier:
            if (MoveBackComponent (startTextPos))
            {
                state = SmartComponent;
            }
            else
            {
                bReady = true;
            };
            break;

        case SmartIndexing:
            if (MoveBackIdentifier (startTextPos))
            {
                state = SmartIdentifier;
            }
            else if (MoveBackIndexing (startTextPos))
            {
                state = SmartIndexing;
            }
            else
            {
                MoveForwardIndexing (startTextPos);
                state = GetInitialSmartState (startTextPos);
                bReady = true;
            };
            break;

        case SmartComponent:
            if (MoveBackIdentifier (startTextPos))
            {
                state = SmartIdentifier;
            }
            else if (MoveBackIndexing (startTextPos))
            {
                state = SmartIndexing;
            }
            else
            {
                MoveForwardComponent (startTextPos);
                state = GetInitialSmartState (startTextPos);
                bReady = true;
            };
            break;
        };
    };

    if (state == SmartError)
    {
        return (false);
    };

    endTextPos = startTextPos;

    switch (state)
    {
    case SmartIdentifier:
        MoveForwardIdentifier (endTextPos);
        break;

    case SmartComponent:
        MoveForwardComponent (endTextPos);
        break;

    case SmartIndexing:
        MoveForwardIndexing (endTextPos);
        break;
    };

    bReady = false;
    while (!bReady)
    {
        switch (state)
        {
        case SmartIdentifier:
            if (MoveForwardComponent (endTextPos))
            {
                state = SmartComponent;
            }
            else if (MoveForwardIndexing (endTextPos))
            {
                state = SmartIndexing;
            }
            else
            {
                bReady = true;
            };
            break;

        case SmartComponent:
            if (MoveForwardIdentifier (endTextPos))
            {
                state = SmartIdentifier;
            }
            else
            {
                MoveBackComponent (endTextPos);
                bReady = true;
            };
            break;

        case SmartIndexing:
            if (MoveForwardIndexing (endTextPos))
            {
                state = SmartIndexing;
            }
            else if (MoveForwardComponent (endTextPos))
            {
                state = SmartComponent;
            }
            else
            {
                bReady = true;
            };
            break;
        };
    };


    // Text position passed to this method has to be inside the calcu-
    // lated expression text range. Assume passing the position of '.'
    // in an invalid expression 'y[a].' to this method. The code above
    // calculates an expression text range which spans 'y[a]' without
    // the '.'. We don't want this to be a valid expression text range
    // for 'y[a].' if the starting point was '.'. However we want this
    // to be a valid expression text range for 'y[a].' if the starting
    // point was somewhere inside 'y[a]'.

    lastTextPos = endTextPos;
    MoveBack (lastTextPos);

    if (textPos.pLine->iLineNo < startTextPos.pLine->iLineNo ||
        textPos.pLine->iLineNo == startTextPos.pLine->iLineNo && textPos.iOffset < startTextPos.iOffset ||
        textPos.pLine->iLineNo > lastTextPos.pLine->iLineNo ||
        textPos.pLine->iLineNo == lastTextPos.pLine->iLineNo && textPos.iOffset > lastTextPos.iOffset)
    {
        return (false);
    };

    return (true);
}

CSECEditor::SmartState CSECEditor::GetInitialSmartState (TEXTPOS &textPos)
{
    if (GetCharacter (textPos) == '[')
    {
        return (SmartIndexing);
    };

    if (GetCharacter (textPos) == ']')
    {
        MoveForward (textPos);
        if (!MoveBackIndexing (textPos))
        {
            return (SmartError);
        };

        return (SmartIndexing);
    };

    if (GetCharacter (textPos) == '.')
    {
        return (SmartComponent);
    };

    if (IsIdentifier (GetCharacter (textPos)))
    {
        MoveForward (textPos);
        MoveBackIdentifier (textPos);
        return (SmartIdentifier);
    };

    return (SmartError);
}

bool CSECEditor::FindKeywordPosition (const CStdEditorPosition &idPosition, CStdEditorPosition &keywordPosition, CStdEditorPosition &behindIdPosition)
{
    int iColumn;
    int iLine;
    PLINEDESC pLine;
    TEXTPOS textPos;

    ToOffsetPosition (idPosition).GetXYPosition (iColumn, iLine);

    pLine = GoToLineNo (iLine);

    textPos.pLine = pLine;
    textPos.iOffset = -iColumn;

    if (!MoveBackIdentifier (textPos))
    {
        return (false);
    };

    keywordPosition = CStdEditorPosition (EDP_STLINE, -textPos.iOffset, textPos.pLine->iLineNo);

    textPos.pLine = pLine;
    textPos.iOffset = -iColumn;

    MoveForwardIdentifier (textPos);

    behindIdPosition = CStdEditorPosition (EDP_STLINE, -textPos.iOffset, textPos.pLine->iLineNo);

    return (true);
}


CStdEditorPosition CSECEditor::ToOffsetPosition (const CStdEditorPosition &position)
{
    int iLine;
    int iColumn;

    position.GetXYPosition (iColumn, iLine);

    if (iColumn > 0 && IsValidLine(iLine))
    {
        PLINEDESC pLineDesc;

        pLineDesc = GoToLineNo (iLine);
        iColumn = -(GetRealOffset (pLineDesc, iColumn - 1));
    };

    return (CStdEditorPosition (EDP_STLINE, iColumn, iLine, position.GetCX(), position.GetCY()));	// s-2155 22.03.05 SIS
}

CStdEditorPosition CSECEditor::ToDisplayPosition (const CStdEditorPosition &position)
{
    int iLine;
    int iColumn;

    position.GetXYPosition (iColumn, iLine);

    if (iColumn <= 0&& IsValidLine(iLine))
    {
        PLINEDESC pLineDesc;

        pLineDesc = GoToLineNo (iLine);
        iColumn = GetTabbedOffset (pLineDesc, -iColumn) + 1;
    };

    return (CStdEditorPosition (EDP_STLINE, iColumn, iLine, position.GetCX(), position.GetCY()));	// s-2155 22.03.05 SIS
}


CString CSECEditor::GetTextLine (int iLine)
{
    PLINEDESC pLineDesc;

    pLineDesc = GoToLineNo (iLine);
    return (CString (pLineDesc->pText));
}


bool CSECEditor::IsValidLine(int iLine)
{
    return (iLine >0 && iLine <= m_iLinesInFile);
}


bool CSECEditor::ShowBreakpoint (int iLine, bool bShow)
{
    PLINEDESC pLineDesc;
    if(IsValidLine(iLine))
    {
        pLineDesc = GoToLineNo (iLine);
        assert (pLineDesc != NULL);

        if (pLineDesc != NULL)
        {
            pLineDesc->bBreakpoint = bShow;
            return true;
        };
    }
    return false;
}


bool CSECEditor::ShowStoppoint (int iLine, bool bShow)
{
    PLINEDESC pLineDesc;
    if(IsValidLine(iLine))
    {
        pLineDesc = GoToLineNo (iLine);
        assert (pLineDesc != NULL);

        if (pLineDesc != NULL)
        {
            pLineDesc->bStoppoint = bShow;
            return true;
        };
    }
    return false;
}

bool CSECEditor::ShowStep (int iLine, bool bShow)
{
    PLINEDESC pLineDesc;
    if(IsValidLine(iLine))
    {
        pLineDesc = GoToLineNo (iLine);
        assert (pLineDesc != NULL);

        if (pLineDesc != NULL)
        {
            pLineDesc->bStep = bShow;
            return true;
        };
    }
    return false;
}

bool CSECEditor::ShowAction (int iLine, bool bShow)
{
    PLINEDESC pLineDesc;
    if(IsValidLine(iLine))
    {
        pLineDesc = GoToLineNo (iLine);
        assert (pLineDesc != NULL);

        if (pLineDesc != NULL)
        {
            pLineDesc->bAction = bShow;
            return true;
        };
    }
    return false;
}

bool CSECEditor::ShowTransition(int iLine, bool bShow)
{
    PLINEDESC pLineDesc;
    if(IsValidLine(iLine))
    {
        pLineDesc = GoToLineNo (iLine);
        assert (pLineDesc != NULL);

        if (pLineDesc != NULL)
        {
            pLineDesc->bTransition = bShow;
            return true;
        };
    }
    return false;
}


bool CSECEditor::MoveForwardComponent (TEXTPOS &textPos)
{
    TEXTPOS tempPos=textPos;

    SkipSpacesForward (tempPos);

    if (IsEnd (tempPos) || GetCharacter (tempPos) != '.')
    {
        return (false);
    };

    MoveForward (tempPos);

    textPos = tempPos;
    return (true);
}


bool CSECEditor::MoveForwardIndexing (TEXTPOS &textPos)
{
    TEXTPOS tempPos=textPos;
    unsigned int uiBracketCount=0;

    SkipSpacesForward (tempPos);

    if (IsEnd (tempPos) || GetCharacter (tempPos) != '[')
    {
        return (false);
    };

    do
    {
        if (GetCharacter (tempPos) == '[')
        {
            ++uiBracketCount;
        }
        else if (GetCharacter (tempPos) == ']')
        {
            --uiBracketCount;
        };

        MoveForward (tempPos);
    } while (!IsEnd (tempPos) && uiBracketCount != 0);

    if (uiBracketCount != 0)
    {
        return (false);
    };

    textPos = tempPos;
    return (true);
}


bool CSECEditor::MoveForwardIdentifier (TEXTPOS &textPos)
{
    TEXTPOS tempPos=textPos;

    SkipSpacesForward (tempPos);

    if (IsEnd (tempPos) || !IsIdentifier (GetCharacter (tempPos)))
    {
        return (false);
    };

    do
    {
        MoveForward (tempPos);
    } while (!IsEnd (tempPos) && IsIdentifier (GetCharacter (tempPos)));

    textPos = tempPos;
    return (true);
}


bool CSECEditor::MoveBackComponent (TEXTPOS &textPos)
{
    TEXTPOS tempPos=textPos;

    if (IsBeginning (textPos))
    {
        return (false);
    };

    MoveBack (tempPos);
    SkipSpacesBack (tempPos);

    if (GetCharacter (tempPos) != '.')
    {
        return (false);
    };

    textPos = tempPos;
    return (true);
}


bool CSECEditor::MoveBackIndexing (TEXTPOS &textPos)
{
    TEXTPOS tempPos=textPos;
    unsigned int uiBracketCount=0;

    if (IsBeginning (textPos))
    {
        return (false);
    };

    MoveBack (tempPos);
    SkipSpacesBack (tempPos);

    if (GetCharacter (tempPos) != ']')
    {
        return (false);
    };

    do
    {
        if (GetCharacter (tempPos) == '[')
        {
            --uiBracketCount;
        }
        else if (GetCharacter (tempPos) == ']')
        {
            ++uiBracketCount;
        };

        MoveBack (tempPos);
    } while (!IsBeginning (tempPos) && uiBracketCount != 0);

    if (uiBracketCount != 0)
    {
        return (false);
    };

    MoveForward (tempPos);

    textPos = tempPos;
    return (true);
}


bool CSECEditor::MoveBackIdentifier (TEXTPOS &textPos)
{
    TEXTPOS tempPos=textPos;

    if (IsBeginning (tempPos) || tempPos.pLine==NULL)
    {
        return (false);
    };

    MoveBack (tempPos);
    SkipSpacesBack (tempPos);

    if (!IsIdentifier (GetCharacter (tempPos)))
    {
        return (false);
    };

    do
    {
        MoveBack (tempPos);
    } while (!IsBeginning (tempPos) && IsIdentifier (GetCharacter (tempPos)));

    if (!IsIdentifier (GetCharacter (tempPos)))
    {
        MoveForward (tempPos);
    };

    textPos = tempPos;
    return (true);
}


bool CSECEditor::ExpandLine (PLINEDESC pLine, CString &strText)
{
    LPTSTR pszBuffer;
    int iBufferSize;
    LPCSTR pchText;
    int iLength;
    int iTabWidth;
    int iBufferOffset=0;
    int iOffset;

    ASSERT (pLine != NULL);
    if (pLine == NULL)
        return (false);


    iTabWidth = GetTabSize ();
    if (iTabWidth == 0)
        iTabWidth = 1;


    pchText = pLine->pText;
    ASSERT (pchText != NULL);
    if (pchText == NULL)
        return (false);

    iLength = pLine->iTxtSize;

    if (iLength >= 2 && pchText[iLength - 2] == '\r' && pchText[iLength - 1] == '\n')
    {
        iLength -= 2;
    };


    // As many characters as could be occupied by line containing
    // tabs only. Allocate one extra character for null terminator.

    iBufferSize = iLength * iTabWidth + 1;
    pszBuffer = new TCHAR[iBufferSize + 1];
    ASSERT (pszBuffer != NULL);
    if (pszBuffer == NULL)
        return (false);

    for (iOffset = 0; iOffset < iLength; ++iOffset)
    {
        char ch;

        ch = pchText[iOffset];

        if (ch == '\t')
        {
            int iSpaces;

            iSpaces = 4 - (iBufferOffset % iTabWidth);
            while (iSpaces-- != 0)
            {
                if (iBufferOffset == iBufferSize)
                {
                    delete pszBuffer;
                    return (false);
                };

                pszBuffer[iBufferOffset++] = _T (' ');
            };
        }
        else
        {
            if (iBufferOffset == iBufferSize)
            {
                delete pszBuffer;
                return (false);
            };

            pszBuffer[iBufferOffset++] = ch;
        };
    };

    pszBuffer[iBufferOffset] = _T ('\0');
    strText = pszBuffer;

    delete pszBuffer;
    return (true);
}


bool CSECEditor::IsIdentifier (char c)
{
    return ((c >= 'A') && (c <= 'Z') ||
        (c >= 'a') && (c <= 'z') ||
        (c >= '0') && (c <= '9') ||
        (c == '_'));
}

HRESULT CSECEditor::DecodeLocation (const CString &strLocation, CStdEditorPosition &position)
{
    int iPos;
    CString strLine;
    CString strColumn;
    LPTSTR pszLine;
    LPTSTR pszColumn;
    LPTSTR pszEnd;
    unsigned long ulLine;
    unsigned long ulColumn;

    iPos = strLocation.Find (_T (','));
    if (iPos == -1)
    {
        strLine = strLocation;
        strColumn = "1";
    }
    else
    {
        strLine = strLocation.Left (iPos);

        strColumn = strLocation.Mid (iPos + 1);
        strColumn.TrimLeft ();
        strColumn.TrimRight ();

        if (strColumn.IsEmpty ())
        {
            return (E_FAIL);
        };
    };

    strLine.TrimLeft ();
    strLine.TrimRight ();

    if (strLine.IsEmpty ())
    {
        return (E_FAIL);
    };

    pszLine = strLine.GetBuffer (0);
    ulLine = _tcstoul (pszLine, &pszEnd, 10);
    strLine.ReleaseBuffer ();
    if (*pszEnd != _T ('\0'))
    {
        return (E_FAIL);
    };

    pszColumn = strColumn.GetBuffer (0);
    ulColumn = _tcstoul (pszColumn, &pszEnd, 10);
    strColumn.ReleaseBuffer ();
    if (*pszEnd != _T ('\0'))
    {
        return (E_FAIL);
    };

    position = CStdEditorPosition (EDP_STLINE, ulColumn, ulLine);
    return (S_OK);
}

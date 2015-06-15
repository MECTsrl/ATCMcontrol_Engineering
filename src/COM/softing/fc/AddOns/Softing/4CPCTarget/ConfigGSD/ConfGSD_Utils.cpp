//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "ConfGSD_Utils.hpp"
#include "resource.h"

// some DP structures have odd length so we must align to bytes !!!!!!
#pragma pack (1)
//----  Local Defines:   -----------------------------------------------------*

//----  Local Prototypes:   --------------------------------------------------*
static void DecodeSpecFormat(BYTE const* pCfgData,int idsInOutString, CString& sCfgTxt,
                             int& lenIO);

//----  Static Initializations:   --------------------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//----------------------------- FUNCTION -------------------------------------*
BOOL
    DecodeCfgData(CByteArray const* pCfg,
                  CStringArray& aTexts, CUIntArray& aOffsets, CUIntArray& aLengths,
                  int& iTotalInputs, int& iTotalOutputs, int& iTotalInAndOuts, int& iSeq)
/*>>>> 
decode slave config data bytes to string

I   pCfg:       cfg data structure
O   aTexts:     array of texts to decoded config byte(s)
O   aOffsets:   array with start offsets of config sequence above
O   iTotalInputs    total bytes of inputs and outputs (incl. <iTotalInAndOuts> !)
O   iTotalOutputs
O   iTotalInAndOuts total bytes which allow input and output
0   iSeq        number of config sequences = number of entries in arrays
Result
    TRUE if ok 
<<<<*/
{
    int lenCfgData = pCfg->GetSize();
    BYTE*   pCfgData = (BYTE*) (pCfg->GetData());
    CString sCfgText;
    int     cntDecodedBytes;
    int     iOffset = 0;
    int     lenRead, lenWrite, lenReadAndWrite;
    BOOL    bConfigOK = TRUE;
    iTotalInputs    = 0;
    iTotalOutputs   = 0;
    iTotalInAndOuts = 0;
    iSeq            = 0;
    while (lenCfgData > 0)
    {
        cntDecodedBytes = 0;
        lenRead  = 0;
        lenWrite = 0;
        lenReadAndWrite = 0;
        if (::DecodeCfgBytes(pCfgData, sCfgText,
                             cntDecodedBytes, lenRead, lenWrite, lenReadAndWrite))
        {
            aTexts.SetAtGrow(iSeq, sCfgText);
            aOffsets.SetAtGrow(iSeq, iOffset);
            aLengths.SetAtGrow(iSeq, cntDecodedBytes);
            iOffset         += cntDecodedBytes;
            lenCfgData      -= cntDecodedBytes;
            iTotalInputs    += lenRead;
            iTotalOutputs   += lenWrite;
            iTotalInAndOuts += lenReadAndWrite;
            iSeq++;
        }
        else 
        {
            TRACE0("DecodeCfgData: Bad config data bytes\n");
            bConfigOK = FALSE;
            break;
        }
    };
    if (lenCfgData != 0)
    {
        ASSERT(FALSE);
        bConfigOK = FALSE;
    }

    return bConfigOK;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL
    DecodeCfgBytes(BYTE*& pCfgData, CString& sCfgText, int& cntDecodedBytes,
                   int& lenInputs, int& lenOutputs, int& lenReadAndWrite)
/*>>>> 
decode single sequence of slave config data bytes to string

IO  pCfgData:           buffer to config data bytes
O   sCfgText:           string to receive config text
O   cntDecodedBytes:    number of bytes actually decoded
O   lenInputs:          number of input bytes
O   lenOutputs:         number of output bytes
O   lenReadAndWrite:    number of bytes which allow inputs and outputs

Result
  TRUE if ok
<<<<*/
{
    ASSERT(pCfgData != NULL);
    cntDecodedBytes = 0;
    lenInputs   = 0;
    lenOutputs  = 0;
    lenReadAndWrite = 0;
    CString sTxt;

    /*--- IDENTIFIER -------------------------------------------------------*/
    // decode if standard configuration or special identifier byte
    if ((*pCfgData & DP_IDENTIFIER_IO) != DP_IDENTIFIER_IO_SPECIAL)
    {
        //******    standard configuration bytes:
        int len = ((*pCfgData & DP_IDENTIFIER_DATA_LEN) + 1);

        // is length in word or bytes??
        if (*pCfgData & DP_IDENTIFIER_LEN)
        {
            // length is in words, so double to get bytes:
            len *= 2;
        }

        // decode direction
        int ids = 0;
        switch (*pCfgData & DP_IDENTIFIER_IO)
        {
        case DP_IDENTIFIER_IO_INP:
            lenInputs   = len;
            ids = IDS_DP_CFG_INPUT;
            break;
        case DP_IDENTIFIER_IO_OUTP:
            lenOutputs  = len;
            ids = IDS_DP_CFG_OUTPUT;
            break;
        case DP_IDENTIFIER_IO_INP_OUTP:
            lenInputs   = len;
            lenOutputs  = len;
            lenReadAndWrite = len;
            ids = IDS_DP_CFG_INPUT_OUTPUT;
            break;
        default:
            ASSERT(FALSE);
            break;
        }
        CString sNum;
        sNum.Format(_T("%d"), len);
        ::AfxFormatString1(sCfgText, ids, sNum);

        if (*pCfgData & DP_IDENTIFIER_CONSISTENCY)
        {
            sTxt.LoadString(IDS_DP_CFG_CONSISTENCY);
            sCfgText += sTxt;
        }
        else
        {
            sTxt.LoadString(IDS_DP_CFG_NO_CONSISTENCY);
            sCfgText += sTxt;
        }

        pCfgData++;
        cntDecodedBytes++;

    } /* if DP_IDENTIFIER_IO_INP */
    else
    {
        //******    special identifier configuration bytes:
        sCfgText.LoadString(IDS_DP_CFG_SPECIAL_IDENT);

        BYTE    length_byte = (BYTE)(*pCfgData & DP_IDENTIFIER_SPECIAL);
        int     lenUserData =  _DP_GET_IDENTIFIER_USER_LEN (pCfgData);

        pCfgData++;
        cntDecodedBytes++;
        int lenIO = 0;

        CString sTxt;
        switch (length_byte)
        {
        case DP_IDENTIFIER_FREE_PLACE:
            sTxt.LoadString(IDS_DP_CFG_SPECIAL_EMPTY);
            sCfgText += sTxt;
            // do not increment pCfgData,
            // since this is a empty slot w/o following lengthbyte!
            break;
        case DP_IDENTIFIER_INP_LEN:
            ::DecodeSpecFormat(pCfgData, IDS_DP_CFG_INPUT, sTxt, lenIO);
            sCfgText += sTxt;
            pCfgData++;
            cntDecodedBytes++;
            lenInputs   = lenIO;
            break;
        case DP_IDENTIFIER_OUTP_LEN:
            ::DecodeSpecFormat(pCfgData, IDS_DP_CFG_OUTPUT, sTxt, lenIO);
            sCfgText += sTxt;
            pCfgData++;
            cntDecodedBytes++;
            lenOutputs  = lenIO;
            break;
        case DP_IDENTIFIER_INP_OUTP_LEN:
            ::DecodeSpecFormat(pCfgData, IDS_DP_CFG_OUTPUT, sTxt, lenIO);
            sCfgText += sTxt;
            pCfgData++;
            cntDecodedBytes++;
            lenOutputs   = lenIO;
            ::DecodeSpecFormat(pCfgData, IDS_DP_CFG_INPUT, sTxt, lenIO);
            sCfgText += _T(" / ");
            sCfgText += sTxt;
            pCfgData++;
            cntDecodedBytes++;
            lenInputs  = lenIO;
            break;
        }

        CString sNum;
        sNum.Format(_T("%d"), lenUserData);
        ::AfxFormatString1(sTxt, IDS_DP_CFG_USER_DATA_LEN, sNum);
        sCfgText += _T(", ");
        sCfgText += sTxt;
        pCfgData        += lenUserData;
        cntDecodedBytes += lenUserData;
    }

    return TRUE;
}

//----------------------------- FUNCTION -------------------------------------*
void
    BuildHexString(BYTE const* pCfgData, int cntBytes, CString& sCfgTxt)
/*>>>> 
build a hex string from <cntBytes> bytes in <pCfgData>

I   pCfgData:   ptr to special format byte
I   cntBytes:   number of bytes to display
O   sCfgTxt:    string to receive hex string

Result
  void
<<<<*/
{
    CString sNum;
    for (int i = 0; i < cntBytes; i++)
    {
        if (i == 0) {
            sNum.Format(_T("0x%02X"), *pCfgData);
        } else {
            sNum.Format(_T(", 0x%02X"), *pCfgData);
        }
        sCfgTxt += sNum;
        pCfgData++;
    }
}

//----------------------------- FUNCTION -------------------------------------*
/* static */ void
    DecodeSpecFormat(BYTE const* pCfgData, int idsInOutString, CString& sCfgTxt, int& lenIO)
/*>>>> 
decode special identifier byte

I   pCfgData:   ptr to special format byte
I   idsInOutString: resource ID of input/output string
O   sCfgTxt:    string to receive decoded text
O   lenIO:      length of inputs/outputs in special identifier byte

Result
  void
<<<<*/
{
    lenIO = ((*pCfgData & DP_IDENTIFIER_SPEC_LEN) + 1);


    // is length in word or bytes??
    if (*pCfgData & DP_IDENTIFIER_LEN)
    {
        // length is in words, so double to get bytes:
        lenIO *= 2;
    }

    CString sNum;
    sNum.Format(_T("%d"), lenIO);
    ::AfxFormatString1(sCfgTxt, idsInOutString, sNum);


    CString sTxt;
    if (*pCfgData & DP_IDENTIFIER_CONSISTENCY)
    {
        sTxt.LoadString(IDS_DP_CFG_CONSISTENCY);
        sCfgTxt += sTxt;
    }
    else
    {
        sTxt.LoadString(IDS_DP_CFG_NO_CONSISTENCY);
        sCfgTxt += sTxt;
    }

}

//----------------------------- FUNCTION -------------------------------------*
/* static */ int
    ConvertHexStringToBin(CString &sText, BYTE* pBuffer, int iMaxBufLen)
/*>>>> 
convert a hex string to binary hex bytes

I   pCfgData:   ptr to special format byte
I   idsInOutString: resource ID of input/output string
O   sCfgTxt:    string to receive decoded text
O   lenIO:      length of inputs/outputs in special identifier byte

Result
  number of hex bytes
<<<<*/
{
    LPTSTR  pszEdit = (LPTSTR)(LPCTSTR) sText;

    int iOffset = 0;
    BOOL bEnd = sText.IsEmpty();
    for (iOffset = 0; !bEnd && (iOffset < iMaxBufLen); iOffset++)
    {
        LPCTSTR  pszVal = pszEdit;
        // overread non-whitespace until end of hex field
        while (*pszEdit && !_istspace(*pszEdit))
            pszEdit++;
        
        bEnd = (*pszEdit == '\0');

        // overread whitespace to start of next hex field
        if (!bEnd) {
            *pszEdit = '\0';
            pszEdit++;
            while (*pszEdit && _istspace(*pszEdit))
                pszEdit++;
            bEnd = (*pszEdit == '\0');  // did we now reach end of string?
        }

        ULONG   ulVal = _tcstoul(pszVal, NULL, 16);
        ASSERT(ulVal <= 0xff);
        ulVal &= 0xFF;      // mask to byte
        pBuffer[iOffset] = (BYTE) ulVal;
    }
    return iOffset;
}

//----------------------------- FUNCTION -------------------------------------*
/* static */ CString
    ConvertDWORD2BitString(DWORD dwConvertVal)
/*>>>> 
	convert a DWORD to a string which shows the bits
I:	dwConvertVal is the Value whitch is to be converted

return value:
	CString with setted Bits
<<<<*/
{
	CString szBitString;
	DWORD	dwCompare = 0x80000000;

	do	{
		if ((dwConvertVal & dwCompare) > 0)	{
			szBitString += _T("1");
		}else	{
			szBitString += _T("0");
		}
		
		dwCompare >>= 1;
	} while (dwCompare != 0);
	
	int dwCharCount = szBitString.Find('1');
	if ((dwCharCount > 15) && (dwCharCount < 24)) {
		szBitString = szBitString.Right(16);
	}else if ((dwCharCount > 23) && (dwCharCount < 32) || (dwCharCount == -1)) {
		szBitString = szBitString.Right(8);
	}

	return szBitString;
}


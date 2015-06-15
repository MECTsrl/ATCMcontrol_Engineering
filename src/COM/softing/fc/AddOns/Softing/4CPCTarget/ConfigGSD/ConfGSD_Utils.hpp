
#ifndef __CONFGSD_UTILS_HPP_
#define __CONFGSD_UTILS_HPP_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   ------------------------------------------------*

//----  Defines:    ----------------------------------------------------------*
#define DP_IDENTIFIER_CONSISTENCY   0x80
#define DP_IDENTIFIER_LEN           0x40

#define DP_IDENTIFIER_IO_SPECIAL    0x00
#define DP_IDENTIFIER_IO_INP        0x10
#define DP_IDENTIFIER_IO_OUTP       0x20
#define DP_IDENTIFIER_IO_INP_OUTP   0x30
#define DP_IDENTIFIER_IO            0x30

#define DP_IDENTIFIER_DATA_LEN      0x0F
#define DP_IDENTIFIER_SPEC_LEN      0x3F

#define DP_IDENTIFIER_FREE_PLACE    0x00
#define DP_IDENTIFIER_INP_LEN       0x40
#define DP_IDENTIFIER_OUTP_LEN      0x80
#define DP_IDENTIFIER_INP_OUTP_LEN  0xC0
#define DP_IDENTIFIER_SPECIAL       0xC0

//*--- CFG MACROS -----------------------------------------------------------*
#define _DP_GET_IDENTIFIER_DATA_LEN(idf_ptr)                                 \
        (                                                                    \
            (*idf_ptr & DP_IDENTIFIER_LEN)                ?                  \
          (((*idf_ptr & DP_IDENTIFIER_DATA_LEN) + 1) * 2) :                  \
           ((*idf_ptr & DP_IDENTIFIER_DATA_LEN) + 1)                         \
        )

#define _DP_GET_IDENTIFIER_SPEC_LEN(idf_ptr)                                 \
        (                                                                    \
            (*idf_ptr & DP_IDENTIFIER_LEN)                ?                  \
          (((*idf_ptr & DP_IDENTIFIER_SPEC_LEN) + 1) * 2) :                  \
           ((*idf_ptr & DP_IDENTIFIER_SPEC_LEN) + 1)                         \
        )

#define _DP_GET_IDENTIFIER_USER_LEN(idf_ptr)                                 \
        (                                                                    \
          ( ((*idf_ptr & DP_IDENTIFIER_DATA_LEN) == 0x00) ||                 \
            ((*idf_ptr & DP_IDENTIFIER_DATA_LEN) == 0x0F) ) ?                \
             (0                                )            :                \
             (*idf_ptr & DP_IDENTIFIER_DATA_LEN)                             \
        )

//----  Typedefs:   ----------------------------------------------------------*
// operation mode selection of DP stack
typedef enum {
    offline,
    stop,
    clear,
    operate
} opModeDP;

typedef enum {
    hostFormat,             // = Intel - format
    busFormat               // = Motorola - format
} byteOrder;


// =Enumerator
typedef enum {
    standardDiag,       // Station-specific diagnostic
    unitDiag,           // unit-specific extended diagnostic
    moduleDiag,         // module-specific extended diagnostic
    channelDiag,        // channel-specific extended diagnostic
    revisionNumber      // slave-specific revision number
} DiagType;

//----  Globals:    ----------------------------------------------------------*

//----  Prototypes:    -------------------------------------------------------*
BOOL                    DecodeCfgData(CByteArray const* pCfg,
                                      CStringArray& aTexts, CUIntArray& aOffsets, CUIntArray& aLengths,
                                      int& iTotalInputs, int& iTotalOutputs, int& iTotalInAndOuts,
                                      int& iCnt);
                            // decode slave config data bytes to string

BOOL                    DecodeCfgBytes(BYTE*& pCfgData, CString& sCfgText,
                                       int& cntDecodedBytes,
                                       int& lenInputs, int& lenOutputs, int& lenReadAndWrite);
                            // decode single sequence of slave config data bytes to string

void                    BuildHexString(BYTE const* pCfgData, int cntBytes, CString& sCfgTxt);
                            // build a hex string from <cntBytes> bytes in <pCfgData>

int                     ConvertHexStringToBin(CString &sText, BYTE* pBuffer, int iMaxBufLen);
                            // convert a hex string to binary hex bytes

CString				    ConvertDWORD2BitString(DWORD dwConvertVal);
							// convert a DWORD to a string which shows the bits

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


#endif // __CONFGSD_UTILS_HPP_


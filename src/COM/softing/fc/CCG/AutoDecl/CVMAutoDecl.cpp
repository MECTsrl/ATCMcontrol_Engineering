
#include "stdafx.h"
#include "CVMAutoDecl.h"
#include "utilif.h"
#include "util.h"


#include <comdef.h>

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>


static TCHAR g_szTargetTypeName[64];
static TCHAR g_szAutodeclIniFile[2*MAX_PATH];
IO_COMP_TYP* g_pChannelDefs = NULL;

//----------------------------- HELP ROUTINES ------------------------
int GetDataType(TCHAR* pszSource);
int GetType(TCHAR* pszTypeString);
char GetTypeChar(TYPE CurrentType);
char GetPrefix(DATA_TYPE CurrentType);
//--------------------------------------------------------------------


//////////////////////////////////////////////////////////////////////
// Class CVMVarSection
//////////////////////////////////////////////////////////////////////

CVMVarSection::CVMVarSection(int iVarType,
						TCHAR* szSectName,
						int	iSeg,
						int iDataType,
						long lCount,
						int	iStringLength,
						long lAddress)
  : m_lpNext(NULL),
	m_iVarType(iVarType),
	m_iSeg(iSeg),
	m_iDataType (iDataType),
	m_lCount (lCount),
	m_iStringLength (iStringLength),
	m_lAddress (lAddress)
{
	_tcscpy( m_szSectName, szSectName);
}

CVMVarSection::~CVMVarSection()
{
}

void  CVMVarSection::Add(CVMVarSection *lpNext)
{
	if (m_lpNext)
    {
		CVMVarSection *lpTmp=m_lpNext;
		m_lpNext=lpNext;
		lpNext->SetNext(lpTmp);
    }else{
		m_lpNext=lpNext;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CVMAutoDecl
/////////////////////////////////////////////////////////////////////////////


CVMAutoDecl::CVMAutoDecl() : m_pVarSectionList(NULL)
{

}

CVMAutoDecl::~CVMAutoDecl()
{
	CVMVarSection *lpNext, *lpDelete;
	
    if(m_pVarSectionList != NULL){
        lpNext=m_pVarSectionList->GetNext();
        
		while (lpNext){      
            lpDelete=lpNext;
            lpNext=lpNext->GetNext();
            delete lpDelete;
        }

		delete m_pVarSectionList;
    } 
}




//------------------------------------------------------------------*
/**
 * Function name			CVMAutoDecl::GenerateAddress
 * Description		Generates the address string depending from the actual section,
	                the actual index and the actual subindex. If the data type is BOOL
                    than you decide how to increment the byte and the bit count through
                    the 4. parameter in the following way: 
                    Byte: +1 Bit: 0..7 use BYTES_PRO_BYTE
                    Byte: +2 Bit: 0..15 use BYTES_PRO_WORD
                    Byte: +4 Bit: 0..31 use BYTES_PRO_DOUBLE
                    Byte: +8 Bit: 0..63 use BYTES_PRO_LONG
 * @param			CVMVarSection* pCurrentVarSection
 * @param			long lIndex
 * @param			long lSubIndex
 * @param			int iByteIncrement
 * @return			bool 
 * @exception			
 * @see			
*/
bool CVMAutoDecl::GenerateAddress(CVMVarSection* pCurrentVarSection, long lIndex, long lSubIndex, int iByteIncrement)
{
    ATLASSERT(pCurrentVarSection && (lIndex >= 0) && (lSubIndex>= -1));

    DATA_TYPE CurrentDataType = (DATA_TYPE)pCurrentVarSection->GetDataType();


    //##d-1395 beg
    if((CurrentDataType==DATA_TYPE_BOOL && lSubIndex<0) ||  
       (CurrentDataType!=DATA_TYPE_BOOL && lSubIndex>=0) 
      )
    {
        return false; //_[bit off] is required if BOOL, 
                      //if not BOOL _[bit off] not allowed at all
    }
    //##d-1395 end


    char chPrefix = GetPrefix(CurrentDataType);
    int iCurrentSegment = pCurrentVarSection->GetSegment();
    long lCurrentOffset = pCurrentVarSection->GetAddress();
    int iStringLength = pCurrentVarSection->GetStringLength();

    SetDataTypeString(CurrentDataType, iStringLength);
    
    TYPE CurrentType = (TYPE)pCurrentVarSection->GetVarType();
    char chType = GetTypeChar(CurrentType);

    switch (CurrentDataType)
    {
    case DATA_TYPE_SINT:
    case DATA_TYPE_BYTE:
        lIndex =  lCurrentOffset + lIndex * BYTES_PRO_BYTE;
        _stprintf( m_szIECAddress, ADDRESS_FORMAT_STRING, 
				   chType, chPrefix, iCurrentSegment,lIndex);
		break;
    case DATA_TYPE_INT:
    case DATA_TYPE_WORD:
        lIndex =  lCurrentOffset + lIndex * BYTES_PRO_WORD;
        _stprintf( m_szIECAddress, ADDRESS_FORMAT_STRING, 
				   chType, chPrefix, iCurrentSegment,lIndex);
        break;
    case DATA_TYPE_DINT:
    case DATA_TYPE_DWORD:
	case DATA_TYPE_REAL:	// d-227 01.06.05 SIS
        lIndex =  lCurrentOffset + lIndex * BYTES_PRO_DOUBLE;
        _stprintf( m_szIECAddress, ADDRESS_FORMAT_STRING, 
				   chType, chPrefix, iCurrentSegment,lIndex);
        break;
    case DATA_TYPE_LWORD:
    case DATA_TYPE_LINT:
    case DATA_TYPE_LREAL:
        lIndex = lCurrentOffset + lIndex * BYTES_PRO_LONG;
        _stprintf( m_szIECAddress, ADDRESS_FORMAT_STRING, 
				   chType, chPrefix, iCurrentSegment,lIndex);
        break;
    case DATA_TYPE_BOOL://here per define, many cases are possible
                        //default is 0..15 bit-count
        switch(iByteIncrement)
        {
        case BYTES_PRO_WORD:
            lIndex = lCurrentOffset + lIndex * BYTES_PRO_WORD;
            ATLASSERT(lSubIndex >= 0);
            if(lSubIndex > (BIT_PRO_BYTE * BYTES_PRO_WORD) -1)//0..15
            {
                return false;
            }            
            if(lSubIndex > BIT_PRO_BYTE -1)
            {// if lSubIndex > 7 than increment the byte count 
             // and put the lSubIndex 7 digits back
                lIndex = lIndex + (int)(lSubIndex / BIT_PRO_BYTE);
                lSubIndex = (int)(lSubIndex % BIT_PRO_BYTE);
            }
            _stprintf( m_szIECAddress, ADDRESS_FORMAT_STRING_BOOL, 
				       chType, chPrefix, iCurrentSegment,lIndex, lSubIndex);
            break;
        case BYTES_PRO_BYTE:
            lIndex = lCurrentOffset + lIndex * BYTES_PRO_BYTE;
            ATLASSERT(lSubIndex >= 0);
            if(lSubIndex > (BIT_PRO_BYTE * BYTES_PRO_BYTE) -1)//0..7
            {
                return false;
            }
            if(lSubIndex > BIT_PRO_BYTE -1)
            {// if lSubIndex > 7 than increment the byte count 
             // and put the lSubIndex 7 digits back
                lIndex = lIndex + (int)(lSubIndex / BIT_PRO_BYTE);
                lSubIndex = (int)(lSubIndex % BIT_PRO_BYTE);  
            }
            _stprintf( m_szIECAddress, ADDRESS_FORMAT_STRING_BOOL, 
				       chType, chPrefix, iCurrentSegment,lIndex, lSubIndex);
            break;
        case BYTES_PRO_DOUBLE:
            lIndex = lCurrentOffset + lIndex * BYTES_PRO_DOUBLE;
            ATLASSERT(lSubIndex >= 0);
            if(lSubIndex > (BIT_PRO_BYTE * BYTES_PRO_DOUBLE) -1)//0..31
            {
                return false;
            }
            if(lSubIndex > BIT_PRO_BYTE -1)
            {// if lSubIndex > 7 than increment the byte count 
             // and put the lSubIndex 7 digits back
                lIndex = lIndex + (int)(lSubIndex / BIT_PRO_BYTE);
                lSubIndex = (int)(lSubIndex % BIT_PRO_BYTE);     
            }
            _stprintf( m_szIECAddress, ADDRESS_FORMAT_STRING_BOOL, 
				       chType, chPrefix, iCurrentSegment,lIndex, lSubIndex);
            break;
        case BYTES_PRO_LONG:
            lIndex = lCurrentOffset + lIndex * BYTES_PRO_LONG;
            ATLASSERT(lSubIndex >= 0);
            if(lSubIndex > (BIT_PRO_BYTE * BYTES_PRO_LONG) -1)//0..63
            {
                return false;
            }
            if(lSubIndex > BIT_PRO_BYTE -1)
            {// if lSubIndex > 7 than increment the byte count 
             // and put the lSubIndex 7 digits back  
                lIndex = lIndex + (int)(lSubIndex / BIT_PRO_BYTE);
                lSubIndex = (int)(lSubIndex % BIT_PRO_BYTE);
            }
            _stprintf( m_szIECAddress, ADDRESS_FORMAT_STRING_BOOL, 
				       chType, chPrefix, iCurrentSegment,lIndex, lSubIndex);
            break;
        default:
            break;
        }
        break;
    case DATA_TYPE_WSTRING:
    case DATA_TYPE_STRING:
        lIndex = lCurrentOffset + lIndex * (2 + iStringLength);
		_stprintf( m_szIECAddress, ADDRESS_FORMAT_STRING, 
				   chType, chPrefix, iCurrentSegment,lIndex);

        break;
    default:
        ATLASSERT(FALSE); //must be a mistake
        return false;
    }
    return true;
}


//------------------------------------------------------------------*
/**
 * Function name			GetTypeChar
 * Description		depending on the index in the table, returns the right char	
 * @param			TYPE CurrentType
 * @return			char: I, Q or M  
 * @exception			
 * @see			
*/
char GetTypeChar(TYPE CurrentType)
{
    int iEntries  = sizeof(TypeTbl)/sizeof(TYPE_DESC);
    ATLASSERT(CurrentType < iEntries);
 
    return TypeTbl[CurrentType].chCapitalize;
}


//------------------------------------------------------------------*
/**
 * Function name			GetPrefix
 * Description		depending on the index in the table, returns the right char	
 * @param			DATA_TYPE CurrentType
 * @return			char: X, W, B, D, L or S 
 * @exception			
 * @see			
*/
char GetPrefix(DATA_TYPE CurrentType)
{
    int iEntries  = sizeof(DataTypeTbl)/sizeof(DATATYPE_DESC);
    ATLASSERT(CurrentType < iEntries);

    return DataTypeTbl[CurrentType].chPrefix;
}



//------------------------------------------------------------------*
/**
 * Function name			SetDataTypeString
 * Description		Sets the member variable m_szIECDataType depending on the actual
	                DATA_TYPE
 * @param			DATA_TYPE CurrentType
 * @return			int 
 * @exception			
 * @see			
*/
int CVMAutoDecl::SetDataTypeString(DATA_TYPE CurrentType, int iStringLength)
{
    int iEntries  = sizeof(DataTypeTbl)/sizeof(DATATYPE_DESC);
    ATLASSERT(CurrentType < iEntries);

    _stprintf(m_szIECDataType, iStringLength>0 ? _T("%s[%d]") : _T("%s"),
              DataTypeTbl[CurrentType].pszDATAString, iStringLength);

    return 1;
}

//------------------------------------------------------------------*
/**
 * Function name			CVMAutoDecl::IsVMVar
 * Description		Tets if the variable is in the Section list. For that purpuse, it splits 
	                the variable name into a variable name and an index 
                    (example: VarName100 => VarName + 100). The Name is used to lookup the Section
                    list for the section with the right name. If there is one with that given name
                    than the direct adress for that variable is generated depending on it's type, 
                    index and the segment of that section.
 * @param			TCHAR* szVarName: variable name 
 * @return			bool: true if the variable was found else false. 
 * @exception			
 * @see			    CVMVarSection
*/
bool CVMAutoDecl::IsVMVar(TCHAR* szVarName)
{
	int i, iPos;
	long lIndex, lSubIndex;
	int iLength = _tcslen(szVarName);
	iPos = -1;
	TCHAR* szTemp = (TCHAR* )alloca ((iLength +1) * sizeof(TCHAR));
	
    for(i =0; i < iLength; i++)
    {//extract var name,
		if( IsDigit(szVarName[i]) )
        {
			iPos = i;
			break;
		}
        else
        {
			szTemp[i] = szVarName[i];
		}
	}

	if((iPos > 0) && (iPos < iLength))
    {
		szTemp[iPos] = 0;
		CVMVarSection* pCurrentVarSection = m_pVarSectionList;
		bool bFound = false;
        //look if there is a section with the given name
		while(pCurrentVarSection)
		{
			if(_tcsicmp(szTemp, pCurrentVarSection->GetName()) == 0 )
            {
				bFound = true;
				break;
			}
			pCurrentVarSection = pCurrentVarSection->GetNext();
		}

		szVarName += iPos;
		iLength -= iPos;

		iPos = -1;
		for(i =0; i < iLength; i++)
        {//extract the index
			if( IsDigit(szVarName[i]) )
            {
				szTemp[i] = szVarName[i];
				iPos = i+1;
			}
            else
            {
				break;
			}
		}
		
		if(bFound && (iPos > 0) &&(iPos <= iLength) )
        {
			szTemp[iPos] =0;
			lIndex = _ttol(szTemp);
			if( (lIndex < 0) || lIndex >= pCurrentVarSection->GetCount())
            {
				return false;
            }
			m_iVarType = pCurrentVarSection->GetVarType();
			szVarName += iPos;
			iLength -= iPos;

            if(szVarName[0] == UNDERSCORE)
            {//bool
			    szVarName++;
			    iLength --;
			    iPos = -1;
			    for(i =0; i < iLength; i++)
                {//extract the (bit)sub-index for boolean variable
			        if( IsDigit(szVarName[i]) )
                    {
				        szTemp[i] = szVarName[i];
					    iPos = i+1;
				    }
                    else
                    {
				        break;
				    }
			    }
                if(iPos > 0)
                {
			        iLength -= iPos;
				    if(iLength > 0)
                    {
                        /*ATLASSERT(FALSE);*/ //HE 10.09.01 : This is an normal errorcase, user has given wrong Iden !!!
				        return false;
                    }

				    szTemp[iPos] =0;
				    lSubIndex = _ttol(szTemp);
                }
            }
            else
            {//not a bool
                if (iLength != 0)//HE 10.09.01 : errorcase must be checked, no additional characters allowed !
					return false;
				lSubIndex = -1;
            }

            //Generate the Adress depending from the DATAType
            //If the data type is BOOL, here you may use a forth parameter to decide how 
            //the bit and byte incrementation will accur:
            //Byte: +1 Bit: 0..7 use BYTES_PRO_BYTE
            //Byte: +2 Bit: 0..15 use BYTES_PRO_WORD
            //Byte: +4 Bit: 0..31 use BYTES_PRO_DOUBLE
            //Byte: +8 Bit: 0..63 use BYTES_PRO_LONG
            bool bRes = GenerateAddress(pCurrentVarSection, lIndex, lSubIndex, BYTES_PRO_WORD);
            return bRes;
		}
	}
	return false;
}


//------------------------------------------------------------------*
/**
 * Function name			CVMAutoDecl::InitList
 * Description		Initialises the section list	
 * @param			TCHAR* szConfigFileName
 * @return			bool 
 * @exception			
 * @see			
*/
bool CVMAutoDecl::InitList(TCHAR* szConfigFileName)
{
	FILE* pInFile;
    HRESULT hr;
	int iType, iDataType, iSegment, iStringLen;
	long lCount,lAddress;
    int iPos = -1;
    CVMVarSection* pCurrentVarSection;
    TCHAR pszTemp[MAX_LENGTH_OF_LINE];
    TCHAR pszVarName[MAX_LENGTH_OF_VAR_NAME];
    TCHAR pszInbuf0[MAX_LENGTH_OF_LINE];

	if( (pInFile  = _tfopen( szConfigFileName, _T("rt") )) == NULL )
    {
        ATLASSERT(!"CVMAutoDecl::InitList: cannot open config file"); 
		return false;
    }

	while( _fgetts( pszInbuf0, MAX_LENGTH_OF_LINE, pInFile ) != NULL  )
	{
		TCHAR* pszBuffLine = pszInbuf0;
		iType = -1;
		if( (iPos = NextToken(pszBuffLine, pszTemp) ) > 0 )
        { // type;
            iType = GetType(pszTemp);

			if(iType == -1)
            {
				break;
            }

			pszBuffLine += iPos;
		}
        else
        {
			continue;
		}

		if( (iPos = NextToken(pszBuffLine, pszVarName) ) > 0 ) 
        {//name of variable;
			pszBuffLine += iPos;
            hr = CheckIdentifier(pszVarName);
            if(hr != S_OK)
            {//react here
                break;
            }
        }
		else
        {
			break;
        }

		if( (iPos = NextToken(pszBuffLine, pszTemp) ) > 0 )
        { //segment
			iSegment =  _ttoi(pszTemp );
			if(iSegment >=0 )
            {
				pszBuffLine += iPos;
			}
            else
            {
				break;
			}
		}
        else
        {
			break;
		}

		iDataType = -1;
		if( (iPos = NextToken(pszBuffLine, pszTemp) ) > 0 )
        { // data type;
            iDataType = GetDataType(pszTemp);

			if(iDataType == -1)
            {
				break;
            }

			pszBuffLine += iPos;
		}
        else
        {
			break;
		}

		if( (iPos = NextToken(pszBuffLine, pszTemp) ) > 0 )
        { //count
			lCount =  _ttol(pszTemp );
			if(lCount < 0 )
				break;
			pszBuffLine += iPos;
		}
        else
        {
			break;
		}

        iStringLen = 0;
		if(iDataType == DATA_TYPE_STRING || iDataType == DATA_TYPE_WSTRING)
        {
			if( (iPos = NextToken(pszBuffLine, pszTemp) ) > 0 )
            { //string length
				iStringLen =  _ttoi(pszTemp );	
				if(iStringLen <=0 )
					break;
				pszBuffLine += iPos;
			}
            else
            {
				break;
			}
		}

		if( (iPos = NextToken(pszBuffLine, pszTemp, NEWLINE)) > 0 )
        { //address
			lAddress = _ttol(pszTemp );
		}
        else
        {
			if(pszTemp)
            {
				lAddress = _ttol(pszTemp );
			}
            else
            {
				break;
			}
		}

		if(lAddress < 0 )
			break;

		CVMVarSection* pVarSection = new CVMVarSection(iType, pszVarName, 
			iSegment, iDataType, lCount,iStringLen,lAddress);
			
		if(m_pVarSectionList == NULL){
			m_pVarSectionList = pVarSection;
			pCurrentVarSection = pVarSection;
		}else{
			pCurrentVarSection->Add(pVarSection);
			pCurrentVarSection = pVarSection;
		}
	}

	fclose( pInFile );

	return true;
}


//------------------------------------------------------------------*
/**
 * Function name			GetDataType
 * Description		Looks up the data type table for the corresponding 
	                data type.
 * @param			TCHAR* pszDataTypeString: DATAType to look for
 * @return			int: index of the data type in the table else -1 
 * @exception			
 * @see			
*/
int GetDataType(TCHAR* pszDataTypeString)
{
    int iEntries  = sizeof(DataTypeTbl)/sizeof(DATATYPE_DESC);
    for(int i = 0;i<iEntries;i++) 
    {
		if(!_tcsnicmp(pszDataTypeString, DataTypeTbl[i].pszDATAString, DataTypeTbl[i].iSize)) 
        {
			return DataTypeTbl[i].DataType;
		}
    }
    ATLASSERT(FALSE); //must be a mistake
	return -1;
}


//------------------------------------------------------------------*
/**
 * Function name			GetType
 * Description		Gets the type depending from type string	
 * @param			TCHAR* pszTypeString
 * @return			int 
 * @exception			
 * @see			    TYPE_DESC, TypeTbl
*/
int GetType(TCHAR* pszTypeString)
{
    int iEntries  = sizeof(TypeTbl)/sizeof(TYPE_DESC);
    for(int i = 0;i<iEntries;i++) 
    {
		if(pszTypeString[0] ==  TypeTbl[i].chCapitalize 
           || pszTypeString[0] == TypeTbl[i].chNotCapitalize) 
        {
			return TypeTbl[i].Type;
		}
    }
    ATLASSERT(FALSE); //must be a mistake
	return -1;
}


//------------------------------------------------------------------*
/**
 * Function name			CheckIdentifier
 * Description		Checks if the identifier is IEC conform and the last
	                character is not a digit
 * @param			TCHAR* pszIdentifier
 * @return			HRESULT 
 * @exception			
 * @see			
*/
HRESULT CVMAutoDecl::CheckIdentifier(TCHAR* pszIdentifier)
{
    //check if the last letter is a digit
    int iLength = _tcslen(pszIdentifier);
    if(IsDigit(pszIdentifier[iLength -1 ]))
    {//the last character in the name may not be a digit
        return S_FALSE;
    }
    //check if IEC conform
    HRESULT hr = UTIL_CheckIdentifier(pszIdentifier);
    switch(hr)
    {//here react on a non IEC comform variable name 
    case S_OK:
        return S_OK;
        break;
    case E_UTIL_ID_SYNTAX :
        return S_FALSE;
        break;
    case E_UTIL_ID_IS_KEYWORD :
        return S_FALSE;
        break;
    case E_UTIL_ID_TOO_LONG :
        return S_FALSE;
        break;
    default:
        return hr;
        break;
    }
}



//------------------------------------------------------------------*
/**
 * Function name			CVMAutoDecl::NextToken
 * Description		looks up a line for cSep (COMMA or NEWLINE) where COMMA
	                is the default and gets the token content (variable name, type ...)
 * @param			TCHAR* pszSource: line to be parsed (or the rest of it)
 * @param			TCHAR* lpszToken: the result of the parsing
 * @param			TCHAR cSep: character to look for
 * @return			int 
 * @exception			
 * @see			
*/
int CVMAutoDecl::NextToken(TCHAR* pszSource, TCHAR* lpszToken, TCHAR cSep)
{
	TCHAR *pszDest;
	pszDest = _tcschr( pszSource, cSep );
	if(pszDest == NULL)
		return -1;

	int iPos = pszDest - pszSource + 1;
	if(iPos == 1)
		return -1;
	
	int iIndex =0;
	for(int i=0; i < iPos -1; i++)
    {
		if(pszSource[i] != SPACE)
        {
			lpszToken[iIndex] = pszSource[i];
			iIndex ++;
		}
	}
		
	lpszToken[iIndex] = 0;

	return iPos;
}

//////////////////////////////////////////////////////////////
// Interface ICG_AutoDecl
//////////////////////////////////////////////////////////////
STDMETHODIMP CVMAutoDecl::init(       
    BSTR  sTargetTypeName, // target type name as used by engineering
    BSTR  sConfigFile      //file and fullpath of the variable description file.
)
{
	USES_CONVERSION;

	TCHAR* strFile = W2T(sConfigFile);

    _tcscpy(g_szTargetTypeName, W2T(sTargetTypeName));
    _tcscpy(g_szAutodeclIniFile, strFile);

    if(m_pVarSectionList)
        delete(m_pVarSectionList);

    if(!InitList(strFile))
		return (E_UNEXPECTED);
	

    return S_OK;
}



STDMETHODIMP CVMAutoDecl::getAddressInfo(
    BSTR   sID, 
    BSTR*  psIECAddr, 
    BSTR*  psIECType,
    BOOL*  pbReadOnly
)
{
	USES_CONVERSION;

	TCHAR* strId = W2T(sID);
	int iLength = _tcslen(strId);

	if(iLength <2) //at least two charactors
		return (S_FALSE);


	if (psIECAddr == NULL)
        return (E_POINTER);
  
	if (psIECType == NULL)
        return (E_POINTER);

	if (pbReadOnly == NULL)
        return (E_POINTER);

	if(!m_pVarSectionList){
        ATLASSERT(!"CVMAutoDecl::getAddressInfo: init was not called"); 
        return (E_FAIL);
	}

	int i;
	for(i=0; i < iLength; i++) //trim left
		if(strId[i] != SPACE)
			break;

	strId += i;

	iLength = _tcslen(strId);
	if(iLength <2) //at least two charactors
		return (S_FALSE);

	for(i=iLength-1; i > 0; i--) // trim right
		if(strId[i] != SPACE)
			break;

	if(i <1) //at least two charactors
		return (S_FALSE);

	if(i < (iLength -1))
		strId[i+1] = 0;

	if ( !IsVMVar(strId))
		return (S_FALSE);

	if(m_iVarType == TYPE_INPUT)
		*pbReadOnly = TRUE; 
	else
		*pbReadOnly = FALSE;

	_bstr_t bstr = m_szIECAddress;
	*psIECAddr = bstr.copy();

	bstr = m_szIECDataType;
	*psIECType = bstr.copy();

	return (S_OK);
}




STDMETHODIMP CVMAutoDecl::getOnlStringFromAddr( 
    BSTR sIECAddr,
    BSTR sIECType,
    BSTR* psOnlString
)
{
	USES_CONVERSION;

    BOOL            bOk;
    LU_CDiag        diag;
    LU_IECADDRESS   addr;
    const TCHAR*    pszEnd;
    static bool     b_compilerKadParsed = false;

    //sIECAddr = "%IW0.168"
    //           "%QX4.12.5  

    //check pure systax of address:
    bOk = LU_ParseIECDirectAddress( //return FALSE if any syntax or limit error
              &diag,                //detail diag
              W2T(sIECAddr),        //where to read from
              &addr,                //parse result 
              &pszEnd               //NULL or receives the char that stopped the scan
          );

    if(!bOk)
    {
        //diag should be already set...
        goto ErrExit;
    }
    if(*pszEnd)
    {
        diag.set(_T("syntax error"));
        goto ErrExit;
    }

    //systax ok now check more:
    if(addr.nMods>0)
    {
        diag.set(_T("not supported: !<modifier> syntax"));
        goto ErrExit;
    }

    //check number of fields:
    if(addr.cSize=='X')
    {
        if(addr.nFields!=3)
        {
            diag.set(_T("'%%%c'-type address needs three hierarchy fields"), addr.cLoc);
            goto ErrExit;
        }
    }
    else if(addr.nFields!=2)
    {
        diag.set(_T("'%%%c'-type address needs two hierarchy fields"), addr.cLoc);
        goto ErrExit;
    }
    //pure address syntax is ok now...




    //check correspondense of data type to address size prefix:
    BL_IEC_TYP iecTyp;
    long       strLength;
    if(!ParseDataType(sIECType, &iecTyp, &strLength))
    {
        diag.set(_T("bad data typename '%s'"), W2T(sIECType));
        goto ErrExit;
    }

    if((iecTyp==BL_BOOL && addr.cSize != 'X') || (iecTyp==BL_STRING && addr.cSize != 'S'))
    {
        diag.set(_T("data type '%s' does not match address size prefix '%c'"), W2T(sIECType), addr.cSize);
        goto ErrExit;
    }
    else if(iecTyp==BL_WSTRING || iecTyp==BL_TIME || iecTyp==BL_DT || iecTyp==BL_DATE || iecTyp==BL_TOD)
    {
        diag.set(_T("data type '%s' not supported for use with direct address"), W2T(sIECType));
        goto ErrExit;
    }
    //else TODO: ignore the other restrictions, allow a DWORD to be at %IB and so on..




    //if not already loaded load the 4cvm compiler KAD now to get channel definitions:
    if(!b_compilerKadParsed)
    {
        b_compilerKadParsed = true;
        if(!Parse4CVMKADForChannels(g_szTargetTypeName, g_szAutodeclIniFile, &g_pChannelDefs))
        {
            diag.set(_T("failed to load compiler kad file"));
            goto ErrExit;
        }
    }



    //lookup the channel
    unsigned long ulChannelOffset, ulChannelSize;
    if(!LookUpChannelDef(g_pChannelDefs, addr.cLoc, addr.ulFields[0], &ulChannelOffset, &ulChannelSize))
    {
        diag.set(_T("channel %d not defined"), addr.ulFields[0]); 
        goto ErrExit;
    }

    //L"__system.direct.W.2.2"
    TCHAR szOnlString[MAX_PATH];
    if(!BuildOnlString(szOnlString, &addr, ulChannelOffset, iecTyp, strLength))//TODO check also ulChannelSize??
    {
        diag.set(_T("internal error: 'BuildOnlString' failed")); 
        goto ErrExit;
    }

    *psOnlString = SysAllocString(T2W(szOnlString));
    //TODO *psError = NULL;
    return S_OK;


ErrExit:
    *psOnlString = SysAllocString(L"");
    //TODO *psError = SysAllocString(T2W(diag.szDetails));

    //at least do a debug print on error, errors should not happen due to the fact
    //that this function is used currently only from watch backend, an error is most 
    //likely an internal software bug anyway:
    OutputDebugStringW(L"ERROR: getOnlStringFromAddr failed for '");
    OutputDebugStringW(sIECAddr);
    OutputDebugStringW(L"' type=");
    OutputDebugStringW(sIECType);
    OutputDebugStringW(L", reason: ");
    OutputDebugString(diag.szDetails);
    OutputDebugStringW(L"\n");
    return S_OK;
}





























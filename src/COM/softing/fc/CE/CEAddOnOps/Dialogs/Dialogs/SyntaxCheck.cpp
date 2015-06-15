
#include "stdafx.h"
#include "SyntaxCheck.h"

#include "baselib.h"
#include "BaseLibCom.h"
#include "BaseLibCom_i.c"

CString SyntaxError = _T("Syntax error");
CString RangeError = _T("Invalid range");
CString FormatError = _T("Invalid format");
CString ArgumentError = _T("Invalid argument");
CString FunctionError = _T("Function not implemented");
CString BufferError = _T("Buffer to small");
CString InternalError = _T("Internal error");
CString PrecisitionError = _T("Loss of precisition");
CString RadixError = _T("Invalid radix");

const TCHAR * const Delimiter = _T("");

const TCHAR * const Keywords[] = 
{
    _T("#IMPORT"),

    _T("ACTION"),
    _T("AND"),
    _T("ANY"),
    _T("ANY_BIT"),
    _T("ANY_DATE"),
    _T("ANY_DERIVED"),
    _T("ANY_ELEMENTARY"),
    _T("ANY_INT"),
    _T("ANY_NUM"),
    _T("ANY_MAGNITUDE"),
    _T("ANY_REAL"),
    _T("ANY_STRING"),
    _T("ARRAY"),
    _T("AT"),

    _T("BOOL"),
    _T("BY"),
    _T("BYTE"),

    _T("CASE"),
	_T("CONFIGURATION"),
    _T("CONST"),
    _T("CONSTANT"),

    _T("DATE"),
    _T("DATE_AND_TIME"),
    _T("DINT"),
    _T("DO"),
    _T("DT"),
    _T("DWORD"),

    _T("ELSE"),
    _T("ELSIF"),
    _T("EN"),
    _T("END_ACTION"),
    _T("END_CASE"),
    _T("END_CONFIGURATION"),
    _T("END_CONST"),
    _T("END_FUNCTION"),
    _T("END_FUNCTION_BLOCK"),
    _T("END_FOR"),
    _T("END_IF"),
    _T("END_NATIVE"),
    _T("END_PROGRAM"),
    _T("END_REPEAT"),
    _T("END_RESOURCE"),
    _T("END_STEP"),
    _T("END_STRUCT"),
    _T("END_SYNCHRONIZED"),
    _T("END_TRANSITION"),
    _T("END_TYPE"),
    _T("END_VAR"),
    _T("END_WHILE"),
    _T("ENO"),
    _T("EXIT"),

    _T("F_EDGE"),
    _T("FALSE"),
    _T("FOR"),
    _T("FROM"),
    _T("FUNCTION"),
    _T("FUNCTION_BLOCK"),

    _T("GOTO"),

    _T("IF"),
    _T("INITIAL_STEP"),
    _T("INT"),

    _T("LINT"),
    _T("LREAL"),
    _T("LWORD"),

    _T("MOD"),

    _T("NATIVE"),
    _T("NON_RETAIN"),
    _T("NOT"),

    _T("ON"),
    _T("OF"),
    _T("OR"),

    _T("PROGRAM"),
	
    _T("QVT_BOOL"),
    _T("QVT_BYTE"),
    _T("QVT_DINT"),
    _T("QVT_DWORD"),
    _T("QVT_LINT"),
    _T("QVT_LREAL"),
    _T("QVT_LWORD"),
    _T("QVT_REAL"),
    _T("QVT_STRING"),
    _T("QVT_WORD"),
    _T("QVT_WSTRING"),

    _T("R_EDGE"),
    _T("REAL"),
    _T("READ_ONLY"),
    _T("READ_WRITE"),
    _T("REPEAT"),
    _T("RESOURCE"),
    _T("RETAIN"),
    _T("RETURN"),

    _T("SINT"),
    _T("STEP"),
    _T("STRING"),
    _T("STRUCT"),
    _T("SYNCHRONIZED"),

    _T("TASK"),
    _T("THEN"),
    _T("TIME"),
    _T("TIME_OF_DAY"),
    _T("TO"),
    _T("TOD"),
    _T("TRANSITION"),
    _T("TRUE"),
    _T("TYPE"),

    _T("UNTIL"),
    _T("UDINT"),
    _T("UINT"),
    _T("ULINT"),
    _T("USINT"),

    _T("VAR"),
    _T("VAR_ACCESS"),
    _T("VAR_CONFIG"),
    _T("VAR_EXTERNAL"),
    _T("VAR_GLOBAL"),
    _T("VAR_INPUT"),
    _T("VAR_IN_EXT"),
    _T("VAR_IN_OUT"),
    _T("VAR_NATIVE"),
    _T("VAR_OUTPUT"),
    _T("VAR_TEMP"),

    _T("WHILE"),
    _T("WITH"),
    _T("WORD"),
    _T("WSTRING"),

    _T("XOR"),

    Delimiter
};

//------------------------------------------------------------------*
/**
 * Function name    IsKeyword
 * Description      checks whether String is an st keyword
 * @param           CString String
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL IsKeyword(CString String)
{
	int i = 0;
    while (_tcscmp(Keywords[i], Delimiter) != 0)
    {
        if (String.CompareNoCase(Keywords[i]) == 0)
        {
            return TRUE;
        }
        i++;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * Function name    IsIdentifier
 * Description      checks whether String is an st identifier
 * @param           CString String
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL IsIdentifier(CString String)
{
    if (String.IsEmpty())                       // 1
    {
        return FALSE;
    }

	if (IsKeyword(String))
	{
		return FALSE;
	}

    if (String.Find(_T("__")) != -1)            // 2
    {
        // "__" is not allowed within an identifier. 
        return FALSE;
    }

    int StringLength = String.GetLength();

	ASSERT(StringLength > 0);                   // owing to 1

    const int StartIndex = 0;
    _TCHAR c = String.GetAt(StartIndex);
    
    if (!_istalpha(c) &&
        !((c == _T('_')) && (StringLength > 1)))
    {
        // First char is not a-z or A-Z and
        // first char is not    '_'-and-followed-by-an-other-char.
        // ('_' is no valid identifier.
        // String.GetAt(StartIndex + 1) does not equal '_' due to 2.)
        return FALSE;                           
    }

    for (int i = StartIndex + 1; i < StringLength; i++)
    {
        c = String.GetAt(i);
        if ((!_istalnum(c)) && (c != _T('_')))
        {
            // String contains a char which is not a-z, A-Z, 0-9, or '_',                               
            return FALSE;                       
        }
    }

	return TRUE;
}

//------------------------------------------------------------------*
/**
 * Function name    ExtractBase
 * Description      extracts the base from Literal
 * @param           CString &Literal
 * @return          CString
 * @exception       -
 * @see             -
*/
/*
CString ExtractBase(CString &Literal)
{
    CString Base;
    int CrossPosition = Literal.Find(_T("#"));

    if (CrossPosition != -1)
    {
        // Get base and delete base specification.
        Base = Literal.Left(CrossPosition);
        Literal.Delete(0, CrossPosition + 1);
    }
    else
    {
        Base = _T("10");
    }
    return Base;
}
*/

//------------------------------------------------------------------*
/**
 * Function name    CheckForBase
 * Description      checks whether Literal is a literal for base Base
 * @param           int Base
 * @param           CString MaxLiteral
 * @param           CString Literal
 * @param           CString &ErrorMessage
 * @return          BOOL
 * @exception       -
 * @see             -
*/
/*
BOOL CheckForBase(int Base, CString MaxLiteral, CString Literal, CString &ErrorMessage)
{
    ASSERT((Base == 2) || (Base == 8) || (Base == 10) ||(Base == 16));
    CString Digits;
    if (Base == 2)
    {
        Digits = _T("01");
    }
    if (Base == 8)
    {
        Digits = _T("01234567");
    }
    if (Base == 10)
    {
        Digits = _T("0123456789");
    }
    if (Base == 16)
    {
        Digits = _T("0123456789aAbBcCdDeEfF");
    }

    if (Literal.GetLength() == 0)
    {
        ErrorMessage = SyntaxError;
        return false;
    }

    // Delete preceeding 0s.
    while ((Literal.GetAt(0) == _T('0')) && (Literal.GetLength() > 1))
    {
        Literal.Delete(0, 1);
    }

    while (Literal.GetLength() < MaxLiteral.GetLength())
    {
        Literal.Insert(0, _T('0'));
    }


    if (Literal.SpanIncluding(Digits).GetLength() != Literal.GetLength())  
    {
        ErrorMessage = SyntaxError;
        return FALSE;
    }
    else 
    {
        if (Literal.GetLength() > MaxLiteral.GetLength())
        {
            ErrorMessage = RangeError;
            return FALSE;
        }

        if (Literal.CompareNoCase(MaxLiteral) <= 0)
        {
            return TRUE;
        }
        else
        {
            ErrorMessage = RangeError;
            return FALSE;
        }
    }
}
*/

//------------------------------------------------------------------*
/**
 * Function name    CheckAnyBit
 * Description      checks whether Literal is an st any_bit literal
 * @param           CString Literal
 * @param           CString Max2
 * @param           CString Max8
 * @param           CString Max10
 * @param           CString Max16
 * @param           CString &ErrorMessage
 * @return          BOOL
 * @exception       -
 * @see             -
*/
/*
BOOL CheckAnyBit(CString Literal, 
                 CString Max2, CString Max8, CString Max10, CString Max16, 
                 CString &ErrorMessage)
{
    CString Base = ExtractBase(Literal);
    if (Base == _T("2"))
    {
        return CheckForBase(2, Max2, Literal, ErrorMessage);
    }
    if (Base == _T("8"))
    {
        return CheckForBase(8, Max8, Literal, ErrorMessage);
    }
    if (Base == _T("16"))
    {
        return CheckForBase(16, Max16, Literal, ErrorMessage);
    }
    if (Base == _T("10"))
    {
        if (Literal.GetAt(0) == _T('+'))
        {
            Literal.Delete(0, 1);
        }
        return CheckForBase(10, Max10, Literal, ErrorMessage);
    }

    // wrong base
    ErrorMessage = SyntaxError;
    return FALSE;
}
*/

//------------------------------------------------------------------*
/**
 * Function name    CheckAnyBit
 * Description      checks whether Literal is an st bool literal
 * @param           CString Literal
 * @param           CString &ErrorMessage
 * @return          BOOL
 * @exception       -
 * @see             -
*/
/*
BOOL CheckBoolLiteral(CString Literal, CString &ErrorMessage)
{
    if ((Literal.CompareNoCase(_T("TRUE")) == 0) ||
           (Literal.Compare(_T("1")) == 0) ||
           (Literal.CompareNoCase(_T("FALSE")) == 0) ||
           (Literal.Compare(_T("0")) == 0))
    {
        return TRUE;
    }
    else
    {
        ErrorMessage = SyntaxError;
        return FALSE;
    }
}
*/

//------------------------------------------------------------------*
/**
 * Function name    CheckByteLiteral
 * Description      checks whether Literal is an st byte literal
 * @param           CString Literal
 * @param           CString &ErrorMessage
 * @return          BOOL
 * @exception       -
 * @see             -
*/
/*
BOOL CheckByteLiteral(CString Literal, CString &ErrorMessage)
{
    return CheckAnyBit(Literal, 
                       _T("11111111"),
                       _T("377"),
                       _T("255"),
                       _T("FF"),
                       ErrorMessage);
}
*/

//------------------------------------------------------------------*
/**
 * Function name    CheckWordLiteral
 * Description      checks whether Literal is an st word literal
 * @param           CString Literal
 * @param           CString &ErrorMessage
 * @return          BOOL
 * @exception       -
 * @see             -
*/
/*
BOOL CheckWordLiteral(CString Literal, CString &ErrorMessage)
{
    return CheckAnyBit(Literal, 
                       _T("1111111111111111"),
                       _T("177777"),
                       _T("65535"),
                       _T("FFFF"),
                       ErrorMessage);
}
*/

//------------------------------------------------------------------*
/**
 * Function name    CheckDwordLiteral
 * Description      checks whether Literal is an st dword literal
 * @param           CString Literal
 * @param           CString &ErrorMessage
 * @return          BOOL
 * @exception       -
 * @see             -
*/
/*
BOOL CheckDwordLiteral(CString Literal, CString &ErrorMessage)
{
    return CheckAnyBit(Literal, 
                       _T("11111111111111111111111111111111"),
                       _T("37777777777"),
                       _T("4294967295"),
                       _T("FFFFFFFF"),
                       ErrorMessage);
}
*/

//------------------------------------------------------------------*
/**
 * Function name    CheckLwordLiteral
 * Description      checks whether Literal is an st lword literal
 * @param           CString Literal
 * @param           CString &ErrorMessage
 * @return          BOOL
 * @exception       -
 * @see             -
*/
/*
BOOL CheckLwordLiteral(CString Literal, CString &ErrorMessage)
{
    return CheckAnyBit(Literal, 
                       _T("1111111111111111111111111111111111111111111111111111111111111111"),
                       _T("177777777777777777777"),
                       _T("18446744073709551615"),
                       _T("FFFFFFFFFFFFFFFF"),
                       ErrorMessage);
}
*/

//------------------------------------------------------------------*
/**
 * Function name    CheckDintLiteral
 * Description      checks whether Literal is an st dint literal
 * @param           CString Literal
 * @param           CString &ErrorMessage
 * @return          BOOL
 * @exception       -
 * @see             -
*/
/*
BOOL CheckDintLiteral(CString Literal, CString &ErrorMessage)
{
    if (Literal.GetAt(0) == _T('-'))
    {
        Literal.Delete(0, 1);
        return CheckForBase(10, _T("2147483648"), Literal, ErrorMessage);

    }
    else
    {
        if (Literal.GetAt(0) == _T('+'))
        {
            Literal.Delete(0, 1);
        }
        return CheckForBase(10, _T("2147483647"), Literal, ErrorMessage);
    }
}
*/

//------------------------------------------------------------------*
/**
 * Function name    CheckLintLiteral
 * Description      checks whether Literal is an st lint literal
 * @param           CString Literal
 * @param           CString &ErrorMessage
 * @return          BOOL
 * @exception       -
 * @see             -
*/
/*
BOOL CheckLintLiteral(CString Literal, CString &ErrorMessage)
{
    if (Literal.GetAt(0) == _T('-'))
    {
        Literal.Delete(0, 1);
        return CheckForBase(10, _T("9223372036854775808"), Literal, ErrorMessage);

    }
    else
    {
        if (Literal.GetAt(0) == _T('+'))
        {
            Literal.Delete(0, 1);
        }
        return CheckForBase(10, _T("9223372036854775807"), Literal, ErrorMessage);
    }
}
*/

//------------------------------------------------------------------*
/**
 * Function name    CheckLrealLiteral
 * Description      checks whether Literal is an st lreal literal
 * @param           CString Literal
 * @param           CString &ErrorMessage
 * @return          BOOL
 * @exception       -
 * @see             -
*/
/*
BOOL CheckLrealLiteral(CString Literal, CString &ErrorMessage)
{
    BOOL result = CheckString(Literal, _T("LREAL"), ErrorMessage);
    return result;
}
*/

//------------------------------------------------------------------*
/**
 * Function name    CheckTimeLiteral
 * Description      checks whether Literal is an st time literal
 * @param           CString Literal
 * @param           CString &ErrorMessage
 * @return          BOOL
 * @exception       -
 * @see             -
*/
/*
BOOL CheckTimeLiteral(CString Literal, CString &ErrorMessage)
{
    BOOL result = CheckString(Literal, _T("TIME"), ErrorMessage);
    return result;
}
*/

//------------------------------------------------------------------*
/**
 * Function name    CheckDtLiteral
 * Description      checks whether Literal is an st dt literal
 * @param           CString Literal
 * @param           CString &ErrorMessage
 * @return          BOOL
 * @exception       -
 * @see             -
*/
/*
BOOL CheckDtLiteral(CString Literal, CString &ErrorMessage)
{
    BOOL result = CheckString(Literal, _T("DT"), ErrorMessage);
    return result;
}
*/

//------------------------------------------------------------------*
/**
 * Function name    CheckDateLiteral
 * Description      checks whether Literal is an st date literal
 * @param           CString Literal
 * @param           CString &ErrorMessage
 * @return          BOOL
 * @exception       -
 * @see             -
*/
/*
BOOL CheckDateLiteral(CString Literal, CString &ErrorMessage)
{
    BOOL result = CheckString(Literal, _T("DATE"), ErrorMessage);
    return result;
}
*/

//------------------------------------------------------------------*
/**
 * Function name    CheckTodLiteral
 * Description      checks whether Literal is an st tod literal
 * @param           CString Literal
 * @param           CString &ErrorMessage
 * @return          BOOL
 * @exception       -
 * @see             -
*/
/*
BOOL CheckTodLiteral(CString Literal, CString &ErrorMessage)
{
    BOOL result = CheckString(Literal, _T("TOD"), ErrorMessage);
    return result;
}
*/

//------------------------------------------------------------------*
/**
 * Function name    CheckWstringLiteral
 * Description      checks whether Literal is an st wstring literal
 * @param           CString Literal
 * @param           CString &ErrorMessage
 * @return          BOOL
 * @exception       -
 * @see             -
*/
/*
BOOL CheckWstringLiteral(CString Literal, CString &ErrorMessage)
{
//    return (Literal.Find(_T("\"")) == -1);
    BOOL result = CheckString(Literal, _T("WSTRING"), ErrorMessage);
    return result;
}
*/

//------------------------------------------------------------------*
/**
 * Function name    CheckWstringLiteral
 * Description      checks whether Literal is an st literal of type Type
 * @param           CString Type
 * @param           CString Literal
 * @param           CString &ErrorMessage
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CheckTypeOfLiteral(CString Type, CString Literal, CString &ErrorMessage)
{
    ErrorMessage = _T("");
/*
    if (Type.CompareNoCase(_T("BOOL")) == 0)
        return CheckBoolLiteral(Literal, ErrorMessage);

    if (Type.CompareNoCase(_T("BYTE")) == 0)
        return CheckByteLiteral(Literal, ErrorMessage);

    if (Type.CompareNoCase(_T("WORD")) == 0)
        return CheckWordLiteral(Literal, ErrorMessage);

    if (Type.CompareNoCase(_T("DWORD")) == 0)
        return CheckDwordLiteral(Literal, ErrorMessage);

    if (Type.CompareNoCase(_T("LWORD")) == 0)
        return CheckLwordLiteral(Literal, ErrorMessage);

    if (Type.CompareNoCase(_T("DINT")) == 0)
        return CheckDintLiteral(Literal, ErrorMessage);

    if (Type.CompareNoCase(_T("LINT")) == 0)
        return CheckLintLiteral(Literal, ErrorMessage);

    if (Type.CompareNoCase(_T("LREAL")) == 0)
        return CheckLrealLiteral(Literal, ErrorMessage);

    if (Type.CompareNoCase(_T("TIME")) == 0)
        return CheckTimeLiteral(Literal, ErrorMessage);

    if (Type.CompareNoCase(_T("DT")) == 0)
        return CheckDtLiteral(Literal, ErrorMessage);

    if (Type.CompareNoCase(_T("DATE")) == 0)
        return CheckDateLiteral(Literal, ErrorMessage);

    if (Type.CompareNoCase(_T("TOD")) == 0)
        return CheckTodLiteral(Literal, ErrorMessage);

    if (Type.CompareNoCase(_T("WSTRING")) == 0)
        return CheckWstringLiteral(Literal, ErrorMessage);
*/

    if ((Type.CompareNoCase(_T("BOOL")) == 0) ||
        (Type.CompareNoCase(_T("BYTE")) == 0) ||
        (Type.CompareNoCase(_T("WORD")) == 0) ||
        (Type.CompareNoCase(_T("DWORD")) == 0) ||
        (Type.CompareNoCase(_T("LWORD")) == 0) ||
        (Type.CompareNoCase(_T("INT")) == 0) ||
        (Type.CompareNoCase(_T("DINT")) == 0) ||
        (Type.CompareNoCase(_T("LINT")) == 0) ||
        (Type.CompareNoCase(_T("REAL")) == 0) ||
        (Type.CompareNoCase(_T("LREAL")) == 0) ||
        (Type.CompareNoCase(_T("TIME")) == 0) ||
        (Type.CompareNoCase(_T("TIME_OF_DAY")) == 0) ||
        (Type.CompareNoCase(_T("DATE_AND_TIME")) == 0) ||
        (Type.CompareNoCase(_T("DT")) == 0) ||
        (Type.CompareNoCase(_T("DATE")) == 0) ||
        (Type.CompareNoCase(_T("TOD")) == 0) ||
        (Type.CompareNoCase(_T("STRING")) == 0) ||
        (Type.CompareNoCase(_T("WSTRING")) == 0))     
    {
        BOOL result = CheckString(Literal, Type, ErrorMessage);
        return result;
    }
    else
    {
        return TRUE;
    }
}

//------------------------------------------------------------------*
/**
 * Function name    CheckString
 * Description      checks the syntax of a literal
 * @param           CString Value   
 * @param           CString Datatype
 * @param           CString &ErrorMessage
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CheckString(CString Value, 
                 CString Datatype,
                 CString &ErrorMessage)
{
    long Result;
    HRESULT hr = S_OK;
    IBL_Conversion *pIBL_Conversion = NULL;

    hr = CoInitialize(NULL);
	if(FAILED(hr)) {
        ErrorMessage = InternalError;
        return false;
    }

    hr = CoCreateInstance(CLSID_FCBaseLib,
                          NULL, 
                          CLSCTX_INPROC_SERVER, 
                          IID_IBL_Conversion,
                          ((void**)&pIBL_Conversion));

    if(SUCCEEDED(hr)) 
    {
        BSTR sValue;
        sValue = Value.AllocSysString();
        BSTR sDatatype;
        sDatatype = Datatype.AllocSysString();
        BSTR sFormat;
        sFormat = L"";
        long options = BL_USE_IEC_FORMAT;

        hr = pIBL_Conversion->CheckString(sValue, sDatatype, sFormat, options, &Result);
        ::SysFreeString(sValue);
        ::SysFreeString(sDatatype);

        if (pIBL_Conversion != NULL)
        {
            pIBL_Conversion->Release();
        }
    }
    else
    {
        CoUninitialize( );
        ErrorMessage = InternalError;
        return false;
    }

    CoUninitialize( );

    if (hr == S_OK)
    {
        return true;
    }
    else if (hr == S_FALSE)
    {
        if (Result == E_BL_ERR_SYNTAX)
        {
            ErrorMessage = SyntaxError;
        }
        else if (Result == E_BL_ERR_RANGE)
        {
            ErrorMessage = RangeError;
        }
        else if (Result == E_BL_ERR_FORMATERROR)
        {
            ErrorMessage = FormatError;
        }
        else if (Result == E_BL_ERR_INVALIDARG)
        {
            ErrorMessage = ArgumentError;
        }
        else if (Result == E_BL_ERR_FUN_NOT_IMPL)
        {
            ErrorMessage = FunctionError;
        }
        else if (Result == E_BL_ERR_BUFFER_TO_SMALL)
        {
            ErrorMessage = BufferError;
        }
        else if (Result == E_BL_ERR_INTERNAL)
        {
            ErrorMessage = InternalError;
        }
        else if (Result == E_BL_ERR_LOSS_OF_PRECIS)
        {
            ErrorMessage = PrecisitionError;
        }
        else if (Result == E_BL_ERR_RADIX)
        {
            ErrorMessage = RadixError;
        }
        
        return false;
    }
    else
    {
        ErrorMessage = InternalError;
        return false;
    }
}


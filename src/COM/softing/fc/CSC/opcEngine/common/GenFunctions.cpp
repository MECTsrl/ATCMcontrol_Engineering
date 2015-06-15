//-----------------------------------------------------------------------------
//                OPC ENGINE              OPC SESSION                         |
//                                                                            |
//  Filename    : GENFUNCTIONS.CPP                                            |   
//  Version     : 2.00.0.00.release       2.00.0.00.release                   |
//  Date        : 29-March-1999           29-March-1999                       |
//  Author      : ATCM                                                |
//                                                                            |
//  Description : Helper Functions                                            |
//                                                                            |
//  CHANGE_NOTES                                                              | 
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//                                                                            |
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include <atlconv.h>
#include <opcda.h>
#include "GenFunctions.h"

//-----------------------------------------------------------------------------
// setString
//
// - set the string data
// This method is only needed if using static libraries, because then 2 different
// C-Runtime libraries are used by the toolkit and the application !!!
//
void setString(
	OUT CString &string,	// string
	IN LPCTSTR value)		// new value
{
	string = value;
} // setString


CString g_empty = _T("EMPTY");

//-----------------------------------------------------------------------------
// getEmptyString
//
// - get the empty string
// This method is only needed if using static libraries, because then 2 different
// C-Runtime libraries are used by the toolkit and the application !!!
//
CString &getEmptyString(void)
{
	return g_empty;
} // getEmptyString


//-----------------------------------------------------------------------------
// emptyString
//
// - delete the string data
// This method is only needed if using static libraries, because then 2 different
// C-Runtime libraries are used by the toolkit and the application !!!
//
void emptyString(
	IN const CString &empty,	// empty string
	OUT CString& string)		// string
{ 
	string.Empty(); 
	string = empty;
} // emptyString


//-----------------------------------------------------------------------------
// emptyStringList
//
// - delete all strings in string list
// This method is only needed if using static libraries, because then 2 different
// C-Runtime libraries are used by the toolkit and the application !!!
//
void emptyStringList(
	OUT CStringList& stringList)	// string list
{ 
	stringList.RemoveAll(); 
} // emptyStringList


//-----------------------------------------------------------------------------
// addStringToList
//
// - add a string to a string list
// This method is only needed if using static libraries, because then 2 different
// C-Runtime libraries are used by the toolkit and the application !!!
//
POSITION addStringToList(
	IN CStringList& stringList,		// string list
	IN LPCTSTR value)				// string value
{
	return stringList.AddTail(value);
} // addStringToList


//-----------------------------------------------------------------------------
// emptyPtrList
//
// - delete all strings in pointer list
// This method is only needed if using static libraries, because then 2 different
// C-Runtime libraries are used by the toolkit and the application !!!
//
void emptyPtrList(
	OUT CPtrList& ptrList)	// pointer list
{ 
	ptrList.RemoveAll(); 
} // emptyPtrList


//-----------------------------------------------------------------------------
// addPtrToList
//
// - add a pointer to a pointer list
// This method is only needed if using static libraries, because then 2 different
// C-Runtime libraries are used by the toolkit and the application !!!
//
POSITION addPtrToList(
	IN CPtrList& ptrList,	// pointer list
	IN void* value)			// pointer
{
	return ptrList.AddTail(value);
} // addPtrToList


//-----------------------------------------------------------------------------
// allocateMemory
//
// - allocate memory block
// This method is only needed if using static libraries, because then 2 different
// C-Runtime libraries are used by the toolkit and the application !!!
//
void* allocateMemory(
	IN ULONG size)	// memory size
{
	return malloc(size);
} // allocateMemory


//-----------------------------------------------------------------------------
// freeMemory
//
// - free memory block
// This method is only needed if using static libraries, because then 2 different
// C-Runtime libraries are used by the toolkit and the application !!!
//
void freeMemory(
	IN	void *memptr)	// memory to free
{
	free(memptr);
} // freeMemory


//-----------------------------------------------------------------------------
// getTimeSpan
//
// - calculate span between to DWORD times
//
// returns:
//		time span
//
ULONG getTimeSpan(
	IN DWORD from,	// start of span 
	IN DWORD till)	// end of span
{
	ULONG span;

	if (till >= from)
		span = till - from;
	else 
		span = (ULONG_MAX - from) + till;	// tick count overflow !


	if (span > 4294900000)
		span = 0;

	return span;
} // getTimeSpan


//-----------------------------------------------------------------------------
// enumString2stringList
//
// - copy the entries of an IEnumString object into a MFC string list
//
void enumString2stringList(
	IN IEnumString *enumString,		// interface pointer of IEnumString object
	OUT CStringList *stringList)	// MFC string list
{
	LPOLESTR string;
	ULONG number;
	BOOL end = FALSE;
	USES_CONVERSION;

	if ((!enumString) || (!stringList))
		return;

	enumString->Reset();
	while ((!end) && (SUCCEEDED(enumString->Next(1, &string, &number))))
	{
		if (number == 0)
		{
			end = TRUE;
			break;
		}
		stringList->AddTail(W2CT(string));
	}
} // enumString2stringList


//-----------------------------------------------------------------------------
// variant2string
//
// - convert variant to string
//
// returns:
//		pointer to string
//
LPCTSTR variant2string(
	IN LPTSTR buffer,	// string buffer 
	IN DWORD size,		// string buffer size
	IN LPCVARIANT var)	// variant
{
	CString buf;
	variant2string(&buf, var);
	_tcsncpy(buffer, (LPCTSTR)buf, size);
	buffer[size] = _T('\0');
	return buffer;
} // variant2string


//-----------------------------------------------------------------------------
// variant2string
//
// - convert variant to string
//
// returns:
//		pointer to string
//
LPCTSTR variant2string(
	IN CString *buffer,	// string buffer
	IN LPCVARIANT var)	// variant
{
	switch (var->vt)
	{
		case VT_EMPTY:
			*buffer = _T("");
		break;
		case VT_UI1:
		{
			unsigned short usVal = (unsigned short)var->bVal & 0xFF;
			buffer->Format(_T("%u"), usVal);		
		}
		break;
		case VT_I2:
			buffer->Format(_T("%i"), var->iVal);		
		break;
		case VT_I4:
			buffer->Format(_T("%li"), var->lVal);		
		break;
		case VT_R4:
			buffer->Format(_T("%.2f"), var->fltVal);		
		break;
		case VT_R8:
			buffer->Format(_T("%.2lf"), var->dblVal);		
		break;
		case VT_BOOL:
			buffer->Format(_T("%s"), BOOL2STR(var->boolVal));		
		break;
		case VT_BSTR:
		{
			USES_CONVERSION;
			buffer->Format(_T("%-.90s"), OLE2CT(var->bstrVal));		
		}
		break;
		case VT_DATE:
		{
			SYSTEMTIME t;	
			::VariantTimeToSystemTime(var->date, &t);
			buffer->Format(_T("%02.2d:%02.2d:%02.2d"), t.wHour, t.wMinute, t.wSecond);
		}
		break;
		case VT_DECIMAL:
		{
			USES_CONVERSION;
			VARIANT bstr;
			::VariantInit(&bstr);
			::VariantChangeType(&bstr, (LPVARIANT)var, 0, VT_BSTR);
			buffer->Format(_T("%-.90s"), OLE2CT(bstr.bstrVal));		
			::VariantClear(&bstr);
		}
		break;
		case (VT_ARRAY | VT_I2):
		case (VT_ARRAY | VT_I4):
		case (VT_ARRAY | VT_UI1):
		case (VT_ARRAY | VT_R4):
		case (VT_ARRAY | VT_R8):
		case (VT_ARRAY | VT_BOOL):
		case (VT_ARRAY | VT_BSTR):
		case (VT_ARRAY | VT_DATE):
		case (VT_ARRAY | VT_DECIMAL):
		{	
			LONG lBound, uBound;
			LONG i;
			CString valStr;
			SafeArrayGetLBound(var->parray, 1, &lBound);
			SafeArrayGetUBound(var->parray, 1, &uBound);

			buffer->Format(_T("[%d,%d] ("), lBound, uBound);		
			for (i = lBound; i <= uBound; i++)
			{
				switch (var->vt & ~VT_ARRAY)
				{
					case VT_I2:
					{
						short iVal;
						SafeArrayGetElement(var->parray, &i, &iVal);
						valStr.Format(_T("%i"), iVal);		
					}
					break;
					case VT_I4:
					{
						long lVal;
						SafeArrayGetElement(var->parray, &i, &lVal);
						valStr.Format(_T("%li"), lVal);		
					}
					break;
					case VT_UI1:
					{
						char bVal;
						unsigned short usVal;
						SafeArrayGetElement(var->parray, &i, &bVal);
						usVal = (unsigned short)bVal & 0xFF;
						valStr.Format(_T("%u"), usVal);		
					}
					break;
					case VT_R4:
					{
						float fltVal;
						SafeArrayGetElement(var->parray, &i, &fltVal);
						valStr.Format(_T("%.2f"), fltVal);		
					}
					break;
					case VT_R8:
					{
						double dblVal;
						SafeArrayGetElement(var->parray, &i, &dblVal);
						valStr.Format(_T("%.2lf"), dblVal);		
					}
					break;
					case VT_BOOL:
					{
						VARIANT_BOOL boolVal;
						SafeArrayGetElement(var->parray, &i, &boolVal);
						valStr.Format(_T("%s"), (boolVal) ? _T("1") : _T("0"));		
					}
					break;
					case VT_BSTR:
					{
						USES_CONVERSION;
					    BSTR bstrVal = NULL;

						::SafeArrayGetElement(var->parray, &i, &bstrVal);
						valStr.Format(_T("%-.80s"), OLE2CT(bstrVal));		
						::SysFreeString(bstrVal);
					}
					break;
					case VT_DATE:
					{
						double dblVal;
						SYSTEMTIME t;	
						SafeArrayGetElement(var->parray, &i, &dblVal);
						::VariantTimeToSystemTime(dblVal, &t);
						valStr.Format(_T("%02.2d:%02.2d:%02.2d"), t.wHour, t.wMinute, t.wSecond);
					}
					break;
					case VT_DECIMAL:
					{
						USES_CONVERSION;
						VARIANT bstr;

						::VariantInit(&bstr);
						::SafeArrayGetElement(var->parray, &i, &bstr.decVal);
						bstr.vt = VT_DECIMAL;
						::VariantChangeType(&bstr, &bstr, 0, VT_BSTR);
						valStr.Format(_T("%-.90s"), OLE2CT(bstr.bstrVal));		
						::VariantClear(&bstr);
					}
					break;
				}	
	
				*buffer += valStr;
				if (i+1 <= uBound)
					*buffer += _T(", ");
				else
					*buffer +=  _T(")");
			}
		}
		break;
		default:
			buffer->Format(_T("VARIANT vt; 0x%8.8X"), var->vt);
		break;
	}

	return (LPCTSTR)*buffer;
} // variant2string


//-----------------------------------------------------------------------------
// variantWithType2string
//
// - convert variant with vartype to string
//
// returns:
//		pointer to string
//
LPCTSTR variantWithType2string(
	IN LPTSTR buffer,	// string buffer 
	IN DWORD size,		// string buffer size
	IN LPCVARIANT var)	// variant
{
	CString buf;
	variantWithType2string(&buf, var);
	_tcsncpy(buffer, (LPCTSTR)buf, size);
	buffer[size] = _T('\0');
	return buffer;
} // variantWithType2string


//-----------------------------------------------------------------------------
// variantWithType2string
//
// - convert variant with vartype to string
//
// returns:
//		pointer to string
//
LPCTSTR variantWithType2string(
	IN CString *buffer,	// string buffer
	IN LPCVARIANT var)	// variant
{
	CString type;

	switch (var->vt)
	{
		case VT_EMPTY:
			*buffer = _T("EMPTY");
		break;
		case VT_UI1:
		{
			unsigned short usVal = (unsigned short)var->bVal & 0xFF;
			buffer->Format(_T("UI1(%u)"), usVal);		
		}
		break;
		case VT_I2:
			buffer->Format(_T("I2(%i)"), var->iVal);		
		break;
		case VT_I4:
			buffer->Format(_T("I4(%li)"), var->lVal);		
		break;
		case VT_R4:
			buffer->Format(_T("R4(%.2f)"), var->fltVal);		
		break;
		case VT_R8:
			buffer->Format(_T("R8(%.2lf)"), var->dblVal);		
		break;
		case VT_BOOL:
			buffer->Format(_T("BOOL(%s)"), BOOL2STR(var->boolVal));		
		break;
		case VT_BSTR:
		{
			USES_CONVERSION;
			buffer->Format(_T("BSTR(%-.90s)"), OLE2CT(var->bstrVal));		
		}
		break;
		case VT_DATE:
		{
			SYSTEMTIME t;	
			::VariantTimeToSystemTime(var->date, &t);
			buffer->Format(_T("DATE(%02.2d:%02.2d:%02.2d)"), t.wHour, t.wMinute, t.wSecond);
		}
		break;
		case VT_DECIMAL:
		{
			USES_CONVERSION;
			VARIANT bstr;
			::VariantInit(&bstr);
			::VariantChangeType(&bstr, (LPVARIANT)var, 0, VT_BSTR);
			buffer->Format(_T("DECIMAL(%-.90s)"), OLE2CT(bstr.bstrVal));		
			::VariantClear(&bstr);
		}
		break;
		case (VT_ARRAY | VT_I2):
		case (VT_ARRAY | VT_I4):
		case (VT_ARRAY | VT_UI1):
		case (VT_ARRAY | VT_R4):
		case (VT_ARRAY | VT_R8):
		case (VT_ARRAY | VT_BOOL):
		case (VT_ARRAY | VT_BSTR):
		case (VT_ARRAY | VT_DATE):
		case (VT_ARRAY | VT_DECIMAL):
		{	
			LONG lBound, uBound;
			LONG i;
			CString valStr;
			SafeArrayGetLBound(var->parray, 1, &lBound);
			SafeArrayGetUBound(var->parray, 1, &uBound);

			buffer->Format(_T("%s[%d,%d] ("), vartype2string(&type, var->vt & ~VT_ARRAY), lBound, uBound);		
			for (i = lBound; i <= uBound; i++)
			{
				switch (var->vt & ~VT_ARRAY)
				{
					case VT_I2:
					{
						short iVal;
						SafeArrayGetElement(var->parray, &i, &iVal);
						valStr.Format(_T("%i"), iVal);		
					}
					break;
					case VT_I4:
					{
						int lVal;
						SafeArrayGetElement(var->parray, &i, &lVal);
						valStr.Format(_T("%li"), lVal);		
					}
					break;
					case VT_UI1:
					{
						char bVal;
						unsigned short usVal;
						SafeArrayGetElement(var->parray, &i, &bVal);
						usVal = (unsigned short)bVal & 0xFF;
						valStr.Format(_T("%u"), usVal);		
					}
					break;
					case VT_R4:
					{
						float fltVal;
						SafeArrayGetElement(var->parray, &i, &fltVal);
						valStr.Format(_T("%.2f"), fltVal);		
					}
					break;
					case VT_R8:
					{
						double dblVal;
						SafeArrayGetElement(var->parray, &i, &dblVal);
						valStr.Format(_T("%.2lf"), dblVal);		
					}
					break;
					case VT_BOOL:
					{
						VARIANT_BOOL boolVal;
						SafeArrayGetElement(var->parray, &i, &boolVal);
						valStr.Format(_T("%s"), (boolVal) ? _T("1") : _T("0"));		
					}
					break;
					case VT_BSTR:
					{
						USES_CONVERSION;
					    BSTR bstrVal = NULL;

						::SafeArrayGetElement(var->parray, &i, &bstrVal);
						valStr.Format(_T("%-.80s"), OLE2CT(bstrVal));		
						::SysFreeString(bstrVal);
					}
					break;
					case VT_DATE:
					{
						double dblVal;
						SYSTEMTIME t;	
						SafeArrayGetElement(var->parray, &i, &dblVal);
						::VariantTimeToSystemTime(dblVal, &t);
						valStr.Format(_T("%02.2d:%02.2d:%02.2d"), t.wHour, t.wMinute, t.wSecond);
					}
					break;
					case VT_DECIMAL:
					{
						USES_CONVERSION;
						VARIANT bstr;

						::VariantInit(&bstr);
						::SafeArrayGetElement(var->parray, &i, &bstr.decVal);
						bstr.vt = VT_DECIMAL;
						::VariantChangeType(&bstr, &bstr, 0, VT_BSTR);
						valStr.Format(_T("%-.90s"), OLE2CT(bstr.bstrVal));		
						::VariantClear(&bstr);
					}
					break;
				}	
	
				*buffer += valStr;
				if (i+1 <= uBound)
					*buffer += _T(", ");
				else
					*buffer +=  _T(")");
			}
		}
		break;
		default:
			buffer->Format(_T("VARIANT vt; 0x%8.8X"), var->vt);
		break;
	}

	return (LPCTSTR)*buffer;
} // variantWithType2string


//-----------------------------------------------------------------------------
// vartype2string
//
// - convert vartype to string
//
// returns:
//		pointer to string
//
LPCTSTR vartype2string(
	IN LPTSTR buffer,	// string buffer 
	IN DWORD size,		// string buffer size
	IN VARTYPE vt)		// vartype
{
	CString buf;
	vartype2string(&buf, vt);
	_tcsncpy(buffer, (LPCTSTR)buf, size);
	buffer[size] = _T('\0');
	return buffer;
} // vartype2string


//-----------------------------------------------------------------------------
// vartype2string
//
// - convert vartype to string
//
// returns:
//		pointer to string
//
LPCTSTR vartype2string(
	IN CString *buffer,	// string buffer
	IN VARTYPE vt)		// vartype
{
	switch (vt)
	{
		case VT_I2:
			*buffer = _T("I2");		
		break;
		case VT_I4:
			*buffer = _T("I4");		
		break;
		case VT_UI1:
			*buffer = _T("UI1");		
		break;
		case VT_R4:
			*buffer = _T("R4");		
		break;
		case VT_R8:
			*buffer = _T("R8");		
		break;
		case VT_BOOL:
			*buffer = _T("BOOL");		
		break;
		case VT_BSTR:
			*buffer = _T("BSTR");		
		break;
		case VT_DATE:
			*buffer = _T("DATE");		
		break;
		case VT_DECIMAL:
			*buffer = _T("DECIMAL");
		break;
		case VT_EMPTY:
			*buffer = _T("EMPTY");
		break;
		case VT_UI1 | VT_ARRAY:
			*buffer = _T("UI1 ARRAY");
		break;
		case VT_I2 | VT_ARRAY:
			*buffer = _T("I2 ARRAY");
		break;
		case VT_I4 | VT_ARRAY:
			*buffer = _T("I4 ARRAY");
		break;
		case VT_R4 | VT_ARRAY:
			*buffer = _T("R4 ARRAY");
		break;
		case VT_R8 | VT_ARRAY:
			*buffer = _T("R8 ARRAY");
		break;
		case VT_BOOL | VT_ARRAY:
			*buffer = _T("BOOL ARRAY");
		break;
		case VT_BSTR | VT_ARRAY:
			*buffer = _T("BSTR ARRAY");
		break;
		case VT_DATE | VT_ARRAY:
			*buffer = _T("DATE ARRAY");
		break;
		case VT_DECIMAL | VT_ARRAY:
			*buffer = _T("DECIMAL ARRAY");
		break;
		default:
			buffer->Format(_T("VARTYPE 0x%8.8X"), vt);
		break;
	}

	return (LPCTSTR)*buffer;
} // vartype2string


//-----------------------------------------------------------------------------
// clipformat2string
//
// - convert clipformat to string
//
// returns:
//		pointer to string
//
LPCTSTR clipformat2string(
	IN LPTSTR buffer,		// string buffer 
	IN DWORD size,			// string buffer size
	IN CLIPFORMAT format)	// clipformat
{
	CString buf;
	clipformat2string(&buf, format);
	_tcsncpy(buffer, (LPCTSTR)buf, size);
	buffer[size] = _T('\0');
	return buffer;
} // clipformat2string


//-----------------------------------------------------------------------------
// clipformat2string
//
// - convert clipformat to string
//
// returns:
//		pointer to string
//
LPCTSTR clipformat2string(
	IN CString *buffer,		// string buffer
	IN CLIPFORMAT format)	// clipformat
{
	LPTSTR buf = buffer->GetBuffer(100);
	GetClipboardFormatName(format, buf, 99);
	buffer->ReleaseBuffer();
	return (LPCTSTR)*buffer;
} // clipformat2string


//-----------------------------------------------------------------------------
// quality2string
//
// - convert quality to string
//
// returns:
//		pointer to string
//
LPCTSTR quality2string(
	IN LPTSTR buffer,	// string buffer 
	IN DWORD size,		// string buffer size
	IN WORD quality)	// quality
{
	CString buf;
	quality2string(&buf, quality);
	_tcsncpy(buffer, (LPCTSTR)buf, size);
	buffer[size] = _T('\0');
	return buffer;
} // quality2string


//-----------------------------------------------------------------------------
// quality2string
//
// - convert quality to string
//
// returns:
//		pointer to string
//
LPCTSTR quality2string(
	IN CString *buffer,	// string buffer
	IN WORD quality)	// quality
{
    switch (quality & OPC_QUALITY_MASK) 
	{
		case OPC_QUALITY_BAD:
			*buffer = _T("BAD ");
			switch (quality & OPC_STATUS_MASK) 
			{
				case OPC_QUALITY_CONFIG_ERROR:
					*buffer += _T("(config error)");
					break;
				case OPC_QUALITY_NOT_CONNECTED:
					*buffer += _T("(not connected)");
					break;
				case OPC_QUALITY_DEVICE_FAILURE:
					*buffer += _T("(device failure)");
					break;
				case OPC_QUALITY_SENSOR_FAILURE:
					*buffer += _T("(sensor failure)");
					break;
				case OPC_QUALITY_LAST_KNOWN:
					*buffer += _T("(last known)");
					break;
				case OPC_QUALITY_COMM_FAILURE:
					*buffer += _T("(comm failure)");
					break;
				case OPC_QUALITY_OUT_OF_SERVICE:
					*buffer += _T("(out of service)");
					break;
			}
			break;
		case OPC_QUALITY_UNCERTAIN:
			*buffer = _T("UNCERTAIN ");
			switch (quality & OPC_STATUS_MASK) 
			{
				case OPC_QUALITY_LAST_USABLE:
					*buffer += _T("(last usable)");
					break;
				case OPC_QUALITY_SENSOR_CAL:
					*buffer += _T("(sensor cal)");
					break;
				case OPC_QUALITY_EGU_EXCEEDED:
					*buffer += _T("(egu exceeded)");
					break;
				case OPC_QUALITY_SUB_NORMAL:
					*buffer += _T("(sub normal)");
					break;
			}
			break;

		case OPC_QUALITY_GOOD:
			*buffer = _T("GOOD ");
			switch (quality & OPC_STATUS_MASK) 
			{
				case OPC_QUALITY_LOCAL_OVERRIDE:
					*buffer += _T("(local override)");
					break;
			}
			break;

		default:
			buffer->Format(_T("QUALITY 0x%4.4X"), quality);
		break;
    }

	return (LPCTSTR)*buffer;
} // quality2string


//-----------------------------------------------------------------------------
// accessRights2string
//
// - convert access rights to string
//
// returns:
//		pointer to string
//
LPCTSTR accessRights2string(
	IN LPTSTR buffer,		// string buffer 
	IN DWORD size,			// string buffer size
	IN DWORD accessRight)	// access rights
{
	CString buf;
	accessRights2string(&buf, accessRight);
	_tcsncpy(buffer, (LPCTSTR)buf, size);
	buffer[size] = _T('\0');
	return buffer;
} // accessRights2string


//-----------------------------------------------------------------------------
// accessRights2string
//
// - convert access rights to string
//
// returns:
//		pointer to string
//
LPCTSTR accessRights2string(
	IN CString *buffer,		// string buffer
	IN DWORD accessRight)	// access rights
{
	*buffer = _T("");
	if ((accessRight & OPC_READABLE) == OPC_READABLE)
	{
		*buffer += _T("READ");
	}
	if ((accessRight & OPC_WRITEABLE) == OPC_WRITEABLE)
	{
		if (buffer->GetLength() != 0)
			*buffer += _T(" and ");

		*buffer += _T("WRITE");
	}
	return (LPCTSTR)*buffer;
} // accessRights2string


//-----------------------------------------------------------------------------
// filetime2string
//
// - convert filetime to string
//
// returns:
//		pointer to string
//
LPCTSTR filetime2string(
	IN LPTSTR buffer,		// string buffer 
	IN DWORD size,			// string buffer size
	IN LPFILETIME filetime)	// filetime
{
	CString buf;
	filetime2string(&buf, filetime);
	_tcsncpy(buffer, (LPCTSTR)buf, size);
	buffer[size] = _T('\0');
	return buffer;
} // filetime2string


//-----------------------------------------------------------------------------
// filetime2string
//
// - convert filetime to string
//
// returns:
//		pointer to string
//
LPCTSTR filetime2string(
	IN CString *buffer,		// string buffer
	IN LPFILETIME filetime)	// filetime
{
	SYSTEMTIME t;
	FileTimeToSystemTime(filetime, &t);
	buffer->Format(_T("%02.2d:%02.2d:%02.2d.%03.3d"), t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
	return (LPCTSTR)*buffer;
} // filetime2string



//-----------------------------------------------------------------------------
// IID2string
//
// - convert interface id into interface description string
//
// returns:
//		pointer to string
//
LPCTSTR IID2string(
	IN CString *buffer,		// string buffer
	IN REFIID riid)			// interface id
{
	LPWSTR wIIDstring = NULL;
	CString iidString;

	StringFromIID(riid, &wIIDstring);

	if (wIIDstring)
	{
		CComPtr<IMalloc> malloc;
		USES_CONVERSION;
		iidString = OLE2T(wIIDstring);

		::CoGetMalloc(MEMCTX_TASK, &malloc);
		malloc->Free(wIIDstring);		
	}

	if (!iidString.IsEmpty())
	{
		TCHAR iidPath[48];
		TCHAR iidDescr[100];
		LONG size = 100;

		iidDescr[0] = '\0';
		_stprintf(iidPath, _T("Interface\\%-.38s"), iidString);
		RegQueryValue(HKEY_CLASSES_ROOT, iidPath, iidDescr, &size);

		if (_tcslen(iidDescr) > 0)
		{
			iidString = iidDescr;
		}
	}

	*buffer = iidString;
	return (LPCTSTR)*buffer;
} // IID2string

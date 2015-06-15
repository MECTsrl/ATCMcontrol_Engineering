/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary/CELibraryItem.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CELibraryItem.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary/CELibraryItem.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CL
 *
 * =COMPONENT           CELibrarian
 *
 * =CURRENT      $Date: 28.02.07 18:59 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  6/25/2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/



//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "CELibraryItem.h"
#include "CEFile.h"
//----  Static Initializations:   ----------------------------------*





#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////
/**
 * constructor
 *
 * @param itemName  name of item
 * @return		
 * @exception   
 * @see         
 */
CELibraryItem::CELibraryItem(const CString& itemName)
{
	m_itemName = itemName;
}

/////////////////////////////////////////////////////////////////////
/**
 * destructor
 *
 * @param
 * @return
 * @exception   
 * @see         
 */
CELibraryItem::~CELibraryItem()
{
	
}

/////////////////////////////////////////////////////////////////////
/**
 * add a file name to an item
 *	
 * @param fileName	the file name
 * @return		1 ok
 * @exception   
 * @see         
 */
int CELibraryItem::AddFile(const CString& fileName)
{
	m_fileNames.Add(fileName);
	return 1;
}

/////////////////////////////////////////////////////////////////////
/**
 * delete an item file 
 *
 * @param fileName	the file name
 * @return		1 file deleted, 0 file not found
 * @exception   
 * @see         
 */
int CELibraryItem::DeleteFile(const CString& fileName)
{
	CString name;
	for (int ii=0; ii<m_fileNames.GetSize(); ii++)
	{
		name = m_fileNames.GetAt(ii);
		if ( name.CompareNoCase(fileName) == 0 )
		{
			m_fileNames.RemoveAt(ii);
			return 1;
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////
/**
 * test if item file exist
 *
 * @param fileName	the fileName
 * @return		1 file exists, 0 file does not exist
 * @exception   
 * @see         
 */
BOOL CELibraryItem::ExistFile(const CString& fileName)
{
	CString name;
	for (int ii=0; ii<m_fileNames.GetSize(); ii++)
	{
		name = m_fileNames.GetAt(ii);
		if ( name.CompareNoCase(fileName) == 0 )
		{
			return 1;
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////
/**
 * get name of item
 *
 * @param 
 * @return		name of item
 * @exception   
 * @see         
 */
CString CELibraryItem::GetName()
{
	return m_itemName;
}

/////////////////////////////////////////////////////////////////////
/**
 * get type of item
 *
 * @param 
 * @return		item type
 * @exception   
 * @see         
 */
CString CELibraryItem::GetType()
{
	CString value;
	GetProperty("type",value);
	return value;
}

/////////////////////////////////////////////////////////////////////
/**
 * search file name with specified extension
 *
 * @param fileType	file extension
 * @return		the file name, "" if not found
 * @exception   
 * @see         
 */
CString CELibraryItem::GetFileName(const CString& fileType)
{
	CString fileName,ext;
	for (int ii = 0; ii< m_fileNames.GetSize(); ii ++ )
	{
		fileName = m_fileNames.GetAt(ii);
		ext = CEFile::GetFileExt(fileName);
		if ( ext.CompareNoCase(fileType) == 0)
		{
			return fileName;
		}
	}
	return fileName;
}

/////////////////////////////////////////////////////////////////////
/**
 * get file name at specified position
 *
 * @param position	position in list 
 * @return		the file name
 * @exception   
 * @see         
 */
CString CELibraryItem::GetFileName(int position)
{
	ASSERT(position >=0);
	ASSERT(position < m_fileNames.GetSize());
	return m_fileNames.GetAt(position);
}

/////////////////////////////////////////////////////////////////////
/**
 * get number of files
 *
 * @param 
 * @return		number of files 
 * @exception   
 * @see         
 */
int CELibraryItem::GetFileCount()
{
	return m_fileNames.GetSize();
}

/////////////////////////////////////////////////////////////////////
/**
 * set property of item
 *
 * @param key	key of property
 * @param value value of property
 * @return		1 ok , 0 not ok
 * @exception   
 * @see         
 */
BOOL CELibraryItem::SetProperty(const CString& key, const CString& value)
{
	if ( key.CompareNoCase("file") == 0)
		return FALSE;
	CString tempKey=key;
	tempKey.MakeLower();
	m_properties.SetAt(tempKey,value);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
/**
 * get property of item
 *
 * @param key	key of property
 * @param value value of property
 * @return		1 property found , 0 not found
 * @exception   
 * @see         
 */
BOOL CELibraryItem::GetProperty(const CString& key, CString& value)
{
	CString tempKey=key;
	tempKey.MakeLower();
	BOOL rv = m_properties.Lookup(tempKey,value);
	if ( rv )
	{
		return rv;
	}
	else
	{
		value = "";
		return rv;
	}
}

/////////////////////////////////////////////////////////////////////
/**
 * delete property of item
 *
 * @param key	key of property
 * @return		1 property found , 0 not found
 * @exception   
 * @see         
 */
BOOL CELibraryItem::DeleteProperty(const CString& key)
{
	return m_properties.RemoveKey(key);
}

/////////////////////////////////////////////////////////////////////
/**
 * get property count
 *
 * @return		number of properties
 * @exception   
 * @see         
 */
int CELibraryItem::GetPropertyCount()
{
	return m_properties.GetCount();
}

/////////////////////////////////////////////////////////////////////
/**
 * get property at spec. position
 *
 * @param index 
 * @param key	key of property
 * @param value value of property
 * @return		1 index found , 0 not found
 * @exception   
 * @see         
 */
int CELibraryItem::GetProperty(int index, CString& key, CString& value)
{
	POSITION pos;
	CString tempKey, tempValue;
	int tempIndex = 0;
	for ( pos=m_properties.GetStartPosition(); pos!=NULL; )
	{
		m_properties.GetNextAssoc(pos, tempKey, tempValue);
		if ( tempIndex == index )
		{
			key = tempKey;
			value = tempValue;
			return 1;
		}
		tempIndex ++;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////
/**
 * write item to given stream
 *
 * @param pCeStream		the stream
 * @return		
 * @exception   
 * @see         
 */
/*void CELibraryItem::Write(CEStream* pCeStream)
{
	CString line;
	//
	// write name
	//
	line = '[' + m_itemName + ']';
	pCeStream->WriteString(line);
	//
	// write type
	//
	line = "type=" + m_itemType;
	pCeStream->WriteString(line);
	//
	// write files
	//
	for (int ii=0; ii<m_fileNames.GetSize();ii++)
	{
		line = "file=" + m_fileNames.GetAt(ii);
		pCeStream->WriteString(line);
	}
	//
	// write properties
	//
	CString key,value;
	POSITION position;
	for ( position = m_properties.GetStartPosition() ; position !=NULL; )
	{
		m_properties.GetNextAssoc(position, key, value);
		line = key + "=" + value;
		pCeStream->WriteString(line);
	}
}*/


/*
 *----------------------------------------------------------------------------*
 *  $History: CELibraryItem.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:33p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 *==
 *----------------------------------------------------------------------------*
*/

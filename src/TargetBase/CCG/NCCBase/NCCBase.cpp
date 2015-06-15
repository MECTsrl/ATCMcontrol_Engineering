/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/NCCBase/NCCBase.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: NCCBase.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/NCCBase/NCCBase.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			NCCBase
 *
 * =CURRENT 	 $Date: 28.02.07 19:04 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include <io.h>

#include "hash.h"
#include "parser.h"
#include "Codec.h"
#include "..\ncc\DisAsm.h"
#include "getopt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// Feste Filenamen

#define Hdr_Dateiname            "__common.hdr"     // header
#define Code_Dateiname           "__classes.bin"    // code 
#define IP_Dateiname             "__classes.ip"     // old code
#define Data_Dateiname           "__objects.init"
#define Hash_Dateiname           "__classid.txt"    // old: __objects.hash
#define List_Dateiname           "__objects.lst"
#define Ncc_Dateiname            "__classes.ncc"    // new (temp) code

/////////////////////////////////////////////////////////////////////////////

int NCCBase(int argc, TCHAR *argv[], TCHAR *envp[])
{
	unsigned long  ulIPsize;

    int             chOpt;
	CString			strDir,					// Directory
					strName,				// temp. Filename
					strLine;				// temp. String
	CString			Break_Dateiname;		// binary Files
    char*           pParam;                 // Pointer zu Parameter-Erweiterung
	CFileException	ex;


	C__objects_hash	Hash;					// object list
	CParser			Parser;
	XDownHeader		DownHeader;
	CStringArray	BreakLines;

	unsigned long  ulCodeSize;
	unsigned char* pBuffer	= NULL;

    // get command-line options and parameters
    do
    {
        chOpt = GetOption(argc, argv, validParameter, &pParam);
        if (chOpt > 1)
        {
            // chOpt is valid argument
            switch (chOpt)
            {
            case 'a':   // stack alignment
            case 'A':
                if (!pParam)
                {
		            fprintf(stderr, "Error: No Stack Alignment-Parameter\n");
		            return 1;
                }
		        Parser.m_StackAlignment = strtoul(pParam, NULL, 0);    
                break;
            case 'b':   // BaseAdresse
            case 'B':   
                if( !pParam ) 
	            {
		            fprintf(stderr, "Error: No BaseAddress-Parameter\n");
		            return 1;
	            }
		        Parser.m_NC_LoadAddress = strtoul(pParam, NULL, 0);    
                break;
            case 'c':
            case 'C':
                if( !pParam ) 
	            {
		            fprintf(stderr, "Error: No Max Calls Parameter\n");
		            return 1;
	            }
                Parser.m_max_calls = strtoul(pParam, NULL, 0);
                break;
            case 'd':   // Debugging ein
            case 'D':   
	            Parser.m_iDebugNames = 1;
                break;
            case 'e':   // instance data alignment
            case 'E':
                if (!pParam)
                {
		            fprintf(stderr, "Error: No Instance data Alignment-Parameter\n");
		            return 1;
                }
		        Parser.m_InstAlignment = strtoul(pParam, NULL, 0);    
                break;
            case 'f':   // Firmware version
            case 'F':   
                if (!pParam)
                {
		            fprintf(stderr, "Error: No Firmware Version Parameter Specified\n");
		            return 1;
                }
	            Parser.m_FirmwareVersion = strtoul(pParam, NULL, 0);
                break;
            case 'g':   // 16 bit processor 
            case 'G':   
	            Parser.m_b16BitProc = TRUE;
                break;
            case 'l':   // Listing   ein
            case 'L':  
                if( pParam && tolower(*pParam) == 'i' )
                    Parser.m_iList   |= option_IpList;
                else if( pParam && tolower(*pParam) == 'n' )
                    Parser.m_iList   |= option_NccList;
                else 
                {
		            fprintf(stderr, "Error: List-Parameter\n");
		            return 1;
	            }
                break;
            case 'm':
            case 'M':
                if( !pParam ) 
	            {
		            fprintf(stderr, "Error: No Max Index Entries Parameter\n");
		            return 1;
	            }
                Parser.m_max_index_entries = strtoul(pParam, NULL, 0);
                break;
            case 'n':   // Namenumschaltung   ein
            case 'N':   // nologo
                if(pParam && strcmp(pParam, "ologo") == 0)
                    Parser.m_iList   |= option_nologo;
                else
                    Parser.m_iList   |= option_Names;
                break;
            case 'o':
            case 'O':
                if( !pParam ) 
	            {
		            fprintf(stderr, "Error: No Max Object Entries Parameter\n");
		            return 1;
	            }
                Parser.m_max_object_entries = strtoul(pParam, NULL, 0);
                break;
            case 'p':   // Praefix 64 ein
            case 'P':   
                Parser.m_iList   |= option_Praef64;
                break;
            case 'r':   // ReAssembling ein
            case 'R':   
                Parser.m_iList   |= option_ReAsm;
                break;
            case 's':   // Code Segment Size
            case 'S':   
                if (!pParam)
                {
		            fprintf(stderr, "Error: No Code Segment Size Specified\n");
		            return 1;
                }
	            Parser.m_CodeSegSize = strtoul(pParam, NULL, 0);
                break;
            case 't':   // Code Segment Size
            case 'T':   
	            Parser.m_iTrashBreakpoints = 1;
                break;
            case 'w':   // Write IO flags ein
            case 'W':   
                Parser.m_iList   |= option_WriteIOFlags;
                break;
            case 'i':   // interner Test ein
            case 'I':   
                Parser.m_iList   |= option_Intern;
                break;
            case 'h':   // usage info
            case 'H':   // usage info
                help();
                return(0);
                break;
            case 'z':   // usage info
            case 'Z':   // usage info
                Parser.m_iList   |= option_CheckFreeze;
                break;
            }
        }

        if (chOpt == 1)
        {
	        // Verzeichnis lesen
	        strDir = pParam;
	        if( strDir.Right(1) != '\\' ) strDir += CString('\\');
        }

        if ( chOpt == -1 )
        {
            // error
            fprintf(stderr, "Error: Argument '%s' not recognized\n", pParam);
            return 1;
        }
    } while( chOpt > 0);

    // Tests nach Optionerfassung
	if( Parser.m_NC_LoadAddress && ((Parser.m_NC_LoadAddress & 0xFFFF) > 0xFF00) && !Parser.m_iDebugNames) {
		fprintf(stderr, "Error: LoadAddress too big\n");
		return 1;
	}

	if ((Parser.m_StackAlignment & Parser.m_StackAlignment-1) || Parser.m_StackAlignment < 1 || Parser.m_StackAlignment > 8) {
		fprintf(stderr, "Error: stack alignment is not power of two or it is too big\n");
		return 1;
	}

    if ((Parser.m_InstAlignment & Parser.m_InstAlignment-1) || Parser.m_InstAlignment < 1 || Parser.m_InstAlignment > 8) {
		fprintf(stderr, "Error: stack alignment is not power of two or it is too big\n");
		return 1;
	}

	
    if (Parser.m_FirmwareVersion == 0) {
		fprintf(stderr, "Error: Firmware version not specified\n");
		return 1;
	}

	if( strDir.IsEmpty() ) 
	{
		fprintf(stderr, "Error: No Directory-Parameter\n");
		return 1;
	}

    if( !(Parser.m_iList & option_nologo) )
        logo();

    if( (Parser.m_iList & option_Intern) ) {
        try {
		    // open files
		    CFile CodeFile(strDir + Code_Dateiname, CFile::modeRead|CFile::typeBinary);
		    CStdioFile ListFile(strDir + List_Dateiname, CFile::modeWrite|CFile::modeCreate);

            CDisAsm::test(CodeFile, ListFile, Parser.m_NC_LoadAddress);

		    // close files
		    CodeFile.Close();
		    ListFile.Close();
        }
	    catch( CFileException *e ) {
		    fprintf(stderr, "Fatal Error: File %s %d\n", e->m_strFileName, e->m_cause);
		    return 1;
	    } 
        return 0;
    }

	try {
		// open files
		CFile CodeFile(strDir + Code_Dateiname, CFile::modeRead|CFile::typeBinary);
		
		CFile NccFile(strDir + Ncc_Dateiname, CFile::modeWrite|CFile::modeCreate);
		
		CStdioFile ListFile(strDir + List_Dateiname, CFile::modeWrite|CFile::modeCreate);
		
		// collect all CodeBlockIndexes 
		Hash.collect(strDir + Hash_Dateiname);
		
		ulCodeSize = 0;
		
		// for all Blocks
		while( CodeFile.Read(&DownHeader, sizeof(DownHeader)) > 0 ) 
      {
         if( big_endian == 1 ) 
         {
            DownHeader.uIndex   = ((DownHeader.uIndex   & 0x00FF) << 8) | ((DownHeader.uIndex   & 0xFF00) >> 8);
            DownHeader.uSegment = ((DownHeader.uSegment & 0x00FF) << 8) | ((DownHeader.uSegment & 0xFF00) >> 8);
            DownHeader.ulSize   = 
               ((DownHeader.ulSize & 0x000000FF) << 24) | 
               ((DownHeader.ulSize & 0x0000FF00) <<  8) |
               ((DownHeader.ulSize & 0x00FF0000) >>  8) |
               ((DownHeader.ulSize & 0xFF000000) >> 24);
            DownHeader.ulOffset   = 
               ((DownHeader.ulOffset & 0x000000FF) << 24) | 
               ((DownHeader.ulOffset & 0x0000FF00) <<  8) |
               ((DownHeader.ulOffset & 0x00FF0000) >>  8) |
               ((DownHeader.ulOffset & 0xFF000000) >> 24);
         }
			strName	= Hash.isCodeBlock(DownHeader.uIndex);
			pBuffer	= new unsigned char[DownHeader.ulSize];
			CodeFile.Read(pBuffer, DownHeader.ulSize);
			// Code ?
			if( strName.IsEmpty() ) 
         {
            XDownHeader DownHeaderB;
            if( big_endian == 1 ) 
            {
               DownHeaderB.uIndex   = ((DownHeader.uIndex   & 0x00FF) << 8) | ((DownHeader.uIndex   & 0xFF00) >> 8);
               DownHeaderB.uSegment = ((DownHeader.uSegment & 0x00FF) << 8) | ((DownHeader.uSegment & 0xFF00) >> 8);
               DownHeaderB.ulSize   = 
                  ((DownHeader.ulSize & 0x000000FF) << 24) | 
                  ((DownHeader.ulSize & 0x0000FF00) <<  8) |
                  ((DownHeader.ulSize & 0x00FF0000) >>  8) |
                  ((DownHeader.ulSize & 0xFF000000) >> 24);
               DownHeaderB.ulOffset   = 
                  ((DownHeader.ulOffset & 0x000000FF) << 24) | 
                  ((DownHeader.ulOffset & 0x0000FF00) <<  8) |
                  ((DownHeader.ulOffset & 0x00FF0000) >>  8) |
                  ((DownHeader.ulOffset & 0xFF000000) >> 24);
            } else {
               DownHeaderB = DownHeader;
            }
				// no -> only Copy
				NccFile.Write(&DownHeaderB, sizeof(DownHeaderB));
				NccFile.Write(pBuffer, DownHeader.ulSize);
			} else {
				if( Parser.m_iList & option_Names ) {
					CString str;
					str.Format(".code_modul %s:\n", strName.Left(strName.ReverseFind('.')));
					ListFile.WriteString( str );
				}
				Cxxx_dbp	xxx_dbp(strDir + strName, Parser.m_iTrashBreakpoints);
				xxx_dbp.read();
			
				ulIPsize = DownHeader.ulSize;  // save IP length
				Parser.pars(NccFile, ListFile, DownHeader, pBuffer, xxx_dbp);
				// new code length in DownHeader.ulSize !!
				Parser.m_IP_LoadAddress += ulIPsize;
				Parser.m_NC_LoadAddress += DownHeader.ulSize;
				ulCodeSize     += DownHeader.ulSize;
				
				xxx_dbp.write(Parser.m_iDebugNames);
			}
			delete[] pBuffer;
            pBuffer = NULL;
		}
		//EF:  "update header file" removed, we don't need todo so any longer. 
        //The only reason to open the header file could be to get the code version...

		// close files
		CodeFile.Close();
		NccFile.Close();
		ListFile.Close();

        if (Parser.m_CodeSegSize && Parser.m_CodeSegSize < ulCodeSize)
        {
            CString str;
            str.Format("Code Segment size (%dKB) limit reached. Actual Code size is %dKB", Parser.m_CodeSegSize>>10, ulCodeSize>>10);
            ASSERT(_ASSERT_); 
            throw CFormatException(str);        
        }

        if( !Parser.m_iDebugNames ) {
			// rename  files
			if (_access( strDir + IP_Dateiname, 0) == 0)
			{
				_unlink(strDir + IP_Dateiname );
			}
			rename( strDir + Code_Dateiname, strDir + IP_Dateiname);
			rename( strDir + Ncc_Dateiname,  strDir + Code_Dateiname);
		}
	} 
	catch( CFileException *e ) {
		fprintf(stderr, "Fatal Error: File %s %d\n", e->m_strFileName, e->m_cause);
        if( pBuffer ) delete[] pBuffer;
		return 1;
	} 
	catch(CMemoryException) {
		fprintf(stderr, "Fatal Error: Memory\n");
        if( pBuffer ) delete[] pBuffer;
		return 1;
	}
	catch(CFormatException e) {
		CString m_strError;
		e.GetErrorMessage(m_strError.GetBuffer(130), 130);
		fprintf(stderr, "Error: %s\n", m_strError);
        if( pBuffer ) delete[] pBuffer;
		return 1;
	}
	
	return 0;
}

/* ---------------------------------------------------------------------------- */

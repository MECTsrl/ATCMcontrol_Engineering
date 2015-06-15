/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_ZIP.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_ZIP.cpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_ZIP.cpp $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CBar
 *
 * =COMPONENT           bar
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *           $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  02.07.01  KT      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/

#include "bzlib.h"

#include "BAR_ZIP.hpp"
#include "BAR_Callback.hpp"

//====  Local Defines:   ======================================================*
//#define BAR_ZIP_BUFSIZE 0x1000
const int BAR_ZIP::m_BUFSIZE = 0x1000;

//====  Static Initializations:   =============================================*


//=============================================================================*
// MEMBER FUNCTIONS OF CLASS BAR_ZIP
//=============================================================================*

//=============================================================================*
string BAR_ZIP::compressFile(BAR_Callback& callback,
                             const string& sFileIn,
                             const string& sFileOut)
//=============================================================================*
{
    string err = "";

    //=== open input file for reading (in binary mode)
    FILE* fp_r = fopen(sFileIn.c_str(), "rb");
    if( fp_r == NULL )
    {
        return err += "could not open file for reading, file=" + sFileIn;
    }
    
    //=== open output file for compressed writing
    char mode[4] = "wb";
    BZFILE* BZ2fp_w = bzopen(sFileOut.c_str(), mode);
    if ( BZ2fp_w == NULL )
    {
        fclose(fp_r);
        return err += "could not open file for compressed writing, file="
                    + sFileOut;
    }

    //=== do compression
    char buf[m_BUFSIZE];
    int len;
    while( err.empty() &&
           0 < ( len = fread(buf, 1, m_BUFSIZE, fp_r) )
         )
    {
        int nWritten = bzwrite(BZ2fp_w, buf, len);
        if ( nWritten != len )
        {
            err += "i/o error occured while writing to compressed file, file="
                 + sFileOut;
        }
        else if ( callback.IsCancelRequested() )
        {
            err += "cancelled by user";
        }
    }

    //=== close input file
    fclose(fp_r);

    //=== close output file
    bzclose(BZ2fp_w);

    //== finished
    return err;
}

//=============================================================================*
string BAR_ZIP::uncompressFile(BAR_Callback& callback,
                               const string& sFileIn,
                               const string& sFileOut)
//=============================================================================*
{
    string err = "";

    //=== open input file for compressed reading
    BZFILE* BZ2fp_r = bzopen(sFileIn.c_str(), "rb");
    if( BZ2fp_r == NULL )
    {
        return err += "could not open file for compressed reading, file="
                    + sFileIn;
    }
    
    //=== open output file for writing
    FILE* fp_w = fopen(sFileOut.c_str() , "wb");
    if ( fp_w == NULL )
    {
        bzclose(BZ2fp_r);
        return err += "could not open file for writing, file=" + sFileOut;
    }

    //=== do uncompression
    char buf[m_BUFSIZE];
    int len;
    while( err.empty() &&
           0 < ( len = bzread(BZ2fp_r, buf, m_BUFSIZE) )
         )
    {
        int nWritten = fwrite(buf, 1, len, fp_w);
        if ( nWritten != len )
        {
            err += "i/o error occured while writing to file, file="
                 + sFileOut;
        }
        else if ( callback.IsCancelRequested() )
        {
            err += "cancelled by user";
        }
    }

    //=== close input file
    bzclose(BZ2fp_r);

    //=== close output file
    fclose(fp_w);

    //== finished
    return err;
}

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_ZIP.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 1  *****************
 * User: Kt           Date: 10.07.01   Time: 16:41
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_ZIP.cpp $

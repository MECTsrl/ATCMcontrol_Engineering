package COM.softing.fclib.flib;

/**
 * class FLibMode
 */
public class FLibMode
{
    // FLIB constants
    public final static byte CFA_CARRIAGERETURN             = 13;   // ASCII 'carriage return'
    public final static byte CFA_NEWLINE                    = 10;   // ASCII 'new line'

    // file access
    public final static int CFA_READ                        = 2;    // read; open sets position to BOF
    public final static int CFA_READ_WRITE                  = 6;    // read and write; open sets position to BOF
    public final static int CFA_APPEND                      = 7;    // read and write; open sets position to EOF
        	
    // file position
    public final static int CFA_BOF                         = 0;    // begin of file
    public final static int CFA_EOF                         = -1;   // end of file
    public final static int CFA_EOL                         = -2;   // end of line
        	
    // operations
    public final static String CFA_ASYNC_CLOSEFILE          = "FA_ASYNC_CLOSEFILE";
    public final static String CFA_ASYNC_COPYFILE           = "FA_ASYNC_COPYFILE";
    public final static String CFA_ASYNC_CREATEDIRECTORY    = "FA_ASYNC_CREATEDIRECTORY";
    public final static String CFA_ASYNC_DELETEFILE         = "FA_ASYNC_DELETEFILE";
    public final static String CFA_ASYNC_EXISTSFILE         = "FA_ASYNC_EXISTSFILE";
    public final static String CFA_ASYNC_GETSIZE            = "FA_ASYNC_GETSIZE";
    public final static String CFA_ASYNC_OPENFILE           = "FA_ASYNC_OPENFILE";
    public final static String CFA_ASYNC_READFILE           = "FA_ASYNC_READFILE";
    public final static String CFA_ASYNC_WRITEFILE          = "FA_ASYNC_WRITEFILE";
    public final static String CFA_ASYNC_RENAMEFILE         = "FA_ASYNC_RENAMEFILE";
    public final static String CFA_SYNC_CLOSEFILE           = "FA_SYNC_CLOSEFILE";
    public final static String CFA_SYNC_CREATEDIRECTORY     = "FA_SYNC_CREATEDIRECTORY";
    public final static String CFA_SYNC_DELETEFILE          = "FA_SYNC_DELETEFILE";
    public final static String CFA_SYNC_EXISTSFILE          = "FA_SYNC_EXISTSFILE";
    public final static String CFA_SYNC_GETSIZE             = "FA_SYNC_GETSIZE";
    public final static String CFA_SYNC_OPENFILE            = "FA_SYNC_OPENFILE";
    public final static String CFA_SYNC_READFILE            = "FA_SYNC_READFILE";
    public final static String CFA_SYNC_WRITEFILE           = "FA_SYNC_WRITEFILE";
    public final static String CFA_SYNC_RENAMEFILE          = "FA_SYNC_RENAMEFILE";
}

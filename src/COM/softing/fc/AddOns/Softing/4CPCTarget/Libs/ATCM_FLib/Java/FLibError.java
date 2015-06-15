package COM.softing.fclib.flib;

import COM.softing.fc.CC.util.*;
import java.io.*;

/**
 * class FLibError
 */
public class FLibError
{
	// FLIB error codes
	final static int CFA_CommonError                = 10;
	final static String CFA_MkdirOpFailedStr        = "Createdirectory operation failed";
	final static String CFA_CopyOpFailedStr         = "Copy file operation failed";
	final static String CFA_DeleteOpFailedStr       = "Delete file operation failed";
	final static String CFA_RenameOpFailedStr       = "Rename file operation failed";
	final static String CFA_FileNotPresentStr       = "The specified file does not exist";
	final static String CFA_FileAlreadyExistsStr    = "The specified file alread exists";
	final static String CFA_DirectoryIsNotEmpty     = "Cannot delete directory. Directory is not empty";
	final static String CFA_FileIsWriteprotected    = "File is write protected";
	
	final static int CFA_InternalError              = 11;
	final static String CFA_InternalErrorStr        = "Internal Error";
	
	final static int CFA_InvalidArgument            = 20;
	final static String CFA_InvalidArgumentStr      = "Invalid Argument";
	
	final static int CFA_FileNotOpenError           = 30;
	final static String CFA_FileNotOpenErrorStr     = "The specified file is not open";
	
	final static int CFA_Timeout                    = 40;
	final static String CFA_TimeoutStr              = "Timeout";
	
	final static int CFA_FileAlreadyOpen            = 50;
	final static String CFA_FileAlreadyOpenStr      = "Resource occupied. This instance already opened a file";
}

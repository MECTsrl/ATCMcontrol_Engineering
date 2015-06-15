package COM.softing.fclib.flib;

import COM.softing.fc.CC.util.*;
import java.io.*;
import COM.softing.fc.CC.utility.ByteString;

/**
 * interface IFLib
 */
public interface IFLib
{
	public RandomAccessFile getFile();
	public void getResult(FcBool error, FcInt errcd, FcString errtxt, FcInt position);
	public void setResult(boolean bError, int iErrcd, String strErrtxt);
}

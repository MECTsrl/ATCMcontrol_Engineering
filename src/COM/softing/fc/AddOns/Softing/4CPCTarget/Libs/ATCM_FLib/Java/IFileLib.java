package COM.softing.fclib.flib;

import COM.softing.fc.CC.util.*;
import com.ms.wfc.io.*;
import COM.softing.fc.CC.utility.ByteString;

public interface IFileLib
{
	public File getFile();
	public void getResult(FcBool error, FcInt errcd, FcString errtxt, FcInt position);
}

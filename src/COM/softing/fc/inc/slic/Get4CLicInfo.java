/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/slic/Get4CLicInfo.java 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: Get4CLicInfo.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/slic/Get4CLicInfo.java $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                LIC
 *
 * =COMPONENT           fc_slbase
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==  Contains a java class Get4CLicInfo as an example how to use fc_slbase20.dll 
 *    from java with JDirect. 
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  05.12.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

import com.ms.dll.*;


// Test code: 
// put next line in file named getLic.java and type:
// jvc   getLic.java
// jview getLic
/*
public class getLic
{
  public static void main(String args[])
  {
    Get4CLicInfo.refreshLicInfo();
    System.out.println("instKey    ="+Get4CLicInfo.instKey);
    System.out.println("nLastErr   ="+Get4CLicInfo.nLastErr);
    System.out.println("nIOs       ="+Get4CLicInfo.nIOs);
    System.out.println("nHoursLeft ="+Get4CLicInfo.nHoursLeft);
    System.out.println("bIsDongle  ="+Get4CLicInfo.bIsDongle);
  }
}
*/


class Get4CLicInfo
{
    public static int     nLastErr;   //one of SLIC_*  or SLIC_OK if all ok
    public static String  instKey;    //the installation key
    public static int     nIOs;       //>=0: number of IOs. 0 means Demo Mode,
                                    //which is also in effect if any lic errors
    public static int     nHoursLeft; //-1 if lic is a final license or dongle
                                    //>=0 the number of hour that remain
    public static boolean bIsDongle;  //true if lic media is a dongle


    //read the lic info of 4C PC_based Control and updates the static members:
    //return false if any error or no lic or lic expired
    //The Hadrlock is only testet in the very first call.
    public static boolean refreshLicInfo()
    {
        int          dw[] = {0};
        int          nh[] = {0};
        int          li[] = {SLIC_MEDIA_ANY};
        int          MAXKEY = 260;
        StringBuffer strb = new StringBuffer(MAXKEY);

        //reset static menbers:
        nIOs       = 0;
        nHoursLeft = 0;
        bIsDongle  = false;
        instKey    = "";

        //get install key first:    
        nLastErr = SLIC_GetInstallationKey(strb, MAXKEY);
        if(nLastErr != SLIC_OK)
            return false;

        instKey = strb.toString();
        if(instKey == "")
        {
            nLastErr = SLIC_ERR_INTERAL;
            return false;
        }

 
        //get product lic: 4 is the Product 4C-PC-Control   
        nLastErr = SLIC_GetLicenseForProduct(4, dw, nh, li);

        if(nLastErr==SLIC_OK)
        {
            bIsDongle = (li[0] == SLIC_MEDIA_DONGLE);
            nHoursLeft = nh[0];

            switch(dw[0] & 0xf)//low nipple contains the IO num info
            {
            case 0x1: nIOs = 40;   break;
            case 0x2: nIOs = 100;  break;
            case 0x3: nIOs = 200;  break;
            case 0x4: nIOs = 500;  break;
            case 0x5: nIOs = 1000; break;
            case 0x6: nIOs = 2000; break;
            case 0x7: nIOs = 3000; break;
            case 0x8: nIOs = 5000; break;
            default: 
                nLastErr = SLIC_ERR_INTERAL;
            }
        }
        return nLastErr==SLIC_OK;
    }

    //error values (not all only a supset):
    private static final int SLIC_OK    = 0;
    private static final int SLIC_ERR_INTERAL        = 1;
    private static final int SLIC_ERR_NO_COMP_FOUND  = 20;
    private static final int SLIC_ERR_NO_LIC_FOUND   = 21;
    private static final int SLIC_ERR_TMPLIC_EXPIRED = 22;
    //lic media types:
    private static final int SLIC_MEDIA_ANY    = 0;
    private static final int SLIC_MEDIA_SOFT   = 1;
    private static final int SLIC_MEDIA_DONGLE = 2;


   /**
    * Only the first call to this function for a DLL instance calls 
    * SLIC_RefreshHardLock, this may take approx. max. 1 second. 
    * Subsequent calls do not reload the hardlock data, use 
    * SLIC_RefreshHardLock todo so.
    *
    * @param   prodNum        the product number to look up the lic info
    * @param   pdwFeatureBits receives the feature bits if lic found
    * @param   pnHoursLeft    -1 if lic is a final license or dongle
    *                         >=0 if time limited, then *pnHoursLeft
    *                         is the number of hours left.
    *                         Warning will be 0 if less than 1 hour left
    *                         therefore don't use *pnHoursLeft==0 to 
    *                         test for "expired" instead use the return code.
    *
    * @param   pLicMedia      in/out spcified the lic media to be used (input)
    *                         and found (output). If SLIC_MEDIA_ANY is used on input
    *                         first the dongl is testet and only if no dongle lic
    *                         the soft lic is testet.
    *
    *                         SLIC_MEDIA_ANY = 0
    *                         SLIC_MEDIA_SOFT = 1
    *                         SLIC_MEDIA_DONGLE= 2
    *
    * @return  SLIC_OK = 0
    *          or almost all other SLIC_ERR_* values
    *          typical values could be 
    *          SLIC_ERR_NO_LIC_FOUND
    *          SLIC_ERR_TMPLIC_EXPIRED
    */
    /** @dll.import("fc_slbase20.dll") */
    private static native int SLIC_GetLicenseForProduct(
        int prodNum, 
        int pdwFeatureBits[], 
        int pnHoursLeft[],
        int pLicMedia[]
    );

   /**
    * Get the installation key.
    *
    * @param   pszInstKey  receives the installation key.
    * @param   nBuffer     size of pszInstKey, must be >SLIC_MAX_IKEY_CHARS+1
    *
    * @return  SLIC_OK 
    *          or SLIC_ERR_NO_COMP_FOUND  if no compny name givven yet
    *          or SLIC_ERR_ILLPARAM       
    */  
    /** @dll.import("fc_slbase20.dll") */
    private static native int SLIC_GetInstallationKey(
        StringBuffer  pszInstKey,
        int          nBuffer
    );

}




/*
 *----------------------------------------------------------------------------*
 *  $History: Get4CLicInfo.java $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc/slic
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc/slic
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 5.12.01    Time: 18:34
 * Updated in $/4Control/COM/softing/fc/inc/slic
 * added test code
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 5.12.01    Time: 18:30
 * Created in $/4Control/COM/softing/fc/inc/slic
 * new file
 *==
 *----------------------------------------------------------------------------*
*/
